#include "Core/APre.h"
#include "Core/AssertRUI.h"

NS_BEGIN

class Texture
{
private:
    unsigned int         m_RenderID;
    const AssertRUI      m_Path;
    const unsigned char* m_Buffer;
    int                  m_Width;
    int                  m_Height;
    int                  m_BPP;

public:
    Texture(const std::string& path);
    ~Texture();
    void Bind(unsigned int slot = 0);
    void UnBind();
};

NS_END