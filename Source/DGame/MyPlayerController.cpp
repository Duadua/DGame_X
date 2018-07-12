#include "MyPlayerController.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MyCharacter.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Engine/World.h"
#include "Engine.h"
#include "Kismet/KismetMathLibrary.h"

AMyPlayerController::AMyPlayerController() {
	// 初始化鼠标
	bShowMouseCursor = true;							// 显示鼠标图标
	//DefaultMouseCursor = EMouseCursor::Crosshairs;	// 设置图标样式

	// 初始化技能状态
	b_attack_q = false;
	b_attack_w = false;
	b_attack_e = false;
	b_attack_r = false;


	// 初始化视角移动相关参数
	b_can_rotate = false;
	turn_rate = 45.f;
	lookup_rate = 45.f;

	// 初始化视角缩放参数
	scale_rate = 100.f;
	scale_min = 200.f;
	scale_max = 5000.f;
}

void AMyPlayerController::BeginPlay() {
	Super::BeginPlay();
	init();

}
void AMyPlayerController::init() {
	// 初始化鼠标
	bShowMouseCursor = true;							// 显示鼠标图标
	//DefaultMouseCursor = EMouseCursor::Crosshairs;	// 设置图标样式

	// 初始化技能状态
	b_attack_q = false;
	b_attack_w = false;
	b_attack_e = false;
	b_attack_r = false;


	// 初始化视角移动相关参数
	b_can_rotate = false;
	turn_rate = 45.f;
	lookup_rate = 45.f;

	// 初始化视角缩放参数
	scale_rate = 100.f;
	scale_min = 200.f;
	scale_max = 5000.f;
	

	// 初始化控制模式
	controller_state = EControllerState::E_FREE;
	update_controller_state();

	// 初始化战斗状态
	fight_state = false;
	update_fight_state();

	// 初始化旋转视角
	SetControlRotation(FRotator(-45.0f, 0.0f, 0.0f));
}

void AMyPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	if(b_move_to_cursor) { move_to_cursor(); }			// 如果标记打开, 则进行移动
}

void AMyPlayerController::SetupInputComponent() {
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// 鼠标点击移动事件
	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AMyPlayerController::on_set_destination_pressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AMyPlayerController::on_set_destination_released);

	// wasd移动事件
	InputComponent->BindAxis("MoveForward", this, &AMyPlayerController::on_move_forward);
	InputComponent->BindAxis("MoveRight", this, &AMyPlayerController::on_move_right);
	// run事件
	InputComponent->BindAction("Run", IE_Pressed, this, &AMyPlayerController::on_run_start);
	InputComponent->BindAction("Run", IE_Released, this, &AMyPlayerController::on_run_stop);
	// 技能释放事件
	InputComponent->BindAction("Attack_Q", IE_Pressed, this, &AMyPlayerController::on_attack_q_pressed);
	InputComponent->BindAction("Attack_Q", IE_Released, this, &AMyPlayerController::on_attack_q_released);
	InputComponent->BindAction("Attack_W", IE_Pressed, this, &AMyPlayerController::on_attack_w_pressed);
	InputComponent->BindAction("Attack_W", IE_Released, this, &AMyPlayerController::on_attack_w_released);
	InputComponent->BindAction("Attack_E", IE_Pressed, this, &AMyPlayerController::on_attack_e_pressed);
	InputComponent->BindAction("Attack_E", IE_Released, this, &AMyPlayerController::on_attack_e_released);
	InputComponent->BindAction("Attack_R", IE_Pressed, this, &AMyPlayerController::on_attack_r_pressed);
	InputComponent->BindAction("Attack_R", IE_Released, this, &AMyPlayerController::on_attack_r_released);
	

	// 视角移动事件
	InputComponent->BindAction("CanRotate", IE_Pressed, this, &AMyPlayerController::on_can_rotate_pressed);
	InputComponent->BindAction("CanRotate", IE_Released, this, &AMyPlayerController::on_can_rotate_released);
	InputComponent->BindAxis("TurnRate", this, &AMyPlayerController::on_turn_rate);
	InputComponent->BindAxis("LookupRate", this, &AMyPlayerController::on_lookup_rate);

	// 视角缩放事件
	InputComponent->BindAxis("Scale", this, &AMyPlayerController::on_scale);
	
	// 动作模式下呼出鼠标
	InputComponent->BindAction("Call_Mouse", IE_Pressed, this, &AMyPlayerController::on_call_mouse_pressed);
	InputComponent->BindAction("Call_Mouse", IE_Released, this, &AMyPlayerController::on_call_mouse_released);
}

