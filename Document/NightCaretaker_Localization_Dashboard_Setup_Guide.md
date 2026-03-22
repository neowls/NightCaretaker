# NightCaretaker Localization Dashboard Setup Guide

## 목적

이 문서는 Unreal C++ 네이티브 코드에 작성한 `DisplayName`, `ToolTip`, `ShortToolTip` 같은 메타데이터를 Localization Dashboard로 수집하고, 에디터에서 한국어로 보이게 만드는 실제 셋업 절차를 단계별로 정리한다.

대상은 다음과 같은 선언이다.

```cpp
UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category = "Input",
    meta = (
        DisplayName = "Move Input Action",
        ToolTip = "Input action expected to deliver a two-dimensional locomotion value."
    )
)
TObjectPtr<UInputAction> MoveInputAction;
```

핵심 전제는 아래와 같다.

- 코드 식별자와 주석은 영문 유지
- 에디터에 보이는 이름과 설명만 로컬라이즈
- 리플렉션 선언의 표시명/툴팁은 `FText`를 직접 붙이는 방식이 아니라 metadata gather로 번역
- 런타임 UI와 커스텀 Slate UI는 `FText` / `LOCTEXT`로 별도 처리

## 빠른 결론

에디터 메타데이터 현지화는 한 타깃으로 억지로 처리하기보다 아래처럼 나누는 것이 가장 안전하다.

1. `NightCaretaker_PropertyNames`
   - 용도: `DisplayName` 중심
   - Loading Policy: `PropertyNames`
2. `NightCaretaker_ToolTips`
   - 용도: `ToolTip`, `ShortToolTip`
   - Loading Policy: `ToolTips`
3. 선택 사항: `NightCaretaker_Editor`
   - 용도: 에디터 모듈의 `LOCTEXT`, Slate, 커스텀 디테일 패널
   - Loading Policy: `Editor`

이렇게 나누는 이유는 UE 5.7 기준으로 에디터가 툴팁용 로컬라이제이션 경로는 항상 읽지만, 프로퍼티 이름용 로컬라이제이션 경로는 `Use Localized Property Names` 옵션이 켜졌을 때만 읽기 때문이다.

## 1. 사전 준비

Localization Dashboard를 열기 전에 먼저 아래 조건을 맞춘다.

### 1.1 코드 메타데이터는 영어 소스 문자열로 작성

예시:

```cpp
UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category = "RealityCam",
    meta = (
        DisplayName = "Reality Camera Intensity",
        ToolTip = "Controls the overall intensity of procedural RealityCam motion. Zero falls back to a near-static camera."
    )
)
float RealityCamIntensity = 0.65f;
```

중요한 점:
- `DisplayName`, `ToolTip`, `ShortToolTip`는 영어로 쓴다.
- 코드 식별자는 바꾸지 않는다.
- `Category`는 가능하면 영문 유지가 좋다.

### 1.2 번역 기준 언어 정하기

권장 기준:
- Native Culture: `en`
- Target Culture: `ko`

즉 영어가 원문이고, 한국어가 번역본이다.

### 1.3 에디터에서 보일 범위 정하기

실무적으로는 아래처럼 나누는 것이 좋다.

- `DisplayName`: 현지화 대상
- `ToolTip`, `ShortToolTip`: 현지화 대상
- `Category`: 가능하면 영문 유지

`Category`도 시도는 가능하지만, 프로젝트 전반 구조와 검색성에 영향을 주므로 처음에는 번역하지 않는 편이 안전하다.

## 2. 권장 타깃 구조

### 2.1 NightCaretaker_PropertyNames

이 타깃은 프로퍼티/함수/클래스/구조체/Enum의 표시명 계열을 담당한다.

권장 설정:
- Name: `NightCaretaker_PropertyNames`
- Loading Policy: `PropertyNames`
- Native Culture: `en`
- Supported Cultures: `ko`

수집 대상 예시:
- `DisplayName`

### 2.2 NightCaretaker_ToolTips

이 타깃은 설명 텍스트 계열을 담당한다.

권장 설정:
- Name: `NightCaretaker_ToolTips`
- Loading Policy: `ToolTips`
- Native Culture: `en`
- Supported Cultures: `ko`

수집 대상 예시:
- `ToolTip`
- `ShortToolTip`

### 2.3 선택 사항: NightCaretaker_Editor

