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
	virtual void Tick(float DeltaTime) override;		// 每帧都会做的事情
	virtual void init_fight() override;					// 初始化战斗文本

	void change_sword_place(FName n);		// 改变武器位置
	void change_shield_place(FName n);		// 改变盾牌位置

											// 重写受伤函数
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Fight")
	void DoDamage(float delta);
	virtual void DoDamage_Implementation(float delta) override;

protected:
	//	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	// 武器
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* sword;

	// 盾牌
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* shield;

	void init_body();						// 初始化 本身一些属性  
	void init_movement();					// 设置 移动组件 的属性 
	void init_sword();						// 初始化武器       
	void init_shield();						// 初始化盾牌       


};
