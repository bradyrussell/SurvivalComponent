// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingUnitBase.h"
#include "BufferArchive.h"

// Sets default values
ABuildingUnitBase::ABuildingUnitBase() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	SetRootComponent(BuildingMesh);
}

// Called when the game starts or when spawned
void ABuildingUnitBase::BeginPlay() {
	Super::BeginPlay();
	socketedChildren.AddDefaulted(BuildingMesh->GetAllSocketNames().Num());
}

// Called every frame
void ABuildingUnitBase::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

FName ABuildingUnitBase::getNearestSocket(FVector location) const {
	float distance = INFINITY;
	FName closest = NAME_None;

	auto sockets = BuildingMesh->GetAllSocketNames();
	for (auto& socket : sockets) {
		float tempDist = FVector::DistSquared(location, BuildingMesh->GetSocketLocation(socket));
		if (distance > tempDist) {
			closest = socket;
			distance = tempDist;
		}
	}

	return closest;
}

FName ABuildingUnitBase::getNearestSocketOfType(FVector location, FString type) const {
	float distance = INFINITY;
	FName closest = NAME_None;

	auto sockets = BuildingMesh->GetAllSocketNames();
	for (auto& socket : sockets) {
		float tempDist = FVector::DistSquared(location, BuildingMesh->GetSocketLocation(socket));
		auto parsed = parseSocketName(socket);
		if (parsed.type.Equals(type, ESearchCase::IgnoreCase) && distance > tempDist) {
			closest = socket;
			distance = tempDist;
		}
	}

	return closest;
}

FBuildingSocket ABuildingUnitBase::parseSocketName(FName socket) {
	FString socket_name = socket.ToString();

	FBuildingSocket ret;
	socket_name.Split("_", &ret.axis, &ret.type);

	return ret;
}

void ABuildingUnitBase::AddSocketedChild(FName socket, ABuildingUnitBase* child) {
	// manage attachment
	auto socketIndex = getSocketIndex(socket);
	if (socketIndex < socketedChildren.Num()) {
		UE_LOG(LogTemp, Warning, TEXT("Tried to attach BU to nonexistent socket %s."), *socket.ToString());
		return;
	}
	child->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, socket);
	UE_LOG(LogTemp, Warning, TEXT("si: %d, num: %d"), getSocketIndex(socket), socketedChildren.Num());
	socketedChildren[socketIndex] = child;
}

void ABuildingUnitBase::AddUnsocketedChild(FTransform attachment, ABuildingUnitBase* child) {
	child->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	FUnsocketedAttachment attach;
	attach.transform = attachment;
	attach.child = child;
	attach.parent = this;
	unsocketedChildren.Emplace(attach);
}

int32 ABuildingUnitBase::getSocketIndex(FName socket) const {
	auto sockets = BuildingMesh->GetAllSocketNames();
	return sockets.Find(socket);
}

std::string ABuildingUnitBase::RecursiveSerialize(ABuildingUnitBase* RootBuildingUnitBase) {
	FBufferArchive archive;

	archive << RootBuildingUnitBase;

	return std::string((char*)archive.GetData(), archive.Num());
}

ABuildingUnitBase* ABuildingUnitBase::RecursiveGetRoot() {
	ABuildingUnitBase* parent = Cast<ABuildingUnitBase>(GetAttachParentActor());

	if (parent && IsValid(parent)) {
		return parent->RecursiveGetRoot();
	}

	return this;
}
