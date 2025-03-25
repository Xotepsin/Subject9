# [9번 과제] 숫자 야구 게임
---

## 1. 발제

![](https://velog.velcdn.com/images/xotepsin/post/2a9bdf45-10a0-4b4f-9d63-7fe29058d800/image.png)

## 1️⃣ **Intro :  숫자 야구 게임을 만들어 봅시다.**

비디오 게임의 할아버지의 할아버지로 만들어진지 100년이 넘는 숫자 야구 게임을 만들어 봅시다. 원조 게임은 4자리 숫자로 하는 Bulls and Cows 입니다.

규칙은 간단합니다.

- 처음 접속하면 서버가 1~9까지 겹치지 않는 3자리의 숫자를 만듭니다.
- Host 와 Guest 는 차례대로 “/123”, “/543” 와 같이 겹치지 않는 3자리 숫자를 답합니다.
- 서버가 생성한 랜덤한 숫자와 사용자가 입력한 숫자를 비교하여 결과를 반환합니다.
    - 자리수와 값이 같으면 스트라이크(S) 숫자를 늘립니다.
    - 자리수는 다르고 값이 같으면 볼(B) 숫자를 늘립니다.
    - 예 1. 서버 생성 숫자 386, 사용자 답변 127 ⇒ OUT
    - 예 2. 서버 생성 숫자 386, 사용자 답변 167 ⇒ 0S1B
    - 예 3. 서버 생성 숫자 386, 사용자 답변 367 ⇒ 1S1B
    - 예 4. 서버 생성 숫자 386, 사용자 답변 396 ⇒ 2S0B
    - 예 4. 서버 생성 숫자 386, 사용자 답변 386 ⇒ 3S0B ⇒ Win
- 한쪽 플레이어가 3번 만에 3S를 못하면, 남은 플레이어가 Winner가 됩니다.
- 3S 를 먼저 맞추는 플레이어가 Winner가 됩니다.
- Winner가 나오면 게임은 리셋됩니다. (출력메시지는 “Host Won!! 다시 게임이 시작됐다.” 또는 “Guest Won!! 다시 게임이 시작됐다.”)
- 둘 다 3번만에 못 맞추면 게임은 리셋됩니다. (출력메시지는 “무승부군. 다시 게임을 시작하지”)

로직이 조금 복잡해서 BP로 하기에는 굉장히 길어집니다만, 이러한 로직을 C++로 작성하면 길지 않은 크기로 작성할 수 있습니다. 실무환경에서 자주 만나는 문제로 Blueprint의 복잡성을 단순화 시키고, 게임 성능을 높이기 위해 Blueprint로 만들어진 거대한 코드를 C++ 라이브러리로 재작성하는 흔한 시나리오입니다.

## 2️⃣ 개발 프로세스 가이드

### **1 단계 : 코드 준비**

1. 강의 챕터 2에서 작성한 채팅 예제를 로딩합니다.
2. 강의 챕터 2에서 C++ 블루프린트 라이브러리 생성하는 방법으로 C++ Library를 추가합니다.

### **2단계 : Rule 숙지하기**

1. 각 플레이어에게 3번의 기회가 있습니다.
2. “/124”과 같이 “/” 가 있으면 응답 패턴으로 간주합니다.
3. 아웃조건
    1. “/”다음에는 3자리의 중복이 없는 1~9까지의 숫자가 중복있음.
    2. “/”다음에 3자리가 아님
4. 무승부조건
    1. 플레이어 2명 모두 3번의 기회를 소모하고도 3S(trike)를 하지 못했을 때
5. 승리조건
    1. 먼저 3번의 기회 안에 3S(trike)를 한 플레이어 승
    2. 다른 플레이어가 아웃된 경우 남은 플레이어 승
6. 승리 또는 무승부의 경우 게임이 리셋

### **3단계 :  코드 작성하기**

## 3️⃣ 필수 기능 가이드

### 1. 필수 BP와 C++ 라이브러리 구현

- **GameMode 서버 로직**
    - **채팅 기반**으로 명령어를 입력하면 서버가 이를 해석해서 게임 로직을 처리해야 합니다.
    - 예: “/123” 형태로 입력되었을 때, 게임 진행 상황(스트라이크/볼 계산, 시도 횟수, 승리/패배/무승부)을 판정합니다.
- **3자리 난수 생성 로직 (중복 없는 1~9 숫자)**
    - 게임 시작 시 서버에서 무작위 3자리 숫자를 만듭니다.
    - 예: `386`, `594` 등(0은 포함하지 않음, 중복 없음).
- **판정 로직 (S/B/OUT 계산)**
    - 서버가 생성한 숫자와 플레이어가 입력한 숫자를 비교하여 스트라이크(S), 볼(B), 아웃(OUT)을 계산합니다.
    - 자리수와 값이 같으면 S, 자리수는 다르지만 값이 존재하면 B, 둘 다 아니면 OUT.
- **시도 횟수 및 상태 관리**
    - 플레이어(Host/Guest) 각각 3번의 기회를 가집니다.
    - 유효하지 않은 입력을 했을 때(예: 중복 숫자 포함, 3자리가 아닌 입력 등)는 아웃(OUT) 처리.
    - 입력할 때마다 플레이어별 남은 시도 횟수를 갱신합니다.
- **승리, 무승부, 게임 리셋**
    - 3번 내에 3S를 맞춘 플레이어가 나오면 즉시 승리 처리.
    - 플레이어가 아웃되었을 때, 남은 플레이어가 자동으로 승리.
    - 두 플레이어 모두 3번씩 시도를 끝내도 승자가 없으면 무승부 처리.
    - 승리 혹은 무승부 시, 게임을 재시작(숫자 및 시도 횟수 리셋).
- **C++ 라이브러리 분리**
    - **판정**(S/B 계산)과 **난수 생성** 로직은 별도의 C++ 라이브러리(또는 C++ Class)로 분리해서 구현합니다.
    - GameMode에서는 이 라이브러리를 호출하여 결과를 받아온 후 게임 전체 흐름(채팅 처리, 시도 횟수, 승패 결정 등)을 관리합니다.

## 4️⃣ 도전 기능 가이드

- **턴(Turn) 제어 기능**
    - 고스톱이나 하스스톤처럼 특정 턴이 끝나야 다른 플레이어가 입력할 수 있게 만들어주세요.
    - 혹은, 일정 시간이 지나면 턴이 자동으로 넘어가도록 구현하셔도 좋습니다.
- **게임성 고도화**
    - 제한 시간(Timer)을 두고, 시간 내에 입력을 못 하면 기회를 소진하게 만들어보세요.
    - 누가 몇 번의 승리를 가져갔는지 스코어보드를 보여주는 기능을 추가해주세요.
    
    
## 2. 파일 구성
---
우선, 기본적으로 온라인 강의에 있는 `SimpleChat` 프로젝트를 기반으로 확장하여 구현하였습니다.
### 블루프린트 파일
<div style="display: flex; justify-content: space-between; align-items: center;">
  <img src="https://velog.velcdn.com/images/xotepsin/post/8f1b104a-03ad-45e6-9ad8-278cec29845e/image.png" style="width: 100px; margin-right: 10px;" />
  <img src="https://velog.velcdn.com/images/xotepsin/post/701472a1-5a69-4dd7-a794-801ee7cf5064/image.png" style="width: 100px; margin-right: 10px;" />
  <img src="https://velog.velcdn.com/images/xotepsin/post/3eb381e1-5063-441e-bb65-114294a2d75c/image.png" style="width: 100px; margin-right: 10px;" />
  <img src="https://velog.velcdn.com/images/xotepsin/post/8e784040-4394-4640-bb7f-767410fba09a/image.png" style="width: 100px;" />
</div>

우선 기본적으로 있는 `BP_Controller`, `LvChat`, `Widget_ChatWindow`그대로 사용하였고, 기존에있던 `BP_ChatMode` 대신 `BP_BaseBallGameMode`를 C++기반으로 새로 생성하여 블루프린트를 생성하였습니다.
### C++ 파일
![](https://velog.velcdn.com/images/xotepsin/post/430c1ba5-3834-4215-8eac-8bad6b1e08b9/image.png)
우선 필수기능에 C++ 라이브러리 함수 분리를위하여 `BaseBallBlueprintFunctionLibrary`
를 생성하였고 `BP_ChatMode`를 대신하기 위해 `BaseBallGameMode`를 C++파일로 생성, 그리고 UI와 바인딩시키기위하여 `BaseBallGameState`를 생성하였습니다.

## 3. 필수 기능
---

```cpp
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BaseBallBlueprintFunctionLibrary.generated.h"

UCLASS()
class SAMPLECHAT_API UBaseBallBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
 /**
 * 1부터 9까지의 숫자 중에서 중복 없이 3자리 숫자를 생성합니다.
 *
 * @return 생성된 3자리 숫자 배열
 */
    UFUNCTION(BlueprintCallable, Category = "NumberBaseball")
    static TArray<int32> GenerateSecretNumber();

    /**
     * 서버가 생성한 숫자와 플레이어의 추측을 비교하여 스트라이크와 볼의 개수를 계산합니다.
     *
     * @param SecretNumbers 서버가 생성한 비밀 숫자 배열
     * @param GuessNumbers 플레이어의 추측 숫자 배열
     * @param OutStrikes (출력) 자리와 숫자가 모두 일치하는 개수
     * @param OutBalls (출력) 숫자만 일치하는 개수
     */
    UFUNCTION(BlueprintCallable, Category = "NumberBaseball")
    static bool CheckGuess(const TArray<int32>& SecretNumbers, const TArray<int32>& GuessNumbers, int32& OutStrikes, int32& OutBalls);

    /**
     * 플레이어가 입력한 숫자 배열이 유효한지 검증합니다.
     * 유효 조건: 3자리, 중복 없음, 각 숫자는 1~9 사이.
     *
     * @param GuessNumbers 플레이어가 입력한 숫자 배열
     * @return 입력이 유효하면 true, 아니면 false
     */
    UFUNCTION(BlueprintCallable, Category = "NumberBaseball")
    static bool ValidateGuess(const TArray<int32>& GuessNumbers);

    /**
     * 채팅 메시지로부터 플레이어의 추측을 파싱합니다.
     * 유효한 형식: "/" 다음에 3자리 숫자 (예: "/124")
     *
     * @param ChatMessage 플레이어가 입력한 채팅 메시지
     * @param OutGuessNumbers (출력) 파싱된 숫자 배열
     * @return 파싱 및 유효성 검사 성공 시 true, 실패 시 false
     */
    UFUNCTION(BlueprintCallable, Category = "NumberBaseball")
    static bool ParseChatGuess(const FString& ChatMessage, TArray<int32>& OutGuessNumbers);
	
	
};

```
`BlueprintFunctionLibrary.h`

```cpp
#include "BaseBallBlueprintFunctionLibrary.h"

TArray<int32> UBaseBallBlueprintFunctionLibrary::GenerateSecretNumber()
{
    TArray<int32> NumberPool;
    // 1부터 9까지의 숫자 생성
    for (int32 i = 1; i <= 9; i++)
    {
        NumberPool.Add(i);
    }

    // Fisher-Yates 알고리즘을 이용해 배열 섞기
    int32 PoolSize = NumberPool.Num();
    for (int32 i = 0; i < PoolSize; i++)
    {
        int32 j = FMath::RandRange(i, PoolSize - 1);
        NumberPool.Swap(i, j);
    }

    // 처음 3개의 숫자를 선택
    TArray<int32> SecretNumbers;
    for (int32 i = 0; i < 3; i++)
    {
        SecretNumbers.Add(NumberPool[i]);
    }

	return SecretNumbers;
}

bool UBaseBallBlueprintFunctionLibrary::CheckGuess(const TArray<int32>& SecretNumbers, const TArray<int32>& GuessNumbers, int32& OutStrikes, int32& OutBalls)
{
    // 기본 출력값 초기화
    OutStrikes = 0;
    OutBalls = 0;

    // 배열의 길이가 정확히 3인지 검사 (게임 규칙상 3자리)
    if (SecretNumbers.Num() != 3 || GuessNumbers.Num() != 3)
    {
        UE_LOG(LogTemp, Error, TEXT("CheckGuess: 배열 길이가 올바르지 않습니다. SecretNumber.Num() = %d, GuessNumbers.Num() = %d"),
            SecretNumbers.Num(), GuessNumbers.Num());
        return false;
    }

    // 안전하게 3자리 숫자에 대해 스트라이크/볼 계산
    for (int32 i = 0; i < 3; i++)
    {
        // 배열 접근 전에 인덱스가 유효한지 (이미 위 조건에서 보장됨)
        if (SecretNumbers[i] == GuessNumbers[i])
        {
            OutStrikes++;
        }
        else if (SecretNumbers.Contains(GuessNumbers[i]))
        {
            OutBalls++;
        }
    }

    return true;
}

bool UBaseBallBlueprintFunctionLibrary::ValidateGuess(const TArray<int32>& GuessNumbers)
{
    // 3자리인지 확인
    if (GuessNumbers.Num() != 3)
    {
        return false;
    }

    TSet<int32> UniqueSet;
    for (int32 Digit : GuessNumbers)
    {
        // 각 숫자는 1부터 9까지여야 함
        if (Digit < 1 || Digit > 9)
        {
            return false;
        }
        UniqueSet.Add(Digit);
    }

    return (UniqueSet.Num() == 3);
}

bool UBaseBallBlueprintFunctionLibrary::ParseChatGuess(const FString& ChatMessage, TArray<int32>& OutGuessNumbers)
{
    // 채팅 메시지는 "/"로 시작해야 하며 전체 길이는 4여야 함 (예: "/124")
    if (!ChatMessage.StartsWith("/") || ChatMessage.Len() != 4)
    {
        return false;
    }

    OutGuessNumbers.Empty();
    // "/" 이후 3개의 문자를 파싱
    for (int32 i = 1; i < ChatMessage.Len(); i++)
    {
        TCHAR Char = ChatMessage[i];
        if (!FChar::IsDigit(Char))
        {
            return false;
        }
        // 단일 문자를 FString로 변환 후 정수로 변환
        FString DigitStr = FString::Chr(Char);
        int32 Digit = FCString::Atoi(*DigitStr);
        OutGuessNumbers.Add(Digit);
    }

    return ValidateGuess(OutGuessNumbers);
}
```
`BlueprintFunctionLibrary.cpp`
우선 BP함수라이브러리에는 서버에서 랜덤으로 생성할 `GenerateSecretNumber`함수와 서버가 생성한 숫자와 플레이어의 추측을 비교하여 스트라이크와 볼의 개수를 계산하는 함수인 `CheckGuess`함수, 입력받은 값이 정상적인 입력값인지 확인하는지 검증하는 `ValidateGuess`함수 그리고 채팅 메시지로부터 플레이어의 추측을 파싱하는 함수인`ParseChatGuess`함수가 있습니다.

이 BP함수라이브러리를 사용하여 `BaseBallGameMode`로직을 구현했습니다.
아래는 `BaseBallGameMode`코드입니다.
```cpp
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

```
`BaseBallGameModeBase.h`

우선 `BaseBallGameMode`에서는 전반적으로 숫자 야구 게임의 흐름이 구현되어있습니다.
```cpp
UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
    Host,
    Guest,
    Unknown
};
```
우선 `ENUM`형식으로 플레이어의 역할을 구분하기 위해 사용되는 열거형을 선언했습니다.

```cpp
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
```
생성자 부분입니다.

```cpp
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
```

플레이어의 역할(Host/Guest) 결정하는 함수입니다.

```cpp
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
```

플레이어가 `OUT`을 당하거나 다른 플레이어가 `3S`되거나 두플레이어 모두 3번의 기회에 못맞추어 무승부가 되었을때 호출되는 함수입니다.

```cpp
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
```

숫자 야구 게임의 진행을 담당하고 있는 함수입니다. 필수기능에 있는 모든 기능을 포함하고있습니다.
아래는 수정한 블루프린트 입니다.
우선 `BP_Controller`부분입니다.
![](https://velog.velcdn.com/images/xotepsin/post/6a297004-0e5b-49ad-9d1d-7e089db66b23/image.png)
기존에는 `OnLoginWithID` 커스텀 이벤트밖에없었는데 이러면 `Client`에서의 `UserID`의 정보가 갱신되지않아서 `Client_SetUserID` 커스텀 이벤트를 추가하여 해결하였습니다.
![](https://velog.velcdn.com/images/xotepsin/post/6ea7e16a-6c42-4e81-b49f-c6a92fb7de0e/image.png)
`BaseBallGameModeBase`에 있는 `ProcessChatMessage`함수를 호출하여 `UserID`와 `Msg`를 매개변수로 전달합니다.

![](https://velog.velcdn.com/images/xotepsin/post/a6fd4938-30fd-4d77-b079-baa12ba281b1/image.png)
기존에 있던 `BP_ChatMode`를 그대로 가져와서 `BP_BaseBallGameModeBase`에 사용하였습니다

## 4. 도전 기능

우선 도전 기능에 있는 턴 기능, 시간 타이머 기능, UI 스코어 기능 등등을 구현했습니다.
![](https://velog.velcdn.com/images/xotepsin/post/0ab0181f-e7d8-49ac-b109-15afc6d83c4d/image.png)
기존 `Widget_ChatWindow`입니다

![](https://velog.velcdn.com/images/xotepsin/post/391da71e-e5d9-46a7-b29f-233e0b60c460/image.png)

### 스코어 기능
수정한 `Widget_ChatWindow`입니다.
스코어와 타이머시간들을 바인딩하기 위해서 `BaseBallGameState`를 C++로 만들었습니다.
```cpp
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
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Turn")
	float TurnTimerRemaining;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "NumberBaseball")
	FString RecentNumber;

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

```
`BaseBallGameState.h`

바인딩해야할 변수들을 `UPROPERTY(Replicated)`로 선언해주었고 
```cpp
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
	DOREPLIFETIME(ABaseBallGameState, TurnTimerRemaining);
	DOREPLIFETIME(ABaseBallGameState, RecentNumber);
}
```
`BaseBallGameState.cpp`
`GameMode`에서 서버에서의 값을 가져오고 설정하기위해 Get함수와 Set함수를 구현하고
`#include "Net/UnrealNetwork.h"` 를 선언하고
`DOREPLIFETIME` 매크로를 사용하여 지정된 변수들이 복제 대상으로 등록했습니다.
예를 들어, `DOREPLIFETIME(ABaseBallGameState, CurrentStrikes);`는 `CurrentStrikes `변수가 서버에서 변경되면 클라이언트에 반영되도록 했습니다.
이렇게 설정하고 텍스트박스에서 
![](https://velog.velcdn.com/images/xotepsin/post/063812ca-5c42-41fe-8672-dccf1ff9d744/image.png)
이렇게 이벤트 그래프를 작성하여 반영했습니다. 다른 텍스트박스도 똑같이 이벤트 그래프를 작성해서 구현했기 때문에 대표로 Balls만 바인딩하는 것을 보여드리겠습니다.
![](https://velog.velcdn.com/images/xotepsin/post/0fd3b201-8dd8-499b-8a5e-b352acb431a5/image.png)
위에는 `Attempts` 즉 각 플레이어의 시도 횟수를 바인딩하는 블루프린트입니다.

### 턴 기능, 시간 기능
```cpp

// 게임 턴
UENUM(BlueprintType)
enum class EGameTurn : uint8
{
    HostTurn UMETA(DisplayName = "Host Turn"),
    GuestTurn UMETA(DisplayName = "Guest Turn")
};

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
```
`GameMode`헤더파일에 열거형으로 Turn을 구분하기 위해 선언해놓고 턴 제어 관련한 함수들과 변수들을 선언했습니다.

그리고 현재 남은 시간도 바인딩해야하기때문에 `GameState`파일에 `Replicated`형식으로 변수를 선언하고
```cpp
UPROPERTY(Replicated, BlueprintReadOnly, Category = "Turn")
	float TurnTimerRemaining;
```
cpp 파일에 똑같이 `DOREPLIFETIME`매크로를 사용하여 클라이언트에 반영되도록 했습니다.
```cpp
DOREPLIFETIME(ABaseBallGameState, TurnTimerRemaining);
```
```cpp
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
```
다음 턴으로 변경시키는 `NextTurn()`함수입니다.
```cpp
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
```
시간이 끝나면 호출되는 함수입니다. 무승부 처리가 되는게아니면 `NextTurn()`함수가 호출됩니다.

```cpp
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
```
Window창에 시간을 바인딩하기위해 `UpdateTurnTimer()`함수를 구현했습니다.

```cpp
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
```

그 후에 ` GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ABaseBallGameModeBase::UpdateTurnTimer, 1.0f, true);` 를 작성하여 타이머를 설정했습니다.

## 5. 시연 영상
[링크](https://drive.google.com/file/d/1KWP7KqU5EA8jm7CN4jBkBILJEaJPM_rr/view?usp=sharing)
