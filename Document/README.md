# NightCaretaker Document Hub

## 목적

문서 수를 줄이고 탐색 비용을 낮추기 위해, 현재 활성 문서는 분야별 총합본 3개만 전면에 둔다.
기존 상세 문서는 삭제하지 않고 `Archive` 아래에 원본으로 보관한다.

## 현재 활성 문서

- 기획 총합본: [NightCaretaker_Planning_Master.md](./NightCaretaker_Planning_Master.md)
- 개발 총합본: [NightCaretaker_Development_Master.md](./NightCaretaker_Development_Master.md)
- 아트 총합본: [NightCaretaker_Art_Master.md](./NightCaretaker_Art_Master.md)

## 총합본 구성 원칙

- 각 총합본은 해당 분류의 기존 상세 문서를 원문 기준으로 모두 병합한 문서다.
- 문서 내부 충돌을 피하기 위해 헤딩 레벨만 한 단계 내렸다.
- 상세 원문은 `Archive` 아래에 그대로 남겨 둔다.
- 세부 원문 안의 일부 경로 표기는 작성 당시 기준일 수 있으므로, 현재 원본 위치는 `Archive`를 기준으로 본다.

## 폴더 구조

| 경로 | 역할 |
| --- | --- |
| `Document/` | 현재 읽는 활성 문서 영역 |
| `Document/Archive/Planning` | 기획 상세 원본 보관 |
| `Document/Archive/Development` | 개발 상세 원본 보관 |
| `Document/Archive/Art` | 아트 상세 원본 보관 |
| `Document/Source` | 작업 기록 및 문서 정리 이력 |

## 읽기 원칙

- 제품 방향과 전체 기획은 기획 총합본부터 읽는다.
- 구현 구조와 데이터 파이프라인은 개발 총합본을 읽는다.
- 비주얼 방향과 제작 우선순위는 아트 총합본을 읽는다.
- 원문 단위로 추적이 필요하면 `Archive`에서 해당 파일을 연다.

## 유지보수 규칙

- 새 상세 문서를 만들기 전에 먼저 해당 총합본의 새 섹션으로 흡수 가능한지 검토한다.
- 상세 원본을 수정해 내용이 바뀌면 총합본도 같은 날 다시 병합해야 한다.
- `Document/Source` 아래 문서는 제품 기준서가 아니라 작업 기록으로 유지한다.
