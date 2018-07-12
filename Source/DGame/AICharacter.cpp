#include "AICharacter.h"
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

AAICharacter::AAICharacter() {
	init_body();											// 初始化 本身一些属性
	init_movement();										// 初始化 移动组件
	init_sword();											// 初始化 武器
	init_shield();											// 初始化 盾牌

	change_sword_place(FName("Sword_1"));					// 设置武器位置
	change_shield_place(FName("Shield_1"));					// 设置盾牌位置

	init_fight();
}

//void AAICharacter::BeginPlay() { Super::BeginPlay(); }

void AAICharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}


void AAICharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	check(PlayerInputComponent);

	// 跳跃事件
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAICharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &AAICharacter::StopJumping);


}

// 初始化战斗文本
void AAICharacter::init_fight() {
	Super::init_fight();

}

// 初始化 本身一些属性
void AAICharacter::init_body() {
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
void AAICharacter::init_movement() {
	GetCharacterMovement()->bOrientRotationToMovement = true;				// 以 移动组件 为参照移动物体
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);		// 旋转速度
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->bConstrainToPlane = false;						// 约束到地面上
	GetCharacterMovement()->bSnapToPlaneAtStart = false;					// 一开始就放到地面上

}
// 初始化武器
void AAICharacter::init_sword() {
	sword = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordComponent"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sword_mesh(TEXT("/Game/Assets/Weapon/Sword"));
	if(sword_mesh.Succeeded()) sword->SetSkeletalMesh(sword_mesh.Object, true);
}

// 初始化盾牌
void AAICharacter::init_shield() {
	shield = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShieldComponent"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> shield_mesh(TEXT("/Game/Assets/Weapon/Shield"));
	if(shield_mesh.Succeeded()) shield->SetSkeletalMesh(shield_mesh.Object, true);
}

// 改变武器位置
void AAICharacter::change_sword_place(FName name) {
	sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, name);
}
// 改变盾牌位置
void AAICharacter::change_shield_place(FName name) {
	shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, name);
}

// 受伤计算
void AAICharacter::DoDamage_Implementation(float delta) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("do damage"));
	health -= delta;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(health));
	if(health <= 0.f) { Destroy(); }
}
