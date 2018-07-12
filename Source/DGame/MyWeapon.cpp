#include "MyWeapon.h"

UMyWeapon::UMyWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
}



void UMyWeapon::set_attach_point(USceneComponent* parent, FName name) {
	attach_point = name;
	AttachToComponent(parent, FAttachmentTransformRules::SnapToTargetIncludingScale, attach_point);
}