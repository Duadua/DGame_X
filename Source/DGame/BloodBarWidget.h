#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BloodBarWidget.generated.h"

UCLASS()
class DGAME_API UBloodBarWidget : public UUserWidget {
	GENERATED_BODY()

public:
	//��ʼ��
	virtual bool Initialize() override; 

	//Ѫ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UProgressBar* blood_bar;

	void init();
	void set_percent(float value);
	void set_color(FLinearColor color);
};
