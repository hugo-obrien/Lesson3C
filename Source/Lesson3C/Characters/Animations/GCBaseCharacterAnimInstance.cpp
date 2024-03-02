// Fill out your copyright notice in the Description page of Project Settings.


#include "GCBaseCharacterAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Lesson3C/Characters/GCBaseCharacter.h"
#include "Lesson3C/Components/Movement/GCBaseCharacterMovementComponent.h"

void UGCBaseCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<AGCBaseCharacter>(),
		TEXT("UGCBaseCharacterAnimInstance::NativeBeginPlay() UGCBaseCharacterAnimInstance can be used only with AGCBaseCharacter"));
	CachedBaseCharacter = StaticCast<AGCBaseCharacter*>(TryGetPawnOwner());
}

void UGCBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!CachedBaseCharacter.IsValid())
	{
		return;
	}

	UGCBaseCharacterMovementComponent* CharacterMovement = CachedBaseCharacter->GetBaseCharacterMovementComponent();

	Speed = CharacterMovement->Velocity.Size();
	bIsFalling = CharacterMovement->IsFalling();
	bIsCrouching = CharacterMovement->IsCrouching();
	bIsSprinting = CharacterMovement->IsSprinting();

	RightFootEffectorLocation = FVector(CachedBaseCharacter->GetIKRightFootOffset(),0.0f, 0.0f);
	LeftFootEffectorLocation = FVector(-CachedBaseCharacter->GetIKLeftFootOffset(),0.0f, 0.0f);
}
