// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"
#define OUT


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	//get owning actor
	Owner = GetOwner();
	//set actor that opens
	StartingRotation = Owner->GetActorRotation();
}

void UOpenDoor::OpenDoor()
{
	OnOpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, CloseAngle, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//poll the trigger volume every frame
	if (GetTotalMassOfActorsOnPlate() >= 30.f)//if the actor that opens is in the volume
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if ((GetWorld()->GetTimeSeconds() - LastDoorOpenTime) >= 0.75)//check if its time to close the door
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;
	if (!PressurePlate) { UE_LOG(LogTemp, Error, TEXT("No PressurePlate Found!!!"))return TotalMass; }
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

