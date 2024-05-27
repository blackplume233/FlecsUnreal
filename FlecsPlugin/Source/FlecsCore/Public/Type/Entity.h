#pragma once
#include "FlecsCore/FlecsRaw/flecs.h"
#include "Entity.generated.h"

USTRUCT()
struct FEcsID
{
	GENERATED_BODY()
public:
	explicit FEcsID(const flecs::id& value):id(value){}
	FEcsID(const flecs::entity_t& value):id(value){}
	FEcsID():id(){}

	flecs::entity_t GetID() const { return id; }
private:

	flecs::id id;
};

USTRUCT()
struct FEntity
{
	GENERATED_BODY()
	
private:
	flecs::entity entity;
};
