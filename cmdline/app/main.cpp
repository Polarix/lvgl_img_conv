//===========================================================//
//= Include files.                                          =//
//===========================================================//
#include <log.h>
#include <image_raw_data.h>
#include <binary_generator.h>
#include <code_generator.h>
#include <fstream>
#include <iostream>

void source_code_file_generate_demo(bool with_alpha, bool with_chroma);
void binary_file_generate_demo(bool with_alpha, bool with_chroma);
void alpha_generate_demo(void);

int main(int argc, char** argv)
{
    // source_code_file_generate_demo(true, false);
    // binary_file_generate_demo(true, false);
    // indexed_generate_demo();
    alpha_generate_demo();

    return 0;
}

void alpha_generate_demo(void)
{
    code_generator generator;

    generator.open("input_test1.png");

    if(generator.image_raw().is_ok())
    {
        image_binary img;

        /* For 1-bit alpha data. */
        generator.set_res_name("TEST_ALPHA1");
        generator.set_color_format("LV_IMG_CF_ALPHA_1BIT");

        generator.prepare("test_a1b.c");
        generator.image_raw().create_1bit_alpha_binary(img);
        generator.generate_data(img);
        generator.finialize();

        /* For 2-bit alpha data. */
        generator.set_res_name("TEST_ALPHA2");
        generator.set_color_format("LV_IMG_CF_ALPHA_2BIT");

        generator.prepare("test_a2b.c");
        generator.image_raw().create_2bit_alpha_binary(img);
        generator.generate_data(img);
        generator.finialize();


        /* For 4-bit alpha data. */
        generator.set_res_name("TEST_ALPHA4");
        generator.set_color_format("LV_IMG_CF_ALPHA_4BIT");

        generator.prepare("test_a4b.c");
        generator.image_raw().create_4bit_alpha_binary(img);
        generator.generate_data(img);
        generator.finialize();


        /* For 8-bit alpha data. */
        generator.set_res_name("TEST_ALPHA8");
        generator.set_color_format("LV_IMG_CF_ALPHA_8BIT");

        generator.prepare("test_a8b.c");
        generator.image_raw().create_8bit_alpha_binary(img);
        generator.generate_data(img);
        generator.finialize();
    }
    else
    {
        ERR_LOG("Read image failed.");
    }
}

void source_code_file_generate_demo(bool with_alpha, bool with_chroma)
{
    code_generator generator;

    generator.open("7125.png");
    /* Resource name will auto set with filename if not specify a resource name. */
    generator.set_res_name("PERVIEW");

    if(with_alpha)
    {
        if(with_chroma)
        {
            generator.set_color_format("LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED");
        }
        else
        {
            generator.set_color_format("LV_IMG_CF_TRUE_COLOR_ALPHA");
        }
    }
    else
    {
        generator.set_color_format("LV_IMG_CF_TRUE_COLOR");
    }

    if(generator.image_raw().is_ok())
    {
        generator.prepare("test1.c");
        image_binary img;
        /* For RGB332 data. */
        generator.image_raw().create_8b_binary(img, with_alpha);
        generator.generate_data(img);
        /* For RGB565 data. */
        generator.image_raw().create_16b_binary(img, with_alpha, false);
        generator.generate_data(img);
        /* For RGB565 swap(BE) data. */
        generator.image_raw().create_16b_binary(img, with_alpha, true);
        generator.generate_data(img);
        /* For ARGB8888 data. */
        generator.image_raw().create_32b_binary(img, with_alpha);
        generator.generate_data(img);

        generator.finialize();
    }
    else
    {
        ERR_LOG("Read image failed.");
    }
}

void binary_file_generate_demo(bool with_alpha, bool with_chroma)
{
    binary_generator generator;

    generator.open("input_test2.png");
    /* Resource name will auto set with filename if not specify a resource name. */
    generator.set_res_name("TEST2");
    if(with_alpha)
    {
        if(with_chroma)
        {
            generator.set_color_format("LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED");
        }
        else
        {
            generator.set_color_format("LV_IMG_CF_TRUE_COLOR_ALPHA");
        }
    }
    else
    {
        generator.set_color_format("LV_IMG_CF_TRUE_COLOR");
    }

    if(generator.image_raw().is_ok())
    {
        generator.prepare("test2.bin");
        image_binary img;
        generator.image_raw().create_32b_binary(img, true);
        generator.generate_data(img);
        generator.finialize();
    }
    else
    {
        ERR_LOG("Read image failed.");
    }
}
