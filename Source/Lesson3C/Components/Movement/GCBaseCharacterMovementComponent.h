// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GCBaseCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class LESSON3C_API UGCBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsSprinting() { return bIsSprinting; }

	virtual float GetMaxSpeed() const override;

	void StartSprint();
	void StopSprint();

	FORCEINLINE bool IsOutOfStamina() const {return bIsOutOfStamina;}

	void SetIsOutOfStamina(bool bIsOutOfStamina_In);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character movement: sprint", meta=(ClampMin = 0.0f, UIMin = 0.0f))
	float SprintSpeed = 1200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character movement: Sprint", meta=(ClampMin = 0.0f, UIMin = 0.0f))
	float OutOfStaminaSpeed = 200.0f;

private:
	bool bIsSprinting;
	bool bIsOutOfStamina;
};
