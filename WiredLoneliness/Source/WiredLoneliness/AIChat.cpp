// Fill out your copyright notice in the Description page of Project Settings.

#include "WiredLoneliness.h"
#include "AIChat.h"


AAIChat::AAIChat()
{
	PrimaryActorTick.bCanEverTick = true;
	aichatbot = new AIChatBot("AIChat");
}


void AAIChat::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAIChat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString AAIChat::getUserInput(FString input)
{

		return FString(aichatbot->getInput(TCHAR_TO_UTF8(*input)).c_str());

}

FString AAIChat::getBotResponse() {
	return FString(aichatbot->respond().c_str());
}
