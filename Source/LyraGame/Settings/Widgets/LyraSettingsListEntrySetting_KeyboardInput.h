// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Widgets/GameSettingListEntry.h"

#include "LyraSettingsListEntrySetting_KeyboardInput.generated.h"

class UKeyRemapButtonBase;
class UKeyAlreadyBoundWarning;

class UGameSetting;
class UGameSettingPressAnyKey;
class ULyraButtonBase;
class ULyraSettingKeyboardInput;
class UObject;

//////////////////////////////////////////////////////////////////////////
// ULyraSettingsListEntrySetting_KeyboardInput
//////////////////////////////////////////////////////////////////////////

UCLASS(Abstract, Blueprintable, meta = (Category = "Settings", DisableNativeTick))
class ULyraSettingsListEntrySetting_KeyboardInput : public UGameSettingListEntry_Setting
{
	GENERATED_BODY()

public:
	virtual void SetSetting(UGameSetting* InSetting) override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnEntryReleased() override;
	virtual void OnSettingChanged() override;

	void HandleRemapKeyClicked(uint8 SlotID);
	void HandleClearClicked() const;
	void HandleResetToDefaultClicked() const;

	void HandleRemapKeySelected(FKey InKey, uint8 SlotID, UGameSettingPressAnyKey* PressAnyKeyPanel);

	void HandleDuplicateKeySelected(FKey InKey, uint8 SlotID, UKeyAlreadyBoundWarning* DuplicateKeyPressAnyKeyPanel) const;
	
	void ChangeBinding(uint8 InBindSlot, const FKey& InKey);
	
	void HandleKeySelectionCanceled(UGameSettingPressAnyKey* PressAnyKeyPanel) const;
	void HandleKeySelectionCanceled(UKeyAlreadyBoundWarning* PressAnyKeyPanel) const;

	void Refresh() const;

private:
	UPROPERTY(Transient)
	FKey OriginalKeyToBind = EKeys::Invalid;

protected:
	UPROPERTY()
	TObjectPtr<ULyraSettingKeyboardInput> KeyboardInputSetting;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameSettingPressAnyKey> PressAnyKeyPanelClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UKeyAlreadyBoundWarning> KeyAlreadyBoundWarningPanelClass;

private:	// Bound Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UKeyRemapButtonBase> Button_PrimaryKey;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UKeyRemapButtonBase> Button_SecondaryKey;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UKeyRemapButtonBase> Button_ThirdKey;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UKeyRemapButtonBase> Button_FourthKey;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<ULyraButtonBase> Button_Clear;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<ULyraButtonBase> Button_ResetToDefault;

	TArray<TObjectPtr<UKeyRemapButtonBase>> ButtonArray;
};