커스텀 에디터 모듈에서 `LOCTEXT`로 작성한 텍스트가 있다면 별도 `Editor` 타깃을 둔다.
현재 질문 범위가 `UPROPERTY/UFUNCTION` 메타데이터라면 필수는 아니다.

## 3. Localization Dashboard에서 실제로 만드는 순서

## 3.1 Dashboard 열기

1. Unreal Editor 실행
2. 상단 메뉴에서 `Tools > Localization Dashboard` 열기
3. Localization Dashboard 창이 비어 있다면 새 Target 생성

## 3.2 첫 번째 타깃 생성: PropertyNames

1. `Add New Target` 클릭
2. Target 이름을 `NightCaretaker_PropertyNames`로 지정
3. Native Culture를 `en`으로 설정
4. Supported Cultures에 `ko` 추가
5. 저장

그 다음 Target 상세 설정에서 아래 항목을 맞춘다.

### Target Settings

- Loading Policy: `PropertyNames`

이 설정을 써야 `Use Localized Property Names` 옵션과 정상적으로 연동된다.

### Gather Text From Meta Data

- Enabled: `true`
- Should Gather From Editor Only Data: `true`
- Include Path Wildcards:
  - `Source/NightCaretaker/*`
- Exclude Path Wildcards:
  - `Intermediate/*`
  - `Binaries/*`

처음엔 Include만 `Source/NightCaretaker/*`로 두고 시작해도 충분하다.

### Key Specifications

최소 권장값은 아래 1개다.

1. `DisplayName`
   - Meta Data Key: `DisplayName`
   - Text Namespace: `NightCaretaker.Editor.Metadata`
   - Text Key Pattern: `{FieldPath}.DisplayName`

설명:
- `FieldPath`는 해당 필드의 정규화된 경로를 기반으로 키를 만든다.
- 키에 원문 문장 자체를 넣지 않기 때문에 번역 자산이 더 안정적이다.

선택 사항:
- `Category`
  - Meta Data Key: `Category`
  - Text Namespace: `NightCaretaker.Editor.Metadata`
  - Text Key Pattern: `{FieldPath}.Category`

다만 `Category`는 에디터 구조와 검색성에 영향을 주고, 패널별 표시 일관성도 직접 검증이 필요하므로 기본 권장값은 아니다.

## 3.3 두 번째 타깃 생성: ToolTips

1. `Add New Target` 클릭
2. Target 이름을 `NightCaretaker_ToolTips`로 지정
3. Native Culture를 `en`으로 설정
4. Supported Cultures에 `ko` 추가
5. 저장

그 다음 Target 상세 설정에서 아래 항목을 맞춘다.

### Target Settings

- Loading Policy: `ToolTips`

### Gather Text From Meta Data

- Enabled: `true`
- Should Gather From Editor Only Data: `true`
- Include Path Wildcards:
  - `Source/NightCaretaker/*`
- Exclude Path Wildcards:
  - `Intermediate/*`
  - `Binaries/*`

### Key Specifications

권장값은 아래 2개다.

1. `ToolTip`
   - Meta Data Key: `ToolTip`
   - Text Namespace: `NightCaretaker.Editor.Metadata`
   - Text Key Pattern: `{FieldPath}.ToolTip`

2. `ShortToolTip`
   - Meta Data Key: `ShortToolTip`
   - Text Namespace: `NightCaretaker.Editor.Metadata`
   - Text Key Pattern: `{FieldPath}.ShortToolTip`

## 4. Gather, Translate, Compile 순서

타깃을 만들었다고 바로 에디터에 번역이 보이지는 않는다. 아래 순서를 따라야 한다.

### 4.1 Gather Text

각 타깃에서 `Gather Text`를 실행한다.

이 단계에서 Dashboard는 C++ 헤더/소스의 메타데이터를 읽고 번역 원문 목록을 만든다.

기대 결과:
- `Reality Camera Intensity`
- `Set Precision Interaction Enabled`
- `Controls the overall intensity of procedural RealityCam motion...`

같은 문자열이 타깃에 수집된다.

### 4.2 번역 작성

방법은 두 가지가 있다.

1. Dashboard/Translation Editor 안에서 직접 번역
2. `Export Text` 후 `PO` 파일을 외부 번역 툴에서 작업한 뒤 `Import Text`

처음에는 대시보드 안에서 바로 번역해도 충분하다.

