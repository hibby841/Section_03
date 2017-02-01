// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"


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
	ActorThatOpenes = GetWorld()->GetFirstPlayerController()->GetPawn();
	StartingRotation = Owner->GetActorRotation();
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
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
	if (PressurePlate->IsOverlappingActor(ActorThatOpenes))//if the actor that opens is in the volume
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if ((GetWorld()->GetTimeSeconds() - LastDoorOpenTime) >= 0.75)//check if its time to close the door
	{
		CloseDoor();
	}
}

