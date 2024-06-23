#pragma once
#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(Location) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 20.f, 12, FColor::Red, true)
#define DRAW_LINE(StartLocation, EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f)
#define DRAW_POINT(EndLocation) if(GetWorld()) DrawDebugPoint(GetWorld(), EndLocation, 5.f, FColor::Red, true)
#define DRAW_VECTOR(StartLocation, EndLocation) DRAW_LINE(StartLocation, EndLocation); DRAW_POINT(EndLocation);