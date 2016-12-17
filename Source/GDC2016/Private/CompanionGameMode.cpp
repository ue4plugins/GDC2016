// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "CompanionGameMode.h"

#include "IMessageRpcClient.h"
#include "IMessagingRpcModule.h"
#include "RpcMessages.h"
#include "Containers/Ticker.h"
#include "Helpers/MessageEndpointBuilder.h"


void ACompanionGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	MessageEndpoint = FMessageEndpoint::Builder("ACompanionGameMode")
		.Handling<FConfiguratorPong>(this, &ACompanionGameMode::HandleMessage);

	RpcClient = FModuleManager::LoadModuleChecked<IMessagingRpcModule>("MessagingRpc").CreateRpcClient();
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &ACompanionGameMode::HandleTicker), 1.0f);
}


ACompanionGameMode::~ACompanionGameMode()
{
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);

	if (RpcClient.IsValid())
	{
		RpcClient->Disconnect();
	}
}


bool ACompanionGameMode::IsConnected() const
{
	return ConfiguratorAddress.IsValid();
}


void ACompanionGameMode::Reset()
{
	RpcClient->Call<FConfiguratorCommand>(EConfiguratorCommand::Reset, 0, 0, 0, 0.0f, 0.0f);
}


void ACompanionGameMode::Tap()
{
	RpcClient->Call<FConfiguratorCommand>(EConfiguratorCommand::Tap, 0, 0, 0, 0.0f, 0.0f);
}


void ACompanionGameMode::UpdateRotation(float Yaw, float Pitch)
{
	RpcClient->Call<FConfiguratorCommand>(EConfiguratorCommand::UpdateRotation, 0, 0, 0, Pitch, Yaw);
}


void ACompanionGameMode::UpdateSelectedItem(int32 CategoryIndex, int32 ItemIndex)
{
	RpcClient->Call<FConfiguratorCommand>(EConfiguratorCommand::UpdateSelectedItem, CategoryIndex, ItemIndex, 0, 0.0f, 0.0f);
}


void ACompanionGameMode::UpdateVisualization(int32 ModeIndex)
{
	RpcClient->Call<FConfiguratorCommand>(EConfiguratorCommand::UpdateVisualization, 0, 0, ModeIndex, 0.0f, 0.0f);
}


void ACompanionGameMode::Zoom()
{
	RpcClient->Call<FConfiguratorCommand>(EConfiguratorCommand::Zoom, 0, 0, 0, 0.0f, 0.0f);
}


void ACompanionGameMode::HandleMessage(const FConfiguratorPong& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	FMessageAddress NewServerAddress;

	if (FMessageAddress::Parse(Message.ServerAddress, NewServerAddress) && (NewServerAddress != ConfiguratorAddress))
	{
		ConfiguratorAddress = NewServerAddress;
		RpcClient->Connect(NewServerAddress);
		OnConfiguratorFound.Broadcast();
	}

	LastConfiguratorResponse = FDateTime::UtcNow();
}


bool ACompanionGameMode::HandleTicker(float DeltaTime)
{
	if (ConfiguratorAddress.IsValid() && ((FDateTime::UtcNow() - LastConfiguratorResponse).GetTotalSeconds() > 4.0))
	{
		ConfiguratorAddress.Invalidate();
		OnConfiguratorLost.Broadcast();
	}

	MessageEndpoint->Publish(new FConfiguratorPing, EMessageScope::Network);

	return true;
}
