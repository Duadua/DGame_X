#pragma once
#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyWeapon.generated.h"

// ������װ���ķ�װ��
UCLASS(Blueprintable)
class DGAME_API UMyWeapon : public USkeletalMeshComponent {
	GENERATED_UCLASS_BODY()
	
public:

	UMyWeapon();

	UPROPERTY(EditDefaultsOnly, Category = "Attachment")
	FName attach_point;					// ���ӵİ󶨵�
	
	// ���ø�����
	UFUNCTION()
	void set_attach_point(USceneComponent* parent, FName name);
	
};
