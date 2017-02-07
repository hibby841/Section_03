// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	///if physics handle is attatched
	if (!(PhysicsComponent == nullptr))
	{
		if (PhysicsComponent->GrabbedComponent)
		{
			///move object were holding
			PhysicsComponent->SetTargetLocation(GetReachLineEnd());
			if (!ActorHit) { return; }
			FRotator ObjectRotation = ActorHit->GetActorRotation();
			FString ObjectRotationString = ObjectRotation.ToString();
			if (YawIsPressed)
			{
				PhysicsComponent->SetTargetRotation(ObjectRotation + FRotator(0, YawDirAndSpeed, 0));///form (pitch, yaw, roll)
			}
			if (RotationReset)
			{
				PhysicsComponent->SetTargetRotation(FRotator(0, 0, 0));
			}
			if (RollIsPressed)
			{
				PhysicsComponent->SetTargetRotation(ObjectRotation + FRotator(0, 0, RollDirAndSpeed));
			}
		}
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsComponent = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsComponent == nullptr) { UE_LOG(LogTemp, Error, TEXT("%s has no PhysicsComponent"), *GetOwner()->GetName()) }
}

void UGrabber::SetupInputComponent()
{
	MyInputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (MyInputComponent)
	{
		MyInputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		MyInputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
		MyInputComponent->BindAction("ResetRotation", IE_Pressed, this, &UGrabber::RotationResetPress);
		MyInputComponent->BindAction("RotateYaw", IE_Pressed, this, &UGrabber::YawPress);
		MyInputComponent->BindAction("RotateRoll", IE_Pressed, this, &UGrabber::RollPress);
		MyInputComponent->BindAction("ResetRotation", IE_Released, this, &UGrabber::RotationResetRelease);
		MyInputComponent->BindAction("RotateYaw", IE_Released, this, &UGrabber::YawRelease);
		MyInputComponent->BindAction("RotateRoll", IE_Released, this, &UGrabber::RollRealease);
	}
	else { UE_LOG(LogTemp, Error, TEXT("%s is missing a InputComponent")) }
}

void UGrabber::Grab()
{
	auto HitResult = GetHitFromRayCast();
	auto ComponentToGrab = HitResult.GetComponent();//gets the mesh in our case
	ActorHit = HitResult.GetActor();

	if (ActorHit)
	{if (!PhysicsComponent) { return; }
		///attatch physics handle
		PhysicsComponent->GrabComponentAtLocationWithRotation(
			ComponentToGrab, 
			NAME_None, //no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(), 
			ComponentToGrab->GetOwner()->GetActorRotation());
	}
}

void UGrabber::Release()
{
	if (!PhysicsComponent) { return; }
	PhysicsComponent->ReleaseComponent();
}

const FHitResult UGrabber::GetHitFromRayCast()
{
	///Line trace(AKA ray-cast) out to max grab distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())//Trace Parameters
	); return Hit;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation;
}

void UGrabber::YawPress()
{
	YawIsPressed = true;
}

void UGrabber::RotationResetPress()
{
	RotationReset = true;
}

void UGrabber::RollPress()
{
	RollIsPressed = true;
}

void UGrabber::YawRelease()
{
	YawIsPressed = false;
}

void UGrabber::RotationResetRelease()
{
	RotationReset = false;
}

void UGrabber::RollRealease()
{
	RollIsPressed = false;
}

