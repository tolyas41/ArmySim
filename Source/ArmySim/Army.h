// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Army.generated.h"

class UCubesProceduralMeshComponent;

USTRUCT(BlueprintType)
struct FSoldierInfo
{
	GENERATED_BODY()

	int32 CurrentHP;
	int32 MeshSection;
	float CurrentLocationX;
	int32 CurrentRow;
	bool bDead = false;
	bool bMoving = false;

	FSoldierInfo()
		: CurrentHP(10)
		, MeshSection(0)
		, CurrentLocationX(0.f)
		, CurrentRow(0)
	{}

	FSoldierInfo(int32 newCurrentHP, int32 newMeshSection, float newCurrentLocationX, int32 newCurrentRow)
		: CurrentHP(newCurrentHP)
		, MeshSection(newMeshSection)
		, CurrentLocationX(newCurrentLocationX)
		, CurrentRow(newCurrentRow)
	{}
};

UCLASS()
class ARMYSIM_API AArmy : public AActor
{
	GENERATED_BODY()
	
public:	
	AArmy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 1, ClampMax = 100, UIMin = 1, UIMax = 100))
	int32 Rows = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 1, ClampMax = 100, UIMin = 1, UIMax = 100))
	int32 Columns = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Units = "Percent", ClampMin = 0, ClampMax = 100, UIMin = 0, UIMax = 100))
	float DamageChance = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DamageToSoldier_Min = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DamageToSoldier_Max = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Units = "Seconds", ClampMin = 0, ClampMax = 15, UIMin = 0, UIMax = 15))
	float ApplyDamageRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Units = "Percent", ClampMin = 0, ClampMax = 100, UIMin = 0, UIMax = 100))
	float ReplacementSpeed = 5.f;

	UFUNCTION()
	void ApplyDamageToSoldiers();

	UFUNCTION(BlueprintCallable)
	void GenerateArmy();

private:
	TArray<FSoldierInfo> Soldiers;
	const int32 SoldierMaxHP = 10;
	const float SoldierDistance = 100.f;
	const float SoldierCubeExtents = 25.f;

	FTimerHandle ApplyDamageTH;

	double TimeToMoveSoldiers;
	int32 SoldiersMoving;

	UPROPERTY()
	TObjectPtr<UCubesProceduralMeshComponent> ArmyMesh;

	void MoveSoldiers(float DeltaTime);
};
