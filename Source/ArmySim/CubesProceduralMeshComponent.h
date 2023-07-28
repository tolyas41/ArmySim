// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "CubesProceduralMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARMYSIM_API UCubesProceduralMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
	
public:
	void GenerateCube(const FVector& CenterLocation, const float Extents, const int32 CubeIndex);

	void TranslateCube(const int32 CubeIndex, const FVector& TranslationVector);
};
