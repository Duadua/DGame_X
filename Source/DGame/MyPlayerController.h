#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

// 控制模式
UENUM(BlueprintType)
enum class EControllerState :uint8 {				// 设置uint8类型
	E_FREE   UMETA(DisplayName = "自在视角"),		//想要显示中文 需要将编码格式设置为utf-8    
	E_SOLID  UMETA(DisplayName = "固定视角"),		//DisPlayName表示显示的名称，在蓝图中可见 
	E_THREED UMETA(DisplayName = "3D模式"),			//DisPlayName表示显示的名称，在蓝图中可见 
	E_ACT    UMETA(DisplayName = "动作模式"),		//DisPlayName表示显示的名称，在蓝图中可见 
};

UCLASS()
class DGAME_API AMyPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	AMyPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight", meta = (AllowPrivateAccess = "true"))
	uint32 b_attack_q        : 1; // 是否释放q技能 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight", meta = (AllowPrivateAccess = "true"))
	uint32 b_attack_w        : 1; // 是否释放w技能 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight", meta = (AllowPrivateAccess = "true"))
	uint32 b_attack_e        : 1; // 是否释放e技能 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight", meta = (AllowPrivateAccess = "true"))
	uint32 b_attack_r        : 1; // 是否释放r技能 

	UFUNCTION(BlueprintCallable)
	bool get_b_attack();			// 是否处于攻击状态


	// 控制状态变量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controller, meta = (AllowPrivateAccess = "true"))
	EControllerState controller_state;
	// 更新控制状态 -- 根据当前状态
	void update_controller_state();

	// 战斗状态变量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight", meta = (AllowPrivateAccess = "true"))
	uint32 fight_state : 1;
	// 更新战斗状态 -- 根据当前状态
	UFUNCTION(BlueprintCallable)
	void update_fight_state();


protected:
	virtual void BeginPlay()                 override;	// 游戏开始时的初始化操作 
	virtual void PlayerTick(float DeltaTime) override;	// 玩家每帧更新的事件   
	virtual void SetupInputComponent()       override;	// 设置输入组件      

	// 鼠标点击移动事件
	void on_set_destination_pressed();
	void on_set_destination_released();
	void move_to_cursor();                                                                    
	void set_new_move_destination(const FVector DestLocation);                                

	// wasd移动事件
	void on_move_forward(float value);
	void on_move_right(float value);
	// run事件
	void on_run_start();
	void on_run_stop();
	// 攻击事件
	void on_attack_q_pressed();
	void on_attack_q_released();
	void on_attack_w_pressed();
	void on_attack_w_released();
	void on_attack_e_pressed();
	void on_attack_e_released();
	void on_attack_r_pressed();
	void on_attack_r_released();
	

	// 视角移动事件
	void on_can_rotate_pressed();     
	void on_can_rotate_released();    
	void on_turn_rate(float value);   
	void on_lookup_rate(float value); 

	// 视角缩放事件
	void on_scale(float value);

	// 动作模式下呼出鼠标
	void on_call_mouse_pressed();
	void on_call_mouse_released();

private:

	uint32 b_move_to_cursor : 1;				// 人物移动到鼠标标记


	// 视角移动相关
	uint32 b_can_rotate : 1;					// 是否可以移动视角
	// 视角移动速度
	float turn_rate;
	float lookup_rate;
	// 视角缩放速度
	float scale_rate;
	float scale_min;
	float scale_max;
};
