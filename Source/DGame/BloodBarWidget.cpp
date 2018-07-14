#include "BloodBarWidget.h"
#include "Components/ProgressBar.h"

bool UBloodBarWidget::Initialize() {
	if(!Super::Initialize()) return false;
	init();
	return true;
}

// ≥ı ºªØ
void UBloodBarWidget::init() {
	blood_bar = Cast<UProgressBar>(GetWidgetFromName(TEXT("bar_blood")));
	blood_bar->SetPercent(1.0);
}

void UBloodBarWidget::set_percent(float value) { blood_bar->SetPercent(value); }
void UBloodBarWidget::set_color(FLinearColor color) { blood_bar->FillColorAndOpacity = color; }