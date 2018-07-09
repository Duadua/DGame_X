#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WelcomeWidget.h"
#include "MainWidget.h"
#include "DGameGameModeBase.generated.h"

UCLASS()
class DGAME_API ADGameGameModeBase : public AGameModeBase {
	GENERATED_BODY()

public:
	ADGameGameModeBase();

	virtual void BeginPlay() override;

	// 欢迎界面
	UPROPERTY()
		UWelcomeWidget* welcome_ui;
	// 主界面
	UPROPERTY()
		UMainWidget* main_ui;

	// 界面初始化
	void init_welcome();
	void init_main();

	// 注册事件
	UFUNCTION()							// 一定要加上, 否则不会调用
	void on_welcome_start_clicked();	// 欢迎界面 开始按钮
	UFUNCTION()
	void on_main_mode_clicked();		// 主界面 模式切换按钮

		// 模式切换控件
	UFUNCTION()
	void on_main_free_clicked();		// 切换到自在模式
	UFUNCTION()
	void on_main_solid_clicked();		// 切换到固定模式
	UFUNCTION()
	void on_main_threeD_clicked();		// 切换到3D模式
	UFUNCTION()
	void on_main_act_clicked();			// 切换到动作模式

	// 战斗状态切换控件
	UFUNCTION()
	void on_main_fight_click();			// 切换战斗状态

protected:

private:

	// 小工具
	// 显示哪个窗口
	UUserWidget* cur_widget;
	void show_widget(UUserWidget* widget);
	void hide_widget();
	// 改变输入模式
	void change_input_to_game_only();
	void change_input_to_game_ui();
	void change_input_to_ui_only();
};
