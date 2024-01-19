#ifndef _INCLUDE_CLASS_SOURCE_CODE_FILE_GENERATOR_H_
#define _INCLUDE_CLASS_SOURCE_CODE_FILE_GENERATOR_H_
//===========================================================//
//= Include files.                                          =//
//===========================================================//
#include <generator_base.h>
#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>

//===========================================================//
//= Class declare.                                          =//
//===========================================================//
class code_generator : public generator_base
{
private:
    std::ofstream           m_source_file;

protected:
    virtual void            output_img_header(void);
    virtual void            output_img_bmp_line(const void* data, size_t len);
    virtual void            output_img_dsc_data(void);

public:
    using generator_base::generator_base;
    virtual bool            prepare(const std::string& output_path);
    virtual bool            generate_data(const image_binary& binary_data);
    virtual void            finialize(void);
};

#endif // _INCLUDE_CLASS_SOURCE_CODE_FILE_GENERATOR_H_

