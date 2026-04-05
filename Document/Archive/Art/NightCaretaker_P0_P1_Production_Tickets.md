# NightCaretaker P0/P1 Production Tickets

## 문서 정보

- 문서 종류: 아트/레벨 제작 티켓 문서
- 버전: v0.1
- 작성일: 2026-03-29
- 대상: Environment Artist, Prop Artist, Level Artist, Lighting Artist, Outsource Artist
- 목적: `Complaint 20 Pack`과 `307 BeatSheet`를 기준으로 P0/P1 범위의 실무용 제작 티켓을 고정한다.
- 관련 문서: `NightCaretaker_Art_Asset_Priority_List.md`, `NightCaretaker_Complaint_20_Pack.md`, `NightCaretaker_307_Staging_BeatSheet.md`

## 1. 티켓 작성 규칙

- 각 티켓은 최소 2개 complaint를 직접 지원해야 한다.
- 각 티켓은 최소 1개 이상의 307 beat를 지원해야 한다.
- P0는 수직 슬라이스와 챕터 1~2 일부를 커버해야 한다.
- P1은 챕터 2~3의 기록 공포, 지하실, 엘리베이터, CCTV, 세대 내부 확장까지 커버해야 한다.
- P2 이상 자산은 참고만 하고 이 문서에서는 티켓화하지 않는다.

## 2. P0 Tickets

### ART-P0-01 Corridor Modular Kit

- Priority: `P0`
- Discipline: `Environment`
- Goal: 반복 방문에 버티는 기본 복도 모듈과 기준점 구조를 제작한다.
- Covers Complaints: `CMP_PRO_203_WaterAtDoor`, `CMP_CH1_3F_EmergencyLight`, `CMP_CH3_ExitSignDepth`, `CMP_CH4_307_PackageAtDoor`
- Supports 307 Beats: `단계 2`, `단계 3`, `단계 7`, `단계 8`
- Required Assets:
  - 직선 복도 벽/바닥/천장 모듈
  - 코너 모듈
  - 복도 끝 유리문/철문
  - 복도 기준점용 벽면 패널 슬롯
- Minimum Variations:
  - 벽 상태 3종
  - 천장재 상태 3종
  - 바닥 오염 3종
- Dependencies: `None`
- Acceptance Criteria:
  - 2층과 3층 복도 프로토타입을 같은 키트로 구성 가능해야 한다.
  - 문/소화기/비상등/게시물 위치를 기준점으로 반복 배치할 수 있어야 한다.
  - `CMP_CH3_ExitSignDepth`용 끝이 읽히지 않는 깊이 구성이 가능해야 한다.

### ART-P0-02 Apartment Door and Nameplate Set

- Priority: `P0`
- Discipline: `Environment`, `Prop`
- Goal: 세대 문, 도어락, 문패, 문 아래 틈 디테일을 공통 세트로 만든다.
- Covers Complaints: `CMP_CH1_205_OdorAtDoor`, `CMP_CH2_204_NameplateMismatch`, `CMP_CH3_306_OpenDoorAlarm`, `CMP_CH4_307_PackageAtDoor`
- Supports 307 Beats: `단계 2`, `단계 8`, `단계 11`
- Required Assets:
  - 기본 현관문 2종
  - 도어락 2종
  - 문패 슬롯과 숫자 카드 세트
  - 문 아래 틈/문풍지 디테일
- Minimum Variations:
  - 도색 3종
  - 스크래치/오염 3종
  - 문패 카드 4종
- Dependencies: `ART-P0-01`
- Acceptance Criteria:
  - 203/204/205/306/307 문 앞 씬을 모두 같은 세트로 구성 가능해야 한다.
  - 문패만 교체해 기록 공포 변주를 줄 수 있어야 한다.
  - 문 아래 빛, 습기, 소리 암시 연출이 가능해야 한다.

### ART-P0-03 Corridor Lighting and Electrical Fixture Set

- Priority: `P0`
- Discipline: `Lighting`, `Prop`
- Goal: 형광등, 비상등, 스위치, 소형 전기함을 표준 세트로 만든다.
- Covers Complaints: `CMP_PRO_OfficeLightBuzz`, `CMP_CH1_2F_IntercomStatic`, `CMP_CH1_3F_EmergencyLight`, `CMP_CH1_StairAutoLightDelay`
- Supports 307 Beats: `단계 1`, `단계 3`, `단계 5`
- Required Assets:
  - 형광등 fixture 2종
  - 비상등 2종
  - 스위치 박스
  - 소형 전기함
  - 노출 배선/배선관 파츠
