# 야간 관리인: 307호의 민원 - 개발 총합본

## 문서 정보

- 문서 종류: 개발 총합본
- 작성일: 2026-04-05
- 목적: 개발 문서군의 상세 내용을 한 문서에 통합하되, 반복되는 소개와 경로 안내를 줄이고 실제 구현 기준을 중심으로 정리한다.
- 통합 기준:
  - 범위, 제품 목표, 코어 시스템 기준은 `개발 기획서`를 기준 본문으로 삼음
  - 런타임 프레임워크, 카메라, 물리 상호작용, 데이터 작성/매핑은 중복되지 않는 상세 기술 문서로 유지
  - 문서 정보와 오래된 참조 경로는 제거하고 현재 구조 기준으로 정리
- 원본 보관 위치: `Document/Archive/Development`

## 문서 내 목차

- 중복 정리 기준
- 빠른 구현 기준
  - 지금 이 프로젝트의 개발 방향
  - 현재 가장 중요한 연결 작업
  - 구현 시 계속 확인할 질문
- 개발 기획 및 범위 기준
  - 1. 문서 목적
  - 2. 개발 핵심 원칙
  - 3. 제품 목표
  - 4. 범위 잠금
  - 5. 핵심 플레이 구조
  - 6. 핵심 시스템 정의
  - 7. 시스템-콘텐츠 분리 원칙
  - 8. Unreal 구현 방침
  - 9. 레벨 제작 전략
  - 10. 마일스톤 계획
  - 11. 우선순위 백로그
  - 12. 리스크와 대응
  - 13. 플레이 테스트 기준
  - 14. 즉시 실행 항목
  - 15. 최종 개발 판단
- 런타임 프레임워크 상세
  - 1. 문서 목적
  - 2. 현재 구조 한눈에 보기
  - 3. 지금 구현된 것과 아직 없는 것
  - 4. 업적 프레임워크 설명
  - 5. Steam Integration Kit 연결 절차
  - 6. GameState 기반 상태 모듈 설명
  - 7. `ComplaintRuntimeSubsystem` 사용 기준
  - 8. GameMode / GameState 역할 분리
  - 9. 디버그 관리자 사용법
  - 10. 추천 다음 구현 순서
  - 11. 구현 원칙 정리
- RealityCam 상세
  - 문서 목적
  - 1. RealityCam의 역할
  - 2. 왜 UCameraComponent를 상속했는가
  - 3. 헤더 구조 설명
  - 4. 플레이어에 어떻게 연결되는가
  - 5. 프레임별 동작 순서
  - 6. 각 레이어가 어떻게 계산되는가
  - 7. 최종 합성 방식
  - 8. Precision Interaction 모드
  - 9. 이 구조의 장점
  - 10. 현재 구조의 한계
  - 11. 의사코드
  - 12. 확장 우선순위
- 물리 상호작용 상세
  - 1. 현재 구현 범위 요약
  - 2. 플레이어 소유 구조
  - 3. Grab 입력 라우팅 순서
  - 4. 물리 프랍 홀드 시스템
  - 5. 물리 문 시스템
  - 6. RealityCam 연동
  - 7. 현재 알려진 제한사항
  - 8. 디자이너 체크리스트
  - 9. 빠른 검증 절차
  - 10. 후속 확장 추천
- 데이터 작성 가이드 상세
  - 1. 문서 목적
  - 2. 현재 구현 범위
  - 3. 전체 구조 한눈에 보기
  - 4. 추천 폴더 구조
  - 5. 먼저 만들어야 하는 순서
  - 6. ID, Enum, Tag를 언제 쓰는가
  - 7. 업적 정의 자산 설정 방법
  - 8. 이상 현상 자산 설정 방법
  - 9. 민원 자산 설정 방법
  - 10. 민원 업적 이벤트 설정 방법
  - 11. 이상 현상 업적 이벤트 설정 방법
  - 12. 챕터 DataTable 설정 방법
  - 13. 실제 구성 흐름 예시
  - 14. 자주 하는 실수
  - 15. 최종 체크리스트
  - 16. 이후 구현에서 이 데이터가 소비되는 흐름
  - 17. 한 줄 기준 정리
- 데이터 매핑 상세
  - 1. 고정 매핑 규칙
  - 2. 추천 자산 경로
  - 3. Complaint Definition Mapping
  - 4. Anomaly Definition Pack
  - 5. Chapter Row Mapping
  - 6. 구현 시 확인할 것


## 중복 정리 기준

- 제품 목표, 범위 잠금, 마일스톤, 리스크는 `개발 기획서`를 기준으로 유지했다.
- 런타임 프레임워크 문서는 구조와 접근 계층 설명에 집중하도록 독립 유지했다.
- `RealityCam`, `물리 상호작용`, `데이터 작성/매핑`은 실제 구현과 자산 작성에 필요한 고유 상세 내용이 많아 전문을 유지했다.
- 문서 정보, 예전 기준 문서 참조, 이전 경로 안내 같은 반복 정보는 삭제했다.

## 빠른 구현 기준

### 지금 이 프로젝트의 개발 방향

- 많은 기능보다 적은 기능의 고밀도 변주
- 시스템과 콘텐츠 분리
- 데이터 추가만으로 민원 확장 가능
- 조명, 사운드, 기록, 상태 변화가 실제 게임 상태를 설명

### 현재 가장 중요한 연결 작업

- 민원 보드 / 보고 UI와 `ComplaintRuntimeSubsystem` 연결
- 챕터 DataTable 자동 로드 및 순차 해금 로직
- 월드 상호작용 액터와 민원 상태 브리지 연결
- 저장/로드 연결
- Steam 업적 adapter 연결

### 구현 시 계속 확인할 질문

- 민원 1건이 조사, 행동, 보고, 상태 변화까지 닫히는가
- 실패 후 같은 일을 같은 상태로 반복시키지 않는가
- 새 콘텐츠를 추가할 때 새 시스템이 아니라 데이터 추가로 끝나는가
- 카메라, 조명, 사운드가 연출 장식이 아니라 시스템 출력으로 동작하는가

## 개발 기획 및 범위 기준

### 1. 문서 목적

이 문서는 1차 상세 기획서를 실제 제작 기준으로 재정리한 개발용 문서다.

핵심 목표는 다음 세 가지다.

- 제작 범위를 초기에 잠그고 끝까지 유지한다.
- 시스템과 콘텐츠를 분리해 반복 제작이 가능하도록 만든다.
- 데모와 본편이 같은 구조 위에서 확장되도록 설계한다.

즉, 이 프로젝트는 "많은 기능"이 아니라 "적은 기능을 높은 밀도로 반복 변주"하는 방식으로 완성한다.

### 2. 개발 핵심 원칙

- 건물은 1개동만 사용한다.
- 플레이어의 역할은 끝까지 "야간 관리인"으로 유지한다.
- 공포는 전투가 아니라 업무 루프의 붕괴로 만든다.
- 같은 공간을 여러 번 방문하되, 상태 변화로 새롭게 보이게 한다.
- 데이터는 Data Table 또는 Data Asset을 기준으로 관리한다.
- 시스템은 적게 만들고, 데이터와 연출 변주로 분량을 만든다.
- 빛과 사운드는 공포와 몰입을 결정하는 최우선 체험 축으로 둔다.
- Lumen을 적극 활용해 조명, 반사, 간접광 변화 자체가 게임 상태를 전달하게 만든다.
- Nanite를 적극 활용해 반복 공간에서도 실사적인 질감 밀도와 구조물 설득력을 확보한다.
- MetaSound를 적극 활용해 전기음, 생활 소음, 긴장 단계 변화를 반응형으로 구성한다.
- 초현실은 별도 판타지 공간보다 익숙한 생활 공간의 논리가 조금씩 어긋나는 방식으로 만든다.
- 괴물의 직접 등장과 노골적인 추격 연출은 최소화하고, 존재감은 빛, 소리, 흔적, 공간 왜곡으로 전달한다.
- 공포의 중심은 적의 공격보다 심연, 미지, 설명되지 않는 공간의 깊이감에 둔다.
- 코어 로직에서는 과도한 `IsValid` 분기보다 단순한 상태 흐름과 명확한 초기화 순서를 우선한다.
- 모든 민원은 정해진 템플릿 안에서만 제작한다.
- 후반부 확장은 새 기능 추가보다 기존 규칙의 뒤틀림으로 해결한다.
- 시점은 끝까지 RealityCam 기준 1인칭 카메라를 유지한다.

### 3. 제품 목표

#### 3.1 최종 제품 목표

- 장르: 1인칭 심리 공포 + 업무 시뮬레이션
- 플레이타임: 3~4시간
- 목표 가격: USD 9.99~14.99
- 핵심 판매 문장:
  오래된 아파트의 야간 관리인이 되어 주민 민원을 처리하라. 하지만 밤이 깊어질수록, 이 건물의 문제는 더 이상 수리로 해결되지 않는다.

#### 3.2 데모 목표

- 플레이타임: 30~45분
- 플레이어가 5분 안에 "민원 처리 공포"라는 콘셉트를 이해한다.
- 정상 민원과 이상 민원의 차이를 한 세션 안에서 체감한다.
- 괴물을 거의 직접 보여주지 않고도 불안이 유지된다.
- 307호를 직접 보여주기보다, 들어가고 싶게 만드는 데 집중한다.

### 4. 범위 잠금

#### 4.1 반드시 포함

- 관리실
- 로비 또는 1층 공용 구역
- 복도 2종
- 계단실
- 엘리베이터 앞 공간
- 세대 내부 4종 이상, 최종 6종 목표
- 지하 전기실
- 지하 창고 또는 보조 설비실
- 민원 보드
- 보고/판정 시스템
- 손전등, 마스터 키, 드라이버, 전기 설비 조작 도구
- 정상 민원, 점검 민원, 이상 판정 민원
- 307호 관련 후반 이벤트
- 초현실 공간 변주 규칙 세트

#### 4.2 가능하면 제외

- 전투
- 적 AI 추격
- 복잡한 물리 퍼즐
- 주민 대면 대화 시스템
- 괴물 직접 노출 중심 연출
- 멀티 엔딩 확장
- 대규모 월드 스트리밍
- 다중 건물 구조

#### 4.3 콘텐츠 잠금 규칙

- 민원은 "아이디어 수"가 아니라 "작동 템플릿 수"로 관리한다.
- 세대 내부는 6~8개를 모두 개별 제작하지 않고, 4개의 기본형을 만들고 상태 변화로 확장한다.
- 이상 현상은 15개 전체를 처음부터 만들지 않는다.
- 실제 출시 기준 핵심 세트는 민원 8종, 이상 현상 10종을 우선 목표로 한다.
- 초안의 12종 민원과 15종 이상 현상은 확장 후보군으로 유지한다.

### 5. 핵심 플레이 구조

#### 5.1 기본 루프

1. 관리실에서 민원을 확인한다.
2. 필요한 도구를 챙긴다.
3. 현장으로 이동한다.
4. 문제를 조사한다.
5. 수리하거나 이상 여부를 판정한다.
6. 관리실로 돌아와 보고한다.
7. 보고 결과에 따라 건물 상태가 변한다.

#### 5.2 민원 템플릿

모든 민원은 아래 3개 템플릿 중 하나에 속해야 한다.

1. 수리형
2. 점검형
3. 이상 판정형

이 규칙을 유지하면 콘텐츠 추가 시 새 시스템이 아니라 새 데이터만 추가하면 된다.

#### 5.3 챕터 진행 구조

- 프롤로그: 관리실과 이동, 상호작용, 보고 방식 학습
- 챕터 1: 수리형 중심
- 챕터 2: 점검형과 이상 판정형 시작
- 챕터 3: 정전, 지하실, 기록 불일치
- 챕터 4: 307호 집중
- 결말: 해석과 보고 결과의 수렴

#### 5.4 플레이어 목적 계층

- 즉시 목표: 현재 민원 1건을 처리하거나 보고 가능한 상태로 만든다.
- 진행 목표: 2~3건의 민원을 처리하며 건물 상태 변화를 유도한다.
- 장기 목표: 307호와 관련된 기록, 이름, 공간 모순을 따라간다.
- 실패 압박: 죽음보다 근무 상태 악화, 기록 오염, 이동 불편, 더 위험한 상태 전이를 느끼게 만든다.

#### 5.5 민원 1개 제작 단위

민원 하나는 아래 7개를 반드시 가져야 한다.

1. 접수 문구
2. 위치
3. 현장 상태
4. 확인 단서 2~3개
5. 플레이어 행동(수리/점검/판정)
6. 보고 결과
7. 월드 상태 변화

이 7개 중 하나라도 빠지면, 플레이어는 민원을 단순 이벤트로 느끼고 루프의 목적이 약해진다.

#### 5.6 챕터별 콘텐츠 의무

- 프롤로그: 수리형 민원 2개 이하, 관리실/보고 학습 필수
- 챕터 1: 수리형 민원이 중심이되, 작은 위화감 1회 이상 삽입
- 챕터 2: 이상 판정형 민원을 본격 도입하고 기록 불일치를 보여줘야 함
- 챕터 3: 정전, 지하실, 보조 설비실, 관리 기록 뒤틀림이 반드시 연결되어야 함
- 챕터 4: 307호 관련 민원과 공간 붕괴가 하나의 목표로 수렴되어야 함
- 결말: 마지막 보고 또는 마지막 진입이 플레이어 해석을 마감해야 함

### 6. 핵심 시스템 정의

#### 6.1 상호작용 시스템

- 레이캐스트 기반 포커스를 유지한다.
- 문, 스위치, 배전반, 메모, 민원 보드, 공구함에 공통 규칙을 적용한다.
- 상호작용 타입은 `Use`, `Inspect`, `Read`, `Report` 정도로 제한한다.
- 상호작용 카메라와 손전등/도구 사용은 RealityCam 기준으로 정렬한다.
- 현재 `Grab` 입력은 `IA_Grab` 하나로 통합하고, 입력 시작 시 카메라 중심 `Line Trace`로 문을 먼저 판정한 뒤 문이 아니면 물리 프랍 홀드를 시도한다.
- 문과 프랍의 기본 탐색 채널은 모두 `Visibility`를 사용한다.
- 물리 프랍은 전용 마커 컴포넌트가 붙은 오브젝트만 상호작용 대상으로 인정한다.
- 상호작용 중에는 RealityCam의 정밀 상호작용 감쇠를 재사용해 읽기성과 조작성을 유지한다.
#### 6.2 민원 보드 시스템

- 활성 민원 목록을 보여준다.
- 민원 선택 시 목적지와 민원 종류를 제공한다.
- 보고 결과는 다음 상태 중 일부만 사용한다.
  `해결 완료`, `이상 없음`, `추가 확인 필요`
- `일시 조치`, `접근 불가`는 본편 확장 시점에만 추가 검토한다.

#### 6.3 도구 시스템

- 손전등은 기본 장비다.
- 도구는 제한된 슬롯 또는 단순 순환 방식으로 관리한다.
- 도구는 퍼즐 복잡도를 만들기 위한 수단이 아니라 역할 몰입 장치다.
- 도구별 기능은 명확해야 하며, 중복 기능을 만들지 않는다.

