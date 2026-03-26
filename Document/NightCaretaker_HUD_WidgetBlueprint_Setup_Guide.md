# NightCaretaker HUD Widget Blueprint 설정 가이드

## 1. 문서 목적

이 문서는 현재 NightCaretaker 프로젝트에 추가된 C++ UI 베이스를 기준으로,
플레이 HUD를 Widget Blueprint로 만들고 실제 게임에 연결하는 과정을 단계별로 설명한다.

이 문서의 목적은 아래 3가지다.

1. HUD BP를 어디에 등록해야 하는지 정확히 이해한다.
2. 각 단계에서 왜 그렇게 해야 하는지까지 이해한다.
3. 이후 상호작용 프롬프트, 자막, 도구 표시를 확장할 때 구조가 흔들리지 않게 한다.

---

## 2. 현재 HUD 생성 구조

현재 코드 기준으로 런타임 HUD는 아래 흐름으로 생성된다.

```text
ANCGameMode
    -> ANCPlayerController
        -> ANCPlayerControllerBase::BeginPlay()
            -> ShowRuntimeHUD()
                -> UNCUISubsystem::ShowWidget(...)
                    -> UNCPlayerHUDWidget 생성
```

핵심은 아래와 같다.

- HUD는 `GameMode`가 직접 생성하지 않는다.
- HUD는 `PlayerController`가 로컬 플레이어 기준으로 띄운다.
- 실제 HUD 위젯 클래스 지정 지점은 `ANCPlayerControllerBase`의 `PlayerHUDWidgetClass`다.

즉, HUD를 실제로 등록하려면 `PlayerController Blueprint`에서 `PlayerHUDWidgetClass`를 설정해야 한다.

---

## 3. 왜 PlayerController에 등록해야 하는가

이 부분이 가장 중요하다.

### 3.1 HUD는 월드 객체가 아니라 로컬 플레이어 UI다

HUD는 문, 전등, 민원 상태처럼 월드에 붙은 객체가 아니다.
HUD는 현재 화면을 보고 있는 로컬 플레이어에게 붙는 UI다.

그래서 HUD 소유권은 아래 쪽이 자연스럽다.

- `PlayerController`
- `LocalPlayerSubsystem`

반대로 아래는 적절하지 않다.

- `Character`
- `GameMode`
- `GameState`

### 3.2 Character에 두지 않는 이유

캐릭터는 이후 교체되거나 재소유될 수 있다.
HUD를 캐릭터에 묶으면 다음 문제가 생긴다.

- Pawn 교체 시 HUD 생명주기가 꼬일 수 있다.
- UI가 플레이어 입력보다 캐릭터 수명주기에 끌려간다.
- HUD가 캐릭터 구현에 과하게 의존하게 된다.

반면 `PlayerController`는 플레이어 UI를 소유하는 위치라서 구조적으로 훨씬 안전하다.

### 3.3 GameMode에 두지 않는 이유

`GameMode`는 로컬 UI를 직접 소유하는 객체가 아니다.
`GameMode`는 규칙과 흐름을 관리하는 런타임 객체다.

HUD는 플레이어 단위로 생성되어야 하므로,
`GameMode`가 직접 HUD를 만들거나 HUD 클래스를 들고 있는 구조는 적합하지 않다.

### 3.4 현재 구조에서 PlayerController가 등록 지점인 이유

현재 프로젝트는 `ANCPlayerControllerBase::BeginPlay()`에서 `ShowRuntimeHUD()`를 호출한다.
즉 자동 HUD 부팅이 이미 PlayerController 기준으로 설계돼 있다.

그래서 지금 단계에서 가장 안전하고 단순한 방식은 아래다.

1. `ANCPlayerController` 기반 BP 생성
2. 그 BP에 `PlayerHUDWidgetClass` 지정
3. 그 BP를 실제 GameMode에서 사용

---

## 4. 현재 HUD가 이미 가지고 있는 데이터

`UNCPlayerHUDWidget`은 아래 상태를 C++에서 이미 제공한다.

- `bShowReticle`
- `ReticleOpacity`
- `bCanInteract`
- `InteractionPromptText`
- `ToolLabelText`
- `bShowSubtitle`
- `SubtitleSpeakerText`
- `SubtitleLineText`

이 값들은 `UNCPlayerHUDWidgetSource`에서 들어오고,
그 source는 `ANCPlayerControllerBase`가 자동으로 만든다.

즉 HUD BP는 직접 월드를 뒤질 필요가 없다.
HUD BP는 부모 위젯이 채워 준 값을 화면에 반영하기만 하면 된다.

