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
    bHostOut = false;
    bGuestOut = false;
    // 기본 턴은 Host로 설정
    CurrentTurn = EGameTurn::HostTurn;
    // 턴 제한 시간
    TurnTimeLimit = 30.0f;

    HostPlayer = nullptr;
    GuestPlayer = nullptr;
}


void ABaseBallGameModeBase::ProcessChatMessage(APlayerController* Sender, const FString& Message, const FString& UserID)
{
    // "/"로 시작하지 않으면 무시
    if (!Message.StartsWith("/"))
    {
        return;
    }

    // 각 플레이어의 실패 상태 처리
    if (AttemptsHost >= MaxAttempts && AttemptsGuest < MaxAttempts)
    {
        bHostOut = true;
    }
    if (AttemptsGuest >= MaxAttempts && AttemptsHost < MaxAttempts)
    {
        bGuestOut = true;
    }

    // 플레이어 역할 결정
    EPlayerRole PlayerRole = GetPlayerRole(Sender, UserID);
    if (PlayerRole == EPlayerRole::Unknown)
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("알 수 없는 플레이어입니다."));
        return;
    }

    // 턴 체크: 현재 턴과 일치하지 않으면 입력 무시
    if ((CurrentTurn == EGameTurn::HostTurn && PlayerRole != EPlayerRole::Host) ||
        (CurrentTurn == EGameTurn::GuestTurn && PlayerRole != EPlayerRole::Guest))
    {
        if (GEngine)
        {
            FString TurnMsg = FString::Printf(TEXT("아직 당신의 턴이 아닙니다. 현재 턴: %s"),
                (CurrentTurn == EGameTurn::HostTurn ? TEXT("Host") : TEXT("Guest")));
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TurnMsg);
        }
        return;
    }

    // 이미 실패한 플레이어는 입력 무시
    if ((PlayerRole == EPlayerRole::Host && bHostOut) ||
        (PlayerRole == EPlayerRole::Guest && bGuestOut))
    {
        if (GEngine)
        {
            FString OutMsg = (PlayerRole == EPlayerRole::Host) ? TEXT("Host는 더 이상 입력할 수 없습니다.") : TEXT("Guest는 더 이상 입력할 수 없습니다.");
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, OutMsg);
        }
        return;
    }

    // 채팅 메시지 파싱
    TArray<int32> GuessNumbers;
    bool bValidInput = UBaseBallBlueprintFunctionLibrary::ParseChatGuess(Message, GuessNumbers);
    ABaseBallGameState* MyGameState = GetGameState<ABaseBallGameState>();

    // 파싱 실패: OUT 처리 및 승리 처리 후 게임 리셋
    if (!bValidInput)
    {
        if (GEngine)
        {
            FString ErrorMsg = FString::Printf(TEXT("플레이어 %s: 잘못된 입력입니다. (OUT 처리)"),
                (PlayerRole == EPlayerRole::Host ? TEXT("Host") : TEXT("Guest")));
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ErrorMsg);
        }
        if (PlayerRole == EPlayerRole::Host)
        {
            AttemptsHost++;
            if (MyGameState)
            {
                MyGameState->AddGuestScore();
            }
        }
        else // Guest
        {
            AttemptsGuest++;
            if (MyGameState)
            {
                MyGameState->AddHostScore();
            }
        }
        ResetGame();

        return;
    }
    else
    {
        if (MyGameState)
        {
            FString Combined;
            for (int32 Digit : GuessNumbers)
            {
                Combined.AppendInt(Digit);
            }
            MyGameState->RecentNumber = Combined;
        }
        // 유효한 입력: S/B 판정
        int32 Strikes = 0, Balls = 0;
        UBaseBallBlueprintFunctionLibrary::CheckGuess(SecretNumber, GuessNumbers, Strikes, Balls);
        FString ResultMsg = (Strikes == 0 && Balls == 0) ? TEXT("OUT") : FString::Printf(TEXT("%dS%dB"), Strikes, Balls);

        if (MyGameState)
        {
            MyGameState->SetBall(Balls);
            MyGameState->SetStrike(Strikes);
        }

        if (GEngine)
        {
            FString PlayerMsg = FString::Printf(TEXT("플레이어 %s: %s -> %s"),
                (PlayerRole == EPlayerRole::Host ? TEXT("Host") : TEXT("Guest")), *Message, *ResultMsg);
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, PlayerMsg);
        }

        // 만약 결과가 OUT이면 처리 (여기서는 OUT 처리 후 바로 리셋)
        if (Strikes == 0 && Balls == 0)
        {
            if (PlayerRole == EPlayerRole::Host)
            {
                if (MyGameState)
                {
                    MyGameState->AddGuestScore();
                }
            }
            else
            {
                if (MyGameState)
                {
                    MyGameState->AddHostScore();
                }
            }
            ResetGame();
            return;
        }
        else if (Strikes == 3) // 3S 승리 조건
        {
            if (GEngine)
            {
                FString WinMsg = FString::Printf(TEXT("플레이어 %s 승리!! 다시 게임이 시작되었습니다."),
                    (PlayerRole == EPlayerRole::Host ? TEXT("Host") : TEXT("Guest")));
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, WinMsg);
            }
            if (PlayerRole == EPlayerRole::Host)
            {
                AttemptsHost++;
                if (MyGameState)
                {
                    MyGameState->AddHostScore();
                }
            }
            else
            {
                AttemptsGuest++;
                if (MyGameState)
                {
                    MyGameState->AddGuestScore();
                }
            }
            ResetGame();
            return;
        }
        else
        {
            // 유효한 추측 결과이지만 승리나 OUT 조건에 해당하지 않는 경우
            if (PlayerRole == EPlayerRole::Host)
            {
                AttemptsHost++;
                if (MyGameState)
                {
                    MyGameState->SetHostAttempts(AttemptsHost);
                }
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Host 횟수 증가"));
                }
                // 입력 처리로 턴 전환 발생: 타이머 클리어 후 NextTurn() 호출
                GetWorldTimerManager().ClearTimer(TurnTimerHandle);
                NextTurn();
            }
            else
            {
                AttemptsGuest++;
                if (MyGameState)
                {
                    MyGameState->SetGuestAttempts(AttemptsGuest);
                }
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Guest 횟수 증가"));
                }
                GetWorldTimerManager().ClearTimer(TurnTimerHandle);
                NextTurn();
            }
        }
    }

    // 두 플레이어 모두 3회 소진 시 무승부 처리
    if (AttemptsHost >= MaxAttempts && AttemptsGuest >= MaxAttempts)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("무승부군. 다시 게임이 리셋됩니다."));
        }
        ResetGame();
        return;
    }

}

