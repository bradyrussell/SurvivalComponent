// Fill out your copyright notice in the Description page of Project Settings.


#include "DatabaseProvider.h"
#include "Engine/World.h"
#include "InventoryStructs.h"
#include "InventoryGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "IDatabaseProvider.h"

FItemDefinition UDatabaseProvider::GetItemDefinition(UObject* WorldContextObject, FName item) {
	FString context = FString();

	const auto ItemDB = IIDatabaseProvider::Execute_GetItemDefinitions(UGameplayStatics::GetGameInstance(WorldContextObject));
	check(ItemDB);
	
	return *ItemDB->FindRow<FItemDefinition>(item, context);
}

FProcessingRecipe UDatabaseProvider::GetRecipeDefinition(UObject* WorldContextObject, FName item) {
	FString context = FString();

	const auto DB = IIDatabaseProvider::Execute_GetRecipeDefinitions(UGameplayStatics::GetGameInstance(WorldContextObject));
	check(DB);
	
	return *DB->FindRow<FProcessingRecipe>(item, context);
}

TMap<FName, FProcessingRecipe> UDatabaseProvider::GetAllRecipesOfType(UObject* WorldContextObject, ECraftingType Type) {
	FString context = FString();

	const auto DB = IIDatabaseProvider::Execute_GetRecipeDefinitions(UGameplayStatics::GetGameInstance(WorldContextObject));
	check(DB);

	TMap<FName, FProcessingRecipe> output;

	for(auto &elem:DB->GetRowMap()) {
		FProcessingRecipe* recipe = reinterpret_cast<FProcessingRecipe*>(elem.Value); 
		if(recipe->RecipeType == Type) output.Emplace(elem.Key, *recipe);
	}//todo optimize

	return output; // if inventory.hasAll output.Input

}

TMap<FName, FProcessingRecipe> UDatabaseProvider::GetAllRecipesForCraftingComponent(UObject* WorldContextObject, UCraftingInventoryComponent* Inventory) {
	FString context = FString();

	const auto DB = IIDatabaseProvider::Execute_GetRecipeDefinitions(UGameplayStatics::GetGameInstance(WorldContextObject));
	check(DB);

	TMap<FName, FProcessingRecipe> output;

	for(auto &elem:DB->GetRowMap()) {
		FProcessingRecipe* recipe = reinterpret_cast<FProcessingRecipe*>(elem.Value);
		if(recipe->RecipeType == Inventory->Type && Inventory->hasItems(recipe->InputItems)) output.Emplace(elem.Key, *recipe);
	}//todo optimize

	return output; 
}


int32 UDatabaseProvider::ItemToIndex(UObject* WorldContextObject, FName Item) {
	// auto instance = Cast<IIDatabaseProvider>(UGameplayStatics::GetGameInstance(WorldContextObject));
	// check(instance);
	// const auto ItemDB = instance->Execute_GetItemDefinitions(UGameplayStatics::GetGameInstance(WorldContextObject));
	// check(ItemDB);
	const auto ItemDB = IIDatabaseProvider::Execute_GetItemDefinitions(UGameplayStatics::GetGameInstance(WorldContextObject));
	check(ItemDB);
	
	return ItemDB->GetRowNames().Find(Item)+1;
}

FName UDatabaseProvider::IndexToItem(UObject* WorldContextObject, int32 Index) {
	//auto instance = /*Cast<IIDatabaseProvider>*/(UGameplayStatics::GetGameInstance(WorldContextObject));
	//check(instance);
	const auto ItemDB = IIDatabaseProvider::Execute_GetItemDefinitions(UGameplayStatics::GetGameInstance(WorldContextObject));
	check(ItemDB);


	auto Names = ItemDB->GetRowNames();
	if(!Names.IsValidIndex(Index-1)) return NAME_None;
	return Names[Index-1];
}

int32 UDatabaseProvider::RecipeToIndex(UObject* WorldContextObject, FName Recipe) {
	const auto DB = IIDatabaseProvider::Execute_GetRecipeDefinitions(UGameplayStatics::GetGameInstance(WorldContextObject));
	check(DB);

	return DB->GetRowNames().Find(Recipe)+1;
}

FName UDatabaseProvider::IndexToRecipe(UObject* WorldContextObject, int32 Index) {
	const auto DB = IIDatabaseProvider::Execute_GetRecipeDefinitions(UGameplayStatics::GetGameInstance(WorldContextObject));
	check(DB);

	auto Names = DB->GetRowNames();
	if(!Names.IsValidIndex(Index-1)) return NAME_None;
	return Names[Index-1];
}
