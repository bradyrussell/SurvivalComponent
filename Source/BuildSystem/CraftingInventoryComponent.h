// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContainerInventoryComponent.h"
#include "Queue.h"
#include "Engine/DataTable.h"
#include "CraftingInventoryComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent) )
	class BUILDSYSTEM_API UCraftingInventoryComponent : public UContainerInventoryComponent {
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated, Category="Inventory - Crafting") ECraftingType Type;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Inventory - Crafting") UDataTable* RecipeDataTable;

	TQueue<FProcessingRecipe> ProcessingQueue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category="Inventory - Crafting") float Fuel = 0.f;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Replicated, Category="Inventory - Crafting") bool bIsCurrentlyProcessing = false;

	UPROPERTY(BlueprintReadWrite, Category="Inventory - Crafting") FTimerHandle CurrentJob_Timer;
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Replicated, Category="Inventory - Crafting") FProcessingRecipe CurrentJob_Recipe;

	//  replicate the expected finish time of the recipe for client side visualization
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated) FDateTime Client_ExpectedFinish;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory - Crafting") bool QueueRecipe(FProcessingRecipe InRecipe);

	// will begin working through the queued recipes
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory - Crafting") bool BeginProcessing();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory - Crafting") FProcessingRecipe PeekNextRecipe() const;

private:
	//called when a recipe finishes. 
	void EndProcessing();

	// // keeps the client timer in sync with the server timer
	// UFUNCTION(Client, Reliable) void Client_SetTimer(float duration);
	// void Client_TimerFinished(); // todo find out how to do a null timer
public:
	//interrupt crafting, does not clear queue
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory - Crafting") void CancelProcessing();

	// clear queue. does not cancel in progress recipe
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory - Crafting") void ClearQueue();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory - Crafting") void CancelAndClearQueue();

	UFUNCTION(BlueprintCallable, Category="Inventory - Crafting") bool isCurrentlyProcessing() const;
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory - Crafting") float GetRemainingTime() const;
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory - Crafting") float GetElapsedTime() const;
};
