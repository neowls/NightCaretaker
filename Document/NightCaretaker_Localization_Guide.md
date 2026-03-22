# Unreal C++ 에디터 로컬라이제이션 가이드

## 문서 목적

이 문서는 Unreal C++ 코드의 식별자는 영문으로 유지하면서, 에디터에 보이는 멤버 이름과 설명만 로컬라이즈하는 권장 흐름을 정리한다.
대상은 `UPROPERTY`, `UFUNCTION`, `UCLASS`, `USTRUCT`, `UENUM`처럼 리플렉션 시스템에 노출되는 선언이며, 특히 Details 패널, Blueprint 노드, 툴팁, Enum 표시명을 다룬다.

## 1. 기본 원칙

1. 코드 식별자는 영문으로 유지한다.
   - 클래스명, 함수명, 변수명, 파일명, enum 값, 코드 주석은 영문 기준으로 둔다.
   - 예: `RealityCamIntensity`, `SetPrecisionInteractionEnabled`, `FNCRealityCameraTuning`
2. 에디터에 보이는 텍스트만 로컬라이즈한다.
   - Details 패널 표시명
   - Blueprint 노드명
   - 툴팁
   - Enum 표시명
3. 소스 문자열은 영어를 기준으로 둔다.
   - `DisplayName`, `ToolTip`, `ShortToolTip`에는 영어 문장을 넣는다.
   - 한국어는 Localization Dashboard에서 번역한다.

이 방식이 좋은 이유는 코드 검색성과 유지보수성을 해치지 않으면서, 디자이너와 기획자가 에디터에서는 한국어 설명을 볼 수 있기 때문이다.

## 2. 멤버 메타데이터는 어떻게 작성하나

Unreal의 리플렉션 선언은 `meta=(...)`를 통해 에디터 표시용 텍스트를 지정할 수 있다.
권장하는 핵심 키는 다음 세 가지다.

- `DisplayName`
- `ToolTip`
- `ShortToolTip`

### 2.1 UPROPERTY 예시

```cpp
UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category = "RealityCam|Base",
    meta = (
        DisplayName = "Reality Camera Intensity",
        ToolTip = "Controls the overall intensity of procedural RealityCam motion. Zero falls back to a near-static camera."
    )
)
float RealityCamIntensity = 0.65f;
```

결과:
- 코드에서는 `RealityCamIntensity`를 그대로 사용한다.
- 에디터에서는 `Reality Camera Intensity`가 소스 문자열로 노출된다.
- 한국어 번역이 있으면 에디터 언어 변경 시 번역된 표시명으로 보인다.

### 2.2 UFUNCTION 예시

```cpp
UFUNCTION(
    BlueprintCallable,
    Category = "RealityCam",
    meta = (
        DisplayName = "Set Precision Interaction Enabled",
        ToolTip = "Enables or disables reduced-motion damping for precise inspection and interaction work."
    )
)
void SetPrecisionInteractionEnabled(bool bEnabled);
```

### 2.3 함수 파라미터 이름

함수 파라미터는 `UPARAM(DisplayName = "...")`로 에디터 표시명을 별도로 줄 수 있다.

```cpp
UFUNCTION(BlueprintCallable, Category = "RealityCam")
void SetPrecisionInteractionEnabled(
    UPARAM(DisplayName = "Enabled") bool bEnabled);
```

### 2.4 Enum 표시명

Enum 항목은 `UMETA(DisplayName = "...")`를 사용한다.

```cpp
UENUM(BlueprintType)
enum class ENCCameraMode : uint8
{
    Default UMETA(DisplayName = "Default"),
    Precision UMETA(DisplayName = "Precision"),
    Sprint UMETA(DisplayName = "Sprint")
};
```

## 3. FText Localization과 metadata localization의 역할 구분

질문처럼 "에디터에서 볼 때만 다른 언어로 바꾸고 싶다"는 요구는 맞는 방향이다.
다만 Unreal에서는 그 방법이 두 갈래로 나뉜다.

### 3.1 리플렉션으로 노출되는 항목

`UPROPERTY`, `UFUNCTION`, `UCLASS`, `USTRUCT`, `UENUM`에 붙는 표시명과 툴팁은 보통 `meta=(DisplayName="...", ToolTip="...")`로 작성한다.
이 텍스트는 직접 `FText` 변수를 두는 방식이 아니라, 메타데이터 소스 문자열을 Localization Dashboard가 수집해서 번역하는 흐름이 더 적절하다.

즉, 에디터 노출용 선언에서는 보통 아래 방식이 표준에 가깝다.

- 코드 식별자: 영문
- 메타데이터 소스 문자열: 영문
- 번역: Localization Dashboard에서 관리

### 3.2 런타임 UI나 커스텀 에디터 UI

직접 Slate를 만들거나, 커스텀 디테일 패널을 만들거나, 런타임 위젯 텍스트를 구성할 때는 `FText`, `LOCTEXT`, `NSLOCTEXT`, String Table을 사용하는 것이 맞다.

