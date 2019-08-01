#pragma once

#include "CoreMinimal.h"
#include "InventoryStructs.generated.h"

USTRUCT(BlueprintType)
	struct FItemStack {
	GENERATED_BODY()
	FItemStack()
		: Amount(0), Item(0) {
	}

	FItemStack(int32 Item, int32 Amount)
		: Amount(Amount),
		  Item(Item) {
	}

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) int32 Amount;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) int32 Item;

	bool isEmpty() { return Item == 0 || Amount == 0; }
};
