# NightCaretaker 런타임 프레임워크 설정 가이드

## 1. 문서 목적

이 문서는 최근 추가한 런타임 프레임워크 구조를 설명한다.
대상 범위는 아래와 같다.

- 업적 접근 계층 (`Achievement Backend`, `Achievement Subsystem`)
- `GameMode / GameState` 역할 분리
- `ShiftStateComponent`, `ComplaintRuntimeComponent`
- `ComplaintRuntimeSubsystem`
- 디버그용 `CheatManager`
- 이후 Steam Integration Kit를 붙이는 절차

이 문서는 "현재 구조가 왜 이렇게 나뉘었는지"와 "앞으로 어디에 기능을 추가해야 하는지"를 빠르게 파악하는 용도로 작성한다.

## 2. 현재 구조 한눈에 보기

```text
Gameplay / UI / Debug
    -> UNCComplaintRuntimeSubsystem (WorldSubsystem)
        -> ANCGameStateBase
            -> UNCShiftStateComponent
            -> UNCComplaintRuntimeComponent
        -> UNCAchievementSubsystem (GameInstanceSubsystem)
            -> UNCAchievementBackendBase
                -> UNCNullAchievementBackend (현재 기본값)
                -> Steam Integration Kit Adapter (추후 추가)
```

핵심 원칙은 아래와 같다.

- 월드 상태 변경은 `Subsystem`을 통해 들어간다.
- 상태 저장은 `GameState`에 붙은 컴포넌트가 가진다.
- 업적 플랫폼 호출은 gameplay code가 직접 하지 않는다.
- 디버그는 `CheatManager`를 통해 공통 진입점으로 노출한다.

## 3. 지금 구현된 것과 아직 없는 것

### 3.1 구현된 것

- 업적 백엔드 추상 클래스
- 기본 오프라인 백엔드 (`Null backend`)
- 업적 write 평가 및 dispatch용 `GameInstanceSubsystem`
- `GameInstance`에서 백엔드 클래스 선택 지점
- `GameState` 기반의 shift 상태 컴포넌트
- `GameState` 기반의 민원 런타임 상태 컴포넌트
- 민원 상태 제어용 `WorldSubsystem`
- 디버그용 `CheatManager` 명령 세트

### 3.2 아직 없는 것

- Steam Integration Kit 실제 어댑터 클래스
- 민원 보드 / 보고 UI에서 `ComplaintRuntimeSubsystem` 호출
- 챕터 DataTable 자동 로드 및 민원 순차 해금 로직
- 저장/로드 연결
- 실제 월드 상호작용 액터가 민원 상태를 변경하는 연결부

즉 지금은 "구조와 접근 계층이 준비된 상태"이고, 이후 gameplay loop를 안전하게 올릴 수 있는 기반이 생긴 단계다.

## 4. 업적 프레임워크 설명

### 4.1 클래스 역할

#### `UNCAchievementBackendBase`

플랫폼별 업적 구현체의 부모 클래스다.
이 클래스가 맡는 책임은 아래와 같다.

- backend 초기화
- 업적 write 처리
- backend 종료
- local debug snapshot 유지

현재는 `BlueprintNativeEvent`로 열어두었기 때문에, 나중에 Steam Integration Kit가 블루프린트 노드 중심이어도 연결할 수 있다.

#### `UNCNullAchievementBackend`

현재 기본 backend다.
실제 Steam 호출은 하지 않고 아래만 수행한다.

- 업적 unlock local 기록
- stat 증가 / 설정 local 기록
- threshold를 넘으면 local unlock 반영

즉 실제 플랫폼 없이도 `NCDumpAchievements`로 흐름 검증이 가능하다.

#### `UNCAchievementSubsystem`

게임플레이에서 업적에 접근할 때 반드시 거쳐야 하는 중앙 접근 계층이다.
아래 작업을 담당한다.

- `FNCAchievementWriteRequest` 평가
- progression tag 조건 확인
- 중복 `WriteId` 소비 여부 확인
- `AchievementDefinition` 로드
- backend에 dispatch
- 결과를 `FNCAchievementWriteResult`로 반환

중요한 규칙은 아래와 같다.

- 민원 시스템, UI, 월드 액터는 backend를 직접 호출하지 않는다.
- 항상 `UNCAchievementSubsystem`만 호출한다.

### 4.2 업적 write 흐름

```text
Complaint / Anomaly Event
    -> FNCAchievementWriteRequest
    -> UNCAchievementSubsystem
        -> 조건 검사
        -> AchievementDefinition 로드
        -> Active Backend 호출
            -> Unlock / IncrementStat / SetStat
        -> 결과 반환
```

### 4.3 `WriteMode` 해석 기준

- `Unlock`
  - 단발 업적 해금
  - Steam에서는 보통 `SetAchievement`에 대응
