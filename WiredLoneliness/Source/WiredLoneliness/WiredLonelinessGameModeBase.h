#pragma once

#include "GameFramework/GameModeBase.h"
#include "WiredLonelinessGameModeBase.generated.h"

UCLASS()
class WIREDLONELINESS_API AWiredLonelinessGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void StartPlay() override;
};
