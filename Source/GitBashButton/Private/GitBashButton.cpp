#include "GitBashButton.h"
#include "GitBashButtonStyle.h"
#include "GitBashCommands.h"

#include "ToolMenus.h"
#include "ToolMenu.h"
#include "ToolMenuSection.h"
#include "ToolMenuEntry.h"

#include "Framework/Commands/UICommandList.h"
#include "LevelEditor.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"

#define LOCTEXT_NAMESPACE "FGitBashButtonModule"

FString CachedGitBashPath;
TSharedPtr<FUICommandList> PluginCommands;

void FGitBashButtonModule::StartupModule()
{
    FGitBashButtonStyle::Initialize();
    FGitBashCommands::Register();

    // Discover Git Bash path
    void* ReadPipe = nullptr;
    void* WritePipe = nullptr;
    FPlatformProcess::CreatePipe(ReadPipe, WritePipe);

    const FString CommandLine = TEXT("/C where git");
    const FString CmdExe = TEXT("C:\\Windows\\System32\\cmd.exe");

    FProcHandle ProcHandle = FPlatformProcess::CreateProc(
        *CmdExe,
        *CommandLine,
        false, true, false,
        nullptr, 0,
        nullptr,
        WritePipe, ReadPipe
    );

    FString GitOutput;
    if (ProcHandle.IsValid())
    {
        FPlatformProcess::WaitForProc(ProcHandle);
        GitOutput = FPlatformProcess::ReadPipe(ReadPipe);
        FPlatformProcess::CloseProc(ProcHandle);
    }

    FPlatformProcess::ClosePipe(ReadPipe, WritePipe);

    if (!GitOutput.IsEmpty())
    {
        TArray<FString> Lines;
        GitOutput.ParseIntoArrayLines(Lines);
        if (Lines.Num() > 0)
        {
            const FString GitExePath = Lines[0].TrimStartAndEnd();
            const FString GitFolder = FPaths::GetPath(FPaths::GetPath(GitExePath));
            CachedGitBashPath = GitFolder / TEXT("git-bash.exe");

            if (!FPaths::FileExists(CachedGitBashPath))
            {
                CachedGitBashPath.Empty();
                UE_LOG(LogTemp, Error, TEXT("Git Bash not found at resolved path."));
            }
        }
    }

    // Define shared action logic
    const FExecuteAction LaunchGitBashAction = FExecuteAction::CreateLambda([this]()
        {
            if (CachedGitBashPath.IsEmpty() || !FPaths::FileExists(CachedGitBashPath))
            {
                UE_LOG(LogTemp, Error, TEXT("Git Bash path not cached or invalid."));
                return;
            }

            FPlatformProcess::CreateProc(
                *CachedGitBashPath,
                TEXT(""),
                true,
                false,
                false,
                nullptr,
                0,
                *FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()),
                nullptr
            );
        });

    // Setup shortcut command
    PluginCommands = MakeShareable(new FUICommandList);
    PluginCommands->MapAction(FGitBashCommands::Get().LaunchGitBash, LaunchGitBashAction);

    FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
    LevelEditorModule.GetGlobalLevelEditorActions()->Append(PluginCommands.ToSharedRef());

    // Setup UI menu
    UToolMenus::RegisterStartupCallback(
        FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FGitBashButtonModule::RegisterMenuExtensions)
    );
}

void FGitBashButtonModule::ShutdownModule()
{
    FGitBashButtonStyle::Shutdown();
    UToolMenus::UnRegisterStartupCallback(this);
    UToolMenus::UnregisterOwner(this);
    FGitBashCommands::Unregister();
}

void FGitBashButtonModule::RegisterMenuExtensions()
{
    FToolMenuOwnerScoped OwnerScoped(this);

    if (UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.StatusBar.ToolBar"))
    {
        FToolMenuSection& ToolbarSection = ToolbarMenu->AddSection("Git", TAttribute<FText>(), FToolMenuInsert("Compile", EToolMenuInsertType::After));

        ToolbarSection.AddEntry(FToolMenuEntry::InitToolBarButton(
            "GitBashButton",
            FUIAction(FExecuteAction::CreateLambda([this]()
                {
                    if (CachedGitBashPath.IsEmpty() || !FPaths::FileExists(CachedGitBashPath))
                    {
                        UE_LOG(LogTemp, Error, TEXT("Git Bash path not cached or invalid."));
                        return;
                    }

                    FPlatformProcess::CreateProc(
                        *CachedGitBashPath,
                        TEXT(""),
                        true,
                        false,
                        false,
                        nullptr,
                        0,
                        *FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()),
                        nullptr
                    );
                })),
            FText::GetEmpty(),
            LOCTEXT("GitBashTooltip", "Launch Git Bash"),
            FSlateIcon("GitBashButtonStyle", "GitBash.LaunchIcon")
        ));
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGitBashButtonModule, GitBashButton)
