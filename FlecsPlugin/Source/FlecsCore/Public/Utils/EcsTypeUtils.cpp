#include "EcsTypeUtils.h"

#include <assert.h>

const char* EcsTypeUtils::GetSymbol(UStruct* StructType)
{
	assert(StructType != nullptr);
	return TCHAR_TO_UTF8(ToCStr(StructType->GetFullName())); 
}

const char* EcsTypeUtils::GetName(UStruct* StructType)
{
	assert(StructType != nullptr);
	return TCHAR_TO_UTF8(ToCStr(StructType->GetName())); 
}

const char* EcsTypeUtils::GetSymbol(UEnum* StructType)
{
	assert(StructType != nullptr);
	return TCHAR_TO_UTF8(ToCStr(StructType->GetFullName())); 
}

const char* EcsTypeUtils::GetName(UEnum* StructType)
{
	assert(StructType != nullptr);
	return TCHAR_TO_UTF8(ToCStr(StructType->GetName())); 
}

void EcsTypeUtils::register_lifecycle_actions(UObject* MetaType, flecs::world_t* World, flecs::entity_t EntityId)
{
	//所有的反射结构体都视为平凡结构体
}
