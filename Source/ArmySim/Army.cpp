// Fill out your copyright notice in the Description page of Project Settings.


#include "Army.h"
#include "CubesProceduralMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AArmy::AArmy()
{
	PrimaryActorTick.bCanEverTick = true;

	ArmyMesh = CreateDefaultSubobject<UCubesProceduralMeshComponent>("ArmyMesh");
	SetRootComponent(ArmyMesh);
}

void AArmy::BeginPlay()
{
	Super::BeginPlay();

	//tick starts in GenerateArmy()
	SetActorTickEnabled(false);
}

void AArmy::GenerateArmy()
{
	double startSeconds = FPlatformTime::Seconds();
	//generate cubes-soldiers (procedural static mesh)
	for (int32 i = 0; i < Rows; i++)
	{
		for (int32 j = 0; j < Columns; j++)
		{
			FVector Location(i * SoldierDistance * -1, j * SoldierDistance * -1, 0.f); 
			int32 SoldierID = Columns * i + j;
			Soldiers.Add(FSoldierInfo(SoldierMaxHP, SoldierID, Location.X, i));
			ArmyMesh->GenerateCube(Location, SoldierCubeExtents, SoldierID);
		}
	}

	double secondsElapsed = FPlatformTime::Seconds() - startSeconds;
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Elapsed time for /GenerateArmy/ : %f"), secondsElapsed));

	if (Soldiers.Num() > 0 && DamageChance > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(ApplyDamageTH, this, &AArmy::ApplyDamageToSoldiers, ApplyDamageRate, true);
	}

	//calculate actual speed of replacement based on distance between soldiers
	ReplacementSpeed = SoldierDistance * ReplacementSpeed / 100.f;

	SetActorTickEnabled(true);
}

void AArmy::ApplyDamageToSoldiers()
{
	double startSeconds = FPlatformTime::Seconds();

	int32 DeadSoldiers = 0;
	int32 AliveSoldiers = 0; //for processing time info
	int32 index = -1;
	for (auto& Soldier : Soldiers)
	{
		index++;
		if (Soldier.bDead)
		{
			continue;
		}

		AliveSoldiers++;
		//check chance to deal damage
		if (DamageChance / 100 >= FMath::FRandRange(0.0f, 1.0f))
		{
			int32 Damage = FMath::FRandRange(DamageToSoldier_Min, DamageToSoldier_Max);
			if (Soldier.CurrentHP <= Damage)
			{
				Soldier.bDead = true;
				Soldier.bMoving = false;
				ArmyMesh->ClearMeshSection(Soldier.MeshSection);
				//tell soldiers down in column to move up
				for (int32 i = index + Columns; i < Soldiers.Num(); i += Columns)
				{
					Soldiers[i].bMoving = true;
					Soldiers[i].CurrentRow -= 1;
				}
				AliveSoldiers--;
				DeadSoldiers++;
			}
			else
			{
				Soldier.CurrentHP -= Damage;
			}
		}
	}
	//stop sim if no soldiers are alive
	if (AliveSoldiers == 0)
	{
		UGameplayStatics::OpenLevel(this, FName("L_Main"));
	}

	double secondsElapsed = FPlatformTime::Seconds() - startSeconds;
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Yellow, FString::Printf(TEXT("Soldiers alive : %i  Soldiers dead : %i   ///   Elapsed time for Damaging Soldiers : %f"), AliveSoldiers, DeadSoldiers, secondsElapsed));
}

void AArmy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalOffset(FVector(MovementSpeed, 0.f, 0.f));

	MoveSoldiers(DeltaTime);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("Soldiers moving : %i   // average time to move : %f"), SoldiersMoving, TimeToMoveSoldiers));
}

void AArmy::MoveSoldiers(float DeltaTime)
{
	double startSeconds = FPlatformTime::Seconds();

	SoldiersMoving = 0;
	float EndLocationX;
	for (auto& Soldier : Soldiers)
	{
		//skip soldiers without bMoving flag
		if (!Soldier.bMoving)
		{
			continue;
		}
		SoldiersMoving++;
		//move soldier
		ArmyMesh->TranslateCube(Soldier.MeshSection, FVector(ReplacementSpeed, 0.f, 0.f));
		Soldier.CurrentLocationX += ReplacementSpeed;

		//detect if soldier achieve destination
		EndLocationX = Soldier.CurrentRow * SoldierDistance * -1;
		if (FMath::IsNearlyEqual(EndLocationX, Soldier.CurrentLocationX, ReplacementSpeed + 1.f))
		{
			Soldier.bMoving = false;
		}
	}

	double secondsElapsed = FPlatformTime::Seconds() - startSeconds;
	TimeToMoveSoldiers = secondsElapsed;
}