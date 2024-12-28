#include "OpenGLVertexArray.h"
#include "Graphics/APreRHI.h"
#include "OpenGLBuffer.h"
#include "OpenGLHelpers.h"
#include "glad/glad.h"



namespace SP::RHI {
OpenGLVertexArray::OpenGLVertexArray()
{
    glGenVertexArrays(1, &m_renderid);
}
OpenGLVertexArray::~OpenGLVertexArray()
{
    SP_ASSERT(m_renderid > 0);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &m_renderid);
    m_renderid = 0;
}

void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer>& vb)
{
    glBindVertexArray(m_renderid);
    vb->Bind();
    m_vertexbuffers.push_back(vb);
    E_VertexFormateLayout formate_layout = vb->GetLayout();
    const auto&           info           = g_vfl_config(formate_layout);
    size_t                offset         = 0;
    size_t                stride         = info.stride_bytes;
    for (size_t i = 0; i < info.comp_count; i++) {

        auto gldata_info = GetGLDataType(info.comp_list[i].data_type);
        glVertexAttribPointer(i,
                              info.comp_list[i].element_count,
                              gldata_info.GLDataType,
                              info.comp_list[i].is_normaized ? GL_FALSE : GL_TRUE,
                              stride,
                              (void*)offset);
        offset += gldata_info.stride;
        glEnableVertexAttribArray(i);
    }

    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}
void OpenGLVertexArray::AddIndexBuffer(std::shared_ptr<IndexBuffer>& ib)
{
    glBindVertexArray(m_renderid);
    m_indexbuffer = ib;
    m_indexbuffer->Bind();
}
void OpenGLVertexArray::Bind() const
{
    glBindVertexArray(m_renderid);
}
void OpenGLVertexArray::UnBind() const
{
    glBindVertexArray(0);
}

}   // namespace SP::RHI