#### 6.4 전기/조명 상태 시스템

- 전역 상태를 단순하게 유지한다.
- 추천 상태:
  `정상`, `부분 소등`, `층 정전`, `비상등`, `지하 독립 전원`
- 빛은 단순 가시성 확보가 아니라 감정 곡선과 공간 인식을 통제하는 핵심 시스템이다.
- 빛은 단순히 길을 보여주는 수단이 아니라, 현실의 경계와 공간의 깊이를 흔드는 장치다.
- Lumen 기반 간접광, 반사, 암부 변화까지 포함해 상태 전환을 설계한다.
- 조명 상태는 시각 효과만 바꾸지 않는다.
- 문 개폐, 사운드, 활성 민원, 특정 연출 조건과 연결한다.

#### 6.5 사운드 반응 시스템

- 사운드는 빛과 함께 긴장도를 가장 직접적으로 전달하는 코어 시스템이다.
- MetaSound를 기준으로 전기 잡음, 생활 소음, 구조 신호를 상태 반응형으로 구성한다.
- 정적과 생활 소음의 대비를 활용해 "누가 있는지 없는지 확신할 수 없는 불안"을 만든다.
- 사운드는 보이지 않는 존재를 설명하지 않고 암시해야 하며, 정면 노출보다 거리감과 방향감이 중요하다.
- 사운드는 단순 배경음이 아니라 조명 상태, 민원 종류, 제한 구역, 긴장도 단계와 연결한다.

#### 6.6 이상 판정 시스템

이 시스템은 본 게임의 핵심이므로 가장 먼저 구조를 고정해야 한다.

단순 규칙은 아래와 같다.

1. 민원에는 정상 해석과 이상 해석 가능성이 함께 존재한다.
2. 플레이어는 현장에서 2~3개의 증거를 확인한다.
3. 보고 시 최종 판정을 선택한다.
4. 판정 결과는 다음 민원, 긴장도, 공간 상태에 영향을 준다.

중요한 점은 이것을 추리 게임처럼 복잡하게 만들지 않는 것이다.

- 정답률보다 "찝찝한 확신"이 중요하다.
- 잘못된 판정은 즉시 게임오버가 아니라 업무 악화로 이어진다.
- 일부 민원은 정답이 하나가 아니라 챕터 상태에 따라 결과가 달라져도 된다.

#### 6.7 긴장도 시스템

긴장도는 숫자를 크게 드러내지 않는 내부 상태로 관리한다.

- 입력 요인: 민원 처리 횟수, 오판, 정전 체류, 제한 구역 진입, 307호 단서 획득
- 출력 요인: 복도 시야 왜곡, 반복 민원, 관리 기록 오류, 생활 소음 증가, 연출 트리거

권장 방식은 "완전 연속 수치"보다 "단계형 상태"다.

- 단계 0: 정상 근무
- 단계 1: 위화감
- 단계 2: 기록 불일치
- 단계 3: 공간 붕괴
- 단계 4: 307호 집중

##### 실패 압박 설계 원칙

- 실패는 진행 삭제가 아니라 월드 상태 악화로 환산한다.
- 실패는 조명, 사운드, 기록, 동선 중 최소 1개 이상을 바꿔야 한다.
- 실패는 다음 1~3분 플레이를 바꾸되, 전체 챕터 재시작을 만들지 않는다.
- 플레이어가 왜 불리해졌는지는 이해하되, 정확히 무엇이 더 나빠질지는 모르게 해야 한다.

##### 실패 단계 정의

- Soft Failure: 민원 지연, 단서 누락, 경미한 오판. 결과는 민원 악화, 추가 점검, 조명 상태 하락 정도로 제한한다.
- Pressure Failure: 정전 방치, 반복 오판, 제한 구역 무리한 체류. 결과는 기록 오염, 동선 우회, 생활 소음 강화, 관리실 재출력 민원 증가로 연결한다.
- Hard Failure: 후반 명백한 위험 구간에서만 허용한다. 결과는 짧은 체크포인트 롤백 또는 강제 퇴각이며, 긴 재반복은 금지한다.

##### 안티-프러스트레이션 규칙

- 동일 민원을 같은 상태로 재실행시키지 않는다.
- 실패 후에는 반드시 새 연출 정보 또는 새 목표를 제공한다.
- 손실은 시간보다 신뢰도와 안정감 붕괴로 치환한다.
- 플레이어는 실패 직후 회복 행동을 하나 이상 선택할 수 있어야 한다.

#### 6.8 저장/로드

- 자동 저장: 관리실 귀환 시
- 체크포인트 저장: 챕터 시작 시
- 수동 저장: 관리실 책상에서만 허용
- 저장 대상은 월드 상태 전체가 아니라 핵심 상태만 다룬다.

저장 항목 예시:

- 챕터 번호
- 활성 민원 목록
- 해결 완료 민원
- 조명/전기 상태
- 긴장도 단계
- 해금된 구역
- 307호 관련 단서 상태

#### 6.9 초현실 공간 연출 원칙

- 초현실은 꿈 장면을 새로 만드는 방식보다 복도, 계단실, 세대, 지하실의 구조 논리를 어긋나게 만드는 방식으로 구현한다.
- 공간 왜곡은 복도 길이 변화, 문 번호 불일치, 반사 오류, 닿지 않는 빛, 끝이 보이지 않는 암부로 표현한다.
- 괴물의 실체를 직접 보여주기보다, 빈 공간의 깊이, 문 너머 인기척, 시야 밖 이동감을 통해 심연과 미지의 공포를 만든다.
- 307호 관련 연출도 새 규칙을 추가하기보다 기존 공간 규칙이 무너지는 방식으로 강화한다.

### 7. 시스템-콘텐츠 분리 원칙

#### 7.1 콘텐츠 제작 단위

콘텐츠는 아래 단위로 쪼갠다.

- 민원 데이터
- 이상 현상 데이터
- 공간 상태 프리셋
- 조명 프리셋
- MetaSound 프리셋/사운드 이벤트
- 문서/기록 단서

#### 7.2 데이터 설계 원칙

- 민원은 Data Table 또는 Data Asset으로 관리한다.
- 민원 하나는 고유 ID를 가진다.
- 월드 액터는 직접 민원 내용을 갖지 않고, ID나 태그로 연결한다.
- 307호 관련 특수 로직도 가능한 한 데이터로 분리한다.
- 런타임 상태의 단일 진실 공급원은 데이터 ID와 상태 enum 조합으로 유지한다.
- 하드코딩 분기는 챕터 잠금, 특수 연출, 임시 디버그를 제외하면 지양한다.

#### 7.3 제작 효율 규칙

- 새로운 민원을 추가할 때 새 클래스를 만들지 않는다.
- 가능한 한 같은 상호작용 액터를 재사용한다.
- Nanite 기반 공간 자산을 기본값으로 삼고, 변주는 조명/소품/사운드 상태 변화로 만든다.
- 같은 공간은 조명, 소품, 사운드, 문 상태만 바꿔 재활용한다.
- 시퀀스 의존도를 줄이고 인게임 상태 변경으로 연출한다.

#### 7.4 콘텐츠 패키지 정의

##### Complaint Package

- 접수 문구
- 대상 위치
- 필요한 도구
- 현장 단서
- 가능한 보고 결과
- 성공/오판 시 상태 변화

##### Anomaly Package

- 이상 현상 이름
- 첫 노출 위치
- 시각 또는 사운드 단서
- 정상 해석 가능성
- 이상 판정 근거
- 이후 민원 또는 월드 상태와의 연결

##### Discovery Package

- 입주자 명부, 정비 로그, 경고문, 배전반 표기처럼 서사를 설명하는 단서 묶음
- 단독 수집품이 아니라 다음 민원 해석을 바꾸는 정보여야 한다.

##### World State Change Package

- 조명 상태 변화
- 출입 가능 구역 변화
- 반복 민원 재출력
- 기록/표기 오염
- 307호 관련 연출 단계 상승

### 8. Unreal 구현 방침

#### 8.1 구현 분리

- C++: 공통 프레임, 상태 관리, 상호작용 인터페이스, 저장 구조, 재사용 가능한 시스템
- Blueprint: 레벨 배치, 연출, UI, 사운드 트리거, 민원 콘텐츠 세팅
- Data Table/Data Asset: 민원 정의, 이상 현상 정의, 챕터 진행 데이터, 공간 상태 프리셋
- Lumen: 조명, 반사, 간접광 변화 기반 분위기 제어
- Nanite: 반복 공간에서도 실사 밀도를 유지하는 기본 메시 전략
- MetaSound: 전기/생활 소음의 상태 반응형 오디오 레이어

#### 8.2 현재 프로젝트 기준 권장 책임

- `ANCCharacterBase`: 이동 기반 공통 기능
- `ANCPlayerCharacter`: RealityCam 시점, 상호작용 추적, 물리 프랍 홀드, 물리 문 드래그, 손전등
- `ANCPlayerControllerBase`: UI 입력, 민원 보드, 보고 화면
- `ANCGameModeBase`: 챕터 시작, 민원 시퀀싱, 진행 규칙
- `ANCGameStateBase`: 현재 근무 상태, 조명 상태, 긴장도 단계
- `ANCPlayerStateBase`: 현재 단계에서는 비워두거나 최소 정보만 유지

#### 8.3 구현 규칙

- 싱글플레이 기준으로 설계한다.
- 네트워크 대응 코드는 만들지 않는다.
- GAS는 현재 범위에서 사용하지 않는다.
- Tick 기반 상시 검사보다 이벤트 기반 갱신을 우선한다.
- 빛과 사운드는 후반 폴리싱 항목이 아니라 초기 프로토타입부터 검증한다.
- 공통 상호작용은 인터페이스 또는 Actor Component로 분리한다.
- 코어 플로우에서는 `IsValid`를 남발하지 않는다.
- Lumen과 MetaSound는 조명 상태, 긴장도, 민원 상태와 직접 연결된 반응형 시스템으로 설계한다.
- Nanite는 배경 구조물과 고밀도 소품의 기본 선택지로 보고, 상호작용 요구가 분명한 자산만 예외 처리한다.
- 소유권과 초기화가 보장된 경로는 `check`, `checkf`, 명확한 순서 보장으로 관리한다.
- `IsValid`는 비동기 콜백, 약한 참조, 외부 입력처럼 경계가 불확실한 경우에만 사용한다.
- 주요 시스템은 선형 상태 흐름으로 유지한다.
- 추천 흐름은 `민원 활성화 -> 현장 조사 -> 결과 보고 -> 월드 상태 갱신 -> 다음 민원 해금` 이다.

#### 8.4 RealityCam 시스템 방침

- 플레이어 체험은 풀바디 1인칭이 아니라 메쉬 없는 카메라 중심 1인칭을 기준으로 한다.
- 플레이어 메쉬와 그림자는 현재 범위에서 사용하지 않고, 현실감은 카메라 거동과 후처리로 만든다.
- RealityCam은 과장된 바디캠 연출보다 가독성을 우선하는 현실적 핸드헬드 감각을 목표로 한다.
- 기본 레이어는 `Idle micro noise`, `Walk bob`, `Strafe sway`, `Turn inertia`, `Start/Stop impulse` 다섯 가지로 고정한다.
- 손전등, 문 열기, 스위치 조작은 모두 캐릭터 몸이 아니라 RealityCam 시야 기준으로 정렬한다.
- 정밀 상호작용 중에는 카메라 흔들림과 롤을 30~50% 감쇠해 읽기성과 조작성을 보장한다.
- 연출 시점 전환은 최소화하고, 가능한 한 플레이 입력을 유지한 상태에서 처리한다.
- v1 후처리는 낮은 `Scene Fringe`, 낮은 `Vignette`, `Motion Blur 0` 수준으로 제한한다.
- `lens distortion`, `overscan`, 과한 롤, 강한 모션 블러는 초기 범위에서 제외한다.

#### 8.5 물리 프랍 홀드 상호작용 방침

- 플레이어는 RealityCam 중심 `Line Trace`로 정확히 조준한 프랍만 잡을 수 있어야 한다.
- 프랍 홀드는 `UNCPropInteractorComponent`가 담당하고, `UPhysicsHandleComponent`를 사용해 카메라 전방의 고정 홀드 지점으로 끌어온다.
- 대상은 `UNCPhysicsCarryTargetComponent`가 붙은 액터로 제한한다.
- `ResolveGrabPrimitive()`는 `GrabPrimitiveOverride`가 유효하고 물리 시뮬레이션 중이면 그것을 우선 사용하고, 아니면 오너 액터의 첫 번째 물리 시뮬레이션 `UPrimitiveComponent`를 사용한다.
- 기본 입력은 `IA_Grab` 홀드이며, 입력을 떼면 즉시 프랍을 놓는다.
- 현재 기본 탐색 거리/홀드 거리/분리 거리는 각각 `350cm`, `90~170cm`, `220cm`다.
- 현재 최대 허용 질량은 `120kg`이며, 이를 초과하면 잡을 수 없고 홀드 중 질량이 기준을 넘겨도 즉시 릴리즈한다.
- 무거운 프랍일수록 `Physics Handle` stiffness와 interpolation은 낮아지고 damping은 높아져 더 둔하고 무겁게 움직여야 한다.
- 잡는 순간의 프랍 자세를 유지한 채 카메라 기준 상대 자세로 따라오게 한다.
- 홀드 중에는 RealityCam 흔들림을 감쇠해 읽기성과 배치 감각을 유지한다.
- 디버그 시각화는 `nc.PropInteraction.Debug 1`로 켜고, 라인트레이스 결과, 현재 히트 대상 정보, 주변 carry target 후보, 현재 홀드 중인 프랍 정보를 표시한다.
- 던지기, 회전 조작, 정밀 배치, 네트워크 동기화는 후속 범위로 분리한다.
#### 8.6 물리 문 래치/잠금 상호작용 방침

