// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCodeBasePawn.h"

#include "Components/SphereComponent.h"
#include "Lesson3C/Components/Movement/GCBasePawnMovementComponent.h"

// Sets default values
AGameCodeBasePawn::AGameCodeBasePawn()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComponent->SetSphereRadius(50.0f);
	CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	RootComponent = CollisionComponent;

	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UGCBasePawnMovementComponent>(TEXT("Movement component"));
	MovementComponent->SetUpdatedComponent(CollisionComponent);
}

// Called to bind functionality to input
void AGameCodeBasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGameCodeBasePawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGameCodeBasePawn::MoveRight);
}

void AGameCodeBasePawn::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AGameCodeBasePawn::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

