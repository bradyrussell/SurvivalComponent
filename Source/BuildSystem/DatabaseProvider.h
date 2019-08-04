// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Factories/CompositeDataTableFactory.h"
#include "InventoryStructs.h"
#include "DatabaseProvider.generated.h"

/**
 * 
 */
UCLASS()
class BUILDSYSTEM_API UDatabaseProvider : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	//UFUNCTION(BlueprintPure, Category="Inventory Data", meta = (WorldContext = "WorldContextObject")) static int32 GetMaxStackForItem(UObject* WorldContextObject, FName item);

	UFUNCTION(BlueprintPure, Category="Inventory Data", meta = (WorldContext = "WorldContextObject")) static FItemDefinition GetItemDefinition(UObject* WorldContextObject, FName item);
	UFUNCTION(BlueprintPure, Category="Inventory Data", meta = (WorldContext = "WorldContextObject")) static FProcessingRecipe GetRecipeDefinition(UObject* WorldContextObject, FName item);
	//UFUNCTION(BlueprintPure, Category="Inventory", meta = (WorldContext = "WorldContextObject")) static int32 GetLootDefinition(UObject* WorldContextObject, FName item);

	UFUNCTION(BlueprintPure, Category="Inventory Data", meta = (WorldContext = "WorldContextObject")) static int32 ItemToIndex(UObject* WorldContextObject, FName Item);
	UFUNCTION(BlueprintPure, Category="Inventory Data", meta = (WorldContext = "WorldContextObject")) static FName IndexToItem(UObject* WorldContextObject, int32 Index);
};
