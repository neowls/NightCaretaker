# NightCaretaker 물리 상호작용 구현 가이드

## 문서 정보

- 문서 종류: 구현 가이드
- 작성일: 2026-03-23
- 대상 구현: `UNCPropInteractorComponent`, `UNCPhysicsCarryTargetComponent`, `ANCDoorActor`, `ANCPlayerCharacter`
- 기준 프로젝트: `NightCaretaker`, Unreal Engine 5.7
- 목적: 현재 프로젝트에 구현된 물리 프랍 홀드와 물리 문 상호작용의 구조, 기본값, 저작 절차, 디버그 방법을 빠짐없이 정리한다.

## 1. 현재 구현 범위 요약

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

## 2. 플레이어 소유 구조

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

## 3. Grab 입력 라우팅 순서

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

## 4. 물리 프랍 홀드 시스템

### 4.1 책임 클래스

- `UNCPropInteractorComponent`
  플레이어가 카메라 중심으로 프랍을 탐색하고, 물리 핸들로 잡고, 유지하고, 놓는 흐름 전체를 담당한다.
- `UNCPhysicsCarryTargetComponent`
  액터가 프랍 홀드 대상임을 표시하고, 실제로 어떤 `UPrimitiveComponent`를 잡을지 결정한다.

### 4.2 대상 액터에 필요한 조건

프랍을 잡히게 하려면 아래 조건을 만족해야 한다.

1. 액터에 `UNCPhysicsCarryTargetComponent`가 붙어 있어야 한다.
2. 잡힐 `UPrimitiveComponent`가 존재해야 한다.
3. 그 컴포넌트는 `Simulate Physics = true`여야 한다.
4. 그 컴포넌트는 `Visibility` 라인트레이스를 막아야 한다.
5. 질량이 최대 허용 질량 이하여야 한다.

여러 프리미티브가 있는 액터는 `GrabPrimitiveOverride`를 지정하는 편이 안전하다.

### 4.3 `ResolveGrabPrimitive()` 규칙

`UNCPhysicsCarryTargetComponent::ResolveGrabPrimitive()`는 아래 순서로 동작한다.

1. `GrabPrimitiveOverride`가 유효한 컴포넌트를 가리키는지 확인한다.
2. 그 컴포넌트가 `UPrimitiveComponent`이고 `IsSimulatingPhysics() == true`이면 즉시 반환한다.
3. override가 없거나 유효하지 않으면, 오너 액터의 모든 `UPrimitiveComponent`를 순회한다.
4. 그 중 첫 번째로 `IsSimulatingPhysics() == true`인 컴포넌트를 반환한다.
5. 끝까지 없으면 `nullptr`를 반환한다.

즉, override가 있어도 물리 시뮬레이션이 꺼져 있으면 무시되고, 자동 탐색으로 넘어간다.

### 4.4 라인트레이스와 대상 검증

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

### 4.5 기본 튜닝 값

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

### 4.6 무게 기반 반응 스케일

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

### 4.7 프랍 grab 시작과 유지

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

### 4.8 프랍 홀드 유지와 자동 릴리즈

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

### 4.9 프랍 회전 규칙

프랍은 잡는 순간의 카메라 상대 회전을 저장한 뒤, 이후에도 그 상대 회전을 유지한다.

즉, 문과 달리 프랍은 아래 방식이다.

- 위치: 카메라 전방 고정 거리
- 회전: 카메라 회전에 종속되는 상대 자세 유지

### 4.10 프랍 디버그 시각화

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

## 5. 물리 문 시스템

### 5.1 책임 클래스

문은 일반 프랍과 달리 `ANCDoorActor` 하나에 상태기와 물리 제약이 함께 들어 있다.

현재 문 액터는 다음 책임을 가진다.

- 닫힘 기준 포즈 캐시
- 힌지 constraint 설정
- 잠금/래치/자유 스윙/직접 드래그 상태 관리
- direct grab의 목표 각도 계산
- 자동 래치 판정
- 잠금 실패 이벤트 제공

### 5.2 문 액터 기본 컴포넌트 구조

`ANCDoorActor`는 기본적으로 아래 컴포넌트를 만든다.

- `DoorFrameComponent`: 루트. 문틀 또는 앵커.
- `DoorLeafComponent`: 실제 문짝.
- `DoorHingeConstraintComponent`: 힌지 제약.

의도는 아래와 같다.

- `DoorFrame`은 움직이지 않는다.
- `DoorLeaf`만 움직인다.
- `DoorHingeConstraint`가 `DoorLeaf`의 이동을 잠그고 회전만 허용한다.

### 5.3 문 배치 규칙

디자이너가 문을 배치할 때 지켜야 하는 규칙은 아래와 같다.

1. `DoorFrame`에는 고정된 문틀 메시 또는 보이지 않는 앵커 메시를 넣는다.
2. `DoorLeaf`에는 실제로 회전할 문짝 메시만 넣는다.
3. 프레임과 문짝이 합쳐진 단일 메시를 `DoorLeaf`에 넣지 않는다.
4. 문은 닫힌 기준 포즈로 배치한다.
5. `HingeLocalOffset`은 실제 경첩 축 위치에 최대한 맞춘다.
6. `DoorLeaf`는 `Visibility` 라인트레이스를 막는 편이 좋다.
7. `DoorLeaf`에는 적절한 simple collision이 있는 편이 안정적이다.

