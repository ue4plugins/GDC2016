// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "RpcMessage.h"
#include "UObject/ObjectMacros.h"

#include "RpcMessages.generated.h"


/* Service discovery
 *****************************************************************************/

USTRUCT()
struct FConfiguratorPing
{
	GENERATED_BODY()
};


USTRUCT()
struct FConfiguratorPong
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Message")
	FString ServerAddress;

	FConfiguratorPong() { }
	FConfiguratorPong(const FString& InServerAddress)
		: ServerAddress(InServerAddress)
	{ }
};


/* Command
*****************************************************************************/

UENUM()
enum class EConfiguratorCommand : uint8
{
	Reset,
	Tap,
	UpdateRotation,
	UpdateSelectedItem,
	UpdateVisualization,
	Zoom
};


USTRUCT()
struct FConfiguratorCommandRequest
	: public FRpcMessage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Message")
	EConfiguratorCommand Command;

	UPROPERTY(EditAnywhere, Category="Message")
	int32 CategoryIndex;

	UPROPERTY(EditAnywhere, Category="Message")
	int32 ItemIndex;

	UPROPERTY(EditAnywhere, Category="Message")
	int32 ModeIndex;

	UPROPERTY(EditAnywhere, Category="Message")
	float Pitch;

	UPROPERTY(EditAnywhere, Category="Message")
	float Yaw;

	FConfiguratorCommandRequest(
		EConfiguratorCommand InCommand,
		int32 InCategoryIndex,
		int32 InItemIndex,
		int32 InModeIndex,
		float InPitch,
		float InYaw
	)
		: Command(InCommand)
		, CategoryIndex(InCategoryIndex)
		, ItemIndex(InItemIndex)
		, ModeIndex(InModeIndex)
		, Pitch(InPitch)
		, Yaw(InYaw)
	{ }

	FConfiguratorCommandRequest() { }
};


USTRUCT()
struct FConfiguratorCommandResponse
	: public FRpcMessage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Message")
	int32 Result;

	FConfiguratorCommandResponse()
		: Result(INDEX_NONE)
	{ }

	FConfiguratorCommandResponse(int32 InResult)
		: Result(InResult)
	{ }
};


DECLARE_RPC(FConfiguratorCommand, int32)
