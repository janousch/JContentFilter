// Copyright 2020 Ramon Janousch

#include "JContentBrowserExtensions.h"

#include "Algo/Count.h"
#include "ContentBrowserModule.h"
#include "AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "Engine/Blueprint.h"
#include "Editor.h"
#include "UObject/UObjectGlobals.h"


#define LOCTEXT_NAMESPACE "JContentBrowser"

DEFINE_LOG_CATEGORY_STATIC(LogJContentBrowser, All, All)


//////////////////////////////////////////////////////////////////////////
// FFrontendFilter_DialogueParticipants
/** A filter that search for blueprints that have implemented the Dialogue Participant */
class FFrontendFilter_JUnusedAssets : public FFrontendFilter
{
public:
	FFrontendFilter_JUnusedAssets(const TSharedPtr<FFrontendFilterCategory>& InCategory)
		: FFrontendFilter(InCategory)
	{
	}

	// FFrontendFilter implementation
	/** Returns the system name for this filter */
	FString GetName() const override
	{
		return TEXT("Inappropriate Texture Filter");
	}

	/** Returns the human readable name for this filter */
	FText GetDisplayName() const override
	{
		return LOCTEXT("InappropriateTextureFilter_Name", "Inappropriate Textures");
	}

	/** Returns the tooltip for this filter, shown in the filters menu */
	FText GetToolTipText() const override
	{
		return LOCTEXT("InappropriateTextureFilter_ToolTip", "Show all textures without power of two and are not UI textures.");
	}

	/** Returns the color this filter button will be when displayed as a button */
	FLinearColor GetColor() const override { return FLinearColor::Red; }

	/** Returns the name of the icon to use in menu entries */
	FName GetIconName() const override { return NAME_None; }

	/** Invoke to set the ARFilter that is currently used to filter assets in the asset view */
	void SetCurrentFilter(const FARFilter& InBaseFilter) override { }
	// End of FFrontendFilter implementation

	// IFilter implementation
	/** Returns whether the specified Item passes the Filter's restrictions */
	bool PassesFilter(FAssetFilterType InItem) const override
	{
		// NOTE: we need this plugin enabled?
		return FJContentBrowserExtensions::FindPackageReferencersForAsset(InItem.ObjectPath.ToString());
	}
	// End of IFilter implementation
};

//////////////////////////////////////////////////////////////////////////
// UDialogueSearchFilter
void UJUnusedAssetsSearchFilter::AddFrontEndFilterExtensions(TSharedPtr<FFrontendFilterCategory> DefaultCategory,
	TArray<TSharedRef<FFrontendFilter>>& InOutFilterList) const
{
	InOutFilterList.Add(MakeShared<FFrontendFilter_JUnusedAssets>(DefaultCategory));
}


//////////////////////////////////////////////////////////////////////////
// FJContentBrowserExtensions
bool FJContentBrowserExtensions::FindPackageReferencersForAsset(const FString& AJAssetPath)
{
	TGuardValue<bool> UnattendedScriptGuard(GIsRunningUnattendedScript, true);

	if (!CheckIfInEditorAndPIE() || !IsAssetRegistryModuleLoading())
	{
		return false;
	}

	FString FailureReason;
	const FString AssetPath = ConvertAJPathToObjectPath(AJAssetPath, FailureReason);
	if (AssetPath.IsEmpty())
	{
		UE_LOG(LogJContentBrowser, Error, TEXT("FindAssetPackageReferencers. %s"), *FailureReason);
		return false;
	}

	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *AssetPath));
	if (!IsValid(Texture)) return false;

	if (Texture->CompressionSettings == TextureCompressionSettings::TC_EditorIcon) return false;
	if (!IsPowerOfTwo(Texture->GetSizeX())) return true;
	if (!IsPowerOfTwo(Texture->GetSizeY())) return true;

	return false;
}

bool FJContentBrowserExtensions::IsPowerOfTwo(int32 Value) {
	float Log2 = FMath::Log2((float)Value);

	return FMath::CeilToFloat(Log2) == FMath::FloorToFloat(Log2);
}

