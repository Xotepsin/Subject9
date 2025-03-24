#include "BaseBallGameModeBase.h"
#include "BaseBallBlueprintFunctionLibrary.h"
#include "GameFramework/PlayerController.h"
#include "BaseBallGameState.h"
#include "Engine/Engine.h"

ABaseBallGameModeBase::ABaseBallGameModeBase()
{
    SecretNumber = UBaseBallBlueprintFunctionLibrary::GenerateSecretNumber();
    AttemptsHost = 0;
    AttemptsGuest = 0;
    HostPlayer = nullptr;
    GuestPlayer = nullptr;
}


void ABaseBallGameModeBase::ProcessChatMessage(APlayerController* Sender, const FString& Message, const FString& UserID)
{
    // 채팅 메시지가 "/"로 시작하지 않으면 무시
    if (!Message.StartsWith("/"))
    {
        return;
    }

    // 플레이어 역할 결정
    EPlayerRole PlayerRole = GetPlayerRole(Sender, UserID);
    if (PlayerRole == EPlayerRole::Unknown)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("알 수 없는 플레이어입니다."));
        }
        return;
    }

    // 채팅 메시지에서 추측 숫자 파싱
    TArray<int32> GuessNumbers;
    if (!UBaseBallBlueprintFunctionLibrary::ParseChatGuess(Message, GuessNumbers))
    {
        // 유효하지 않은 입력이면 해당 플레이어의 시도 증가 후 OUT 처리
        if (PlayerRole == EPlayerRole::Host)
        {
            AttemptsHost++;
        }
        else if (PlayerRole == EPlayerRole::Guest)
        {
            AttemptsGuest++;
        }

        if (GEngine)
        {
            FString ErrorMsg = FString::Printf(TEXT("플레이어 %s: 잘못된 입력입니다. (OUT 처리)"),
                (PlayerRole == EPlayerRole::Host ? TEXT("Host") : TEXT("Guest")));
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ErrorMsg);
        }
    }
    else
    {
        // 유효한 추측인 경우 S/B 판정
        int32 Strikes = 0;
        int32 Balls = 0;
        UBaseBallBlueprintFunctionLibrary::CheckGuess(SecretNumber, GuessNumbers, Strikes, Balls);

        FString ResultMsg;
        if (Strikes == 0 && Balls == 0)
        {
            ResultMsg = "OUT";
        }
        else
        {
            ResultMsg = FString::Printf(TEXT("%dS%dB"), Strikes, Balls);
        }

        if (GEngine)
        {
            FString PlayerMsg = FString::Printf(TEXT("플레이어 %s: %s -> %s"),
                (PlayerRole == EPlayerRole::Host ? TEXT("Host") : TEXT("Guest")), *Message, *ResultMsg);
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, PlayerMsg);
        }

        // 시도 횟수 증가
        if (PlayerRole == EPlayerRole::Host)
        {
            AttemptsHost++;
        }
        else if (PlayerRole == EPlayerRole::Guest)
        {
            AttemptsGuest++;
        }

        // 3S(3스트라이크)인 경우 즉시 승리 처리
        if (Strikes == 3)
        {
            if (GEngine)
            {
                FString WinMsg = FString::Printf(TEXT("플레이어 %s 승리!! 다시 게임이 시작되었습니다."),
                    (PlayerRole == EPlayerRole::Host ? TEXT("Host") : TEXT("Guest")));
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, WinMsg);

                if (PlayerRole == EPlayerRole::Host)
                {
                    ABaseBallGameState* MyGameState = GetGameState<ABaseBallGameState>();
                    if (MyGameState)
                    {
                        MyGameState->AddHostScore();
                    }
                }
                else
                {
                    ABaseBallGameState* MyGameState = GetGameState<ABaseBallGameState>();
                    if (MyGameState)
                    {
                        MyGameState->AddGuestScore();
                    }
                }
            }
            ResetGame();
            return;
        }
    }

    // 각 플레이어가 3번의 시도 내에 승리하지 못한 경우 처리
    if (AttemptsHost >= MaxAttempts && AttemptsGuest < MaxAttempts)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Host가 3번 시도 실패. Guest 승리!! 게임이 리셋됩니다."));
        }
        ResetGame();
        return;
    }
    else if (AttemptsGuest >= MaxAttempts && AttemptsHost < MaxAttempts)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Guest가 3번 시도 실패. Host 승리!! 게임이 리셋됩니다."));
        }
        ResetGame();
        return;
    }
    else if (AttemptsHost >= MaxAttempts && AttemptsGuest >= MaxAttempts)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("무승부군. 다시 게임이 시작됩니다."));
        }
        ResetGame();
        return;
    }
}

void ABaseBallGameModeBase::ResetGame()
{
    SecretNumber = UBaseBallBlueprintFunctionLibrary::GenerateSecretNumber();
    AttemptsHost = 0;
    AttemptsGuest = 0;
    HostPlayer = nullptr;
    GuestPlayer = nullptr;

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("게임이 리셋되었습니다."));
    }
}

EPlayerRole ABaseBallGameModeBase::GetPlayerRole(APlayerController* Sender, const FString& UserID)
{
    UE_LOG(LogTemp, Warning, TEXT("GetPlayerRole called. UserID='%s'"), *UserID);
    if (UserID == TEXT("Host"))
    {
        HostPlayer = Sender;
        return EPlayerRole::Host;
    }
    if (Sender == HostPlayer.Get())
    {
        return EPlayerRole::Host;
    }
    if (UserID == TEXT("Guest"))
    {
        GuestPlayer = Sender;
        return EPlayerRole::Guest;
    }
    if (Sender == GuestPlayer.Get())
    {
        return EPlayerRole::Guest;
    }
    return EPlayerRole::Unknown;
}

