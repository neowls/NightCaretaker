# 야간 관리인: 307호의 민원 - Complaint Data Mapping

## 문서 정보

- 문서 종류: 데이터 자산 매핑 문서
- 버전: v0.1
- 작성일: 2026-03-29
- 목적: `NightCaretaker_Complaint_20_Pack.md`의 20개 민원을 현재 런타임 스키마인 `UNCComplaintDefinition`, `UNCAnomalyDefinition`, `FNCChapterComplaintRow` 기준으로 바로 입력 가능한 수준으로 정리한다.
- 기준 문서: `NightCaretaker_Complaint_20_Pack.md`, `NightCaretaker_307_Staging_BeatSheet.md`, `NightCaretaker_ComplaintData_Authoring_Guide.md`

## 1. 고정 매핑 규칙

- `ENCComplaintTemplateType`
  - 수리형, 복구형 -> `Repair`
  - 점검형, 기록형, 접근형 -> `Inspection`
  - 판정형 -> `AnomalyJudgement`
- `ENCReportResult`
  - 해결 완료 -> `Resolved`
  - 이상 없음 -> `NoAnomaly`
  - 추가 확인 필요 -> `NeedsFollowUp`
- `AchievementEvents`
  - 이번 단계에서는 전부 비운다.
- `MinEvidenceCountForReport`
  - 튜토리얼 직선형 -> `1`
  - 일반 수리/점검형 -> `2`
  - 기록 붕괴 / 후반 307 관련 -> `3`
- `AllowedReportResults`
  - 문서에 적힌 결과만 허용한다.
- `bStartAvailable`
  - 각 챕터의 첫 민원만 `true`
- `BlockedByTags`
  - 중복 재활성화 제어가 필요한 민원 외에는 비운다.

## 2. 추천 자산 경로

```text
/Game/NightCaretaker/Data/Complaint/
/Game/NightCaretaker/Data/Anomaly/
/Game/NightCaretaker/Data/Chapter/
```

## 3. Complaint Definition Mapping

### CMP_PRO_OfficeLightBuzz

- Title: `관리실 형광등 깜빡임`
- BoardSummary: `야간 근무 시작 전 관리실 형광등 안정화 필요`
- InternalNote: `튜토리얼 시작 민원. 관리실 조명과 인터폰 잡음이 가볍게 연결되며, 3F 07 흔적을 첫 단서로 심는다.`
- TemplateType: `Repair`
- LocationId: `LOC_OFFICE_MAIN`
- DomainTags: `Complaint.Domain.Lighting`, `Complaint.Domain.Power`
- RequiredToolTags: `Complaint.RequiredTool.Flashlight`, `Complaint.RequiredTool.Screwdriver`
- FlagTags: `Complaint.Flag.Tutorial`, `Complaint.Flag.Mandatory`
- LinkedAnomalies: `ANM_OFFICE_3F07_NOTE`
- RequiredEvidenceTags: `Evidence.Visual`
- MinEvidenceCountForReport: `1`
- AllowedReportResults: `Resolved`, `NeedsFollowUp`
- DefaultCanonicalResult: `Resolved`
- ActivationTags: `Progression.Chapter.Prologue`
- CompletionTags: `Progression.Story.Room307Clue`
- ConsequenceTags: `Progression.Tension.Stage0`

### CMP_PRO_203_WaterAtDoor

- Title: `203호 문 앞 물기 확인`
- BoardSummary: `203호 앞 물기 원인 확인`
- InternalNote: `천장과 벽은 마른 상태인데 문틈 주변만 차갑고 축축하다. 설명 가능한 누수처럼 시작하지만 원인이 끝까지 불명확해야 한다.`
- TemplateType: `Inspection`
- LocationId: `LOC_2F_203_DOOR`
- DomainTags: `Complaint.Domain.Water`, `Complaint.Domain.Contamination`
- RequiredToolTags: `Complaint.RequiredTool.Flashlight`
- FlagTags: `Complaint.Flag.Tutorial`, `Complaint.Flag.Mandatory`
- LinkedAnomalies: `ANM_203_WATER_WITHOUT_SOURCE`
- RequiredEvidenceTags: `Evidence.Visual`, `Evidence.Environmental`
- MinEvidenceCountForReport: `1`
- AllowedReportResults: `NoAnomaly`, `NeedsFollowUp`
- DefaultCanonicalResult: `NeedsFollowUp`
- ActivationTags: `Progression.Chapter.Prologue`, `Progression.Story.Room307Clue`
- CompletionTags: `Progression.Chapter.One`
- ConsequenceTags: `Progression.Tension.Stage0`

### CMP_CH1_2F_IntercomStatic

- Title: `2층 인터폰 잡음`
- BoardSummary: `2층 복도 인터폰 배선 및 수신 상태 점검`
- InternalNote: `설비 고장처럼 보이지만 특정 세대 호출 구간에서만 생활 소음 파편과 307 관련 끼어듦이 생긴다.`
- TemplateType: `Repair`
- LocationId: `LOC_2F_INTERCOM`
- DomainTags: `Complaint.Domain.Intercom`, `Complaint.Domain.Power`
- RequiredToolTags: `Complaint.RequiredTool.Flashlight`, `Complaint.RequiredTool.Screwdriver`
- FlagTags: `Complaint.Flag.Mandatory`
- LinkedAnomalies: `ANM_INTERCOM_VOICE`
- RequiredEvidenceTags: `Evidence.Audio`, `Evidence.Visual`
- MinEvidenceCountForReport: `2`
- AllowedReportResults: `Resolved`, `NeedsFollowUp`
- DefaultCanonicalResult: `NeedsFollowUp`
- ActivationTags: `Progression.Chapter.One`
- CompletionTags: `Progression.Story.IntercomStaticObserved`
- ConsequenceTags: `Progression.Tension.Stage1`

