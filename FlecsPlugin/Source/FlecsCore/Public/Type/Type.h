// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EcsStruct.h"
#include "Entity.h"
#include "UObject/Object.h"
#include "flecs.h"
#include "Utils/EcsTypeUtils.h"
#include "Type.generated.h"

class UEcsWorld;

template<typename T>
concept IsUEStruct = requires()
{
	T::StaticClass();
	!TIsDerivedFrom<T,UObject>::Value;
};

USTRUCT(Blueprintable)
struct FLECSCORE_API FEcsTypeBase
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FEcsID EcsId{};
public:
	flecs::entity_t Entity = 0;
	size_t Size = 0;
	size_t Alignment = 0;
	bool AllowTag = true;
	int32_t ResetCount = 0;
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FLECSCORE_API FEcsType : public FEcsTypeBase
{
	GENERATED_BODY()
	using StructTypeMeta =UStruct*;
	using EnumTypeMeta =UEnum*;
public:
#pragma region API
	size_t GetSize() const
	{
		ecs_assert(s_id != 0, ECS_INTERNAL_ERROR, NULL);
		return Size;
	}
	
	size_t GetAlignment() const
	{
		ecs_assert(s_id != 0, ECS_INTERNAL_ERROR, NULL);
		return Alignment;
	}
	
	bool IsRegistered(const flecs::world_t *world) {
		if (ResetCount != ecs_cpp_reset_count_get()) {
			reset();
		}
		if (Entity == 0) {
			return false;
		}
		if (world && !ecs_exists(world, Entity)) {
			return false;
		}
		return true;
	}
#pragma endregion 
public://CPP Function

#pragma region StructReg
	void init(StructTypeMeta struct_type,
			flecs::entity_t entity,
			bool allow_tag = true)
	{
		assert(struct_type != nullptr);
		if (ResetCount != ecs_cpp_reset_count_get()) {
			reset();
		}

		// If an identifier was already set, check for consistency
		if (Entity) {
			ecs_assert(s_id == entity, ECS_INCONSISTENT_COMPONENT_ID,
				type_name<T>());
			ecs_assert(allow_tag == s_allow_tag, ECS_INVALID_PARAMETER, NULL);

			// Component was already registered and data is consistent with new
			// identifier, so nothing else to be done.
			return;
		}

		// Component wasn't registered yet, set the values. Register component
		// name as the fully qualified flecs path.
		Entity = entity;
		EcsId = Entity;
		AllowTag = allow_tag;
		Size = struct_type->PropertiesSize;
		Alignment = struct_type->MinAlignment;
		// 不允许空对象作为Tag
		// if (is_empty<T>::value && allow_tag) {
		// 	s_size = 0;
		// 	s_alignment = 0;
		// }

		ResetCount = ecs_cpp_reset_count_get();
	}
	
	flecs::entity_t id_explicit(StructTypeMeta struct_type,flecs::world_t *world = nullptr,
		const char *name = nullptr, bool allow_tag = true, flecs::id_t id = 0,
		bool is_component = true, bool *existing = nullptr)
	{
		if (!Entity) {
			// If no world was provided the component cannot be registered
			ecs_assert(world != nullptr, ECS_COMPONENT_NOT_REGISTERED, name);
		} else {
			ecs_assert(!id || s_id == id, ECS_INCONSISTENT_COMPONENT_ID, NULL);
		}

		// If no id has been registered yet for the component (indicating the
		// component has not yet been registered, or the component is used
		// across more than one binary), or if the id does not exists in the
		// world (indicating a multi-world application), register it.
		if (!Entity || (world && !ecs_exists(world, Entity))) {
			init(struct_type, Entity ? Entity : id, allow_tag);

			ecs_assert(!id || s_id == id, ECS_INTERNAL_ERROR, NULL);

			const char *symbol = nullptr;
			if (id) {
				symbol = ecs_get_symbol(world, id);
			}
			if (!symbol) {
				symbol = EcsTypeUtils::GetSymbol(struct_type);
			}

			flecs::entity_t entity = ecs_cpp_component_register_explicit(
					world, Entity, id, name, EcsTypeUtils::GetName(struct_type), symbol,
						Size, Alignment, is_component, existing);

			Entity = entity;
			EcsId = entity;
			StructWeakPtr = struct_type;
			//Enum需要单独支持
			// If component is enum type, register constants
// #if FLECS_CPP_ENUM_REFLECTION_SUPPORT
// 			_::init_enum<T>(world, entity);
// #endif
		}

		// By now the identifier must be valid and known with the world.
		ecs_assert(s_id != 0 && ecs_exists(world, s_id),
			ECS_INTERNAL_ERROR, NULL);

		return Entity;
	}

	
	flecs::id_t id(StructTypeMeta struct_type, flecs::world_t *world = nullptr, const char *name = nullptr,
		bool allow_tag = true)
	{
		// If no id has been registered yet, do it now.
#ifndef FLECS_CPP_NO_AUTO_REGISTRATION
		if (!IsRegistered(world)) {
			ecs_entity_t prev_scope = 0;
			ecs_id_t prev_with = 0;

			if (world) {
				prev_scope = ecs_set_scope(world, 0);
				prev_with = ecs_set_with(world, 0);
			}

			// This will register a component id, but will not register
			// lifecycle callbacks.
			bool existing;
			const bool bComponent = struct_type->IsChildOf(FEcsComponentBase::StaticStruct());
			id_explicit(struct_type, world, name, allow_tag, 0, bComponent, &existing);

			// Register lifecycle callbacks, but only if the component has a
			// size. Components that don't have a size are tags, and tags don't
			// require construction/destruction/copy/move's.
			if (GetSize() && !existing) {
				EcsTypeUtils::register_lifecycle_actions(struct_type,world, Entity);
			}

			if (prev_with) {
				ecs_set_with(world, prev_with);
			}
			if (prev_scope) {
				ecs_set_scope(world, prev_scope);
			}
		}
#else
		(void)world;
		(void)name;
		(void)allow_tag;

		ecs_assert(registered(world), ECS_INVALID_OPERATION,
			"component '%s' was not registered before use",
			type_name<T>());
#endif

		// By now we should have a valid identifier
		ecs_assert(s_id != 0, ECS_INTERNAL_ERROR, NULL);

		return Entity;
	}


#pragma endregion


#pragma region Native
	template<typename T>
	void id_native_type(flecs::world* world_ins)
	{
		auto entity = world_ins->component<T>();
		Entity = entity;
		EcsId = Entity;
		AllowTag = entity->allow_tag;
		Size = flecs::_::type<T>::size();
		Alignment = flecs::_::type<T>::alignment();
	}
#pragma endregion
#pragma region EnumReg
	void init(EnumTypeMeta struct_type,
	flecs::entity_t entity,
	bool allow_tag = false)
	{
		assert(struct_type != nullptr);
		if (ResetCount != ecs_cpp_reset_count_get()) {
			reset();
		}

		// If an identifier was already set, check for consistency
		if (Entity) {
			ecs_assert(s_id == entity, ECS_INCONSISTENT_COMPONENT_ID,
				type_name<T>());
			ecs_assert(allow_tag == s_allow_tag, ECS_INVALID_PARAMETER, NULL);

			// Component was already registered and data is consistent with new
			// identifier, so nothing else to be done.
			return;
		}

		// Component wasn't registered yet, set the values. Register component
		// name as the fully qualified flecs path.
		Entity = entity;
		EcsId = Entity;
		AllowTag = allow_tag;
		Size = 0;
		Alignment = 0;

		ResetCount = ecs_cpp_reset_count_get();
	}

	flecs::entity_t id_explicit(EnumTypeMeta struct_type,flecs::world_t *world = nullptr,
	const char *name = nullptr, bool allow_tag = false, flecs::id_t id = 0,
	bool is_component = false, bool *existing = nullptr)
	{
		if (!Entity) {
			// If no world was provided the component cannot be registered
			ecs_assert(world != nullptr, ECS_COMPONENT_NOT_REGISTERED, name);
		} else {
			ecs_assert(!id || s_id == id, ECS_INCONSISTENT_COMPONENT_ID, NULL);
		}

		// If no id has been registered yet for the component (indicating the
		// component has not yet been registered, or the component is used
		// across more than one binary), or if the id does not exists in the
		// world (indicating a multi-world application), register it.
		if (!Entity || (world && !ecs_exists(world, Entity))) {
			init(struct_type, Entity ? Entity : id, allow_tag);

			ecs_assert(!id || s_id == id, ECS_INTERNAL_ERROR, NULL);

			const char *symbol = nullptr;
			if (id) {
				symbol = ecs_get_symbol(world, id);
			}
			if (!symbol) {
				symbol = EcsTypeUtils::GetSymbol(struct_type);
			}

			flecs::entity_t entity = ecs_cpp_component_register_explicit(
					world, Entity, id, name, EcsTypeUtils::GetName(struct_type), symbol,
						Size, Alignment, is_component, existing);

			Entity = entity;
			EcsId = entity;

			// If component is enum type, register constants
			auto EnumCount = struct_type->NumEnums();
			//auto LastValue = 0;
			for (int i = 0; i < EnumCount; i++)
			{
				auto EnumName = struct_type->GetNameByIndex(i);
				auto EnumValue = struct_type->GetValueByIndex(i);
				ecs_cpp_enum_constant_register(
				world, entity, 0, name, static_cast<int32_t>(EnumValue));
			}
		}

		// By now the identifier must be valid and known with the world.
		ecs_assert(s_id != 0 && ecs_exists(world, s_id),
			ECS_INTERNAL_ERROR, NULL);

		return Entity;
	}

	flecs::id_t id(EnumTypeMeta struct_type, flecs::world_t *world = nullptr, const char *name = nullptr,
	bool allow_tag = true)
	{
		// If no id has been registered yet, do it now.
#ifndef FLECS_CPP_NO_AUTO_REGISTRATION
		if (!IsRegistered(world)) {
			ecs_entity_t prev_scope = 0;
			ecs_id_t prev_with = 0;

			if (world) {
				prev_scope = ecs_set_scope(world, 0);
				prev_with = ecs_set_with(world, 0);
			}

			// This will register a component id, but will not register
			// lifecycle callbacks.
			bool existing;
			id_explicit(struct_type, world, name, allow_tag, 0, true, &existing);

			// Register lifecycle callbacks, but only if the component has a
			// size. Components that don't have a size are tags, and tags don't
			// require construction/destruction/copy/move's.
			if (GetSize() && !existing) {
				EcsTypeUtils::register_lifecycle_actions(struct_type,world, Entity);
			}

			if (prev_with) {
				ecs_set_with(world, prev_with);
			}
			if (prev_scope) {
				ecs_set_scope(world, prev_scope);
			}
		}
#else
		(void)world;
		(void)name;
		(void)allow_tag;

		ecs_assert(registered(world), ECS_INVALID_OPERATION,
			"component '%s' was not registered before use",
			type_name<T>());
#endif

		// By now we should have a valid identifier
		ecs_assert(s_id != 0, ECS_INTERNAL_ERROR, NULL);

		return Entity;
	}
#pragma endregion 
protected:
	void reset();

	TWeakObjectPtr<UObject> StructWeakPtr;
};
