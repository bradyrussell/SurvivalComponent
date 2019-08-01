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

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Item Stack") int32 Amount;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Item Stack") int32 Item;

	/*UFUNCTION(BlueprintPure) not allowed*/
	bool isEmpty() { return Item == 0 || Amount == 0; }
};

UENUM(BlueprintType)
	enum class ECraftingType : uint8 {
	CT_None UMETA(DisplayName="None"),
	CT_Crafting UMETA(DisplayName="Crafting"),
	CT_Cooking UMETA(DisplayName="Cooking"),
	CT_Advanced UMETA(DisplayName="Advanced")
};

USTRUCT(BlueprintType)
	struct FProcessingRecipe {
	GENERATED_BODY()


	FProcessingRecipe() = default;

	FProcessingRecipe(ECraftingType RecipeType, const TArray<FItemStack> InputItems, const TArray<FItemStack> OutputItems, const float ProcessingDuration)
		: RecipeType(RecipeType), ProcessingDuration(ProcessingDuration),
		  InputItems(InputItems), OutputItems(OutputItems) {
	}

	FProcessingRecipe(
		ECraftingType RecipeType,
		const TArray<FItemStack> InputItems,
		const TArray<FItemStack> OutputItems,
		const float ProcessingDuration,
		bool bRequiresFuel = false,
		float FuelRequired = 0.0,
		bool bHasVariableYield = false,
		const FVector2D VariableYieldRange = FVector2D(0, 0),
		bool bRequiresCatalyst = false,
		const TArray<FItemStack> CatalystItems = TArray<FItemStack>()
	)
		: RecipeType(RecipeType), ProcessingDuration(ProcessingDuration),
		  bRequiresFuel(bRequiresFuel),
		  bRequiresCatalyst(bRequiresCatalyst),
		  bHasVariableYield(bHasVariableYield),
		  InputItems(InputItems),
		  OutputItems(OutputItems),
		  VariableYieldRange(VariableYieldRange),
		  CatalystItems(CatalystItems),
		  FuelRequired(FuelRequired) {
	}

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Recipe") ECraftingType RecipeType = ECraftingType::CT_None;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Recipe") float ProcessingDuration = 1.0f;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Recipe") bool bRequiresFuel = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Recipe") bool bRequiresCatalyst = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Recipe") bool bHasVariableYield = false;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Recipe") TArray<FItemStack> InputItems;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Recipe") TArray<FItemStack> OutputItems;

	// the min - max percentages which the output will be multiplied by
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Recipe") FVector2D VariableYieldRange;

	// if RequiresCatalyst, these items must be present as input but wont be consumed
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Recipe") TArray<FItemStack> CatalystItems;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Recipe") float FuelRequired = 0.f;
};
