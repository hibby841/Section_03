// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"

#define OUT


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
	bool YawIsPressed = false;
	bool PitchIsPressed = false;
	bool RollIsPressed = false;
	//needed for getting the rotation the held object
	AActor *ActorHit = nullptr;

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
	//look for (assumed) input component that appears at runtime
	void SetupInputComponent();
	//gets current end of reach line
	FVector GetReachLineEnd();
	//gets current start of reach line
	FVector GetReachLineStart();
	//does a raycast out to LineTraceEnd and returns what it hits if its a physics body
	const FHitResult GetHitFromRayCast();
	//calls GetHitFromRaycast and determines what to do based on the hit
	void Grab();
	//will drop the object the player is holding on key release
	void Release();
	//sets YawIsPressed to true
	void YawPress();
	//sets PitchIsPressed to true
	void PitchPress();
	//sets RollIsPressed to true
	void RollPress();
	//sets YawIsPressed to false
	void YawRelease();
	//sets PitchIsPressed to false
	void PitchRelease();
	//sets RollIsPressed to false
	void RollRealease();

};
