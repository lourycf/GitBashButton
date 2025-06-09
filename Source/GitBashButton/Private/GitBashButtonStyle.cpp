#include "GitBashButtonStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "Brushes/SlateImageBrush.h"

TSharedPtr<FSlateStyleSet> FGitBashButtonStyle::StyleInstance = nullptr;

void FGitBashButtonStyle::Initialize()
{
    if (!StyleInstance.IsValid())
    {
        const FString PluginDir = IPluginManager::Get().FindPlugin(TEXT("GitBashButton"))->GetBaseDir();
        const FString IconPath = FPaths::Combine(PluginDir, TEXT("Resources/GitBashIconSmall.png"));
        const FVector2D IconSize(20.0f, 20.0f);

        StyleInstance = MakeShared<FSlateStyleSet>("GitBashButtonStyle");
        StyleInstance->SetContentRoot(PluginDir);

        StyleInstance->Set("GitBash.LaunchIcon", new FSlateImageBrush(IconPath, IconSize));

        FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
    }
}

void FGitBashButtonStyle::Shutdown()
{
    if (StyleInstance.IsValid())
    {
        FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
        StyleInstance.Reset();
    }
}

const ISlateStyle& FGitBashButtonStyle::Get()
{
    check(StyleInstance.IsValid());
    return *StyleInstance;
}

FName FGitBashButtonStyle::GetStyleSetName()
{
    return "GitBashButtonStyle";
}
