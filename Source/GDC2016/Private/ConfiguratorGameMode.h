// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AsyncResult.h"
#include "GameFramework/GameMode.h"

#include "ConfiguratorGameMode.generated.h"

class FMessageEndpoint;
class IMessageContext;
class IMessageRpcServer;

struct FConfiguratorPing;
struct FConfiguratorCommandRequest;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfiguratorEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConfiguratorUpdateRotation, float, Yaw, float, Pitch);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConfiguratorUpdateSelectedItem, int32, CategoryIndex, int32, ItemIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfiguratorUpdateVisualization, int32, ModeIndex);


UCLASS()
class AConfiguratorGameMode
	: public AGameMode
{
	GENERATED_BODY()

public:

	/** Event delegate that is called when the view should be reset. */
	UPROPERTY(BlueprintAssignable, Category="GDC2016|ConfiguratorGameMode")
	FOnConfiguratorEvent OnReset;

	/** Event delegate that is called when the view has been tapped. */
	UPROPERTY(BlueprintAssignable, Category="GDC2016|ConfiguratorGameMode")
	FOnConfiguratorEvent OnTap;

	/** Event delegate that is called when the car's rotation should be updated. */
	UPROPERTY(BlueprintAssignable, Category="GDC2016|ConfiguratorGameMode")
	FOnConfiguratorUpdateRotation OnUpdateRotation;

	/** Event delegate that is called when the car's selected item should be updated. */
	UPROPERTY(BlueprintAssignable, Category="GDC2016|ConfiguratorGameMode")
	FOnConfiguratorUpdateSelectedItem OnUpdateSelectedItem;

	/** Event delegate that is called when the visualization should be updated. */
	UPROPERTY(BlueprintAssignable, Category="GDC2016|ConfiguratorGameMode")
	FOnConfiguratorUpdateVisualization OnUpdateVisualization;

	/** Event delegate that is called when the view should be zoomed. */
	UPROPERTY(BlueprintAssignable, Category="GDC2016|ConfiguratorGameMode")
	FOnConfiguratorEvent OnZoom;

public:

	//~ AGameMode interface

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

private:

	void HandleMessage(const FConfiguratorPing& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);
	TAsyncResult<int32> HandleRequest(const FConfiguratorCommandRequest& Message);

private:

	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> MessageEndpoint;
	TSharedPtr<IMessageRpcServer> RpcServer;
};
