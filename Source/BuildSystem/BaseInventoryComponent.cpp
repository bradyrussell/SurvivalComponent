// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInventoryComponent.h"
#include "UnrealNetwork.h"
#include "Engine/Engine.h"
#include "InventoryGameInstance.h"

// Sets default values for this component's properties
UBaseInventoryComponent::UBaseInventoryComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bReplicates = true;
	// ...
}


// Called when the game starts
void UBaseInventoryComponent::BeginPlay() {
	Super::BeginPlay();
	InventorySlots.AddDefaulted(NumberSlots);
	// ...

}

FItemStack UBaseInventoryComponent::AddItem(FItemStack NewItem) {

	//do we already have any of this item?
	for (auto& elem : InventorySlots) {
		if (elem.Item == NewItem.Item && !elem.isEmpty()) {
			// how much can we put in this stack
			if (elem.Amount < DBG_MAX_STACK) {
				// how much more can we put in this stack
				const int AmountTaken = FMath::Min(NewItem.Amount, DBG_MAX_STACK - elem.Amount); // either all of the new items OR as much as we can 

				NewItem.Amount -= AmountTaken;
				elem.Amount += AmountTaken; // perform swap

			}
		}
	}

	// do we have leftover items?
	while (NewItem.Amount > 0) {
		// can we make a new stack?
		int emptySlot = GetFirstEmptySlot();

		if (InventorySlots.IsValidIndex(emptySlot)) {
			int newStackAmount = FMath::Min(NewItem.Amount, DBG_MAX_STACK);
			NewItem.Amount -= newStackAmount;
			InventorySlots[emptySlot] = FItemStack(NewItem.Item, newStackAmount);
		}
		else { return NewItem; }
	}
	return FItemStack(0, 0);
}

TArray<FItemStack> UBaseInventoryComponent::AddItems(TArray<FItemStack> NewItems) {
	TArray<FItemStack> excess;

	for (auto& elem : NewItems) {
		auto excessStack = AddItem(elem);
		if (!excessStack.isEmpty())
			excess.Add(excessStack);
	}
	return excess;
}

FItemStack UBaseInventoryComponent::RemoveItem(FItemStack Item) {
	for (auto& elem : InventorySlots) {
		if (Item.Item == elem.Item) {
			const int32 AmountRemoved = FMath::Min(elem.Amount, Item.Amount); // the lesser amount
			elem.Amount -= AmountRemoved;
			Item.Amount -= AmountRemoved;

			if (Item.Amount <= 0)
				return Item;
		}
	}

	return Item;
}

TArray<FItemStack> UBaseInventoryComponent::RemoveItems(TArray<FItemStack> Items) {
	TArray<FItemStack> excess;

	for (auto& elem : Items) {
		auto excessStack = RemoveItem(elem);
		if (!excessStack.isEmpty())
			excess.Add(excessStack);
	}
	return excess;
}

bool UBaseInventoryComponent::TransferToInventory(UBaseInventoryComponent* Recipient, int32 Slot) {
	auto ItemInSlot = InventorySlots[Slot];
	if(ItemInSlot.isEmpty()) return false;

	InventorySlots[Slot] = Recipient->AddItem(ItemInSlot); // move into recipient, leaving overflow
	return true;
}

bool UBaseInventoryComponent::TransferAllToInventory(UBaseInventoryComponent* Recipient) {
	if(GetNumberFilledSlots() == 0) return false;
	InventorySlots = Recipient->AddItems(InventorySlots);
	return true;
}

FItemStack UBaseInventoryComponent::ExchangeItem(int32 Slot, FItemStack NewItem) {
	const auto Old = InventorySlots[Slot];
	InventorySlots[Slot] = NewItem;
	return Old;
}

bool UBaseInventoryComponent::hasItem(FItemStack Item) {
	for (auto& elem : InventorySlots) {
		if (Item.Item == elem.Item) { Item.Amount -= elem.Amount; }

		if (Item.Amount <= 0)
			return true; // stop early
	}
	return false;
}

bool UBaseInventoryComponent::hasItems(TArray<FItemStack> Items) {
	for (auto& elem : Items) {
		if (!hasItem(elem))
			return false;
	}
	return true;
}

TArray<FItemStack> UBaseInventoryComponent::GetInventorySnapshot() const { return InventorySlots; }

void UBaseInventoryComponent::GetInventoryReference(TArray<FItemStack>& InventoryRef) const { InventoryRef = InventorySlots; }


void UBaseInventoryComponent::ResizeInventory(int32 NewNumberSlots) {
	const auto SlotsCopy = InventorySlots;
	InventorySlots.Reset(NewNumberSlots);
	InventorySlots.AddDefaulted(NewNumberSlots);
	NumberSlots = NewNumberSlots;
	InventorySlots.Append(SlotsCopy);
}

void UBaseInventoryComponent::ClearInventory() { InventorySlots.Reset(); }

int32 UBaseInventoryComponent::GetFirstEmptySlot() {
	for (int i = 0; i < InventorySlots.Num(); i++) {
		if (InventorySlots[i].isEmpty())
			return i;
	}
	return -1;
}

int32 UBaseInventoryComponent::GetNumberEmptySlots() {
	int32 empty = 0;
	for (auto& elem : InventorySlots) {
		if (elem.isEmpty())
			empty++;
	}
	return empty;
}

int32 UBaseInventoryComponent::GetNumberFilledSlots() { return NumberSlots - GetNumberEmptySlots(); }

void UBaseInventoryComponent::SwapSlots(int32 FirstSlot, int32 SecondSlot) { InventorySlots.Swap(FirstSlot, SecondSlot); }

bool UBaseInventoryComponent::areAllEmpty(TArray<FItemStack> Items) {
	for (auto& elem : Items) {
		if (!elem.isEmpty())
			return false;
	}
	return true;
}

bool UBaseInventoryComponent::isEmpty(FItemStack Item) { return Item.isEmpty(); }

FString UBaseInventoryComponent::ToString(FItemStack Item) {
	if(Item.isEmpty()) return FString(TEXT("Empty"));
	return *FString::Printf(TEXT("%i: %i"), Item.Item, Item.Amount);
}

FString UBaseInventoryComponent::ToStrings(TArray<FItemStack> Items) {
	FString out(""); 
	for(auto &elem:Items) {
		out.Append(ToString(elem)+LINE_TERMINATOR);
	}
	return out;
}

int32 UBaseInventoryComponent::GetMaxStackForItem(FName item) const { // todo i dont like that this is a property of the inventory
	FString context = FString();
	auto world = GetWorld();
	check(world);
	auto instance = Cast<UInventoryGameInstance>(world->GetGameInstance());
	check(instance);
	const auto ItemDB = instance->ItemDefinitions;
	check(ItemDB);
	return ItemDB->FindRow<FItemDefinition>(item, context)->MaxStack;
}


void UBaseInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	DOREPLIFETIME(UBaseInventoryComponent, NumberSlots);
	DOREPLIFETIME(UBaseInventoryComponent, InventorySlots);
}