void AMyPlayerController::move_to_cursor() {
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	if(Hit.bBlockingHit) set_new_move_destination(Hit.ImpactPoint);
}
void AMyPlayerController::set_new_move_destination(const FVector DestLocation) {
	APawn* const my_pawn = GetPawn();
	if(my_pawn) {
		UNavigationSystem* const nav_sys = GetWorld()->GetNavigationSystem();
		float const distance = FVector::Dist(DestLocation, my_pawn->GetActorLocation());
		if(nav_sys && (distance > 120.0f)) nav_sys->SimpleMoveToLocation(this, DestLocation);
	}
}

// 事件相关
void AMyPlayerController::on_set_destination_pressed() {
	if(controller_state == EControllerState::E_ACT) {
		b_attack_r = true;
		b_can_rotate = true;
		return; 
	}
	if(controller_state == EControllerState::E_THREED) {
		b_can_rotate = true;
		return;
	}
	if(b_can_rotate) return;
	bShowMouseCursor = true;
	b_move_to_cursor = true;
	AMyCharacter* my_pawn = Cast<AMyCharacter>(GetPawn());		// Danger
	my_pawn->cursor_show(true);
	FInputModeGameAndUI input;
	input.SetHideCursorDuringCapture(false);
	SetInputMode(input);

}
void AMyPlayerController::on_set_destination_released() {
	if(controller_state == EControllerState::E_ACT) {
		b_attack_r = false;
		b_can_rotate = true;
		return;
	}
	if(controller_state == EControllerState::E_THREED) {
		b_can_rotate = false;
		return;
	}
	if(b_can_rotate) return;
	bShowMouseCursor = true;
	b_move_to_cursor = false;
	AMyCharacter* my_pawn = Cast<AMyCharacter>(GetPawn());		// Danger
	my_pawn->cursor_show(false);
}

