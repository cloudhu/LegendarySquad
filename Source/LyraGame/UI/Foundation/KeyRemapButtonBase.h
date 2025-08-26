// CloudHu:604746493@qq.com All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "LyraButtonBase.h"
#include "KeyRemapButtonBase.generated.h"

class UCommonLazyImage;
/**
 * 
 */
UCLASS()
class LYRAGAME_API UKeyRemapButtonBase : public ULyraButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtonDisplayImage(const FSlateBrush& InBrush);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIconStyle();

	DECLARE_EVENT_OneParam(UWidget_KeyRemapButtonBase, FOnKeyRemapClicked, int32);

	FOnKeyRemapClicked OnKeyRemapClicked;

	void SetKeyIndex(int32 Index);

protected:

	virtual void NativeOnClicked() override;
	
	virtual void NativeDestruct() override;

private:
	int32 KeyIndex = 0;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonLazyImage> CommonLazyImage_Icon;
};
