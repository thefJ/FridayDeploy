#pragma once

#include "FDTaskTypes.generated.h"

UENUM(BlueprintType)
enum class ETaskType : uint8
{
    Art UMETA(DisplayName = "Art"),
    Developing UMETA(DisplayName = "Developing"),
    Testing UMETA(DisplayName = "Testing"),
    Server UMETA(DisplayName = "Server"),
    PO UMETA(DisplayName = "PO"),
    Bug UMETA(DisplayName = "Bug")
};