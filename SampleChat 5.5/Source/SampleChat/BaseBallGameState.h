#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BaseBallGameState.generated.h"

UCLASS()
class SAMPLECHAT_API ABaseBallGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ABaseBallGameState();

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball")
	int32 CurrentStrikes;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball")
	int32 CurrentBalls;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball")
	int32 CurrentHostScore;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball")
	int32 CurrentGuestScore;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball")
	int32 CurrentGuestAttempts;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball")
	int32 CurrentHostAttempts;

	int32 GetStrikes() const;
	int32 GetBalls() const;
	int32 GetHostScore() const;
	int32 GetGuestScore() const;
	void SetStrike(int32 Strike);
	void SetBall(int32 Balls);
	void SetHostAttempts(int32 Score);
	void SetGuestAttempts(int32 Score);
	void AddHostScore();
	void AddGuestScore();

};