bool FJContentBrowserExtensions::IsAssetRegistryModuleLoading()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	if (AssetRegistryModule.Get().IsLoadingAssets())
	{
		UE_LOG(LogJContentBrowser, Error, TEXT("The AssetRegistry is currently loading."));
		return false;
	}
	return true;
}

bool FJContentBrowserExtensions::CheckIfInEditorAndPIE()
{
	if (!IsInGameThread())
	{
		UE_LOG(LogJContentBrowser, Error, TEXT("You are not on the main thread."));
		return false;
	}
	if (!GIsEditor)
	{
		UE_LOG(LogJContentBrowser, Error, TEXT("You are not in the Editor."));
		return false;
	}
	if (GEditor->PlayWorld || GIsPlayInEditorWorld)
	{
		UE_LOG(LogJContentBrowser, Error, TEXT("The Editor is currently in a play mode."));
		return false;
	}
	return true;
}

// Test for invalid characters
bool FJContentBrowserExtensions::IsAValidPath(const FString& Path, const TCHAR* InvalidChar, FString& OutFailureReason)
{
	const int32 StrLen = FCString::Strlen(InvalidChar);
	for (int32 Index = 0; Index < StrLen; ++Index)
	{
		int32 FoundIndex = 0;
		if (Path.FindChar(InvalidChar[Index], FoundIndex))
		{
			OutFailureReason = FString::Printf(TEXT("Can't convert the path %s because it contains invalid characters."), *Path);
			return false;
		}
	}

	if (Path.Len() > FPlatformMisc::GetMaxPathLength())
	{
		OutFailureReason = FString::Printf(TEXT("Can't convert the path %s because it is too long; this may interfere with cooking for consoles. Unreal filenames should be no longer than %d characters."), *Path, FPlatformMisc::GetMaxPathLength());
		return false;
	}
	return true;
}

bool FJContentBrowserExtensions::HasValidRoot(const FString& ObjectPath)
{
	FString Filename;
	bool bValidRoot = true;
	if (!ObjectPath.IsEmpty() && ObjectPath[ObjectPath.Len() - 1] == TEXT('/'))
	{
		bValidRoot = FPackageName::TryConvertLongPackageNameToFilename(ObjectPath, Filename);
	}
	else
	{
		FString ObjectPathWithSlash = ObjectPath;
		ObjectPathWithSlash.AppendChar(TEXT('/'));
		bValidRoot = FPackageName::TryConvertLongPackageNameToFilename(ObjectPathWithSlash, Filename);
	}

	return bValidRoot;
}

FString FJContentBrowserExtensions::RemoveFullName(const FString& AJAssetPath, FString& OutFailureReason)
{
	FString Result = AJAssetPath.TrimStartAndEnd();
	int32 NumberOfSpace = Algo::Count(AJAssetPath, TEXT(' '));

	if (NumberOfSpace == 0)
	{
		return MoveTemp(Result);
	}
	else if (NumberOfSpace > 1)
	{
		OutFailureReason = FString::Printf(TEXT("Can't convert path '%s' because there are too many spaces."), *AJAssetPath);
		return FString();
	}
	else
	{
		int32 FoundIndex = 0;
		AJAssetPath.FindChar(TEXT(' '), FoundIndex);
		check(FoundIndex > INDEX_NONE && FoundIndex < AJAssetPath.Len()); // because of TrimStartAndEnd

		// Confirm that it's a valid Class
		FString ClassName = AJAssetPath.Left(FoundIndex);

		// Convert \ to /
		ClassName.ReplaceInline(TEXT("\\"), TEXT("/"), ESearchCase::CaseSensitive);

		// Test ClassName for invalid Char
		const int32 StrLen = FCString::Strlen(INVALID_OBJECTNAME_CHARACTERS);
		for (int32 Index = 0; Index < StrLen; ++Index)
		{
			int32 InvalidFoundIndex = 0;
			if (ClassName.FindChar(INVALID_OBJECTNAME_CHARACTERS[Index], InvalidFoundIndex))
			{
				OutFailureReason = FString::Printf(TEXT("Can't convert the path %s because it contains invalid characters (probably spaces)."), *AJAssetPath);
				return FString();
			}
		}

		// Return the path without the Class name
		return AJAssetPath.Mid(FoundIndex + 1);
	}
}

