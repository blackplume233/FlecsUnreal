#pragma once
#include "Containers/Map.h"
#include "UObject/NameTypes.h"
struct FNativeNameCache
{
public:
	const char* GetNativeName(FName Name);
	const char* GetNativeName(const FString& Name);
	~FNativeNameCache();
private:
	TMap<FName, const char *> NameToNativeNameMap{};
	TMap<FString, const char *> StrToNativeNameMap{};
	
};
