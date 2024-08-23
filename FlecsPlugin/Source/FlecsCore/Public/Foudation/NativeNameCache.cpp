#include "NativeNameCache.h"

#include "Containers/StringConv.h"


const char* FNativeNameCache::GetNativeName(FName Name)
{
	if(NameToNativeNameMap.Contains(Name))
	{
		return NameToNativeNameMap[Name];
	}
	auto* TempValue = StringCast<ANSICHAR>(ToCStr(Name.ToString())).Get();
	auto len =  strlen(TempValue);
	auto* Result = static_cast<char*>(FMemory::Malloc(len + 1));
	FMemory::Memcpy(Result, TempValue, len + 1);
	NameToNativeNameMap.Add(Name, Result);
	return Result;
}

const char* FNativeNameCache::GetNativeName(const FString& Name)
{
	if(StrToNativeNameMap.Contains(Name))
	{
		return StrToNativeNameMap[Name];
	}
	auto* TempValue = StringCast<ANSICHAR>(ToCStr(Name)).Get();
	auto len =  strlen(TempValue);
	auto* Result = static_cast<char*>(FMemory::Malloc(len + 1));
	FMemory::Memcpy(Result, TempValue, len + 1);
	StrToNativeNameMap.Add(Name, Result);
	return Result;
}

FNativeNameCache::~FNativeNameCache()
{
	for (auto PairValue : NameToNativeNameMap)
	{
		if(PairValue.Value != nullptr)
		{
			FMemory::Free(((void *)PairValue.Value));
		}
	}
	NameToNativeNameMap.Empty();

	for (auto StrPairValue : StrToNativeNameMap)
	{
		if(StrPairValue.Value != nullptr)
		{
			FMemory::Free(((void *)StrPairValue.Value));
		}
	}
	StrToNativeNameMap.Empty();
}