- `IncrementStat`
  - 현재 값에 누적
  - Steam에서는 stat 증가 + 필요 시 업적 해금
- `SetStat`
  - 절대값 설정
  - Steam에서는 stat set + 필요 시 업적 해금

### 4.4 현재 기준으로 빠진 부분 체크

지금 시점에서 업적 프레임워크에 실제로 남은 핵심 누락은 아래뿐이다.

- Steam Integration Kit adapter class
- `.uproject`에 플러그인 활성화
- 실제 `BP_GameInstance` 또는 프로젝트 기본 GameInstance에 backend class 지정

데이터 구조, write 결과, 중복 소비, debug snapshot, backend 선택 지점은 이미 있다.
즉 "프레임은 준비됐고, 실제 Steam 연결만 남은 상태"라고 보면 된다.

## 5. Steam Integration Kit 연결 절차

아래 절차를 추천한다.

### 5.1 Adapter 클래스 만들기

다음 중 하나를 선택한다.

- C++ subclass of `UNCAchievementBackendBase`
- Blueprint subclass of `UNCAchievementBackendBase`

Steam Integration Kit가 블루프린트 노드 기반이면 BP subclass가 더 빠를 가능성이 높다.

### 5.2 구현해야 하는 함수

- `InitializeBackend`
  - Steam Kit 초기 상태 확인
  - 필요한 참조 캐시
- `DispatchAchievementWrite`
  - `WriteMode` 분기
  - 업적 / stat 이름 읽기
  - Steam Kit 노드 호출
  - 필요 시 `StoreStats` 또는 flush 호출
- `ShutdownBackend`
  - 캐시 정리

### 5.3 BP 구현 시 권장 규칙

`DispatchAchievementWrite`를 BP에서 override할 때는 아래를 지킨다.

- 성공 시 `true`, 실패 시 `false`를 명확하게 반환한다.
- 실패 이유가 있으면 `OutFailureReason`을 채운다.
- local debug snapshot은 subsystem이 성공 후 자동 기록하므로, parent 호출은 필수가 아니다.

### 5.4 GameInstance에 연결하기

- `UNCGameInstance` 파생 BP 또는 기본 GameInstance 설정에서 `AchievementBackendClass`를 Steam adapter로 지정한다.
- 지정하지 않으면 기본값은 `UNCNullAchievementBackend`다.

### 5.5 Steam adapter 구현 체크리스트

- `Unlock`이면 `SteamAchievementApiName` 사용
- `IncrementStat` / `SetStat`이면 `SteamStatApiName` 사용
- `SteamUnlockThreshold > 0`이면 stat 결과와 비교해 업적도 처리
- 성공 시 `true`, 실패 시 `false` 반환
- 실패 시 `OutFailureReason`을 채워서 subsystem이 로그를 남길 수 있게 할 것

## 6. GameState 기반 상태 모듈 설명

### 6.1 왜 GameState에 붙였는가

민원 루프의 현재 상태는 아래 특성을 가진다.

- 여러 시스템이 읽어야 한다.
- UI, 디버그, GameMode, Subsystem이 공통으로 참조한다.
- 나중에 멀티플레이나 리플레이를 고려해도 `GameState` 쪽이 의미상 더 맞다.

그래서 상태는 `ANCGameStateBase`가 직접 들지 않고, 아래 두 컴포넌트로 쪼갰다.

### 6.2 `UNCShiftStateComponent`

이 컴포넌트는 "근무 전체의 상위 상태"를 담당한다.

보관하는 핵심 값:

- `CurrentChapterId`
- `ShiftPhase`
- `FocusedComplaintId`
- `ActiveProgressionTags`

이 컴포넌트는 아래 상황에서 주로 읽힌다.

- 민원 보드 UI가 현재 챕터를 보여줄 때
- 조사 중인지 보고 단계인지 UI를 바꿀 때
- 업적이나 민원 잠금 조건에서 progression tag를 확인할 때

### 6.3 `UNCComplaintRuntimeComponent`

이 컴포넌트는 "민원 개별 인스턴스 상태"를 담당한다.

보관하는 핵심 값:

- `ComplaintId`
- `RuntimeState`
- `DiscoveredEvidenceTags`
- `SubmittedReportResult`
- `RuntimeProgressTags`
- `ConsumedAchievementWriteIds`

이 컴포넌트는 아래 상황에서 주로 읽힌다.

- 현재 민원이 어디까지 진행됐는지
- 증거를 몇 개 모았는지
- 이미 보고했는지
- 같은 업적 write를 또 보내면 안 되는지

## 7. `ComplaintRuntimeSubsystem` 사용 기준

`UNCComplaintRuntimeSubsystem`은 "민원 루프의 단일 접근점"이다.
`GameMode`, UI, 디버그, 월드 액터는 이 subsystem을 통해 상태를 바꾼다.

### 7.1 지금 제공하는 함수

