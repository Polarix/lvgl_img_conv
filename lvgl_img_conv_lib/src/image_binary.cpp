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
 , m_line_width(0)
 , m_data_size(0)
 , m_format(img_bin_format_unknown)
{

}

image_binary::image_binary(const image_binary& source)
 : image_binary()
{
    DBG_LOG("image_binary_base copy");
    if(create(source.m_width, source.m_height, source.m_line_width))
    {
        ::memcpy(m_data, &source.m_data, m_data_size);
    }
}

image_binary::image_binary(image_binary&& right)
 : m_data(right.m_data)
 , m_width(right.m_width)
 , m_height(right.m_height)
 , m_line_width(right.m_line_width)
 , m_data_size(right.m_data_size)
 , m_format(right.m_format)
{
    DBG_LOG("image_binary_base move");
    right.m_data = nullptr;
    right.m_width = 0;
    right.m_height = 0;
    right.m_data_size = 0;
    right.m_line_width = 0;
}

image_binary::~image_binary(void)
{
    release();
}

bool image_binary::create(int width, int height, color_format_t color_format, bool with_alpha)
{
    bool result = false;
    int line_width;
    int additional = 0;
    switch(color_format)
    {
        case img_bin_format_rgb332:
        {
            line_width = (1 + (with_alpha?1:0)) * width;
            break;
        }
        case img_bin_format_rgb565:
        case img_bin_format_rgb565_swap:
        {
            line_width = (2 + (with_alpha?1:0)) * width;
            break;
        }
        case img_bin_format_argb8888:
        {
            line_width = 4 * width;
            break;
        }
        case img_bin_format_indexed_1bit:
        {
            line_width = (width - 1) / 8 + 1;
            additional = (0x01 << 1);
            break;
        }
        case img_bin_format_indexed_2bit:
        {
            line_width = (width - 1) / 4 + 1;
            additional = (0x01 << 2);
            break;
        }
        case img_bin_format_indexed_4bit:
        {
            line_width = (width - 1) / 2 + 1;
            additional = (0x01 << 4);
            break;
        }
        case img_bin_format_indexed_8bit:
        {
            line_width = width;
            additional = (0x01 << 8);
            break;
        }
        case img_bin_format_alpha_1bit:
        {
            line_width = (width - 1) / 8 + 1;
            break;
        }
        case img_bin_format_alpha_2bit:
        {
            line_width = (width - 1) / 4 + 1;
            break;
        }
        case img_bin_format_alpha_4bit:
        {
            line_width = (width - 1) / 2 + 1;
            break;
        }
        case img_bin_format_alpha_8bit:
        {
            line_width = width;
            break;
        }
        default:
        {
            line_width = 0;
        }
    }

    if(width && height && line_width)
    {
        result = create(width, height, line_width, additional);
        m_format = color_format;
    }
    return result;
}

bool image_binary::create(int width, int height, int line_width, int addition)
{
    bool result = false;
    if(width && height && line_width)
    {
        m_width = width;
        m_height = height;
        m_line_width = line_width;
        m_data_size = line_width * m_height + addition;
        m_data = new uint8_t[m_data_size];
        if(m_data)
        {
            ::memset(m_data, 0x00, m_data_size);
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
        m_line_width = 0;
    }
}

std::string image_binary::line_to_string(int line, bool change_line) const
{
    std::ostringstream oss;

    if((m_data) && (line < m_height))
    {
        int start_index = m_line_width * line;
        for(int byte_idx=0; byte_idx<m_line_width; ++byte_idx)
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

int image_binary::line_width(void) const
{
    return m_line_width;
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