### CMP_CH1_1F_MailboxMisdelivery

- Title: `우편함 오배송 표기`
- BoardSummary: `1층 우편함 이름표와 명부 정보 대조`
- InternalNote: `기록 기반 공포의 첫 진입. 우편함 라벨 조각과 명부 배열 어긋남으로 307 흔적을 문서형 단서로 남긴다.`
- TemplateType: `Inspection`
- LocationId: `LOC_1F_MAILBOX`
- DomainTags: `Complaint.Domain.Record`
- RequiredToolTags: `Complaint.RequiredTool.Notebook`
- FlagTags: `Complaint.Flag.Mandatory`
- LinkedAnomalies: `ANM_MAILBOX_RECORD_CONFLICT`
- RequiredEvidenceTags: `Evidence.Document`, `Evidence.Records`
- MinEvidenceCountForReport: `2`
- AllowedReportResults: `NoAnomaly`, `NeedsFollowUp`
- DefaultCanonicalResult: `NeedsFollowUp`
- ActivationTags: `Progression.Chapter.One`, `Progression.Story.IntercomStaticObserved`
- CompletionTags: `Progression.Story.RecordMismatchNoted`
- ConsequenceTags: `Progression.Tension.Stage1`

### CMP_CH1_3F_EmergencyLight

- Title: `3층 비상등 상시 점등`
- BoardSummary: `3층 비상등 전원/배터리 상태 확인`
- InternalNote: `3층이 다른 층보다 먼저 불안정해지고 있다는 감각을 조명으로 만든다.`
- TemplateType: `Repair`
- LocationId: `LOC_3F_EMERGENCY_LIGHT`
- DomainTags: `Complaint.Domain.Lighting`, `Complaint.Domain.Power`
- RequiredToolTags: `Complaint.RequiredTool.Flashlight`, `Complaint.RequiredTool.Screwdriver`
- FlagTags: `Complaint.Flag.Mandatory`
- LinkedAnomalies: `ANM_3F_PERSISTENT_EMERGENCY_LIGHT`
- RequiredEvidenceTags: `Evidence.Visual`, `Evidence.Environmental`
- MinEvidenceCountForReport: `2`
- AllowedReportResults: `Resolved`, `NeedsFollowUp`
- DefaultCanonicalResult: `NeedsFollowUp`
- ActivationTags: `Progression.Chapter.One`, `Progression.Story.RecordMismatchNoted`
- CompletionTags: `Progression.Story.ThreeFloorUnstable`
- ConsequenceTags: `Progression.Tension.Stage1`

### CMP_CH1_205_OdorAtDoor

- Title: `205호 문 앞 이상 냄새`
- BoardSummary: `가스 누출 또는 오염 여부 확인`
- InternalNote: `오염과 환기 불량 공포를 여는 민원. 직접적인 위험보다 눅눅한 불쾌함이 앞서야 한다.`
- TemplateType: `Inspection`
- LocationId: `LOC_2F_205_DOOR`
- DomainTags: `Complaint.Domain.Contamination`, `Complaint.Domain.Water`
- RequiredToolTags: `Complaint.RequiredTool.Flashlight`, `Complaint.RequiredTool.Thermometer`
- FlagTags: `Complaint.Flag.Mandatory`
- LinkedAnomalies: `ANM_205_WATER_WITHOUT_SOURCE`
- RequiredEvidenceTags: `Evidence.Environmental`, `Evidence.Visual`
- MinEvidenceCountForReport: `2`
- AllowedReportResults: `NoAnomaly`, `NeedsFollowUp`
- DefaultCanonicalResult: `NeedsFollowUp`
- ActivationTags: `Progression.Chapter.One`, `Progression.Story.ThreeFloorUnstable`
- CompletionTags: `Progression.Tension.Stage1`
- ConsequenceTags: `Progression.Story.Room307Clue`

### CMP_CH1_StairAutoLightDelay

- Title: `계단실 자동등 지연`
- BoardSummary: `계단실 센서등 반응 시간 점검`
- InternalNote: `불이 켜지기 전 1초를 공포 자원으로 쓰는 민원. 이후 계단실을 긴장 경로로 만든다.`
- TemplateType: `Repair`
- LocationId: `LOC_STAIR_1F_2F`
- DomainTags: `Complaint.Domain.Lighting`, `Complaint.Domain.Power`
- RequiredToolTags: `Complaint.RequiredTool.Flashlight`, `Complaint.RequiredTool.Screwdriver`
- FlagTags: `Complaint.Flag.Mandatory`
- LinkedAnomalies: `ANM_STAIR_REFLECTION_DELAY`
- RequiredEvidenceTags: `Evidence.Visual`, `Evidence.Audio`
- MinEvidenceCountForReport: `2`
- AllowedReportResults: `Resolved`, `NeedsFollowUp`
- DefaultCanonicalResult: `Resolved`
- ActivationTags: `Progression.Chapter.One`, `Progression.Story.ThreeFloorUnstable`
- CompletionTags: `Progression.Chapter.Two`
- ConsequenceTags: `Progression.Tension.Stage1`

### CMP_CH2_302_TVHum

