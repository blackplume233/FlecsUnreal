// Fill out your copyright notice in the Description page of Project Settings.


#include "Type/Type.h"

#include "Type/EcsStruct.h"
#include "Type/EcsWorld.h"



void FEcsType::reset()
{
	entity_id = 0;
	Size = 0;
	Alignment = 0;
	AllowTag = true;
}
