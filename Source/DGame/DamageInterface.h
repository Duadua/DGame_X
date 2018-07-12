#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageInterface.generated.h"

UINTERFACE(BlueprintType)
class UDamageInterface : public UInterface {
	GENERATED_UINTERFACE_BODY()
};

class DGAME_API IDamageInterface {
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void DoDamage(float delta);
};
