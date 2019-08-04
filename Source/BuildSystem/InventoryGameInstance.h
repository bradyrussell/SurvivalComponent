// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "InventoryGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BUILDSYSTEM_API UInventoryGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)	UDataTable* ItemDefinitions;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)	UDataTable* RecipeDefinitions;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)	UDataTable* LootDefinitions;
};
