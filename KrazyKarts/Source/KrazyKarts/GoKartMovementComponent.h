// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoKartMovementComponent.generated.h"

USTRUCT()
struct FGoKartMove {
	GENERATED_USTRUCT_BODY()
		//data values to be able to simulate move from a given state
	UPROPERTY()
	float DeltaTime;

	UPROPERTY()
	float Force;

	UPROPERTY()
	float SteeringCrank;

	UPROPERTY()
	float Time;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KRAZYKARTS_API UGoKartMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	// Sets default values for this component's properties
	UGoKartMovementComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SimulateMove(const FGoKartMove& Move);
	
	FGoKartMove GetLastMove() { return LastMove; }
	FVector GetVelocity() { return Velocity; }
	
	void SetVelocity(FVector val) { Velocity = val; }
	void SetForce(float force) { Force = force; }
	void SetSteeringCrank(float sc) { SteeringCrank = sc; }



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	FVector GetAirResistance();
	FVector GetRollingResistance();

	void UpdateLocationFromVelocity(float DeltaTime);
	void ApplyRotation(float DeltaTime, float SteeringThrow);

	FGoKartMove CreateMove(float DeltaTime);

	//The mass of the car in kg
	UPROPERTY(EditAnywhere)
	float Mass = 1000;
	//in newtons
	UPROPERTY(EditAnywhere)
	float MaxForce = 5000;
	//minimum radius to turn at full control in m
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 8;
	//amount of drag on the car: higher is more drag
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;
	//amount of drag on the car: higher is more r.resistance
	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficient = 0.015;

	FGoKartMove LastMove;

	FVector Velocity;
	float Force;
	float SteeringCrank;

};
