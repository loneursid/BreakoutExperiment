#pragma once

// Minimal math types — identical layout to raylib's Vector2 / Rectangle
// so no casting is needed when passing to raylib functions in Renderer.cpp.

#ifndef RAYLIB_H
struct Vector2   { float x, y; };
struct Rectangle { float x, y, width, height; };
#endif