이 방식이 중요한 이유는 아래와 같다.

- BP가 `GetWorld`, `GetSubsystem`, `GetPlayerController`를 계속 찾지 않아도 된다.
- UI가 게임 로직을 직접 건드리지 않고 표시 역할에 집중할 수 있다.
- 나중에 HUD 데이터 공급 방식이 바뀌어도 BP 수정량이 적다.

---

## 5. 추천 에셋 구성

추천 이름은 아래처럼 잡는 것이 좋다.

- `WBP_PlayerHUD`
- `BP_PlayerController_Runtime`
- `BP_GameMode_Runtime`

추천 폴더 예시는 아래와 같다.

```text
/Game/NightCaretaker/UI/HUD/WBP_PlayerHUD
/Game/NightCaretaker/Blueprints/System/BP_PlayerController_Runtime
/Game/NightCaretaker/Blueprints/System/BP_GameMode_Runtime
```

이렇게 나누는 이유는 아래와 같다.

- HUD 에셋과 시스템 BP를 분리하면 찾기 쉽다.
- 이후 메뉴, 옵션, 민원보드도 같은 규칙으로 정리할 수 있다.
- UI가 늘어나도 폴더 구조가 무너지지 않는다.

---

## 6. 단계별 설정 절차

## 6.1 HUD Widget Blueprint 만들기

### 작업

1. 콘텐츠 브라우저에서 HUD 폴더로 이동한다.
2. `User Interface > Widget Blueprint`를 생성한다.
3. 부모 클래스를 `UNCPlayerHUDWidget`으로 지정한다.
4. 이름을 `WBP_PlayerHUD`로 저장한다.

### 왜 이렇게 해야 하는가

`UNCPlayerHUDWidget`을 부모로 잡아야 아래가 자동으로 따라온다.

- HUD 전용 상태 변수
- source 기반 갱신 구조
- `RefreshView()` 기반 이벤트 갱신
- HUD용 입력 정책 (`GameOnly`)

만약 그냥 `UserWidget`으로 만들면,
현재 만든 C++ HUD 구조를 전혀 못 쓴다.
그러면 PlayerController를 찾고 source를 찾고 갱신 로직을 다시 직접 만들어야 한다.

즉 부모를 `UNCPlayerHUDWidget`으로 잡는 것이 지금 구조를 사용하는 출발점이다.

---

## 6.2 HUD 레이아웃 만들기

### 권장 기본 구성

Root 아래에 아래 정도를 두는 것을 권장한다.

1. `Canvas Panel`
2. 중앙 앵커에 `Image` 또는 `Border`
   - 용도: 중심점 / reticle
3. 중앙 하단에 `TextBlock`
   - 용도: 상호작용 프롬프트
4. 우하단 또는 하단에 `TextBlock`
   - 용도: 현재 도구 이름
5. 하단 중앙에 `Overlay` 또는 `Vertical Box`
   - 용도: 자막 박스
   - 내부: 화자 이름 `TextBlock`, 자막 본문 `TextBlock`

### 왜 이렇게 배치하는가

GDD 기준 UI 원칙은 아래다.

- 상시 정보 최소화
- 중요한 정보는 읽기 쉬워야 함
- 현실적인 소품 UI와 게임 HUD 사이 균형 유지

그래서 HUD도 과하게 복잡하게 만들 필요가 없다.
초기 HUD는 아래 3축만 확실히 보이면 된다.

- 중심 상호작용
- 순간 안내
- 자막 전달

즉 HUD는 정보판이 아니라,
현재 플레이어가 보고 있는 상황을 최소한으로 보조하는 계층으로 두는 것이 맞다.

---

## 6.3 위젯 변수 연결 준비

### 작업

Designer에서 아래 위젯들에 `Is Variable`을 켠다.

- Reticle용 `Image`
- Interaction Prompt용 `TextBlock`
- Tool Label용 `TextBlock`
- Subtitle Root용 `Overlay` 또는 `Border`
- Subtitle Speaker용 `TextBlock`
- Subtitle Line용 `TextBlock`

### 왜 이렇게 해야 하는가

`RefreshView()`가 호출될 때,
C++가 갱신한 HUD 상태를 실제 위젯 컴포넌트에 반영해야 한다.

이를 위해서는 그래프에서 해당 위젯들을 직접 참조할 수 있어야 한다.
`Is Variable`을 켜지 않으면 그래프에서 직접 제어할 수 없다.

---

## 6.4 RefreshView 이벤트 구현하기

### 작업

