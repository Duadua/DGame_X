#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MainWidget.generated.h"

UCLASS()
class DGAME_API UMainWidget : public UUserWidget {
	GENERATED_BODY()

public:
	//��ʼ��
	virtual bool Initialize() override;

	// �ı�ģʽ��ť
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* mode_bt;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UTextBlock* mode_text;

	// ����ģʽ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* free_bt;
	// �̶�ģʽ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* solid_bt;
	// 3Dģʽ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* threeD_bt;
	// ����ģʽ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* act_bt;

	// ս��״̬
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* fight_state_bt;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UTextBlock* fight_state_text;

	// ��ʼ�� -- ��ÿؼ������� ��С�¼� -- ���¼��߼�������д -- MVC
	void init();
	// ��ʾ����ѡ��ģʽ
	uint32 b_show_select : 1;
	void show_mode_select();
	void hide_mode_select();
};
