#include "EcsTypeUtils.h"
#include <assert.h>
#include "Foudation/NativeNameCache.h"

const char* EcsTypeUtils::GetSymbol(UStruct* StructType)
{
	assert(StructType != nullptr);
	return GetNativeNameCache().GetNativeName(StructType->GetFullName()); 
}

const char* EcsTypeUtils::GetName(UStruct* StructType)
{
	assert(StructType != nullptr);
	return GetNativeNameCache().GetNativeName(StructType->GetName()); 
}

const char* EcsTypeUtils::GetSymbol(UEnum* StructType)
{
	assert(StructType != nullptr);
	return GetNativeNameCache().GetNativeName(StructType->GetFullName()); 
}

const char* EcsTypeUtils::GetName(UEnum* StructType)
{
	assert(StructType != nullptr);
	return GetNativeNameCache().GetNativeName(StructType->GetFName()); 
}

void EcsTypeUtils::register_lifecycle_actions(UObject* MetaType, flecs::world_t* World, flecs::entity_t EntityId)
{
}

FNativeNameCache& EcsTypeUtils::GetNativeNameCache()
{
	static FNativeNameCache NativeNameCache;
	return NativeNameCache;
}

