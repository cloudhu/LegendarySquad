// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonLoggerBPLibrary.h"
#include "CommonLogger.h"

#include "Kismet/KismetSystemLibrary.h"

//Defining the CommonLogger category
DEFINE_LOG_CATEGORY( CommonLogger );

int UCommonLoggerBPLibrary::Counter = 0;
TMap<FString, int> UCommonLoggerBPLibrary::CachedLogs;
int UCommonLoggerBPLibrary::NextLogId = 0;

UCommonLoggerBPLibrary::UCommonLoggerBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}


void UCommonLoggerBPLibrary::Log(const FString& Message, const ELogVerbosity::Type Verbosity, const ECommonLoggerType On, const float Duration, int Key)
{
	//Switching based on the given verbosity 
	switch ( Verbosity ) {
	case ELogVerbosity::Error:
		//printing the log to the screen
		if ( GEngine && On != ECommonLoggerType::Console )
			GEngine->AddOnScreenDebugMessage( Key, Duration, FColor::Red, *Message );

		//(also)printing the log to the output
		if ( On != ECommonLoggerType::Screen ) UE_LOG( CommonLogger, Error, TEXT("%s"), *Message );
		break;
	case ELogVerbosity::Warning:
		//printing the log to the screen
		if ( GEngine && On != ECommonLoggerType::Console )
			GEngine->AddOnScreenDebugMessage( Key, Duration, FColor::Orange, *Message );

		//(also)printing the log to the output
		if ( On != ECommonLoggerType::Screen ) UE_LOG( CommonLogger, Warning, TEXT("%s"), *Message );
		break;
	case ELogVerbosity::Display:
		//printing the log to the screen
		if ( GEngine && On != ECommonLoggerType::Console )
			GEngine->AddOnScreenDebugMessage( Key, Duration, /*FColor::FromHex( "FFA651FF" )*/FColor::Cyan, *Message );

		//(also)printing the log to the output
		if ( On != ECommonLoggerType::Screen ) UE_LOG( CommonLogger, Display, TEXT("%s"), *Message );
		break;
	default: ;
	}
}

void UCommonLoggerBPLibrary::LogInfo(const UObject* Object, const FString& Message, const ECommonLoggerType On, const float Duration, const int Key)
{
	const auto KeyId = GetKeyIdByObject( Key, Object );
	//Calling the Log function with provided parameters
	if ( Object ) {
		Log( FString::Printf( TEXT( "[%s] %s" ), *UKismetSystemLibrary::GetDisplayName( Object ), *Message ), ELogVerbosity::Display, On, Duration, KeyId );
	} else
		Log( *Message, ELogVerbosity::Display, On, Duration, Key );
}

void UCommonLoggerBPLibrary::LogWarning(const UObject* Object, const FString& Message, const ECommonLoggerType On, const float Duration, const int Key)
{
	const auto KeyId = GetKeyIdByObject( Key, Object );
	//Calling the Log function with provided parameters
	if ( Object ) {
		Log( FString::Printf( TEXT( "[%s] %s" ), *UKismetSystemLibrary::GetDisplayName( Object ), *Message ), ELogVerbosity::Warning, On, Duration, KeyId );
	} else
		Log( *Message, ELogVerbosity::Warning, On, Duration, Key );
}

void UCommonLoggerBPLibrary::LogError(const UObject* Object, const FString& Message, const ECommonLoggerType On, const float Duration, const int Key)
{
	const auto KeyId = GetKeyIdByObject( Key, Object );
	//Calling the Log function with provided parameters
	if ( Object ) {
		Log( FString::Printf( TEXT( "[%s] %s" ), *UKismetSystemLibrary::GetDisplayName( Object ), *Message ), ELogVerbosity::Error, On, Duration, KeyId );
	} else
		Log( *Message, ELogVerbosity::Display, On, Duration, Key );
}

void UCommonLoggerBPLibrary::LogCounter(const ECommonLoggerType On, const float Duration)
{
	//Calling the Log function with provided parameters
	Log( FString::Printf( TEXT( "Counter: %d" ), Counter++ ), ELogVerbosity::Display, On, Duration );
}

int UCommonLoggerBPLibrary::GetKeyIdByObject(const int Key, const UObject* Object)
{
	if ( Key != 0 )
		return Key;

	const auto ObjectName = GetNameSafe( Object );

	const auto FoundIdPtr = CachedLogs.Find( ObjectName );

	int GeneratedKey;
	if ( FoundIdPtr == nullptr ) {
		GeneratedKey = NextLogId++;
		CachedLogs.Add( ObjectName, GeneratedKey );
	} else {
		GeneratedKey = *FoundIdPtr;
	}
	return GeneratedKey;
}

int UCommonLoggerBPLibrary::GetCounter()
{
	return Counter;
}

int UCommonLoggerBPLibrary::GetRandomKey()
{
	return FMath::Rand();
}