- 문은 일반 프랍과 분리된 전용 물리 문 액터 `ANCDoorActor`로 구현하고, `LockedClosed`, `LatchedClosed`, `FreeSwing`, `GrabDragging` 네 상태만 사용한다.
- 문 액터는 `DoorFrame`, `DoorLeaf`, `DoorHingeConstraint` 세 컴포넌트를 기본 구조로 사용한다.
- `DoorFrame`은 고정 앵커 역할만 하고 물리 시뮬레이션을 하지 않는다.
- `DoorLeaf`는 실제로 열리고 닫히는 문짝만 담당하며, `FreeSwing` 또는 `GrabDragging` 상태에서만 물리 시뮬레이션을 사용한다.
- 기본 닫힘 상태의 문은 몸으로 밀어도 열리지 않아야 하며, 먼저 `Grab` 또는 향후 `Use` 상호작용으로 unlatch 되어야 한다.
- 문이 unlatch 된 뒤에는 물리 힌지 상태로 전환되어 몸으로 밀거나 직접 잡아 당겨서 열 수 있어야 한다.
- 문 direct grab 은 플레이어의 공유 `Physics Handle`을 사용하지만, 문짝을 카메라 앞으로 옮기지 않고 현재 문 위치를 유지한 채 회전 목표만 갱신한다.
- 문 grab 목표는 `카메라 회전량` 자체가 아니라, grab 시작 시 라인트레이스로 맞춘 문 표면 지점을 문 로컬 공간과 카메라 로컬 공간에 각각 저장한 뒤, 이후 그 지점이 따라가야 할 월드 위치 차이를 힌지 평면 signed angle로 환산해 계산한다.
- 문이 거의 닫힌 각도에 도달하고 각속도가 충분히 낮아지면 자동으로 다시 `LatchedClosed` 상태로 돌아가야 한다.
- 현재 자동 래치 기준은 `닫힘 각도 4도 이하`, `각속도 8도/초 이하`다.
- 다시 latch 된 문은 `Grab` 또는 향후 `Use` 입력 없이는 몸으로 밀어도 다시 열리지 않아야 한다.
- `LockedClosed` 상태의 문은 몸, `Grab`, 일반 조작 모두 실패해야 하며, 외부 로직이 잠금을 해제하기 전에는 열리지 않아야 한다.
- 문 스윙 제한은 `MinOpenAngleDegrees`, `MaxOpenAngleDegrees`를 기반으로 하나, 실제 물리 힌지에는 두 값의 절대값 중 큰 값을 사용한 대칭 제한을 적용한다.
- 문 드래그 중에는 RealityCam 정밀 상호작용 감쇠를 재사용해 읽기성과 조작성을 유지한다.

#### 8.7 물리 상호작용 저작/디버그 기준

- 프랍을 잡히게 하려면 액터에 `UNCPhysicsCarryTargetComponent`를 붙이고, 대상 `UPrimitiveComponent`가 `Simulate Physics = true`이며 `Visibility` 라인트레이스를 막아야 한다.
- 여러 프리미티브를 가진 액터는 `GrabPrimitiveOverride`를 명시적으로 지정해 디자이너가 어느 컴포넌트를 잡을지 고정한다.
- 문을 배치할 때는 `DoorFrame`에는 고정된 문틀 또는 앵커 메시를, `DoorLeaf`에는 움직이는 문짝 메시만 넣는다.
- 문은 닫힌 기준 포즈로 배치해야 하며, `HingeLocalOffset`은 실제 경첩 위치와 최대한 맞춰야 한다.
- 현재 문 회전 해석은 일반적인 수직 힌지 도어를 전제로 하므로, 특수한 축을 가진 문이나 슬라이딩 도어는 범위 밖으로 본다.
- 프랍 디버그는 `nc.PropInteraction.Debug 1`, 비활성화는 `nc.PropInteraction.Debug 0`을 사용한다.
### 9. 레벨 제작 전략

#### 9.1 공간 제작 우선순위

1. 관리실
2. 복도 기본형
3. 세대 내부 기본형 1개
4. 지하 전기실
5. 계단실/엘리베이터 앞
6. 세대 변주형
7. 307호 특수 공간

#### 9.2 반복감 대응 방식

- 이동 동선을 짧게 유지한다.
- 같은 경로를 다시 지날 때는 최소 1개 이상 상태를 바꾼다.
- 복도 길이, 조명, 문 열림, 사운드, 표지판, 우편함 정보 중 하나는 달라져야 한다.
- 관리실과 지하실 이벤트로 루프 감정을 재설정한다.

### 10. 마일스톤 계획

#### 10.1 0단계: 사전 정리 (1주)

- 핵심 민원 템플릿 3종 확정
- 보고 결과 3종 확정
- 조명 상태 5종 확정
- Lumen 조명 원칙 확정
- MetaSound 사운드 원칙 확정
- Nanite 적용 기준 정리
- 데이터 구조 설계
- 첫 데모 동선 확정

합격 기준:

- "무엇을 만들지"보다 "무엇을 만들지 않을지"가 문서로 고정되어 있다.

#### 10.2 1단계: 프로토타입 (3~4주)

- 이동과 상호작용
- 래치/잠금 지원 문, 스위치, 손전등
- 관리실 민원 보드
- 보고 루프
- Lumen 조명 프리셋 1차
- MetaSound 환경음 프로토타입
- 복도 1종
- 세대 1종
- 정상 민원 2개
- 이상 판정 민원 1개

합격 기준:

- 민원 수주부터 보고까지 1개 루프가 완전히 동작한다.
- 빛과 사운드 최소 세트만으로도 공간 분위기 차이가 체감된다.

#### 10.3 2단계: 수직 슬라이스 (4~6주)

- 관리실, 복도, 세대 2~3종, 지하실
- 정전 이벤트
- 307호 떡밥 첫 제시
- 저장/로드
- Lumen 조명 패스 1차
- MetaSound 사운드 1차 패스
- Nanite 기반 공간 디테일 패스
- 30~45분 데모 완성

합격 기준:

- 플레이어가 데모 종료 시 307호에 대한 호기심을 가진다.
- 정상 민원과 이상 민원의 차이를 명확히 체감한다.

#### 10.4 3단계: 본편 생산 (8~12주)

- 핵심 민원 8종 완성
- 핵심 이상 현상 10종 완성
- 세대 내부 6종 구성
- 307호 후반부 제작
- 사운드 확장
- 조명 상태 변주 확장
- 문서/기록 단서 배치

합격 기준:

- 3~4시간 길이의 단일 캠페인 골격이 끊기지 않고 이어진다.

#### 10.5 4단계: 마감 (4~6주)

- 최적화
- 버그 수정
- UX 다듬기
- 트레일러 캡처
- Steam 페이지 준비
- 데모/출시 빌드 정리

합격 기준:

- 초반 15분 이탈률을 줄일 정도로 이동, 길찾기, 보고 UX가 안정적이다.

### 11. 우선순위 백로그

#### P0

- 상호작용 인터페이스
- 민원 데이터 구조
- 이상 현상 데이터 구조
- 챕터 진행 데이터 구조
- 민원 보드 UI
- 보고 UI
- 손전등
- RealityCam 시스템 세팅
- 물리 프랍 홀드 상호작용
- 래치/잠금 지원 문 액터, 스위치, 배전반 액터
- 조명 상태 전환
- Lumen 기반 조명 프리셋
- MetaSound 기반 환경 사운드 프레임
- MetaSound/사운드 트리거 매니저
- Nanite 자산 적용 기준
- 저장 데이터 구조

#### P1

- 이상 판정 분기
- 반복 민원 재출력
- 관리 기록 오류 연출
- 지하실 이벤트
- 307호 단서 시스템

#### P2

- 결말 연출 변주
- 추가 세대 변주
- 선택적 수집 요소
- 난이도/편의 옵션 확장

### 12. 리스크와 대응

#### 리스크 1: 콘텐츠가 예상보다 많아짐

- 대응: 새 민원 추가 금지, 기존 템플릿 재조합 우선

#### 리스크 2: 반복 이동이 지루해짐

- 대응: 왕복 동선 단축, 상태 변화 강제, 관리실 이벤트 삽입

#### 리스크 3: 공포가 약해짐

- 대응: 빛과 사운드 우선순위 상향, Lumen 조명 상태 변화와 MetaSound 기반 생활 소음/기계음 반응을 강화

#### 리스크 4: Blueprint가 비대해짐

- 대응: 상태 관리와 저장 구조는 초기부터 C++ 또는 공통 시스템으로 고정

#### 리스크 5: 후반부 범위 폭증

- 대응: 307호는 새 장르가 아니라 기존 루프의 뒤틀림으로 처리

#### 리스크 6: Lumen/Nanite/MetaSound 작업량 과소평가

- 대응: 프로토타입 단계에서 기술 기준과 예외 기준을 먼저 잠그고, 후반에 전면 교체하지 않는다.

### 13. 플레이 테스트 기준

- 플레이어가 3분 안에 지금 해야 하는 일을 말할 수 있는가
- 플레이어가 5분 안에 자신의 역할을 이해하는가
- 민원 처리 절차를 UI 도움 없이 따라갈 수 있는가
- 같은 공간 재방문이 지루하지 않은가
- 이상 판정 순간에 확신과 불안을 동시에 느끼는가
- 조명 상태와 사운드 변화만으로도 공간 감정이 뚜렷하게 달라지는가
- 괴물을 직접 보여주지 않아도 심연과 미지에 대한 공포가 유지되는가
- 데모 종료 시 307호 진입 욕구가 생기는가
- 플레이테스터가 "무서웠다"뿐 아니라 "무엇을 해야 하는 게임인지 명확했다"고 답하는가
- 실패했을 때 짜증보다 공포와 긴장이 먼저 느껴지는가

### 14. 즉시 실행 항목

1. 민원/이상 현상/보고 결과용 데이터 구조 정의
2. RealityCam 기준 캐릭터, 카메라, 손전등 정렬
3. 상호작용 인터페이스와 기본 액터 세트 제작
4. 관리실 민원 보드와 보고 화면 제작
5. 프로토타입용 복도 1개, 세대 1개, 지하실 1개 그레이박싱
6. 정상 민원 2개와 이상 민원 1개 구현
7. 조명 상태와 긴장도 단계 연동 구현
8. Lumen 기준 조명 프리셋과 광원 규칙 정의
9. MetaSound 기준 전기음/생활 소음 프로토타입 구성
10. Nanite 우선 적용 자산 분류와 실사 밀도 기준 정리

### 15. 최종 개발 판단

이 프로젝트는 "많이 만드는 게임"이 아니라 "적게 만들고, 반복 방문할수록 더 무서워지는 게임"으로 가야 한다.

성공 조건은 아래 3개다.

1. 관리실-현장-보고 루프를 끝까지 유지한다.
2. 307호를 향한 긴장을 데이터와 상태 변화로 누적한다.
3. 새 기능 추가보다 기존 시스템의 재조합으로 분량을 만든다.
4. 빛과 사운드가 단순 연출이 아니라 게임 상태를 설명하는 핵심 시스템으로 작동해야 한다.

이 원칙만 지키면 초안의 강점은 유지되고, 1인 개발 기준에서도 완성 가능성이 높다.

## 런타임 프레임워크 상세

### 1. 문서 목적

이 문서는 최근 추가한 런타임 프레임워크 구조를 설명한다.
대상 범위는 아래와 같다.

- 업적 접근 계층 (`Achievement Backend`, `Achievement Subsystem`)
- `GameMode / GameState` 역할 분리
- `ShiftStateComponent`, `ComplaintRuntimeComponent`
- `ComplaintRuntimeSubsystem`
- 디버그용 `CheatManager`
- 이후 Steam Integration Kit를 붙이는 절차

이 문서는 "현재 구조가 왜 이렇게 나뉘었는지"와 "앞으로 어디에 기능을 추가해야 하는지"를 빠르게 파악하는 용도로 작성한다.

### 2. 현재 구조 한눈에 보기

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

### 3. 지금 구현된 것과 아직 없는 것

#### 3.1 구현된 것

- 업적 백엔드 추상 클래스
- 기본 오프라인 백엔드 (`Null backend`)
- 업적 write 평가 및 dispatch용 `GameInstanceSubsystem`
- `GameInstance`에서 백엔드 클래스 선택 지점
- `GameState` 기반의 shift 상태 컴포넌트
- `GameState` 기반의 민원 런타임 상태 컴포넌트
- 민원 상태 제어용 `WorldSubsystem`
- 디버그용 `CheatManager` 명령 세트

#### 3.2 아직 없는 것

- Steam Integration Kit 실제 어댑터 클래스
- 민원 보드 / 보고 UI에서 `ComplaintRuntimeSubsystem` 호출
- 챕터 DataTable 자동 로드 및 민원 순차 해금 로직
- 저장/로드 연결
- 실제 월드 상호작용 액터가 민원 상태를 변경하는 연결부

즉 지금은 "구조와 접근 계층이 준비된 상태"이고, 이후 gameplay loop를 안전하게 올릴 수 있는 기반이 생긴 단계다.

### 4. 업적 프레임워크 설명

#### 4.1 클래스 역할

##### `UNCAchievementBackendBase`

플랫폼별 업적 구현체의 부모 클래스다.
이 클래스가 맡는 책임은 아래와 같다.

- backend 초기화
- 업적 write 처리
- backend 종료
- local debug snapshot 유지

현재는 `BlueprintNativeEvent`로 열어두었기 때문에, 나중에 Steam Integration Kit가 블루프린트 노드 중심이어도 연결할 수 있다.

##### `UNCNullAchievementBackend`

현재 기본 backend다.
실제 Steam 호출은 하지 않고 아래만 수행한다.

- 업적 unlock local 기록
- stat 증가 / 설정 local 기록
- threshold를 넘으면 local unlock 반영

즉 실제 플랫폼 없이도 `NCDumpAchievements`로 흐름 검증이 가능하다.

##### `UNCAchievementSubsystem`

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

#### 4.2 업적 write 흐름

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

#### 4.3 `WriteMode` 해석 기준

- `Unlock`
  - 단발 업적 해금
  - Steam에서는 보통 `SetAchievement`에 대응
- `IncrementStat`
  - 현재 값에 누적
  - Steam에서는 stat 증가 + 필요 시 업적 해금
- `SetStat`
  - 절대값 설정
  - Steam에서는 stat set + 필요 시 업적 해금

#### 4.4 현재 기준으로 빠진 부분 체크

지금 시점에서 업적 프레임워크에 실제로 남은 핵심 누락은 아래뿐이다.

- Steam Integration Kit adapter class
- `.uproject`에 플러그인 활성화
- 실제 `BP_GameInstance` 또는 프로젝트 기본 GameInstance에 backend class 지정

데이터 구조, write 결과, 중복 소비, debug snapshot, backend 선택 지점은 이미 있다.
즉 "프레임은 준비됐고, 실제 Steam 연결만 남은 상태"라고 보면 된다.

### 5. Steam Integration Kit 연결 절차

아래 절차를 추천한다.

#### 5.1 Adapter 클래스 만들기

다음 중 하나를 선택한다.

- C++ subclass of `UNCAchievementBackendBase`
- Blueprint subclass of `UNCAchievementBackendBase`

Steam Integration Kit가 블루프린트 노드 기반이면 BP subclass가 더 빠를 가능성이 높다.

#### 5.2 구현해야 하는 함수

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

#### 5.3 BP 구현 시 권장 규칙

`DispatchAchievementWrite`를 BP에서 override할 때는 아래를 지킨다.

- 성공 시 `true`, 실패 시 `false`를 명확하게 반환한다.
- 실패 이유가 있으면 `OutFailureReason`을 채운다.
- local debug snapshot은 subsystem이 성공 후 자동 기록하므로, parent 호출은 필수가 아니다.

#### 5.4 GameInstance에 연결하기

- `UNCGameInstance` 파생 BP 또는 기본 GameInstance 설정에서 `AchievementBackendClass`를 Steam adapter로 지정한다.
- 지정하지 않으면 기본값은 `UNCNullAchievementBackend`다.