예시:

```cpp
#define LOCTEXT_NAMESPACE "NightCaretakerEditor"

const FText SectionTitle = LOCTEXT("RealityCamSectionTitle", "Reality Camera");

#undef LOCTEXT_NAMESPACE
```

정리하면:
- 리플렉션 선언의 표시명/툴팁: metadata gather
- 런타임 UI, Slate, 커스텀 에디터 위젯: `FText` / `LOCTEXT`

## 4. Localization Dashboard 설정 방법

영문 메타데이터를 한국어로 보이게 하려면 Localization Dashboard에서 메타데이터 수집을 활성화해야 한다.

권장 절차:
1. Unreal Editor에서 `Tools > Localization Dashboard`를 연다.
2. 새 Localization Target을 만든다.
3. Source 경로에 프로젝트 C++ 소스 경로를 포함한다.
   - 예: `Source/NightCaretaker`
4. Gather 설정에서 `Gather from Meta Data`를 활성화한다.
5. 수집 대상 키에 아래 항목을 추가한다.
   - `DisplayName`
   - `ToolTip`
   - `ShortToolTip`
6. `Gather Text`를 실행한다.
7. 대상 문화권에 `ko`를 추가한다.
8. 수집된 영문 소스 문자열에 한국어 번역을 작성한다.
9. `Compile Text`를 실행한다.
10. 에디터 언어를 한국어로 바꿔 결과를 확인한다.

## 5. Category는 어떻게 둘 것인가

`Category`는 기술 구조를 드러내는 역할도 크기 때문에, 장기적으로는 영문 유지 쪽이 더 안전하다.

권장 예시:

```cpp
Category = "RealityCam|Idle"
Category = "RealityCam|Locomotion"
Category = "RealityCam|Rotation"
```

이유:
- 코드 구조와 문서 구조를 맞추기 쉽다.
- 다국어 환경에서도 분류명이 흔들리지 않는다.
- 검색과 유지보수가 수월하다.

에디터에서 완전한 현지화를 더 강하게 원하면 Category까지 별도 전략을 세울 수 있지만, 일반적으로는 `DisplayName`과 `ToolTip` 현지화만으로도 충분하다.

## 6. 주석과 툴팁은 분리해서 생각한다

주석과 툴팁은 목적이 다르다.

- 코드 주석: 개발자용 설명, 영문 유지
- 툴팁: 에디터 사용자용 설명, metadata 기반 로컬라이즈

즉 한국어 설명이 필요하다고 해서 C++ 주석을 한국어로 바꾸기보다는, 툴팁 메타데이터를 영어 소스 문자열로 작성하고 번역 파이프라인에 태우는 편이 더 낫다.

## 7. 실무 권장 규칙

- 클래스명, 함수명, 변수명, 파일명은 영문 유지
- 코드 주석은 영문 유지
- `DisplayName`, `ToolTip`, `ShortToolTip`는 영어 소스 문자열로 작성
- Blueprint 파라미터 이름은 `UPARAM(DisplayName = "...")` 사용
- Enum 표시는 `UMETA(DisplayName = "...")` 사용
- 런타임/UI 텍스트는 `FText`, `LOCTEXT`, String Table 사용
- 에디터 현지화는 Localization Dashboard의 metadata gather로 처리
- 가능하면 `Category`는 영문 유지

## 8. 추천 적용 대상

다음 항목들은 metadata localization 효과가 바로 드러나는 대표 사례다.

- 튜닝용 `UPROPERTY`
- BlueprintCallable `UFUNCTION`
- Blueprint에서 보이는 파라미터명
- Enum 항목 표시명
- 컴포넌트/데이터 구조체의 `UCLASS` / `USTRUCT` 표시명

## 9. 템플릿 예시

### UPROPERTY 템플릿

```cpp
UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category = "System|Base",
    meta = (
        DisplayName = "Some Value",
        ToolTip = "Explains what this value controls in the editor."
    )
)
float SomeValue = 0.0f;
```

### UFUNCTION 템플릿

```cpp
UFUNCTION(
    BlueprintCallable,
    Category = "System",
    meta = (
        DisplayName = "Do Something",
        ToolTip = "Executes the requested behavior."
    )
)
void DoSomething(
    UPARAM(DisplayName = "Enabled") bool bEnabled);
```

### FText 템플릿

```cpp
#define LOCTEXT_NAMESPACE "NightCaretakerRuntime"

const FText Label = LOCTEXT("RealityCamLabel", "Reality Camera");

#undef LOCTEXT_NAMESPACE
```

## 10. 최종 정리

이 프로젝트에서는 코드 자체는 영문으로 유지하고, 에디터에서 보이는 이름과 설명만 현지화하는 방식이 가장 적절하다.
리플렉션 선언은 영어 `DisplayName` / `ToolTip` 메타데이터를 작성한 뒤 Localization Dashboard에서 번역하고, 런타임 UI나 커스텀 에디터 UI는 `FText` 기반 로컬라이제이션을 사용하면 된다.