- Title: `302호 TV 잔류 소음`
- BoardSummary: `응답 없는 세대의 TV/생활 소음 확인`
- InternalNote: `소리의 방향과 전력 기록이 충돌하는 판정형 민원. 눈보다 귀를 믿게 했다가 다시 무너뜨린다.`
- TemplateType: `AnomalyJudgement`
- LocationId: `LOC_3F_302_DOOR`
- DomainTags: `Complaint.Domain.TVNoise`, `Complaint.Domain.LifeNoise`
- RequiredToolTags: `Complaint.RequiredTool.Flashlight`, `Complaint.RequiredTool.Notebook`
- FlagTags: `Complaint.Flag.Mandatory`
- LinkedAnomalies: `ANM_302_TV_ON_IN_DARK`, `ANM_LIFE_NOISE_IN_VACANT_UNIT`
- RequiredEvidenceTags: `Evidence.Audio`, `Evidence.Environmental`
- MinEvidenceCountForReport: `2`
- AllowedReportResults: `NoAnomaly`, `NeedsFollowUp`
- DefaultCanonicalResult: `NeedsFollowUp`
- ActivationTags: `Progression.Chapter.Two`
- CompletionTags: `Progression.Tension.Stage2`
- ConsequenceTags: `Progression.Story.Room307Clue`

### CMP_CH2_204_NameplateMismatch

- Title: `204호 이름표 불일치`
- BoardSummary: `문패, 우편함, 명부 정보 비교`
- InternalNote: `문패와 명부가 서로를 부정하는 기록 공포의 본격화. 이후 모든 세대 표기가 의심스러워져야 한다.`
- TemplateType: `Inspection`
- LocationId: `LOC_2F_204_DOOR`
- DomainTags: `Complaint.Domain.Record`
- RequiredToolTags: `Complaint.RequiredTool.Notebook`
- FlagTags: `Complaint.Flag.Mandatory`
- LinkedAnomalies: `ANM_204_NAMEPLATE_MISMATCH`, `ANM_RECORD_CONFLICT`
- RequiredEvidenceTags: `Evidence.Document`, `Evidence.Records`, `Evidence.Visual`
- MinEvidenceCountForReport: `2`
- AllowedReportResults: `NoAnomaly`, `NeedsFollowUp`
- DefaultCanonicalResult: `NeedsFollowUp`
- ActivationTags: `Progression.Chapter.Two`, `Progression.Tension.Stage2`
- CompletionTags: `Progression.Story.RecordMismatchNoted`
- ConsequenceTags: `Progression.Tension.Stage2`

### CMP_CH2_4F_CCTVBlank

- Title: `4층 CCTV 블랭크 구간`
- BoardSummary: `4층 CCTV 영상 및 시간 로그 점검`
- InternalNote: `안전 장치의 신뢰가 깨지는 민원. 현장 카메라와 관리실 시간 로그가 같이 틀어져야 한다.`
- TemplateType: `Inspection`
- LocationId: `LOC_4F_CCTV`
- DomainTags: `Complaint.Domain.Security`, `Complaint.Domain.Record`
- RequiredToolTags: `Complaint.RequiredTool.Flashlight`, `Complaint.RequiredTool.Notebook`
- FlagTags: `Complaint.Flag.Mandatory`
- LinkedAnomalies: `ANM_4F_CCTV_TIME_GAP`
- RequiredEvidenceTags: `Evidence.Visual`, `Evidence.Document`, `Evidence.Records`
- MinEvidenceCountForReport: `2`
- AllowedReportResults: `NoAnomaly`, `NeedsFollowUp`
- DefaultCanonicalResult: `NeedsFollowUp`
- ActivationTags: `Progression.Chapter.Two`, `Progression.Story.RecordMismatchNoted`
- CompletionTags: `Progression.Tension.Stage2`
- ConsequenceTags: `Progression.Story.Room307Clue`

### CMP_CH2_ElevatorFloorError

- Title: `엘리베이터 층 표시 오류`
- BoardSummary: `층 표시창과 실제 도착 층 대조`
- InternalNote: `층 감각을 흔들어 3층 접근 자체를 불안하게 만드는 민원.`
- TemplateType: `AnomalyJudgement`
- LocationId: `LOC_ELEVATOR_MAIN`
- DomainTags: `Complaint.Domain.Elevator`, `Complaint.Domain.Space`
- RequiredToolTags: `Complaint.RequiredTool.Flashlight`, `Complaint.RequiredTool.Notebook`
- FlagTags: `Complaint.Flag.Mandatory`
- LinkedAnomalies: `ANM_ELEVATOR_FLOOR_MISMATCH`
- RequiredEvidenceTags: `Evidence.Visual`, `Evidence.Audio`
- MinEvidenceCountForReport: `2`
- AllowedReportResults: `NoAnomaly`, `NeedsFollowUp`
- DefaultCanonicalResult: `NeedsFollowUp`
- ActivationTags: `Progression.Chapter.Two`, `Progression.Tension.Stage2`
- CompletionTags: `Progression.Story.ThreeFloorUnstable`
- ConsequenceTags: `Progression.Tension.Stage2`

### CMP_CH2_ReopenedLightCase

