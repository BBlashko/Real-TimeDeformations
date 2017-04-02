#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "atlas/gl/Texture.hpp"
#include "atlas/utils/Geometry.hpp"
#include "atlas/gl/VertexArrayObject.hpp"
#include "atlas/gl/Buffer.hpp"
#include "SOIL.h"
#include "atlas/core/Log.hpp"
#include "atlas/gl/Texture.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <math.h>

#define BYTE uint8_t
#define WORD uint16_t
#define DWORD unsigned long

#define SUCCESS 0
#define FILE_ERROR 1
#define INCORRECT_BMP 2

namespace rt_deformations
{
    using Vector3 = atlas::math::Vector;
    using Vector4 = atlas::math::Vector4;

    class image : public atlas::utils::Geometry
    {

    public:
        image(char *file_path);
        void renderGeometry(atlas::math::Matrix4 const& projection,
            atlas::math::Matrix4 const& view) override;
        int width() const { return _width; };
        int height() const { return _height; };

        const std::vector<Vector4>& vertices() const { return _vertices; };
        std::vector<Vector4>& vertices() { return _vertices; };

        //const std::vector<std::string>& image_names() const { return _image_names; };
        std::vector<std::string>& image_names() { return _image_names; };

        const int& curr_image() const { return _curr_image; };
        int& curr_image() { return _curr_image; };

        //image properties
        const int& scale() const { return _scale; };
        int& scale() { return _scale; };

        void update_image();
        int load_image();
        void show_grid(bool b);
        

    private:
        void normalize_dimensions();
        void generate_quad();
        void generate_grid();
        void load_image_filenames();

        //loaded images
        int _curr_image = 0;
        std::vector<std::string> _image_names;

        //OpenGL variables
        atlas::gl::VertexArrayObject _Vao;
        atlas::gl::Buffer _Buffer;
        atlas::gl::Buffer _ebo;

        //variables
        int _width = 0;
        int _height = 0;
        float _norm_width = 0;
        float _norm_height = 0;
        int _scale = 10;

        //grid
        atlas::gl::VertexArrayObject _gridVao;
        atlas::gl::Buffer _gridBuffer;
        std::size_t _numGridIndicies;
        std::vector<Vector3> _gridVertices;
        bool _showGrid;

        //quad data
        std::vector<Vector4> _vertices;
        atlas::gl::Texture _texture;
    };
}

#endif
