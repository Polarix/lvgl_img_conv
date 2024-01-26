//===========================================================//
//= Include files.                                          =//
//===========================================================//
#include <image_palette.h>
#include <cstring>
#include <log.h>
#include <algorithm>

//===========================================================//
//= Function definition.                                    =//
//===========================================================//
image_pixel::image_pixel(uint8_t red, uint8_t green, uint8_t blue)
 : m_red(red), m_green(green), m_blue(blue)
{
    // for(int bit=0; bit<8; ++bit)
    for(int bit=7; bit>=0; --bit)
    {
        m_index[bit] = (uint8_t)(((blue & 0x1) << 2) + ((green & 0x1) << 1) + (red & 0x1));
        blue >>= 1;
        green >>= 1;
        red >>= 1;
    }
}

image_pixel::image_pixel(const image_pixel& source)
 : m_red(source.m_red), m_green(source.m_green), m_blue(source.m_blue)
{
    ::memcpy(m_index, source.m_index, sizeof(uint8_t)*8);
}

image_pixel::~image_pixel(void)
{
    /* do nothing. */
}

uint8_t image_pixel::red(void) const
{
    return m_red;
}

uint8_t image_pixel::green(void) const
{
    return m_green;
}

uint8_t image_pixel::blue(void) const
{
    return m_blue;
}

uint8_t image_pixel::index(uint8_t level)
{
    return m_index[level];
}

octree_node::octree_node(const octree_node* parent, int level)
 : m_parent(parent)
 , m_level(level)
 , m_pixel_cnt(0)
{
    ::memset(m_children, 0x00, sizeof(octree_node*) * 8);
}

octree_node::~octree_node(void)
{
    for(int child_idx=0; child_idx < 8; ++child_idx)
    {
        if(m_children[child_idx])
        {
            delete m_children[child_idx];
        }
    }
}

void octree_node::use_count_inc(int increment)
{
    m_pixel_cnt += increment;
}

octree_node* const octree_node::add_child(int index)
{
    octree_node* new_child = nullptr;
    if(index < 8)
    {
        if(nullptr == m_children[index])
        {
            m_children[index] = new octree_node(this, m_level+1);
            new_child = m_children[index];
        }
        m_children[index]->use_count_inc();
    }
    return new_child;
}

octree_node* const octree_node::get_child(int index)
{
    octree_node* child = nullptr;
    if(index < 8)
    {
        child = m_children[index];
    }
    return child;
}

int octree_node::pixel_count(void) const
{
    return m_pixel_cnt;
}

int octree_node::level(void) const
{
    return m_level;
}

void octree_node::remove_child(int index)
{
    if((index < 8) && (m_children[index]))
    {
        delete m_children[index];
        m_children[index] = nullptr;
    }
}

image_palette::image_palette(void)
{
    m_tree_root = new octree_node(nullptr);
}

image_palette::~image_palette(void)
{
    if(m_tree_root)
    {
        delete m_tree_root;
        m_tree_root = nullptr;
    }
}

bool image_palette::node_comparator(const octree_node& left, const octree_node& right)
{
    return (left.pixel_count() > right.pixel_count()) || (left.pixel_count() == right.pixel_count() && left.level() < right.level());
}

void image_palette::add_pixel(uint8_t red, uint8_t green, uint8_t blue)
{
    image_pixel pixel(red, green, blue);
    octree_node* cur = m_tree_root;

    /* increase pixel count. */
    cur->use_count_inc();
    /* Recorder pixel color index. */
    for(int i_level=0; i_level < 8; ++i_level)
    {
        /* Color index for each level. */
        int child_index = pixel.index(i_level);
        /* Create child node if not existed. */
        cur->add_child(child_index);
        cur = cur->get_child(child_index);
    }
}

void image_palette::quantify(int color_max)
{

}

const octree_node* image_palette::oct_tree(void) const
{
    return m_tree_root;
}

void image_palette::debug_print_each_leaf_cnt(octree_node* node)
{
    if(node)
    {
        bool has_child = false;
        for(int child_idx=0; child_idx<8; ++child_idx)
        {
            if(node->get_child(child_idx))
            {
                debug_print_each_leaf_cnt(node->get_child(child_idx));
                has_child = true;
            }
        }
        if(!has_child)
        {
            ++m_debug_info.m_leaf_cnt;
            m_debug_info.m_pixel_cnt += node->pixel_count();
            // DBG_LOG("Pixel count: %d.", node->m_pixel_cnt);
        }
    }
}

void image_palette::debug_print_leaf_cnt(void)
{
    m_debug_info.m_leaf_cnt = 0;
    m_debug_info.m_pixel_cnt = 0;
    for(int child_idx=0; child_idx<8; ++child_idx)
    {
        if(m_tree_root->get_child(child_idx))
        {
            debug_print_each_leaf_cnt(m_tree_root->get_child(child_idx));
        }
    }
    DBG_LOG("Total leafs: %d.", m_debug_info.m_leaf_cnt);
    DBG_LOG("Total pixel: %d.", m_debug_info.m_pixel_cnt);
}
