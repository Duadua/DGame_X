#pragma once
#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "DamageInterface.h"
#include "AICharacter.generated.h"

UCLASS(Blueprintable)
class DGAME_API AAICharacter : public ABaseCharacter, public IDamageInterface {
	GENERATED_BODY()


public:
	AAICharacter();

public:
	virtual void Tick(float DeltaTime) override;		// ÿ֡������������
	virtual void init_fight() override;					// ��ʼ��ս���ı�

	void change_sword_place(FName n);		// �ı�����λ��
	void change_shield_place(FName n);		// �ı����λ��

											// ��д���˺���
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Fight")
	void DoDamage(float delta);
	virtual void DoDamage_Implementation(float delta) override;

protected:
	//	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	// ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* sword;

	// ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* shield;

	void init_body();						// ��ʼ�� ����һЩ����  
	void init_movement();					// ���� �ƶ���� ������ 
	void init_sword();						// ��ʼ������       
	void init_shield();						// ��ʼ������       


};
