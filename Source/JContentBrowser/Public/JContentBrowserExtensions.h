// Copyright 2020 Ramon Janousch

#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserFrontEndFilterExtension.h"

#include "JContentBrowserExtensions.generated.h"

/**
 * Register custom  filters with the content browser
 */
UCLASS()
class UJUnusedAssetsSearchFilter : public UContentBrowserFrontEndFilterExtension
{
public:
	GENERATED_BODY()

	// UContentBrowserFrontEndFilterExtension interface
	void AddFrontEndFilterExtensions(TSharedPtr<FFrontendFilterCategory> DefaultCategory,
		TArray<TSharedRef<FFrontendFilter>>& InOutFilterList) const override;
};

class FJContentBrowserExtensions
{
public:
	/**
	 * NOTE: similar to UEditorAssetLibrary::FindPackageReferencersForAsset but mostly without bLoadAssetsToConfirm
	 * Find Package Referencers for an asset. Only Soft and Hard dependencies would be looked for.
	 * Soft are dependencies which don't need to be loaded for the object to be used.
	 * Hard are dependencies which are required for correct usage of the source asset and must be loaded at the same time.
	 * Other references may exist. The asset may be currently used in memory by another asset, by the editor or by code.
	 * Package dependencies are cached with the asset. False positive can happen until all the assets are loaded and re-saved.
	 * @param	AssetPath				Asset Path of the asset that we are looking for (that is not a level).
	 * @return	The package path of the referencers.
	 */
	static bool FindPackageReferencersForAsset(const FString& AssetPath);

	// Some of these are from EditorScriptingUtils namespace

	// Check if the Path is a valid ContentBrowser Path
	static bool IsAValidPath(const FString& Path, const TCHAR* InvalidChar, FString& OutFailureReason);

	// Check if the Path have a valid root
	static bool HasValidRoot(const FString& ObjectPath);

	// Remove Class from "Class /Game/MyFolder/MyAsset
	static FString RemoveFullName(const FString& AJAssetPath, FString& OutFailureReason);

	// From "AssetClass'/Game/Folder/Package.Asset'", "AssetClass /Game/Folder/Package.Asset", "/Game/Folder/Package.Asset", "/Game/Folder/MyAsset" "/Game/Folder/Package.Asset:InnerAsset.2ndInnerAsset"
	// and convert to "/Game/Folder/Package.Asset"
	// @note: Object name is inferred from package name when missing
	static FString ConvertAJPathToObjectPath(const FString& AJAssetPath, FString& OutFailureReason);

	// Check if the asset registry module is loading
	static bool IsAssetRegistryModuleLoading();

	// Check if the editor is in a valid state to run a command.
	static bool CheckIfInEditorAndPIE();

	static void InstallHooks();
	static void RemoveHooks();

private:
	// Check whether the value is a power of two or not
	static bool IsPowerOfTwo(int32 Value);
};
