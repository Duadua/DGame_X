#pragma once
#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyWeapon.generated.h"

// 武器等装备的封装类
UCLASS(Blueprintable)
class DGAME_API UMyWeapon : public USkeletalMeshComponent {
	GENERATED_UCLASS_BODY()
	
public:

	UMyWeapon();

	UPROPERTY(EditDefaultsOnly, Category = "Attachment")
	FName attach_point;					// 附加的绑定点
	
	// 设置附着着
	UFUNCTION()
	void set_attach_point(USceneComponent* parent, FName name);
	
};