- Minimum Variations:
  - 점등 상태 4종
  - 커버 손상 2종
  - 라벨/스티커 4종
- Dependencies: `ART-P0-01`
- Acceptance Criteria:
  - 관리실, 복도, 계단실에서 동일 세트를 재사용할 수 있어야 한다.
  - `정상`, `깜빡임`, `상시 비상등`, `부분 소등` 상태를 교체 비용 없이 만들 수 있어야 한다.
  - 전기 라벨과 07 흔적을 얹을 수 있는 표면이 있어야 한다.

### ART-P0-04 Stairwell Core Kit

- Priority: `P0`
- Discipline: `Environment`, `Lighting`
- Goal: 층 이동 긴장 구간인 계단실 기본 키트를 제작한다.
- Covers Complaints: `CMP_CH1_StairAutoLightDelay`, `CMP_CH2_ElevatorFloorError`, `CMP_CH3_ExitSignDepth`
- Supports 307 Beats: `단계 3`, `단계 7`
- Required Assets:
  - 콘크리트 벽/바닥
  - 난간 2종
  - 층수 표기판
  - 센서등
  - 비상구 철문
- Minimum Variations:
  - 층수 표기 4층분
  - 오염도 3단계
- Dependencies: `ART-P0-03`
- Acceptance Criteria:
  - 1층-3층을 같은 키트로 연결 가능해야 한다.
  - 늦게 켜지는 센서등 연출이 가능한 구조여야 한다.
  - 계단실만으로도 3층 불안이 체감되어야 한다.

### ART-P0-05 Management Office Gameplay Kit

- Priority: `P0`
- Discipline: `Environment`, `Prop`
- Goal: 허브 공간이자 기록 공포의 시작점인 관리실을 제작한다.
- Covers Complaints: `CMP_PRO_OfficeLightBuzz`, `CMP_CH2_ReopenedLightCase`, `CMP_CH4_RecordResident307`
- Supports 307 Beats: `단계 5`, `단계 10`, `단계 13`
- Required Assets:
  - 책상/의자
  - 민원 보드
  - 서류 캐비닛
  - 입주자 명부 바인더
  - 전화/인터폰 수신기
  - 마스터 키 보드
  - 생활 흔적 소품
- Minimum Variations:
  - 책상 위 배치 3종
  - 민원 보드 상태 3종
  - 명부/로그 문서 세트
- Dependencies: `None`
- Acceptance Criteria:
  - 민원 보드, 기록 비교, 마지막 보고 연출이 모두 같은 공간에서 가능해야 한다.
  - 안전지대처럼 보이지만 완전히 편하지 않은 인상이 유지되어야 한다.
  - 해결된 민원이 다시 열리는 보드 상태를 표현할 수 있어야 한다.

### ART-P0-06 Mailbox Intercom Bulletin Set

- Priority: `P0`
- Discipline: `Prop`
- Goal: 우편함, 인터폰, 게시판, 메모류를 정보 프랍 세트로 만든다.
- Covers Complaints: `CMP_CH1_2F_IntercomStatic`, `CMP_CH1_1F_MailboxMisdelivery`, `CMP_CH2_204_NameplateMismatch`, `CMP_CH4_RecordResident307`
- Supports 307 Beats: `단계 2`, `단계 4`, `단계 10`
- Required Assets:
  - 우편함 블록 2종
  - 인터폰 패널 2종
  - 게시판
  - 경고문/광고지/메모 팩
- Minimum Variations:
  - 이름표 카드 10종 이상
  - 찢긴 메모 6종 이상
  - 공지문 8종 이상
- Dependencies: `ART-P0-05`
- Acceptance Criteria:
  - 우편함 라벨 불일치, 인터폰 긁힘, 게시판 공지 변주를 모두 처리 가능해야 한다.
  - 문서형 공포를 텍스트량 과다 없이 시각적으로 전달할 수 있어야 한다.

### ART-P0-07 Living Trace Prop Pack

- Priority: `P0`
- Discipline: `Prop`
- Goal: 문 앞과 공용부에 배치되는 기본 생활 흔적 프랍을 만든다.
- Covers Complaints: `CMP_PRO_203_WaterAtDoor`, `CMP_CH1_205_OdorAtDoor`, `CMP_CH4_307_PackageAtDoor`, `CMP_CH4_307_LifeNoise`
- Supports 307 Beats: `단계 4`, `단계 8`, `단계 9`
- Required Assets:
  - 신발/슬리퍼
  - 우산
  - 택배 상자
  - 제습제 통
  - 생수병
  - 매트/재활용 묶음