### 5.4 문 상태기

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

### 5.5 문 상태 전환 규칙

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

### 5.6 문 기본값

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

### 5.7 힌지 constraint 설정 방식

`RefreshConstraintSetup()`은 다음을 고정한다.

- 힌지 위치를 `DoorFrameTransform.TransformPosition(HingeLocalOffset)`에 둔다.
- `DoorFrame`과 `DoorLeaf`를 constraint로 연결한다.
- 모든 linear movement를 잠근다.
- `Swing1`, `Swing2`는 잠근다.
- 실제 회전 자유도는 `Twist` 하나만 사용한다.
- `Twist` 제한값은 `max(abs(MinOpenAngleDegrees), abs(MaxOpenAngleDegrees))`를 `0~170` 범위로 clamp한 대칭 제한이다.

즉, 현재 문은 저작상 비대칭 개방 각도를 입력할 수 있어도 물리 힌지는 대칭 제한으로 동작한다. 이는 현재 구현의 한계이자 안정화 선택이다.

### 5.8 닫힘 포즈 캐시와 스냅

`CacheClosedPose()`는 레벨에 배치된 현재 문짝 포즈를 `DoorFrame` 기준 상대 트랜스폼으로 저장한다.

그 후 `ApplyClosedState()`가 호출되면 아래가 일어난다.

1. 활성 grab을 해제한다.
2. 현재 물리 속도를 0으로 만든다.
3. `DoorLeaf` 물리 시뮬레이션을 끈다.
4. 캐시된 닫힘 월드 트랜스폼으로 문을 순간 이동시킨다.
5. 상태를 `LatchedClosed` 또는 `LockedClosed`로 바꾼다.

즉, 문이 다시 닫히면 단순히 각도만 0에 가까워지는 것이 아니라, 닫힌 기준 포즈에 정확히 스냅된다.

### 5.9 문 unlatch와 자유 스윙

`TransitionToFreeSwing()`은 아래만 담당한다.

1. 잠긴 문이면 아무 것도 하지 않는다.
2. 이미 `FreeSwing` 또는 `GrabDragging`이면 아무 것도 하지 않는다.
3. 문짝을 닫힘 기준 포즈에 맞춘다.
4. `DoorLeaf`의 물리 시뮬레이션을 켠다.
5. rigid body를 깨운다.
6. 상태를 `FreeSwing`으로 바꾼다.

이 상태에 들어오면 몸으로 밀어 문을 열 수 있다.

### 5.10 문 direct grab 방식

현재 문 direct grab은 문을 카메라 앞으로 끌지 않는다. 대신 아래 방식으로 움직인다.

- `Physics Handle`이 문짝을 잡고는 있다.
- 목표 위치는 항상 `DoorLeaf`의 현재 위치다.
- 목표 회전만 매 프레임 갱신한다.
- 회전 목표는 grab 시작 시 맞춘 문 표면 지점을 기준으로 계산한다.

이 방식의 목적은 다음과 같다.

- 문이 힌지에서 뜯겨 나가려는 선형 흔들림을 줄인다.
- 플레이어가 실제로 잡은 문 표면 지점이 손을 따라간다는 감각을 만든다.
- 카메라 회전값만으로 문이 돌아가는 부자연스러운 느낌을 줄인다.

### 5.11 문 grab 시작 시 캐시하는 값

`BeginDoorGrab()` 성공 직전에 아래 값을 저장한다.

- `GrabPointLocalToDoorLeaf`
  - 히트한 문 표면 지점을 `DoorLeaf` 로컬 공간으로 변환한 값
- `GrabPointCameraLocalOffset`
  - 같은 히트 지점을 현재 카메라 로컬 공간으로 변환한 값
- `LastStableTargetAngleDegrees`
  - grab 시작 순간의 문 개방 각도

여기서 grab 위치는 `Hit.ImpactPoint`를 우선 사용하고, 히트 컴포넌트가 정확히 `DoorLeaf`가 아니면 문짝 중심을 사용한다.

### 5.12 문 grab 업데이트 수식

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

### 5.13 signed angle 계산 방식

문이 실제로 도는 각도는 `GetSignedAngleOnHingePlane()`에서 계산한다.

방식은 다음과 같다.

1. `FromWorld - HingeWorldLocation` 벡터를 힌지 평면에 투영한다.
2. `ToWorld - HingeWorldLocation` 벡터를 힌지 평면에 투영한다.
3. 두 벡터를 정규화한다.
4. `cross`와 `dot`를 이용해 `atan2` signed angle을 계산한다.
5. 라디안을 도 단위로 바꿔 반환한다.

이 방식 덕분에 단순 yaw 차이를 쓰는 것보다, 실제 힌지 위치와 grab 위치를 기준으로 한 회전량을 얻을 수 있다.

### 5.14 수치 안정화 장치

