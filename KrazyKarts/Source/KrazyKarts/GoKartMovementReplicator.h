// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoKartMovementComponent.h"
#include "GoKartMovementReplicator.generated.h"

USTRUCT()
struct FGoKartState {
	GENERATED_USTRUCT_BODY()
	//data values representing current state of the kart
	UPROPERTY()
	FVector Velocity;
	
	UPROPERTY()
	FGoKartMove LastMove;
	
	UPROPERTY()
	FTransform Transform;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class KRAZYKARTS_API UGoKartMovementReplicator : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGoKartMovementReplicator();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void ClearAcknowledgedMoves(FGoKartMove LastMove);
	void UpdateServerState(const FGoKartMove Move);
	void ClientTick(float DeltaTime);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SendMove(FGoKartMove Move);

	UFUNCTION()
		void OnRep_ServerState();
	UFUNCTION()
		void AutonomousProxy_OnRep_ServerState();
	UFUNCTION()
		void SimulatedProxy_OnRep_ServerState();


	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
		FGoKartState ServerState;

	UPROPERTY()
		UGoKartMovementComponent* MovementComponent;

	//list of unacknowledged moves
	TArray<FGoKartMove> UnacknowledgedMoves;

	float ClientTimeSinceUpdate;
	float ClientTimeBetweenLastUpdates;

	FTransform ClientStartTransform;
	FVector ClientStartVelocity;
};
