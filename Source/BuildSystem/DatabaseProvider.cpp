// Fill out your copyright notice in the Description page of Project Settings.


#include "DatabaseProvider.h"
#include "Engine/World.h"
#include "InventoryStructs.h"
#include "InventoryGameInstance.h"
#include "Kismet/GameplayStatics.h"


/*int32 UDatabaseProvider::GetMaxStackForItem(UObject* WorldContextObject, FName item) {
	FString context = FString();

	auto instance = Cast<UInventoryGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	check(instance);
	const auto ItemDB = instance->ItemDefinitions;
	check(ItemDB);
	return ItemDB->FindRow<FItemDefinition>(item, context)->MaxStack;
}*/

FItemDefinition UDatabaseProvider::GetItemDefinition(UObject* WorldContextObject, FName item) {
	FString context = FString();

	auto instance = Cast<UInventoryGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	check(instance);
	const auto ItemDB = instance->ItemDefinitions;
	check(ItemDB);
	return *ItemDB->FindRow<FItemDefinition>(item, context);
}

FProcessingRecipe UDatabaseProvider::GetRecipeDefinition(UObject* WorldContextObject, FName item) {
	FString context = FString();

	auto instance = Cast<UInventoryGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	check(instance);
	const auto DB = instance->RecipeDefinitions;
	check(DB);
	return *DB->FindRow<FProcessingRecipe>(item, context);
}

int32 UDatabaseProvider::ItemToIndex(UObject* WorldContextObject, FName Item) {
	auto instance = Cast<UInventoryGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	check(instance);
	const auto ItemDB = instance->ItemDefinitions;
	check(ItemDB);

	return ItemDB->GetRowNames().Find(Item)+1;
}

FName UDatabaseProvider::IndexToItem(UObject* WorldContextObject, int32 Index) {
	auto instance = Cast<UInventoryGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	check(instance);
	const auto ItemDB = instance->ItemDefinitions;
	check(ItemDB);


	auto Names = ItemDB->GetRowNames();
	if(!Names.IsValidIndex(Index-1)) return NAME_None;
	return Names[Index-1];
}
