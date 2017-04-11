// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreTypes.h"
#include "Delegates/Delegate.h"
#include "GameFramework/GameMode.h"
#include "IMessageContext.h"
#include "Misc/DateTime.h"
#include "UObject/ScriptMacros.h"

#include "CompanionGameMode.generated.h"

class FMessageEndpoint;
class IMessageRpcClient;

struct FConfiguratorPong;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfiguratorFound);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfiguratorLost);


UCLASS()
class ACompanionGameMode
	: public AGameMode
{
	GENERATED_BODY()

public:

	/** Virtual destructor. */
	virtual ~ACompanionGameMode();

public:

	//~ AGameMode interface

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

public:

	/** Event delegate that is called when the configurator has been found. */
	UPROPERTY(BlueprintAssignable, Category="GDC2016|CompanionGameMode")
	FOnConfiguratorFound OnConfiguratorFound;

	/** Event delegate that is called when the configurator has been lost. */
	UPROPERTY(BlueprintAssignable, Category="GDC2016|CompanionGameMode")
	FOnConfiguratorLost OnConfiguratorLost;

public:

	/** Whether a connection to the configurator has been established. */
	UFUNCTION(BlueprintCallable, Category="GDC2016|CompanionGameMode")
	bool IsConnected() const;

	/** Reset the view. */
	UFUNCTION(BlueprintCallable, Category="GDC2016|CompanionGameMode")
	void Reset();

	/** Tap the view. */
	UFUNCTION(BlueprintCallable, Category="GDC2016|CompanionGameMode")
	void Tap();

	/** Update the car's rotation. */
	UFUNCTION(BlueprintCallable, Category="GDC2016|CompanionGameMode")
	void UpdateRotation(float Yaw, float Pitch);

	/** Update the car's selected item. */
	UFUNCTION(BlueprintCallable, Category="GDC2016|CompanionGameMode")
	void UpdateSelectedItem(int32 CategoryIndex, int32 ItemIndex);

	/** Update the visualization mode. */
	UFUNCTION(BlueprintCallable, Category="GDC2016|CompanionGameMode")
	void UpdateVisualization(int32 ModeIndex);

	/** Zoom the view. */
	UFUNCTION(BlueprintCallable, Category="GDC2016|CompanionGameMode")
	void Zoom();

private:

	void HandleMessage(const FConfiguratorPong& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);
	bool HandleTicker(float DeltaTime);

private:

	FMessageAddress ConfiguratorAddress;
	FDateTime LastConfiguratorResponse;
	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> MessageEndpoint;
	TSharedPtr<IMessageRpcClient> RpcClient;
	FDelegateHandle TickDelegateHandle;
};
