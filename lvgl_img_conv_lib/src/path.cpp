#include <path.h>

//===========================================================//
//= Static variable definition.                             =//
//===========================================================//

//===========================================================//
//= Global variable definition.                             =//
//===========================================================//
constexpr char path_split_symbol[] = "/\\";

//===========================================================//
//= Function definition.                                    =//
//===========================================================//
file_path::file_path(void)
 : m_path_split_symbol(path_split_symbol)
{

}

file_path::file_path(const std::string& file_path_str)
 : m_full_path(file_path_str)
 , m_path_split_symbol(path_split_symbol)
{

}

std::string file_path::path(void) const
{
    size_t last_split_found = m_full_path.find_last_of(m_path_split_symbol);
    return (last_split_found != std::string::npos) ? m_full_path.substr(0, last_split_found) : m_full_path;
}

std::string file_path::name(void) const
{
    size_t last_path_split_found = m_full_path.find_last_of(m_path_split_symbol);
    std::string file_full_name = (last_path_split_found != std::string::npos) ? m_full_path.substr(last_path_split_found + 1) : m_full_path;
    size_t extension_found = file_full_name.find_last_of(".");
    return (extension_found != std::string::npos) ? file_full_name.substr(0, extension_found) : file_full_name;
}

std::string file_path::extension(void) const
{
    size_t found = m_full_path.find_last_of(".");
    return (found != std::string::npos) ? m_full_path.substr(found + 1) : "";
}
