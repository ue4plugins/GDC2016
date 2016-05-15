// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "GDC2016PrivatePCH.h"
#include "ModuleInterface.h"
#include "ModuleManager.h"


#define LOCTEXT_NAMESPACE "FConfiguratorRemoteModule"


/**
 * Implements the GDC2016 module.
 */
class FGDC2016Module
	: public IModuleInterface
{
public:

	// IModuleInterface interface

	virtual void StartupModule() override
	{
	}

	virtual void ShutdownModule() override
	{
	}

	virtual bool SupportsDynamicReloading() override
	{
		return true;
	}
};


IMPLEMENT_MODULE(FGDC2016Module, GDC2016);


#undef LOCTEXT_NAMESPACE
