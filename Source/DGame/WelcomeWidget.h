#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "WelcomeWidget.generated.h"

UCLASS()
class DGAME_API UWelcomeWidget : public UUserWidget {
	GENERATED_BODY()

public:
	//��ʼ��
	virtual bool Initialize() override;

	// ��ʼ��ť
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* start_bt;
	// ��ʼ��ť
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* regist_bt;
	// ��ʼ��ť
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* quit_bt;

	// ��ʼ�� -- ��ÿؼ������� ��С�¼� -- ���¼��߼�������д -- MVC
	void init();

	// �˳���ť�¼� -- ��Ϊ�Ƚ�С������������д
	UFUNCTION()
	void on_quit_clicked();
};