#### 5.5 Steam adapter 구현 체크리스트

- `Unlock`이면 `SteamAchievementApiName` 사용
- `IncrementStat` / `SetStat`이면 `SteamStatApiName` 사용
- `SteamUnlockThreshold > 0`이면 stat 결과와 비교해 업적도 처리
- 성공 시 `true`, 실패 시 `false` 반환
- 실패 시 `OutFailureReason`을 채워서 subsystem이 로그를 남길 수 있게 할 것

### 6. GameState 기반 상태 모듈 설명

#### 6.1 왜 GameState에 붙였는가

민원 루프의 현재 상태는 아래 특성을 가진다.

- 여러 시스템이 읽어야 한다.
- UI, 디버그, GameMode, Subsystem이 공통으로 참조한다.
- 나중에 멀티플레이나 리플레이를 고려해도 `GameState` 쪽이 의미상 더 맞다.

그래서 상태는 `ANCGameStateBase`가 직접 들지 않고, 아래 두 컴포넌트로 쪼갰다.

#### 6.2 `UNCShiftStateComponent`

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

#### 6.3 `UNCComplaintRuntimeComponent`

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

### 7. `ComplaintRuntimeSubsystem` 사용 기준

`UNCComplaintRuntimeSubsystem`은 "민원 루프의 단일 접근점"이다.
`GameMode`, UI, 디버그, 월드 액터는 이 subsystem을 통해 상태를 바꾼다.

#### 7.1 지금 제공하는 함수

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

#### 7.2 사용 원칙

- 보드 UI는 `AcceptComplaint`를 호출한다.
- 현장 조사 시작 연출이나 진입 트리거는 `BeginInvestigation`을 호출한다.
- 단서 확보 액터는 `AddEvidenceTag`를 호출한다.
- 보고 UI는 `SubmitReport`를 호출한다.
- 민원 종료 확정 시 `CloseComplaint`를 호출한다.
- 업적 event는 subsystem을 통해 dispatch한다.

#### 7.3 하지 말아야 할 것

아래는 피하는 게 좋다.

- UI가 `GameState` 컴포넌트에 직접 값 쓰기
- 월드 액터가 achievement subsystem을 직접 호출하기
- GameMode가 민원 상태 배열을 직접 수정하기

이유는 access path가 여러 갈래가 되면 나중에 저장/로드와 디버그가 바로 꼬이기 때문이다.

### 8. GameMode / GameState 역할 분리

#### 8.1 `ANCGameModeBase`

현재 책임은 의도적으로 얇다.

- `StartPlay`에서 기본 shift phase 지정
- world subsystem 접근 helper 제공

앞으로 여기에 들어갈 책임은 아래 정도가 적절하다.

- 챕터 시작 시 DataTable 로드
- 초기 민원 활성화
- 보고 결과에 따른 다음 민원 해금
- 전체 루프 sequencing

#### 8.2 `ANCGameStateBase`

현재는 상태 저장용 modular shell이다.

- `ShiftStateComponent`
- `ComplaintRuntimeComponent`

즉 `GameModeBase`는 흐름을 관리하고, `GameStateBase`는 읽고 쓸 상태를 들고 있는 구조다.

### 9. 디버그 관리자 사용법

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

#### 9.1 enum 숫자 기준

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

### 10. 추천 다음 구현 순서

이 프레임 위에 바로 올리기 좋은 순서는 아래와 같다.

1. `BP_GameInstance`에 backend class 선택 가능하도록 asset 준비
2. Steam Integration Kit adapter backend 추가
3. 민원 보드 UI에서 `AcceptComplaint` 호출
4. 보고 UI에서 `SubmitReport` / `CloseComplaint` 호출
5. 월드 조사 포인트에서 `AddEvidenceTag` 연결
6. 챕터 DataTable 로드와 민원 해금 로직을 `GameModeBase`에 연결
7. 저장/로드에서 `ShiftStateComponent`, `ComplaintRuntimeComponent` 복원

### 11. 구현 원칙 정리

이 구조를 오래 쓰려면 아래만 지키면 된다.

- 플랫폼 업적 호출은 backend 안에만 둔다.
- gameplay는 subsystem만 호출한다.
- 상태는 GameState 컴포넌트가 들고, GameMode는 흐름만 관리한다.
- 디버그는 CheatManager를 통해 같은 경로를 탄다.
- UI와 월드 액터가 내부 배열이나 backend를 직접 건드리지 않는다.

## RealityCam 상세

### 문서 목적

이 문서는 `UNCRealityCameraComponent`가 어떤 구조로 설계되었고, 매 프레임 어떤 순서로 동작하는지 정리한 학습용 문서다.
`NightCaretaker` 프로젝트 안에서 구현된 버전을 기준으로 설명하며, 이후 `Sprint`, `Crouch`, `Landing`, `Breathing` 같은 상태를 추가할 때도 기준 문서로 사용할 수 있도록 작성한다.

대상 코드:
- `Source/NightCaretaker/Camera/NCRealityCameraComponent.h`
- `Source/NightCaretaker/Camera/NCRealityCameraComponent.cpp`
- `Source/NightCaretaker/Character/NCPlayerCharacter.cpp`

### 1. RealityCam의 역할

`UNCRealityCameraComponent`는 단순히 카메라를 붙여두는 컴포넌트가 아니라, 매 프레임 최종 카메라 뷰를 계산하는 "뷰 합성기"다.

핵심 개념은 아래와 같다.

1. 기본 시선은 플레이어 컨트롤러 회전을 그대로 사용한다.
2. 그 위에 아주 작은 현실감 레이어를 여러 개 더한다.
3. 각 레이어는 속도, 방향, 회전 변화량 같은 런타임 데이터를 입력으로 사용한다.
4. 마지막에 위치, 회전, FOV, 포스트 프로세스를 합산해 최종 POV를 만든다.

즉, 이 시스템은 "카메라를 흔드는 시스템"이라기보다 "카메라 결과를 절제된 방식으로 후처리하는 시스템"에 가깝다.

### 2. 왜 UCameraComponent를 상속했는가

`UNCRealityCameraComponent`는 `UCameraComponent`를 상속한다.
이 선택의 이유는 현재 프로젝트 구조가 단순하기 때문이다.

장점:
- 플레이어 Pawn에 바로 붙일 수 있다.
- 카메라의 기준 위치와 최종 뷰 계산을 한 클래스에 모을 수 있다.
- 블루프린트에서 파라미터를 노출하기 쉽다.
- `PlayerCameraManager`까지 가지 않아도 구현이 단순하다.

단점:
- 나중에 사망 카메라, 컷씬 카메라, 관전자 카메라, 조준 카메라가 많아지면 중앙 카메라 관리자 구조보다 확장성이 떨어질 수 있다.

현재 `NightCaretaker`는 싱글 플레이, 단일 기본 시점, 단순한 카메라 모드 전환을 전제로 하고 있으므로 `UCameraComponent` 상속이 가장 실용적이다.

### 3. 헤더 구조 설명

#### 3.1 `FNCRealityCameraTuning`

이 구조체는 "디자이너가 만질 수 있는 설정값 묶음"이다.
실행 중 변화하는 값이 아니라, 카메라 성격을 정의하는 값이다.

구성은 크게 여덟 영역이다.

- Base
  - `RealityCamIntensity`: 전체 RealityCam 효과 강도
  - `BaseOffset`: 캡슐 기준 카메라 기본 위치
  - `BaseFOV`: 정지 상태 기본 FOV
  - `MoveFOVBoostMax`: 최대 이동 시 추가 FOV
  - `MovementResponseSpeed`: 이동 기반 카메라 응답 보간 속도
- Idle
  - `IdleNoiseLocationAmplitude`
  - `IdleNoiseRotationAmplitude`
  - `IdleNoiseFrequency`
- Locomotion
  - `WalkBobLocationAmplitude`
  - `WalkBobRotationAmplitude`
  - `BaseBobFrequency`
  - `WalkBobFrequencyOverride`
  - `StrafeSwayLocationAmplitude`
  - `StrafeSwayYawAmplitude`
  - `MaxRollDegrees`
- Sprint
  - `SprintBobFrequencyOverride`
  - `SprintBobScale`
  - `SprintFOVBoostExtra`
- Rotation
  - `TurnYawLagDegrees`
  - `TurnPitchLagDegrees`
  - `TurnLagSpeed`
- Impulse
  - `StartStopLocationAmplitude`
  - `StartStopPitchAmplitude`
  - `StartStopResponseSpeed`
  - `StartStopRecoverySpeed`
- Interaction
  - `InteractionDampingFactor`
  - `PrecisionModeInterpSpeed`
- Post Process
  - `bEnablePostProcess`
  - `PostProcessBlendWeight`
  - `ChromaticAberrationIntensity`
  - `VignetteIntensity`
핵심 포인트는 "하나의 큰 수식"이 아니라, 목적이 다른 작은 레이어들의 강도를 각각 조절할 수 있게 나눠둔 점이다.

#### 3.2 `UNCRealityCameraComponent`의 런타임 상태값

컴포넌트는 설정값 외에도 내부 상태를 갖는다.
이 값들은 매 프레임 누적되거나 보간된다.

- 시간 누적
  - `NoiseTime`: idle 노이즈 시간축
  - `WalkCycleTime`: 보행 bob 주기 시간축
- 이동 스무딩
  - `SmoothedForwardAlpha`
  - `SmoothedRightAlpha`
  - `SmoothedSpeedAlpha`
- 회전 관성 상태
  - `CurrentTurnYawOffset`
  - `CurrentTurnPitchOffset`
- 시작/정지 충격 상태
  - `CurrentImpulseForwardOffset`
  - `CurrentImpulsePitchOffset`
- 상호작용 감쇠 상태
  - `PrecisionModeBlend`
  - `bPrecisionInteractionEnabled`
- 이전 프레임 회전 저장
  - `LastControlRotation`
  - `bHasLastControlRotation`

이 상태값들이 있기 때문에, 효과가 "입력을 바로 반영한 딱딱한 값"이 아니라 "살짝 지연되고 복귀하는 값"이 된다.

### 4. 플레이어에 어떻게 연결되는가

`ANCPlayerCharacter` 생성자에서 `RealityCameraComponent`를 만든다.

동작 순서:
1. 플레이어 캡슐을 세팅한다.
2. 이동 컴포넌트 속도를 설정한다.
3. `UNCRealityCameraComponent`를 생성해 캡슐에 부착한다.
4. `ApplyRuntimeTuning()`으로 기본 오프셋/FOV/포스트 프로세스를 적용한다.
5. `BaseEyeHeight`를 `BaseOffset.Z`에 맞춘다.
6. 메시와 그림자를 숨긴다.

이 구조 덕분에 현재 카메라는 "몸이 보이는 1인칭"이 아니라 "캡슐 중심의 카메라 전용 1인칭"으로 동작한다.

### 5. 프레임별 동작 순서

실제 핵심은 `GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)`에 있다.
이 함수는 매 프레임 호출되어 최종 POV를 만들어낸다.

전체 흐름은 아래와 같다.

1. `Super::GetCameraView()`로 기본 POV를 받는다.
2. 오너 캐릭터가 있는지 확인한다.
3. 현재 속도와 회전 데이터를 읽는다.
4. 이동 방향을 로컬 공간 성분으로 분해한다.
5. 입력값을 스무딩한다.
6. Idle / Walk / Strafe / Turn / Impulse 레이어를 각각 계산한다.
7. 레이어를 위치, 회전, FOV, 포스트 프로세스에 합산한다.
8. 최종 POV를 반환한다.

### 6. 각 레이어가 어떻게 계산되는가

#### 6.1 Base View

`Super::GetCameraView()`가 먼저 실행된다.
이 결과는 "캡슐에 붙은 기본 카메라 + 컨트롤러 회전"이다.
RealityCam은 이 값을 완전히 대체하지 않고, 그 위에 오프셋을 더한다.

#### 6.2 이동 데이터 읽기

컴포넌트는 오너 캐릭터의 `Velocity`를 읽고, 수직 성분을 제거한 평면 속도만 사용한다.
그 다음 속도를 `MaxWalkSpeed`로 나눠 `NormalizedSpeed`를 만든다.

#### 6.3 로컬 공간 분해

현재 카메라가 향하는 yaw 기준으로 forward/right 축을 만든 뒤, 속도를 dot product로 분해한다.

이 결과:
- `ForwardAlpha`: 전후 이동량
- `RightAlpha`: 좌우 이동량

#### 6.4 스무딩

`FInterpTo`를 사용해 이동 성분과 속도 비율을 부드럽게 만든다.
이 단계 덕분에 입력이 디지털이어도 카메라가 튀지 않는다.

#### 6.5 Idle Noise

`NoiseTime`을 누적하면서 `FMath::PerlinNoise1D()`를 사용해 아주 작은 랜덤성 있는 흔들림을 만든다.
이 레이어는 "정적이지 않은 손-held 느낌"을 담당한다.

#### 6.6 Walk Bob

걷기 흔들림은 리듬이 있어야 하므로 `sin`을 사용한다.
속도가 클수록 진폭이 커지고, 수직 위치와 pitch가 함께 움직인다.

#### 6.7 Strafe Sway

좌우 이동 시 카메라를 살짝 반대로 밀고, yaw와 roll도 같이 준다.
이 레이어가 없으면 횡이동이 미끄러지듯 느껴질 수 있다.

#### 6.8 Turn Inertia

이전 프레임 회전과 현재 회전의 차이를 이용해 회전 속도를 계산하고, 그 속도에 비례하는 yaw/pitch offset을 만든다.
빠르게 홱 돌릴 때만 약한 관성이 생기고, 천천히 볼 때는 거의 느껴지지 않는다.

#### 6.9 Start/Stop Impulse

속도 변화량 기반으로 출발과 정지 순간의 짧은 밀림을 만든다.
출발할 때는 살짝 뒤로 끌리고, 정지할 때는 살짝 앞으로 쏠리는 느낌을 준다.

### 7. 최종 합성 방식

각 레이어는 마지막에 둘로 합쳐진다.

- 위치 오프셋: `LocalLocationOffset`
- 회전 오프셋: `LocalRotationOffset`

위치 오프셋은 현재 카메라 회전에 맞춰 월드 공간으로 변환한다.

```cpp
DesiredView.Location += DesiredView.Rotation.RotateVector(LocalLocationOffset + IdleLocationOffset);
```

회전은 단순히 더한다.

```cpp
DesiredView.Rotation += LocalRotationOffset;
DesiredView.Rotation.Normalize();
```

FOV는 이동 속도에 따라 소폭 넓힌다.

```cpp
DesiredView.FOV = BaseFOV + SpeedAlpha * MoveFOVBoostMax * Intensity;
```

### 8. Precision Interaction 모드

이 시스템에는 정밀 상호작용용 감쇠 기능이 이미 들어 있다.

- `SetPrecisionInteractionEnabled(bool)`가 목표 상태를 켠다.
- `GetEffectiveIntensity()`에서 `PrecisionModeBlend`를 0~1로 천천히 보간한다.
- `InteractionDampingFactor`를 이용해 최종 강도를 줄인다.

