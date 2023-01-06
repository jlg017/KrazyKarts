// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKartMovementComponent.h"
//#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"

// Sets default values for this component's properties
UGoKartMovementComponent::UGoKartMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGoKartMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGoKartMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(GetOwnerRole() == ROLE_AutonomousProxy || GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		LastMove = CreateMove(DeltaTime);
		SimulateMove(LastMove);
	}	
}


FGoKartMove UGoKartMovementComponent::CreateMove(float DeltaTime)
{
	FGoKartMove Move;
	Move.DeltaTime = DeltaTime;
	Move.Force = Force;
	Move.SteeringCrank = SteeringCrank;
	Move.Time = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

	return Move;
}

void UGoKartMovementComponent::SimulateMove(const FGoKartMove& Move)
{
	FVector ForceVector = GetOwner()->GetActorForwardVector() * MaxForce * Move.Force;

	ForceVector += GetAirResistance();
	ForceVector += GetRollingResistance();

	FVector Acceleration = ForceVector / Mass;

	Velocity = Velocity + (Acceleration * Move.DeltaTime);

	ApplyRotation(Move.DeltaTime, Move.SteeringCrank);
	UpdateLocationFromVelocity(Move.DeltaTime);
}

FVector UGoKartMovementComponent::GetAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector UGoKartMovementComponent::GetRollingResistance()
{
	float AccelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;
	float NormalForce = Mass * AccelerationDueToGravity;
	return -Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}

void UGoKartMovementComponent::UpdateLocationFromVelocity(float DeltaTime)
{
	FVector Translation = Velocity * 100 * DeltaTime;
	FHitResult hitResult;

	GetOwner()->AddActorWorldOffset(Translation, true, &hitResult);

	if (hitResult.IsValidBlockingHit())
	{
		Velocity = Velocity * 0;
	}
}

void UGoKartMovementComponent::ApplyRotation(float DeltaTime, float SteeringThrow)
{
	float DeltaLocation = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngle = DeltaLocation / MinTurningRadius * SteeringThrow;

	FQuat RotationDelta(GetOwner()->GetActorUpVector(), RotationAngle);
	Velocity = RotationDelta.RotateVector(Velocity);

	GetOwner()->AddActorWorldRotation(RotationDelta);
}