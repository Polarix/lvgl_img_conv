//===========================================================//
//= Include files.                                          =//
//===========================================================//
#include <generator_base.h>
#include <algorithm>
#include <path.h>
#include <log.h>
#include <lvgl.h>

//===========================================================//
//= Static variable definition.                             =//
//===========================================================//
static constexpr image_cf s_image_cf_array[] =                      {{LV_IMG_CF_TRUE_COLOR, "LV_IMG_CF_TRUE_COLOR"},
                                                                     {LV_IMG_CF_TRUE_COLOR_ALPHA, "LV_IMG_CF_TRUE_COLOR_ALPHA"},
                                                                     {LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED, "LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED"},
                                                                     {LV_IMG_CF_INDEXED_1BIT, "LV_IMG_CF_INDEXED_1BIT"},
                                                                     {LV_IMG_CF_INDEXED_2BIT, "LV_IMG_CF_INDEXED_2BIT"},
                                                                     {LV_IMG_CF_INDEXED_4BIT, "LV_IMG_CF_INDEXED_4BIT"},
                                                                     {LV_IMG_CF_INDEXED_8BIT, "LV_IMG_CF_INDEXED_8BIT"},
                                                                    };


//===========================================================//
//= Function definition.                                    =//
//===========================================================//
generator_base::generator_base(void)
{

}

generator_base::~generator_base(void)
{

}

std::string generator_base::cf_name(uint8_t val)
{
    std::string cf_name_str = "LV_IMG_CF_UNKNOWN";
    for(const image_cf item : s_image_cf_array)
    {
        if(item.val == val)
        {
            cf_name_str = item.name;
            break;
        }
    }
    return cf_name_str;
}

uint8_t generator_base::cf_code(const std::string& get_res_name)
{
    uint8_t cf_code_val = LV_IMG_CF_UNKNOWN;
    for(const image_cf item : s_image_cf_array)
    {
        if(0 == get_res_name.compare(item.name))
        {
            cf_code_val = item.val;
            break;
        }
    }
    return cf_code_val;
}

bool generator_base::open(const std::string& image_path)
{
    bool result = m_image_raw.load(image_path);
    if(result)
    {
        if(m_name.empty())
        {
            file_path img_path(image_path);
            m_name = img_path.name();
            /* Convert resource to upper case. */
            std::transform(m_name.begin(), m_name.end(), m_name.begin(), [](unsigned char c) { return std::toupper(c);});
            DBG_LOG("Resource name: %s", m_name.c_str());
        }
        ::memset(&m_image_dsc_data, 0x00, sizeof(image_dsc_t));
        m_image_dsc_data.header.width = m_image_raw.get_width();
        m_image_dsc_data.header.height = m_image_raw.get_height();
    }
    return result;
}

bool generator_base::output_img_bit_map(const image_binary& binary_data)
{
    if(binary_data.data())
    {
        /* Update data size info in image dscription.  */
        m_image_dsc_data.data_size = binary_data.size();
        /* Loop for output each line. */
        int pixel_row_idx = 0;
        while(pixel_row_idx < binary_data.height())
        {
            int byte_idx = pixel_row_idx * binary_data.line_width();
            output_img_bmp_line(binary_data.data(byte_idx), binary_data.line_width());
            ++pixel_row_idx;
        }
    }
    return true;
}

const image_raw_data& generator_base::image_raw(void) const
{
    return m_image_raw;
}

const std::string& generator_base::get_res_name(void) const
{
    return m_name;
}

void generator_base::set_res_name(const std::string& new_name)
{
    m_name = new_name;
    /* Convert resource to upper case. */
    std::transform(m_name.begin(), m_name.end(), m_name.begin(), [](unsigned char c) { return std::toupper(c);});
}

void generator_base::output_img_bmp_line(const void* data, size_t len)
{
    const uint8_t* data_byte = static_cast<const uint8_t*>(data);
    if(data_byte)
    {
        size_t remaining = len;
        while(remaining)
        {
            printf("0x%02X, ", *data_byte);
            ++data_byte;
            --remaining;
        }
        putchar('\n');
    }
}

void generator_base::set_color_format(uint8_t lv_color_format)
{
    m_image_dsc_data.header.color_format = lv_color_format;
}

void generator_base::set_color_format(const std::string& lv_color_format)
{
    m_image_dsc_data.header.color_format = cf_code(lv_color_format);
}

