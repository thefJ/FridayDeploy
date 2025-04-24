#pragma once

#include "FDBacklogTypes.generated.h"

UENUM(BlueprintType)
enum class EBacklogType : uint8
{
    Default UMETA(DisplayName = "Default"),
    Server UMETA(DisplayName = "Server")
};