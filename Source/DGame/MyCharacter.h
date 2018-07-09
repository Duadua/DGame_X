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
	virtual void Tick(float DeltaTime) override;		// 每帧都会做的事情

	// get 私有属性
	FORCEINLINE class UCameraComponent* get_camera_component() const { return camera_component; } 
	FORCEINLINE class USpringArmComponent* get_camera_boom()   const { return camera_boom;      } 
	FORCEINLINE class UDecalComponent* get_cursor_world()      const { return cursor_to_world;  }  

	void cursor_show(int32 b);			// 显示贴花 -- 供 controller 调用

	void change_sword_place(FName n);		// 改变武器位置
	void change_shield_place(FName n);		// 改变盾牌位置

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// 相机arm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* camera_boom;
	//相机
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* camera_component;

	// 鼠标贴花
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* cursor_to_world;

	// 武器
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* shield;
	// 盾牌
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* sword;

	void init_body();						// 初始化 本身一些属性  
	void init_movement();					// 设置 移动组件 的属性 
	void init_camera();						// 初始化相机       
	void init_cursor();						// 初始化光标       
	void init_sword();						// 初始化武器       
	void init_shield();						// 初始化盾牌       

	void cursor_follow_mouse();				// 贴花跟鼠标走      
};
