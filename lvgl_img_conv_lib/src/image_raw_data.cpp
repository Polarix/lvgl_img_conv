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
 , m_type(img_data_type_unknown)
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
 , m_type(source.m_type)
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
 , m_type(right.m_type)
 , m_width(right.m_width)
 , m_height(right.m_height)
{
    right.m_data = nullptr;
    right.m_data_size = 0;
    right.m_type = img_data_type_unknown;
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
        m_type = source.m_type;
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
    m_type = img_data_type_unknown;
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
            m_type = img_data_type_true_color;
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

bool image_raw_data::create_gray(const image_raw_data& source)
{
    bool result = false;

    if(source.m_data == m_data)
    {
        ERR_LOG("Inavlid input source, CANNOT NOT output to self.");
    }
    else if(source.is_ok())
    {
        if(source.m_type == img_data_type_gray)
        {
            /* Source is gray image, copy directly. */
            copy(source);
        }
        else if(source.m_type == img_data_type_true_color)
        {
            /* Source is true color, convert. */
            /* Release existed data if existed. */
            force_release();
            m_width = source.m_width;
            m_height = source.m_height;
            m_data_size = m_width * m_height;
            m_data = new uint8_t[m_data_size];
            for(int pidx=0; pidx<m_data_size; ++pidx)
            {
                // Gray value = 0.299¡ÁR+0.587¡ÁG+0.114¡ÁB
                unsigned int gray_val = source.m_data[pidx*4+argb_idx_r] * 299 / 1000 +
                                        source.m_data[pidx*4+argb_idx_g] * 587 / 1000 +
                                        source.m_data[pidx*4+argb_idx_b] * 114 / 1000;

                gray_val = gray_val * source.m_data[pidx*4+argb_idx_a] / 255;

                m_data[pidx] = static_cast<uint8_t>(gray_val);
            }
            m_type = img_data_type_gray;
        }
        else
        {
            ERR_LOG("Unknown image format, cannot convert.");
        }
    }
    else
    {
        ERR_LOG("Load source data error.");
    }

    return result;
}

bool image_raw_data::is_ok(void) const
{
    bool result = (m_data != nullptr) && (m_width > 0) && (m_height > 0);
    return result;
}

bool image_raw_data::save(const std::string& save_path)
{
    return stbi_write_png(save_path.c_str(), m_width, m_height, (m_type==img_data_type_gray)?1:4, m_data, 0);
}

int image_raw_data::get_width(void) const
{
    return m_width;
}

int image_raw_data::get_height(void) const
{
    return m_height;
}

img_data_type image_raw_data::get_type(void) const
{
    return m_type;
}

uint32_t image_raw_data::as_argb32(int x, int y)
{
    int pixel_idx = y*m_width+x;
    argb32_px_t argb32_conv;

    argb32_conv.ch.red =    m_data[pixel_idx*4+argb_idx_r];
    argb32_conv.ch.green =  m_data[pixel_idx*4+argb_idx_g];
    argb32_conv.ch.blue =   m_data[pixel_idx*4+argb_idx_b];
    argb32_conv.ch.alpha =  m_data[pixel_idx*4+argb_idx_a];

    return argb32_conv.full;
}

uint16_t image_raw_data::as_rgb565(int x, int y, bool swap)
{
    int pixel_idx = y*m_width+x;
    uint16_t rgb565_val = 0;

    rgb565_px_t rgb565_conv;
    rgb565_conv.ch.red =    m_data[pixel_idx*4+argb_idx_r] >> 3;
    rgb565_conv.ch.green =  m_data[pixel_idx*4+argb_idx_g] >> 2;
    rgb565_conv.ch.blue =   m_data[pixel_idx*4+argb_idx_b] >> 3;

    if(swap)
    {
        rgb565_val = (rgb565_conv.full << 8) | (rgb565_conv.full >> 8);
    }
    else
    {
        rgb565_val = rgb565_conv.full;
    }

    return rgb565_val;
}

uint8_t image_raw_data::as_rgb332(int x, int y)
{
    int pixel_idx = y*m_width+x;
    rgb332_px_t rgb332_conv;
    rgb332_conv.ch.red =    m_data[pixel_idx*4+argb_idx_r] >> 5;
    rgb332_conv.ch.green =  m_data[pixel_idx*4+argb_idx_g] >> 5;
    rgb332_conv.ch.blue =   m_data[pixel_idx*4+argb_idx_b] >> 6;
    return rgb332_conv.full;
}

uint8_t image_raw_data::red(int x, int y)
{
    int pixel_idx = y*m_width+x;
    return m_data[pixel_idx*4+argb_idx_r];
}

uint8_t image_raw_data::green(int x, int y)
{
    int pixel_idx = y*m_width+x;
    return m_data[pixel_idx*4+argb_idx_g];
}

uint8_t image_raw_data::blue(int x, int y)
{
    int pixel_idx = y*m_width+x;
    return m_data[pixel_idx*4+argb_idx_b];
}

uint8_t image_raw_data::alpha(int x, int y)
{
    int pixel_idx = y*m_width+x;
    return m_data[pixel_idx*4+argb_idx_a];
}

bool image_raw_data::create_8b_binary(image_binary& bin, bool with_alpha) const
{
    bool result = false;
    rgb332_px_t rgb332_val;
    int pixel_count = m_width * m_height;
    int pixel_idx = 0;
    int pixel_data_len = with_alpha?2:1;

    /* Release existed data. */
    bin.release();
    /* Allocate new memory. */
    if(bin.create(m_width, m_height, img_bin_color_format_rgb332, with_alpha))
    {
        /* Fill data. */
        while(pixel_idx < pixel_count)
        {
            int byte_idx = pixel_idx * 4;
            rgb332_val.ch.red = m_data[byte_idx + argb_idx_r] >> 5;
            rgb332_val.ch.green = m_data[byte_idx + argb_idx_g] >> 5;
            rgb332_val.ch.blue = m_data[byte_idx + argb_idx_b] >> 6;
            rgb332_val.ch.alpha = m_data[byte_idx + argb_idx_a];

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
    rgb565_px_t rgb565_val;
    int pixel_count = m_width * m_height;
    int pixel_idx = 0;
    int pixel_data_len = with_alpha?3:2;

    /* Release existed data. */
    bin.release();
    /* Allocate new memory. */
    if(bin.create(m_width, m_height, swap?img_bin_color_format_rgb565_swap:img_bin_color_format_rgb565, with_alpha))
    {
        /* Fill data. */
        while(pixel_idx < pixel_count)
        {
            int byte_idx = pixel_idx * 4;
            rgb565_val.ch.red = m_data[byte_idx + argb_idx_r] >> 3;
            rgb565_val.ch.green = m_data[byte_idx + argb_idx_g] >> 2;
            rgb565_val.ch.blue = m_data[byte_idx + argb_idx_b] >> 3;
            rgb565_val.ch.alpha = m_data[byte_idx + argb_idx_a];

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
    argb32_px_t argb32_val;
    int pixel_count = m_width * m_height;
    int pixel_idx = 0;
    uint8_t alpha_mask = with_alpha?0x00:0xFF;

    /* Release existed data. */
    bin.release();
    /* Allocate new memory, force append alpha channel. */
    if(bin.create(m_width, m_height, img_bin_color_format_argb8888, true))
    {
        /* Fill data. */
        while(pixel_idx < pixel_count)
        {
            int byte_idx = pixel_idx * 4;

            argb32_val.ch.red = m_data[byte_idx + argb_idx_r];
            argb32_val.ch.green = m_data[byte_idx + argb_idx_g];
            argb32_val.ch.blue = m_data[byte_idx + argb_idx_b];
            argb32_val.ch.alpha = m_data[byte_idx + argb_idx_a] | alpha_mask;

            /* 32bit bitmap pixel size fixed to 4 bytes. */
            ::memcpy(bin.data() + (pixel_idx * 4), &argb32_val, 4);

            ++pixel_idx;
        }
    }

    return result;
}

bool image_raw_data::create_index_bunary(image_binary& bin, int index_bits) const
{
    bool result = false;
#if 0
    int index_len = 1 << index_bits;
    int img_data_len = m_width * m_height;
    int pixel_idx = 0;
    int data_len = index_len + img_data_len;
    /* Release existed data. */
    bin.release();
    if(bin.create(m_width, m_height, img_bin_color_indexed_1bit, false))
    {
        unsigned char *indexed_data = stbi_quantize_colorspace(image_data, width * height, STBI_rgb_alpha, NULL, 256);
        uint8_t* indexed_data = stbi_q

    }
#endif
    return result;
}
