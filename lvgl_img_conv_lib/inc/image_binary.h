#ifndef _INCLUDE_CLASS_IMAGE_BINARY_DATA_H_
#define _INCLUDE_CLASS_IMAGE_BINARY_DATA_H_
//===========================================================//
//= Include files.                                          =//
//===========================================================//
#include <cstdint>
#include <string>

//===========================================================//
//= Data type declare.                                      =//
//===========================================================//
typedef enum _e_color_format_
{
    img_bin_format_unknown = 0,
    img_bin_format_rgb332,
    img_bin_format_rgb565,
    img_bin_format_rgb565_swap,
    img_bin_format_argb8888,
    img_bin_format_indexed_1bit,
    img_bin_format_indexed_2bit,
    img_bin_format_indexed_4bit,
    img_bin_format_indexed_8bit,
}color_format_t;

//===========================================================//
//= Class declare.                                          =//
//===========================================================//
class image_binary
{
private:
    uint8_t*            m_data;
    int                 m_width;
    int                 m_height;
    int                 m_line_width;
    int                 m_data_size;
    color_format_t      m_format;

public:
                        image_binary(void);
                        image_binary(const image_binary& source);
                        image_binary(image_binary&& right);
    virtual             ~image_binary(void);
    void                release(void);
    bool                create(int width, int height, color_format_t color_format, bool with_alpha);
    bool                create(int width, int height, int line_width, int addition = 0);
    std::string         line_to_string(int line, bool change_line = false) const;
    int                 width(void) const;
    int                 height(void) const;
    int                 line_width(void) const;
    uint32_t            size(void) const;
    uint8_t* const      data(int offset = 0) const;
    color_format_t      format(void) const;
};

#endif // _INCLUDE_CLASS_IMAGE_BINARY_DATA_H_

