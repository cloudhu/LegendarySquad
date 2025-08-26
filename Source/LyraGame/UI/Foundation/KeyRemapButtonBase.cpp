// CloudHu:604746493@qq.com All Rights Reserved


#include "KeyRemapButtonBase.h"

#include "CommonLazyImage.h"

void UKeyRemapButtonBase::SetButtonDisplayImage(const FSlateBrush& InBrush)
{
	if (CommonLazyImage_Icon.Get())
	{
		CommonLazyImage_Icon.Get()->SetBrush(InBrush);
		UpdateIconStyle();
	}
}

void UKeyRemapButtonBase::SetKeyIndex(const int32 Index)
{
	KeyIndex = Index;
}

void UKeyRemapButtonBase::NativeOnClicked()
{
	Super::NativeOnClicked();
	if (OnKeyRemapClicked.IsBound())
	{
		OnKeyRemapClicked.Broadcast(KeyIndex);
	}
}

void UKeyRemapButtonBase::NativeDestruct()
{
	Super::NativeDestruct();
	if (OnKeyRemapClicked.IsBound())
	{
		OnKeyRemapClicked.Clear();
	}
}
