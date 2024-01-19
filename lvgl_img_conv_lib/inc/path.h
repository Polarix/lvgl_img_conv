#ifndef _INCLUDED_CLASS_PATH_H_
#define _INCLUDED_CLASS_PATH_H_
//===========================================================//
//= Include files.                                          =//
//===========================================================//
#include <string>

//===========================================================//
//= Class declare.                                          =//
//===========================================================//
class file_path
{
private:
    std::string     m_full_path;
    std::string     m_path_split_symbol;

public:
    explicit        file_path(void);
    explicit        file_path(const std::string& file_path_str);
    std::string     path(void) const;
    std::string     name(void) const;
    std::string     extension(void) const;
};

#endif // _INCLUDED_CLASS_PATH_H_