1. `WBP_PlayerHUD`를 연다.
2. Graph로 이동한다.
3. `Refresh View` 이벤트를 구현한다.
4. 반드시 `Parent: Refresh View`를 먼저 호출한다.
5. 그 다음 아래처럼 위젯 값을 실제 화면에 적용한다.

예시 흐름:

- Reticle Image
  - `Set Render Opacity(ReticleOpacity)`
  - 필요하면 `bShowReticle`에 따라 `Set Visibility`
- Interaction Prompt Text
  - `SetText(InteractionPromptText)`
  - `bCanInteract` 또는 텍스트 비어 있음 여부로 `Set Visibility`
- Tool Label Text
  - `SetText(ToolLabelText)`
  - 비어 있으면 `Collapsed`
- Subtitle Root
  - `bShowSubtitle`에 따라 `Visible` 또는 `Collapsed`
- Subtitle Speaker Text
  - `SetText(SubtitleSpeakerText)`
- Subtitle Line Text
  - `SetText(SubtitleLineText)`

### 왜 부모 호출이 필요한가

이게 가장 중요하다.

`UNCPlayerHUDWidget::RefreshView_Implementation()`은 source에서 값을 읽어서 아래 변수들을 먼저 갱신한다.

- `ReticleOpacity`
- `bCanInteract`
- `InteractionPromptText`
- `SubtitleLineText`
- 기타 HUD 상태값

즉 부모 호출을 하지 않으면 BP가 보고 있는 값이 최신 상태가 아닐 수 있다.

흐름은 아래처럼 이해하면 된다.

1. source 변경
2. `UNCPlayerHUDWidget`가 C++ 변수 갱신
3. BP가 그 값을 실제 위젯에 반영

그래서 `RefreshView` BP 구현에서는 부모 호출이 먼저 와야 한다.

---

## 6.5 왜 Tick이나 Designer Binding 대신 RefreshView를 쓰는가

### Tick을 쓰지 않는 이유

Tick으로 HUD를 계속 갱신하면 아래 문제가 생긴다.

- 필요 없을 때도 계속 UI 업데이트가 돈다.
- 나중에 HUD 요소가 늘어나면 비효율이 커진다.
- 지금 구조의 장점인 값이 바뀔 때만 갱신이 사라진다.

### Designer Binding을 기본으로 추천하지 않는 이유

UMG 바인딩은 편해 보이지만,
프로젝트가 커질수록 값이 언제 읽히는지 흐름이 흐려진다.

현재 구조는 명시적으로 아래 흐름을 가진다.

- 게임 로직이 source를 바꿈
- source가 widget refresh를 요청함
- widget이 한 번 갱신함

이 구조가 디버그와 유지보수에 더 유리하다.

그래서 현재 프로젝트에서는 `RefreshView` 기반 이벤트 갱신이 더 적합하다.

---

## 6.6 Reticle Fade In / Fade Out 만들기

### 가장 단순한 방법

- Reticle의 `Render Opacity`를 `ReticleOpacity`로 직접 설정한다.

현재 코드 기본 흐름은 아래를 상정한다.

- 평소: 낮은 opacity
- 상호작용 가능: 높은 opacity

### 추천 연출 방향

- 평소 opacity: `0.25 ~ 0.4`
- 상호작용 가능 시: `0.9 ~ 1.0`
- 크기 변화는 최소화
- 색 변화도 과하지 않게 사용

### 왜 이렇게 해야 하는가

이 게임은 HUD가 전면에 나서면 안 된다.
레티클은 조준 보조이자 읽기 가능 상태를 알려주는 신호면 충분하다.

즉 강한 애니메이션보다 아래가 더 중요하다.

- 존재감은 약하게
- 상호작용 가능할 때만 확실하게
- 공포 분위기를 해치지 않게

### 애니메이션을 넣고 싶다면

UMG Animation을 써도 된다.
다만 v1에서는 아래 원칙을 권장한다.

- 애니메이션은 opacity 중심
- scale 변화는 아주 미세하게
- 트리거는 `RefreshView` 안에서 상태 비교 후 재생

초기에는 `Set Render Opacity`만으로 충분하다.
애니메이션은 나중에 붙이는 것이 더 안전하다.

---

## 6.7 Subtitle 영역 만들기

### 구성 권장

- 하단 중앙 정렬
- Speaker 이름은 작은 크기
- Subtitle 본문은 더 큰 크기
- 배경은 아주 약한 반투명

### 이유

자막은 게임의 중심 UI가 아니라,
공간 탐색을 방해하지 않는 선에서 정보만 전달해야 한다.

특히 NightCaretaker는 공간 읽기와 사운드가 중요하므로,
자막이 화면을 과하게 점유하면 오히려 체험을 해친다.

