# NightCaretaker 민원 / 이상 현상 / 업적 데이터 설정 가이드

## 1. 문서 목적

이 문서는 현재 C++에 구현된 데이터 구조를 기준으로, 에디터에서 어떤 자산을 어떤 순서로 만들고 어떤 값을 넣어야 하는지 설명한다.
읽으면서 바로 자산을 만들 수 있도록 아래 4가지를 중심으로 정리한다.

- 무엇을 먼저 만들지
- 각 필드에 무엇을 넣을지
- 민원, 이상 현상, 업적이 서로 어떻게 연결되는지
- 실제 런타임에서 어떤 흐름으로 소비될 예정인지

## 2. 현재 구현 범위

현재 코드 기준으로 구현된 범위는 아래와 같다.

- `UNCAchievementDefinition`: 업적 정의 Data Asset
- `UNCAnomalyDefinition`: 이상 현상 정의 Data Asset
- `UNCComplaintDefinition`: 민원 정의 Data Asset
- `FNCChapterComplaintRow`: 챕터 편성용 DataTable Row
- `FNCComplaintRuntimeData`: 런타임 상태 저장 구조
- Steam 업적 연결용 필드: `SteamAchievementApiName`, `SteamStatApiName`, `SteamUnlockThreshold`
- 업적 발동 이벤트 구조: Complaint / Anomaly 양쪽 모두 지원

아직 구현되지 않은 범위도 있다.

- `GameMode / GameState`가 이 데이터를 끝까지 소비하는 런타임 브리지
- 실제 Steam API 호출기 (`SetAchievement`, `SetStat`, `StoreStats`)
- 민원 보드 / 보고 UI와의 완전한 연결

즉, 지금은 "정확한 데이터 자산 구조를 먼저 구축하는 단계"라고 보면 된다.

## 3. 전체 구조 한눈에 보기

```text
DataTable: FNCChapterComplaintRow
    -> UNCComplaintDefinition
        -> LinkedAnomalies: UNCAnomalyDefinition[]
        -> AchievementEvents: FNCComplaintAchievementEvent[]
            -> FNCAchievementWriteRequest
                -> UNCAchievementDefinition

UNCAnomalyDefinition
    -> AchievementEvents: FNCAnomalyAchievementEvent[]
        -> FNCAchievementWriteRequest
            -> UNCAchievementDefinition

Runtime
    -> FNCComplaintRuntimeData
        -> DiscoveredEvidenceTags
        -> SubmittedReportResult
        -> RuntimeProgressTags
        -> ConsumedAchievementWriteIds
```

핵심 원칙은 아래와 같다.

- 정확한 식별은 `FName`으로 한다.
- 고정된 상태나 템플릿은 `enum`으로 한다.
- 확장 가능성이 큰 분류/조건은 `GameplayTag`로 한다.
- Steam 문자열은 민원 자산이 아니라 업적 정의 자산에 모은다.

## 4. 추천 폴더 구조

코드가 강제하는 구조는 아니지만, 아래처럼 정리하는 것을 권장한다.

```text
/Game/NightCaretaker/Data/Achievement/
/Game/NightCaretaker/Data/Anomaly/
/Game/NightCaretaker/Data/Complaint/
/Game/NightCaretaker/Data/Chapter/
```

추천 이유는 간단하다.

- 업적은 여러 민원과 이상 현상이 공용 참조할 수 있다.
- 이상 현상은 여러 민원이 재사용할 수 있다.
- 민원은 챕터 DataTable에서 순서만 관리하는 편이 깔끔하다.

## 5. 먼저 만들어야 하는 순서

아래 순서를 그대로 따르면 가장 덜 꼬인다.

1. GameplayTag 확인
2. `UNCAchievementDefinition` 자산 생성
3. `UNCAnomalyDefinition` 자산 생성
4. `UNCComplaintDefinition` 자산 생성
5. `FNCChapterComplaintRow` DataTable 생성
6. 챕터별 정렬 / 잠금 조건 입력
7. 이후 런타임 브리지 연결

이 순서를 추천하는 이유는 의존성 때문이다.

- 민원은 이상 현상을 참조한다.
- 민원과 이상 현상은 업적 정의를 참조한다.
- 챕터 DataTable은 민원 정의를 참조한다.

## 6. ID, Enum, Tag를 언제 쓰는가

### 6.1 FName

`FName`은 "정확히 하나를 가리키는 값"에 쓴다.

