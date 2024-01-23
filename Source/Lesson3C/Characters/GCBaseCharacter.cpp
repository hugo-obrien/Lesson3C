// Fill out your copyright notice in the Description page of Project Settings.


#include "GCBaseCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

void AGCBaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsSprintRequested && !bIsSprinting && CanSprint())
	{
		bIsSprinting = true;
		DefaultMaxMovementSpeed = GetCharacterMovement()->MaxWalkSpeed;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}

	if (!bIsSprintRequested && bIsSprinting)
	{
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = DefaultMaxMovementSpeed;
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
}

void AGCBaseCharacter::StopSprint()
{
	bIsSprintRequested = false;
}

bool AGCBaseCharacter::CanSprint()
{
	return true;
}
