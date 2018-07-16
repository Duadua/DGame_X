#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Engine.h"
#include "BloodBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

AMyCharacter::AMyCharacter() {
	init_body();											// 初始化 本身一些属性
	init_movement();										// 初始化 移动组件
	init_camera();											// 初始化 相机
	init_cursor();											// 初始化 光标贴花
	init_sword();											// 初始化 武器
	init_shield();											// 初始化 盾牌
	init_blood_bar();										// 初始化 血条

	change_sword_place(FName("Sword_2"));					// 设置武器位置
	change_shield_place(FName("Shield_2"));					// 设置盾牌位置
	cursor_show(false);

	init_fight();
}

void AMyCharacter::BeginPlay() { 
	Super::BeginPlay(); 
	update_blood_bar();
}

void AMyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	cursor_follow_mouse();						// 让贴花跟鼠标走
	update_blood_bar();							// 更新血量

	if(GetActorLocation().Z < -200.f) { SetActorLocation(FVector(0.f, 0.f, 500.f)); }
}


void AMyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	check(PlayerInputComponent);

	// 跳跃事件
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyCharacter::StopJumping);

	
}

// 初始化战斗文本
void AMyCharacter::init_fight() {
	Super::init_fight();
	power = 200.f;
	power_max = 300.f;
}


// 初始化 本身一些属性
void AMyCharacter::init_body() {
	// Tick 属性
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);			// 初始化胶囊体大小

	// 不让人物随Controller移动
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 设置 mesh 位置
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -97.5f), FRotator(0.f, -90.f, 0.f).Quaternion());
}

// 设置 移动组件 的属性
void AMyCharacter::init_movement() {
	GetCharacterMovement()->bOrientRotationToMovement = true;				// 以 移动组件 为参照移动物体
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);		// 旋转速度
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->bConstrainToPlane = false;						// 约束到地面上
	GetCharacterMovement()->bSnapToPlaneAtStart = false;					// 一开始就放到地面上
	
}
// 初始化相机
void AMyCharacter::init_camera() {
	// 初始化相机 arm
	camera_boom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	camera_boom->SetupAttachment(RootComponent);
	camera_boom->TargetArmLength = 600.f;								// 初始臂长
	camera_boom->RelativeRotation        = FRotator(-45.f, 0.f, 0.f);	// 初始角度
	camera_boom->bDoCollisionTest        = false;						// 禁止碰撞
	camera_boom->bAbsoluteRotation       = true;						// 人物移动的时候相机arm不移动
	camera_boom->bUsePawnControlRotation = true;						// 随PlayerController 旋转

	// 初始化相机
	camera_component = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	camera_component->SetupAttachment(camera_boom, USpringArmComponent::SocketName);
	camera_component->bUsePawnControlRotation = false;					// 相机不随arm移动
}
// 初始化光标
void AMyCharacter::init_cursor() {
	cursor_to_world = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	cursor_to_world->SetupAttachment(RootComponent);
	// 设置贴花样式
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Assets/Cursor/M_Cursor_Decal.M_Cursor_Decal'"));
	if(DecalMaterialAsset.Succeeded()) cursor_to_world->SetDecalMaterial(DecalMaterialAsset.Object);

	cursor_to_world->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	cursor_to_world->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	cursor_to_world->SetVisibility(false);
}
// 初始化武器
void AMyCharacter::init_sword() {
	sword = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordComponent"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sword_mesh(TEXT("/Game/Assets/Weapon/Sword"));
	if(sword_mesh.Succeeded()) sword->SetSkeletalMesh(sword_mesh.Object, true);
}

// 初始化盾牌
void AMyCharacter::init_shield() {
	shield = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShieldComponent"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> shield_mesh(TEXT("/Game/Assets/Weapon/Shield"));
	if(shield_mesh.Succeeded()) shield->SetSkeletalMesh(shield_mesh.Object, true);
}

// 初始化血条	
void AMyCharacter::init_blood_bar() {
	blood_bar = CreateDefaultSubobject<UWidgetComponent>(TEXT("BloodBar"));
	blood_bar->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// 设置User Widget
	UClass* widget = LoadClass<UBloodBarWidget>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/UI/bp_umg_blood.bp_umg_blood_C'"));
	blood_bar->SetWidgetClass(widget);
	blood_bar->SetRelativeLocation(FVector(0.f, 0.f, 100.f));

}
// 更新血条	
void AMyCharacter::update_blood_bar() {
	UBloodBarWidget* bld = Cast<UBloodBarWidget>(blood_bar->GetUserWidgetObject());
	if(!bld) return;
	bld->set_percent(health / health_max);
	bld->set_color(FLinearColor(210.f/ 255.f, 305.f/255.f, 30.f/255.f));

	// 设置为始终朝向镜头
	FVector con_location = UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->K2_GetActorLocation();
	FVector bar_location = blood_bar->GetComponentLocation();
	FRotator rot = UKismetMathLibrary::MakeRotFromX(con_location - bar_location);
	blood_bar->SetWorldRotation(rot);

}

// 改变武器位置
void AMyCharacter::change_sword_place(FName name) {
	sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, name);
}
// 改变盾牌位置
void AMyCharacter::change_shield_place(FName name) {
	shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, name);
}

// 显示贴花 -- 供 controller 调用
void AMyCharacter::cursor_show(int32 f) { cursor_to_world->SetVisibility(f); }
// 贴花跟鼠标走
void AMyCharacter::cursor_follow_mouse() {
	if(cursor_to_world != nullptr) {
		if(APlayerController* PC = Cast<APlayerController>(GetController())) {
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			cursor_to_world->SetWorldLocation(TraceHitResult.Location);
			cursor_to_world->SetWorldRotation(CursorR);
		} // 处理 pc 端的 贴花位置 让贴花跟鼠标走
		
	}
}

// 受伤计算
void AMyCharacter::DoDamage_Implementation(float delta) {
	health -= delta;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("do damage"));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(health));
	if(health <= 0.f) b_death_start = true;
}
