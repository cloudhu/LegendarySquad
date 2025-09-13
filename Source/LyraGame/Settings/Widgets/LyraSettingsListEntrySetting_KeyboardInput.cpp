// Copyright Epic Games, Inc. All Rights Reserved.

#include "Settings/Widgets/LyraSettingsListEntrySetting_KeyboardInput.h"

#include "CommonUIExtensions.h"
#include "NativeGameplayTags.h"
#include "Settings/CustomSettings/LyraSettingKeyboardInput.h"

#include "UI/Foundation/KeyRemapButtonBase.h"
#include "UI/Foundation/LyraButtonBase.h"
#include "Widgets/Misc/GameSettingPressAnyKey.h"
#include "Widgets/Misc/KeyAlreadyBoundWarning.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraSettingsListEntrySetting_KeyboardInput)

#define LOCTEXT_NAMESPACE "LyraSettings"

UE_DEFINE_GAMEPLAY_TAG_STATIC(PressAnyKeyLayer, "UI.Layer.Modal");

void ULyraSettingsListEntrySetting_KeyboardInput::SetSetting(UGameSetting* InSetting)
{
	KeyboardInputSetting = CastChecked<ULyraSettingKeyboardInput>(InSetting);

	Super::SetSetting(InSetting);

	Refresh();
}

void ULyraSettingsListEntrySetting_KeyboardInput::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Button_PrimaryKey->OnKeyRemapClicked.AddUObject(this, &ThisClass::HandleRemapKeyClicked);
	Button_SecondaryKey->OnKeyRemapClicked.AddUObject(this, &ThisClass::HandleRemapKeyClicked);
	ButtonArray.Add(Button_PrimaryKey);
	ButtonArray.Add(Button_SecondaryKey);
	Button_Clear->OnClicked().AddUObject(this, &ThisClass::HandleClearClicked);
	Button_ResetToDefault->OnClicked().AddUObject(this, &ThisClass::HandleResetToDefaultClicked);
}

void ULyraSettingsListEntrySetting_KeyboardInput::ChangeBinding(const uint8 InBindSlot, const FKey& InKey)
{
	OriginalKeyToBind = InKey;
	TArray<FName> ActionsForKey;
	KeyboardInputSetting->GetAllMappedActionsFromKey(InKey, ActionsForKey);
	if (!ActionsForKey.IsEmpty())
	{
		UKeyAlreadyBoundWarning* KeyAlreadyBoundWarning = CastChecked<UKeyAlreadyBoundWarning>(
			UCommonUIExtensions::PushContentToLayer_ForPlayer(GetOwningLocalPlayer(), PressAnyKeyLayer, KeyAlreadyBoundWarningPanelClass));
		KeyAlreadyBoundWarning->SetKeyIndex(InBindSlot);

		FString ActionNames;
		for (FName ActionName : ActionsForKey)
		{
			ActionNames += ActionName.ToString() += ", ";
		}

		FFormatNamedArguments Args;
		Args.Add(TEXT("InKey"), InKey.GetDisplayName());
		Args.Add(TEXT("ActionNames"), FText::FromString(ActionNames));

		KeyAlreadyBoundWarning->SetWarningText(
			FText::Format(LOCTEXT("WarningText", "{InKey} is already bound to {ActionNames} are you sure you want to rebind it?"), Args));
		KeyAlreadyBoundWarning->SetCancelText(
			FText::Format(LOCTEXT("CancelText", "Press escape to cancel, or press {InKey} again to confirm rebinding."), Args));

		KeyAlreadyBoundWarning->OnKeySelected.AddUObject(this, &ThisClass::HandleDuplicateKeySelected, KeyAlreadyBoundWarning);
		KeyAlreadyBoundWarning->OnKeySelectionCanceled.AddUObject(this, &ThisClass::HandleKeySelectionCanceled, KeyAlreadyBoundWarning);
	}
	else
	{
		KeyboardInputSetting->ChangeBinding(InBindSlot, InKey);
	}
}

void ULyraSettingsListEntrySetting_KeyboardInput::HandleKeySelectionCanceled(UGameSettingPressAnyKey* PressAnyKeyPanel) const
{
	PressAnyKeyPanel->OnKeySelectionCanceled.RemoveAll(this);
}

void ULyraSettingsListEntrySetting_KeyboardInput::HandleKeySelectionCanceled(UKeyAlreadyBoundWarning* PressAnyKeyPanel) const
{
	PressAnyKeyPanel->OnKeySelectionCanceled.RemoveAll(this);
}