FString FJContentBrowserExtensions::ConvertAJPathToObjectPath(const FString& AJAssetPath, FString& OutFailureReason)
{
	if (AJAssetPath.Len() < 2) // minimal length to have /G
	{
		OutFailureReason = FString::Printf(TEXT("Can't convert the path '%s' because the Root path need to be specified. ie /Game/"), *AJAssetPath);
		return FString();
	}

	// Remove class name from Reference Path
	FString TextPath = FPackageName::ExportTextPathToObjectPath(AJAssetPath);

	// Remove class name Fullname
	TextPath = RemoveFullName(TextPath, OutFailureReason);
	if (TextPath.IsEmpty())
	{
		return FString();
	}

	// Extract the subobject path if aJ
	FString SubObjectPath;
	int32 SubObjectDelimiterIdx;
	if (TextPath.FindChar(SUBOBJECT_DELIMITER_CHAR, SubObjectDelimiterIdx))
	{
		SubObjectPath = TextPath.Mid(SubObjectDelimiterIdx + 1);
		TextPath = TextPath.Left(SubObjectDelimiterIdx);
	}

	// Convert \ to /
	TextPath.ReplaceInline(TEXT("\\"), TEXT("/"), ESearchCase::CaseSensitive);
	FPaths::RemoveDuplicateSlashes(TextPath);

	FString AssetFullName;
	{
		// Get everything after the last slash
		int32 IndexOfLastSlash = INDEX_NONE;
		TextPath.FindLastChar('/', IndexOfLastSlash);

		FString Folders = TextPath.Left(IndexOfLastSlash);
		// Test for invalid characters
		if (!IsAValidPath(Folders, INVALID_LONGPACKAGE_CHARACTERS, OutFailureReason))
		{
			return FString();
		}

		AssetFullName = TextPath.Mid(IndexOfLastSlash + 1);
	}

	// Get the object name
	FString ObjectName = FPackageName::ObjectPathToObjectName(AssetFullName);
	if (ObjectName.IsEmpty())
	{
		OutFailureReason = FString::Printf(TEXT("Can't convert the path '%s' because it doesn't contain an asset name."), *AJAssetPath);
		return FString();
	}

	// Test for invalid characters
	if (!IsAValidPath(ObjectName, INVALID_OBJECTNAME_CHARACTERS, OutFailureReason))
	{
		return FString();
	}

	// Confirm that we have a valid Root Package and get the valid PackagePath /Game/MyFolder/MyAsset
	FString PackagePath;
	if (!FPackageName::TryConvertFilenameToLongPackageName(TextPath, PackagePath, &OutFailureReason))
	{
		return FString();
	}

	if (PackagePath.Len() == 0)
	{
		OutFailureReason = FString::Printf(TEXT("Can't convert path '%s' because the PackagePath is empty."), *AJAssetPath);
		return FString();
	}

	if (PackagePath[0] != TEXT('/'))
	{
		OutFailureReason = FString::Printf(TEXT("Can't convert path '%s' because the PackagePath '%s' doesn't start with a '/'."), *AJAssetPath, *PackagePath);
		return FString();
	}

	FString ObjectPath = FString::Printf(TEXT("%s.%s"), *PackagePath, *ObjectName);

	if (FPackageName::IsScriptPackage(ObjectPath))
	{
		OutFailureReason = FString::Printf(TEXT("Can't convert the path '%s' because it start with /Script/"), *AJAssetPath);
		return FString();
	}
	if (FPackageName::IsMemoryPackage(ObjectPath))
	{
		OutFailureReason = FString::Printf(TEXT("Can't convert the path '%s' because it start with /Memory/"), *AJAssetPath);
		return FString();
	}

	// Confirm that the PackagePath starts with a valid root
	if (!HasValidRoot(PackagePath))
	{
		OutFailureReason = FString::Printf(TEXT("Can't convert the path '%s' because it does not map to a root."), *AJAssetPath);
		return FString();
	}

	return ObjectPath;
}

void FJContentBrowserExtensions::InstallHooks()
{

}

void FJContentBrowserExtensions::RemoveHooks()
{

}

#undef LOCTEXT_NAMESPACE
