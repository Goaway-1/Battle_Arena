#include "DamageTextWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Components/TextBlock.h"

void UDamageTextWidget::NativeConstruct() {
	Super::NativeConstruct();

	SetPositionInViewport(InintialScreenLocation);

	FinalScreenLocation.X = InintialScreenLocation.X + UKismetMathLibrary::RandomFloatInRange(-150.f,150.f);
	FinalScreenLocation.Y = InintialScreenLocation.Y + UKismetMathLibrary::RandomFloatInRange(-300.f,-150.f);
	
	TEnumAsByte<ERoundingMode> Mode;

	DamageText->SetText(UKismetTextLibrary::Conv_FloatToText(DamageToDisplay, Mode));
	PlayAnimation(Fade);
}

void UDamageTextWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime){
	Super::NativeTick(MyGeometry, InDeltaTime);

	InintialScreenLocation = UKismetMathLibrary::Vector2DInterpTo(InintialScreenLocation,FinalScreenLocation,InDeltaTime,1.0f);
	SetPositionInViewport(InintialScreenLocation);
}

void UDamageTextWidget::SetInitialSetting(FVector2D& vec, float& name) {
	InintialScreenLocation = vec;
	DamageToDisplay = name;
	this->AddToViewport();
}