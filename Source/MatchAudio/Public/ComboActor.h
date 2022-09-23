// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ComboActor.generated.h"

class UWidgetComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegateCombo, int32, Combo, int32, Type);

UCLASS()
class MATCHAUDIO_API AComboActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AComboActor();

	USceneComponent* Root = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UWidgetComponent* ComboWidget = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Type = -1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Combo = -1;

	UPROPERTY(BlueprintAssignable)
	FDelegateCombo DelegateCombo;

	void ShowCombo(int32 ComboToShow, int32 TypeToShow);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
