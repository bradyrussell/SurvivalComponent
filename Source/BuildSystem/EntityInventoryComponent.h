// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInventoryComponent.h"
#include "EntityInventoryComponent.generated.h"

class AController;
/**
 * 
 */
UCLASS( ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent) )
class BUILDSYSTEM_API UEntityInventoryComponent : public UBaseInventoryComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly) bool ConsumeItem(int32 Slot, APawn* Cause = nullptr, AController* Instigator = nullptr, float MagnitudeMultiplier = 1.0);
};
