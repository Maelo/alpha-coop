// Copyright Epic Games, Inc. All Rights Reserved.

#include "ReplicationGameMode.h"
#include "ReplicationCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"

AReplicationGameMode::AReplicationGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	TimeBetweenWaves = 2.0f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.f;
}

void AReplicationGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}

void AReplicationGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();
}

void AReplicationGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	NbOfEnemyToSpawn--;

	if(NbOfEnemyToSpawn <= 0)
	{
		EndWave();
	}
}

void AReplicationGameMode::StartWave()
{
	WaveCount++;
	
	NbOfEnemyToSpawn = 2 * WaveCount;
	
	GetWorldTimerManager().SetTimer(TimerHandle_EnemySpawner, this, &AReplicationGameMode::SpawnBotTimerElapsed, 1.f, true, 0.f);
}

void AReplicationGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_EnemySpawner);

	//PrepareForNextWave();
}

void AReplicationGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &AReplicationGameMode::StartWave, TimeBetweenWaves,false);
}

void AReplicationGameMode::CheckWaveState()
{
	bool bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);

	if(NbOfEnemyToSpawn || bIsPreparingForWave)
	{
		return;
	}
	
	bool bIsAnyEnemyAlive = false;
	
	for(FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();

		if(TestPawn == nullptr || TestPawn->IsPlayerControlled())
		{
			continue;
		}
		
		bIsAnyEnemyAlive = true;
		break;
	}

	if(!bIsAnyEnemyAlive)
	{
		PrepareForNextWave();
	}
}
