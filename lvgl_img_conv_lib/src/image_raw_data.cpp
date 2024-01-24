//===========================================================//
//= Include files.                                          =//
//===========================================================//
#include <image_raw_data.h>
#include <stb_image.h>
#include <stb_image_write.h>
#include <log.h>

//===========================================================//
//= Function definition.                                    =//
//===========================================================//
image_raw_data::image_raw_data(void)
 : m_data(nullptr)
 , m_data_size(0)
 , m_width(0)
 , m_height(0)
{

}

image_raw_data::image_raw_data(const std::string& image_file_path)
 : image_raw_data()
{
    load(image_file_path);
}

image_raw_data::image_raw_data(const image_raw_data& source)
 : m_data(nullptr)
 , m_data_size(source.m_data_size)
 , m_width(source.m_width)
 , m_height(source.m_height)
{
    m_data = new uint8_t[m_data_size];
    if(m_data)
    {
        ::memcpy(m_data, source.m_data, m_data_size);
    }
    else
    {
        force_release();
    }
}

image_raw_data::image_raw_data(image_raw_data&& right)
 : m_data(right.m_data)
 , m_data_size(right.m_data_size)
 , m_width(right.m_width)
 , m_height(right.m_height)
{
    right.m_data = nullptr;
    right.m_data_size = 0;
    right.m_width = 0;
    right.m_height = 0;
}

image_raw_data::~image_raw_data(void)
{
    force_release();
}

void image_raw_data::copy(const image_raw_data& source)
{
    m_data = new uint8_t[source.m_data_size];
    if(m_data)
    {
        memcpy(m_data, source.m_data, source.m_data_size);
    }
    else
    {
        m_width = source.m_width;
        m_height = source.m_height;
        m_data_size = source.m_data_size;
    }
}

void image_raw_data::force_release(void)
{
    if(m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }
    m_width = 0;
    m_height = 0;
    m_data_size = 0;
}

bool image_raw_data::load(const std::string& image_file_path)
{
    bool result = false;
    int image_ch;
    /* Release old data if existed. */
    force_release();
    /* Force read image data as ARGB format. */
    m_data = ::stbi_load(image_file_path.c_str(), &m_width, &m_height, &image_ch, STBI_rgb_alpha);
    if(m_data)
    {
        if((m_width > 2047) || (m_height > 2047))
        {
            ERR_LOG("Image size too large(%d x %d), width and height COUNT NOT large then 2047 px.", m_width, m_height);
            force_release();
        }
        else
        {
            m_data_size = m_width * m_height * 4;
            DBG_LOG("Opened file %s, width %d, height %d, data size %d.", image_file_path.c_str(), m_width, m_height, m_data_size);
            result = true;
        }
    }
    else
    {
        ERR_LOG("Load %s failed.", image_file_path.c_str());
        result = false;
    }

    return result;
}

bool image_raw_data::is_ok(void) const
{
    bool result = (m_data != nullptr) && (m_width > 0) && (m_height > 0);
    return result;
}

int image_raw_data::get_width(void) const
{
    return m_width;
}

int image_raw_data::get_height(void) const
{
    return m_height;
}

bool image_raw_data::create_8b_binary(image_binary& bin, bool with_alpha) const
{
    bool result = false;
    int pixel_count = m_width * m_height;
    int pixel_idx = 0;
    int pixel_data_len = with_alpha?2:1;
    /* Release existed data. */
    bin.release();
    /* Allocate new memory. */
    if(bin.create(m_width, m_height, img_bin_format_rgb332, with_alpha))
    {
        const img_raw_px_data* raw_px = reinterpret_cast<const img_raw_px_data*>(m_data);
        rgb332_px_t rgb332_val;
        /* Fill data. */
        while(pixel_idx < pixel_count)
        {
            rgb332_val.ch.red = raw_px[pixel_idx].ch.red >> 5;
            rgb332_val.ch.green = raw_px[pixel_idx].ch.green >> 5;
            rgb332_val.ch.blue = raw_px[pixel_idx].ch.blue >> 6;
            rgb332_val.ch.alpha = raw_px[pixel_idx].ch.alpha;
            ::memcpy(bin.data() + (pixel_idx * pixel_data_len), &rgb332_val, pixel_data_len);
            ++pixel_idx;
        }
        result = true;
    }
    return result;
}

bool image_raw_data::create_16b_binary(image_binary& bin, bool with_alpha, bool swap) const
{
    bool result = false;
    int pixel_count = m_width * m_height;
    int pixel_idx = 0;
    int pixel_data_len = with_alpha?3:2;
    /* Release existed data. */
    bin.release();
    /* Allocate new memory. */
    if(bin.create(m_width, m_height, swap?img_bin_format_rgb565_swap:img_bin_format_rgb565, with_alpha))
    {
        const img_raw_px_data* raw_px = reinterpret_cast<const img_raw_px_data*>(m_data);
        rgb565_px_t rgb565_val;
        /* Fill data. */
        while(pixel_idx < pixel_count)
        {
            rgb565_val.ch.red = raw_px[pixel_idx].ch.red >> 3;
            rgb565_val.ch.green = raw_px[pixel_idx].ch.green >> 2;
            rgb565_val.ch.blue = raw_px[pixel_idx].ch.blue >> 3;
            rgb565_val.ch.alpha = raw_px[pixel_idx].ch.alpha;
            if(swap)
            {
                uint8_t swap_val = rgb565_val.byte[1];
                rgb565_val.byte[1] = rgb565_val.byte[0];
                rgb565_val.byte[0] = swap_val;
            }
            ::memcpy(bin.data() + (pixel_idx * pixel_data_len), &rgb565_val, pixel_data_len);
            ++pixel_idx;
        }
    }

    return result;
}

