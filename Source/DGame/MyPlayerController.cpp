#include "MyPlayerController.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MyCharacter.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Engine/World.h"
#include "Engine.h"

AMyPlayerController::AMyPlayerController() {
	// ��ʼ�����
	bShowMouseCursor = true;							// ��ʾ���ͼ��
	//DefaultMouseCursor = EMouseCursor::Crosshairs;	// ����ͼ����ʽ

	// ��ʼ���ܲ�
	b_run = false;

	// ��ʼ������״̬
	b_attack_q = false;

	// ��ʼ���ӽ��ƶ���ز���
	b_can_rotate = false;
	turn_rate = 45.f;
	lookup_rate = 45.f;

	// ��ʼ���ӽ����Ų���
	scale_rate = 100.f;
	scale_min = 200.f;
	scale_max = 5000.f;
}

void AMyPlayerController::BeginPlay() {
	Super::BeginPlay();
	// ��ʼ����ת�ӽ�
	SetControlRotation(FRotator(-60.0f, 0.0f, 0.0f));

	// ��ʼ������ģʽ
	controller_state = EControllerState::E_FREE;
	update_controller_state();

	// ��ʼ��ս��״̬
	fight_state = false;
	update_fight_state();
}

void AMyPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	if(b_move_to_cursor) { move_to_cursor(); }			// �����Ǵ�, ������ƶ�
}

void AMyPlayerController::SetupInputComponent() {
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// ������ƶ��¼�
	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AMyPlayerController::on_set_destination_pressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AMyPlayerController::on_set_destination_released);
	// ֧�ִ���
	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AMyPlayerController::on_reset_VR);
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AMyPlayerController::move_to_touch_location);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AMyPlayerController::move_to_touch_location);

	// wasd�ƶ��¼�
	InputComponent->BindAxis("MoveForward", this, &AMyPlayerController::on_move_forward);
	InputComponent->BindAxis("MoveRight", this, &AMyPlayerController::on_move_right);
	// run�¼�
	InputComponent->BindAction("Run", IE_Pressed, this, &AMyPlayerController::on_run_start);
	InputComponent->BindAction("Run", IE_Released, this, &AMyPlayerController::on_run_stop);
	// �����ͷ��¼�
	InputComponent->BindAction("Attack_Q", IE_Pressed, this, &AMyPlayerController::on_attack_q_pressed);
	InputComponent->BindAction("Attack_Q", IE_Released, this, &AMyPlayerController::on_attack_q_released);

	// �ӽ��ƶ��¼�
	InputComponent->BindAction("CanRotate", IE_Pressed, this, &AMyPlayerController::on_can_rotate_pressed);
	InputComponent->BindAction("CanRotate", IE_Released, this, &AMyPlayerController::on_can_rotate_released);
	InputComponent->BindAxis("TurnRate", this, &AMyPlayerController::on_turn_rate);
	InputComponent->BindAxis("LookupRate", this, &AMyPlayerController::on_lookup_rate);

	// �ӽ������¼�
	InputComponent->BindAxis("Scale", this, &AMyPlayerController::on_scale);
}

void AMyPlayerController::move_to_cursor() {
	if(UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled()) {
		if(AMyCharacter* my_pawn = Cast<AMyCharacter>(GetPawn())) {
			if(my_pawn->get_cursor_world()) UNavigationSystem::SimpleMoveToLocation(this, my_pawn->get_cursor_world()->GetComponentLocation());
		}
	}
	else {
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		if(Hit.bBlockingHit) set_new_move_destination(Hit.ImpactPoint);
	}
}
void AMyPlayerController::set_new_move_destination(const FVector DestLocation) {
	APawn* const my_pawn = GetPawn();
	if(my_pawn) {
		UNavigationSystem* const nav_sys = GetWorld()->GetNavigationSystem();
		float const distance = FVector::Dist(DestLocation, my_pawn->GetActorLocation());
		if(nav_sys && (distance > 120.0f)) nav_sys->SimpleMoveToLocation(this, DestLocation);
	}
}

// �¼����
void AMyPlayerController::on_set_destination_pressed() {
	b_move_to_cursor = true;
	AMyCharacter* my_pawn = (AMyCharacter*) GetPawn();		// Danger
	my_pawn->cursor_show(true);
}
void AMyPlayerController::on_set_destination_released() {
	b_move_to_cursor = false;
	AMyCharacter* my_pawn = (AMyCharacter*) GetPawn();		// Danger
	my_pawn->cursor_show(false);
}
void AMyPlayerController::on_reset_VR() { UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition(); }
void AMyPlayerController::move_to_touch_location(const ETouchIndex::Type FingerIndex, const FVector Location) {
	FVector2D ScreenSpaceLocation(Location);

	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if(HitResult.bBlockingHit) set_new_move_destination(HitResult.ImpactPoint);
}