void ABaseBallGameModeBase::NextTurn()
{
    UE_LOG(LogTemp, Warning, TEXT("NextTurn() 시작: 이전 턴 = %s"),
        (CurrentTurn == EGameTurn::HostTurn ? TEXT("Host") : TEXT("Guest")));

    // 턴 전환: Host -> Guest, Guest -> Host
    if (CurrentTurn == EGameTurn::HostTurn)
    {
        CurrentTurn = EGameTurn::GuestTurn;
    }
    else
    {
        CurrentTurn = EGameTurn::HostTurn;
    }
    bTurnChanged = true; // 턴 전환이 입력에 의해 발생했음을 표시

    ABaseBallGameState* MyGameState = GetGameState<ABaseBallGameState>();
    if (MyGameState)
    {
        MyGameState->TurnTimerRemaining = TurnTimeLimit;
    }

    // 타이머 재설정 (Clear 후, 반복 호출)
    GetWorldTimerManager().ClearTimer(TurnTimerHandle);
    GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ABaseBallGameModeBase::UpdateTurnTimer, 1.0f, true);

    UE_LOG(LogTemp, Warning, TEXT("NextTurn() 종료: 새 턴 = %s"),
        (CurrentTurn == EGameTurn::HostTurn ? TEXT("Host") : TEXT("Guest")));
    if (GEngine)
    {
        FString TurnMsg = FString::Printf(TEXT("턴이 전환되었습니다. 현재 턴: %s"),
            (CurrentTurn == EGameTurn::HostTurn ? TEXT("Host") : TEXT("Guest")));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TurnMsg);
    }
}

