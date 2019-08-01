// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryStructs.h"
#include "BaseInventoryComponent.generated.h"

#define ITEM_NONE FItemStack(0,0);
#define DBG_MAX_STACK 256

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDSYSTEM_API UBaseInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseInventoryComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated) int32 NumberSlots;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated) TArray<FItemStack> InventorySlots;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//adds an item stack to the inventory, returning any excess items
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly) FItemStack AddItem(FItemStack NewItem);

	//adds each item stack to the inventory, returning any excess items
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly) TArray<FItemStack> AddItems(TArray<FItemStack> NewItems);

	// removes the amount of the item from the inv, returning the items it could not remove
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly) FItemStack RemoveItem(FItemStack Item);

	// removes the amounts of the items from the inv, returning the items it could not remove
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly) TArray<FItemStack> RemoveItems(TArray<FItemStack> Items);

	// returns whether the inventory contains at least the given Amount of Item
	UFUNCTION(BlueprintCallable) bool hasItem(FItemStack Item);

	//returns whether the inventory contains at least the given Amounts of Items
	UFUNCTION(BlueprintCallable) bool hasItems(TArray<FItemStack> Items);

	//returns a copy of the current state of the inventory
	UFUNCTION(BlueprintCallable) TArray<FItemStack> GetInventorySnapshot() const;

	// returns a reference to the inventory array. do not modify it
	UFUNCTION(BlueprintCallable) TArray<FItemStack>& GetInventoryReference();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly) void ResizeInventory(int32 NewNumberSlots);
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly) void ClearInventory();

	UFUNCTION(BlueprintCallable) int32 GetFirstEmptySlot();
	UFUNCTION(BlueprintCallable) int32 GetNumberEmptySlots();
	UFUNCTION(BlueprintCallable) int32 GetNumberFilledSlots();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly) void SwapSlots(int32 FirstSlot, int32 SecondSlot);

	// returns true if all items are empty
	UFUNCTION(BlueprintCallable) static bool areAllEmpty(TArray<FItemStack> Items);

public:	

};
