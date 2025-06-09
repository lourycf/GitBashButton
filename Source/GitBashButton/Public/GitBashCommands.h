#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class FGitBashCommands : public TCommands<FGitBashCommands>
{
public:
    FGitBashCommands()
        : TCommands<FGitBashCommands>(
            TEXT("GitBash"),
            NSLOCTEXT("Contexts", "GitBash", "Git Bash Plugin"),
            NAME_None,
            FAppStyle::GetAppStyleSetName())
    {
    }

    virtual void RegisterCommands() override;

    TSharedPtr<FUICommandInfo> LaunchGitBash;
};
