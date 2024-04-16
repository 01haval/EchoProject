#pragma once
#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(),Location,25.f,12,FColor::Blue,true);
#define DRAW_SPHERE_COLOR_FOR_5S(Location,Color) if (GetWorld()) DrawDebugSphere(GetWorld(),Location,8.f,12,Color,false,5.f);
#define DRAW_SPHERE_SingleFrame(Location) if  (GetWorld()) DrawDebugSphere(GetWorld(),Location,25.f,12,FColor::Blue,false,-1.f);
#define DRAW_LINE(Location,EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(),Location,EndLocation,FColor::Red,true,-1.f,0,1.f);
#define DRAW_LINE_SingleFrame(Location,EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(),Location,EndLocation,FColor::Red,false,-1.f,0,1.f);
#define DRAW_POINT(Location,Color) if (GetWorld()) DrawDebugPoint(GetWorld(),Location,15,Color,true);
#define DRAW_POINT_SingleFrame(Location,Color) if (GetWorld()) DrawDebugPoint(GetWorld(),Location,15,Color,false,-1.f);
#define DRAW_VECTOR(StartLocation,EndLocation)\
	 if (GetWorld()) \
	{  \
		DrawDebugLine(GetWorld(),StartLocation,EndLocation,FColor::Red,true,-1.f,0,1.f);\
		DrawDebugPoint(GetWorld(),EndLocation,15,FColor::Blue,true);\
	};
#define DRAW_VECTOR_SingleFrame(StartLocation,EndLocation)\
	 if (GetWorld()) \
	{  \
		DrawDebugLine(GetWorld(),StartLocation,EndLocation,FColor::Red,false,-1.f,0,1.f);\
		DrawDebugPoint(GetWorld(),EndLocation,15,FColor::Blue,false,-1.f);\
	};
#define DRAW_BOX(StartLocation,EndLocation)\
	 if (GetWorld()) DrawDebugBox(World, Location, Forword * 300 + Location, FColor::Yellow, true);