// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AIChatBot.h"
#include "AIChat.generated.h"

UCLASS()
class WIREDLONELINESS_API AAIChat : public AActor
{
	GENERATED_BODY()
	
public:	
	AAIChat();

protected:
	virtual void BeginPlay() override;

public:	
	AIChatBot* aichatbot;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category=Aichat)
	FString getUserInput(FString input);

	UFUNCTION(BlueprintCallable, Category = Aichat)
	FString getBotResponse();

private:
	

};
