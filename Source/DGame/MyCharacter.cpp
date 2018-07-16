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
	init_body();											// ��ʼ�� ����һЩ����
	init_movement();										// ��ʼ�� �ƶ����
	init_camera();											// ��ʼ�� ���
	init_cursor();											// ��ʼ�� �������
	init_sword();											// ��ʼ�� ����
	init_shield();											// ��ʼ�� ����
	init_blood_bar();										// ��ʼ�� Ѫ��

	change_sword_place(FName("Sword_2"));					// ��������λ��
	change_shield_place(FName("Shield_2"));					// ���ö���λ��
	cursor_show(false);

	init_fight();
}

void AMyCharacter::BeginPlay() { 
	Super::BeginPlay(); 
	update_blood_bar();
}

void AMyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	cursor_follow_mouse();						// �������������
	update_blood_bar();							// ����Ѫ��

	if(GetActorLocation().Z < -200.f) { SetActorLocation(FVector(0.f, 0.f, 500.f)); }
}


void AMyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	check(PlayerInputComponent);

	// ��Ծ�¼�
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyCharacter::StopJumping);

	
}

// ��ʼ��ս���ı�
void AMyCharacter::init_fight() {
	Super::init_fight();
	power = 200.f;
	power_max = 300.f;
}


// ��ʼ�� ����һЩ����
void AMyCharacter::init_body() {
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
void AMyCharacter::init_movement() {
	GetCharacterMovement()->bOrientRotationToMovement = true;				// �� �ƶ���� Ϊ�����ƶ�����
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);		// ��ת�ٶ�
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->bConstrainToPlane = false;						// Լ����������
	GetCharacterMovement()->bSnapToPlaneAtStart = false;					// һ��ʼ�ͷŵ�������
	
}
// ��ʼ�����
void AMyCharacter::init_camera() {
	// ��ʼ����� arm
	camera_boom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	camera_boom->SetupAttachment(RootComponent);
	camera_boom->TargetArmLength = 600.f;								// ��ʼ�۳�
	camera_boom->RelativeRotation        = FRotator(-45.f, 0.f, 0.f);	// ��ʼ�Ƕ�
	camera_boom->bDoCollisionTest        = false;						// ��ֹ��ײ
	camera_boom->bAbsoluteRotation       = true;						// �����ƶ���ʱ�����arm���ƶ�
	camera_boom->bUsePawnControlRotation = true;						// ��PlayerController ��ת

	// ��ʼ�����
	camera_component = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	camera_component->SetupAttachment(camera_boom, USpringArmComponent::SocketName);
	camera_component->bUsePawnControlRotation = false;					// �������arm�ƶ�
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
	if(shield_mesh.Succeeded()) shield->SetSkeletalMesh(shield_mesh.Object, true);
}

// ��ʼ��Ѫ��	
void AMyCharacter::init_blood_bar() {
	blood_bar = CreateDefaultSubobject<UWidgetComponent>(TEXT("BloodBar"));
	blood_bar->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// ����User Widget
	UClass* widget = LoadClass<UBloodBarWidget>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/UI/bp_umg_blood.bp_umg_blood_C'"));
	blood_bar->SetWidgetClass(widget);
	blood_bar->SetRelativeLocation(FVector(0.f, 0.f, 100.f));

}
// ����Ѫ��	
void AMyCharacter::update_blood_bar() {
	UBloodBarWidget* bld = Cast<UBloodBarWidget>(blood_bar->GetUserWidgetObject());
	if(!bld) return;
	bld->set_percent(health / health_max);
	bld->set_color(FLinearColor(210.f/ 255.f, 305.f/255.f, 30.f/255.f));

	// ����Ϊʼ�ճ���ͷ
	FVector con_location = UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->K2_GetActorLocation();
	FVector bar_location = blood_bar->GetComponentLocation();
	FRotator rot = UKismetMathLibrary::MakeRotFromX(con_location - bar_location);
	blood_bar->SetWorldRotation(rot);

}

// �ı�����λ��
void AMyCharacter::change_sword_place(FName name) {
	sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, name);
}
// �ı����λ��
void AMyCharacter::change_shield_place(FName name) {
	shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, name);
}

// ��ʾ���� -- �� controller ����
void AMyCharacter::cursor_show(int32 f) { cursor_to_world->SetVisibility(f); }
// �����������
void AMyCharacter::cursor_follow_mouse() {
	if(cursor_to_world != nullptr) {
		if(APlayerController* PC = Cast<APlayerController>(GetController())) {
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			cursor_to_world->SetWorldLocation(TraceHitResult.Location);
			cursor_to_world->SetWorldRotation(CursorR);
		} // ���� pc �˵� ����λ�� �������������
		
	}
}

// ���˼���
void AMyCharacter::DoDamage_Implementation(float delta) {
	health -= delta;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("do damage"));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(health));
	if(health <= 0.f) b_death_start = true;
}
