#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

// ����ģʽ
UENUM(BlueprintType)
enum class EControllerState :uint8 {				// ����uint8����
	E_FREE   UMETA(DisplayName = "�����ӽ�"),		//��Ҫ��ʾ���� ��Ҫ�������ʽ����Ϊutf-8    
	E_SOLID  UMETA(DisplayName = "�̶��ӽ�"),		//DisPlayName��ʾ��ʾ�����ƣ�����ͼ�пɼ� 
	E_THREED UMETA(DisplayName = "3Dģʽ"),			//DisPlayName��ʾ��ʾ�����ƣ�����ͼ�пɼ� 
	E_ACT    UMETA(DisplayName = "����ģʽ"),		//DisPlayName��ʾ��ʾ�����ƣ�����ͼ�пɼ� 
};

UCLASS()
class DGAME_API AMyPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	AMyPlayerController();

	UFUNCTION(BlueprintCallable, Category = "Run")
	bool get_b_run() { return b_run; }

	// ����״̬����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controller)
	EControllerState controller_state;
	// ���¿���״̬ -- ���ݵ�ǰ״̬
	void update_controller_state();

	// ս��״̬����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fight")
	uint32 fight_state : 1;
	// ����ս��״̬ -- ���ݵ�ǰ״̬
	UFUNCTION(BlueprintCallable)
	void update_fight_state();

protected:
	virtual void BeginPlay()                 override;	// ��Ϸ��ʼʱ�ĳ�ʼ������ 
	virtual void PlayerTick(float DeltaTime) override;	// ���ÿ֡���µ��¼�   
	virtual void SetupInputComponent()       override;	// �����������      

	void move_to_cursor();                                                                    
	void move_to_touch_location(const ETouchIndex::Type FingerIndex, const FVector Location); 
	void set_new_move_destination(const FVector DestLocation);                                

	// ������ƶ��¼�
	void on_reset_VR();
	void on_set_destination_pressed();
	void on_set_destination_released();

	// wasd�ƶ��¼�
	void on_move_forward(float value);
	void on_move_right(float value);
	// run�¼�
	void on_run_start();
	void on_run_stop();

	// �ӽ��ƶ��¼�
	void on_can_rotate_pressed();     
	void on_can_rotate_released();    
	void on_turn_rate(float value);   
	void on_lookup_rate(float value); 

	// �ӽ������¼�
	void on_scale(float value);

private:

	uint32 b_move_to_cursor : 1;				// �����ƶ��������

	uint32 b_run : 1;							// �Ƿ��ܲ�

	// �ӽ��ƶ����
	uint32 b_can_rotate : 1;					// �Ƿ�����ƶ��ӽ�
	// �ӽ��ƶ��ٶ�
	float turn_rate;
	float lookup_rate;
	// �ӽ������ٶ�
	float scale_rate;
	float scale_min;
	float scale_max;
};