### 표시 규칙 권장

- `bShowSubtitle == false`면 subtitle root 전체를 `Collapsed`
- speaker가 비어 있으면 speaker text만 숨김
- line이 비어 있으면 전체를 숨김

이렇게 하면 나중에 dialogue system이 붙어도 BP 수정량이 적다.

---

## 6.8 PlayerController Blueprint 만들기

### 작업

1. `Blueprint Class`를 생성한다.
2. 부모 클래스를 `ANCPlayerController`로 지정한다.
3. 이름을 `BP_PlayerController_Runtime`처럼 저장한다.
4. Class Defaults에서 `PlayerHUDWidgetClass`를 `WBP_PlayerHUD`로 지정한다.

### 왜 이 단계가 필요한가

현재 자동 HUD 생성은 `ANCPlayerControllerBase::BeginPlay()`에서 일어난다.
그리고 이 함수는 `PlayerHUDWidgetClass`가 설정돼 있을 때만 HUD를 띄운다.

즉 HUD BP를 만들기만 하고,
PlayerController BP에 연결하지 않으면 실제 게임에서는 아무 일도 일어나지 않는다.

이 단계가 HUD 등록의 핵심이다.

### 왜 C++ 클래스가 아니라 BP로 지정하는가

HUD는 시각 레이아웃과 스타일이 자주 바뀐다.
이 영역은 C++보다 BP가 훨씬 빠르다.

그래서 구조는 C++에 두고,
최종 화면 클래스 지정은 BP에서 하는 방식이 가장 효율적이다.

---

## 6.9 GameMode 또는 월드에 PlayerController BP 연결하기

### 작업

아래 둘 중 하나로 연결한다.

#### 방법 A: GameMode Blueprint 사용

1. `ANCGameMode`를 부모로 하는 `BP_GameMode_Runtime` 생성
2. Class Defaults에서 `PlayerControllerClass`를 `BP_PlayerController_Runtime`으로 변경
3. 프로젝트 기본 GameMode 또는 맵 `World Settings`에서 이 GameMode를 사용

#### 방법 B: 기존 GameMode에서 교체

이미 사용 중인 GameMode BP가 있다면,
그 GameMode의 `PlayerControllerClass`만 바꿔도 된다.

### 왜 이 단계가 필요한가

현재 기본 C++ `ANCGameMode`는 `ANCPlayerController::StaticClass()`를 사용한다.
즉 아무 설정도 바꾸지 않으면 게임은 계속 C++ 기본 PlayerController를 사용한다.

그러면 방금 만든 `BP_PlayerController_Runtime`는 실제 플레이에서 생성되지 않는다.
당연히 `PlayerHUDWidgetClass`도 적용되지 않는다.

즉 다음 둘은 반드시 세트다.

1. PlayerController BP 생성
2. 그 BP가 실제 게임에서 사용되도록 GameMode 연결

---

## 6.10 첫 실행 테스트

### 최소 테스트 방법

1. PIE 실행
2. 중앙 reticle이 보이는지 확인
3. 게임 시작 직후 HUD가 자동 생성되는지 확인
4. subtitle root가 기본적으로 숨겨져 있는지 확인
5. 상호작용 프롬프트가 없을 때 prompt text가 숨겨져 있는지 확인

### 더 확실한 테스트 방법

임시로 PlayerController BP 또는 테스트 BP에서 아래를 호출해 볼 수 있다.

- `Get Player HUD Widget Source`
- `Set Interaction State(true, "Open Door", 1.0)`
- `Set Subtitle("307호", "문 좀 열어 주세요.")`

이렇게 하면 실제 상호작용 시스템이 아직 완전히 붙지 않았더라도,
HUD BP가 구조적으로 제대로 연결됐는지 바로 검증할 수 있다.

### 왜 source 테스트가 좋은가

이 테스트는 단순히 위젯이 예쁜지 보는 것이 아니라,
현재 UI 아키텍처 전체가 정상인지 확인한다.

- PlayerController가 source를 만들었는가
- HUD가 생성됐는가
- source 변경이 widget refresh를 유발하는가
- BP가 값을 올바르게 화면에 반영하는가

즉 나중에 상호작용 시스템이 붙을 때 생길 문제를 초기에 줄일 수 있다.

---

## 7. 추천 초기 HUD 구성안

현재 단계에서 가장 적절한 HUD 구성은 아래 정도다.

### 반드시 넣기

- 중앙 점 또는 작은 십자 reticle
- 상호작용 프롬프트 텍스트
- subtitle 영역

