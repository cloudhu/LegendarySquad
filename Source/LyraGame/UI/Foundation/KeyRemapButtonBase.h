// CloudHu:604746493@qq.com All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "LyraButtonBase.h"
#include "KeyRemapButtonBase.generated.h"

class UCommonLazyImage;
/**
 * @class UKeyRemapButtonBase
 * @brief A base class for key remapping buttons, inheriting from ULyraButtonBase. Provides functionality to set and update the button's display image, handle click events, and manage a key index.
 *
 * This class is designed to be used in settings or configurations where users can remap keys. It supports setting a custom display image, updating the icon style, and broadcasting an event when the button is clicked, passing along the associated key index.
 */
UCLASS()
class LYRAGAME_API UKeyRemapButtonBase : public ULyraButtonBase
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets the display image of the button to the provided Slate brush.
	 *
	 * This function updates the button's icon by setting the brush of the internal `CommonLazyImage_Icon` widget. After updating the brush, it also triggers an update to the icon style.
	 *
	 * @param InBrush The new FSlateBrush to be set as the button's display image.
	 */
	UFUNCTION(BlueprintCallable)
	void SetButtonDisplayImage(const FSlateBrush& InBrush);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIconStyle();

	DECLARE_EVENT_OneParam(UWidget_KeyRemapButtonBase, FOnKeyRemapClicked, uint8);

	FOnKeyRemapClicked OnKeyRemapClicked;

	void SetKeyIndex(const uint8 Index);

protected:

	virtual void NativeOnClicked() override;
	
	virtual void NativeDestruct() override;

private:
	//desc which key slot we are mess with
	uint8 KeyIndex = 0;

	// key icon to show which key user can remap
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonLazyImage> CommonLazyImage_Icon;
};
