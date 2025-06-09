#include "GitBashCommands.h"

#define LOCTEXT_NAMESPACE "FGitBashCommands"

void FGitBashCommands::RegisterCommands()
{
	UI_COMMAND(LaunchGitBash, "Launch Git Bash", "Launch Git Bash terminal", EUserInterfaceActionType::Button, FInputChord(EKeys::X, EModifierKey::Alt));
}

#undef LOCTEXT_NAMESPACE
