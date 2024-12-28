#include "Dx11Device.h"

namespace SP::RHI {

Device* Device::s_device = new Dx11Device();
}