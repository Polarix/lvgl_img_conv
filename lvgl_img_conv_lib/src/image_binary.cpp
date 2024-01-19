//===========================================================//
//= Include files.                                          =//
//===========================================================//
#include <image_binary.h>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <log.h>

//===========================================================//
//= Function definition.                                    =//
//===========================================================//
image_binary::image_binary(void)
 : m_data(nullptr)
 , m_width(0)
 , m_height(0)
 , m_byte_per_pixel(0)
 , m_data_size(0)
 , m_format(img_bin_color_format_unknown)
{

}

image_binary::image_binary(const image_binary& source)
 : image_binary()
{
    DBG_LOG("image_binary_base copy");
    if(create(source.m_width, source.m_height, source.m_byte_per_pixel))
    {
        ::memcpy(m_data, &source.m_data, m_data_size);
    }
}

image_binary::image_binary(image_binary&& right)
 : m_data(right.m_data)
 , m_width(right.m_width)
 , m_height(right.m_height)
 , m_byte_per_pixel(right.m_byte_per_pixel)
 , m_data_size(right.m_data_size)
 , m_format(right.m_format)
{
    DBG_LOG("image_binary_base move");
    right.m_data = nullptr;
    right.m_width = 0;
    right.m_height = 0;
    right.m_data_size = 0;
    right.m_byte_per_pixel = 0;
}

image_binary::~image_binary(void)
{
    release();
}

bool image_binary::create(int width, int height, color_format_t color_format, bool with_alpha)
{
    bool result = false;
    int byte_per_px;
    switch(color_format)
    {
        case img_bin_color_format_rgb332:
        {
            byte_per_px = 1 + (with_alpha?1:0);
            break;
        }
        case img_bin_color_format_rgb565:
        case img_bin_color_format_rgb565_swap:
        {
            byte_per_px = 2 + (with_alpha?1:0);
            break;
        }
        case img_bin_color_format_argb8888:
        {
            byte_per_px = 4;
            break;
        }
        default:
        {
            byte_per_px = 0;
        }
    }

    if(width && height && byte_per_px)
    {
        result = create(width, height, byte_per_px);
        m_format = color_format;
    }
    return result;
}

bool image_binary::create(int width, int height, int pixel_data_width, int offset)
{
    bool result = false;
    if(width && height && pixel_data_width)
    {
        m_width = width;
        m_height = height;
        m_byte_per_pixel = pixel_data_width;
        m_data_size = m_width * m_height * m_byte_per_pixel + offset;
        m_data = new uint8_t[m_data_size];
        if(m_data)
        {
            result = true;
        }
    }
    return result;
}

void image_binary::release(void)
{
    if(m_data)
    {
        delete[] m_data;
        m_data = nullptr;
        m_width = 0;
        m_height = 0;
        m_data_size = 0;
        m_byte_per_pixel = 0;
    }
}

std::string image_binary::line_to_string(int line, bool change_line) const
{
    std::ostringstream oss;

    if((m_data) && (line < m_height))
    {
        int start_index = m_byte_per_pixel * m_width * line;
        int out_count = m_byte_per_pixel * m_width;
        for(int byte_idx=0; byte_idx<out_count; ++byte_idx)
        {
            oss << "0x" << std::setw(2) << std::setfill('0') << std::hex << m_data[start_index++] << ", ";
            oss.flush();
        }
        if(change_line)
        {
            oss << std::endl;
        }
    }

    return oss.str();
}

int image_binary::width(void) const
{
    return m_width;
}

int image_binary::height(void) const
{
    return m_height;
}

int image_binary::pixel_data_width(void) const
{
    return m_byte_per_pixel;
}

uint32_t image_binary::size(void) const
{
    return m_data_size;
}

uint8_t* const image_binary::data(int offset) const
{
    return m_data + offset;
}

color_format_t image_binary::format(void) const
{
    return m_format;
}
