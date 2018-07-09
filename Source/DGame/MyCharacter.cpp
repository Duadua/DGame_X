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

AMyCharacter::AMyCharacter() {
	init_body();											// ��ʼ�� ����һЩ����
	init_movement();										// ��ʼ�� �ƶ����
	init_camera();											// ��ʼ�� ���
	init_cursor();											// ��ʼ�� �������
	init_sword();											// ��ʼ�� ����
	init_shield();											// ��ʼ�� ����

	change_sword_place(FName("Sword_2"));					// ��������λ��
	change_shield_place(FName("Shield_2"));					// ���ö���λ��
}

void AMyCharacter::BeginPlay() { Super::BeginPlay(); }

void AMyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	cursor_follow_mouse();						// �������������
}

void AMyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	check(PlayerInputComponent);

	// ��Ծ�¼�
	PlayerInputComponent->BindAction("Jump", IE_Pressed,  this, &AMyCharacter::Jump);        
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyCharacter::StopJumping); 
}

// ��ʼ�� ����һЩ����
void AMyCharacter::init_body() {
	// Tick ����
	PrimaryActorTick.bCanEverTick          = true; 
	PrimaryActorTick.bStartWithTickEnabled = true; 

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);			// ��ʼ���������С

	// ����������Controller�ƶ�
	bUseControllerRotationPitch = false; 
	bUseControllerRotationYaw   = false; 
	bUseControllerRotationRoll  = false; 

	// ���� mesh λ��
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -97.5f), FRotator(0.f, -90.f, 0.f).Quaternion());
}

// ���� �ƶ���� ������
void AMyCharacter::init_movement() {
	GetCharacterMovement()->bOrientRotationToMovement = true;							// �� �ƶ���� Ϊ�����ƶ�����            
	GetCharacterMovement()->RotationRate              = FRotator(0.f, 640.f, 0.f);		// ��ת�ٶ� 
	GetCharacterMovement()->JumpZVelocity             = 600.f;                             
	GetCharacterMovement()->bConstrainToPlane         = false;							// Լ����������                   
	GetCharacterMovement()->bSnapToPlaneAtStart       = false;							// һ��ʼ�ͷŵ�������                
}
// ��ʼ�����
void AMyCharacter::init_camera() {
	// ��ʼ����� arm
	camera_boom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	camera_boom->SetupAttachment(RootComponent);
	camera_boom->TargetArmLength         = 800.f;                     // ��ʼ�۳�                 
	//camera_boom->RelativeRotation        = FRotator(-60.f, 0.f, 0.f); // ��ʼ�Ƕ�                 
	//camera_boom->bDoCollisionTest        = false;                     // ��ֹ��ײ                 
	//camera_boom->bAbsoluteRotation       = true;                      // �����ƶ���ʱ�����arm���ƶ�      
	camera_boom->bUsePawnControlRotation = true;                      // ��PlayerController ��ת  

	// ��ʼ�����
	camera_component = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	camera_component->SetupAttachment(camera_boom, USpringArmComponent::SocketName);
	camera_component->bUsePawnControlRotation = false;									// �������arm�ƶ�
}
// ��ʼ�����
void AMyCharacter::init_cursor() {
	cursor_to_world = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	cursor_to_world->SetupAttachment(RootComponent);
	// ����������ʽ
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Assets/Cursor/M_Cursor_Decal.M_Cursor_Decal'"));
	if(DecalMaterialAsset.Succeeded()) cursor_to_world->SetDecalMaterial(DecalMaterialAsset.Object);

	cursor_to_world->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	cursor_to_world->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	cursor_to_world->SetVisibility(false);
}
// ��ʼ������
void AMyCharacter::init_sword() {
	sword = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordComponent"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sword_mesh(TEXT("/Game/Assets/Weapon/Sword"));
	if(sword_mesh.Succeeded()) sword->SetSkeletalMesh(sword_mesh.Object, true);
}
// ��ʼ������
void AMyCharacter::init_shield() {
	shield = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShieldComponent"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> shield_mesh(TEXT("/Game/Assets/Weapon/Shield"));
	shield->SetSkeletalMesh(shield_mesh.Object, true);
}

// �ı�����λ��
void AMyCharacter::change_sword_place(FName name) {
	sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, name);
}
// �ı����λ��
void AMyCharacter::change_shield_place(FName name) {
	shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, name);
}

// ��ʾ���� -- �� controller ����
void AMyCharacter::cursor_show(int32 f) { cursor_to_world->SetVisibility(f); }
// �����������
void AMyCharacter::cursor_follow_mouse() {
	if(cursor_to_world != nullptr) {
		if(UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled()) {
			if(UWorld* World = GetWorld()) {
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = camera_component->GetComponentLocation();
				FVector EndLocation = camera_component->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				cursor_to_world->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if(APlayerController* PC = Cast<APlayerController>(GetController())) {
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			cursor_to_world->SetWorldLocation(TraceHitResult.Location);
			cursor_to_world->SetWorldRotation(CursorR);
		} // ���� pc �˵� ����λ�� �������������
	}
}