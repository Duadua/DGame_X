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
	init_body();											// ��ʼ�� ����һЩ����
	init_movement();										// ��ʼ�� �ƶ����
	init_sword();											// ��ʼ�� ����
	init_shield();											// ��ʼ�� ����

	change_sword_place(FName("Sword_1"));					// ��������λ��
	change_shield_place(FName("Shield_1"));					// ���ö���λ��

	init_fight();
}

//void AAICharacter::BeginPlay() { Super::BeginPlay(); }

void AAICharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}


void AAICharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	check(PlayerInputComponent);

	// ��Ծ�¼�
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAICharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &AAICharacter::StopJumping);


}

// ��ʼ��ս���ı�
void AAICharacter::init_fight() {
	Super::init_fight();

}

// ��ʼ�� ����һЩ����
void AAICharacter::init_body() {
	// Tick ����
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);			// ��ʼ���������С
																	// ����������Controller�ƶ�
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// ���� mesh λ��
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -97.5f), FRotator(0.f, -90.f, 0.f).Quaternion());
}

// ���� �ƶ���� ������
void AAICharacter::init_movement() {
	GetCharacterMovement()->bOrientRotationToMovement = true;				// �� �ƶ���� Ϊ�����ƶ�����
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);		// ��ת�ٶ�
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->bConstrainToPlane = false;						// Լ����������
	GetCharacterMovement()->bSnapToPlaneAtStart = false;					// һ��ʼ�ͷŵ�������

}
// ��ʼ������
void AAICharacter::init_sword() {
	sword = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordComponent"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sword_mesh(TEXT("/Game/Assets/Weapon/Sword"));
	if(sword_mesh.Succeeded()) sword->SetSkeletalMesh(sword_mesh.Object, true);
}

// ��ʼ������
void AAICharacter::init_shield() {
	shield = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShieldComponent"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> shield_mesh(TEXT("/Game/Assets/Weapon/Shield"));
	if(shield_mesh.Succeeded()) shield->SetSkeletalMesh(shield_mesh.Object, true);
}

// �ı�����λ��
void AAICharacter::change_sword_place(FName name) {
	sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, name);
}
// �ı����λ��
void AAICharacter::change_shield_place(FName name) {
	shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, name);
}

// ���˼���
void AAICharacter::DoDamage_Implementation(float delta) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("do damage"));
	health -= delta;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(health));
	if(health <= 0.f) { Destroy(); }
}