- Minimum Variations:
  - 신발 배치 8종
  - 상자 라벨 6종
  - 오염도 3단계
- Dependencies: `ART-P0-02`
- Acceptance Criteria:
  - 거주 흔적은 분명하지만 현재 부재감도 함께 느껴져야 한다.
  - 307호 앞 흔적 누적 연출의 베이스로 재사용 가능해야 한다.

### ART-P0-08 Leak Mold Repair Decal Pack

- Priority: `P0`
- Discipline: `Material`, `Environment`
- Goal: 누수, 곰팡이, 테이프 자국, 보수 흔적 decal 팩을 제작한다.
- Covers Complaints: `CMP_PRO_203_WaterAtDoor`, `CMP_CH1_205_OdorAtDoor`, `CMP_CH3_BasementPumpAlarm`, `CMP_CH3_BasementPanelMislabel`
- Supports 307 Beats: `단계 1`, `단계 6`, `단계 12`
- Required Assets:
  - 누수 자국
  - 곰팡이 번짐
  - 결로/물때
  - 테이프 자국
  - 부분 수리 경계선
- Minimum Variations:
  - 크기별 4세트
  - 강도별 3단계
- Dependencies: `ART-P0-01`
- Acceptance Criteria:
  - 복도, 문 앞, 지하실, 관리실 어디든 재사용 가능해야 한다.
  - 예쁜 낡음이 아니라 관리 피로와 생활 오염이 느껴져야 한다.

## 3. P1 Tickets

### ART-P1-01 Elevator Hall Set

- Priority: `P1`
- Discipline: `Environment`, `Lighting`
- Goal: 층 표시와 대기 불안을 만드는 엘리베이터 홀 세트를 제작한다.
- Covers Complaints: `CMP_CH2_ElevatorFloorError`, `CMP_CH3_ExitSignDepth`
- Supports 307 Beats: `단계 4`, `단계 7`
- Required Assets:
  - 엘리베이터 문 프레임
  - 버튼 패널
  - 층 표시창
  - 홀 거울/반사면
  - CCTV 포인트
- Minimum Variations:
  - 표시 상태 4종
  - 버튼 마모도 3종
- Dependencies: `ART-P0-01`, `ART-P0-03`
- Acceptance Criteria:
  - 표시층과 실제 감각이 미세하게 어긋나는 연출이 가능해야 한다.
  - 반사면이 공간 불신을 강화해야 한다.

### ART-P1-02 Unit Floorplan Base Set

- Priority: `P1`
- Discipline: `Level`
- Goal: 세대 내부 기본 평면 3~4종을 제작한다.
- Covers Complaints: `CMP_CH2_302_TVHum`, `CMP_CH4_307_LifeNoise`, `CMP_CH4_FinalCheck307`
- Supports 307 Beats: `단계 4`, `단계 9`, `단계 12`
- Required Assets:
  - 전실형 평면
  - 복도형 평면
  - 원룸형 평면
  - 방 2개형 평면
- Minimum Variations:
  - 조명 상태 4단계
  - 환기/오염 상태 3단계
- Dependencies: `ART-P0-02`, `ART-P0-07`
- Acceptance Criteria:
  - 일반 세대와 307호 변주의 공통 베이스로 사용할 수 있어야 한다.
  - 문을 열어도 한눈에 다 보이지 않는 구조여야 한다.

### ART-P1-03 Unit Furniture and Bathroom Pack

- Priority: `P1`
- Discipline: `Prop`, `Environment`
- Goal: 세대 내부 생활감과 현재 부재를 동시에 표현할 기본 가구 팩을 만든다.
- Covers Complaints: `CMP_CH2_302_TVHum`, `CMP_CH4_307_LifeNoise`, `CMP_CH4_FinalCheck307`
- Supports 307 Beats: `단계 4`, `단계 9`, `단계 12`
- Required Assets:
  - 주방/싱크 세트
  - TV장/소파/의자
  - 침대/수납장
  - 욕실 세면대/거울장
  - 커튼/블라인드
- Minimum Variations:
  - 배치 프리셋 6종
  - 사용감 3단계
- Dependencies: `ART-P1-02`
- Acceptance Criteria:
  - 실제로 거주 가능한 구조처럼 읽혀야 한다.
  - 307호 내부에서는 같은 가구가 미세하게 잘못된 인상으로 변주 가능해야 한다.

### ART-P1-04 Basement Pump and Electrical Room Set