- `SetCurrentChapter`
- `SetShiftPhase`
- `RegisterComplaint`
- `AcceptComplaint`
- `BeginInvestigation`
- `SubmitReport`
- `CloseComplaint`
- `AddEvidenceTag`
- `SubmitComplaintAchievementEvent`
- `SubmitAnomalyAchievementEvent`

### 7.2 사용 원칙

- 보드 UI는 `AcceptComplaint`를 호출한다.
- 현장 조사 시작 연출이나 진입 트리거는 `BeginInvestigation`을 호출한다.
- 단서 확보 액터는 `AddEvidenceTag`를 호출한다.
- 보고 UI는 `SubmitReport`를 호출한다.
- 민원 종료 확정 시 `CloseComplaint`를 호출한다.
- 업적 event는 subsystem을 통해 dispatch한다.

### 7.3 하지 말아야 할 것

아래는 피하는 게 좋다.

- UI가 `GameState` 컴포넌트에 직접 값 쓰기
- 월드 액터가 achievement subsystem을 직접 호출하기
- GameMode가 민원 상태 배열을 직접 수정하기

이유는 access path가 여러 갈래가 되면 나중에 저장/로드와 디버그가 바로 꼬이기 때문이다.

## 8. GameMode / GameState 역할 분리

### 8.1 `ANCGameModeBase`

현재 책임은 의도적으로 얇다.

- `StartPlay`에서 기본 shift phase 지정
- world subsystem 접근 helper 제공

앞으로 여기에 들어갈 책임은 아래 정도가 적절하다.

- 챕터 시작 시 DataTable 로드
- 초기 민원 활성화
- 보고 결과에 따른 다음 민원 해금
- 전체 루프 sequencing

### 8.2 `ANCGameStateBase`

현재는 상태 저장용 modular shell이다.

- `ShiftStateComponent`
- `ComplaintRuntimeComponent`

즉 `GameModeBase`는 흐름을 관리하고, `GameStateBase`는 읽고 쓸 상태를 들고 있는 구조다.

## 9. 디버그 관리자 사용법

`ANCPlayerControllerBase`에 공통 `CheatManager`가 연결되어 있다.
따라서 콘솔에서 아래 명령을 바로 사용할 수 있다.

- `NCSetChapter CH_01`
- `NCSetShiftPhase 1`
- `NCSetFocusedComplaint CMP_2F203_LightBuzz`
- `NCAddProgressionTag Progression.Chapter.One`
- `NCRemoveProgressionTag Progression.Chapter.One`
- `NCRegisterComplaint CMP_2F203_LightBuzz`
- `NCAcceptComplaint CMP_2F203_LightBuzz`
- `NCBeginInvestigation CMP_2F203_LightBuzz`
- `NCSubmitComplaintReport CMP_2F203_LightBuzz 0`
- `NCCloseComplaint CMP_2F203_LightBuzz`
- `NCAddComplaintEvidence CMP_2F203_ShadowComplaint Evidence.Visual`
- `NCDumpShiftState`
- `NCDumpComplaints`
- `NCDumpAchievements`

### 9.1 enum 숫자 기준

`ENCShiftPhase`

- `0`: None
- `1`: BoardReview
- `2`: Investigating
- `3`: Reporting
- `4`: Suspended

`ENCReportResult`

- `0`: Resolved
- `1`: NoAnomaly
- `2`: NeedsFollowUp

## 10. 추천 다음 구현 순서

이 프레임 위에 바로 올리기 좋은 순서는 아래와 같다.

1. `BP_GameInstance`에 backend class 선택 가능하도록 asset 준비
2. Steam Integration Kit adapter backend 추가
3. 민원 보드 UI에서 `AcceptComplaint` 호출
4. 보고 UI에서 `SubmitReport` / `CloseComplaint` 호출
5. 월드 조사 포인트에서 `AddEvidenceTag` 연결
6. 챕터 DataTable 로드와 민원 해금 로직을 `GameModeBase`에 연결
7. 저장/로드에서 `ShiftStateComponent`, `ComplaintRuntimeComponent` 복원

## 11. 구현 원칙 정리

이 구조를 오래 쓰려면 아래만 지키면 된다.

- 플랫폼 업적 호출은 backend 안에만 둔다.
- gameplay는 subsystem만 호출한다.
- 상태는 GameState 컴포넌트가 들고, GameMode는 흐름만 관리한다.
- 디버그는 CheatManager를 통해 같은 경로를 탄다.
- UI와 월드 액터가 내부 배열이나 backend를 직접 건드리지 않는다.

## 12. 함께 보면 좋은 문서

데이터 자산 설정 자체는 아래 문서를 함께 보면 된다.

- `NightCaretaker_ComplaintData_Authoring_Guide.md`

이 문서는 "어떤 데이터를 넣을지"를 설명하고,
현재 문서는 "그 데이터가 런타임에서 어디로 흐를지"를 설명한다.