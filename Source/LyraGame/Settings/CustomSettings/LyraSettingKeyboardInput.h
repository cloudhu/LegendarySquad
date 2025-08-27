// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameSettingValue.h"
#include "UserSettings/EnhancedInputUserSettings.h"

#include "LyraSettingKeyboardInput.generated.h"

enum class ECommonInputType : uint8;
class UObject;

//--------------------------------------
// ULyraSettingKeyboardInput
//--------------------------------------

UCLASS()
class ULyraSettingKeyboardInput : public UGameSettingValue
{
	GENERATED_BODY()

public:
	ULyraSettingKeyboardInput();

	void InitializeInputData(const UEnhancedPlayerMappableKeyProfile* KeyProfile, const FPlayerKeyMapping& Mapping ,
	                         const FPlayerMappableKeyQueryOptions& InQueryOptions);

	FText GetKeyTextFromSlot(const EPlayerMappableKeySlot InSlot) const;

	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;

	bool ChangeBinding(uint8 InKeyBindSlot, const FKey& NewKey);
	void GetAllMappedActionsFromKey(FKey Key, TArray<FName>& OutActionNames) const;

	/** Returns true if mappings on this setting have been customized */
	bool IsMappingCustomized() const;

	FText GetSettingDisplayName() const;
	FText GetSettingDisplayCategory() const;

	const FKeyMappingRow* FindKeyMappingRow() const;
	UEnhancedPlayerMappableKeyProfile* FindMappableKeyProfile() const;
	UEnhancedInputUserSettings* GetUserSettings() const;

	const FKey& GetCurrentKey(const EPlayerMappableKeySlot InSlot) const;
	bool GetIconFromKey(FSlateBrush& OutBrush, const FKey& InKey) const;

	FORCEINLINE ECommonInputType GetDesiredInputKeyType() const { return CachedDesiredInputKeyType; }
	FORCEINLINE TMap<EPlayerMappableKeySlot, FKey> GetKeyMappings() const { return InitialKeyMappings; }

protected:
	/** ULyraSetting */
	virtual void OnInitialized() override;


	ECommonInputType CachedDesiredInputKeyType;

	/** The name of this action's mappings */
	FName ActionMappingName;

	/** The query options to filter down keys on this setting for */
	FPlayerMappableKeyQueryOptions QueryOptions;

	/** The profile identifier that this key setting is from */
	FString ProfileIdentifier;

	/** Store the initial key mappings that are set on this for each slot */
	TMap<EPlayerMappableKeySlot, FKey> InitialKeyMappings;
};