- `ComplaintId`
- `AnomalyId`
- `AchievementId`
- `LocationId`
- `ChapterId`
- `WriteId`

권장 규칙은 아래와 같다.

- 민원: `CMP_...`
- 이상 현상: `ANM_...`
- 업적: `ACH_...`
- 위치: `LOC_...`
- 챕터: `CH_...`
- 업적 쓰기 이벤트: `WR_...`

예시:

- `CMP_2F203_LightBuzz`
- `ANM_2F203_ShadowUnderDoor`
- `ACH_FirstCorrectReport`
- `LOC_2F_203`
- `WR_CMP_2F203_LightBuzz_Closed_FirstResolve`

주의할 점:

- 저장 데이터와 연결될 수 있으므로 배포 후에는 가급적 이름을 바꾸지 않는다.
- 위치를 태그로 표현하지 말고 `LocationId`로 고정한다.

### 6.2 Enum

enum은 "개수가 작고 코드 분기가 고정된 값"에 쓴다.

- `ENCComplaintTemplateType`
- `ENCReportResult`
- `ENCComplaintRuntimeState`
- `ENCAchievementWriteMode`
- `ENCComplaintAchievementTrigger`
- `ENCAnomalyAchievementTrigger`

### 6.3 GameplayTag

GameplayTag는 "확장될 수 있는 분류나 조건"에 쓴다.

현재 등록된 축은 아래와 같다.

- `Complaint.Domain.*`
- `Complaint.RequiredTool.*`
- `Complaint.Flag.*`
- `Anomaly.Type.*`
- `Achievement.Category.*`
- `Evidence.*`
- `Progression.*`

원칙은 아래처럼 잡으면 된다.

- 분류: 태그
- 위치: `LocationId`
- 상태 머신: enum
- 저장 키: `FName`

## 7. 업적 정의 자산 설정 방법

업적 자산은 `UNCAchievementDefinition`으로 만든다.
이 자산은 "업적 자체의 정체성"과 "Steam 쪽 이름"을 담당한다.
민원이나 이상 현상은 이 자산을 참조만 한다.

### 7.1 필드 설명

- `AchievementId`
  - 프로젝트 내부에서 쓰는 안정적인 ID다.
  - 로컬 저장, 디버그, 다른 백엔드 전환 시 기준점이 된다.
- `DisplayName`
  - 에디터와 디버그 UI에서 읽기 쉬운 이름이다.
  - Steam 스토어 문자열과 반드시 같을 필요는 없다.
- `Description`
  - 디자이너 메모다.
  - 해금 조건을 짧게 적어두면 나중에 유지보수가 쉽다.
- `CategoryTags`
  - `Achievement.Category.*`를 사용한다.
  - 예: `Story`, `Investigation`, `Exploration`, `Performance`, `Hidden`, `System`
- `DefaultWriteMode`
  - `Unlock`: 단발 해금
  - `IncrementStat`: 누적 스탯 증가
  - `SetStat`: 스탯을 특정 값으로 설정
- `bHiddenUntilUnlocked`
  - 향후 로컬 UI나 프로필 화면에서 숨김 처리를 할 때 쓴다.
- `SteamAchievementApiName`
  - Steamworks 업적 API 이름이다.
  - 예: `ACH_FIRST_CORRECT_REPORT`
- `SteamStatApiName`
  - 누적형 업적에 연결할 Steam stat 이름이다.
  - 예: `STAT_COMPLAINTS_CLOSED`
- `SteamUnlockThreshold`
  - stat 누적형 업적에서 몇 이상일 때 업적을 해금할지 정의한다.

### 7.2 추천 입력 규칙

- 단발 업적이면 `DefaultWriteMode = Unlock`
- 누적형 업적이면 `DefaultWriteMode = IncrementStat`
- 누적형 업적은 가능하면 `SteamAchievementApiName`과 `SteamStatApiName`을 둘 다 채운다.
- `SteamUnlockThreshold`는 stat 업적일 때만 1 이상으로 넣는다.

### 7.3 단발 업적 예시

- `AchievementId`: `ACH_FirstCorrectReport`
- `DisplayName`: `첫 정확한 보고`
- `CategoryTags`: `Achievement.Category.Story`
- `DefaultWriteMode`: `Unlock`
- `SteamAchievementApiName`: `ACH_FIRST_CORRECT_REPORT`
- `SteamStatApiName`: 비움
- `SteamUnlockThreshold`: `0`

### 7.4 누적 업적 예시