- Title: `재개된 3층 조명 민원`
- BoardSummary: `이미 해결한 3층 조명 민원 재접수 확인`
- InternalNote: `건물이 해결 완료를 인정하지 않는 구조를 처음 명시적으로 드러낸다.`
- TemplateType: `Inspection`
- LocationId: `LOC_OFFICE_BOARD_3F_LIGHT`
- DomainTags: `Complaint.Domain.Record`, `Complaint.Domain.Lighting`
- RequiredToolTags: `Complaint.RequiredTool.Notebook`
- FlagTags: `Complaint.Flag.Mandatory`, `Complaint.Flag.Repeatable`, `Complaint.Flag.StoryCritical`
- LinkedAnomalies: `ANM_COMPLAINT_REPEATS`, `ANM_RECORD_CONFLICT`
- RequiredEvidenceTags: `Evidence.Document`, `Evidence.Records`, `Evidence.Visual`
- MinEvidenceCountForReport: `3`
- AllowedReportResults: `NeedsFollowUp`
- DefaultCanonicalResult: `NeedsFollowUp`
- ActivationTags: `Progression.Chapter.Two`, `Progression.Story.ThreeFloorUnstable`
- CompletionTags: `Progression.Story.ComplaintLoopObserved`
- ConsequenceTags: `Progression.Tension.Stage2`

### CMP_CH3_BasementPumpAlarm

- Title: `지하 펌프 경보`
- BoardSummary: `지하 펌프 경보음과 수분 상태 점검`
- InternalNote: `지하실 진입을 정당화하는 민원. 저주파 경보와 부분 소등으로 감각적 압박을 크게 올린다.`
- TemplateType: `Repair`
- LocationId: `LOC_BSMT_PUMP`
- DomainTags: `Complaint.Domain.Power`, `Complaint.Domain.Water`
- RequiredToolTags: `Complaint.RequiredTool.Flashlight`, `Complaint.RequiredTool.BreakerTool`
- FlagTags: `Complaint.Flag.Mandatory`, `Complaint.Flag.RestrictedArea`
- LinkedAnomalies: `ANM_BSMT_PANEL_LABEL_CHANGED`
- RequiredEvidenceTags: `Evidence.Audio`, `Evidence.Environmental`
- MinEvidenceCountForReport: `2`
- AllowedReportResults: `Resolved`, `NeedsFollowUp`
- DefaultCanonicalResult: `NeedsFollowUp`
- ActivationTags: `Progression.Chapter.Three`, `Progression.Story.ComplaintLoopObserved`
- CompletionTags: `Progression.Area.BasementUnlocked`
- ConsequenceTags: `Progression.Tension.Stage3`

### CMP_CH3_306_OpenDoorAlarm

- Title: `306호 문 열림 경보`
- BoardSummary: `도어 센서와 실제 문 상태 불일치 확인`
- InternalNote: `306과 307이 기록에서 서로 미끄러지는 느낌을 만드는 미러 민원.`
- TemplateType: `Inspection`
- LocationId: `LOC_3F_306_DOOR`
- DomainTags: `Complaint.Domain.Door`, `Complaint.Domain.Security`
- RequiredToolTags: `Complaint.RequiredTool.Flashlight`, `Complaint.RequiredTool.Notebook`
- FlagTags: `Complaint.Flag.Mandatory`
- LinkedAnomalies: `ANM_306_DOOR_SENSOR_MISMATCH`
- RequiredEvidenceTags: `Evidence.Visual`, `Evidence.Records`
- MinEvidenceCountForReport: `2`
- AllowedReportResults: `NoAnomaly`, `NeedsFollowUp`
- DefaultCanonicalResult: `NeedsFollowUp`
- ActivationTags: `Progression.Chapter.Three`, `Progression.Area.BasementUnlocked`
- CompletionTags: `Progression.Story.Room307DoorReached`
- ConsequenceTags: `Progression.Tension.Stage3`

### CMP_CH3_ExitSignDepth

- Title: `복도 끝 비상구 거리감 이상`
- BoardSummary: `비상구 표지와 복도 기준점 재확인`
- InternalNote: `같은 복도인데 길이감과 기준점이 미세하게 달라지는 공간 공포의 대표 장면.`
- TemplateType: `AnomalyJudgement`
- LocationId: `LOC_2F_EXIT_HALL`
- DomainTags: `Complaint.Domain.Space`, `Complaint.Domain.Security`
- RequiredToolTags: `Complaint.RequiredTool.Flashlight`, `Complaint.RequiredTool.Notebook`
- FlagTags: `Complaint.Flag.Mandatory`
- LinkedAnomalies: `ANM_HALLWAY_STRETCH`, `ANM_REFLECTION_DELAY`
- RequiredEvidenceTags: `Evidence.Visual`, `Evidence.Environmental`
- MinEvidenceCountForReport: `3`
- AllowedReportResults: `NeedsFollowUp`
- DefaultCanonicalResult: `NeedsFollowUp`
- ActivationTags: `Progression.Chapter.Three`, `Progression.Story.Room307DoorReached`
- CompletionTags: `Progression.Story.SpaceDepthShift`
- ConsequenceTags: `Progression.Tension.Stage3`

### CMP_CH3_BasementPanelMislabel

