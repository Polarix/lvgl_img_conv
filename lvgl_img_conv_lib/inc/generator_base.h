#ifndef _INCLUDE_CLASS_CODE_GENERATOR_H_
#define _INCLUDE_CLASS_CODE_GENERATOR_H_
//===========================================================//
//= Include files.                                          =//
//===========================================================//
#include <cstdint>
#include <ostream>
#include <image_raw_data.h>
#include <string>
#include <utility>
#include <image_binary.h>

//===========================================================//
//= Data type declare.                                      =//
//===========================================================//
typedef struct
{
    uint8_t     val;
    const char* name;
}image_cf;

typedef struct
{
#if BIG_ENDIAN_BINARY
        uint32_t height : 11;
        uint32_t width : 11;
        uint32_t reserved : 2;
        uint32_t always_zero : 3;
        uint32_t color_format : 5;
#else
        uint32_t color_format : 5;  /* Color format: See `lv_img_color_format_t` */
        uint32_t always_zero : 3;   /* It the upper bits of the first byte. Always zero to look like a non-printable character */
        uint32_t reserved : 2;      /* Reserved to be used later */
        uint32_t width : 11;        /* Width of the image map */
        uint32_t height : 11;       /* Height of the image map */
#endif
}image_dsc_header_t;

typedef struct {
    image_dsc_header_t header;
    uint32_t data_size;             /* Size of the image in bytes*/
}image_dsc_t;

//===========================================================//
//= Class declare.                                          =//
//===========================================================//
class generator_base
{
protected:
    image_raw_data          m_image_raw;
    std::string             cf_name(uint8_t val);
    uint8_t                 cf_code(const std::string& get_res_name);
    image_dsc_t             m_image_dsc_data;
    std::string             m_name;

    virtual void            output_img_header(void) = 0;
    virtual void            output_img_bmp_line(const void* data, size_t len);
    virtual void            output_img_dsc_data(void) = 0;

public:
    explicit                generator_base(void);
    virtual                 ~generator_base(void);
    bool                    open(const std::string& image_path);
    bool                    output_img_bit_map(const image_binary& binary_data);
    const image_raw_data&   image_raw(void) const;
    const std::string&      get_res_name(void) const;
    void                    set_res_name(const std::string& new_name);
    void                    set_color_format(uint8_t lv_color_format);
    void                    set_color_format(const std::string& lv_color_format);
};

#endif // _INCLUDE_CLASS_CODE_GENERATOR_H_

