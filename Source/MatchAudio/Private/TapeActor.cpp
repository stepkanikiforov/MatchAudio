// Fill out your copyright notice in the Description page of Project Settings.


#include "TapeActor.h"

// Sets default values
ATapeActor::ATapeActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Body->CastShadow = false;
}

// Called when the game starts or when spawned
void ATapeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATapeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

