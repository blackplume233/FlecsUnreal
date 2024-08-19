// Fill out your copyright notice in the Description page of Project Settings.


#include "Type/EcsWorld.h"
#include "Type/EcsStruct.h"
#include "UObject/UnrealTypePrivate.h"

UEcsWorld::UEcsWorld():worldIns{}
{
	worldIns.set_entity_range(1,0);
#ifdef REGISTER_META
	InitBuiltinTypes();
#endif
	
}

FEcsType UEcsWorld::RegisterComponent(UStruct* StructType)
{
	if(TypeRegisterMap.Contains(StructType) && TypeRegisterMap[StructType].IsRegistered(worldIns))
	{
		return TypeRegisterMap[StructType];
	}
	
	FEcsType EcsType = RegisterType(StructType);
	return EcsType;
}

FEcsType UEcsWorld::RegisterType(UField* Type)
{
	FEcsType* TypeDefine = TypeRegisterMap.Find(Type);
	if(TypeDefine != nullptr  && TypeDefine->IsRegistered(worldIns))
	{
		return *TypeDefine;
	}

	const bool bAllowTag = true;
	const auto Name = nullptr;

	FEcsType EcsType{};
	if(const auto EnumType = Cast<UEnum>(Type))
	{
		EcsType.id(EnumType,worldIns,Name,bAllowTag);
	}
	else if(const auto StructType = Cast<UStruct>(Type))
	{
		EcsType.id(StructType,worldIns,Name,bAllowTag);
		TypeRegisterMap.Add(Type, EcsType);
#ifdef REGISTER_META
		//reg layout
		auto meta = ecs_struct_desc_t();
		int  memberIndex = 0;
		meta.entity = EcsType.entity_id;
		for(TFieldIterator<FProperty> it(StructType);it;++it,++memberIndex)
		{
			auto* Property = *it;
			auto& member = meta.members[memberIndex];
			member.size = Property->GetSize();
			member.offset = Property->GetOffset_ForDebug();
			member.count = Property->ArrayDim;
			member.name = TCHAR_TO_UTF8(ToCStr(Property->GetName())); 
			if(Property->IsA<FIntProperty>())
			{
				member.type = worldIns.component<int>();
			}
			else if (Property->IsA<FBoolProperty>()){
				member.type = worldIns.component<bool>();
			}
			else if (Property->IsA<FFloatProperty>()){
				member.type = worldIns.component<float>();
			}
			else if (Property->IsA<FDoubleProperty>()){
				member.type = worldIns.component<double>();
			}
			else if (Property->IsA<FStrProperty>()){
				member.type = worldIns.component<FString>();
			}
			//else if (Property->IsA<FObjectProperty>()){
			//	member.type = worldIns.component<UObject*>();
			//}
			else if (Property->IsA<FStructProperty>())
			{
				
			}
		}

		ecs_struct_init(worldIns, &meta);
#endif
	}
	else
	{
		ecs_assert(false, ECS_INVALID_PARAMETER, NULL);
	}
	

	
	return EcsType;
}

void UEcsWorld::RegisterTypeMeta(FEcsType EcsType,UStruct* StructType)
{
#ifdef REGISTER_META
	//reg layout
	auto meta = ecs_struct_desc_t();
	int  memberIndex = 0;
	meta.entity = EcsType.entity_id;
#endif
	
}


void UEcsWorld::InitBuiltinTypes()
{
	worldIns.component<int>().set(EcsPrimitive{ecs_primitive_kind_t::EcsI32});
	worldIns.component<float>().set(EcsPrimitive{ecs_primitive_kind_t::EcsF32});
	worldIns.component<bool>().set(EcsPrimitive{ecs_primitive_kind_t::EcsBool});
	worldIns.component<double>().set(EcsPrimitive{ecs_primitive_kind_t::EcsF64});
	worldIns.component<FString>().set(EcsPrimitive{ecs_primitive_kind_t::EcsString});
	worldIns.component<long>().set(EcsPrimitive{ecs_primitive_kind_t::EcsI64});
	worldIns.component<FEcsType>().set(EcsPrimitive{ecs_primitive_kind_t::EcsEntity});
	//worldIns.component<UObject*>().set(EcsPrimitive{ecs_primitive_kind_t::EcsUPtr});
}
