// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include"Camera/CameraComponent.h"
#include "EnhancedInputLibrary.h"
#include "Math/Vector2D.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/Rotator.h"
#include"InputAction.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ActorPaperCharacter.generated.h"



/**
 * 
 */
UCLASS()
class FIRSTGAME_API AActorPaperCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:

	AActorPaperCharacter();

	void BeginPlay() override;
	

	UPROPERTY(Category = Character,EditAnywhere)
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(Category = Character,EditAnywhere)
	UCameraComponent* CameraComponent;
	
	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
	class UInputMappingContext* InputMappingContext;
	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
	class UInputAction* WalkAction;
	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
	class UInputAction* JumpAction;
	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
	class UInputAction* AttackAction;


	/// animation stuffs

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* IdleAnime;
	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* RunningAnime;
	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* FallAnime;
	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* JumpAnime;


	void EnhanceInputMove(const FInputActionValue&value);
	void JumpInputMove(const FInputActionValue& value);
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void Tick(float DeltaSeconds) override;
	void UpdateCharacter();
	void UpdateAnimation();
	





	
};