- `AchievementId`: `ACH_Close10Complaints`
- `DisplayName`: `야간 근무 적응`
- `CategoryTags`: `Achievement.Category.System`
- `DefaultWriteMode`: `IncrementStat`
- `SteamAchievementApiName`: `ACH_CLOSE_10_COMPLAINTS`
- `SteamStatApiName`: `STAT_COMPLAINTS_CLOSED`
- `SteamUnlockThreshold`: `10`

## 8. 이상 현상 자산 설정 방법

이상 현상 자산은 `UNCAnomalyDefinition`으로 만든다.
이 자산은 "이상 현상이 무엇인지"와 "발견 시 어떤 증거/업적이 연결되는지"를 정의한다.

### 8.1 필드 설명

- `AnomalyId`
  - 이상 현상의 고유 ID다.
- `DisplayName`
  - 디버그나 툴에서 읽기 쉬운 이름이다.
- `Description`
  - 어떤 연출인지, 어떤 상황에서 보이는지 설명한다.
- `AnomalyTypeTag`
  - `Anomaly.Type.*` 중 하나를 넣는다.
- `EvidenceTagsGranted`
  - 플레이어가 이 이상 현상을 확인했을 때 획득하는 증거 태그다.
- `AchievementEvents`
  - 이 이상 현상이 업적 발동 조건이 될 때 사용한다.
- `ActivationTags`
  - 어떤 진행 상태에서 이 이상 현상이 나타날 수 있는지 제한한다.
- `PresentationTags`
  - 조명, 사운드, 공간 연출 시스템이 공통 반응할 때 사용한다.
- `ConsequenceTags`
  - 확인 또는 해소 후 후속 진행에 반영할 태그다.

### 8.2 AchievementEvents의 의미

- `Discovered`
  - 월드에서 처음 발견했을 때
- `Confirmed`
  - 플레이어가 명시적으로 판정하거나 확인했을 때
- `Resolved`
  - 연출 또는 보고 결과로 마무리되었을 때

현재 런타임 브리지가 아직 없으므로, 이 값은 "다음 단계에서 어떤 타이밍으로 처리할지 미리 명세하는 계약"이라고 생각하면 된다.

### 8.3 예시

문 아래 그림자 이상 현상:

- `AnomalyId`: `ANM_2F203_ShadowUnderDoor`
- `DisplayName`: `203호 문 아래 그림자`
- `AnomalyTypeTag`: `Anomaly.Type.ShadowUnderDoor`
- `EvidenceTagsGranted`:
  - `Evidence.Visual`
  - `Evidence.Environmental`
- `ActivationTags`:
  - `Progression.Chapter.One`

## 9. 민원 자산 설정 방법

민원 자산은 `UNCComplaintDefinition`으로 만든다.
이 자산이 실제 플레이 루프의 중심이다.

### 9.1 필드 설명

- `ComplaintId`
  - 민원의 고유 ID다.
- `Title`
  - 민원 보드 제목이다.
- `BoardSummary`
  - 보드에서 짧게 보여줄 설명이다.
- `InternalNote`
  - 리포트 UI나 내부 메모 성격의 텍스트다.
- `TemplateType`
  - `Repair`, `Inspection`, `AnomalyJudgement` 중 하나다.
- `LocationId`
  - 민원이 연결되는 정확한 위치 ID다.
- `DomainTags`
  - `Lighting`, `Door`, `Water` 같은 도메인 분류다.
- `RequiredToolTags`
  - 손전등, 마스터키 등 필요한 도구 조건이다.
- `FlagTags`
  - 튜토리얼, 필수, 반복 가능 여부 같은 저작 플래그다.
- `LinkedAnomalies`
  - 이 민원 조사 중 등장 가능한 이상 현상 목록이다.
- `RequiredEvidenceTags`
  - 보고 전에 확보해야 하는 핵심 증거 태그다.
- `MinEvidenceCountForReport`
  - 몇 개 이상의 증거를 봐야 보고 가능하다고 볼지 정한다.
- `AllowedReportResults`
  - UI에서 선택 가능한 보고 결과 목록이다.
- `DefaultCanonicalResult`
  - 현재 프로토타입 기준의 기본 정답 또는 기준 결과다.
- `AchievementEvents`
  - 민원 lifecycle에 연결된 업적 이벤트다.
- `ActivationTags`
  - 이 민원이 열리기 전에 필요한 진행 태그다.
- `CompletionTags`
  - 완료 후 부여할 진행 태그다.
