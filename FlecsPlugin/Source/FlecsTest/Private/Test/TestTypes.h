﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Type/EcsStruct.h"
#include "UObject/Object.h"
#include "TestTypes.generated.h"

USTRUCT()
struct FTestComponent : public FEcsComponentBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int Value;
	UPROPERTY()
	float fValue;
	UPROPERTY()
	UObject* ObjectPtr;
	UPROPERTY()
	FEcsComponentBase ComponentBase;
};

/**
 * 
 */
UCLASS()
class FLECSTEST_API UTestTypes : public UObject
{
	GENERATED_BODY()
};