즉 문 읽기, 스위치 조작, 잠금 해제 같은 상황에서 흔들림을 줄일 준비가 되어 있다.

### 9. 이 구조의 장점

- 구현이 단순하다.
- 카메라 효과가 레이어별로 분리되어 있어 튜닝이 쉽다.
- 이동, 회전, 시작/정지 같은 체감 요소를 독립적으로 조절할 수 있다.
- 메쉬가 없어도 일정 수준의 현실감을 만들 수 있다.
- 강도를 0으로 낮추면 거의 기본 카메라처럼 복귀할 수 있다.

### 10. 현재 구조의 한계

- `Sprint` 전용 bob/FOV 확장은 구현되었지만, `Crouch`, `Landing`, `Breathing` 전용 레이어는 아직 없다.
- 착지 충격은 아직 구현되지 않았다.
- 시작/정지 impulse는 실제 가속도가 아니라 속도 변화량 기반 근사다.
- 상호작용 감쇠는 현재 `Grab` 기반 문/프랍 상호작용에 연결되어 있으며, 향후 `Use`, `Inspect`, `Read`까지 확장 여지가 있다.
- 카메라 모드가 많아지면 나중에 `PlayerCameraManager` 계층으로 옮길 가능성이 있다.

### 11. 의사코드

```text
function GetCameraView(deltaTime):
    desiredView = Super.GetCameraView(deltaTime)

    owner = GetOwningCharacter()
    if owner is null or deltaTime is too small:
        return desiredView

    effectiveIntensity = GetEffectiveIntensity(deltaTime)
    if effectiveIntensity <= 0:
        reset transient offsets
        desiredView.FOV = BaseFOV
        desiredView.PostProcessBlendWeight = 0
        remember current rotation
        return desiredView

    planarVelocity = owner.velocity with Z removed
    normalizedSpeed = clamp(planarVelocity.length / maxWalkSpeed, 0, 1)

    controlYaw = desiredView.rotation.yaw only
    forwardDir = yaw basis X
    rightDir = yaw basis Y

    forwardAlpha = dot(planarVelocity, forwardDir) / maxWalkSpeed
    rightAlpha = dot(planarVelocity, rightDir) / maxWalkSpeed

    smoothedForwardAlpha = interp(smoothedForwardAlpha, forwardAlpha)
    smoothedRightAlpha = interp(smoothedRightAlpha, rightAlpha)
    smoothedSpeedAlpha = interp(smoothedSpeedAlpha, normalizedSpeed)

    noiseTime += deltaTime * IdleNoiseFrequency
    if normalizedSpeed > 0:
        walkCycleTime += deltaTime * activeBobFrequency * speedScale

    yawRate = deltaAngle(lastYaw, currentYaw) / deltaTime
    pitchRate = deltaAngle(lastPitch, currentPitch) / deltaTime

    targetTurnYawOffset = clamp(-yawRate * yawFactor, -TurnYawLagDegrees, TurnYawLagDegrees)
    targetTurnPitchOffset = clamp(-pitchRate * pitchFactor, -TurnPitchLagDegrees, TurnPitchLagDegrees)

    currentTurnYawOffset = interp(currentTurnYawOffset, targetTurnYawOffset)
    currentTurnPitchOffset = interp(currentTurnPitchOffset, targetTurnPitchOffset)

    speedDelta = normalizedSpeed - smoothedSpeedAlpha
    targetImpulseForward = clamp(-speedDelta * StartStopLocationAmplitude * 3)
    targetImpulsePitch = clamp(speedDelta * StartStopPitchAmplitude * 3)

    currentImpulseForward = interp(currentImpulseForward, targetImpulseForward)
    currentImpulsePitch = interp(currentImpulsePitch, targetImpulsePitch)

    idleLocationOffset = perlin noise based vector
    idleRotationOffset = perlin noise based rotator

    walkLocationOffset = sin(walkPhase) * WalkBobLocationAmplitude * smoothedSpeedAlpha
    walkPitchOffset = sin(walkPhase) * WalkBobRotationAmplitude * smoothedSpeedAlpha

    strafeLocationOffset = -smoothedRightAlpha * StrafeSwayLocationAmplitude
    strafeYawOffset = -smoothedRightAlpha * StrafeSwayYawAmplitude
    strafeRollOffset = clamp(-smoothedRightAlpha * MaxRollDegrees)

    localLocationOffset =
        impulseForward + strafeLocation + walkVertical

    localRotationOffset =
        idleRotation + walkPitch + turnPitch + impulsePitch + walkYaw + strafeYaw + turnYaw + strafeRoll

    desiredView.Location += rotateVectorByViewRotation(localLocationOffset + idleLocationOffset)
    desiredView.Rotation += localRotationOffset
    desiredView.Rotation.Normalize()
    desiredView.FOV = BaseFOV + smoothedSpeedAlpha * MoveFOVBoostMax * effectiveIntensity
    desiredView.PostProcessBlendWeight = PostProcessBlendWeight * effectiveIntensity

    return desiredView
```

### 12. 확장 우선순위

추천 순서:
1. 상호작용 시스템과 SetPrecisionInteractionEnabled()를 Use/Inspect/Read까지 확장
2. Landing 충격 레이어 추가
3. Breathing 또는 긴장도 기반 미세 흔들림 연동
4. Crouch 높이 전환과 카메라 오프셋 보간
5. 손전등 흔들림을 RealityCam 계산값과 연동

## 물리 상호작용 상세

### 1. 현재 구현 범위 요약

현재 물리 상호작용은 크게 두 갈래다.

1. 물리 프랍 홀드
2. 물리 문 래치/잠금/직접 드래그

두 시스템은 모두 플레이어의 RealityCam 중심 `Line Trace`를 사용하지만, 구현 책임은 분리되어 있다.

- 프랍 홀드: `UNCPropInteractorComponent`
- 프랍 대상 마커: `UNCPhysicsCarryTargetComponent`
- 문 상호작용: `ANCDoorActor`
- 입력 라우팅과 공용 `Physics Handle` 소유: `ANCPlayerCharacter`

핵심 전제는 다음과 같다.

- 모든 상호작용 기준은 플레이어 메쉬가 아니라 `RealityCameraComponent`다.
- 탐색 방식은 `Visibility` 채널 기반 `Line Trace`다.
- `IA_Grab` 입력 시작 시 문을 먼저 판정하고, 문이 아니면 프랍 홀드를 시도한다.
- 문 탐색 거리는 별도 값이 아니라 현재 프랍 상호작용의 `TraceDistance`를 재사용하며, 프랍 시스템이 없을 때만 `350cm`를 fallback으로 사용한다.
- 문과 프랍은 같은 입력을 공유하지만 런타임 로직은 완전히 분리되어 있다.
- 상호작용 중에는 RealityCam의 `Precision Interaction` 감쇠를 재사용한다.

### 2. 플레이어 소유 구조

현재 플레이어 `ANCPlayerCharacter`는 아래 컴포넌트를 직접 소유한다.

- `UNCRealityCameraComponent RealityCameraComponent`
- `UNCPropInteractorComponent PropInteractorComponent`
- `UPhysicsHandleComponent PhysicsHandleComponent`

또한 아래 입력 자산을 생성자에서 경로 로드한다.

- `/Game/NightCaretaker/Input/IMC_Default.IMC_Default`
- `/Game/NightCaretaker/Input/IMC_MouseLook.IMC_MouseLook`
- `/Game/NightCaretaker/Input/Actions/IA_Move.IA_Move`
- `/Game/NightCaretaker/Input/Actions/IA_Look.IA_Look`
- `/Game/NightCaretaker/Input/Actions/IA_MouseLook.IA_MouseLook`
- `/Game/NightCaretaker/Input/Actions/IA_Grab.IA_Grab`

입력 바인딩 책임은 다음과 같이 나뉜다.

- `Move`, `Look`: 일반 이동/시점
- `BeginGrabHold`: `IA_Grab Started`
- `EndGrabHold`: `IA_Grab Completed`, `IA_Grab Canceled`

현재 `RunSpeed` 기본값은 `450.0f`다.

### 3. Grab 입력 라우팅 순서

`ANCPlayerCharacter::BeginGrabHold()`의 동작 순서는 아래와 같다.

1. 이미 잡고 있는 문이 있고 아직 유효하면 아무 것도 하지 않는다.
2. 카메라 기준 `TraceDoorTarget()`로 문을 먼저 탐색한다.
3. 히트한 액터가 `ANCDoorActor`이면 `BeginDoorGrab()`을 시도한다.
4. 문 grab이 성공하면 `ActiveGrabbedDoor`를 저장하고 RealityCam 정밀 감쇠를 켠다.
5. 문이 아니거나 문 grab이 실패하면 `PropInteractorComponent->TryBeginGrab()`을 호출한다.

`ANCPlayerCharacter::EndGrabHold()`의 동작 순서는 아래와 같다.

1. 현재 잡고 있는 문이 있으면 문 grab을 먼저 끝낸다.
2. 문이 없으면 프랍 홀드를 끝낸다.
3. 마지막으로 RealityCam 정밀 감쇠 상태를 다시 계산한다.

추가로 `ANCPlayerCharacter::Tick()`은 현재 잡고 있는 문이 더 이상 유효한 `GrabDragging` 상태가 아니면 `ActiveGrabbedDoor`를 자동으로 비우고 Precision 상태를 다시 맞춘다.

즉, 현재 설계는 문이 프랍보다 높은 우선순위를 가진다.

### 4. 물리 프랍 홀드 시스템

#### 4.1 책임 클래스

- `UNCPropInteractorComponent`
  플레이어가 카메라 중심으로 프랍을 탐색하고, 물리 핸들로 잡고, 유지하고, 놓는 흐름 전체를 담당한다.
- `UNCPhysicsCarryTargetComponent`
  액터가 프랍 홀드 대상임을 표시하고, 실제로 어떤 `UPrimitiveComponent`를 잡을지 결정한다.

#### 4.2 대상 액터에 필요한 조건

프랍을 잡히게 하려면 아래 조건을 만족해야 한다.

1. 액터에 `UNCPhysicsCarryTargetComponent`가 붙어 있어야 한다.
2. 잡힐 `UPrimitiveComponent`가 존재해야 한다.
3. 그 컴포넌트는 `Simulate Physics = true`여야 한다.
4. 그 컴포넌트는 `Visibility` 라인트레이스를 막아야 한다.
5. 질량이 최대 허용 질량 이하여야 한다.

여러 프리미티브가 있는 액터는 `GrabPrimitiveOverride`를 지정하는 편이 안전하다.

#### 4.3 `ResolveGrabPrimitive()` 규칙

`UNCPhysicsCarryTargetComponent::ResolveGrabPrimitive()`는 아래 순서로 동작한다.

1. `GrabPrimitiveOverride`가 유효한 컴포넌트를 가리키는지 확인한다.
2. 그 컴포넌트가 `UPrimitiveComponent`이고 `IsSimulatingPhysics() == true`이면 즉시 반환한다.
3. override가 없거나 유효하지 않으면, 오너 액터의 모든 `UPrimitiveComponent`를 순회한다.
4. 그 중 첫 번째로 `IsSimulatingPhysics() == true`인 컴포넌트를 반환한다.
5. 끝까지 없으면 `nullptr`를 반환한다.

즉, override가 있어도 물리 시뮬레이션이 꺼져 있으면 무시되고, 자동 탐색으로 넘어간다.

#### 4.4 라인트레이스와 대상 검증

프랍 탐색은 `UNCPropInteractorComponent::TryResolveGrabTarget()`에서 처리한다.

동작 순서:

1. `UCameraComponent`와 `UWorld`를 얻는다.
2. 카메라 위치에서 전방 `TraceDistance`만큼 `LineTraceSingleByChannel`을 수행한다.
3. 채널은 `ECC_Visibility`를 사용한다.
4. 히트한 액터에서 `UNCPhysicsCarryTargetComponent`를 찾는다.
5. `ResolveGrabPrimitive()`로 실제 프리미티브를 얻는다.
6. `CanGrabPrimitive()`로 최종 검증한다.

검증 실패 사유 문자열은 현재 다음 값 중 하나로 나온다.

- `No trace hit`
- `Missing camera or world`
- `Hit actor has no carry target marker`
- `No resolved primitive component`
- `Primitive is not simulating physics`
- `Mass X kg exceeds Y kg`

#### 4.5 기본 튜닝 값

`FNCPropInteractionTuning` 기본값은 다음과 같다.

| 항목 | 기본값 | 의미 |
| --- | ---: | --- |
| `TraceDistance` | `350.0` cm | 프랍 탐색 최대 거리 |
| `MinHoldDistance` | `90.0` cm | 최소 홀드 거리 |
| `MaxHoldDistance` | `170.0` cm | 최대 홀드 거리 |
| `BreakDistance` | `220.0` cm | 홀드 유지 최대 이탈 거리 |
| `MaxCarryMassKg` | `120.0` kg | 최대 허용 질량 |
| `HandleLinearStiffness` | `7500.0` | 기본 선형 강성 |
| `HandleLinearDamping` | `220.0` | 기본 선형 감쇠 |
| `HandleAngularStiffness` | `4000.0` | 기본 회전 강성 |
| `HandleAngularDamping` | `250.0` | 기본 회전 감쇠 |
| `HandleInterpolationSpeed` | `12.0` | 기본 보간 속도 |
| `HeavyMassStiffnessScale` | `0.35` | 최대 질량 근처에서 남는 강성 비율 |
| `HeavyMassDampingScale` | `1.35` | 최대 질량 근처에서 적용되는 damping 비율 |
| `HeavyMassInterpolationScale` | `0.45` | 최대 질량 근처에서 남는 보간 속도 비율 |

#### 4.6 무게 기반 반응 스케일

프랍 질량이 무거워질수록 손에 더 무겁게 느껴지도록 `ApplyHandleTuning()`에서 질량 비례 스케일을 적용한다.

계산 흐름은 다음과 같다.

1. `MassAlpha = clamp(CurrentMass / MaxCarryMassKg, 0, 1)`
2. `StiffnessScale = lerp(1.0, HeavyMassStiffnessScale, MassAlpha)`
3. `DampingScale = lerp(1.0, HeavyMassDampingScale, MassAlpha)`
4. `InterpolationScale = lerp(1.0, HeavyMassInterpolationScale, MassAlpha)`
5. 위 스케일을 `Physics Handle`의 linear/angular stiffness, damping, interpolation speed에 곱한다.

결과적으로 무거운 프랍일수록 아래처럼 반응한다.

- 더 늦게 따라온다.
- 더 많이 끌리는 느낌이 남는다.
- 방향 전환이 둔해진다.
- 같은 거리에서도 더 큰 inertia가 느껴진다.

#### 4.7 프랍 grab 시작과 유지

`TryBeginGrab()` 흐름은 아래와 같다.

