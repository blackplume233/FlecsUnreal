﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Type.h"
#include "UObject/Object.h"
#include "FlecsCore/FlecsRaw/flecs.h"
#include "EcsWorld.generated.h"


/**
 * 
 */

UCLASS(Blueprintable)
class FLECSCORE_API UEcsWorld : public UObject
{
	GENERATED_BODY()
	
public:
	UEcsWorld();
	
	UFUNCTION(Blueprintable,BlueprintCallable)
	FEcsType RegisterComponent(UStruct* StructType) ;
	UFUNCTION(Blueprintable,BlueprintCallable)
	FEcsType RegisterType( UField* Type);

	void RegisterTypeMeta(FEcsType EcsType,UStruct* StructType);
#pragma region Test
	inline bool EnableMonitor()
	{
		//https://www.flecs.dev/explorer/?host=localhost:7759
		worldIns.set<flecs::Rest>({7759});
		worldIns.import<flecs::monitor>();
		
		return true;
	}
	inline bool Progress(float DeltaTime) const
	{

		return worldIns.progress(DeltaTime);
	}
#pragma endregion 
	
	const flecs::world& GetRawEcsWorld(){ return worldIns;}

protected:
	void InitBuiltinTypes();
private:
	flecs::world worldIns;
	TMap<const UField*, FEcsType> TypeRegisterMap{};
};