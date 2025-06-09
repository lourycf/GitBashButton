#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class FGitBashButtonStyle
{
public:
    static void Initialize();
    static void Shutdown();

    static const ISlateStyle& Get();
    static FName GetStyleSetName();

private:
    static TSharedPtr<FSlateStyleSet> StyleInstance;
};