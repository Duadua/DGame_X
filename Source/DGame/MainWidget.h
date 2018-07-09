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
	//初始化
	virtual bool Initialize() override;

	// 改变模式按钮
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* mode_bt;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UTextBlock* mode_text;

	// 自在模式
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* free_bt;
	// 固定模式
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* solid_bt;
	// 3D模式
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* threeD_bt;
	// 动作模式
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* act_bt;

	// 战斗状态
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* fight_state_bt;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UTextBlock* fight_state_text;

	// 初始化 -- 获得控件的引用 绑定小事件 -- 大事件逻辑在外面写 -- MVC
	void init();
	// 显示隐藏选择模式
	uint32 b_show_select : 1;
	void show_mode_select();
	void hide_mode_select();
};
