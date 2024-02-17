// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GCBaseCharacter.generated.h"

class UGCBaseCharacterMovementComponent;
UCLASS(Abstract, NotBlueprintable)
class LESSON3C_API AGCBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGCBaseCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void MoveForward(float Value) {};
	virtual void MoveRight(float Value) {};
	virtual void Turn(float Value) {};
	virtual void LookUp(float Value) {};

	virtual void TurnAtRate(float Value) {};
	virtual void LookUpAtRate(float Value) {};

	virtual void ChangeCrouchState();

	virtual void StartSprint();
	virtual void StopSprint();
	
	virtual void Tick(float DeltaSeconds) override;
	void TryChangeSprintState();

	FORCEINLINE UGCBaseCharacterMovementComponent* GetBaseCharacterMovementComponent() {return GCBaseCharacterMovementComponent;}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character | Controls")
	float BaseTurnRate = 45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character | Controls")
	float BaseLookUpRate = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character | Movement")
	float SprintSpeed = 800.0f;

	virtual bool CanSprint();

	UGCBaseCharacterMovementComponent* GCBaseCharacterMovementComponent;

private:
	bool bIsSprintRequested = false;
};
