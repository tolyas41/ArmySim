// Fill out your copyright notice in the Description page of Project Settings.


#include "CubesProceduralMeshComponent.h"

TArray<int32> CreateTriangles()
{
	TArray<int32> Triangles;
	auto AddTriangles = [&Triangles](int32 V1, int32 V2, int32 V3) { 
		Triangles.Add(V1);
		Triangles.Add(V2);
		Triangles.Add(V3);
	};

	//Back face
	AddTriangles(0, 2, 3);
	AddTriangles(3, 1, 0);

	//Left face 
	AddTriangles(0, 1, 4);
	AddTriangles(4, 1, 5);

	//Front face 
	AddTriangles(4, 5, 7);
	AddTriangles(7, 5, 6);

	//Right face 
	AddTriangles(7, 6, 3);
	AddTriangles(3, 2, 7);

	//Top face
	AddTriangles(1, 3, 5);
	AddTriangles(6, 5, 3);

	//bottom face
	AddTriangles(2, 0, 4);
	AddTriangles(4, 7, 2);

	return Triangles;
}

void UCubesProceduralMeshComponent::GenerateCube(const FVector& CenterLocation, const float Extents, const int32 CubeIndex)
{
	const float ExtentsNegative = Extents * -1;
	TArray<FVector> Vertices;
	auto AddVertices = [&](int32 V1, int32 V2, int32 V3) {
		Vertices.Add(CenterLocation + FVector(V1, V2, V3));
	};

	AddVertices(ExtentsNegative, ExtentsNegative, ExtentsNegative); //lower left - 0
	AddVertices(ExtentsNegative, ExtentsNegative, Extents); //upper left - 1
	AddVertices(ExtentsNegative, Extents, ExtentsNegative); //lower right - 2 
	AddVertices(ExtentsNegative, Extents, Extents); //upper right - 3

	AddVertices(Extents, ExtentsNegative, ExtentsNegative); //lower front left - 4
	AddVertices(Extents, ExtentsNegative, Extents); //upper front left - 5
	AddVertices(Extents, Extents, Extents); //upper front right - 6
	AddVertices(Extents, Extents, ExtentsNegative); //lower front right - 7

	CreateMeshSection(CubeIndex, Vertices, CreateTriangles(), TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);
}

void UCubesProceduralMeshComponent::TranslateCube(const int32 CubeIndex, const FVector& TranslationVector)
{
	FProcMeshSection* MeshSection = GetProcMeshSection(CubeIndex);
	if (MeshSection && MeshSection->ProcVertexBuffer.Num() > 0)
	{
		TArray<FVector> UpdatedVertices;
		for (FProcMeshVertex MeshVertex : MeshSection->ProcVertexBuffer)
		{
			//add all vertex positions TranslationVector
			UpdatedVertices.Add(MeshVertex.Position + TranslationVector);
		}

		UpdateMeshSection(CubeIndex, UpdatedVertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>());
	}
}

