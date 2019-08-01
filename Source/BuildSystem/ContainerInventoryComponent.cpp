// Fill out your copyright notice in the Description page of Project Settings.


#include "ContainerInventoryComponent.h"
#include "UnrealNetwork.h"

void UContainerInventoryComponent::setLocked(bool Locked) {
	bIsLocked = Locked;
}

void UContainerInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	DOREPLIFETIME(UContainerInventoryComponent, bIsLocked);
	//DOREPLIFETIME(UContainerInventoryComponent, InventorySlots);
}