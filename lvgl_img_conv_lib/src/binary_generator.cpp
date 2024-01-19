//===========================================================//
//= Include files.                                          =//
//===========================================================//
#include <binary_generator.h>
#include <path.h>
#include <log.h>
#include <lvgl.h>

//===========================================================//
//= Function definition.                                    =//
//===========================================================//
void binary_generator::output_img_header(void)
{
    /* Binary file not needed. */
}

void binary_generator::output_img_bmp_line(const void* data, size_t len)
{
    if(m_binary_file.is_open())
    {
        m_binary_file.write(reinterpret_cast<const char*>(data), len/sizeof(char));
    }
}

void binary_generator::output_img_dsc_data(void)
{
    if(m_binary_file.is_open())
    {
        m_binary_file.write(reinterpret_cast<const char*>(&m_image_dsc_data.header), sizeof(image_dsc_header_t));
    }
}

bool binary_generator::prepare(const std::string& output_path)
{
    m_binary_file.open(output_path, std::ios::out | std::ios::binary);
    output_img_dsc_data();
    return m_binary_file.is_open();
}

bool binary_generator::generate_data(const image_binary& binary_data)
{
    bool result = false;
    if(m_binary_file.is_open() && binary_data.data())
    {
        /* Update data size info in image dscription.  */
        m_image_dsc_data.data_size = binary_data.size();
        /* Loop for output each line. */
        int pixel_row_idx = 0;
        while(pixel_row_idx < binary_data.height())
        {
            int byte_idx = pixel_row_idx * binary_data.width() * binary_data.pixel_data_width();
            output_img_bmp_line(binary_data.data(byte_idx), binary_data.width() * binary_data.pixel_data_width());
            ++pixel_row_idx;
        }
        result = true;
    }
    return result;
}

void binary_generator::finialize(void)
{
    if(m_binary_file.is_open())
    {
        m_binary_file.close();
    }
}
