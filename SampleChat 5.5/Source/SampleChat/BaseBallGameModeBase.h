#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseBallGameModeBase.generated.h"

UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
    Host,
    Guest,
    Unknown
};

// 게임 턴
UENUM(BlueprintType)
enum class EGameTurn : uint8
{
    HostTurn UMETA(DisplayName = "Host Turn"),
    GuestTurn UMETA(DisplayName = "Guest Turn")
};

UCLASS()
class SAMPLECHAT_API ABaseBallGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ABaseBallGameModeBase();
/**
 * 채팅 메시지 처리 함수
 * Sender : 메시지를 보낸 플레이어 컨트롤러
 * Message : 채팅 메시지 (예: "/123")
 */
    UFUNCTION(BlueprintCallable, Category = "NumberBaseball")
    void ProcessChatMessage(APlayerController* Sender, const FString& Message, const FString& UserID);

    // 턴 제어 관련
    UPROPERTY(BlueprintReadOnly, Category = "Turn")
    EGameTurn CurrentTurn;

    // 턴 제한 시간 (초 단위)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn")
    float TurnTimeLimit;


    // 타이머 핸들
    FTimerHandle TurnTimerHandle;

    // 턴 전환 함수
    UFUNCTION(BlueprintCallable, Category = "Turn")
    void NextTurn();

    // 제한 시간 만료 시 호출되는 함수
    UFUNCTION()
    void OnTurnTimeExpired();

    UFUNCTION()
    void UpdateTurnTimer();
	void BeginPlay() override;

    UPROPERTY()
    bool bTurnChanged;
	
	
protected:
    // 게임 상태 리셋 (비밀 숫자 재생성, 시도 카운트 초기화 등)
    void ResetGame();

    // 플레이어의 역할(Host/Guest) 결정 함수
    EPlayerRole GetPlayerRole(APlayerController* Sender, const FString& UserID);

    // 게임 상태 변수
    TArray<int32> SecretNumber;
    int32 AttemptsHost;
    int32 AttemptsGuest;
    const int32 MaxAttempts = 3;
    bool bHostOut;
	bool bGuestOut;

    // 플레이어 컨트롤러 (첫 번째 접속: Host, 두 번째: Guest)
    TWeakObjectPtr<APlayerController> HostPlayer;
    TWeakObjectPtr<APlayerController> GuestPlayer;
};
