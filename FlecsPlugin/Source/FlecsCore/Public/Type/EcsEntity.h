// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlecsCore/FlecsRaw/flecs.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EcsEntity.generated.h"

/**
 * 
 */
USTRUCT()
struct FLECSCORE_API FEcsEntity 
{
	GENERATED_BODY()
public:
	FEcsEntity() : entity(nullptr) {}
	FEcsEntity(flecs::entity entity) : entity(entity) {}
	flecs::entity entity;
};


