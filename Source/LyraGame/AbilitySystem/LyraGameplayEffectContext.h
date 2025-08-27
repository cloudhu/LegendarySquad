// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "GameplayEffectTypes.h"

#include "AbilitySystem/Types/FNinjaCombatGameplayEffectContext.h"


#include "LyraGameplayEffectContext.generated.h"

class AActor;
class FArchive;
class ILyraAbilitySourceInterface;
class UObject;
class UPhysicalMaterial;

USTRUCT()
struct FLyraGameplayEffectContext : public FNinjaCombatGameplayEffectContext
{
	GENERATED_BODY()

	FLyraGameplayEffectContext():FNinjaCombatGameplayEffectContext()
	{
	}

	FLyraGameplayEffectContext(AActor* InInstigator, AActor* InEffectCauser)
		: FNinjaCombatGameplayEffectContext(InInstigator, InEffectCauser)
	{
	}

	/** Returns the wrapped FLyraGameplayEffectContext from the handle, or nullptr if it doesn't exist or is the wrong type */
	static LYRAGAME_API FLyraGameplayEffectContext* ExtractEffectContext(FGameplayEffectContextHandle Handle);

	/** Sets the object used as the ability source */
	void SetAbilitySource(const ILyraAbilitySourceInterface* InObject, float InSourceLevel);

	/** Returns the ability source interface associated with the source object. Only valid on the authority. */
	const ILyraAbilitySourceInterface* GetAbilitySource() const;

	virtual FGameplayEffectContext* Duplicate() const override
	{
		return Super::Duplicate();
	}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return Super::StaticStruct();
	}

	/** Overridden to serialize new fields */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	/** Returns the physical material from the hit result if there is one */
	const UPhysicalMaterial* GetPhysicalMaterial() const;

public:
	/** ID to allow the identification of multiple bullets that were part of the same cartridge */
	UPROPERTY()
	int32 CartridgeID = -1;

protected:
	/** Ability Source object (should implement ILyraAbilitySourceInterface). NOT replicated currently */
	UPROPERTY()
	TWeakObjectPtr<const UObject> AbilitySourceObject;
};

template<>
struct TStructOpsTypeTraits<FLyraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FLyraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};

