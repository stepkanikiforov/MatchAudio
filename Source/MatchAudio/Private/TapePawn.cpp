// Fill out your copyright notice in the Description page of Project Settings.


#include "TapePawn.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BeatActor.h"
#include "MatchTable.h"

ATapePawn::ATapePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	MoveableComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MoveableComponent"));
	MoveableComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->AttachToComponent(MoveableComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Body->CastShadow = false;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ATapePawn::BeginOverlap);
}

void ATapePawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
}

void ATapePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerController)
	{
		PlayerController->GetInputTouchState(ETouchIndex::Touch1, FinishtLocation.X, FinishtLocation.Y, IsTouch);
		if (IsTouch)
		{
			float Delta = FinishtLocation.X - StartLocation.X;
			if (Delta != 0)
			{
				FVector Location = MoveableComponent->GetRelativeLocation();
				Location.Y += Delta * DeltaTime * SpeedMovement;
				if (Location.Y >= LocalBorder)
				{
					Location.Y = LocalBorder;
				}
				if (Location.Y <= -LocalBorder)
				{
					Location.Y = -LocalBorder;
				}
				MoveableComponent->SetRelativeLocation(Location);
			}
			StartLocation = FinishtLocation;

		}
	}
}

void ATapePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Input", IE_Pressed, this, &ATapePawn::TouchPressed);
}

void ATapePawn::TouchPressed()
{
	if (PlayerController)
	{
		PlayerController->GetInputTouchState(ETouchIndex::Touch1, StartLocation.X, StartLocation.Y, IsTouch);
	}
}

void ATapePawn::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABeatActor* Beat = Cast<ABeatActor>(OtherActor);
		if (Beat && MatchTable)
		{
			MatchTable->PlaceTile(Beat->NumTrack, Beat->Type);
			Beat->Destroy();
		}
	}
}

