# RealityCam 구현 해설서

## 문서 목적

이 문서는 `UNCRealityCameraComponent`가 어떤 구조로 설계되었고, 매 프레임 어떤 순서로 동작하는지 정리한 학습용 문서다.
`NightCaretaker` 프로젝트 안에서 구현된 버전을 기준으로 설명하며, 이후 `Sprint`, `Crouch`, `Landing`, `Breathing` 같은 상태를 추가할 때도 기준 문서로 사용할 수 있도록 작성한다.

대상 코드:
- `Source/NightCaretaker/Camera/NCRealityCameraComponent.h`
- `Source/NightCaretaker/Camera/NCRealityCameraComponent.cpp`
- `Source/NightCaretaker/Character/NCPlayerCharacter.cpp`

관련 로컬라이제이션 가이드는 `NightCaretaker_Localization_Guide.md`를 참조한다.

## 1. RealityCam의 역할

`UNCRealityCameraComponent`는 단순히 카메라를 붙여두는 컴포넌트가 아니라, 매 프레임 최종 카메라 뷰를 계산하는 "뷰 합성기"다.

핵심 개념은 아래와 같다.

1. 기본 시선은 플레이어 컨트롤러 회전을 그대로 사용한다.
2. 그 위에 아주 작은 현실감 레이어를 여러 개 더한다.
3. 각 레이어는 속도, 방향, 회전 변화량 같은 런타임 데이터를 입력으로 사용한다.
4. 마지막에 위치, 회전, FOV, 포스트 프로세스를 합산해 최종 POV를 만든다.

즉, 이 시스템은 "카메라를 흔드는 시스템"이라기보다 "카메라 결과를 절제된 방식으로 후처리하는 시스템"에 가깝다.

## 2. 왜 UCameraComponent를 상속했는가

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

## 3. 헤더 구조 설명

### 3.1 `FNCRealityCameraTuning`

이 구조체는 "디자이너가 만질 수 있는 설정값 묶음"이다.
실행 중 변화하는 값이 아니라, 카메라 성격을 정의하는 값이다.

구성은 크게 여섯 영역이다.

- 기본값
  - `RealityCamIntensity`: 전체 RealityCam 효과 강도
  - `BaseOffset`: 캡슐 기준 카메라 기본 위치
  - `BaseFOV`: 정지 상태 기본 FOV
  - `MoveFOVBoostMax`: 최대 이동 시 추가 FOV
- Idle
  - `IdleNoiseLocationAmplitude`
  - `IdleNoiseRotationAmplitude`
  - `IdleNoiseFrequency`
- Locomotion
  - `WalkBobLocationAmplitude`
  - `WalkBobRotationAmplitude`
  - `WalkBobFrequency`
  - `StrafeSwayLocationAmplitude`
  - `StrafeSwayYawAmplitude`
  - `MaxRollDegrees`
- Rotation
  - `TurnYawLagDegrees`
  - `TurnPitchLagDegrees`
  - `TurnLagSpeed`
- Impulse
  - `StartStopLocationAmplitude`
  - `StartStopPitchAmplitude`
  - `StartStopResponseSpeed`
  - `StartStopRecoverySpeed`
- Interaction/Post Process
  - `InteractionDampingFactor`
  - `PrecisionModeInterpSpeed`
  - `bEnablePostProcess`
  - `PostProcessBlendWeight`
  - `ChromaticAberrationIntensity`
  - `VignetteIntensity`

핵심 포인트는 "하나의 큰 수식"이 아니라, 목적이 다른 작은 레이어들의 강도를 각각 조절할 수 있게 나눠둔 점이다.

### 3.2 `UNCRealityCameraComponent`의 런타임 상태값

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

## 4. 플레이어에 어떻게 연결되는가

`ANCPlayerCharacter` 생성자에서 `RealityCameraComponent`를 만든다.

동작 순서:
1. 플레이어 캡슐을 세팅한다.
2. 이동 컴포넌트 속도를 설정한다.
3. `UNCRealityCameraComponent`를 생성해 캡슐에 부착한다.
4. `ApplyRuntimeTuning()`으로 기본 오프셋/FOV/포스트 프로세스를 적용한다.
5. `BaseEyeHeight`를 `BaseOffset.Z`에 맞춘다.
6. 메시와 그림자를 숨긴다.

이 구조 덕분에 현재 카메라는 "몸이 보이는 1인칭"이 아니라 "캡슐 중심의 카메라 전용 1인칭"으로 동작한다.

## 5. 프레임별 동작 순서

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

## 6. 각 레이어가 어떻게 계산되는가

