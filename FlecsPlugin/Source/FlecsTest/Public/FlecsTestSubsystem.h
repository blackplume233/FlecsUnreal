// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Type/EcsWorld.h"

#include "FlecsTestSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class FLECSTEST_API UFlecsTestSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual TStatId GetStatId() const override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UFUNCTION(BlueprintImplementableEvent, Category = "FlecsTestSubsystem")
	UEcsWorld* GetEcsWorld() const;

	
	UPROPERTY()
	UEcsWorld* World;
	// UEcsWorld* World;
};