bool image_raw_data::create_32b_binary(image_binary& bin, bool with_alpha) const
{
    bool result = false;
    int pixel_count = m_width * m_height;
    int pixel_idx = 0;
    uint8_t alpha_mask = with_alpha?0x00:0xFF;
    /* Release existed data. */
    bin.release();
    /* Allocate new memory, force append alpha channel. */
    if(bin.create(m_width, m_height, img_bin_format_argb8888, true))
    {
        const img_raw_px_data* raw_px = reinterpret_cast<const img_raw_px_data*>(m_data);
        argb32_px_t argb32_val;
        /* Fill data. */
        while(pixel_idx < pixel_count)
        {
            argb32_val.ch.red = raw_px[pixel_idx].ch.red;
            argb32_val.ch.green = raw_px[pixel_idx].ch.green;
            argb32_val.ch.blue = raw_px[pixel_idx].ch.blue;
            argb32_val.ch.alpha = raw_px[pixel_idx].ch.alpha | alpha_mask;
            ::memcpy(bin.data() + (pixel_idx * 4), &argb32_val, 4);
            ++pixel_idx;
        }
    }

    return result;
}

bool image_raw_data::create_1bit_alpha_binary(image_binary& bin) const
{
    bool result = false;
    uint8_t divisor = 0xFF / (0x01 << 1) + 1;
    /* Release existed data. */
    bin.release();
    /* Allocate new memory, force append alpha channel. */
    if(bin.create(m_width, m_height, img_bin_format_alpha_1bit, true))
    {
        const img_raw_px_data* raw_data = reinterpret_cast<const img_raw_px_data*>(m_data);
        for(int row_idx=0; row_idx<m_height; ++row_idx)
        {
            /* Src line head. */
            const img_raw_px_data* raw_line_data = raw_data + (m_width * row_idx);
            /* Dest line head. */
            uint8_t* img_bin_data = bin.data() + (bin.line_width() * row_idx);
            for(int col_idx=0; col_idx<m_width; ++col_idx)
            {
                if(raw_line_data[col_idx].ch.alpha / divisor)
                {
                    img_bin_data[col_idx / 8] |= (0x80 >> (col_idx % 8));
                }
            }
        }
    }
    return result;
}

bool image_raw_data::create_2bit_alpha_binary(image_binary& bin) const
{
    bool result = false;
    uint8_t divisor = 0xFF / (0x01 << 2) + 1;
    /* Release existed data. */
    bin.release();
    /* Allocate new memory, force append alpha channel. */
    if(bin.create(m_width, m_height, img_bin_format_alpha_2bit, true))
    {
        const img_raw_px_data* raw_data = reinterpret_cast<const img_raw_px_data*>(m_data);
        for(int row_idx=0; row_idx<m_height; ++row_idx)
        {
            /* Src line head. */
            const img_raw_px_data* raw_line_data = raw_data + (m_width * row_idx);
            /* Dest line head. */
            uint8_t* img_bin_data = bin.data() + (bin.line_width() * row_idx);
            for(int col_idx=0; col_idx<m_width; ++col_idx)
            {
                uint8_t ch_val = raw_line_data[col_idx].ch.alpha / divisor;
                img_bin_data[col_idx / 4] |= ((ch_val << 6) >> ((col_idx % 4) * 2));
            }
        }
    }
    return result;
}

bool image_raw_data::create_4bit_alpha_binary(image_binary& bin) const
{
    bool result = false;
    uint8_t divisor = 0xFF / (0x01 << 4) + 1;
    /* Release existed data. */
    bin.release();
    /* Allocate new memory, force append alpha channel. */
    if(bin.create(m_width, m_height, img_bin_format_alpha_4bit, true))
    {
        const img_raw_px_data* raw_data = reinterpret_cast<const img_raw_px_data*>(m_data);
        for(int row_idx=0; row_idx<m_height; ++row_idx)
        {
            /* Src line head. */
            const img_raw_px_data* raw_line_data = raw_data + (m_width * row_idx);
            /* Dest line head. */
            uint8_t* img_bin_data = bin.data() + (bin.line_width() * row_idx);
            for(int col_idx=0; col_idx<m_width; ++col_idx)
            {
                uint8_t ch_val = raw_line_data[col_idx].ch.alpha / divisor;
                img_bin_data[col_idx / 2] |= ((ch_val << 4) >> ((col_idx % 2) * 4));
            }
        }
    }
    return result;
}

bool image_raw_data::create_8bit_alpha_binary(image_binary& bin) const
{
    bool result = false;
    int pixel_count = m_width * m_height;
    int pixel_idx = 0;
    /* Release existed data. */
    bin.release();
    /* Allocate new memory, force append alpha channel. */
    if(bin.create(m_width, m_height, img_bin_format_alpha_8bit, true))
    {
        const img_raw_px_data* raw_data = reinterpret_cast<const img_raw_px_data*>(m_data);
        uint8_t* img_bin_data = bin.data();
        /* Fill data. */
        while(pixel_idx < pixel_count)
        {
            img_bin_data[pixel_idx] = raw_data[pixel_idx].ch.alpha;
            ++pixel_idx;
        }
    }
    return result;
}
