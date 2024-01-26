#ifndef __INCLUDE_CLASS_IMAGE_PALETTE_H_
#define __INCLUDE_CLASS_IMAGE_PALETTE_H_

//===========================================================//
//= Include files.                                          =//
//===========================================================//
#include <cstdint>
#include <array>
#include <vector>
#include <memory>

//===========================================================//
//= Class declare.                                          =//
//===========================================================//
class image_pixel
{
private:
    uint8_t                 m_red;
    uint8_t                 m_green;
    uint8_t                 m_blue;
    uint8_t                 m_index[8];

public:
    explicit                image_pixel(uint8_t red, uint8_t green, uint8_t blue);
    explicit                image_pixel(const image_pixel& source);
    virtual                 ~image_pixel(void);
    uint8_t                 red(void) const;
    uint8_t                 green(void) const;
    uint8_t                 blue(void) const;
    uint8_t                 index(uint8_t level);
};

class octree_node
{
private:
    const octree_node*      m_parent;
    int                     m_level;
    int                     m_pixel_cnt;
    octree_node*            m_children[8];
public:
    explicit                octree_node(const octree_node* parent, int level = -1);
    virtual                 ~octree_node(void);
    void                    use_count_inc(int increment = 1);
    octree_node* const      add_child(int index);
    octree_node* const      get_child(int index);
    int                     pixel_count(void) const;
    int                     level(void) const;
    void                    remove_child(int index);
};

class image_palette
{
private:
    octree_node*            m_tree_root;
    void                    release_tree_node(octree_node* node);
    struct
    {
        int                 m_leaf_cnt;
        int                 m_pixel_cnt;
    }m_debug_info;

    static bool             node_comparator(const octree_node& left, const octree_node& right);

protected:
    void                    debug_print_each_leaf_cnt(octree_node* node);

public:
    explicit                image_palette(void);
    virtual                 ~image_palette(void);
    void                    add_pixel(uint8_t red, uint8_t green, uint8_t blue);
    void                    quantify(int color_max);
    const octree_node*      oct_tree(void) const;

    void                    debug_print_leaf_cnt(void);
};

#endif // __INCLUDE_CLASS_IMAGE_PALETTE_H_
