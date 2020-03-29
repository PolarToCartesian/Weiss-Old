#pragma once

#include "../../common/Include.h"

struct ShaderInputElement {
    const char* name;
    ShaderInputElementType type;
};

class VertexShader {
public:
    virtual void Bind() = 0;
};

class PixelShader {
public:
    virtual void Bind() = 0;
};