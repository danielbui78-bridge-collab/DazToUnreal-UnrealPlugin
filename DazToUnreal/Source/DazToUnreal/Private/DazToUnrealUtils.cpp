#include "DazToUnrealUtils.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "Misc/Paths.h"

FString FDazToUnrealUtils::SanitizeName(FString OriginalName)
{
	return OriginalName.Replace(TEXT(" "), TEXT(""))
		.Replace(TEXT("("), TEXT("_"))
		.Replace(TEXT(")"), TEXT("_"))
		.Replace(TEXT("."), TEXT("_"))
		.Replace(TEXT("&"), TEXT("_"))
		.Replace(TEXT("!"), TEXT("_"))
		.Replace(TEXT("*"), TEXT("_"))
		.Replace(TEXT("<"), TEXT("_"))
		.Replace(TEXT(">"), TEXT("_"))
		.Replace(TEXT("?"), TEXT("_"))
		.Replace(TEXT("\\"), TEXT("_"));
}

bool FDazToUnrealUtils::MakeDirectoryAndCheck(FString& Directory)
{
	// Directory Tree Algorithm courtesy of: "https://nerivec.github.io/old-ue4-wiki/pages/algorithm-analysis-create-directory-recursively.html"
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString TargetFolderPath = Directory;
	FPaths::NormalizeDirectoryName(TargetFolderPath);
	TargetFolderPath += "/";

	FString CurrentFolderLevel;
	FString NextFolderLevel;
	FString RemainingString;

	TargetFolderPath.Split(TEXT("/"), &CurrentFolderLevel, &RemainingString);
	CurrentFolderLevel += "/";

	int loopIterations = 0;
	while (RemainingString != "" && loopIterations < 1000)
	{
		RemainingString.Split(TEXT("/"), &NextFolderLevel, &RemainingString);
		CurrentFolderLevel += NextFolderLevel + FString("/");
		if (!FPaths::DirectoryExists(CurrentFolderLevel))
		{
			PlatformFile.CreateDirectory(*CurrentFolderLevel);
			if (!FPaths::DirectoryExists(CurrentFolderLevel))
			{
				return false;
			}
		}
		loopIterations++;
	}
	return true;
}