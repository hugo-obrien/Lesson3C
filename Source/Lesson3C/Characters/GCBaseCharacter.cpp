// Fill out your copyright notice in the Description page of Project Settings.


#include "GCBaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Lesson3C/Components/Movement/GCBaseCharacterMovementComponent.h"

AGCBaseCharacter::AGCBaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGCBaseCharacterMovementComponent>(
		ACharacter::CharacterMovementComponentName))
{
	GCBaseCharacterMovementComponent = StaticCast<UGCBaseCharacterMovementComponent*>(GetCharacterMovement());

	IKScale = GetActorScale3D().Z;
	const float UnscaledCapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	IKTraceDistance = UnscaledCapsuleHalfHeight * IKScale;
	UE_LOG(LogTemp, Log, TEXT("Scale %f, unscaled %f, scaled %f"), IKScale, UnscaledCapsuleHalfHeight, IKTraceDistance);
}

void AGCBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentStamina = MaxStamina;
}

void AGCBaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	IKRightFootOffset = FMath::FInterpTo(IKRightFootOffset, GetIKOffsetForASocket(RightFootSocketName), DeltaSeconds, IKInterpSpeed);
	IKLeftFootOffset = FMath::FInterpTo(IKLeftFootOffset, GetIKOffsetForASocket(LeftFootSocketName), DeltaSeconds, IKInterpSpeed);
	
	TryChangeSprintState();

	// const FString OnScreenString = FString::Printf(TEXT("%s\n\tState %s [%i]\n\t%s\n\tBlending %i On-Demand %i"), *Actor->GetName(), *StateEnum->GetDisplayNameTextByValue(ActorData.CurrentState).ToString(), ActorData.PermutationIndex, *StateEnum->GetDisplayNameTextByValue(ActorData.PreviousState).ToString(), ActorData.bBlending, ActorData.bRunningOnDemand);
	// GEngine->AddOnScreenDebugMessage((uint64)-1, 2.0f, FColor::Emerald, Message.ToString());

	const FString StaminaDebugString = FString::Printf(TEXT("Current stamina: %f"), CurrentStamina);
	GEngine->AddOnScreenDebugMessage(-1, 0.05f, FColor::White, StaminaDebugString);
	
	if (!GCBaseCharacterMovementComponent->IsSprinting())
	{
		RestoreStamina(DeltaSeconds);
	} else
	{
		SprintConsumeStamina(DeltaSeconds);
	}
}

void AGCBaseCharacter::TryChangeSprintState()
{
	if (bIsSprintRequested && !GCBaseCharacterMovementComponent->IsSprinting() && CanSprint())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, "Successfully try change state");
		GCBaseCharacterMovementComponent->StartSprint();
	}

	if (
		(!bIsSprintRequested || FMath::IsNearlyZero(CurrentStamina, 0.01f))
		&& GCBaseCharacterMovementComponent->IsSprinting())
	{
		GCBaseCharacterMovementComponent->StopSprint();
	}
}

void AGCBaseCharacter::ChangeCrouchState()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	} else
	{
		Crouch();
	}
}

void AGCBaseCharacter::StartSprint()
{
	//const FString StaminaDebugString = FString::Printf(TEXT("Current stamina: %f"), CurrentStamina);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Start sprint");
	
	bIsSprintRequested = true;
	if (bIsCrouched)
	{
		UnCrouch();
	}
}

void AGCBaseCharacter::StopSprint()
{
	bIsSprintRequested = false;
}

bool AGCBaseCharacter::CanSprint()
{
	if (bIsOutOfStamina)
	{
		return false;
	}

	return true;
}

float AGCBaseCharacter::GetIKOffsetForASocket(const FName& SocketName)
{
	float Result = 0.0f;
	
	FVector SocketLocation = GetMesh()->GetSocketLocation(SocketName);
	FVector TraceStart(SocketLocation.X, SocketLocation.Y, GetActorLocation().Z);
	FVector TraceEnd = TraceStart - IKTraceDistance * FVector::UpVector;
	FVector ExtendedTraceEnd = TraceEnd - IKTraceExtendDistance * FVector::UpVector;

	FHitResult HitResult;
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_Visibility);

	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStart, TraceEnd, TraceType, true,
		TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, HitResult, true))
	{
		Result = (TraceEnd.Z - HitResult.Location.Z) / IKScale;
	}
	else if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceEnd, ExtendedTraceEnd, TraceType, true,
		TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, HitResult, true))
	{
		Result = (TraceEnd.Z - HitResult.Location.Z) /IKScale;
	}
	
	return Result;
}

void AGCBaseCharacter::RestoreStamina(float DeltaSeconds)
{
	if (CurrentStamina < MaxStamina)
	{
		CurrentStamina += StaminaRestoreRate * DeltaSeconds;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.0f, MaxStamina);
		if (FMath::IsNearlyEqual(CurrentStamina, MaxStamina, 0.01f))
		{
			bIsOutOfStamina = false;
		}
	}
}

void AGCBaseCharacter::SprintConsumeStamina(float DeltaSeconds)
{
	if (CurrentStamina > 0.0f)
	{
		CurrentStamina -= SprintStaminaConsumptionRate * DeltaSeconds;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.0f, MaxStamina);
		if (FMath::IsNearlyZero(CurrentStamina, 0.01f))
		{
			StopSprint();
			bIsOutOfStamina = true;
		}
	}
}
