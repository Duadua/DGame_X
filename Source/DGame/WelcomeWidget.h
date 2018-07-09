#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "WelcomeWidget.generated.h"

UCLASS()
class DGAME_API UWelcomeWidget : public UUserWidget {
	GENERATED_BODY()

public:
	//初始化
	virtual bool Initialize() override;

	// 开始按钮
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* start_bt;
	// 开始按钮
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* regist_bt;
	// 开始按钮
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UButton* quit_bt;

	// 初始化 -- 获得控件的引用 绑定小事件 -- 大事件逻辑在外面写 -- MVC
	void init();

	// 退出按钮事件 -- 因为比较小，可以在这里写
	UFUNCTION()
	void on_quit_clicked();
};
