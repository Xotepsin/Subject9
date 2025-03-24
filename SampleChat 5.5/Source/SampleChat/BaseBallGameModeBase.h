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

    // 플레이어 컨트롤러 (첫 번째 접속: Host, 두 번째: Guest)
    TWeakObjectPtr<APlayerController> HostPlayer;
    TWeakObjectPtr<APlayerController> GuestPlayer;
};
