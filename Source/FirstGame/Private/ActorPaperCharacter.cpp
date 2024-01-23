// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPaperCharacter.h"

AActorPaperCharacter::AActorPaperCharacter()
{ 
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));

	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);

}

void AActorPaperCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController=Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			//Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}

	}
}


void AActorPaperCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{

		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AActorPaperCharacter::JumpInputMove);
		Input->BindAction(WalkAction, ETriggerEvent::Triggered, this, &AActorPaperCharacter::EnhanceInputMove);


	}
}

void AActorPaperCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateCharacter();
}

void AActorPaperCharacter::UpdateCharacter()
{
	UpdateAnimation();
	const FVector PlayerVelocity = GetVelocity();
	float TravelDirection = PlayerVelocity.X;
	if (Controller!=nullptr)
	{
		if (TravelDirection<0.0F)
		{
			Controller->ClientSetRotation(FRotator(0.0f, 180.0f, 0.0f));

			
			
		}
		else if (TravelDirection > 0.0F)
		{
			Controller->ClientSetRotation(FRotator(0.0f, 0.0f, 0.0f));

			
		}

	}
}

void AActorPaperCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedsSqr = PlayerVelocity.SizeSquared();

	if (GetCharacterMovement()->IsFalling())
	{
		UPaperFlipbook* DesiredFlipbook = (PlayerVelocity.Z < 0.0f) ? FallAnime : JumpAnime;

		if (GetSprite()->GetFlipbook() != DesiredFlipbook)
		{
			GetSprite()->SetFlipbook(DesiredFlipbook);
		}
	}
	else {
		UPaperFlipbook* DesiredFlipbook = (PlayerSpeedsSqr > 0.0f) ? RunningAnime : IdleAnime;

		if (GetSprite()->GetFlipbook() != DesiredFlipbook)
		{
			GetSprite()->SetFlipbook(DesiredFlipbook);
		}

	}
}

void AActorPaperCharacter::EnhanceInputMove(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Log, TEXT("Hello world!"));
	const 	FVector2D MoveVector = value.Get<FVector2D>();

	if (MoveVector.X > 0.05f || MoveVector.X < -0.05f)
	{
		AddMovementInput(FVector(1.0f,0.0f,0.0f), MoveVector.X);
	}


}

void AActorPaperCharacter::JumpInputMove(const FInputActionValue& value)
{
	Jump();
}

