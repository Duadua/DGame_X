#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS(Blueprintable)
class DGAME_API AMyCharacter : public ACharacter {
	GENERATED_BODY()

public:
	AMyCharacter();

public:
	virtual void Tick(float DeltaTime) override;		// ÿ֡������������

	// get ˽������
	FORCEINLINE class UCameraComponent* get_camera_component() const { return camera_component; } 
	FORCEINLINE class USpringArmComponent* get_camera_boom()   const { return camera_boom;      } 
	FORCEINLINE class UDecalComponent* get_cursor_world()      const { return cursor_to_world;  }  

	void cursor_show(int32 b);			// ��ʾ���� -- �� controller ����

	void change_sword_place(FName n);		// �ı�����λ��
	void change_shield_place(FName n);		// �ı����λ��

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// ���arm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* camera_boom;
	//���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* camera_component;

	// �������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* cursor_to_world;

	// ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* shield;
	// ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* sword;

	void init_body();						// ��ʼ�� ����һЩ����  
	void init_movement();					// ���� �ƶ���� ������ 
	void init_camera();						// ��ʼ�����       
	void init_cursor();						// ��ʼ�����       
	void init_sword();						// ��ʼ������       
	void init_shield();						// ��ʼ������       

	void cursor_follow_mouse();				// �����������      
};