- `ConsequenceTags`
  - 보고 결과 이후의 후속 상태 변화에 쓰일 태그다.

### 9.2 TemplateType 해석 기준

- `Repair`
  - 정상 고장 해결 중심 민원
- `Inspection`
  - 상태 확인, 현장 조사 중심 민원
- `AnomalyJudgement`
  - 정상 / 이상 여부를 판정하는 민원

헷갈리면 아래 기준으로 고르면 된다.

- 수리 행위가 핵심이면 `Repair`
- 확인과 기록이 핵심이면 `Inspection`
- 보고 판정이 핵심이면 `AnomalyJudgement`

### 9.3 AllowedReportResults 작성 원칙

- 비워두지 않는다.
- 실제 UI에 노출할 선택지만 넣는다.
- 정상 민원은 보통 `Resolved` 하나로 충분하다.
- 이상 판정 민원은 `NoAnomaly`, `NeedsFollowUp` 조합이 자주 쓰인다.

### 9.4 LinkedAnomalies 작성 원칙

- 이상 현상 정의를 중복 작성하지 말고 재사용한다.
- 같은 이상 현상이 여러 민원에 재사용되어도 괜찮다.
- 이상 현상이 없는 정상 민원은 비워둘 수 있다.

### 9.5 예시 1: 정상 수리 민원

- `ComplaintId`: `CMP_2F203_LightBuzz`
- `Title`: `203호 형광등 점검`
- `TemplateType`: `Repair`
- `LocationId`: `LOC_2F_203`
- `DomainTags`: `Complaint.Domain.Lighting`
- `RequiredToolTags`: `Complaint.RequiredTool.Flashlight`
- `LinkedAnomalies`: 비움
- `RequiredEvidenceTags`: 비움
- `MinEvidenceCountForReport`: `0`
- `AllowedReportResults`: `Resolved`
- `DefaultCanonicalResult`: `Resolved`

### 9.6 예시 2: 이상 판정 민원

- `ComplaintId`: `CMP_2F203_ShadowComplaint`
- `Title`: `203호 문 앞 이상 소음`
- `TemplateType`: `AnomalyJudgement`
- `LocationId`: `LOC_2F_203`
- `DomainTags`: `Complaint.Domain.Door`
- `RequiredToolTags`: `Complaint.RequiredTool.Flashlight`
- `LinkedAnomalies`: `ANM_2F203_ShadowUnderDoor`
- `RequiredEvidenceTags`:
  - `Evidence.Visual`
  - `Evidence.Environmental`
- `MinEvidenceCountForReport`: `2`
- `AllowedReportResults`:
  - `NoAnomaly`
  - `NeedsFollowUp`
- `DefaultCanonicalResult`: `NeedsFollowUp`

## 10. 민원 업적 이벤트 설정 방법

민원 업적은 `FNCComplaintAchievementEvent`로 작성한다.
이 구조는 "언제", "어떤 조건에서", "어떤 업적 정의를 어떤 값으로 기록할지"를 뜻한다.

### 10.1 Trigger 의미

- `Accepted`: 민원을 수주했을 때
- `InvestigationStarted`: 현장 조사 시작 시점
- `ReportSubmitted`: 보고 제출 시점
- `Closed`: 민원이 최종 종료되었을 때

### 10.2 필드 설명

- `RequiredReportResults`
  - 특정 보고 결과에서만 업적이 발동해야 할 때 사용한다.
  - 비워두면 결과 상관없이 평가 가능하다고 본다.
- `RequiredEvidenceTags`
  - 반드시 확보된 증거가 있어야 할 때 사용한다.
- `MinDiscoveredEvidenceCount`
  - 증거 개수 기준을 추가로 제한한다.
- `AchievementWrite`
  - 실제 업적 정의와 write 조건이다.

### 10.3 AchievementWrite 필드 설명

- `WriteId`
  - 같은 이벤트가 중복 호출될 때 한 번만 처리하려면 반드시 안정적으로 고정한다.
- `Achievement`
  - 참조할 `UNCAchievementDefinition` 자산이다.
- `ProgressValue`
  - `Unlock`이면 사실상 무시해도 된다.
  - `IncrementStat`이면 증가량이다.
  - `SetStat`이면 최종 설정 값이다.
- `bConsumeWriteOnce`
  - `true`면 같은 민원 인스턴스 안에서 한 번만 처리한다.
- `RequiredProgressionTags`
  - 특정 챕터, 긴장도, 스토리 상태에서만 발동시키고 싶을 때 사용한다.
