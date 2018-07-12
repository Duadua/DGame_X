#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS(Blueprintable)
class DGAME_API ABaseCharacter : public ACharacter {
	GENERATED_BODY()

public:
	ABaseCharacter();
	
	// 战斗文本
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight")
	float health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight")
	float health_max;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight")
	float power;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight")
	float power_max;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight")
	float attack;		// 攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight")
	float defence;		// 防御

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight")
	uint32 b_death_start : 1;	// 死亡标记
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight")
	uint32 b_death_end : 1;	// 放完死亡动画



protected:
	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void init_fight();

public:

};
