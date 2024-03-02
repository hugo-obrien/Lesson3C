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

void AGCBaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	IKRightFootOffset = FMath::FInterpTo(IKRightFootOffset, GetIKOffsetForASocket(RightFootSocketName), DeltaSeconds, IKInterpSpeed);
	IKLeftFootOffset = FMath::FInterpTo(IKLeftFootOffset, GetIKOffsetForASocket(LeftFootSocketName), DeltaSeconds, IKInterpSpeed);
	
	TryChangeSprintState();
}

void AGCBaseCharacter::TryChangeSprintState()
{
	if (bIsSprintRequested && !GCBaseCharacterMovementComponent->IsSprinting() && CanSprint())
	{
		GCBaseCharacterMovementComponent->StartSprint();
	}

	if (!bIsSprintRequested && GCBaseCharacterMovementComponent->IsSprinting())
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