grab 지점이 힌지에 너무 가까우면 문 회전 계산이 불안정해진다. 이를 막기 위해 현재 구현은 아래 안전장치를 둔다.

- 힌지 평면에 투영한 현재/목표 벡터 길이가 `GrabMinimumProjectedDistance`보다 작으면 새 각도를 계산하지 않는다.
- 대신 `LastStableTargetAngleDegrees`를 그대로 사용한다.

즉, 힌지 바로 옆을 잡았을 때 급격한 각도 튐을 억제한다.

### 5.15 자동 래치 조건

`ShouldAutoLatch()`는 현재 아래 조건을 모두 만족할 때만 true다.

1. 상태가 `FreeSwing`일 것
2. 절대 개방 각도 `<= ClosedAngleThresholdDegrees`
3. 각속도 `<= LatchAngularVelocityThresholdDegPerSec`

여기서 각속도는 `DoorLeaf`의 물리 각속도를 `DoorFrame`의 up vector에 투영한 값으로 계산한다.

### 5.16 문 이벤트

현재 문 액터는 아래 Blueprint 이벤트를 노출한다.

- `OnDoorLatched`
- `OnDoorUnlocked`
- `OnLockedInteractionAttempted`

현재 잠금 해제 로직은 별도 키 시스템과 연결되어 있지 않고, 외부 C++ 또는 Blueprint가 `SetDoorLocked(bool)`를 호출해 제어하는 구조다.

## 6. RealityCam 연동

프랍과 문은 모두 RealityCam 정밀 감쇠를 사용한다.

- 프랍 grab 성공 시 `SetPrecisionInteractionEnabled(true)`
- 프랍 grab 종료 시 `false`
- 문 grab 성공 시 `true`
- 문 grab 종료 또는 door state 동기화 후 필요 없으면 `false`

플레이어는 `RefreshPrecisionInteractionState()`에서 아래 기준으로 최종 상태를 계산한다.

- 현재 잡고 있는 문이 있는가
- 현재 프랍을 들고 있는가

둘 중 하나라도 true면 RealityCam 정밀 감쇠를 유지한다.

## 7. 현재 알려진 제한사항

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

## 8. 디자이너 체크리스트

### 8.1 잡을 수 있는 프랍 액터

- `UNCPhysicsCarryTargetComponent`를 붙였는가
- 대상 `UPrimitiveComponent`가 `Simulate Physics = true`인가
- `Visibility` 트레이스를 막는가
- 질량이 `120kg` 이내인가
- 여러 프리미티브가 있다면 `GrabPrimitiveOverride`를 지정했는가

### 8.2 문 액터

- `DoorFrame`에는 문틀 또는 고정 앵커 메시를 넣었는가
- `DoorLeaf`에는 움직이는 문짝 메시만 넣었는가
- 문이 닫힌 기준 포즈로 배치되어 있는가
- `HingeLocalOffset`이 실제 경첩 위치와 맞는가
- `MinOpenAngleDegrees`, `MaxOpenAngleDegrees`가 의도한 문 개방 범위와 비슷한가
- `DoorLeaf` collision이 너무 복잡하지 않은가
- 몸으로 밀 수 있는 상태와 래치 상태가 의도대로 나오는가

## 9. 빠른 검증 절차

### 9.1 프랍

1. 테스트 프랍 액터에 `UNCPhysicsCarryTargetComponent`를 붙인다.
2. 물리 시뮬레이션과 `Visibility` 충돌을 켠다.
3. `nc.PropInteraction.Debug 1`을 입력한다.
4. 라인트레이스 라벨이 `Grabbable`인지 본다.
5. `IA_Grab`으로 잡는다.
6. 무거운 프랍일수록 더 느리게 따라오는지 확인한다.
7. 목표 지점과 너무 멀어지면 자동으로 놓이는지 확인한다.

### 9.2 문

1. `ANCDoorActor`를 배치한다.
2. `DoorFrame`과 `DoorLeaf` 메시를 지정한다.
3. 문을 닫힌 기준 포즈로 맞춘다.
4. `LatchedClosed` 상태에서 몸으로 밀어도 안 열리는지 확인한다.
5. `IA_Grab`으로 직접 잡아 열 수 있는지 확인한다.
6. grab 상태에서 좌우 이동과 앞뒤 이동 모두 문 회전에 반영되는지 확인한다.
7. 거의 닫힌 위치에서 놓으면 다시 래치되는지 확인한다.
8. `LockedClosed`로 바꿨을 때 어떤 방식으로도 열리지 않는지 확인한다.

## 10. 후속 확장 추천

현재 구조를 유지한 채 다음 확장이 가능하다.

1. 문 `IA_Use` 입력 추가
2. 프랍 throw 기능
3. 프랍 rotation adjust 기능
4. 문 전용 디버그 draw
5. 문 손잡이 컴포넌트 기반 세밀한 grab 포인트 정책
6. 비대칭 스윙 제한 지원
7. 잠금 해제용 키/퍼즐 시스템 연동
8. 저장/로드 시 문 상태와 프랍 위치 저장 정책 정리


