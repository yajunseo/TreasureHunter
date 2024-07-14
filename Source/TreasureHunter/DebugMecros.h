#pragma once
#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(Location) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 20.f, 12, FColor::Red, true)
#define DRAW_SPHERE_SINGLE_FRAME(Location) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 20.f, 12, FColor::Red, false, -1.f)
#define DRAW_LINE(StartLocation, EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f)
#define DRAW_LINE_SINGLE_FRAME(StartLocation, EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f)
#define DRAW_POINT(EndLocation) if(GetWorld()) DrawDebugPoint(GetWorld(), EndLocation, 5.f, FColor::Red, true)
#define DRAW_POINT_SINGLE_FRAME(EndLocation) if(GetWorld()) DrawDebugPoint(GetWorld(), EndLocation, 5.f, FColor::Red, false, -1.f)
#define DRAW_VECTOR(StartLocation, EndLocation) DRAW_LINE(StartLocation, EndLocation); DRAW_POINT(EndLocation);
#define DRAW_VECTOR_SINGLE_FRAME(StartLocation, EndLocation) DRAW_LINE_SINGLE_FRAME(StartLocation, EndLocation); DRAW_POINT_SINGLE_FRAME(EndLocation);