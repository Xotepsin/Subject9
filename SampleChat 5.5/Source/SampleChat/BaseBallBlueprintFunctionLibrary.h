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
    static void CheckGuess(const TArray<int32>& SecretNumbers, const TArray<int32>& GuessNumbers, int32& OutStrikes, int32& OutBalls);

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
