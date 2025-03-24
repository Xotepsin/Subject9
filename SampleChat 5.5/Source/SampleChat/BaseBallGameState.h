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
	int32 CurrentOuts;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball")
	int32 CurrentHostScore;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Baseball")
	int32 CurrentGuestScore;

	UFUNCTION(BlueprintPure, Category = "Baseball")
	int32 GetOuts() const;
	UFUNCTION(BlueprintPure, Category = "Baseball")
	int32 GetStrikes() const;
	UFUNCTION(BlueprintPure, Category = "Baseball")
	int32 GetBalls() const;
	UFUNCTION(BlueprintPure, Category = "Baseball")
	int32 GetHostScore() const;
	UFUNCTION(BlueprintPure, Category = "Baseball")
	int32 GetGuestScore() const;
	void SetStrike(int32 Strike);
	void SetBall(int32 Balls);
	void SetOut(int32 Outs);
	void AddHostScore();
	void AddGuestScore();
};
