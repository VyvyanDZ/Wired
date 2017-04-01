// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "WiredLonelinessGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class WIREDLONELINESS_API AWiredLonelinessGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void StartPlay() override;
};
