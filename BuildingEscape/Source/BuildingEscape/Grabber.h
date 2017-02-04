// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	FVector LineTraceDirection;
	FVector LineTraceEnd;
	bool YawIsPressed = false;
	bool PitchIsPressed = false;
	bool RollIsPressed = false;
	AActor *ActorHit;

	UPhysicsHandleComponent *PhysicsComponent = nullptr;
	UInputComponent *MyInputComponent = nullptr;

	UPROPERTY(EditAnywhere)
	float Reach = 100.f;//how far in cm. in front of the player can they grab

	UPROPERTY(EditAnywhere)
	int32 YawDirAndSpeed = 4;

	UPROPERTY(EditAnywhere)
	int32 PitchDirAndSpeed = 4;

	UPROPERTY(EditAnywhere)
	int32 RollDirAndSpeed = 4;

	//finds the (assumed) attatched physics handle component
	void FindPhysicsHandleComponent();

	//look for input component that appears at runtime
	void SetupInputComponent();

	//gets PlayerViewPointLocation And PlayerViewPointRotation plus calculates the LineTraceDirection And LineTraceEnd
	void GetPVPLAndRToLTDAndE();

	//does a raycast out to LineTraceEnd and returns what it hits if its a physics body
	FHitResult GetHitFromRayCast();

	//calls GetHitFromRaycast and determines what to do based on the hit
	void Grab();

	//TODO will drop the object the player is holding on key release
	void Release();

	void YawPress();
	void PitchPress();
	void RollPress();
	void YawRelease();
	void PitchRelease();
	void RollRealease();
};