- Title: `지하 배전반 라벨 불일치`
- BoardSummary: `3층 전원 불안정 관련 배전반 라벨과 실제 라인 확인`
- InternalNote: `건물 인프라 수준까지 307 관련 불일치가 번졌음을 드러내는 민원.`
- TemplateType: `Repair`
- LocationId: `LOC_BSMT_PANEL`
- DomainTags: `Complaint.Domain.Power`, `Complaint.Domain.Record`
- RequiredToolTags: `Complaint.RequiredTool.Flashlight`, `Complaint.RequiredTool.BreakerTool`, `Complaint.RequiredTool.Notebook`
- FlagTags: `Complaint.Flag.Mandatory`, `Complaint.Flag.RestrictedArea`, `Complaint.Flag.StoryCritical`
- LinkedAnomalies: `ANM_BSMT_PANEL_LABEL_CHANGED`, `ANM_RECORD_CONFLICT`
- RequiredEvidenceTags: `Evidence.Document`, `Evidence.Environmental`, `Evidence.Records`
- MinEvidenceCountForReport: `3`
- AllowedReportResults: `Resolved`, `NeedsFollowUp`
- DefaultCanonicalResult: `NeedsFollowUp`
- ActivationTags: `Progression.Chapter.Three`, `Progression.Story.SpaceDepthShift`
- CompletionTags: `Progression.Story.BasementPowerLinked`
- ConsequenceTags: `Progression.Tension.Stage3`

### CMP_CH4_307_PackageAtDoor

- Title: `307호 앞 수취인 불명 택배`
- BoardSummary: `307호 앞 택배와 세대 정보 확인`
- InternalNote: `307호 앞 첫 체류를 만드는 민원. 물리적 흔적은 분명하지만 세대 존재는 확정되지 않는다.`
- TemplateType: `Inspection`
- LocationId: `LOC_3F_307_DOOR`
- DomainTags: `Complaint.Domain.Record`, `Complaint.Domain.LifeNoise`
- RequiredToolTags: `Complaint.RequiredTool.Flashlight`, `Complaint.RequiredTool.Notebook`
- FlagTags: `Complaint.Flag.Mandatory`, `Complaint.Flag.StoryCritical`
- LinkedAnomalies: `ANM_307_PACKAGE_AT_MISSING_UNIT`
- RequiredEvidenceTags: `Evidence.Document`, `Evidence.Visual`, `Evidence.Records`
- MinEvidenceCountForReport: `3`
- AllowedReportResults: `NeedsFollowUp`
- DefaultCanonicalResult: `NeedsFollowUp`
- ActivationTags: `Progression.Chapter.Four`, `Progression.Story.BasementPowerLinked`
- CompletionTags: `Progression.Story.Room307PackageSeen`
- ConsequenceTags: `Progression.Tension.Stage4`

### CMP_CH4_307_LifeNoise

- Title: `307호 생활 소음`
- BoardSummary: `비어 있는 세대의 생활 소음 여부 확인`
- InternalNote: `307호는 응답이 없지만, 내부 생활 소음은 현재 진행형처럼 들린다.`
- TemplateType: `AnomalyJudgement`
- LocationId: `LOC_3F_307_DOOR`
- DomainTags: `Complaint.Domain.LifeNoise`, `Complaint.Domain.Intercom`
- RequiredToolTags: `Complaint.RequiredTool.Flashlight`, `Complaint.RequiredTool.Notebook`
- FlagTags: `Complaint.Flag.Mandatory`, `Complaint.Flag.StoryCritical`
- LinkedAnomalies: `ANM_307_LIFE_NOISE_IN_VACANT_UNIT`
- RequiredEvidenceTags: `Evidence.Audio`, `Evidence.Visual`, `Evidence.Records`
- MinEvidenceCountForReport: `3`
- AllowedReportResults: `NeedsFollowUp`
- DefaultCanonicalResult: `NeedsFollowUp`
- ActivationTags: `Progression.Chapter.Four`, `Progression.Story.Room307PackageSeen`
- CompletionTags: `Progression.Story.Room307LifeNoise`
- ConsequenceTags: `Progression.Tension.Stage4`

### CMP_CH4_RecordResident307

- Title: `307호 입주 기록 정리`
- BoardSummary: `명부, 정비 로그, 우편함 정보 상충 여부 정리`
- InternalNote: `307호가 장소이자 기록 병변이라는 사실을 명확히 만든다. 플레이어 자신의 필체와 닮은 메모도 여기서 등장한다.`
- TemplateType: `Inspection`
- LocationId: `LOC_OFFICE_307_RECORDS`
- DomainTags: `Complaint.Domain.Record`
- RequiredToolTags: `Complaint.RequiredTool.Notebook`
- FlagTags: `Complaint.Flag.Mandatory`, `Complaint.Flag.StoryCritical`
- LinkedAnomalies: `ANM_307_RECORD_CONFLICT`
- RequiredEvidenceTags: `Evidence.Document`, `Evidence.Records`, `Evidence.Visual`
- MinEvidenceCountForReport: `3`
- AllowedReportResults: `NeedsFollowUp`
- DefaultCanonicalResult: `NeedsFollowUp`
- ActivationTags: `Progression.Chapter.Four`, `Progression.Story.Room307LifeNoise`
- CompletionTags: `Progression.Story.Room307RecordConflict`
- ConsequenceTags: `Progression.Tension.Stage4`

### CMP_CH4_FinalCheck307