void AMyPlayerController::on_move_forward(float value) {
	if(get_b_attack()) { return; }
	if(value != 0.0f) {
		// 获得旋转角度
		const FRotator rotation = GetControlRotation();
		const FRotator yaw_rotation(0, rotation.Yaw, 0);
		// 根据旋转计算前向量
		FVector direction, y, z;
		FRotationMatrix(yaw_rotation).GetUnitAxes(direction, y, z);
		// 在前向量方向前进
		APawn* const my_pawn = GetPawn();
		if(my_pawn) my_pawn->AddMovementInput(direction, value);
		StopMovement();
	}
}
void AMyPlayerController::on_move_right(float value) {
	if(get_b_attack()) return;
	if(value != 0.0f) {
		// 获得旋转角度
		const FRotator rotation = GetControlRotation();
		const FRotator yaw_rotaion(0, rotation.Yaw, 0);
		// 根据旋转计算右向量
		FVector direction, x, z;
		FRotationMatrix(yaw_rotaion).GetUnitAxes(x, direction, z);
		// 在右向量方向前进
		APawn* my_pawn = GetPawn();
		if(my_pawn) my_pawn->AddMovementInput(direction, value);
		StopMovement();
	}
}
// run事件
void AMyPlayerController::on_run_start() {
	AMyCharacter* my_pawn = Cast<AMyCharacter>(GetPawn());		// Danger
	if(!my_pawn) return;
	my_pawn->GetCharacterMovement()->MaxWalkSpeed = 800.f;
}
void AMyPlayerController::on_run_stop() {
	AMyCharacter* my_pawn = Cast<AMyCharacter>(GetPawn());		// Danger
	if(!my_pawn) return;
	my_pawn->GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

// 攻击事件
void AMyPlayerController::on_attack_q_pressed() { b_attack_q = true; }
void AMyPlayerController::on_attack_q_released() {  }
void AMyPlayerController::on_attack_w_pressed() { b_attack_w = true; }
void AMyPlayerController::on_attack_w_released() {  }
void AMyPlayerController::on_attack_e_pressed() { b_attack_e = true; }
void AMyPlayerController::on_attack_e_released() {  }
void AMyPlayerController::on_attack_r_pressed() { b_attack_r = true; }
void AMyPlayerController::on_attack_r_released() {  }


// 是否处于攻击状态
bool AMyPlayerController::get_b_attack() {
	if(b_attack_q || b_attack_w || b_attack_e || b_attack_r) return true;
	return false;
}

// 视角移动事件
void AMyPlayerController::on_can_rotate_pressed() {
	if(controller_state == EControllerState::E_ACT) { return; }
	if(controller_state == EControllerState::E_SOLID) { return; }
	b_can_rotate = true;
	bShowMouseCursor = false;
	FInputModeGameOnly input;
	SetInputMode(input);
}
void AMyPlayerController::on_can_rotate_released() {
	if(controller_state == EControllerState::E_ACT) { return; }
	if(controller_state == EControllerState::E_SOLID) { return; }
	b_can_rotate = false;
	bShowMouseCursor = true;
	FInputModeGameAndUI input;
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

// 视角缩放事件
void AMyPlayerController::on_scale(float value) {
	AMyCharacter* my_pawn = (AMyCharacter*) GetPawn();
	my_pawn->get_camera_boom()->TargetArmLength += value * scale_rate;
	my_pawn->get_camera_boom()->TargetArmLength = FMath::Clamp(my_pawn->get_camera_boom()->TargetArmLength, scale_min, scale_max);
}

// 动作模式下呼出鼠标
void AMyPlayerController::on_call_mouse_pressed() {
	if(controller_state != EControllerState::E_ACT) return;
	static bool flag = false;
	bShowMouseCursor = flag;
	if(flag) {
		b_can_rotate = false;
		bShowMouseCursor = true;
		FInputModeGameAndUI input;
		input.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(input);
	} 
	else {
		b_can_rotate = true;
		bShowMouseCursor = false;
		FInputModeGameOnly input;
		SetInputMode(input);
	}
	flag = !flag;
}
void AMyPlayerController::on_call_mouse_released() {
	if(controller_state != EControllerState::E_ACT) return;
	//bShowMouseCursor = false;
}

// 更新控制状态 -- 根据当前状态
void AMyPlayerController::update_controller_state() {
	APawn * my_pawn = GetPawn();
	if(!my_pawn) return;
	switch(controller_state) {
		case EControllerState::E_FREE:
				my_pawn->bUseControllerRotationYaw = false;
			break;
		case EControllerState::E_SOLID:
				SetControlRotation(FRotator(-45.0f, 0.0f, 0.0f));
				my_pawn->bUseControllerRotationYaw = false;
			break;
		case EControllerState::E_THREED:
				my_pawn->bUseControllerRotationYaw = true;
			break;
		case EControllerState::E_ACT:
				b_can_rotate = true;
				bShowMouseCursor = false;
				FInputModeGameOnly input;
				SetInputMode(input);

				my_pawn->bUseControllerRotationYaw = true;
				fight_state = true;
				update_fight_state();
			break;
	}
}

void AMyPlayerController::update_fight_state() {
	AMyCharacter* my_pawn = Cast<AMyCharacter>(GetPawn());
	if(!my_pawn) return;
	if(fight_state) {
		my_pawn->change_sword_place(FName("Sword_1"));   // 设置武器位置
		my_pawn->change_shield_place(FName("Shield_1")); // 设置盾牌位置
	}
	else {
		my_pawn->change_sword_place(FName("Sword_2"));   // 设置武器位置
		my_pawn->change_shield_place(FName("Shield_2")); // 设置盾牌位置
	}
}