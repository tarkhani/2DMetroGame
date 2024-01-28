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
		Input->BindAction(SitAction, ETriggerEvent::Triggered, this, &AActorPaperCharacter::SitDashInputMove);
		Input->BindAction(ReleaseSitAction, ETriggerEvent::Triggered, this, &AActorPaperCharacter::CancelSitDashInputMove);
		


	}
}

void AActorPaperCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	DeltaTimeBetweenDash += 1;
	if (DeltaTimeBetweenDash>30)
	{
		CanDash = true;
	}
	StopAnimationUpdate();
	UpdateCharacter();
}


void AActorPaperCharacter::UpdateCharacter()
{
	if (updateAnime==true)
	{
		UpdateAnimation();
	}
	
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
	const float playerSpeed = GetVelocity().Size();
	const float MaxPlayerSpeed = GetCharacterMovement()->GetMaxSpeed();

	if (GetCharacterMovement()->IsFalling())
	{
		UPaperFlipbook* DesiredFlipbook = (PlayerVelocity.Z < 0.0f) ? FallAnime : JumpAnime;

		if (GetSprite()->GetFlipbook() != DesiredFlipbook)
		{
			GetSprite()->SetFlipbook(DesiredFlipbook);
			CurrentFlipbook = DesiredFlipbook;
			
			
		}
	}
	else {
		UPaperFlipbook* DesiredFlipbook = (PlayerSpeedsSqr > 0.0f) ? RunningAnime : IdleAnime;
		FString PlayerSpeedsSqrText = FString::Printf(TEXT("%f"), PlayerVelocity.Size());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, PlayerSpeedsSqrText);

		if ((PlayerSpeedsSqr == 0.0f) && (isSiting == true)) {

			 DesiredFlipbook = SitAnime;

		}
		else if ((playerSpeed ==MaxPlayerSpeed) && (isSiting == true) && (CanDash == true))
		{
			DesiredFlipbook = DashAnime;
			CanDash = false;
			
			
			GetSprite()->SetRelativeLocation(FVector(0.0f, 0.0f, 44.0f));
			Crouch();
			DeltaTimeBetweenDash = 0;

			
		}

		if (GetSprite()->GetFlipbook() != DesiredFlipbook)
		{
			GetSprite()->SetFlipbook(DesiredFlipbook);
			CurrentFlipbook = DesiredFlipbook;
		
		}

	}
}

void AActorPaperCharacter::StopAnimationUpdate()
{
	if (CurrentFlipbook == DashAnime) {
		const int32 PlaybackPositionInFrames = GetSprite()->GetPlaybackPositionInFrames();
		const int32 FlipbookLengthInFrames = GetSprite()->GetFlipbookLengthInFrames();
		updateAnime = false;
		if (PlaybackPositionInFrames == FlipbookLengthInFrames - 1 ) {

			updateAnime = true;
			UnCrouch();
			GetSprite()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
			
			
		}
		
		
	}
}

void AActorPaperCharacter::MakeCanDashTrue()
{
	if (true)
	{
		GetSprite()->OnFinishedPlaying.AddDynamic(this, &AActorPaperCharacter::FinishedPlaying);
	}
	

}

void AActorPaperCharacter::FinishedPlaying()
{
	UnCrouch();
}

void AActorPaperCharacter::EnhanceInputMove(const FInputActionValue& value)
{
	
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

void AActorPaperCharacter::SitDashInputMove(const FInputActionValue& value)
{
	isSiting=true;
	
	
}

void AActorPaperCharacter::CancelSitDashInputMove(const FInputActionValue& value)
{
	isSiting = false;
}