- `BlockedByProgressionTags`
  - 특정 상태가 켜져 있으면 발동하지 않게 막는다.

### 10.4 예시 1: 첫 민원 완료 업적

민원 `Closed` 시 업적 해금:

- `Trigger`: `Closed`
- `RequiredReportResults`: `Resolved`
- `AchievementWrite.WriteId`: `WR_CMP_2F203_LightBuzz_FirstResolve`
- `AchievementWrite.Achievement`: `ACH_FirstCorrectReport`
- `AchievementWrite.ProgressValue`: `1`
- `AchievementWrite.bConsumeWriteOnce`: `true`

### 10.5 예시 2: 민원 10회 완료 누적 업적

모든 민원 종료 시 stat 1 증가:

- `Trigger`: `Closed`
- `AchievementWrite.WriteId`: `WR_Generic_ComplaintClosed_Count`
- `AchievementWrite.Achievement`: `ACH_Close10Complaints`
- `AchievementWrite.ProgressValue`: `1`
- `AchievementWrite.bConsumeWriteOnce`: `true`

주의:

- 이 write는 여러 민원 자산에 공통으로 넣을 수 있다.
- 다만 `WriteId`는 소비 기준이 source asset별인지 전역인지 나중에 브리지에서 정해야 하므로, 현재는 "자산 안에서 의미가 명확한 이름"으로 짓는 것을 권장한다.

## 11. 이상 현상 업적 이벤트 설정 방법

이상 현상 업적은 `FNCAnomalyAchievementEvent`로 작성한다.
민원 업적보다 단순하고, 트리거와 업적 write만 가진다.

추천 용도는 아래와 같다.

- 특정 이상 현상을 처음 발견했을 때 해금
- 특정 이상 현상을 올바르게 확인했을 때 해금
- 특정 이상 현상 해소 횟수 누적

예시:

- `Trigger`: `Discovered`
- `AchievementWrite.WriteId`: `WR_ANM_2F203_Shadow_Discovered`
- `AchievementWrite.Achievement`: `ACH_FirstAnomalyWitness`
- `AchievementWrite.ProgressValue`: `1`
- `AchievementWrite.bConsumeWriteOnce`: `true`

## 12. 챕터 DataTable 설정 방법

챕터 편성은 `FNCChapterComplaintRow` DataTable에서 한다.
여기에는 민원 본문을 넣지 않는다.
본문은 항상 `UNCComplaintDefinition` 자산에만 둔다.

### 12.1 필드 설명

- `RowId`
  - DataTable 내부 식별자다.
  - 디버그, 정렬, 검색에 쓰기 쉬운 이름으로 둔다.
- `ChapterId`
  - 이 민원이 소속된 챕터 ID다.
- `Complaint`
  - 참조할 민원 정의 자산이다.
- `SortOrder`
  - 보드에서 보이는 순서다.
- `RequiredProgressionTags`
  - 이 행이 열리기 전에 반드시 있어야 하는 진행 태그다.
- `BlockedByTags`
  - 이 태그가 있으면 열리면 안 되는 행이다.
- `bStartAvailable`
  - 챕터 시작 시 바로 후보에 넣을지 여부다.

### 12.2 예시

챕터 1의 첫 번째 민원:

- `RowId`: `ROW_CH1_010_LightBuzz`
- `ChapterId`: `CH_01`
- `Complaint`: `CMP_2F203_LightBuzz`
- `SortOrder`: `10`
- `RequiredProgressionTags`: 비움
- `BlockedByTags`: 비움
- `bStartAvailable`: `true`

챕터 1의 후속 민원:

- `RowId`: `ROW_CH1_020_ShadowComplaint`
- `ChapterId`: `CH_01`
- `Complaint`: `CMP_2F203_ShadowComplaint`
- `SortOrder`: `20`
- `RequiredProgressionTags`: `Progression.Story.Room307Clue`
- `bStartAvailable`: `false`

## 13. 실제 구성 흐름 예시

"이상 민원 하나 + 업적 두 개"를 만드는 흐름은 아래처럼 잡으면 된다.

1. 업적 자산 생성
2. 이상 현상 자산 생성
3. 민원 자산 생성
4. 챕터 DataTable에 민원 등록
5. 나중에 런타임 브리지가 들어오면 같은 구조를 그대로 소비

### 13.1 업적 자산 2개 생성

