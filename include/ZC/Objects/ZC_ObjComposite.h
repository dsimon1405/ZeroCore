#pragma once

#include "ZC_Object.h"
#include <ZC/Tools/ZC_uptr.h>

#include <list>

class ZC_ObjComposite : public ZC_Object
{
public:
    ~ZC_ObjComposite() override = default;

private:
    std::list<ZC_uptr<ZC_Object>> objects;

    ZC_ObjComposite() = default;
};