- Title: `307호 최종 확인`
- BoardSummary: `307호 접근 제한 해제 또는 봉쇄 판단을 위한 최종 확인`
- InternalNote: `최종 민원. 전투나 괴물 쇼가 아니라 기록, 구조, 생활 흔적, 자기 의심이 한 공간 안에서 결론나야 한다.`
- TemplateType: `Inspection`
- LocationId: `LOC_3F_307_INTERIOR`
- DomainTags: `Complaint.Domain.Record`, `Complaint.Domain.Space`, `Complaint.Domain.LifeNoise`
- RequiredToolTags: `Complaint.RequiredTool.Flashlight`, `Complaint.RequiredTool.MasterKey`, `Complaint.RequiredTool.Notebook`
- FlagTags: `Complaint.Flag.Mandatory`, `Complaint.Flag.StoryCritical`, `Complaint.Flag.RestrictedArea`
- LinkedAnomalies: `ANM_307_RECORD_CONFLICT`, `ANM_307_REFLECTION_DELAY`, `ANM_HALLWAY_STRETCH`
- RequiredEvidenceTags: `Evidence.Document`, `Evidence.Records`, `Evidence.Visual`, `Evidence.Audio`, `Evidence.Environmental`
- MinEvidenceCountForReport: `3`
- AllowedReportResults: `Resolved`, `NeedsFollowUp`
- DefaultCanonicalResult: `NeedsFollowUp`
- ActivationTags: `Progression.Chapter.Four`, `Progression.Story.Room307RecordConflict`, `Progression.Story.Room307DoorReached`
- CompletionTags: `Progression.Tension.Stage4`
- ConsequenceTags: `Progression.Story.Room307DoorReached`

## 4. Anomaly Definition Pack

### ANM_OFFICE_3F07_NOTE

- DisplayName: `의미 불명 3F 07 메모`
- Description: `관리실 전등 교체 흔적 아래에 남은 의미 불명 메모. 아직은 단서라기보다 숫자 잔상에 가깝다.`
- AnomalyTypeTag: `Anomaly.Type.RecordConflict`
- EvidenceTagsGranted: `Evidence.Document`
- ActivationTags: `Progression.Chapter.Prologue`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.Room307Clue`

### ANM_203_WATER_WITHOUT_SOURCE

- DisplayName: `원인 없는 물기`
- Description: `문 앞 타일과 문풍지 주변에만 차갑고 축축한 수분이 생긴다.`
- AnomalyTypeTag: `Anomaly.Type.WaterWithoutSource`
- EvidenceTagsGranted: `Evidence.Visual`, `Evidence.Environmental`
- ActivationTags: `Progression.Chapter.Prologue`
- PresentationTags: `None`
- ConsequenceTags: `None`

### ANM_INTERCOM_VOICE

- DisplayName: `인터폰 잡음 속 음성`
- Description: `호출음 대신 생활 소리 파편이나 애매한 음성 리듬이 섞인다.`
- AnomalyTypeTag: `Anomaly.Type.IntercomVoice`
- EvidenceTagsGranted: `Evidence.Audio`, `Evidence.Visual`
- ActivationTags: `Progression.Chapter.One`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.IntercomStaticObserved`

### ANM_MAILBOX_RECORD_CONFLICT

- DisplayName: `우편함/명부 불일치`
- Description: `우편함 이름표 배열, 명부, 세대 정보가 서로 정확히 맞지 않는다.`
- AnomalyTypeTag: `Anomaly.Type.MailboxMismatch`
- EvidenceTagsGranted: `Evidence.Document`, `Evidence.Records`
- ActivationTags: `Progression.Chapter.One`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.RecordMismatchNoted`

### ANM_3F_PERSISTENT_EMERGENCY_LIGHT

- DisplayName: `상시 점등 비상등`
- Description: `정상 전원이 살아 있는데도 비상등 하나가 계속 켜진다.`
- AnomalyTypeTag: `Anomaly.Type.PersistentEmergencyLight`
- EvidenceTagsGranted: `Evidence.Visual`, `Evidence.Environmental`
- ActivationTags: `Progression.Chapter.One`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.ThreeFloorUnstable`

### ANM_205_WATER_WITHOUT_SOURCE

- DisplayName: `냄새만 남은 습기`
- Description: `설비 누출은 없지만 문 앞 공기와 표면 상태만 눅눅하게 변한다.`
- AnomalyTypeTag: `Anomaly.Type.WaterWithoutSource`
- EvidenceTagsGranted: `Evidence.Environmental`, `Evidence.Visual`
- ActivationTags: `Progression.Chapter.One`
- PresentationTags: `None`
- ConsequenceTags: `None`

### ANM_STAIR_REFLECTION_DELAY

- DisplayName: `계단실 반응 지연`
- Description: `센서등이나 반사 피드백이 플레이어보다 늦게 따라온다.`
- AnomalyTypeTag: `Anomaly.Type.ReflectionDelay`
- EvidenceTagsGranted: `Evidence.Visual`, `Evidence.Audio`
- ActivationTags: `Progression.Chapter.One`
- PresentationTags: `None`
- ConsequenceTags: `None`

### ANM_302_TV_ON_IN_DARK

- DisplayName: `어두운 세대의 TV 잔광`
- Description: `문 아래 빛은 없지만 TV나 대화음 같은 생활 리듬이 남아 있다.`
- AnomalyTypeTag: `Anomaly.Type.TVOnInDark`
- EvidenceTagsGranted: `Evidence.Audio`, `Evidence.Environmental`
- ActivationTags: `Progression.Chapter.Two`
- PresentationTags: `None`
- ConsequenceTags: `None`

### ANM_LIFE_NOISE_IN_VACANT_UNIT

