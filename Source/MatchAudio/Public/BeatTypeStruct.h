#pragma once

#include "BeatTypeStruct.generated.h"

USTRUCT(BlueprintType)
struct FBeatTypeStruct
{
	GENERATED_BODY()

	int32 Type = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1", ClampMax = "20", UIMin = "1", UIMax = "20"))
	int32 ScorePoints = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* Material = nullptr;
};