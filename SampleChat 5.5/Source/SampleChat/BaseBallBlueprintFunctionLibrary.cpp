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

void UBaseBallBlueprintFunctionLibrary::CheckGuess(const TArray<int32>& SecretNumbers, const TArray<int32>& GuessNumbers, int32& OutStrikes, int32& OutBalls)
{
	OutStrikes = 0;
	OutBalls = 0;
	for (int32 i = 0; i < 3; i++)
	{
		if (SecretNumbers[i] == GuessNumbers[i])
		{
			OutStrikes++;
		}
		else if (SecretNumbers.Contains(GuessNumbers[i]))
		{
			OutBalls++;
		}
	}
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