#pragma once

#include <list>

class ZC_Object
{
public:
    virtual ~ZC_Object() = default;

    virtual void Update(double prevFrameTime) {}
    virtual void Draw() {}

protected:
    ZC_Object() = default;
};