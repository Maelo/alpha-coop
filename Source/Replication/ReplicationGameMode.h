// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ReplicationGameMode.generated.h"

UCLASS(minimalapi)
class AReplicationGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	FTimerHandle TimerHandle_EnemySpawner;

	FTimerHandle TimerHandle_NextWaveStart;

	int32 NbOfEnemyToSpawn;

	int32 WaveCount;

	UPROPERTY(EditDefaultsOnly, Category="GameMode")
	float TimeBetweenWaves;


	
public:
	AReplicationGameMode();

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	
protected:

	UFUNCTION(BlueprintImplementableEvent, Category="GameMode")
    void SpawnNewBot();


	void SpawnBotTimerElapsed();
    
    void StartWave();
    
    void EndWave();
    
    void PrepareForNextWave();

	void CheckWaveState();

};



