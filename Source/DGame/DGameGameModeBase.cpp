#include "DGameGameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "MyPlayerController.h"
#include "MyCharacter.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"

//#include "Runtime/SQLiteSupport/Public/SQLiteDatabaseConnection.h"

//static void Log(FString info) { if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, info);                         } 
//static void Log(int     info) { if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::FromInt(info));       } 
//static void Log(float   info) { if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::SanitizeFloat(info)); } 

ADGameGameModeBase::ADGameGameModeBase() {
	// 设置默认角色控制器
	PlayerControllerClass = AMyPlayerController::StaticClass();
	// 设置默认角色
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Character/bp_MyCharacter"));
	if(PlayerPawnBPClass.Class != NULL) { DefaultPawnClass = PlayerPawnBPClass.Class; }

	// 当前widget
	hide_widget();
}

void ADGameGameModeBase::BeginPlay() {
	Super::BeginPlay();

	// 界面初始化
	init_welcome();					// 欢迎界面
	init_main();					// 主界面

	// 游戏初始化
	show_widget(welcome_ui);		// 显示欢迎界面
	change_input_to_ui_only();		// 设置UIOnly输入

	// datebase test
	/*FSQLiteDatabase my_db;
	if(my_db.Open(TEXT("E:/mydb.db"), nullptr, nullptr)) {
		int id = 1;
		FString pw = TEXT("'123'");
		FString name = TEXT("'dua'");
		FString sql = FString::Printf(TEXT("insert into user (user_id, user_pw, user_name) values (%d, %s, %s)"), id, *pw, *name);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, sql);
		if(!my_db.Execute(*(sql))) {
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, "no");
		}
		else GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, "good");
	}*/
}


void ADGameGameModeBase::Tick(float Delta) {
	Super::Tick(Delta);

	update_health_and_pow();
	update_exp();
}