예시:
- `Reality Camera Intensity` -> `현실감 카메라 강도`
- `Move Input Action` -> `이동 입력 액션`
- `Input action expected to deliver a two-dimensional locomotion value.` -> `2차원 이동 입력 값을 전달하는 입력 액션입니다.`

### 4.3 Compile Text

번역을 작성했으면 각 타깃에서 `Compile Text`를 실행한다.

이 단계에서 `.locres`가 생성된다.
보통 프로젝트 기준으로 `Content/Localization/<TargetName>/` 아래에 결과물이 만들어진다.

## 5. 에디터에서 실제로 번역을 보이게 하는 방법

여기가 가장 많이 놓치는 부분이다.

## 5.1 에디터 언어를 한국어로 변경

에디터 언어가 영어면 한국어 번역 리소스를 만들어도 바로 체감이 안 될 수 있다.
먼저 에디터 언어를 한국어로 맞춘다.

권장 확인 항목:
- Editor Language: `ko` 또는 Korean

## 5.2 Use Localized Property Names 켜기

UE 5.7 기준으로 프로퍼티 이름 계열은 이 옵션이 꺼져 있으면 `PropertyNames` 타깃이 로드되지 않는다.

설정 위치:
- `Editor Preferences`
- `Region & Language`
- `Use Localized Property Names`

이 옵션을 켜면 에디터가 프로퍼티 이름용 로컬라이제이션 경로를 읽는다.

정리:
- `DisplayName`이 안 바뀌면 가장 먼저 이 옵션을 확인한다.
- `ToolTip`은 별도 `ToolTips` 로딩 정책을 통해 에디터에서 읽힌다.

## 5.3 에디터 새로고침 또는 재시작

옵션을 바꾸거나 새 `.locres`를 만든 직후에는 재시작이 가장 확실하다.
간단한 변경은 바로 반영될 수도 있지만, 처음 셋업 단계에서는 에디터를 한 번 재시작하는 것을 권장한다.

## 6. 권장 입력값 예시

아래는 바로 따라 넣기 쉬운 기준값이다.

### PropertyNames 타깃 예시

- Target Name: `NightCaretaker_PropertyNames`
- Loading Policy: `PropertyNames`
- Native Culture: `en`
- Supported Cultures: `ko`
- Include Path Wildcards:
  - `Source/NightCaretaker/*`
- Exclude Path Wildcards:
  - `Intermediate/*`
  - `Binaries/*`
- Should Gather From Editor Only Data: `true`
- Key Specifications:
  - `DisplayName` / `NightCaretaker.Editor.Metadata` / `{FieldPath}.DisplayName`

### ToolTips 타깃 예시

- Target Name: `NightCaretaker_ToolTips`
- Loading Policy: `ToolTips`
- Native Culture: `en`
- Supported Cultures: `ko`
- Include Path Wildcards:
  - `Source/NightCaretaker/*`
- Exclude Path Wildcards:
  - `Intermediate/*`
  - `Binaries/*`
- Should Gather From Editor Only Data: `true`
- Key Specifications:
  - `ToolTip` / `NightCaretaker.Editor.Metadata` / `{FieldPath}.ToolTip`
  - `ShortToolTip` / `NightCaretaker.Editor.Metadata` / `{FieldPath}.ShortToolTip`

## 7. 현재 프로젝트에 적용해서 확인할 항목

현재 NightCaretaker 기준으로는 아래처럼 검증하면 된다.

### PropertyNames 쪽

[NCRealityCameraComponent.h](/F:/UnrealProjects/NightCaretaker/Source/NightCaretaker/Camera/NCRealityCameraComponent.h)
- `Reality Camera Tuning`
- `Reality Camera Intensity`
- `Apply Runtime Tuning`
- `Set Precision Interaction Enabled`

[NCPlayerCharacter.h](/F:/UnrealProjects/NightCaretaker/Source/NightCaretaker/Character/NCPlayerCharacter.h)
- `Reality Camera`
- `Move Input Action`
- `Run Speed`

### ToolTips 쪽

같은 파일에서 각 항목의 설명 문장이 한국어 번역으로 보여야 한다.

예를 들어 `Move Input Action`의 툴팁 위로 마우스를 올렸을 때, 영어 원문 대신 한국어 설명이 나오면 `ToolTips` 타깃이 정상 동작하는 것이다.

## 8. 자주 막히는 지점

### 8.1 Gather를 했는데 문자열이 안 잡힌다

