// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameMode.h"
#include "SGameState.h"
#include "SPlayerState.h"
#include "TimerManager.h"
#include "SHealtComponent.h"
#include "Engine/EngineTypes.h"

/**TODO 
	if(HasAuthority)
		GetGameInstance();
		GameInstance->StartSession();
**/
ASGameMode::ASGameMode()
{
	TimeBetweenWaves = 2.0f;

	WaveCount = 0;

	GameStateClass = ASGameState::StaticClass();
	PlayerStateClass = ASPlayerState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;

	bUseSeamlessTravel = true;

	bGameOver = false;

}

void ASGameMode::StartPlay()
{
	Super::StartPlay();
}

void ASGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!bGameOver)
	{
		CheckWaveState();
		CheckAnyPlayerAlive();
	}
}

void ASGameMode::StartWave()
{
	WaveCount++;
	NrOfBotPerWave = 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ASGameMode::SpawnBotTimerElapsed, 1.0f, true, 1.0f);

	SetWaveState(EWaveState::WaveInProgress);
}

//void ASGameMode::PostLogin(APlayerController * NewPlayer)
//{
//	Super::PostLogin(NewPlayer);
//	++NumberOfPlayers;
//
//	if (NumberOfPlayers = 1)
//	{
//		/*UWorld* World = GetWorld();
//		if (!ensure(World)) return;
//
//		bUseSeamlessTravel = true;*/
//
//		PrepareForNextWave();
//
//		UE_LOG(LogTemp, Warning, TEXT("Reached minimun amount of players"));
//	}
//}
//
//void ASGameMode::Logout(AController * Exiting)
//{
//	Super::Logout(Exiting);
//	--NumberOfPlayers;
//}

void ASGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();
	NrOfBotPerWave--;
	if (NrOfBotPerWave <= 0)
	{
		EndWave();
	}
}

void ASGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);

	SetWaveState(EWaveState::WaitingToComplete);
	
}

void ASGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASGameMode::StartWave, TimeBetweenWaves, false);

	SetWaveState(EWaveState::WaitingToStart);

	RespawnDeadPlayers();

}

void ASGameMode::CheckWaveState()
{
	bool bIsPreperingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);

	if (NrOfBotPerWave > 0 || bIsPreperingForWave)
	{
		return;
	}

	bool bIsAnyBotAlive = false;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();
		if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
		{
			continue;
		}

		USHealtComponent* HealthComp = Cast<USHealtComponent>(TestPawn->GetComponentByClass(USHealtComponent::StaticClass()));
		if (HealthComp && HealthComp->GetHealth() > 0.0f)
		{
			bIsAnyBotAlive = true;
			break;
		}
	}

	if (!bIsAnyBotAlive)
	{
		SetWaveState(EWaveState::WaveComplete);
		PrepareForNextWave();
	}
}

void ASGameMode::GameOver()
{
	EndWave();
	bGameOver = true;
	SetWaveState(EWaveState::GameOver);
	GetWorldTimerManager().SetTimer(TimerHandle_DeadTime, this, &ASGameMode::RestartGame, 0.1f, false, 5.0f);
}

void ASGameMode::SetWaveState(EWaveState NewState)
{
	ASGameState* GS = GetGameState<ASGameState>();
	if (ensureAlways(GS))
	{
		GS->SetWaveState(NewState);
	}
}

void ASGameMode::RespawnDeadPlayers()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC)
		{
			APawn* Pawn = PC->GetPawn();
			if (PC && PC->GetPawn() == nullptr)
			{
				RestartPlayer(PC);
			}
		}
	}

	PrimaryActorTick.bCanEverTick = true;
	SetWaveState(EWaveState::WaitingToStart);
}

void ASGameMode::KillRemainingBots()
{
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();
		if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
		{
			continue;
		}
		USHealtComponent* HealthComp = Cast<USHealtComponent>(TestPawn->GetComponentByClass(USHealtComponent::StaticClass()));
		HealthComp->OnHealthChanged.Broadcast(HealthComp, 0.0f, 100.0f, nullptr, nullptr, nullptr);
	}
}

void ASGameMode::RestartGame()
{
	WaveCount = 0;
	KillRemainingBots();
	RespawnDeadPlayers();
	bGameOver = false;
	StartWave();
}

void ASGameMode::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			APawn* Pawn = PC->GetPawn();
			USHealtComponent* HealthComp = Cast<USHealtComponent>(Pawn->GetComponentByClass(USHealtComponent::StaticClass()));
			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)
			{
				return;
			}
		}
	}
	GameOver();
}





