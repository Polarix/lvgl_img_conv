# lvgl_img_conv
### 简述
这是一个尝试使用最少的编译和构建工具，编写一个可以创建LVGL使用的图片资源的工具，方便在不便接入网络的地方使用。

### 内容
暂时包含以下文件夹：
* lvgl_img_conv_lib: 这个文件夹中的内容可以编译出一个C++库，提供图片资源制作功能。
* cmdline 一个使用lvgl_img_conv_lib创建的一个简易范例。这个例子运行于控制台模式下，可以通过简单的修改使用。

参考以上内容，可以快速的构建一个离弦的LVGL图片转换工具。

### 功能
截至目前，lvgl_img_conv_lib可以创建C源码形式的图片资源以及bin文件形式的资源文件，支持输出的图像色彩格式如下：
* LV_IMG_CF_TRUE_COLOR
* LV_IMG_CF_TRUE_COLOR_ALPHA
* LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED

### 依赖
lvgl_img_conv_lib的开发依赖了以下开源软件：
* stb image：使用了stb的image组件用于支持多种图片文件的输入，如bmp、jpg、png等。
* lvgl：因为输出内容是面向LVGL的，所以需要参考LVGL的数据结构定义。

### 计划
lvgl_img_conv_lib正在计划开发和补全以下内容。

- [ ] 添加索引色格式的图片资源输出。因为True Color格式并不能很好的支持单色屏幕。
- [ ] 简单方便使用的编译环境。目前这个库仅仅是初版，正在着重于功能开发，我并没有整理目录结构和，编译环境。我希望能够仅使用GCC就可以完成工具可执行文件的编译，这样工具就可以完美的融合进LVGL的开发环境里，甚至可以让图片资源在编译时生成。



