#include "WelcomeWidget.h"
#include "Kismet/KismetSystemLibrary.h"

bool UWelcomeWidget::Initialize() {
	if(!Super::Initialize()) return false;
	init();
	return true;
}

void UWelcomeWidget::init() {
	// ��ʼ��
	start_bt  = Cast<UButton>(GetWidgetFromName(TEXT("bt_start")));  
	regist_bt = Cast<UButton>(GetWidgetFromName(TEXT("bt_regist"))); 
	quit_bt   = Cast<UButton>(GetWidgetFromName(TEXT("bt_quit")));   

	// ���¼�
	quit_bt->OnClicked.AddDynamic(this, &UWelcomeWidget::on_quit_clicked);
}

void UWelcomeWidget::on_quit_clicked() {
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit);
}