#ifndef _INCLUDE_CLASS_IMAGE_RAW_DATA_H_
#define _INCLUDE_CLASS_IMAGE_RAW_DATA_H_
//===========================================================//
//= Include files.                                          =//
//===========================================================//
#include <cstdint>
#include <string>
#include <image_binary.h>

//===========================================================//
//= Macro definition.                                       =//
//===========================================================//

//===========================================================//
//= Data type declare.                                      =//
//===========================================================//
/* 8Bits RGB332 format pixel data. */
typedef union
{
    uint8_t full;
    struct
    {
        uint8_t     blue : 2;
        uint8_t     green : 3;
        uint8_t     red : 3;
        uint8_t     alpha;
    } ch;
}rgb332_px_t;

/* 16Bits RGB565 format pixel data. */
typedef union
{
    uint16_t full;
    struct
    {
        uint16_t    blue : 5;
        uint16_t    green : 6;
        uint16_t    red : 5;
        uint8_t     alpha;
    } ch;
    uint8_t         byte[3];
}rgb565_px_t;

/* 32Bits ARGB8888 format pixel data. */
typedef union
{
    uint32_t full;
    struct
    {
        uint8_t     blue;
        uint8_t     green;
        uint8_t     red;
        uint8_t     alpha;
    } ch;
}argb32_px_t;

typedef union _st_img_raw_px_
{
    uint32_t full;
    struct
    {
        uint8_t     red;
        uint8_t     green;
        uint8_t     blue;
        uint8_t     alpha;
    }ch;
    uint8_t         bytes[4];
}img_raw_px_data;

//===========================================================//
//= Class declare.                                          =//
//===========================================================//
class image_raw_data
{
private:
    uint8_t*            m_data;
    int                 m_data_size;
    int                 m_width;
    int                 m_height;

    void                copy(const image_raw_data& source);
    void                force_release(void);
public:
    explicit            image_raw_data(void);
    explicit            image_raw_data(const std::string& image_file_path);
    explicit            image_raw_data(const image_raw_data& source);
    explicit            image_raw_data(image_raw_data&& right);
    virtual             ~image_raw_data(void);
    bool                load(const std::string& image_file_path);
    bool                is_ok(void) const;
    int                 get_width(void) const;
    int                 get_height(void) const;
    bool                create_8b_binary(image_binary& bin, bool with_alpha) const;
    bool                create_16b_binary(image_binary& bin, bool with_alpha, bool swap) const;
    bool                create_32b_binary(image_binary& bin, bool with_alpha) const;
};

#endif // _INCLUDE_CLASS_IMAGE_RAW_DATA_H_