1. 이미 들고 있으면 true를 반환한다.
2. 카메라와 `Physics Handle`이 모두 있는지 확인한다.
3. 라인트레이스로 유효한 프랍을 찾는다.
4. 대상 질량에 맞춰 `Physics Handle` 튜닝을 적용한다.
5. 실제 잡은 위치는 `Hit.ImpactPoint`를 우선 사용하고, 아니면 프리미티브 중심을 사용한다.
6. 현재 카메라에서 grab 위치까지 거리를 계산해 `HeldDistance`로 저장하고, 이 값은 `MinHoldDistance`와 `MaxHoldDistance` 사이로 clamp한다.
7. 카메라 회전 기준 상대 회전 `HeldCameraRelativeRotation`을 저장한다.
8. `GrabComponentAtLocationWithRotation()`으로 잡는다.
9. 성공하면 즉시 현재 홀드 지점과 회전 목표를 한 번 적용한다.
10. RealityCam 정밀 감쇠를 켠다.

#### 4.8 프랍 홀드 유지와 자동 릴리즈

`UpdateHeldTarget()`은 매 틱 아래 순서로 실행된다.

1. 현재 홀드 중이 아니면 종료한다.
2. `HeldPrimitive`, `PhysicsHandle`, `ViewCamera`가 모두 유효한지 확인한다.
3. 핸들이 아직 같은 프리미티브를 잡고 있는지 확인한다.
4. 질량 검사를 다시 수행한다.
5. 현재 프리미티브 위치와 홀드 목표 지점 사이 거리가 `BreakDistance`를 넘으면 자동 릴리즈한다.
6. 조건을 모두 통과하면 `SetTargetLocationAndRotation()`으로 목표를 갱신한다.

즉, 홀드 시작 이후에도 아래 경우엔 자동으로 놓인다.

- 핸들이 다른 컴포넌트를 잡게 된 경우
- 프리미티브가 사라진 경우
- 물리 시뮬레이션이 꺼진 경우
- 현재 질량이 허용치를 넘긴 경우
- 홀드 목표와 프랍 사이 간격이 너무 벌어진 경우

#### 4.9 프랍 회전 규칙

프랍은 잡는 순간의 카메라 상대 회전을 저장한 뒤, 이후에도 그 상대 회전을 유지한다.

즉, 문과 달리 프랍은 아래 방식이다.

- 위치: 카메라 전방 고정 거리
- 회전: 카메라 회전에 종속되는 상대 자세 유지

#### 4.10 프랍 디버그 시각화

현재 프랍 디버그 cvar는 아래와 같다.

이 cvar는 현재 프로젝트 코드에서 유일하게 허용된 file-local `static TAutoConsoleVariable` 등록이다.

```text
nc.PropInteraction.Debug 0  // Off
nc.PropInteraction.Debug 1  // On
```

켜면 `UNCPropInteractorComponent::DrawDebugVisualization()`이 다음을 그린다.

1. 현재 카메라 기준 라인트레이스 선
2. 라인트레이스 히트 지점
3. 현재 히트 액터 이름
4. 대상 상태(`Grabbable` 또는 실패 이유)
5. 해석된 primitive 이름
6. 대상 질량
7. 카메라에서 대상까지 거리
8. 현재 들고 있는 프랍 정보
9. 현재 홀드 목표 지점 구체
10. 프랍 위치와 홀드 목표를 잇는 선
11. 주변 carry target 후보들의 상태 라벨

주변 carry target 후보는 현재 카메라 전방 시야 쪽에 있고 `TraceDistance` 안에 있는 액터만 표시한다.

라벨 색상 규칙은 현재 아래와 같다.

- Cyan: 현재 들고 있는 프랍
- Green: 현재 조준한 유효 대상
- Blue: 근처에 있고 잡을 수 있는 대상
- Orange: 근처에 있지만 현재 조건상 잡을 수 없는 대상
- Red: 현재 라인트레이스 히트는 했지만 유효 대상이 아님
- Yellow: 라인트레이스 미히트

### 5. 물리 문 시스템

#### 5.1 책임 클래스

문은 일반 프랍과 달리 `ANCDoorActor` 하나에 상태기와 물리 제약이 함께 들어 있다.

현재 문 액터는 다음 책임을 가진다.

- 닫힘 기준 포즈 캐시
- 힌지 constraint 설정
- 잠금/래치/자유 스윙/직접 드래그 상태 관리
- direct grab의 목표 각도 계산
- 자동 래치 판정
- 잠금 실패 이벤트 제공

#### 5.2 문 액터 기본 컴포넌트 구조

`ANCDoorActor`는 기본적으로 아래 컴포넌트를 만든다.

- `DoorFrameComponent`: 루트. 문틀 또는 앵커.
- `DoorLeafComponent`: 실제 문짝.
- `DoorHingeConstraintComponent`: 힌지 제약.

의도는 아래와 같다.

- `DoorFrame`은 움직이지 않는다.
- `DoorLeaf`만 움직인다.
- `DoorHingeConstraint`가 `DoorLeaf`의 이동을 잠그고 회전만 허용한다.

#### 5.3 문 배치 규칙

디자이너가 문을 배치할 때 지켜야 하는 규칙은 아래와 같다.

1. `DoorFrame`에는 고정된 문틀 메시 또는 보이지 않는 앵커 메시를 넣는다.
2. `DoorLeaf`에는 실제로 회전할 문짝 메시만 넣는다.
3. 프레임과 문짝이 합쳐진 단일 메시를 `DoorLeaf`에 넣지 않는다.
4. 문은 닫힌 기준 포즈로 배치한다.
5. `HingeLocalOffset`은 실제 경첩 축 위치에 최대한 맞춘다.
6. `DoorLeaf`는 `Visibility` 라인트레이스를 막는 편이 좋다.
7. `DoorLeaf`에는 적절한 simple collision이 있는 편이 안정적이다.

#### 5.4 문 상태기

현재 상태 enum은 아래 네 개뿐이다.

- `LockedClosed`
- `LatchedClosed`
- `FreeSwing`
- `GrabDragging`

상태 의미는 다음과 같다.

- `LockedClosed`
  완전히 잠긴 닫힘 상태. 몸으로 밀기, grab, 일반 조작 모두 실패한다.
- `LatchedClosed`
  잠겨 있지는 않지만 닫힘 래치가 걸린 상태. 몸으로 밀어서는 열리지 않는다.
- `FreeSwing`
  래치가 해제되어 물리 힌지로 자유롭게 흔들리는 상태. 몸으로 밀 수 있다.
- `GrabDragging`
  플레이어가 직접 문을 잡고 드래그하는 상태. 내부적으로는 여전히 물리 힌지 위에서 회전 목표를 갱신한다.

#### 5.5 문 상태 전환 규칙

현재 동작은 아래 규칙을 따른다.

- 시작 상태
  - `bStartLocked = true`면 `LockedClosed`
  - 아니면 `LatchedClosed`
- `TryActivateDoor()`
  - `LockedClosed`면 실패
  - `LatchedClosed`면 `FreeSwing`으로 전환
  - 이미 `FreeSwing` 또는 `GrabDragging`이면 그대로 유지
- `BeginDoorGrab()`
  - `LockedClosed`면 실패
  - `LatchedClosed`면 먼저 `TransitionToFreeSwing()`을 호출한 뒤 `GrabDragging`
  - `FreeSwing`이면 바로 `GrabDragging`
- `EndDoorGrab()`
  - `GrabDragging`이면 `FreeSwing`으로 돌아간다.
  - 직후 자동 래치 조건을 만족하면 `LatchedClosed`로 스냅한다.
- `SetDoorLocked(true)`
  - 언제든 `LockedClosed`로 스냅한다.
- `SetDoorLocked(false)`
  - `LockedClosed`였다면 `LatchedClosed`로 돌아간다.
- 틱 중 자동 래치
  - `FreeSwing` 상태에서만 체크한다.
  - 각도와 각속도가 모두 기준 이내면 `LatchedClosed`로 스냅한다.

#### 5.6 문 기본값

현재 문 액터 기본값은 다음과 같다.

| 항목 | 기본값 | 의미 |
| --- | ---: | --- |
| `bStartLocked` | `false` | 시작 시 잠금 여부 |
| `HingeLocalOffset` | `(0,0,0)` | 힌지 로컬 오프셋 |
| `MinOpenAngleDegrees` | `-100.0` | 최소 개방 각도 |
| `MaxOpenAngleDegrees` | `100.0` | 최대 개방 각도 |
| `ClosedAngleThresholdDegrees` | `4.0` | 자동 래치 가능한 닫힘 각도 기준 |
| `LatchAngularVelocityThresholdDegPerSec` | `8.0` | 자동 래치 가능한 최대 각속도 |
| `GrabLinearStiffness` | `0.0` | 문 grab 중 선형 강성 |
| `GrabLinearDamping` | `0.0` | 문 grab 중 선형 감쇠 |
| `GrabAngularStiffness` | `7000.0` | 문 grab 중 회전 강성 |
| `GrabAngularDamping` | `650.0` | 문 grab 중 회전 감쇠 |
| `GrabInterpolationSpeed` | `14.0` | 문 grab 보간 속도 |
| `MaxStableSwingLimitDegrees` | `170.0` | 물리적으로 안정적인 최대 대칭 스윙 한계 |
| `GrabMinimumProjectedDistance` | `10.0` cm | grab 수치 안정화에 필요한 최소 힌지 평면 거리 |

#### 5.7 힌지 constraint 설정 방식

`RefreshConstraintSetup()`은 다음을 고정한다.

- 힌지 위치를 `DoorFrameTransform.TransformPosition(HingeLocalOffset)`에 둔다.
- `DoorFrame`과 `DoorLeaf`를 constraint로 연결한다.
- 모든 linear movement를 잠근다.
- `Swing1`, `Swing2`는 잠근다.
- 실제 회전 자유도는 `Twist` 하나만 사용한다.
- `Twist` 제한값은 `max(abs(MinOpenAngleDegrees), abs(MaxOpenAngleDegrees))`를 `0~170` 범위로 clamp한 대칭 제한이다.

즉, 현재 문은 저작상 비대칭 개방 각도를 입력할 수 있어도 물리 힌지는 대칭 제한으로 동작한다. 이는 현재 구현의 한계이자 안정화 선택이다.

#### 5.8 닫힘 포즈 캐시와 스냅

`CacheClosedPose()`는 레벨에 배치된 현재 문짝 포즈를 `DoorFrame` 기준 상대 트랜스폼으로 저장한다.

그 후 `ApplyClosedState()`가 호출되면 아래가 일어난다.

1. 활성 grab을 해제한다.
2. 현재 물리 속도를 0으로 만든다.
3. `DoorLeaf` 물리 시뮬레이션을 끈다.
4. 캐시된 닫힘 월드 트랜스폼으로 문을 순간 이동시킨다.
5. 상태를 `LatchedClosed` 또는 `LockedClosed`로 바꾼다.

즉, 문이 다시 닫히면 단순히 각도만 0에 가까워지는 것이 아니라, 닫힌 기준 포즈에 정확히 스냅된다.

#### 5.9 문 unlatch와 자유 스윙

`TransitionToFreeSwing()`은 아래만 담당한다.

1. 잠긴 문이면 아무 것도 하지 않는다.
2. 이미 `FreeSwing` 또는 `GrabDragging`이면 아무 것도 하지 않는다.
3. 문짝을 닫힘 기준 포즈에 맞춘다.
4. `DoorLeaf`의 물리 시뮬레이션을 켠다.
5. rigid body를 깨운다.
6. 상태를 `FreeSwing`으로 바꾼다.

이 상태에 들어오면 몸으로 밀어 문을 열 수 있다.

#### 5.10 문 direct grab 방식

현재 문 direct grab은 문을 카메라 앞으로 끌지 않는다. 대신 아래 방식으로 움직인다.

- `Physics Handle`이 문짝을 잡고는 있다.
- 목표 위치는 항상 `DoorLeaf`의 현재 위치다.
- 목표 회전만 매 프레임 갱신한다.
- 회전 목표는 grab 시작 시 맞춘 문 표면 지점을 기준으로 계산한다.

이 방식의 목적은 다음과 같다.

- 문이 힌지에서 뜯겨 나가려는 선형 흔들림을 줄인다.
- 플레이어가 실제로 잡은 문 표면 지점이 손을 따라간다는 감각을 만든다.
- 카메라 회전값만으로 문이 돌아가는 부자연스러운 느낌을 줄인다.

#### 5.11 문 grab 시작 시 캐시하는 값

`BeginDoorGrab()` 성공 직전에 아래 값을 저장한다.

- `GrabPointLocalToDoorLeaf`
  - 히트한 문 표면 지점을 `DoorLeaf` 로컬 공간으로 변환한 값
- `GrabPointCameraLocalOffset`
  - 같은 히트 지점을 현재 카메라 로컬 공간으로 변환한 값
- `LastStableTargetAngleDegrees`
  - grab 시작 순간의 문 개방 각도

여기서 grab 위치는 `Hit.ImpactPoint`를 우선 사용하고, 히트 컴포넌트가 정확히 `DoorLeaf`가 아니면 문짝 중심을 사용한다.

#### 5.12 문 grab 업데이트 수식

`UpdateGrabDragTarget()`의 핵심 흐름은 아래와 같다.

1. 현재 문 위의 grab 지점을 다시 월드 공간으로 계산한다.
   - `CurrentDoorGrabPointWorld = DoorLeafTransform * GrabPointLocalToDoorLeaf`
2. 현재 카메라 기준 손이 있어야 할 월드 위치를 계산한다.
   - `DesiredHandPointWorld = CameraTransform * GrabPointCameraLocalOffset`
3. 힌지 원점과 힌지 축을 구한다.
4. 두 점을 모두 힌지 평면에 투영한다.
5. 힌지에서 현재 grab 지점으로 향하는 벡터와, 힌지에서 원하는 손 위치로 향하는 벡터 사이의 signed angle을 구한다.
6. `TargetDoorAngle = clamp(CurrentDoorAngle + DeltaAngle, MinOpenAngleDegrees, MaxOpenAngleDegrees)`를 계산한다.
7. 이 목표 각도로부터 문 목표 회전을 만든다.
8. `PhysicsHandle->SetTargetLocationAndRotation(CurrentDoorLocation, TargetDoorRotation)`을 호출한다.

즉, 현재 구현은 다음 입력을 모두 자연스럽게 반영한다.

- 마우스로 시점을 돌리는 것
- 플레이어가 앞뒤로 움직이는 것
- 플레이어가 좌우로 움직이는 것
- 잡은 지점이 손잡이에 가까운지, 문 중앙에 가까운지에 따른 레버리지 차이

#### 5.13 signed angle 계산 방식

문이 실제로 도는 각도는 `GetSignedAngleOnHingePlane()`에서 계산한다.

방식은 다음과 같다.

1. `FromWorld - HingeWorldLocation` 벡터를 힌지 평면에 투영한다.
2. `ToWorld - HingeWorldLocation` 벡터를 힌지 평면에 투영한다.
3. 두 벡터를 정규화한다.
4. `cross`와 `dot`를 이용해 `atan2` signed angle을 계산한다.
5. 라디안을 도 단위로 바꿔 반환한다.