- Priority: `P1`
- Discipline: `Environment`, `Prop`
- Goal: 지하 펌프실과 전기실의 현실 설비 공포를 만들 기본 세트를 제작한다.
- Covers Complaints: `CMP_CH3_BasementPumpAlarm`, `CMP_CH3_BasementPanelMislabel`
- Supports 307 Beats: `단계 6`
- Required Assets:
  - 대형 분전반
  - 펌프/배관/밸브
  - 경고 라벨
  - 임시 작업등
  - 지하 벽/바닥 세트
- Minimum Variations:
  - 배관 상태 3종
  - 수분/부식 상태 3종
- Dependencies: `ART-P0-03`, `ART-P0-08`
- Acceptance Criteria:
  - 초자연 제단처럼 보이지 않고, 기능적으로 너무 현실적인 불편함을 줘야 한다.
  - 라벨/차단기 불일치 연출이 가능해야 한다.

### ART-P1-05 CCTV Monitor and Recording Pack

- Priority: `P1`
- Discipline: `Prop`, `UI`, `Lighting`
- Goal: CCTV, 관리실 모니터, 녹화 시간 오차 표현 세트를 제작한다.
- Covers Complaints: `CMP_CH2_4F_CCTVBlank`, `CMP_CH4_RecordResident307`
- Supports 307 Beats: `단계 4`, `단계 10`
- Required Assets:
  - CCTV 카메라 2종
  - 모니터 프레임
  - 타임코드 오버레이
  - 블랭크/잡음/정상 화면 텍스처
- Minimum Variations:
  - 화면 상태 5종
  - 시간 오류 3종
- Dependencies: `ART-P0-05`
- Acceptance Criteria:
  - 안전 장치가 안전하지 않다는 감각을 만들 수 있어야 한다.
  - 시간 로그 오류가 문서형 단서와 연결될 수 있어야 한다.

### ART-P1-06 Safety Signage and Wayfinding Variants

- Priority: `P1`
- Discipline: `Prop`, `Environment`
- Goal: 비상구, 층수 표기, 경고 표식, 소화기 기준점을 변주 가능한 세트로 만든다.
- Covers Complaints: `CMP_CH1_3F_EmergencyLight`, `CMP_CH2_ElevatorFloorError`, `CMP_CH3_ExitSignDepth`
- Supports 307 Beats: `단계 3`, `단계 7`, `단계 11`
- Required Assets:
  - 비상구 표지
  - 층수 표기판
  - 소화기/소화전 표식
  - 경고 스티커
  - 출입 금지 테이프
- Minimum Variations:
  - 신규/낡음/찢김 3단계
  - 위치 대체 버전 5종
- Dependencies: `ART-P0-01`, `ART-P0-04`
- Acceptance Criteria:
  - 플레이어가 기억하는 기준점을 아트만으로 흔들 수 있어야 한다.
  - `CMP_CH3_ExitSignDepth`에 필요한 소화기/비상구 위치 차이를 구현할 수 있어야 한다.

## 4. 제작 순서

1. `ART-P0-01 Corridor Modular Kit`
2. `ART-P0-02 Apartment Door and Nameplate Set`
3. `ART-P0-03 Corridor Lighting and Electrical Fixture Set`
4. `ART-P0-05 Management Office Gameplay Kit`
5. `ART-P0-06 Mailbox Intercom Bulletin Set`
6. `ART-P0-07 Living Trace Prop Pack`
7. `ART-P0-08 Leak Mold Repair Decal Pack`
8. `ART-P0-04 Stairwell Core Kit`
9. `ART-P1-01 Elevator Hall Set`
10. `ART-P1-02 Unit Floorplan Base Set`
11. `ART-P1-03 Unit Furniture and Bathroom Pack`
12. `ART-P1-04 Basement Pump and Electrical Room Set`
13. `ART-P1-05 CCTV Monitor and Recording Pack`
14. `ART-P1-06 Safety Signage and Wayfinding Variants`

## 5. 완료 기준

- P0만으로 프롤로그, 챕터 1 주요 민원, 챕터 2 초반, 307호 앞 첫 체류까지 재현 가능해야 한다.
- P1까지 완료하면 챕터 2 기록 공포, 챕터 3 지하실/공간 불신, 307호 직전 준비 상태를 커버해야 한다.
- 어떤 티켓도 단일 이벤트용 고립 자산으로 끝나면 안 된다.
- 모든 티켓은 최소 2개 이상 민원과 연결된 사용 사례를 가져야 한다.
