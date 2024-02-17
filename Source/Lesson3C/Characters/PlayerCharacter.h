// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCBaseCharacter.h"
#include "Components/TimelineComponent.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class LESSON3C_API APlayerCharacter : public AGCBaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sprint")
	UCurveFloat* SprintCurve;
	
	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;
	virtual void Turn(float Value) override;
	virtual void LookUp(float Value) override;
	virtual void TurnAtRate(float Value) override;
	virtual void LookUpAtRate(float Value) override;

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual bool CanJumpInternal_Implementation() const override;
	virtual void OnJumped_Implementation() override;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character | Camera")
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character | Camera")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character | Camera", meta=(UIMin = 0.0f, ClampMin = 0.0f))
	float DefaultSpringArmLength = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character | Camera", meta=(UIMin = 0.0f, ClampMin = 0.0f))
	float SprintSpringArmLength = 500.0f;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void StartSprint() override;
	virtual void StopSprint() override;


private:
	FTimeline SprintCameraTimeline;
	
	UFUNCTION()
	void UpdateSprintCamera(float Value) const;

	FORCEINLINE
	void BeginSprintCamera() { SprintCameraTimeline.Play(); }
	FORCEINLINE
	void EndSprintCamera() {SprintCameraTimeline.Reverse(); }
};
