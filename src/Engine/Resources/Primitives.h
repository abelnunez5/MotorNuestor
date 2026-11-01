#pragma once

#include "Resources/Mesh.h"

// Basic shapes, every one is at the origin (1 unit wide, the plane is 10x10)
namespace Primitives {

enum class Type { Cube, Sphere, Plane, Cylinder };

inline constexpr Type kAll[] = { Type::Cube, Type::Sphere, Type::Plane, Type::Cylinder };

const char* Name(Type type);

Mesh Create(Type type);

}
