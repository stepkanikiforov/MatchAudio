// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboActor.h"
#include "Components/WidgetComponent.h"

// Sets default values
AComboActor::AComboActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	ComboWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ComboWidget"));
	ComboWidget->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AComboActor::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(2.0f);
}

// Called every frame
void AComboActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AComboActor::ShowCombo(int32 ComboToShow, int32 TypeToShow)
{
	DelegateCombo.Broadcast(ComboToShow, TypeToShow);
}
