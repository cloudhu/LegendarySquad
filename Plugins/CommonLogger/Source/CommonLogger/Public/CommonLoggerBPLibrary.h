// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonLoggerBPLibrary.generated.h"

DECLARE_LOG_CATEGORY_EXTERN( CommonLogger, All, All );

UENUM( BlueprintType, Category="Common Logger" )
enum class ECommonLoggerType :uint8 {
	Screen = 0 UMETA( DisplayName = "Screen" ),
	Console = 1 UMETA( DisplayName = "Console" ),
	Both = 2 UMETA( DisplayName = "Screen and Console" ),
};

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class UCommonLoggerBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	static void Log(const FString& Message, const ELogVerbosity::Type Verbosity, const ECommonLoggerType On, const float Duration, int Key = 0);

	//Logging info message with Object's DisplayName to the desired channel. Screen, Console or both
	UFUNCTION( BlueprintCallable, Category = "Easy Logger", meta=(DevelopmentOnly) )
	static void LogInfo(const UObject* Object, const FString& Message, const ECommonLoggerType On = ECommonLoggerType::Both, const float Duration = 3.0f, const int Key = 0);

	//Logging warning message with Object's DisplayName to the desired channel. Screen, Console or both
	UFUNCTION( BlueprintCallable, Category = "Easy Logger", meta=(DevelopmentOnly) )
	static void LogWarning(const UObject* Object, const FString& Message, const ECommonLoggerType On = ECommonLoggerType::Both, const float Duration = 5.0f, const int Key = 0);

	//Logging error message with Object's DisplayName to the desired channel. Screen, Console or both
	UFUNCTION( BlueprintCallable, Category = "Easy Logger", meta=(DevelopmentOnly) )
	static void LogError(const UObject* Object, const FString& Message, const ECommonLoggerType On = ECommonLoggerType::Both, const float Duration = 10.0f, const int Key = 0);

	UFUNCTION( BlueprintCallable, Category = "Easy Logger", meta=(DevelopmentOnly) )
	static void LogCounter(const ECommonLoggerType On = ECommonLoggerType::Both, const float Duration = 10.0f);

	UFUNCTION( BlueprintCallable, Category = "Easy Logger", meta=(DevelopmentOnly) )
	static int GetKeyIdByObject(const int Key, const UObject* Object);

	static int GetCounter();

	static int GetRandomKey();

private:
	static int Counter;
	static TMap<FString, int> CachedLogs;
	static int NextLogId;
};