void ULyraSettingsListEntrySetting_KeyboardInput::HandleClearClicked() const
{
	if (KeyboardInputSetting->GetDesiredInputKeyType()==ECommonInputType::MouseAndKeyboard)
	{
		KeyboardInputSetting->ChangeBinding(0, EKeys::Invalid);
		KeyboardInputSetting->ChangeBinding(1, EKeys::Invalid);
	}
	else
	{
		KeyboardInputSetting->ChangeBinding(2, EKeys::Invalid);
		KeyboardInputSetting->ChangeBinding(3, EKeys::Invalid);
	}
}

void ULyraSettingsListEntrySetting_KeyboardInput::HandleResetToDefaultClicked() const
{
	KeyboardInputSetting->ResetToDefault();
}

void ULyraSettingsListEntrySetting_KeyboardInput::HandleRemapKeySelected(FKey InKey, uint8 SlotID, UGameSettingPressAnyKey* PressAnyKeyPanel)
{
	PressAnyKeyPanel->OnKeySelected.RemoveAll(this);
	ChangeBinding(SlotID, InKey);
}

void ULyraSettingsListEntrySetting_KeyboardInput::HandleDuplicateKeySelected(FKey InKey, uint8 SlotID,
                                                                             UKeyAlreadyBoundWarning* DuplicateKeyPressAnyKeyPanel) const
{
	DuplicateKeyPressAnyKeyPanel->OnKeySelected.RemoveAll(this);
	KeyboardInputSetting->ChangeBinding(SlotID, OriginalKeyToBind);
}

void ULyraSettingsListEntrySetting_KeyboardInput::OnSettingChanged()
{
	Refresh();
}

void ULyraSettingsListEntrySetting_KeyboardInput::HandleRemapKeyClicked(uint8 SlotID)
{
	UGameSettingPressAnyKey* PressAnyKeyPanel = CastChecked<UGameSettingPressAnyKey>(
		UCommonUIExtensions::PushContentToLayer_ForPlayer(GetOwningLocalPlayer(), PressAnyKeyLayer, PressAnyKeyPanelClass));
	PressAnyKeyPanel->SetKeyIndex(SlotID);
	PressAnyKeyPanel->OnKeySelected.AddUObject(this, &ThisClass::HandleRemapKeySelected, PressAnyKeyPanel);
	PressAnyKeyPanel->OnKeySelectionCanceled.AddUObject(this, &ThisClass::HandleKeySelectionCanceled, PressAnyKeyPanel);
}

void ULyraSettingsListEntrySetting_KeyboardInput::Refresh() const
{
	if (ensure(KeyboardInputSetting))
	{
		const FText NotBound = LOCTEXT("NotBound", "Not Bound");
		Button_PrimaryKey->SetButtonText(NotBound);
		Button_SecondaryKey->SetButtonText(NotBound);
		int Index = 0;
		FSlateBrush FoundBrush;
		int PrevIndex = 0;
		for (TPair Pair : KeyboardInputSetting->GetKeyMappings())
		{
			if (Pair.Value.IsValid())
			{
				if (const FKey& Key = KeyboardInputSetting->GetCurrentKey(Pair.Key); KeyboardInputSetting->GetIconFromKey(FoundBrush, Key))
				{
					ButtonArray[Index]->SetButtonDisplayImage(FoundBrush);
				}
				else
				{
					ButtonArray[Index]->SetButtonText(Key.GetDisplayName());
				}
				PrevIndex = static_cast<int32>(Pair.Key);
				ButtonArray[Index]->SetKeyIndex(PrevIndex);
				ButtonArray[Index]->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				ButtonArray[Index]->SetVisibility(ESlateVisibility::Collapsed);
			}
			Index++;
			if (Index > ButtonArray.Num() - 1)
			{
				Index = 0;
			}
		}
		// if this Index is 1,Button_SecondaryKey is not bound
		if (Index == 1)
		{
			Button_SecondaryKey->SetKeyIndex(PrevIndex + 2);
		}
		// Only display the reset to default button if a mapping is customized
		if (ensure(Button_ResetToDefault))
		{
			if (KeyboardInputSetting->IsMappingCustomized())
			{
				Button_ResetToDefault->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				Button_ResetToDefault->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void ULyraSettingsListEntrySetting_KeyboardInput::NativeOnEntryReleased()
{
	Super::NativeOnEntryReleased();

	KeyboardInputSetting = nullptr;
}

#undef LOCTEXT_NAMESPACE
