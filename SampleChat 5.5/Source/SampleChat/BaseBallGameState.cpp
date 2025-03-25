#include "BaseBallGameState.h"
#include "Net/UnrealNetwork.h"

ABaseBallGameState::ABaseBallGameState()
{
	CurrentStrikes = 0;
	CurrentBalls = 0;
	CurrentHostScore = 0;
	CurrentGuestScore = 0;
	CurrentGuestAttempts = 0;
	CurrentHostAttempts = 0;
}


int32 ABaseBallGameState::GetStrikes() const
{
	return CurrentStrikes;
}

int32 ABaseBallGameState::GetBalls() const
{
	return CurrentBalls;
}

int32 ABaseBallGameState::GetHostScore() const
{
	return CurrentHostScore;
}

int32 ABaseBallGameState::GetGuestScore() const
{
	return CurrentGuestScore;
}

void ABaseBallGameState::SetStrike(int32 Strike)
{
	CurrentStrikes = Strike;
}

void ABaseBallGameState::SetBall(int32 Balls)
{
	CurrentBalls = Balls;
}

void ABaseBallGameState::SetHostAttempts(int32 Score)
{
	CurrentHostAttempts = Score;
}

void ABaseBallGameState::SetGuestAttempts(int32 Score)
{
	CurrentGuestAttempts = Score;
}


void ABaseBallGameState::AddHostScore()
{
	CurrentHostScore++;
}

void ABaseBallGameState::AddGuestScore()
{
	CurrentGuestScore++;
}

void ABaseBallGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseBallGameState, CurrentStrikes);
	DOREPLIFETIME(ABaseBallGameState, CurrentBalls);
	DOREPLIFETIME(ABaseBallGameState, CurrentHostScore);
	DOREPLIFETIME(ABaseBallGameState, CurrentGuestScore);
	DOREPLIFETIME(ABaseBallGameState, CurrentGuestAttempts);
	DOREPLIFETIME(ABaseBallGameState, CurrentHostAttempts);
}