- `ACH_FirstAnomalyWitness`
  - 첫 이상 현상 발견
  - `DefaultWriteMode = Unlock`
- `ACH_Close10Complaints`
  - 민원 10회 종료
  - `DefaultWriteMode = IncrementStat`
  - `SteamStatApiName = STAT_COMPLAINTS_CLOSED`
  - `SteamUnlockThreshold = 10`

### 13.2 이상 현상 자산 생성

- `ANM_2F203_ShadowUnderDoor`
- `AnomalyTypeTag = Anomaly.Type.ShadowUnderDoor`
- `EvidenceTagsGranted = Evidence.Visual, Evidence.Environmental`
- `AchievementEvents`
  - `Discovered -> ACH_FirstAnomalyWitness`

### 13.3 민원 자산 생성

- `CMP_2F203_ShadowComplaint`
- `TemplateType = AnomalyJudgement`
- `LinkedAnomalies = ANM_2F203_ShadowUnderDoor`
- `RequiredEvidenceTags = Evidence.Visual, Evidence.Environmental`
- `MinEvidenceCountForReport = 2`
- `AllowedReportResults = NoAnomaly, NeedsFollowUp`
- `DefaultCanonicalResult = NeedsFollowUp`
- `AchievementEvents`
  - `Closed -> ACH_Close10Complaints`, `ProgressValue = 1`

### 13.4 DataTable 등록

- `ChapterId = CH_01`
- `SortOrder = 20`
- `bStartAvailable = false`
- `RequiredProgressionTags = Progression.Story.Room307Clue`

## 14. 자주 하는 실수

- `LocationId` 대신 도메인 태그에 위치 정보를 넣는 것
- `AllowedReportResults`를 비워두는 것
- `DefaultWriteMode = IncrementStat`인데 `SteamStatApiName`을 비워두는 것
- 업적 자산 없이 Complaint / Anomaly 이벤트에 write만 넣는 것
- `WriteId`를 임시 이름으로 두고 나중에 계속 바꾸는 것
- 민원의 `RequiredEvidenceTags`와 이상 현상의 `EvidenceTagsGranted`가 서로 맞지 않는 것
- 반복 업적인데 `bConsumeWriteOnce = true`를 무심코 켜두는 것

## 15. 최종 체크리스트

자산 하나를 만들 때 아래 순서로 확인하면 된다.

- ID가 안정적인가
- 위치는 `LocationId`에 들어갔는가
- 도메인 / 도구 / 플래그는 태그로 정리됐는가
- 보고 결과는 비어 있지 않은가
- 이상 현상 연결이 필요한가
- 증거 태그가 실제로 회수 가능한 구조인가
- 업적이 필요하면 업적 자산부터 만들었는가
- `WriteId`가 중복되지 않는가
- stat 업적이면 `SteamStatApiName`과 `SteamUnlockThreshold`가 맞는가
- 챕터 DataTable에서 순서와 잠금 조건이 맞는가

## 16. 이후 구현에서 이 데이터가 소비되는 흐름

향후 런타임 브리지가 들어오면 흐름은 아래처럼 연결된다.

1. 챕터 DataTable이 현재 챕터의 민원 후보를 결정한다.
2. 플레이어가 민원을 수주하면 `Accepted` 상태로 전이한다.
3. 조사 중 이상 현상과 증거 태그를 수집한다.
4. 보고 시 `SubmittedReportResult`와 증거 충족 여부를 평가한다.
5. 민원 / 이상 현상 `AchievementEvents`를 검사한다.
6. 유효한 `FNCAchievementWriteRequest`를 업적 브리지에 전달한다.
7. 업적 브리지는 `UNCAchievementDefinition`을 읽어 Steam API 이름과 stat 이름으로 변환한다.
8. 중복 write는 `ConsumedAchievementWriteIds`로 막는다.

즉, 지금 작성하는 데이터는 나중에 그대로 런타임과 Steam 연동 계층에 연결되는 전제 정보다.

## 17. 한 줄 기준 정리

빠르게 기억하려면 아래 한 줄로 정리하면 된다.

- 업적은 `UNCAchievementDefinition`에 정의한다.
- 이상 현상은 `UNCAnomalyDefinition`에 정의한다.
- 민원은 `UNCComplaintDefinition`에 정의한다.
- 챕터 순서는 `FNCChapterComplaintRow` DataTable에서 관리한다.
- 정확한 ID는 `FName`, 고정 분기는 enum, 확장 조건은 GameplayTag를 쓴다.