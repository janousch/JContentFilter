// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "JContentBrowser/Public/JContentBrowserExtensions.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeJContentBrowserExtensions() {}
// Cross Module References
	JCONTENTBROWSER_API UClass* Z_Construct_UClass_UJUnusedAssetsSearchFilter_NoRegister();
	JCONTENTBROWSER_API UClass* Z_Construct_UClass_UJUnusedAssetsSearchFilter();
	CONTENTBROWSER_API UClass* Z_Construct_UClass_UContentBrowserFrontEndFilterExtension();
	UPackage* Z_Construct_UPackage__Script_JContentBrowser();
// End Cross Module References
	void UJUnusedAssetsSearchFilter::StaticRegisterNativesUJUnusedAssetsSearchFilter()
	{
	}
	UClass* Z_Construct_UClass_UJUnusedAssetsSearchFilter_NoRegister()
	{
		return UJUnusedAssetsSearchFilter::StaticClass();
	}
	struct Z_Construct_UClass_UJUnusedAssetsSearchFilter_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UJUnusedAssetsSearchFilter_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UContentBrowserFrontEndFilterExtension,
		(UObject* (*)())Z_Construct_UPackage__Script_JContentBrowser,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UJUnusedAssetsSearchFilter_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "JContentBrowserExtensions.h" },
		{ "ModuleRelativePath", "Public/JContentBrowserExtensions.h" },
		{ "ToolTip", "Register custom  filters with the content browser" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UJUnusedAssetsSearchFilter_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UJUnusedAssetsSearchFilter>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UJUnusedAssetsSearchFilter_Statics::ClassParams = {
		&UJUnusedAssetsSearchFilter::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UJUnusedAssetsSearchFilter_Statics::Class_MetaDataParams, ARRAY_COUNT(Z_Construct_UClass_UJUnusedAssetsSearchFilter_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UJUnusedAssetsSearchFilter()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UJUnusedAssetsSearchFilter_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UJUnusedAssetsSearchFilter, 1374021254);
	template<> JCONTENTBROWSER_API UClass* StaticClass<UJUnusedAssetsSearchFilter>()
	{
		return UJUnusedAssetsSearchFilter::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UJUnusedAssetsSearchFilter(Z_Construct_UClass_UJUnusedAssetsSearchFilter, &UJUnusedAssetsSearchFilter::StaticClass, TEXT("/Script/JContentBrowser"), TEXT("UJUnusedAssetsSearchFilter"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UJUnusedAssetsSearchFilter);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
