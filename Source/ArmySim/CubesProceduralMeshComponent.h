// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "CubesProceduralMeshComponent.generated.h"


UCLASS()
class ARMYSIM_API UCubesProceduralMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
	
public:
	void AddCubeInfo(const FVector& CenterLocation, const float Extents, const int32 CubeIndex);
	void GenerateCubes();

	void TranslateCube(const int32 CubeIndex, const FVector& TranslationVector);
	void RemoveCube(const int32 CubeIndex);

private:
	TArray<FVector> Vertices;
	TArray<int32> Triangles;

	void AddVertices(const FVector& CenterLocation, const float Extents);
	void AddTriangles(const int32 CubeIndex);
};
