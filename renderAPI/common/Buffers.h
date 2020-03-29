#pragma once

#include "../../common/Include.h"

class VertexBuffer {
public:
    virtual void   Bind()     = 0;
    virtual size_t GetCount() = 0;
    virtual void SetData(const void* buff, const size_t bufferSize) = 0;

public:
    template <typename CONTAINER>
    void SetData(const CONTAINER& container)
    {
        this->SetData(container.data(), container.size());
    }
};

class IndexBuffer {
public:
    virtual void   Bind()     = 0;
    virtual size_t GetCount() = 0;
    virtual void SetData(const void* buff, const size_t bufferSize) = 0;

public:
    template <typename CONTAINER>
    void SetData(const CONTAINER& container)
    {
        this->SetData(container.data(), container.size());
    }
};

class ConstantBuffer {
public:
    virtual void Bind() = 0;
    virtual void SetData(const void* buff, const size_t bufferSize) = 0;
};