#include "MainWidget.h"
#include "Components/ProgressBar.h"

//初始化
bool UMainWidget::Initialize() {
	if(!Super::Initialize()) return false;
	init();
	hide_mode_select();
	return true;
}

// 初始化 -- 获得控件的引用 绑定小事件 -- 大事件逻辑在外面写 -- MVC
void UMainWidget::init() {
	// 初始化
	mode_bt   = Cast<UButton>(GetWidgetFromName(TEXT("bt_mode")));      
	mode_text = Cast<UTextBlock>(GetWidgetFromName(TEXT("text_mode"))); 
	mode_text->SetText(FText::FromString(TEXT("自在视角")));

	free_bt   = Cast<UButton>(GetWidgetFromName(TEXT("bt_free")));  
	solid_bt  = Cast<UButton>(GetWidgetFromName(TEXT("bt_solid"))); 
	threeD_bt = Cast<UButton>(GetWidgetFromName(TEXT("bt_3d")));    
	act_bt    = Cast<UButton>(GetWidgetFromName(TEXT("bt_act")));   

	fight_state_bt   = Cast<UButton>(GetWidgetFromName(TEXT("bt_fight_state")));    
	fight_state_text = Cast<UTextBlock>(GetWidgetFromName(TEXT("text_fight_state"))); 
	fight_state_text->SetText(FText::FromString(TEXT("和")));

	// 血量
	health_bar  = Cast<UProgressBar>(GetWidgetFromName(TEXT("bar_health"))); 
	health_text = Cast<UTextBlock>(GetWidgetFromName(TEXT("text_health")));  
	// 魔法
	pow_bar  = Cast<UProgressBar>(GetWidgetFromName(TEXT("bar_pow"))); 
	pow_text = Cast<UTextBlock>(GetWidgetFromName(TEXT("text_pow"))); 

	// 等级
	level_text = Cast<UTextBlock>(GetWidgetFromName(TEXT("text_level")));

}
// 显示隐藏选择模式
void UMainWidget::show_mode_select() {
	b_show_select = true;
	free_bt->SetVisibility(ESlateVisibility::Visible);
	solid_bt->SetVisibility(ESlateVisibility::Visible);
	threeD_bt->SetVisibility(ESlateVisibility::Visible);
	act_bt->SetVisibility(ESlateVisibility::Visible);
}
void UMainWidget::hide_mode_select() {
	b_show_select = false;
	free_bt->SetVisibility(ESlateVisibility::Hidden);
	solid_bt->SetVisibility(ESlateVisibility::Hidden);
	threeD_bt->SetVisibility(ESlateVisibility::Hidden);
	act_bt->SetVisibility(ESlateVisibility::Hidden);
}