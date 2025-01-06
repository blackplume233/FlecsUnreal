// Fill out your copyright notice in the Description page of Project Settings.


#include "FlecsTestSubsystem.h"

#include "Test/TestTypes.h"

void UFlecsTestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	World = NewObject<UEcsWorld>(GetWorld());
	World->EnableMonitor();

	World->RegisterComponent(StaticStruct<FTestComponent>());
}

void UFlecsTestSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	World->Progress(DeltaTime);
	
}

ETickableTickType UFlecsTestSubsystem::GetTickableTickType() const
{
	return ETickableTickType::Always;
}

TStatId UFlecsTestSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UFlecsTestSubsystem, STATGROUP_Tickables);
}

bool UFlecsTestSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Outer->GetWorld() != nullptr &&( Outer->GetWorld()->WorldType  ==  EWorldType::PIE || Outer->GetWorld()->WorldType  ==  EWorldType::Game);
}


