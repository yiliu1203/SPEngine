#include "OpenGLHelpers.h"

namespace SP::RHI {


GLDataInfo GetGLDataType(E_DataType datatype)
{
    switch (datatype) {
    case E_DataType::Floatx1:
        return {OpenGLDataType<E_DataType::Floatx1>::GLDataType, OpenGLDataType<E_DataType::Floatx1>::ByteSize};
        break;

    case E_DataType::Floatx2:
        return {OpenGLDataType<E_DataType::Floatx2>::GLDataType, OpenGLDataType<E_DataType::Floatx2>::ByteSize};
        break;

    case E_DataType::Floatx3:
        return {OpenGLDataType<E_DataType::Floatx3>::GLDataType, OpenGLDataType<E_DataType::Floatx3>::ByteSize};
        break;

    case E_DataType::Floatx4:
        return {OpenGLDataType<E_DataType::Floatx4>::GLDataType, OpenGLDataType<E_DataType::Floatx4>::ByteSize};
        break;
    default: break;
    }
    return {0, 0};
}
}   // namespace SP::RHI