확인할 것:
- `Gather from Meta Data`가 켜져 있는가
- Include Path가 실제 소스 경로를 포함하는가
- `Source/NightCaretaker/*`처럼 프로젝트 루트 기준 상대 경로를 썼는가
- 메타데이터 키 이름을 정확히 넣었는가
  - `DisplayName`
  - `ToolTip`
  - `ShortToolTip`
- `Should Gather From Editor Only Data`가 켜져 있는가

## 8.2 번역은 했는데 에디터에서 DisplayName이 영어로 보인다

가장 흔한 원인:
- `Loading Policy`를 `PropertyNames`로 두지 않았다
- `Use Localized Property Names`가 꺼져 있다
- `Compile Text`를 안 했다
- 에디터 언어가 아직 영어다
- 에디터를 재시작하지 않았다

## 8.3 ToolTip만 영어로 남아 있다

확인할 것:
- `ToolTips` 타깃의 Loading Policy가 `ToolTips`인가
- `ToolTip`, `ShortToolTip` 키를 각각 넣었는가
- 해당 문자열에 실제 번역이 들어갔는가
- 다시 `Compile Text` 했는가

## 8.4 Category까지 번역하고 싶다

가능성은 있지만 기본 권장값은 아니다.
이유는 다음과 같다.

- 카테고리는 구조 분류 역할도 한다.
- 영문 구조를 유지하는 편이 검색과 문서화가 쉽다.
- 패널마다 표시 일관성을 직접 검증해야 한다.

그래도 시도한다면 아래처럼 별도 키 스펙을 추가한다.

- Meta Data Key: `Category`
- Text Namespace: `NightCaretaker.Editor.Metadata`
- Text Key Pattern: `{FieldPath}.Category`

다만 이 경우는 실제 Details 패널과 Blueprint 에디터에서 원하는 방식으로 보이는지 프로젝트에서 직접 검증한 뒤 채택하는 것이 좋다.

## 9. 왜 한 타깃으로 하지 않고 둘로 나누는가

한 타깃에 `DisplayName`과 `ToolTip`를 모두 넣고 Loading Policy를 `Editor`로 둘 수도 있다.
하지만 이 방식은 `Use Localized Property Names` 옵션과 분리되어 동작할 수 있어, 프로퍼티 이름만 선택적으로 켜고 끄는 흐름이 흐려진다.

반대로 아래처럼 나누면 엔진 의도와 잘 맞는다.

- `PropertyNames` 타깃: 프로퍼티 이름 현지화 전용
- `ToolTips` 타깃: 툴팁 현지화 전용

처음 셋업이 약간 번거롭지만, 장기적으로는 더 예측 가능하다.

## 10. 추천 운영 방식

가장 현실적인 운영 방식은 아래와 같다.

1. 코드 식별자는 계속 영문 유지
2. `DisplayName`, `ToolTip`, `ShortToolTip`만 영어 소스 문자열로 관리
3. Dashboard에서 `PropertyNames`와 `ToolTips` 타깃을 분리
4. Native Culture는 영어, 번역 대상은 한국어
5. 디자이너/TA는 한국어 에디터에서 작업
6. 개발자는 필요하면 `Use Localized Property Names`를 끄고 영문 구조로 확인

## 11. 최종 체크리스트

셋업이 끝나면 아래 항목을 확인한다.

- `Gather Text`가 정상 완료된다.
- `Compile Text` 후 `.locres`가 생성된다.
- 에디터 언어가 한국어다.
- `Use Localized Property Names`가 켜져 있다.
- Details 패널에서 `Reality Camera Intensity` 같은 이름이 한국어로 보인다.
- 툴팁이 한국어로 보인다.
- Blueprint 노드명과 파라미터 표시명이 의도대로 번역된다.

## 12. 이 문서 이후 권장 다음 단계

이 프로젝트 기준으로 다음 순서를 추천한다.

1. `NightCaretaker_PropertyNames` 타깃 생성
2. `NightCaretaker_ToolTips` 타깃 생성
3. `DisplayName`, `ToolTip`, `ShortToolTip`만 먼저 수집
4. 현재 `RealityCam` 관련 클래스에서 동작 확인
5. 이후 Input, Interaction, UI 관련 코드로 범위 확장

이 흐름으로 가면 코드 영문 유지 원칙을 깨지 않으면서, 에디터 사용자에게는 한국어 표시 환경을 제공할 수 있다.