이 방식 덕분에 단순 yaw 차이를 쓰는 것보다, 실제 힌지 위치와 grab 위치를 기준으로 한 회전량을 얻을 수 있다.

#### 5.14 수치 안정화 장치

grab 지점이 힌지에 너무 가까우면 문 회전 계산이 불안정해진다. 이를 막기 위해 현재 구현은 아래 안전장치를 둔다.

- 힌지 평면에 투영한 현재/목표 벡터 길이가 `GrabMinimumProjectedDistance`보다 작으면 새 각도를 계산하지 않는다.
- 대신 `LastStableTargetAngleDegrees`를 그대로 사용한다.

즉, 힌지 바로 옆을 잡았을 때 급격한 각도 튐을 억제한다.

#### 5.15 자동 래치 조건

`ShouldAutoLatch()`는 현재 아래 조건을 모두 만족할 때만 true다.

1. 상태가 `FreeSwing`일 것
2. 절대 개방 각도 `<= ClosedAngleThresholdDegrees`
3. 각속도 `<= LatchAngularVelocityThresholdDegPerSec`

여기서 각속도는 `DoorLeaf`의 물리 각속도를 `DoorFrame`의 up vector에 투영한 값으로 계산한다.

#### 5.16 문 이벤트

현재 문 액터는 아래 Blueprint 이벤트를 노출한다.

- `OnDoorLatched`
- `OnDoorUnlocked`
- `OnLockedInteractionAttempted`

현재 잠금 해제 로직은 별도 키 시스템과 연결되어 있지 않고, 외부 C++ 또는 Blueprint가 `SetDoorLocked(bool)`를 호출해 제어하는 구조다.

### 6. RealityCam 연동

프랍과 문은 모두 RealityCam 정밀 감쇠를 사용한다.

- 프랍 grab 성공 시 `SetPrecisionInteractionEnabled(true)`
- 프랍 grab 종료 시 `false`
- 문 grab 성공 시 `true`
- 문 grab 종료 또는 door state 동기화 후 필요 없으면 `false`

플레이어는 `RefreshPrecisionInteractionState()`에서 아래 기준으로 최종 상태를 계산한다.

- 현재 잡고 있는 문이 있는가
- 현재 프랍을 들고 있는가

둘 중 하나라도 true면 RealityCam 정밀 감쇠를 유지한다.

### 7. 현재 알려진 제한사항

현재 구현에는 아래 제한이 있다.

1. 문과 프랍 모두 `Visibility` 채널 하나만 사용한다.
2. 프랍은 `IA_Grab`만 지원하고, 던지기/회전/정밀 배치는 없다.
3. 문은 `Use` 입력이 아직 별도 자산으로 연결되지 않았고, 현재 직접적인 체험은 `Grab` 중심이다.
4. 문 constraint는 대칭 twist 제한만 사용하므로 비대칭 스윙 제한을 정밀하게 표현하지 않는다.
5. 문 회전 계산은 일반적인 수직 힌지 도어를 기준으로 한다.
6. 슬라이딩 도어, 위로 열리는 해치문, 복합 힌지문은 현재 범위 밖이다.
7. 네트워크 복제는 고려하지 않는다.
8. 문 전용 디버그 cvar는 아직 없다.
9. 문 탐색은 별도 전용 거리 값을 두지 않고 현재 프랍 상호작용의 trace 거리와 같은 값을 사용한다.

### 8. 디자이너 체크리스트

#### 8.1 잡을 수 있는 프랍 액터

- `UNCPhysicsCarryTargetComponent`를 붙였는가
- 대상 `UPrimitiveComponent`가 `Simulate Physics = true`인가
- `Visibility` 트레이스를 막는가
- 질량이 `120kg` 이내인가
- 여러 프리미티브가 있다면 `GrabPrimitiveOverride`를 지정했는가

#### 8.2 문 액터

- `DoorFrame`에는 문틀 또는 고정 앵커 메시를 넣었는가
- `DoorLeaf`에는 움직이는 문짝 메시만 넣었는가
- 문이 닫힌 기준 포즈로 배치되어 있는가
- `HingeLocalOffset`이 실제 경첩 위치와 맞는가
- `MinOpenAngleDegrees`, `MaxOpenAngleDegrees`가 의도한 문 개방 범위와 비슷한가
- `DoorLeaf` collision이 너무 복잡하지 않은가
- 몸으로 밀 수 있는 상태와 래치 상태가 의도대로 나오는가

### 9. 빠른 검증 절차

#### 9.1 프랍

1. 테스트 프랍 액터에 `UNCPhysicsCarryTargetComponent`를 붙인다.
2. 물리 시뮬레이션과 `Visibility` 충돌을 켠다.
3. `nc.PropInteraction.Debug 1`을 입력한다.
4. 라인트레이스 라벨이 `Grabbable`인지 본다.
5. `IA_Grab`으로 잡는다.
6. 무거운 프랍일수록 더 느리게 따라오는지 확인한다.
7. 목표 지점과 너무 멀어지면 자동으로 놓이는지 확인한다.

#### 9.2 문

1. `ANCDoorActor`를 배치한다.
2. `DoorFrame`과 `DoorLeaf` 메시를 지정한다.
3. 문을 닫힌 기준 포즈로 맞춘다.
4. `LatchedClosed` 상태에서 몸으로 밀어도 안 열리는지 확인한다.
5. `IA_Grab`으로 직접 잡아 열 수 있는지 확인한다.
6. grab 상태에서 좌우 이동과 앞뒤 이동 모두 문 회전에 반영되는지 확인한다.
7. 거의 닫힌 위치에서 놓으면 다시 래치되는지 확인한다.
8. `LockedClosed`로 바꿨을 때 어떤 방식으로도 열리지 않는지 확인한다.

### 10. 후속 확장 추천

현재 구조를 유지한 채 다음 확장이 가능하다.

1. 문 `IA_Use` 입력 추가
2. 프랍 throw 기능
3. 프랍 rotation adjust 기능
4. 문 전용 디버그 draw
5. 문 손잡이 컴포넌트 기반 세밀한 grab 포인트 정책
6. 비대칭 스윙 제한 지원
7. 잠금 해제용 키/퍼즐 시스템 연동
8. 저장/로드 시 문 상태와 프랍 위치 저장 정책 정리

## 데이터 작성 가이드 상세

### 1. 문서 목적

이 문서는 현재 C++에 구현된 데이터 구조를 기준으로, 에디터에서 어떤 자산을 어떤 순서로 만들고 어떤 값을 넣어야 하는지 설명한다.
읽으면서 바로 자산을 만들 수 있도록 아래 4가지를 중심으로 정리한다.

- 무엇을 먼저 만들지
- 각 필드에 무엇을 넣을지
- 민원, 이상 현상, 업적이 서로 어떻게 연결되는지
- 실제 런타임에서 어떤 흐름으로 소비될 예정인지

### 2. 현재 구현 범위

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

### 3. 전체 구조 한눈에 보기

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

### 4. 추천 폴더 구조

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

### 5. 먼저 만들어야 하는 순서

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

### 6. ID, Enum, Tag를 언제 쓰는가

#### 6.1 FName

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

#### 6.2 Enum

enum은 "개수가 작고 코드 분기가 고정된 값"에 쓴다.

- `ENCComplaintTemplateType`
- `ENCReportResult`
- `ENCComplaintRuntimeState`
- `ENCAchievementWriteMode`
- `ENCComplaintAchievementTrigger`
- `ENCAnomalyAchievementTrigger`

#### 6.3 GameplayTag

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

### 7. 업적 정의 자산 설정 방법

업적 자산은 `UNCAchievementDefinition`으로 만든다.
이 자산은 "업적 자체의 정체성"과 "Steam 쪽 이름"을 담당한다.
민원이나 이상 현상은 이 자산을 참조만 한다.

#### 7.1 필드 설명

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

#### 7.2 추천 입력 규칙

- 단발 업적이면 `DefaultWriteMode = Unlock`
- 누적형 업적이면 `DefaultWriteMode = IncrementStat`
- 누적형 업적은 가능하면 `SteamAchievementApiName`과 `SteamStatApiName`을 둘 다 채운다.
- `SteamUnlockThreshold`는 stat 업적일 때만 1 이상으로 넣는다.

#### 7.3 단발 업적 예시

- `AchievementId`: `ACH_FirstCorrectReport`
- `DisplayName`: `첫 정확한 보고`
- `CategoryTags`: `Achievement.Category.Story`
- `DefaultWriteMode`: `Unlock`
- `SteamAchievementApiName`: `ACH_FIRST_CORRECT_REPORT`
- `SteamStatApiName`: 비움
- `SteamUnlockThreshold`: `0`

#### 7.4 누적 업적 예시

- `AchievementId`: `ACH_Close10Complaints`
- `DisplayName`: `야간 근무 적응`
- `CategoryTags`: `Achievement.Category.System`
- `DefaultWriteMode`: `IncrementStat`
- `SteamAchievementApiName`: `ACH_CLOSE_10_COMPLAINTS`
- `SteamStatApiName`: `STAT_COMPLAINTS_CLOSED`
- `SteamUnlockThreshold`: `10`

### 8. 이상 현상 자산 설정 방법

이상 현상 자산은 `UNCAnomalyDefinition`으로 만든다.
이 자산은 "이상 현상이 무엇인지"와 "발견 시 어떤 증거/업적이 연결되는지"를 정의한다.

#### 8.1 필드 설명

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

#### 8.2 AchievementEvents의 의미

- `Discovered`
  - 월드에서 처음 발견했을 때
- `Confirmed`
  - 플레이어가 명시적으로 판정하거나 확인했을 때
- `Resolved`
  - 연출 또는 보고 결과로 마무리되었을 때

현재 런타임 브리지가 아직 없으므로, 이 값은 "다음 단계에서 어떤 타이밍으로 처리할지 미리 명세하는 계약"이라고 생각하면 된다.

#### 8.3 예시

문 아래 그림자 이상 현상:

- `AnomalyId`: `ANM_2F203_ShadowUnderDoor`
- `DisplayName`: `203호 문 아래 그림자`
- `AnomalyTypeTag`: `Anomaly.Type.ShadowUnderDoor`
- `EvidenceTagsGranted`:
  - `Evidence.Visual`
  - `Evidence.Environmental`
- `ActivationTags`:
  - `Progression.Chapter.One`

### 9. 민원 자산 설정 방법

민원 자산은 `UNCComplaintDefinition`으로 만든다.
이 자산이 실제 플레이 루프의 중심이다.

#### 9.1 필드 설명

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

#### 9.2 TemplateType 해석 기준

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

#### 9.3 AllowedReportResults 작성 원칙

- 비워두지 않는다.
- 실제 UI에 노출할 선택지만 넣는다.
- 정상 민원은 보통 `Resolved` 하나로 충분하다.
- 이상 판정 민원은 `NoAnomaly`, `NeedsFollowUp` 조합이 자주 쓰인다.

#### 9.4 LinkedAnomalies 작성 원칙

- 이상 현상 정의를 중복 작성하지 말고 재사용한다.
- 같은 이상 현상이 여러 민원에 재사용되어도 괜찮다.
- 이상 현상이 없는 정상 민원은 비워둘 수 있다.

#### 9.5 예시 1: 정상 수리 민원

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

#### 9.6 예시 2: 이상 판정 민원

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

### 10. 민원 업적 이벤트 설정 방법

민원 업적은 `FNCComplaintAchievementEvent`로 작성한다.
이 구조는 "언제", "어떤 조건에서", "어떤 업적 정의를 어떤 값으로 기록할지"를 뜻한다.

#### 10.1 Trigger 의미

- `Accepted`: 민원을 수주했을 때
- `InvestigationStarted`: 현장 조사 시작 시점
- `ReportSubmitted`: 보고 제출 시점
- `Closed`: 민원이 최종 종료되었을 때

#### 10.2 필드 설명

- `RequiredReportResults`
  - 특정 보고 결과에서만 업적이 발동해야 할 때 사용한다.
  - 비워두면 결과 상관없이 평가 가능하다고 본다.
- `RequiredEvidenceTags`
  - 반드시 확보된 증거가 있어야 할 때 사용한다.
- `MinDiscoveredEvidenceCount`
  - 증거 개수 기준을 추가로 제한한다.
- `AchievementWrite`
  - 실제 업적 정의와 write 조건이다.

#### 10.3 AchievementWrite 필드 설명

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

#### 10.4 예시 1: 첫 민원 완료 업적

민원 `Closed` 시 업적 해금:

- `Trigger`: `Closed`
- `RequiredReportResults`: `Resolved`
- `AchievementWrite.WriteId`: `WR_CMP_2F203_LightBuzz_FirstResolve`
- `AchievementWrite.Achievement`: `ACH_FirstCorrectReport`
- `AchievementWrite.ProgressValue`: `1`
- `AchievementWrite.bConsumeWriteOnce`: `true`

#### 10.5 예시 2: 민원 10회 완료 누적 업적

모든 민원 종료 시 stat 1 증가:

- `Trigger`: `Closed`
- `AchievementWrite.WriteId`: `WR_Generic_ComplaintClosed_Count`
- `AchievementWrite.Achievement`: `ACH_Close10Complaints`
- `AchievementWrite.ProgressValue`: `1`
- `AchievementWrite.bConsumeWriteOnce`: `true`

주의:

- 이 write는 여러 민원 자산에 공통으로 넣을 수 있다.
- 다만 `WriteId`는 소비 기준이 source asset별인지 전역인지 나중에 브리지에서 정해야 하므로, 현재는 "자산 안에서 의미가 명확한 이름"으로 짓는 것을 권장한다.

### 11. 이상 현상 업적 이벤트 설정 방법

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

### 12. 챕터 DataTable 설정 방법

챕터 편성은 `FNCChapterComplaintRow` DataTable에서 한다.
여기에는 민원 본문을 넣지 않는다.
본문은 항상 `UNCComplaintDefinition` 자산에만 둔다.

#### 12.1 필드 설명

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

#### 12.2 예시

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

### 13. 실제 구성 흐름 예시

"이상 민원 하나 + 업적 두 개"를 만드는 흐름은 아래처럼 잡으면 된다.

1. 업적 자산 생성
2. 이상 현상 자산 생성
3. 민원 자산 생성
4. 챕터 DataTable에 민원 등록
5. 나중에 런타임 브리지가 들어오면 같은 구조를 그대로 소비

#### 13.1 업적 자산 2개 생성

- `ACH_FirstAnomalyWitness`
  - 첫 이상 현상 발견
  - `DefaultWriteMode = Unlock`
- `ACH_Close10Complaints`
  - 민원 10회 종료
  - `DefaultWriteMode = IncrementStat`
  - `SteamStatApiName = STAT_COMPLAINTS_CLOSED`
  - `SteamUnlockThreshold = 10`

#### 13.2 이상 현상 자산 생성

