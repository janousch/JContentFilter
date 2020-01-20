// Copyright 2020 Ramon Janousch
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FJContentBrowserModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
