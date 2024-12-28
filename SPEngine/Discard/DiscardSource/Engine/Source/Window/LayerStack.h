#pragma once

#include "Layer.h"
#include <vector>


namespace SP {
class SP_API LayerStack
{
public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);
    void PopLayer(Layer* layer);
    void popOverlay(Layer* layer);

    std::vector<Layer*>::iterator begin() { return _Layers.begin(); }
    std::vector<Layer*>::iterator end() { return _Layers.end(); }


private:
    std::vector<Layer*> _Layers;
    unsigned int        _PushLayerIndex;
};
}   // namespace SP
