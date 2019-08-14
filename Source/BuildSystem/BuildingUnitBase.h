// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "BuildSystemStructs.h"
#include <string>
#include "BuildingUnitBase.generated.h"

UCLASS()
class BUILDSYSTEM_API ABuildingUnitBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingUnitBase();

	// this is aligned with the getSocketNames array indices
	UPROPERTY(BlueprintReadOnly) TArray<ABuildingUnitBase*> socketedChildren;
	UPROPERTY(BlueprintReadOnly) TArray<FUnsocketedAttachment> unsocketedChildren;
	UPROPERTY(BlueprintReadOnly, EditAnywhere) UStaticMeshComponent* BuildingMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//returns NAME_None on failure
	UFUNCTION(BlueprintCallable) FName getNearestSocket(FVector location) const;

	//returns NAME_None on failure
	UFUNCTION(BlueprintCallable) FName getNearestSocketOfType(FVector location, FString type) const;

	//split a socket FName into its constituent parts
	UFUNCTION(BlueprintCallable, BlueprintPure) static FBuildingSocket parseSocketName(FName socket);

	UFUNCTION(BlueprintCallable) void AddSocketedChild(FName socket, ABuildingUnitBase* child);

	UFUNCTION(BlueprintCallable) void AddUnsocketedChild(FTransform attachment, ABuildingUnitBase* child);

	UFUNCTION(BlueprintCallable, BlueprintPure) int32 getSocketIndex(FName socket) const;

	UFUNCTION(BlueprintCallable) ABuildingUnitBase* RecursiveGetRoot();

	static std::string RecursiveSerialize(ABuildingUnitBase* RootBuildingUnitBase);

	UFUNCTION(BlueprintCallable) FString SerializeTest();
	
	//UFUNCTION(BlueprintCallable) FString SerializeTest2(FString x);
};
