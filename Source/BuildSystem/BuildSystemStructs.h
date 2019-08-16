#pragma once

#include "CoreMinimal.h"
#include "DataTableEditor/Private/SRowEditor.h"
#include "BuildSystemStructs.generated.h"

class ABuildingUnitBase;

USTRUCT(BlueprintType)
struct FUnsocketedAttachment
{
	GENERATED_BODY()

	// the transform of the child relative to the parent
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FTransform transform;

	// the 'owner' of the attachment
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) ABuildingUnitBase* parent;

	// the attached object
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) ABuildingUnitBase* child;
};

USTRUCT(BlueprintType)
struct FBuildingSocket
{
	GENERATED_BODY()

	// type of socket, door, wall, window etc
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FString type;

	// the mutually exclusive axis the socket is along. X+ X- etc
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FString axis;

};

USTRUCT(BlueprintType)
struct FElectricConnection
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) uint8 capacity; //todo change to voltage enum

	// the 'owner' of the attachment
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) ABuildingUnitBase* parent;

	// the attached object
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) ABuildingUnitBase* child;

	// the location of the cable end attached to the parent
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FVector parent_attach_location;

	// the location of the cable end attached to the child
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FVector child_attach_location;

};


USTRUCT(BlueprintType)
struct FSerializedBuildingUnit
{
	GENERATED_BODY()

	// 
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) FName Type = "Empty_Node";

	TArray<FSerializedBuildingUnit> SocketedAttachments;

	//todo 
};

USTRUCT(BlueprintType)
	struct FBuildingUnitDefinition : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Building Unit") FText DisplayName;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Building Unit") TSubclassOf<ABuildingUnitBase> BuildingUnitClass;

};