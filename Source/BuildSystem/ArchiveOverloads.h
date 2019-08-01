#pragma once
#include "Platform.h"
#include "Archive.h"
#include "BuildingUnitBase.h"

// this currently traverses a tree in post order
FORCEINLINE FArchive& operator<<(FArchive& Ar, ABuildingUnitBase* BuildingUnit) {
	if (!BuildingUnit)
		return Ar;

	FName BU_Type = "BuildingUnitBase";
	FTransform Transform = BuildingUnit->GetActorTransform();

	Ar << BU_Type;
	//Ar << Transform; //we only want the root's transform or unsocketed attachments

	// look up this meshes number of sockets 

	for(auto &elem:BuildingUnit->socketedChildren) {
		//   save each socket's child or null
		Ar << elem;
	}

	// save number of unsocketed attachments
	int32 usChildren = BuildingUnit->unsocketedChildren.Num();
	Ar << usChildren;

	for(auto &elem:BuildingUnit->unsocketedChildren) {
		// save each unsocketed attachment and its transform
		Ar << elem.transform;
		Ar << elem.child;
	}

	return Ar;
}
