#include "BaseBallGameState.h"
#include "Net/UnrealNetwork.h"

ABaseBallGameState::ABaseBallGameState()
{
	CurrentStrikes = 0;
	CurrentBalls = 0;
	CurrentOuts = 0;
	CurrentHostScore = 0;
	CurrentGuestScore = 0;
}

int32 ABaseBallGameState::GetOuts() const
{
	return CurrentOuts;
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

void ABaseBallGameState::SetOut(int32 Outs)
{
	CurrentOuts = Outs;
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
	DOREPLIFETIME(ABaseBallGameState, CurrentOuts);
	DOREPLIFETIME(ABaseBallGameState, CurrentHostScore);
	DOREPLIFETIME(ABaseBallGameState, CurrentGuestScore);
}