- DisplayName: `빈 세대의 생활 소음`
- Description: `비어 있거나 불명확한 세대에서 현재 거주 중인 것 같은 생활 소리가 난다.`
- AnomalyTypeTag: `Anomaly.Type.LifeNoiseInVacantUnit`
- EvidenceTagsGranted: `Evidence.Audio`, `Evidence.Visual`
- ActivationTags: `Progression.Chapter.Two`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.Room307Clue`

### ANM_204_NAMEPLATE_MISMATCH

- DisplayName: `문패 불일치`
- Description: `문패, 우편함, 명부 정보가 동시에 맞지 않는다.`
- AnomalyTypeTag: `Anomaly.Type.NameplateMismatch`
- EvidenceTagsGranted: `Evidence.Document`, `Evidence.Visual`
- ActivationTags: `Progression.Chapter.Two`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.RecordMismatchNoted`

### ANM_RECORD_CONFLICT

- DisplayName: `기록 충돌`
- Description: `같은 세대나 사건을 가리키는 기록이 서로 다른 사실을 말한다.`
- AnomalyTypeTag: `Anomaly.Type.RecordConflict`
- EvidenceTagsGranted: `Evidence.Document`, `Evidence.Records`
- ActivationTags: `Progression.Chapter.Two`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.RecordMismatchNoted`

### ANM_4F_CCTV_TIME_GAP

- DisplayName: `CCTV 시간 공백`
- Description: `현장 카메라는 살아 있으나 녹화 시간 로그나 블랭크 구간이 비정상적이다.`
- AnomalyTypeTag: `Anomaly.Type.CCTVTimeGap`
- EvidenceTagsGranted: `Evidence.Visual`, `Evidence.Records`
- ActivationTags: `Progression.Chapter.Two`
- PresentationTags: `None`
- ConsequenceTags: `None`

### ANM_ELEVATOR_FLOOR_MISMATCH

- DisplayName: `엘리베이터 층수 불일치`
- Description: `표시층, 도착음, 실제 위치가 잠깐씩 어긋난다.`
- AnomalyTypeTag: `Anomaly.Type.ElevatorFloorMismatch`
- EvidenceTagsGranted: `Evidence.Visual`, `Evidence.Audio`
- ActivationTags: `Progression.Chapter.Two`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.ThreeFloorUnstable`

### ANM_COMPLAINT_REPEATS

- DisplayName: `민원 재개`
- Description: `이미 해결된 민원이 기록상 다시 열린다.`
- AnomalyTypeTag: `Anomaly.Type.ComplaintRepeats`
- EvidenceTagsGranted: `Evidence.Document`, `Evidence.Records`
- ActivationTags: `Progression.Chapter.Two`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.ComplaintLoopObserved`

### ANM_BSMT_PANEL_LABEL_CHANGED

- DisplayName: `지하 설비 라벨 변화`
- Description: `배전반과 경보 패널의 라벨 체계가 실제 공급 라인과 맞지 않는다.`
- AnomalyTypeTag: `Anomaly.Type.PanelLabelChanged`
- EvidenceTagsGranted: `Evidence.Document`, `Evidence.Environmental`
- ActivationTags: `Progression.Chapter.Three`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.BasementPowerLinked`

### ANM_306_DOOR_SENSOR_MISMATCH

- DisplayName: `문 센서 상태 불일치`
- Description: `문은 잠겨 있는데 로그는 문이 열렸다고 기록한다.`
- AnomalyTypeTag: `Anomaly.Type.DoorSensorMismatch`
- EvidenceTagsGranted: `Evidence.Visual`, `Evidence.Records`
- ActivationTags: `Progression.Chapter.Three`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.Room307DoorReached`

### ANM_HALLWAY_STRETCH

- DisplayName: `늘어난 복도`
- Description: `같은 복도인데 끝까지의 거리감과 기준점이 다르게 느껴진다.`
- AnomalyTypeTag: `Anomaly.Type.HallwayStretch`
- EvidenceTagsGranted: `Evidence.Visual`, `Evidence.Environmental`
- ActivationTags: `Progression.Chapter.Three`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.SpaceDepthShift`

### ANM_REFLECTION_DELAY

- DisplayName: `반사 지연`
- Description: `거울, 유리, 도어락 표면 반사가 현재 위치와 미세하게 맞지 않는다.`
- AnomalyTypeTag: `Anomaly.Type.ReflectionDelay`
- EvidenceTagsGranted: `Evidence.Visual`
- ActivationTags: `Progression.Chapter.Three`
- PresentationTags: `None`
- ConsequenceTags: `None`

### ANM_307_PACKAGE_AT_MISSING_UNIT

- DisplayName: `없는 세대 앞 택배`
- Description: `존재가 확정되지 않는 세대 앞에 정상적인 배송 흔적이 남는다.`
- AnomalyTypeTag: `Anomaly.Type.PackageAtMissingUnit`
- EvidenceTagsGranted: `Evidence.Document`, `Evidence.Visual`, `Evidence.Records`
- ActivationTags: `Progression.Chapter.Four`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.Room307PackageSeen`

### ANM_307_LIFE_NOISE_IN_VACANT_UNIT

- DisplayName: `307호 생활 소음`
- Description: `307호는 응답이 없지만, 내부 생활 소음은 현재 진행형처럼 들린다.`
- AnomalyTypeTag: `Anomaly.Type.LifeNoiseInVacantUnit`
- EvidenceTagsGranted: `Evidence.Audio`, `Evidence.Visual`, `Evidence.Records`
- ActivationTags: `Progression.Chapter.Four`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.Room307LifeNoise`

### ANM_307_RECORD_CONFLICT