- `ANM_2F203_ShadowUnderDoor`
- `AnomalyTypeTag = Anomaly.Type.ShadowUnderDoor`
- `EvidenceTagsGranted = Evidence.Visual, Evidence.Environmental`
- `AchievementEvents`
  - `Discovered -> ACH_FirstAnomalyWitness`

#### 13.3 민원 자산 생성

- `CMP_2F203_ShadowComplaint`
- `TemplateType = AnomalyJudgement`
- `LinkedAnomalies = ANM_2F203_ShadowUnderDoor`
- `RequiredEvidenceTags = Evidence.Visual, Evidence.Environmental`
- `MinEvidenceCountForReport = 2`
- `AllowedReportResults = NoAnomaly, NeedsFollowUp`
- `DefaultCanonicalResult = NeedsFollowUp`
- `AchievementEvents`
  - `Closed -> ACH_Close10Complaints`, `ProgressValue = 1`

#### 13.4 DataTable 등록

- `ChapterId = CH_01`
- `SortOrder = 20`
- `bStartAvailable = false`
- `RequiredProgressionTags = Progression.Story.Room307Clue`

### 14. 자주 하는 실수

- `LocationId` 대신 도메인 태그에 위치 정보를 넣는 것
- `AllowedReportResults`를 비워두는 것
- `DefaultWriteMode = IncrementStat`인데 `SteamStatApiName`을 비워두는 것
- 업적 자산 없이 Complaint / Anomaly 이벤트에 write만 넣는 것
- `WriteId`를 임시 이름으로 두고 나중에 계속 바꾸는 것
- 민원의 `RequiredEvidenceTags`와 이상 현상의 `EvidenceTagsGranted`가 서로 맞지 않는 것
- 반복 업적인데 `bConsumeWriteOnce = true`를 무심코 켜두는 것

### 15. 최종 체크리스트

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

### 16. 이후 구현에서 이 데이터가 소비되는 흐름

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

### 17. 한 줄 기준 정리

빠르게 기억하려면 아래 한 줄로 정리하면 된다.

- 업적은 `UNCAchievementDefinition`에 정의한다.
- 이상 현상은 `UNCAnomalyDefinition`에 정의한다.
- 민원은 `UNCComplaintDefinition`에 정의한다.
- 챕터 순서는 `FNCChapterComplaintRow` DataTable에서 관리한다.
- 정확한 ID는 `FName`, 고정 분기는 enum, 확장 조건은 GameplayTag를 쓴다.

## 데이터 매핑 상세

### 1. 고정 매핑 규칙

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

### 2. 추천 자산 경로

```text
/Game/NightCaretaker/Data/Complaint/
/Game/NightCaretaker/Data/Anomaly/
/Game/NightCaretaker/Data/Chapter/
```

### 3. Complaint Definition Mapping

#### CMP_PRO_OfficeLightBuzz

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

#### CMP_PRO_203_WaterAtDoor

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

#### CMP_CH1_2F_IntercomStatic

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

#### CMP_CH1_1F_MailboxMisdelivery

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

#### CMP_CH1_3F_EmergencyLight

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

#### CMP_CH1_205_OdorAtDoor

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

#### CMP_CH1_StairAutoLightDelay

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

#### CMP_CH2_302_TVHum

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

#### CMP_CH2_204_NameplateMismatch

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

#### CMP_CH2_4F_CCTVBlank

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

#### CMP_CH2_ElevatorFloorError

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

#### CMP_CH2_ReopenedLightCase

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

#### CMP_CH3_BasementPumpAlarm

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

#### CMP_CH3_306_OpenDoorAlarm

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

#### CMP_CH3_ExitSignDepth

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

#### CMP_CH3_BasementPanelMislabel

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

#### CMP_CH4_307_PackageAtDoor

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

#### CMP_CH4_307_LifeNoise

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

#### CMP_CH4_RecordResident307

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

#### CMP_CH4_FinalCheck307

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

### 4. Anomaly Definition Pack

#### ANM_OFFICE_3F07_NOTE

- DisplayName: `의미 불명 3F 07 메모`
- Description: `관리실 전등 교체 흔적 아래에 남은 의미 불명 메모. 아직은 단서라기보다 숫자 잔상에 가깝다.`
- AnomalyTypeTag: `Anomaly.Type.RecordConflict`
- EvidenceTagsGranted: `Evidence.Document`
- ActivationTags: `Progression.Chapter.Prologue`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.Room307Clue`

#### ANM_203_WATER_WITHOUT_SOURCE

- DisplayName: `원인 없는 물기`
- Description: `문 앞 타일과 문풍지 주변에만 차갑고 축축한 수분이 생긴다.`
- AnomalyTypeTag: `Anomaly.Type.WaterWithoutSource`
- EvidenceTagsGranted: `Evidence.Visual`, `Evidence.Environmental`
- ActivationTags: `Progression.Chapter.Prologue`
- PresentationTags: `None`
- ConsequenceTags: `None`

#### ANM_INTERCOM_VOICE

- DisplayName: `인터폰 잡음 속 음성`
- Description: `호출음 대신 생활 소리 파편이나 애매한 음성 리듬이 섞인다.`
- AnomalyTypeTag: `Anomaly.Type.IntercomVoice`
- EvidenceTagsGranted: `Evidence.Audio`, `Evidence.Visual`
- ActivationTags: `Progression.Chapter.One`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.IntercomStaticObserved`

#### ANM_MAILBOX_RECORD_CONFLICT

- DisplayName: `우편함/명부 불일치`
- Description: `우편함 이름표 배열, 명부, 세대 정보가 서로 정확히 맞지 않는다.`
- AnomalyTypeTag: `Anomaly.Type.MailboxMismatch`
- EvidenceTagsGranted: `Evidence.Document`, `Evidence.Records`
- ActivationTags: `Progression.Chapter.One`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.RecordMismatchNoted`

#### ANM_3F_PERSISTENT_EMERGENCY_LIGHT

- DisplayName: `상시 점등 비상등`
- Description: `정상 전원이 살아 있는데도 비상등 하나가 계속 켜진다.`
- AnomalyTypeTag: `Anomaly.Type.PersistentEmergencyLight`
- EvidenceTagsGranted: `Evidence.Visual`, `Evidence.Environmental`
- ActivationTags: `Progression.Chapter.One`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.ThreeFloorUnstable`

#### ANM_205_WATER_WITHOUT_SOURCE

- DisplayName: `냄새만 남은 습기`
- Description: `설비 누출은 없지만 문 앞 공기와 표면 상태만 눅눅하게 변한다.`
- AnomalyTypeTag: `Anomaly.Type.WaterWithoutSource`
- EvidenceTagsGranted: `Evidence.Environmental`, `Evidence.Visual`
- ActivationTags: `Progression.Chapter.One`
- PresentationTags: `None`
- ConsequenceTags: `None`

#### ANM_STAIR_REFLECTION_DELAY

- DisplayName: `계단실 반응 지연`
- Description: `센서등이나 반사 피드백이 플레이어보다 늦게 따라온다.`
- AnomalyTypeTag: `Anomaly.Type.ReflectionDelay`
- EvidenceTagsGranted: `Evidence.Visual`, `Evidence.Audio`
- ActivationTags: `Progression.Chapter.One`
- PresentationTags: `None`
- ConsequenceTags: `None`

#### ANM_302_TV_ON_IN_DARK

- DisplayName: `어두운 세대의 TV 잔광`
- Description: `문 아래 빛은 없지만 TV나 대화음 같은 생활 리듬이 남아 있다.`
- AnomalyTypeTag: `Anomaly.Type.TVOnInDark`
- EvidenceTagsGranted: `Evidence.Audio`, `Evidence.Environmental`
- ActivationTags: `Progression.Chapter.Two`
- PresentationTags: `None`
- ConsequenceTags: `None`

#### ANM_LIFE_NOISE_IN_VACANT_UNIT

- DisplayName: `빈 세대의 생활 소음`
- Description: `비어 있거나 불명확한 세대에서 현재 거주 중인 것 같은 생활 소리가 난다.`
- AnomalyTypeTag: `Anomaly.Type.LifeNoiseInVacantUnit`
- EvidenceTagsGranted: `Evidence.Audio`, `Evidence.Visual`
- ActivationTags: `Progression.Chapter.Two`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.Room307Clue`

#### ANM_204_NAMEPLATE_MISMATCH

- DisplayName: `문패 불일치`
- Description: `문패, 우편함, 명부 정보가 동시에 맞지 않는다.`
- AnomalyTypeTag: `Anomaly.Type.NameplateMismatch`
- EvidenceTagsGranted: `Evidence.Document`, `Evidence.Visual`
- ActivationTags: `Progression.Chapter.Two`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.RecordMismatchNoted`

#### ANM_RECORD_CONFLICT

- DisplayName: `기록 충돌`
- Description: `같은 세대나 사건을 가리키는 기록이 서로 다른 사실을 말한다.`
- AnomalyTypeTag: `Anomaly.Type.RecordConflict`
- EvidenceTagsGranted: `Evidence.Document`, `Evidence.Records`
- ActivationTags: `Progression.Chapter.Two`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.RecordMismatchNoted`

#### ANM_4F_CCTV_TIME_GAP

- DisplayName: `CCTV 시간 공백`
- Description: `현장 카메라는 살아 있으나 녹화 시간 로그나 블랭크 구간이 비정상적이다.`
- AnomalyTypeTag: `Anomaly.Type.CCTVTimeGap`
- EvidenceTagsGranted: `Evidence.Visual`, `Evidence.Records`
- ActivationTags: `Progression.Chapter.Two`
- PresentationTags: `None`
- ConsequenceTags: `None`

#### ANM_ELEVATOR_FLOOR_MISMATCH

- DisplayName: `엘리베이터 층수 불일치`
- Description: `표시층, 도착음, 실제 위치가 잠깐씩 어긋난다.`
- AnomalyTypeTag: `Anomaly.Type.ElevatorFloorMismatch`
- EvidenceTagsGranted: `Evidence.Visual`, `Evidence.Audio`
- ActivationTags: `Progression.Chapter.Two`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.ThreeFloorUnstable`

#### ANM_COMPLAINT_REPEATS

- DisplayName: `민원 재개`
- Description: `이미 해결된 민원이 기록상 다시 열린다.`
- AnomalyTypeTag: `Anomaly.Type.ComplaintRepeats`
- EvidenceTagsGranted: `Evidence.Document`, `Evidence.Records`
- ActivationTags: `Progression.Chapter.Two`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.ComplaintLoopObserved`

#### ANM_BSMT_PANEL_LABEL_CHANGED

- DisplayName: `지하 설비 라벨 변화`
- Description: `배전반과 경보 패널의 라벨 체계가 실제 공급 라인과 맞지 않는다.`
- AnomalyTypeTag: `Anomaly.Type.PanelLabelChanged`
- EvidenceTagsGranted: `Evidence.Document`, `Evidence.Environmental`
- ActivationTags: `Progression.Chapter.Three`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.BasementPowerLinked`

#### ANM_306_DOOR_SENSOR_MISMATCH

- DisplayName: `문 센서 상태 불일치`
- Description: `문은 잠겨 있는데 로그는 문이 열렸다고 기록한다.`
- AnomalyTypeTag: `Anomaly.Type.DoorSensorMismatch`
- EvidenceTagsGranted: `Evidence.Visual`, `Evidence.Records`
- ActivationTags: `Progression.Chapter.Three`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.Room307DoorReached`

#### ANM_HALLWAY_STRETCH

- DisplayName: `늘어난 복도`
- Description: `같은 복도인데 끝까지의 거리감과 기준점이 다르게 느껴진다.`
- AnomalyTypeTag: `Anomaly.Type.HallwayStretch`
- EvidenceTagsGranted: `Evidence.Visual`, `Evidence.Environmental`
- ActivationTags: `Progression.Chapter.Three`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.SpaceDepthShift`

#### ANM_REFLECTION_DELAY

- DisplayName: `반사 지연`
- Description: `거울, 유리, 도어락 표면 반사가 현재 위치와 미세하게 맞지 않는다.`
- AnomalyTypeTag: `Anomaly.Type.ReflectionDelay`
- EvidenceTagsGranted: `Evidence.Visual`
- ActivationTags: `Progression.Chapter.Three`
- PresentationTags: `None`
- ConsequenceTags: `None`

#### ANM_307_PACKAGE_AT_MISSING_UNIT

- DisplayName: `없는 세대 앞 택배`
- Description: `존재가 확정되지 않는 세대 앞에 정상적인 배송 흔적이 남는다.`
- AnomalyTypeTag: `Anomaly.Type.PackageAtMissingUnit`
- EvidenceTagsGranted: `Evidence.Document`, `Evidence.Visual`, `Evidence.Records`
- ActivationTags: `Progression.Chapter.Four`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.Room307PackageSeen`

#### ANM_307_LIFE_NOISE_IN_VACANT_UNIT

- DisplayName: `307호 생활 소음`
- Description: `307호는 응답이 없지만, 내부 생활 소음은 현재 진행형처럼 들린다.`
- AnomalyTypeTag: `Anomaly.Type.LifeNoiseInVacantUnit`
- EvidenceTagsGranted: `Evidence.Audio`, `Evidence.Visual`, `Evidence.Records`
- ActivationTags: `Progression.Chapter.Four`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.Room307LifeNoise`

#### ANM_307_RECORD_CONFLICT

- DisplayName: `307호 기록 충돌`
- Description: `입주자 명부, 정비 로그, 우편함, 민원 보드가 모두 307호를 다르게 가리킨다.`
- AnomalyTypeTag: `Anomaly.Type.RecordConflict`
- EvidenceTagsGranted: `Evidence.Document`, `Evidence.Records`, `Evidence.Visual`
- ActivationTags: `Progression.Chapter.Four`
- PresentationTags: `None`
- ConsequenceTags: `Progression.Story.Room307RecordConflict`

#### ANM_307_REFLECTION_DELAY

- DisplayName: `307호 반사 오류`
- Description: `307호 내부의 반사면과 거리감이 정상 세대의 예측과 어긋난다.`
- AnomalyTypeTag: `Anomaly.Type.ReflectionDelay`
- EvidenceTagsGranted: `Evidence.Visual`, `Evidence.Environmental`
- ActivationTags: `Progression.Chapter.Four`
- PresentationTags: `None`
- ConsequenceTags: `None`

### 5. Chapter Row Mapping

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

### 6. 구현 시 확인할 것

- 모든 `LocationId`는 실제 레벨 액터 또는 구역 id와 맞춰서 추후 통일한다.
- `CH_PRO`, `CH_01`, `CH_02`, `CH_03`, `CH_04`는 `NCSetChapter` 디버그 워크플로와 맞는 실제 챕터 id 자산/테이블 명명 규칙으로 통일한다.
- `Title`, `BoardSummary`, `InternalNote`는 이후 로컬라이징 대상이다.
- `CompletionTags`와 `RequiredProgressionTags`는 선형 해금이므로, 런타임 브리지 연결 전 디버그 치트로 deadlock 여부를 반드시 검증한다.
