// Fill out your copyright notice in the Description page of Project Settings.


#include "BeatActor.h"
#include "Components/BoxComponent.h"

ABeatActor::ABeatActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Body->CastShadow = false;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABeatActor::BeginOverlap);
}

void ABeatActor::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(5.0f);
}

void ABeatActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Direction = this->GetActorForwardVector();
	FVector NewLocation = GetActorLocation() + (Direction * SpeedMovement * DeltaTime);
	SetActorLocation(NewLocation);
}

void ABeatActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