### 있어도 좋음

- 현재 도구 이름
- 약한 보조 상태 텍스트

### 아직 넣지 않는 편이 좋은 것

- 과한 상태 바
- 상시 민원 목록
- 화면 구석의 많은 디버그 정보
- 공간 몰입을 깨는 강한 HUD 프레임

이유는 명확하다.
이 게임은 업무 공포이면서 동시에 공간 공포다.
HUD가 너무 크거나 많으면 복도와 공간의 감정 밀도를 깎아 먹는다.

---

## 8. 자주 하는 실수

### 8.1 HUD BP만 만들고 등록을 안 함

증상:

- PIE에서 HUD가 안 뜬다.

원인:

- `PlayerHUDWidgetClass`를 PlayerController BP에 지정하지 않음
- 또는 PlayerController BP를 GameMode에 연결하지 않음

### 8.2 부모를 `UNCPlayerHUDWidget`이 아니라 `UserWidget`으로 만듦

증상:

- HUD는 보이지만 현재 구조의 source 갱신을 쓸 수 없음

원인:

- C++ HUD 베이스를 상속하지 않았기 때문

### 8.3 RefreshView에서 Parent 호출을 안 함

증상:

- source 값이 바뀌어도 HUD가 갱신되지 않거나 옛값처럼 보임

원인:

- 부모 구현이 source 값을 HUD 변수에 복사하는 역할을 하기 때문

### 8.4 Designer Binding에 과하게 의존함

증상:

- 나중에 UI 흐름 추적이 어려워짐
- 어떤 시점에 값이 갱신되는지 헷갈림

원인:

- 이벤트 기반 구조 대신 암묵적 바인딩 흐름이 섞였기 때문

### 8.5 HUD 생성 책임을 Character로 옮김

증상:

- 구조가 퍼지고 소유권이 불분명해짐

원인:

- UI를 캐릭터 수명주기에 묶었기 때문

---

## 9. 지금 구조에서 가장 추천하는 작업 순서

1. `WBP_PlayerHUD` 생성
2. reticle / prompt / subtitle 레이아웃 구성
3. `RefreshView` 구현 후 Parent 호출
4. `BP_PlayerController_Runtime` 생성
5. `PlayerHUDWidgetClass = WBP_PlayerHUD` 지정
6. `BP_GameMode_Runtime` 또는 `World Settings`에 PlayerController BP 연결
7. PIE에서 source 테스트
8. 이후 상호작용 시스템이 `UNCPlayerHUDWidgetSource`를 갱신하도록 연결

이 순서가 좋은 이유는,
화면 모양과 런타임 연결을 한 번에 검증할 수 있기 때문이다.

---

## 10. 이후 확장 방향

HUD BP가 정상 등록되면 다음 연결은 자연스럽다.

### 상호작용 프롬프트

나중에 상호작용 추적 로직이나 PlayerController 쪽에서 아래를 호출하면 된다.

- `SetInteractionState(...)`
- `ClearInteractionState(...)`

### 자막

나중에 대사 시스템이나 연출 로직에서 아래를 호출하면 된다.

- `SetSubtitle(...)`
- `ClearSubtitle()`

### 도구 표시

도구 시스템이 붙으면 아래를 호출하면 된다.

- `SetToolLabel(...)`
- `ClearToolLabel()`

즉 HUD BP는 미리 만들어 두고,
게임 로직은 나중에 source만 갱신하면 된다.

이게 현재 구조의 장점이다.

- HUD BP는 표현만 담당
- source는 상태 전달만 담당
- PlayerController는 HUD 생성과 생명주기만 담당

역할이 섞이지 않는다.

---

## 11. 핵심 요약

- HUD Widget Blueprint의 부모는 반드시 `UNCPlayerHUDWidget`으로 만든다.
- HUD 등록 지점은 현재 구조상 `PlayerController Blueprint`의 `PlayerHUDWidgetClass`다.
- 그 PlayerController BP가 실제 게임에서 사용되도록 `GameMode` 또는 `World Settings`에 연결해야 한다.
- HUD BP에서는 `RefreshView`를 구현하고 반드시 Parent를 먼저 호출한다.
- 실제 표시 갱신은 `ReticleOpacity`, `InteractionPromptText`, `SubtitleLineText` 같은 C++ 제공 변수로 처리한다.
- Tick이나 무거운 Designer Binding보다 현재 구조에서는 `RefreshView` 이벤트 갱신이 더 적합하다.

이 규칙만 지키면,
지금 만든 compact UI 구조 위에서 HUD를 안전하게 확장할 수 있다.