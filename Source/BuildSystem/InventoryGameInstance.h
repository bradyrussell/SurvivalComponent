// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "IDatabaseProvider.h"
#include "InventoryGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BUILDSYSTEM_API UInventoryGameInstance : public UGameInstance, public IIDatabaseProvider
{
	GENERATED_BODY()
public:

};
//todo make this an interface