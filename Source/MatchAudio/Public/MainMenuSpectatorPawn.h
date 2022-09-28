// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainMenuSpectatorPawn.generated.h"

class UCameraComponent;
class UWidgetInteractionComponent;

UCLASS()
class MATCHAUDIO_API AMainMenuSpectatorPawn : public APawn
{
	GENERATED_BODY()

public:
	AMainMenuSpectatorPawn();

	USceneComponent* Root = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCameraComponent* Camera = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UWidgetInteractionComponent* Interaction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpeedMovement = 30;
	
	void SetBorder(float BorderToSet);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void TouchPressed();
	bool IsTouch = false;
	APlayerController* PlayerController = nullptr;
	FVector2D StartLocation = FVector2D(0, 0);
	FVector2D FinishtLocation = FVector2D(0, 0);
	float BeginZ = 0;
	float Border = 0;

};