- DisplayName: `307호 기록 충돌`
- Description: `입주자 명부, 정비 로그, 우편함, 민원 보드가 모두 307호를 다르게 가리킨다.`
- AnomalyTypeTag: `Anomaly.Type.RecordConflict`
- EvidenceTagsGranted: `Evidence.Document`, `Evidence.Records`, `Evidence.Visual`
- ActivationTags: `Progression.Chapter.Four`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.Room307RecordConflict`

### ANM_307_REFLECTION_DELAY

- DisplayName: `307호 반사 오류`
- Description: `307호 내부의 반사면과 거리감이 정상 세대의 예측과 어긋난다.`
- AnomalyTypeTag: `Anomaly.Type.ReflectionDelay`
- EvidenceTagsGranted: `Evidence.Visual`, `Evidence.Environmental`
- ActivationTags: `Progression.Chapter.Four`
- PresentationTags: `None`
- ConsequenceTags: `None`

## 5. Chapter Row Mapping

| RowId | ChapterId | Complaint | SortOrder | RequiredProgressionTags | BlockedByTags | StartAvailable |
| --- | --- | --- | ---: | --- | --- | --- |
| ROW_PRO_01 | CH_PRO | CMP_PRO_OfficeLightBuzz | 10 | `Progression.Chapter.Prologue` | None | true |
| ROW_PRO_02 | CH_PRO | CMP_PRO_203_WaterAtDoor | 20 | `Progression.Chapter.Prologue`, `Progression.Story.Room307Clue` | None | false |
| ROW_CH1_01 | CH_01 | CMP_CH1_2F_IntercomStatic | 10 | `Progression.Chapter.One` | None | true |
| ROW_CH1_02 | CH_01 | CMP_CH1_1F_MailboxMisdelivery | 20 | `Progression.Chapter.One`, `Progression.Story.IntercomStaticObserved` | None | false |
| ROW_CH1_03 | CH_01 | CMP_CH1_3F_EmergencyLight | 30 | `Progression.Chapter.One`, `Progression.Story.RecordMismatchNoted` | None | false |
| ROW_CH1_04 | CH_01 | CMP_CH1_205_OdorAtDoor | 40 | `Progression.Chapter.One`, `Progression.Story.ThreeFloorUnstable` | None | false |
| ROW_CH1_05 | CH_01 | CMP_CH1_StairAutoLightDelay | 50 | `Progression.Chapter.One`, `Progression.Story.ThreeFloorUnstable` | None | false |
| ROW_CH2_01 | CH_02 | CMP_CH2_302_TVHum | 10 | `Progression.Chapter.Two` | None | true |
| ROW_CH2_02 | CH_02 | CMP_CH2_204_NameplateMismatch | 20 | `Progression.Chapter.Two`, `Progression.Tension.Stage2` | None | false |
| ROW_CH2_03 | CH_02 | CMP_CH2_4F_CCTVBlank | 30 | `Progression.Chapter.Two`, `Progression.Story.RecordMismatchNoted` | None | false |
| ROW_CH2_04 | CH_02 | CMP_CH2_ElevatorFloorError | 40 | `Progression.Chapter.Two`, `Progression.Tension.Stage2` | None | false |
| ROW_CH2_05 | CH_02 | CMP_CH2_ReopenedLightCase | 50 | `Progression.Chapter.Two`, `Progression.Story.ThreeFloorUnstable` | None | false |
| ROW_CH3_01 | CH_03 | CMP_CH3_BasementPumpAlarm | 10 | `Progression.Chapter.Three`, `Progression.Story.ComplaintLoopObserved` | None | true |
| ROW_CH3_02 | CH_03 | CMP_CH3_306_OpenDoorAlarm | 20 | `Progression.Chapter.Three`, `Progression.Area.BasementUnlocked` | None | false |
| ROW_CH3_03 | CH_03 | CMP_CH3_ExitSignDepth | 30 | `Progression.Chapter.Three`, `Progression.Story.Room307DoorReached` | None | false |
| ROW_CH3_04 | CH_03 | CMP_CH3_BasementPanelMislabel | 40 | `Progression.Chapter.Three`, `Progression.Story.SpaceDepthShift` | None | false |
| ROW_CH4_01 | CH_04 | CMP_CH4_307_PackageAtDoor | 10 | `Progression.Chapter.Four`, `Progression.Story.BasementPowerLinked` | None | true |
| ROW_CH4_02 | CH_04 | CMP_CH4_307_LifeNoise | 20 | `Progression.Chapter.Four`, `Progression.Story.Room307PackageSeen` | None | false |
| ROW_CH4_03 | CH_04 | CMP_CH4_RecordResident307 | 30 | `Progression.Chapter.Four`, `Progression.Story.Room307LifeNoise` | None | false |
| ROW_CH4_04 | CH_04 | CMP_CH4_FinalCheck307 | 40 | `Progression.Chapter.Four`, `Progression.Story.Room307RecordConflict`, `Progression.Story.Room307DoorReached` | `Progression.Tension.Stage0` | false |

## 6. 구현 시 확인할 것

- 모든 `LocationId`는 실제 레벨 액터 또는 구역 id와 맞춰서 추후 통일한다.
- `CH_PRO`, `CH_01`, `CH_02`, `CH_03`, `CH_04`는 `NCSetChapter` 디버그 워크플로와 맞는 실제 챕터 id 자산/테이블 명명 규칙으로 통일한다.
- `Title`, `BoardSummary`, `InternalNote`는 이후 로컬라이징 대상이다.
- `CompletionTags`와 `RequiredProgressionTags`는 선형 해금이므로, 런타임 브리지 연결 전 디버그 치트로 deadlock 여부를 반드시 검증한다.
