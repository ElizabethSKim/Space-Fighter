#include "preloadasset.h"
#include "engine.h"
PreloadAsset::PreloadAsset(std::function<void(Engine*)> cb)
{
    Engine::onLoad(cb);
}
