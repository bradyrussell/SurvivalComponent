// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityInventoryComponent.h"
#include "DatabaseProvider.h"
#include "ConsumableItemEffectBase.h"

bool UEntityInventoryComponent::ConsumeItem(int32 Slot, APawn* Cause, AController* Instigator, float MagnitudeMultiplier) {
	const FItemStack Item = InventorySlots[Slot];
	if(UBaseInventoryComponent::isEmpty(Item)) return false;
	
	const auto ItemDef = UDatabaseProvider::GetItemDefinition(this, Item.Item);

	if(ItemDef.bIsConsumable) {
		auto EffectObject = NewObject<UConsumableItemEffectBase>(this, ItemDef.ItemEffect);
		const auto FinalMagnitude = ItemDef.ItemEffectMagnitude * MagnitudeMultiplier;
		
		const auto bConsumeItem = EffectObject->OnItemConsumed(Cause, Instigator, FinalMagnitude);
		
		UE_LOG(LogTemp, Warning, TEXT("EXECUTING -> Consumable Item Effect (%s) with magnitude %f: Consumed %i"), *EffectObject->GetName(), FinalMagnitude, bConsumeItem);
		
		EffectObject->ConditionalBeginDestroy(); // we immediately destroy the effect object
		EffectObject = nullptr;

		if(bConsumeItem) {
			InventorySlots[Slot].Amount--;
		}
		
		return bConsumeItem;
	}
	
	return false;
}
