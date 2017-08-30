// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ConfiguratorGameMode.h"

#include "IMessageRpcServer.h"
#include "IMessagingRpcModule.h"
#include "RpcMessages.h"
#include "MessageEndpointBuilder.h"
#include "Modules/ModuleManager.h"
#include "Runtime/Launch/Resources/Version.h"


void AConfiguratorGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	MessageEndpoint = FMessageEndpoint::Builder("AConfiguratorGameMode")
		.Handling<FConfiguratorPing>(this, &AConfiguratorGameMode::HandleMessage);

	if (MessageEndpoint.IsValid())
	{
		MessageEndpoint->Subscribe<FConfiguratorPing>();
	}

	RpcServer = FModuleManager::LoadModuleChecked<IMessagingRpcModule>("MessagingRpc").CreateRpcServer();
	{
		RpcServer->RegisterHandler<FConfiguratorCommand>(this, &AConfiguratorGameMode::HandleRequest);
	}
}


void AConfiguratorGameMode::HandleMessage(const FConfiguratorPing& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
#if ((ENGINE_MAJOR_VERSION == 4) && (ENGINE_MINOR_VERSION == 12) && (ENGINE_PATCH_VERSION == 0))
	if (RpcServer.IsValid())
	{
		RpcServer->GetEndpoint()->Send(new FConfiguratorPong(RpcServer->GetAddress().ToString()), Context->GetSender());
	}
#else
	MessageEndpoint->Send(new FConfiguratorPong(RpcServer->GetAddress().ToString()), Context->GetSender());
#endif
}


TAsyncResult<int32> AConfiguratorGameMode::HandleRequest(const FConfiguratorCommandRequest& Message)
{
	switch (Message.Command)
	{
	case EConfiguratorCommand::Reset:
		OnReset.Broadcast();
		break;

	case EConfiguratorCommand::Tap:
		OnTap.Broadcast();
		break;

	case EConfiguratorCommand::UpdateRotation:
		OnUpdateRotation.Broadcast(Message.Yaw, Message.Pitch);
		break;

	case EConfiguratorCommand::UpdateSelectedItem:
		OnUpdateSelectedItem.Broadcast(Message.CategoryIndex, Message.ItemIndex);
		break;

	case EConfiguratorCommand::UpdateVisualization:
		OnUpdateVisualization.Broadcast(Message.ModeIndex);
		break;

	case EConfiguratorCommand::Zoom:
		OnZoom.Broadcast();
		break;

	default:
		break;
	}

	return 0;
}
