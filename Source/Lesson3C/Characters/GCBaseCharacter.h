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

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void MoveForward(float Value) {}
	virtual void MoveRight(float Value) {}
	virtual void Turn(float Value) {}
	virtual void LookUp(float Value) {}

	virtual void TurnAtRate(float Value) {}
	virtual void LookUpAtRate(float Value) {}

	virtual void ChangeCrouchState();

	virtual void StartSprint();
	virtual void StopSprint();

	void TryChangeSprintState();

	FORCEINLINE UGCBaseCharacterMovementComponent* GetBaseCharacterMovementComponent() { return GCBaseCharacterMovementComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetIKRightFootOffset() const { return IKRightFootOffset; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetIKLeftFootOffset() const { return IKLeftFootOffset; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character | Controls")
	float BaseTurnRate = 45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character | Controls")
	float BaseLookUpRate = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character | Movement | Sprint")
	float SprintSpeed = 800.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character | Movement | Sprint",
		meta=(ClampMin=0.0f, UIMin=0.0f))
	float MaxStamina = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character | Movement | Sprint",
		meta=(ClampMin=0.0f, UIMin=0.0f))
	float StaminaRestoreRate = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character | Movement | Sprint",
		meta=(ClampMin=0.0f, UIMin=0.0f))
	float SprintStaminaConsumptionRate = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character | IK Settings")
	FName RightFootSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character | IK Settings")
	FName LeftFootSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character | IK Settings", meta=(ClampMin=0.0f, UIMin=0.0f))
	float IKTraceExtendDistance = 50.0f;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Character | IK Settings", meta=(ClampMin=0.0f, UIMin=0.0f))
	float IKInterpSpeed = 20.0f;

	UGCBaseCharacterMovementComponent* GCBaseCharacterMovementComponent;

	virtual bool CanSprint();

private:
	bool bIsSprintRequested = false;

	float IKRightFootOffset = 0.0f;
	float IKLeftFootOffset = 0.0f;

	float IKTraceDistance = 45.0f;
	float IKScale = 1.0f;

	float CurrentStamina = 0.0f;

	float GetIKOffsetForASocket(const FName& SocketName);

	void RestoreStamina(float DeltaSeconds);
	void SprintConsumeStamina(float DeltaSeconds);
};