void AMyPlayerController::on_move_forward(float value) {
	if(value != 0.0f) {
		// �����ת�Ƕ�
		const FRotator rotation = GetControlRotation();
		const FRotator yaw_rotation(0, rotation.Yaw, 0);
		// ������ת����ǰ����
		FVector direction, y, z;
		FRotationMatrix(yaw_rotation).GetUnitAxes(direction, y, z);
		// ��ǰ��������ǰ��
		APawn* const my_pawn = GetPawn();
		if(my_pawn) my_pawn->AddMovementInput(direction, value);
	}
}
void AMyPlayerController::on_move_right(float value) {
	if(value != 0.0f) {
		// �����ת�Ƕ�
		const FRotator rotation = GetControlRotation();
		const FRotator yaw_rotaion(0, rotation.Yaw, 0);
		// ������ת����������
		FVector direction, x, z;
		FRotationMatrix(yaw_rotaion).GetUnitAxes(x, direction, z);
		// ������������ǰ��
		APawn* my_pawn = GetPawn();
		if(my_pawn) my_pawn->AddMovementInput(direction, value);
	}
}
// run�¼�
void AMyPlayerController::on_run_start() { b_run = true; }
void AMyPlayerController::on_run_stop() { b_run = false; }
// �����¼�
void AMyPlayerController::on_attack_q_pressed() { b_attack_q = true; }
void AMyPlayerController::on_attack_q_released() { b_attack_q = false; }

// �ӽ��ƶ��¼�
void AMyPlayerController::on_can_rotate_pressed() {
	b_can_rotate = true;
	//bShowMouseCursor = false;
	FInputModeGameOnly input;
	SetInputMode(input);
}
void AMyPlayerController::on_can_rotate_released() {
	b_can_rotate = false;
	//bShowMouseCursor = true;
	FInputModeGameAndUI input;
	input.SetHideCursorDuringCapture(true);
	SetInputMode(input);
}
void AMyPlayerController::on_turn_rate(float value) {
	if(!b_can_rotate) return;
	APawn* my_pawn = GetPawn();
	if(my_pawn) my_pawn->AddControllerYawInput(value * turn_rate * GetWorld()->GetDeltaSeconds());
}
void AMyPlayerController::on_lookup_rate(float value) {
	if(!b_can_rotate) return;
	APawn* my_pawn = GetPawn();
	if(my_pawn) my_pawn->AddControllerPitchInput(value * lookup_rate * GetWorld()->GetDeltaSeconds());
}

// �ӽ������¼�
void AMyPlayerController::on_scale(float value) {
	AMyCharacter* my_pawn = (AMyCharacter*) GetPawn();
	my_pawn->get_camera_boom()->TargetArmLength += value * scale_rate;
	my_pawn->get_camera_boom()->TargetArmLength = FMath::Clamp(my_pawn->get_camera_boom()->TargetArmLength, scale_min, scale_max);
}

// ���¿���״̬ -- ���ݵ�ǰ״̬
void AMyPlayerController::update_controller_state() {
	APawn * my_pawn = GetPawn();
	switch(controller_state) {
		case EControllerState::E_FREE:
			if(my_pawn) {
				my_pawn->bUseControllerRotationYaw = false;
			}
			break;
		case EControllerState::E_SOLID:
			break;
		case EControllerState::E_THREED:
			if(my_pawn) {
				my_pawn->bUseControllerRotationYaw = true;
			}
			break;
		case EControllerState::E_ACT:
			break;
	}
}

void AMyPlayerController::update_fight_state() {
	AMyCharacter* my_pawn = Cast<AMyCharacter>(GetPawn());
	if(!my_pawn) return;
	if(fight_state) {
		my_pawn->change_sword_place(FName("Sword_1"));   // ��������λ��
		my_pawn->change_shield_place(FName("Shield_1")); // ���ö���λ��
	}
	else {
		my_pawn->change_sword_place(FName("Sword_2"));   // ��������λ��
		my_pawn->change_shield_place(FName("Shield_2")); // ���ö���λ��
	}
}