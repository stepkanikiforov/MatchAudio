// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuSpectatorPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Kismet/GameplayStatics.h"

AMainMenuSpectatorPawn::AMainMenuSpectatorPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	Interaction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Interaction"));
	Interaction->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

}

void AMainMenuSpectatorPawn::BeginPlay()
{
	Super::BeginPlay();

	BeginZ = GetActorLocation().Z;
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
}

void AMainMenuSpectatorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerController)
	{
		PlayerController->GetInputTouchState(ETouchIndex::Touch1, FinishtLocation.X, FinishtLocation.Y, IsTouch);
		if (IsTouch && Border > 0)
		{
			float Delta = FinishtLocation.Y - StartLocation.Y;
			if (Delta != 0)
			{
				FVector Location = GetActorLocation();
				Location.Z += Delta * DeltaTime * SpeedMovement;
				if (Location.Z < BeginZ && Location.Z > -(Border + BeginZ))
				{
					SetActorLocation(Location);
				}
			}
			StartLocation = FinishtLocation;
		}
	}
}

void AMainMenuSpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Input", IE_Pressed, this, &AMainMenuSpectatorPawn::TouchPressed);
}

void AMainMenuSpectatorPawn::TouchPressed()
{
	if (PlayerController)
	{
		PlayerController->GetInputTouchState(ETouchIndex::Touch1, StartLocation.X, StartLocation.Y, IsTouch);
	}
}

void AMainMenuSpectatorPawn::SetBorder(float BorderToSet)
{
	Border = BorderToSet;
}
