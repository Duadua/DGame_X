#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS(Blueprintable)
class DGAME_API ABaseCharacter : public ACharacter {
	GENERATED_BODY()

public:
	ABaseCharacter();
	
	// ս���ı�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight")
	float health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight")
	float health_max;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight")
	float power;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight")
	float power_max;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight")
	float attack;		// ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight")
	float defence;		// ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight")
	uint32 b_death_start : 1;	// �������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight")
	uint32 b_death_end : 1;	// ������������



protected:
	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void init_fight();

public:

};
