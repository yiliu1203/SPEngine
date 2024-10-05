#pragma once
#include "PCH.h"
#include "Misc/Path.h"
namespace SP {


class AssertURI
{
public:
    AssertURI() = default;
    AssertURI(const std::string& rel_path)
        : m_rel_path{rel_path}
        , m_is_empty{false}
    {
        SP_ASSERT(AssertURI::is_exist(*this), " File Not Exist :{}", this->get_abs_path());
    }
    AssertURI(const char* rel_path)
        : m_rel_path{rel_path}
        , m_is_empty{false}
    {}
    AssertURI(const AssertURI& uri) = default;
    AssertURI(AssertURI&& uri)      = default;

public:
    const std::string& get_rel_path() const { return m_rel_path; }
    const std::string  get_pure_name() const
    {
        size_t dot_index = m_rel_path.find_last_of('.');
        size_t tempindex = m_rel_path.find_last_of("/\\");
        if (tempindex == m_rel_path.npos) {
            tempindex = 0;
        }
        if (dot_index == m_rel_path.npos) {
            dot_index = m_rel_path.size();
        }
        return m_rel_path.substr(tempindex, dot_index - tempindex);
    }
    const std::string get_abs_path() const
    {
        auto path = Path::res_root / m_rel_path;
        return path.generic_string();
    }
    static bool is_exist(const AssertURI& uri)
    {
        struct stat buffer;
        return (stat(uri.get_abs_path().c_str(), &buffer) == 0);
    }


private:
    bool              m_is_empty     = true;
    bool              m_is_full_path = false;
    const std::string m_rel_path;
};

}   // namespace SP