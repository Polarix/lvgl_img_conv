//===========================================================//
//= Include files.                                          =//
//===========================================================//
#include <code_generator.h>
#include <path.h>
#include <log.h>
#include <lvgl.h>
#include <iomanip>

//===========================================================//
//= Static variable definition.                             =//
//===========================================================//
static constexpr char s_image_data_file_header[] =                  {"#if defined(LV_LVGL_H_INCLUDE_SIMPLE)\n"
                                                                     "    #include \"lvgl.h\"\n"
                                                                     "#else\n"
                                                                     "    #include \"lvgl/lvgl.h\"\n"
                                                                     "#endif\n"
                                                                     "\n"
                                                                    };

static constexpr char s_image_px_map_start[] =                      {"static const uint8_t image_px_map[] = {\n"}; //"const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_COLORFUL_MUSHROOMS uint8_t image_px_map[] = {"

/* For 8 bits RGB332 */
static constexpr char s_image_data_8bit_map_start_marco[] =         {"#if LV_COLOR_DEPTH == 1 || LV_COLOR_DEPTH == 8\n"};
static constexpr char s_image_data_8bit_map_end_marco[] =           {"#endif // LV_COLOR_DEPTH == 1 || LV_COLOR_DEPTH == 8\n"};
/* For 16 bits RGB 565 */
static constexpr char s_image_data_16bit_map_start_marco[] =        {"#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP == 0\n"}; /*Pixel format: Alpha 8 bit, Red: 5 bit, Green: 6 bit, Blue: 5 bit*/
static constexpr char s_image_data_16bit_map_end_marco[] =          {"#endif // LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP == 0\n"};
static constexpr char s_image_data_16bit_map_swap_start_marco[] =   {"#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP != 0\n"}; // /*Pixel format: Alpha 8 bit, Red: 5 bit, Green: 6 bit, Blue: 5 bit*/
static constexpr char s_image_data_16bit_map_swap_end_marco[] =     {"#endif // LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP != 0\n"};
/* For 32 bits ARGB 8888 */
static constexpr char s_image_data_32bit_map_start_marco[] =        {"#if LV_COLOR_DEPTH == 32\n"}; // Pixel format: B,G,R,A.
static constexpr char s_image_data_32bit_map_end_marco[] =          {"#endif // LV_COLOR_DEPTH == 32\n"};

static constexpr char s_image_px_map_end[] =                        {"}; // image_px_map\n"};
#if 0
static constexpr char s_image_dsc_fmt[] =                           {"const lv_img_dsc_t %s = {\n"
                                                                     "    .header.cf = %s,\n"
                                                                     "    .header.always_zero = 0,\n"
                                                                     "    .header.reserved = 0,\n"
                                                                     "    .header.w = %d,\n"
                                                                     "    .header.h = %d,\n"
                                                                     "    .data_size = %d * LV_IMG_PX_SIZE_ALPHA_BYTE,\n"
                                                                     "    .data = image_px_map,\n"
                                                                     "};"
                                                                    };
#endif
//===========================================================//
//= Function definition.                                    =//
//===========================================================//
void code_generator::output_img_header(void)
{
    m_source_file << s_image_data_file_header;
}

void code_generator::output_img_bmp_line(const void* data, size_t len)
{
    if(m_source_file.is_open() && data)
    {
        const uint8_t* data_byte = static_cast<const uint8_t*>(data);
        size_t remaining = len;
        while(remaining)
        {
            char buffer[16];
            snprintf(buffer, 16, "0x%02x", *data_byte);
            m_source_file << buffer << ", ";
            // m_source_file << std::hex << std::setw(2) << std::setfill('0') << std::showbase << *data_byte << ", ";
            ++data_byte;
            --remaining;
        }
        m_source_file << std::endl;
    }
}

void code_generator::output_img_dsc_data(void)
{
    if(m_source_file.is_open())
    {
        m_source_file << "\n";
        m_source_file << "const lv_img_dsc_t " << m_name << " = {\n" ;
        m_source_file << "    .header.cf = " << cf_name(m_image_dsc_data.header.color_format) << ",\n";
        m_source_file << "    .header.always_zero = 0,\n";
        m_source_file << "    .header.reserved = 0,\n";
        m_source_file << "    .header.w = " << m_image_dsc_data.header.width << ",\n";
        m_source_file << "    .header.h = " << m_image_dsc_data.header.height << ",\n";
        m_source_file << "    .data_size = " << m_image_dsc_data.header.width * m_image_dsc_data.header.height << " * LV_IMG_PX_SIZE_ALPHA_BYTE,\n";
        m_source_file << "    .data = image_px_map,\n";
        m_source_file << "};\n";
        m_source_file << "\n";
    }
}

bool code_generator::prepare(const std::string& output_path)
{
    bool result = false;
    m_source_file.open(output_path, std::ios::out);
    if(m_source_file.is_open())
    {
        /* Header. */
        output_img_header();
        /* Bit map start. */
        m_source_file << s_image_px_map_start;

        result = true;
    }
    return result;
}

bool code_generator::generate_data(const image_binary& binary_data)
{
    bool result = false;
    if(m_source_file.is_open() && binary_data.data())
    {
        switch(binary_data.format())
        {
            case img_bin_format_rgb332:
            {
                m_source_file << s_image_data_8bit_map_start_marco;
                break;
            }
            case img_bin_format_rgb565:
            {
                m_source_file << s_image_data_16bit_map_start_marco;
                 break;
           }
            case img_bin_format_rgb565_swap:
            {
                m_source_file << s_image_data_16bit_map_swap_start_marco;
                break;
            }
            case img_bin_format_argb8888:
            {
                m_source_file << s_image_data_32bit_map_start_marco;
                break;
            }
            default:
            {
                /* No any comment. */
            }
        }
        /* Update data size info in image dscription.  */
        m_image_dsc_data.data_size = binary_data.size();
        /* Loop for output each line. */
        int pixel_row_idx = 0;
        while(pixel_row_idx < binary_data.height())
        {
            int byte_idx = pixel_row_idx * binary_data.line_width();
            m_source_file << "    ";
            output_img_bmp_line(binary_data.data(byte_idx), binary_data.line_width());
            ++pixel_row_idx;
        }

        switch(binary_data.format())
        {
            case img_bin_format_rgb332:
            {
                m_source_file << s_image_data_8bit_map_end_marco;
                break;
            }
            case img_bin_format_rgb565:
            {
                m_source_file << s_image_data_16bit_map_end_marco;
                break;
            }
            case img_bin_format_rgb565_swap:
            {
                m_source_file << s_image_data_16bit_map_swap_end_marco;
                break;
            }
            case img_bin_format_argb8888:
            {
                m_source_file << s_image_data_32bit_map_end_marco;
                break;
            }
            default:
            {
                /* No any comment. */
            }
        }
        result = true;
    }
    return result;
}

void code_generator::finialize(void)
{
    if(m_source_file.is_open())
    {
        m_source_file << s_image_px_map_end;

        m_source_file << "\n";

        output_img_dsc_data();
        m_source_file.close();
    }
}
