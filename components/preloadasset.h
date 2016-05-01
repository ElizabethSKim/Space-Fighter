#ifndef PRELOADASSET_H
#define PRELOADASSET_H

#include <functional>
#include "engine.h"

class PreloadAsset
{
public:
    PreloadAsset(std::function<void(Engine*)> cb);
};

#endif // PRELOADASSET_H