void ABaseBallGameModeBase::OnTurnTimeExpired()
{
    // 만약 턴 전환이 이미 입력으로 발생했다면, 타이머는 그냥 플래그를 초기화하고 종료
    if (bTurnChanged)
    {
        bTurnChanged = false;
        return;
    }

    ABaseBallGameState* MyGameState = GetGameState<ABaseBallGameState>();
    if (MyGameState)
    {
        MyGameState->TurnTimerRemaining = TurnTimeLimit;
    }
    // 현재 턴 플레이어가 입력하지 않았으므로, 기회를 소진
    if (CurrentTurn == EGameTurn::HostTurn)
    {
        AttemptsHost++;
        if (MyGameState)
        {
            MyGameState->SetHostAttempts(AttemptsHost);
        }
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Host 입력 제한 시간 만료. Host 기회 소진."));
        }
    }
    else // Guest 턴
    {
        AttemptsGuest++;
        if (MyGameState)
        {
            MyGameState->SetGuestAttempts(AttemptsGuest);
        }
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Guest 입력 제한 시간 만료. Guest 기회 소진."));
        }
    }

    // 무승부 처리
    if (AttemptsHost >= MaxAttempts && AttemptsGuest >= MaxAttempts)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("무승부입니다. 게임이 리셋됩니다."));
        }
        ResetGame();
        return;
    }

    NextTurn();
}

void ABaseBallGameModeBase::UpdateTurnTimer()
{
    // GameState를 가져와서 업데이트
    ABaseBallGameState* MyGameState = GetGameState<ABaseBallGameState>();
    if (MyGameState)
    {
        if (MyGameState->TurnTimerRemaining > 0.f)
        {
            MyGameState->TurnTimerRemaining -= 1.f;
        }
        else
        {
            // 시간이 만료되면 OnTurnTimeExpired 호출 (예: 턴 전환)
            OnTurnTimeExpired();
        }
    }
}

void ABaseBallGameModeBase::BeginPlay()
{
	Super::BeginPlay();
    ABaseBallGameState* MyGameState = GetGameState<ABaseBallGameState>();
    if (MyGameState)
    {
        MyGameState->TurnTimerRemaining = TurnTimeLimit;
    }
    // 턴 타이머 시작
    GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ABaseBallGameModeBase::UpdateTurnTimer, 1.0f, true);
}

void ABaseBallGameModeBase::ResetGame()
{
    // 새 게임 시작 시, 턴을 HostTurn으로 설정
    CurrentTurn = EGameTurn::HostTurn;
	// 비밀 숫자 재생성
    SecretNumber = UBaseBallBlueprintFunctionLibrary::GenerateSecretNumber();
    ABaseBallGameState* MyGameState = GetGameState<ABaseBallGameState>();
	if (MyGameState)
	{
		int32 hostScore = MyGameState->GetHostScore();
		int32 guestScore = MyGameState->GetGuestScore();
		MyGameState->TurnTimerRemaining = TurnTimeLimit;
		MyGameState->SetHostAttempts(0);
		MyGameState->SetGuestAttempts(0);   
        UE_LOG(LogTemp, Warning, TEXT("Host Score : '%d', Guest Score : '%d'"), hostScore, guestScore);
	}
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
    GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ABaseBallGameModeBase::UpdateTurnTimer, 1.0f, true);
    AttemptsHost = 0;
    AttemptsGuest = 0;
    bHostOut = false;
    bGuestOut = false;
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

