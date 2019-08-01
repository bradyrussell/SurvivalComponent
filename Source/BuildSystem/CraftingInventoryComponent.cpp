// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingInventoryComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "UnrealNetwork.h"

bool UCraftingInventoryComponent::QueueRecipe(FProcessingRecipe InRecipe) {
	if(InRecipe.RecipeType == Type) ProcessingQueue.Enqueue(InRecipe);
	return InRecipe.RecipeType == Type;
}

bool UCraftingInventoryComponent::BeginProcessing() {
	if(bIsCurrentlyProcessing || ProcessingQueue.IsEmpty()) return false;

	setLocked(true);
	ProcessingQueue.Dequeue(CurrentJob_Recipe);

	if(CurrentJob_Recipe.ProcessingDuration == 0.f) { // zero duration , complete instantly
		EndProcessing();
		return true;
	}

	GetWorld()->GetTimerManager().SetTimer(CurrentJob_Timer, this, &UCraftingInventoryComponent::EndProcessing, CurrentJob_Recipe.ProcessingDuration, false);
	if(CurrentJob_Timer.IsValid()) bIsCurrentlyProcessing = true;
	//Client_ExpectedFinish = FDateTime::Now()

	return bIsCurrentlyProcessing;
}

FProcessingRecipe UCraftingInventoryComponent::PeekNextRecipe() const {
	FProcessingRecipe out;
	ProcessingQueue.Peek(out);
	return out;
}

void UCraftingInventoryComponent::EndProcessing() {
	if(!bIsCurrentlyProcessing) return;

	// do crafting recipe
	if(hasItems(CurrentJob_Recipe.InputItems) &&
		(!CurrentJob_Recipe.bRequiresCatalyst || hasItems(CurrentJob_Recipe.CatalystItems)) &&
		(!CurrentJob_Recipe.bRequiresFuel || CurrentJob_Recipe.FuelRequired >= Fuel)) {

		if(CurrentJob_Recipe.bRequiresFuel) Fuel -= CurrentJob_Recipe.FuelRequired;

		if(areAllEmpty(RemoveItems(CurrentJob_Recipe.InputItems))) {
			AddItems(CurrentJob_Recipe.OutputItems);
		}
	} else if(CurrentJob_Recipe.FuelRequired > Fuel) {
		ClearQueue();
	}
	
	CurrentJob_Timer.Invalidate();
	bIsCurrentlyProcessing = false;
	setLocked(false);

	if(!ProcessingQueue.IsEmpty()) BeginProcessing();
}

// void UCraftingInventoryComponent::Client_SetTimer_Implementation(float duration) { //todo add delegates // todo add function meta categories
// 	if(duration > 0.f) { // todo i feel that in isngleplayer this will run on the server and step on our real timer
// 		GetWorld()->GetTimerManager().SetTimer(CurrentJob_Timer, this, &UCraftingInventoryComponent::Client_TimerFinished, duration, false);
// 	} else {
// 		CurrentJob_Timer.Invalidate();
// 	}
// }
//
// void UCraftingInventoryComponent::Client_TimerFinished() {
// }

void UCraftingInventoryComponent::CancelProcessing() {
	if(!bIsCurrentlyProcessing) return;
	bIsCurrentlyProcessing = false;
	CurrentJob_Timer.Invalidate();
	setLocked(false);
}

void UCraftingInventoryComponent::ClearQueue() {
	ProcessingQueue.Empty();
}

void UCraftingInventoryComponent::CancelAndClearQueue() {
	ClearQueue();
	CancelProcessing();
}

bool UCraftingInventoryComponent::isCurrentlyProcessing() const {
	return bIsCurrentlyProcessing;
}

float UCraftingInventoryComponent::GetRemainingTime() const {
	return GetWorld()->GetTimerManager().GetTimerRemaining(CurrentJob_Timer);
}

float UCraftingInventoryComponent::GetElapsedTime() const {
	return GetWorld()->GetTimerManager().GetTimerElapsed(CurrentJob_Timer);
}

void UCraftingInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	DOREPLIFETIME(UCraftingInventoryComponent, Type);
	DOREPLIFETIME(UCraftingInventoryComponent, Fuel);
	DOREPLIFETIME(UCraftingInventoryComponent, bIsCurrentlyProcessing);
	DOREPLIFETIME(UCraftingInventoryComponent, CurrentJob_Recipe);
	DOREPLIFETIME(UCraftingInventoryComponent, CurrentJob_Timer);
	//DOREPLIFETIME(UCraftingInventoryComponent, Client_ExpectedFinish);
}