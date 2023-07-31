// Fill out your copyright notice in the Description page of Project Settings.


#include "CubesProceduralMeshComponent.h"



void UCubesProceduralMeshComponent::AddCubeInfo(const FVector& CenterLocation, const float Extents, const int32 CubeIndex)
{
	AddVertices(CenterLocation, Extents);
	AddTriangles(CubeIndex);
}

void UCubesProceduralMeshComponent::AddVertices(const FVector& CenterLocation, const float Extents)
{
	const float ExtentsNegative = Extents * -1;
	auto AddVertices = [&](int32 V1, int32 V2, int32 V3) {
		Vertices.Add(CenterLocation + FVector(V1, V2, V3));
		Colors.Add(FColor::Green);
		normals.Add(FVector(1, 1, 0));
		UV0.Add(FVector2D(0, 1));


	};

	AddVertices(ExtentsNegative, ExtentsNegative, ExtentsNegative); //lower left - 0
	AddVertices(ExtentsNegative, ExtentsNegative, Extents); //upper left - 1
	AddVertices(ExtentsNegative, Extents, ExtentsNegative); //lower right - 2 
	AddVertices(ExtentsNegative, Extents, Extents); //upper right - 3

	AddVertices(Extents, ExtentsNegative, ExtentsNegative); //lower front left - 4
	AddVertices(Extents, ExtentsNegative, Extents); //upper front left - 5
	AddVertices(Extents, Extents, Extents); //upper front right - 6
	AddVertices(Extents, Extents, ExtentsNegative); //lower front right - 7
}

void UCubesProceduralMeshComponent::AddTriangles(const int32 CubeIndex)
{
	auto AddTriangles = [&](int32 V1, int32 V2, int32 V3) { 
		Triangles.Add(V1);
		Triangles.Add(V2);
		Triangles.Add(V3);
	};
	int32 TriangleIndex = CubeIndex * 8;
	//Back face
	AddTriangles(TriangleIndex, TriangleIndex + 2, TriangleIndex + 3);
	AddTriangles(TriangleIndex + 3, TriangleIndex + 1, TriangleIndex + 0);

	//Left face 
	AddTriangles(TriangleIndex + 0, TriangleIndex + 1, TriangleIndex + 4);
	AddTriangles(TriangleIndex + 4, TriangleIndex + 1, TriangleIndex + 5);

	//Front face 
	AddTriangles(TriangleIndex + 4, TriangleIndex + 5, TriangleIndex + 7);
	AddTriangles(TriangleIndex + 7, TriangleIndex + 5, TriangleIndex + 6);

	//Right face 
	AddTriangles(TriangleIndex + 7, TriangleIndex + 6, TriangleIndex + 3);
	AddTriangles(TriangleIndex + 3, TriangleIndex + 2, TriangleIndex + 7);

	//Top face
	AddTriangles(TriangleIndex + 1, TriangleIndex + 3, TriangleIndex + 5);
	AddTriangles(TriangleIndex + 6, TriangleIndex + 5, TriangleIndex + 3);

	//bottom face
	AddTriangles(TriangleIndex + 2, TriangleIndex + 0, TriangleIndex + 4);
	AddTriangles(TriangleIndex + 4, TriangleIndex + 7, TriangleIndex + 2);

}


void UCubesProceduralMeshComponent::GenerateCubes()
{
	CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), Colors, TArray<FProcMeshTangent>(), false);
}

void UCubesProceduralMeshComponent::TranslateCube(const int32 CubeIndex, const FVector& TranslationVector)
{
	for (int32 i = CubeIndex * 8; i < CubeIndex * 8 + 8; i++)
	{
		Vertices[i] += TranslationVector;
	}
	UpdateMeshSection(0, Vertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>());
}

void UCubesProceduralMeshComponent::RemoveCube(const int32 CubeIndex)
{
	for (int32 i = CubeIndex * 8; i < CubeIndex * 8 + 8; i++)
	{
		Vertices[i] = FVector();
	}

	UpdateMeshSection(0, Vertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>());
}

void UCubesProceduralMeshComponent::UpdateColor(const int32 CubeIndex, const FColor& NewColor)
{
	for (int32 i = CubeIndex * 8; i < CubeIndex * 8 + 8; i++)
	{
		Colors[i] = NewColor;
	}

	UpdateMeshSection(0, Vertices, TArray<FVector>(), TArray<FVector2D>(), Colors, TArray<FProcMeshTangent>());
}