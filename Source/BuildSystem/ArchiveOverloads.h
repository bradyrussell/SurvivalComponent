#pragma once
#include "Platform.h"
#include "Archive.h"
#include "BuildingUnitBase.h"
#include "ObjectWriter.h"

// this currently traverses a tree in post order
/*FORCEINLINE FArchive& operator<<(FArchive& Ar, ABuildingUnitBase& BuildingUnit) {
	if (!IsValid(&BuildingUnit))
		return Ar;

	FName BU_Type = "BuildingUnitBase";
	FTransform Transform = BuildingUnit.GetActorTransform();

	Ar << BU_Type;
	//Ar << Transform; //we only want the root's transform or unsocketed attachments

	// look up this meshes number of sockets 

	for(auto &elem:BuildingUnit.socketedChildren) {
		//   save each socket's child or null
		Ar << *elem;
	}

	// save number of unsocketed attachments
	int32 usChildren = BuildingUnit.unsocketedChildren.Num();
	Ar << usChildren;

	for(auto &elem:BuildingUnit.unsocketedChildren) {
		// save each unsocketed attachment and its transform
		Ar << elem.transform;
		Ar << *elem.child;
	}

	return Ar;
}*/

// this currently traverses a tree in post order
FORCEINLINE FArchive& operator<<(FArchive& Ar, FSerializedBuildingUnit &SerializedBuildingUnit) {

	const FString saveLoad = Ar.IsSaving() ? "Saving" : "Loading";

	Ar << SerializedBuildingUnit.Type;

	UE_LOG(LogTemp, Warning, TEXT("%s: Type %s"), *saveLoad, *SerializedBuildingUnit.Type.ToString());
	
	if(SerializedBuildingUnit.Type.IsEqual("Empty_Node")) return Ar; // dont write null nodes children. just saves space 
	
	UE_LOG(LogTemp, Warning, TEXT("-- %s: Begin %s attachments. "),*saveLoad, *saveLoad,SerializedBuildingUnit.SocketedAttachments.Num());
	
	Ar << SerializedBuildingUnit.SocketedAttachments;

	UE_LOG(LogTemp, Warning, TEXT("-- %s: %d SocketedAttachments"),*saveLoad, SerializedBuildingUnit.SocketedAttachments.Num());

	return Ar;
}