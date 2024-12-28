#include "LayerStack.h"
#include "PCH.h"


namespace SP {

LayerStack::LayerStack()
    : _PushLayerIndex{0}
{}

LayerStack::~LayerStack()
{
    for (Layer* layer : _Layers) delete layer;
}

void LayerStack::PushLayer(Layer* layer)
{
    _Layers.emplace(_Layers.begin() + _PushLayerIndex, layer);
    _PushLayerIndex++;
}

void LayerStack::PushOverlay(Layer* layer)
{
    _Layers.emplace_back(layer);
}

void LayerStack::PopLayer(Layer* layer)
{
    auto it = find(_Layers.begin(), _Layers.end(), layer);
    if (it != _Layers.end()) {
        size_t finded_index = it - _Layers.begin();
        if (_PushLayerIndex <= finded_index) {
            _Layers.erase(it);
        }
        else if (_PushLayerIndex > finded_index) {
            _Layers.erase(it);
            _PushLayerIndex--;
        }
    }
}

void LayerStack::popOverlay(Layer* layer)
{
    PopLayer(layer);
}




}   // namespace SP
