// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingUnitBase.h"
#include "BufferArchive.h"
#include "ObjectWriter.h"
#include "MemoryReader.h"
#include "FileHelper.h"
#include "ArchiveSaveCompressedProxy.h"

// Sets default values
ABuildingUnitBase::ABuildingUnitBase() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	SetRootComponent(BuildingMesh);

	BU_Type = FName(*this->GetName());
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
		const float tempDist = FVector::DistSquared(location, BuildingMesh->GetSocketLocation(socket));
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

void ABuildingUnitBase::AddSocketedChild(const FName socket, ABuildingUnitBase* child) {
	// manage attachment
	const auto socketIndex = getSocketIndex(socket);
	if (!socketedChildren.IsValidIndex(socketIndex)) {
		UE_LOG(LogTemp, Warning, TEXT("Tried to attach BU to nonexistent socket %s, index: %d."), *socket.ToString(), socketIndex);
		return;
	}

	if(IsValid(socketedChildren[socketIndex])) {
		UE_LOG(LogTemp, Warning, TEXT("Tried to attach BU to filled socket %s, index: %d."), *socket.ToString(), socketIndex);
		return;
	}
	
	child->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, socket);
	UE_LOG(LogTemp, Warning, TEXT("Attached to socket index: %d, num: %d"), getSocketIndex(socket), socketedChildren.Num());
	socketedChildren[socketIndex] = child;
}

void ABuildingUnitBase::AddUnsocketedChild(const FTransform attachment, ABuildingUnitBase* child) {
	child->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	FUnsocketedAttachment attach;
	attach.transform = attachment;
	attach.child = child;
	attach.parent = this;
	unsocketedChildren.Emplace(attach);
}

int32 ABuildingUnitBase::getSocketIndex(FName socket) const {
	const auto sockets = BuildingMesh->GetAllSocketNames();
	return sockets.Find(socket);
}

std::string ABuildingUnitBase::RecursiveSerialize(ABuildingUnitBase* RootBuildingUnitBase) {
	FBufferArchive buf;

	auto SerializedBuildingUnit = RootBuildingUnitBase->SerializeToStruct();
	
	buf << SerializedBuildingUnit;

	UE_LOG(LogTemp, Warning, TEXT("RecursiveSerialize has finished, %d bytes."),buf.Num());
	
	return std::string((char*)buf.GetData(), buf.Num());
}

FString ABuildingUnitBase::NewSerializeTest(ABuildingUnitBase* RootBuildingUnitBase) {
	FBufferArchive buf(true);
	auto SerializedBuildingUnit = RootBuildingUnitBase->RecursiveGetRoot()->SerializeToStruct();
	
	buf << SerializedBuildingUnit;

	UE_LOG(LogTemp, Warning, TEXT("RecursiveSerialize has finished, %d bytes."),buf.Num());

	//TArray<uint8> copy = buf;
	//FMemoryReader ver(copy, true);
	
	FString ffs;

	for(auto &elem:buf) {
		if(elem > 32 && elem < 126) ffs.AppendChar(elem);
		else {
			ffs.Append(" ");
			ffs.AppendInt(elem);
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("RecursiveSerialize produced the following archive: %s"), *ffs);

	auto a = std::string((char*)buf.GetData(), buf.Num());
	UE_LOG(LogTemp, Warning, TEXT("RecursiveSerialize produced a string of length: %d"), a.length());
	
	
	//FSerializedBuildingUnit verified;
	//ver << verified;
	TArray<uint8> compressedData;
	FArchiveSaveCompressedProxy compress = FArchiveSaveCompressedProxy(compressedData, NAME_Gzip);

	compress << buf;
	compress.Flush();
	
	FFileHelper::SaveArrayToFile(compressedData, TEXT("C:/Users/Admin/Documents/Unreal Projects/BuildSystem/BaseStructure.cbs"));
	UE_LOG(LogTemp, Warning, TEXT("RecursiveSerialize compressed archive length: %d"), compressedData.Num());
	
	return ffs;
}

FString ABuildingUnitBase::SerializeTest() { return FString(UTF8_TO_TCHAR(RecursiveSerialize(RecursiveGetRoot()).c_str())); }

FSerializedBuildingUnit ABuildingUnitBase::SerializeToStruct() {
	FSerializedBuildingUnit sbu;
	const auto Count = socketedChildren.Num();

	sbu.Type = BU_Type;
	sbu.SocketedAttachments.AddDefaulted(Count);

	for (int i = 0; i < Count; ++i) {
		if(IsValid(socketedChildren[i])) {
			//
			sbu.SocketedAttachments[i] = socketedChildren[i]->SerializeToStruct();
		}
	}

	return sbu;
}


ABuildingUnitBase* ABuildingUnitBase::RecursiveGetRoot() {
	ABuildingUnitBase* parent = Cast<ABuildingUnitBase>(GetAttachParentActor());

	if (parent && IsValid(parent)) {
		UE_LOG(LogTemp, Warning, TEXT("RecursiveFindParent: Stepping up to %s."), *parent->GetName());
		return parent->RecursiveGetRoot();
	}
	UE_LOG(LogTemp, Warning, TEXT("RecursiveFindParent: Found parent %s."), *this->GetName());
	return this;
}
/*
 *
 * 	for(auto &elem:BuildingUnit.socketedChildren) {
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
 */