### 6.1 Base View

`Super::GetCameraView()`가 먼저 실행된다.
이 결과는 "캡슐에 붙은 기본 카메라 + 컨트롤러 회전"이다.
RealityCam은 이 값을 완전히 대체하지 않고, 그 위에 오프셋을 더한다.

### 6.2 이동 데이터 읽기

컴포넌트는 오너 캐릭터의 `Velocity`를 읽고, 수직 성분을 제거한 평면 속도만 사용한다.
그 다음 속도를 `MaxWalkSpeed`로 나눠 `NormalizedSpeed`를 만든다.

### 6.3 로컬 공간 분해

현재 카메라가 향하는 yaw 기준으로 forward/right 축을 만든 뒤, 속도를 dot product로 분해한다.

이 결과:
- `ForwardAlpha`: 전후 이동량
- `RightAlpha`: 좌우 이동량

### 6.4 스무딩

`FInterpTo`를 사용해 이동 성분과 속도 비율을 부드럽게 만든다.
이 단계 덕분에 입력이 디지털이어도 카메라가 튀지 않는다.

### 6.5 Idle Noise

`NoiseTime`을 누적하면서 `FMath::PerlinNoise1D()`를 사용해 아주 작은 랜덤성 있는 흔들림을 만든다.
이 레이어는 "정적이지 않은 손-held 느낌"을 담당한다.

### 6.6 Walk Bob

걷기 흔들림은 리듬이 있어야 하므로 `sin`을 사용한다.
속도가 클수록 진폭이 커지고, 수직 위치와 pitch가 함께 움직인다.

### 6.7 Strafe Sway

좌우 이동 시 카메라를 살짝 반대로 밀고, yaw와 roll도 같이 준다.
이 레이어가 없으면 횡이동이 미끄러지듯 느껴질 수 있다.

### 6.8 Turn Inertia

이전 프레임 회전과 현재 회전의 차이를 이용해 회전 속도를 계산하고, 그 속도에 비례하는 yaw/pitch offset을 만든다.
빠르게 홱 돌릴 때만 약한 관성이 생기고, 천천히 볼 때는 거의 느껴지지 않는다.

### 6.9 Start/Stop Impulse

속도 변화량 기반으로 출발과 정지 순간의 짧은 밀림을 만든다.
출발할 때는 살짝 뒤로 끌리고, 정지할 때는 살짝 앞으로 쏠리는 느낌을 준다.

## 7. 최종 합성 방식

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

## 8. Precision Interaction 모드

이 시스템에는 정밀 상호작용용 감쇠 기능이 이미 들어 있다.

- `SetPrecisionInteractionEnabled(bool)`가 목표 상태를 켠다.
- `GetEffectiveIntensity()`에서 `PrecisionModeBlend`를 0~1로 천천히 보간한다.
- `InteractionDampingFactor`를 이용해 최종 강도를 줄인다.

즉 문 읽기, 스위치 조작, 잠금 해제 같은 상황에서 흔들림을 줄일 준비가 되어 있다.

## 9. 이 구조의 장점

- 구현이 단순하다.
- 카메라 효과가 레이어별로 분리되어 있어 튜닝이 쉽다.
- 이동, 회전, 시작/정지 같은 체감 요소를 독립적으로 조절할 수 있다.
- 메쉬가 없어도 일정 수준의 현실감을 만들 수 있다.
- 강도를 0으로 낮추면 거의 기본 카메라처럼 복귀할 수 있다.

## 10. 현재 구조의 한계

- `Sprint`, `Crouch`, `Landing`, `Breathing` 전용 레이어가 아직 없다.
- 착지 충격은 아직 구현되지 않았다.
- 시작/정지 impulse는 실제 가속도가 아니라 속도 변화량 기반 근사다.
- 상호작용 감쇠는 API만 있고 아직 상호작용 시스템과 연결되지 않았다.
- 카메라 모드가 많아지면 나중에 `PlayerCameraManager` 계층으로 옮길 가능성이 있다.

## 11. 의사코드

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
        walkCycleTime += deltaTime * WalkBobFrequency * speedScale

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

## 12. 확장 우선순위

추천 순서:
1. 상호작용 시스템과 `SetPrecisionInteractionEnabled()` 연결
2. `Sprint` 전용 bob/FOV 확장
3. `Landing` 충격 레이어 추가
4. `Breathing` 또는 긴장도 기반 미세 흔들림 연동
5. `Crouch` 높이 전환과 카메라 오프셋 보간
6. 손전등 흔들림을 RealityCam 계산값과 연동
