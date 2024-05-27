#pragma once
#include "UObject/Class.h"
#include "FlecsCore/FlecsRaw/flecs.h"
class EcsTypeUtils
{
public:
	static const char* GetSymbol(UStruct* StructType);
	static const char* GetName(UStruct* StructType);

	static const char* GetSymbol(UEnum* StructType);
	static const char* GetName(UEnum* StructType);
	static void register_lifecycle_actions(UObject* MetaType,flecs::world_t * World, flecs::entity_t EntityId);
};