// 初始化
// 欢迎界面
void ADGameGameModeBase::init_welcome() {
	// 静态加载LoadClass 也可以建一个GameMode蓝图, 使用 TSubClassOf<> 动态加载
	welcome_ui = CreateWidget<UWelcomeWidget>(GetGameInstance(), LoadClass<UWelcomeWidget>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/UI/bp_umg_welcome.bp_umg_welcome_C'")));
	// 给界面的控件注册事件
	welcome_ui->start_bt->OnClicked.AddDynamic(this, &ADGameGameModeBase::on_welcome_start_clicked);
}
// 主界面
void ADGameGameModeBase::init_main() {
	main_ui = CreateWidget<UMainWidget>(GetGameInstance(), LoadClass<UMainWidget>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/UI/bp_umg_main.bp_umg_main_C'")));
	main_ui->mode_bt->OnClicked.AddDynamic(this, &ADGameGameModeBase::on_main_mode_clicked);
	// 给模式选择按钮添加事件
	main_ui->free_bt->OnClicked.AddDynamic(this, &ADGameGameModeBase::on_main_free_clicked);
	main_ui->solid_bt->OnClicked.AddDynamic(this, &ADGameGameModeBase::on_main_solid_clicked);
	main_ui->threeD_bt->OnClicked.AddDynamic(this, &ADGameGameModeBase::on_main_threeD_clicked);
	main_ui->act_bt->OnClicked.AddDynamic(this, &ADGameGameModeBase::on_main_act_clicked);

	// 给战斗状态按钮添加事件
	main_ui->fight_state_bt->OnClicked.AddDynamic(this, &ADGameGameModeBase::on_main_fight_click);
}

// 注册事件
// 欢迎界面 开始按钮
void ADGameGameModeBase::on_welcome_start_clicked() {
	show_widget(main_ui);
	change_input_to_game_ui();
	on_main_act_clicked();
}

// 主界面 模式切换按钮
void ADGameGameModeBase::on_main_mode_clicked() {
	if(!main_ui->b_show_select) main_ui->show_mode_select();
	else main_ui->hide_mode_select();
}

// 切换到自在模式
void ADGameGameModeBase::on_main_free_clicked() {
	AMyPlayerController* con = (AMyPlayerController*) UGameplayStatics::GetPlayerController(GetWorld(), 0);		// 获得 PlayerController
	if(con) {
		con->controller_state = EControllerState::E_FREE;
		con->update_controller_state();
		main_ui->hide_mode_select();
		main_ui->mode_text->SetText(FText::FromString(TEXT("自在视角")));
	}
}
// 切换到固定模式
void ADGameGameModeBase::on_main_solid_clicked() {
	AMyPlayerController* con = (AMyPlayerController*) UGameplayStatics::GetPlayerController(GetWorld(), 0);		// 获得 PlayerController
	if(con) {
		con->controller_state = EControllerState::E_SOLID;
		con->update_controller_state();
		main_ui->hide_mode_select();
		main_ui->mode_text->SetText(FText::FromString(TEXT("固定视角")));
	}
}
// 切换到3D模式
void ADGameGameModeBase::on_main_threeD_clicked() {
	AMyPlayerController* con = (AMyPlayerController*) UGameplayStatics::GetPlayerController(GetWorld(), 0);		// 获得 PlayerController
	if(con) {
		con->controller_state = EControllerState::E_THREED;
		con->update_controller_state();
		main_ui->hide_mode_select();
		main_ui->mode_text->SetText(FText::FromString(TEXT("3D模式")));
	}
}
// 切换到动作模式
void ADGameGameModeBase::on_main_act_clicked() {
	AMyPlayerController* con = (AMyPlayerController*) UGameplayStatics::GetPlayerController(GetWorld(), 0);		// 获得 PlayerController
	if(con) {
		con->controller_state = EControllerState::E_ACT;
		con->update_controller_state();
		main_ui->hide_mode_select();
		main_ui->mode_text->SetText(FText::FromString(TEXT("动作模式")));
		main_ui->fight_state_text->SetText(FText::FromString(TEXT("战")));
	}
}

// 切换战斗状态
void ADGameGameModeBase::on_main_fight_click() {
	AMyPlayerController* con = (AMyPlayerController*) UGameplayStatics::GetPlayerController(GetWorld(), 0);		// 获得 PlayerController
	if(!con) return;
	con->fight_state = !con->fight_state;
	//con->update_fight_state();
	if(con->fight_state) main_ui->fight_state_text->SetText(FText::FromString(TEXT("战")));
	else main_ui->fight_state_text->SetText(FText::FromString(TEXT("和")));
}

// 更新血量和魔法
void ADGameGameModeBase::update_health_and_pow() {
	AMyCharacter* my_pawn = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(!my_pawn) return;
	float value = my_pawn->health / my_pawn->health_max;
	main_ui->health_bar->SetPercent(value);
	FText text = FText::FromString(FString::Printf(TEXT("%d/%d"), (int)my_pawn->health, (int)my_pawn->health_max));
	main_ui->health_text->SetText(text);

	value = my_pawn->power / my_pawn->power_max;
	main_ui->pow_bar->SetPercent(value);
	text = FText::FromString(FString::Printf(TEXT("%d/%d"), (int)my_pawn->power, (int)my_pawn->power_max));
	main_ui->pow_text->SetText(text);
}
// 更新等级
void ADGameGameModeBase::update_exp() {
	AMyCharacter* my_pawn = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(!my_pawn) return;
	int level = (int)my_pawn->exp / 200.f;
	FText text = FText::FromString(FString::Printf(TEXT("%d"), level));
	main_ui->level_text->SetText(text);


}

// 小工具
// 显示哪个窗口
void ADGameGameModeBase::show_widget(UUserWidget* widget) {
	if(cur_widget != nullptr) cur_widget->RemoveFromViewport();
	cur_widget = widget;
	cur_widget->AddToViewport();
}
void ADGameGameModeBase::hide_widget() {
	if(cur_widget != nullptr) cur_widget->RemoveFromViewport();
	cur_widget = nullptr;
}
//改变显示模式
void ADGameGameModeBase::change_input_to_game_only() {
	AMyPlayerController* con = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));		// 获得 PlayerController
	FInputModeGameOnly input;
	con->SetInputMode(input);
}
void ADGameGameModeBase::change_input_to_game_ui() {
	AMyPlayerController* con = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));		// 获得 PlayerController
	FInputModeGameAndUI input;
	con->SetInputMode(input);
}
void ADGameGameModeBase::change_input_to_ui_only() {
	AMyPlayerController* con = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));		// 获得 PlayerController
	FInputModeUIOnly input;
	con->SetInputMode(input);
}


