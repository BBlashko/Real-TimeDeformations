#include "image.hpp"
#include <environment.hpp>
#include <Windows.h>

namespace rt_deformations
{
    image::image(char * file_path) : _Buffer(GL_ARRAY_BUFFER), 
                                     _ebo(GL_ELEMENT_ARRAY_BUFFER), 
                                     _showGrid(true),
                                     _gridBuffer(GL_ARRAY_BUFFER)
    {
        load_image_filenames();
        load_image();

        //TODO: normalize height and width;
        normalize_dimensions();
        generate_quad();
        generate_grid();

        mModel = atlas::math::Matrix4(1.0f);
        std::vector<atlas::gl::ShaderUnit> shaders = {
            { get_shader_path() + "/deformations.fs.glsl", GL_FRAGMENT_SHADER },
            { get_shader_path() + "/deformations.vs.glsl", GL_VERTEX_SHADER }
        };

        mShaders.push_back(atlas::gl::Shader(shaders));
        mShaders[0].compileShaders();
        mShaders[0].linkShaders();

        _Vao.bindVertexArray();
        _Buffer.bindBuffer();
        _Buffer.bufferData(atlas::gl::size<Vector4>(_vertices.size()),
            _vertices.data(), GL_STATIC_DRAW);
        
        //position
        _Buffer.vertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
            atlas::gl::bufferOffset<float>(0));

        //texture
        _Buffer.vertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
            atlas::gl::bufferOffset<float>(2));

        GLuint elements[] = {
            0, 1, 2,
            2, 3, 0
        };
        _ebo.bindBuffer();
        _ebo.bufferData(sizeof(elements), elements, GL_STATIC_DRAW);

        _Vao.enableVertexAttribArray(0);
        _Vao.enableVertexAttribArray(1);

        //initialize grid
        _gridVao.bindVertexArray();
        _gridBuffer.bindBuffer();
        _gridBuffer.bufferData(atlas::gl::size<atlas::math::Vector>(_numGridIndicies),
            _gridVertices.data(), GL_STATIC_DRAW);
        _gridBuffer.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
            atlas::gl::bufferOffset<float>(0));
        _gridVao.enableVertexAttribArray(0);
        _gridBuffer.unBindBuffer();
        _gridVao.unBindVertexArray();


        auto var = mShaders[0].getUniformVariable("projection");
        mUniforms.insert(UniformKey("projection", var));

        var = mShaders[0].getUniformVariable("view");
        mUniforms.insert(UniformKey("view", var));

        var = mShaders[0].getUniformVariable("model");
        mUniforms.insert(UniformKey("model", var));

        var = mShaders[0].getUniformVariable("colour");
        mUniforms.insert(UniformKey("colour", var));

        var = mShaders[0].getUniformVariable("tex");
        mUniforms.insert(UniformKey("tex", var));

        _Buffer.unBindBuffer();
        _Vao.unBindVertexArray();
        mShaders[0].disableShaders();
    }

    void image::renderGeometry(atlas::math::Matrix4 const& projection,
        atlas::math::Matrix4 const& view)
    {
        if (!mShaders[0].shaderProgramValid())
        {
            return;
        }

        mShaders[0].enableShaders();

        //Image
        _Vao.bindVertexArray();

        _texture.bindTexture();
        glUniform1i(mUniforms["tex"], 0);
        glUniformMatrix4fv(mUniforms["projection"], 1, GL_FALSE,
            &projection[0][0]);
        glUniformMatrix4fv(mUniforms["view"], 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(mUniforms["model"], 1, GL_FALSE, &mModel[0][0]);

        //glUniform4f(mUniforms["colour"], 0, 1, 0, 1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        _texture.unBindTexture();
        _Vao.unBindVertexArray();

        //grid
        if (_showGrid)
        {
            _gridVao.bindVertexArray();

            glPointSize(2.0f);
            glUniform4f(mUniforms["colour"], 0, 0, 0, 1);
            int n = (int)(_numGridIndicies / 2) - 2;
            glDrawArrays(GL_LINES, n, 4);

            glPointSize(7.0f);
            float rgb = (127.0f / 255.0f);
            glUniform4f(mUniforms["colour"], rgb, rgb, rgb, 1);
            glDrawArrays(GL_LINES, 0, (int)_numGridIndicies);

            _gridVao.unBindVertexArray();
        }
        mShaders[0].disableShaders();
    }

    int image::load_image()
    {
        _texture.genTexture(GL_TEXTURE_2D);
        _texture.bindTexture();

        std::string file_path;
        if (_image_names.size() > 0)
        {
            file_path = get_images_path() + "/" + _image_names.at(_curr_image);
        }
        else
        {
            file_path = "";
        }
        unsigned char * image = SOIL_load_image(file_path.c_str(), &_width, &_height, 0, SOIL_LOAD_RGBA);

        if (image == NULL)
        {
            WARN_LOG("Failed to load image, verify path string. If PNG format make sure to disable interlaced mode");
            WARN_LOG(SOIL_last_result());
        }

        GLint mode = GL_RGBA;
        _texture.texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        _texture.texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        _texture.texImage2D(0, mode, _width, _height, 0, mode, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
        _texture.unBindTexture();

        return 0;
    }

    void image::normalize_dimensions()
    {
        if (_width > _height)
        {
            float ratio = (float) _height / (float) _width;
            _norm_width = _scale;
            _norm_height = ratio * _scale;
        }
        else if (_width < _height)
        {
            float ratio = (float) _width / (float)_height;
            _norm_width = ratio * _scale;
            _norm_height = _scale;
        }
        else 
        {
            _norm_width = _scale;
            _norm_height = _scale;
        }
        
    }

    void image::generate_quad()
    {
        _vertices.clear();
        _vertices.push_back(Vector4(-_norm_width, _norm_height, 0.0f, 0.0f ));
        _vertices.push_back(Vector4( _norm_width, _norm_height, 1.0f, 0.0f ));
        _vertices.push_back(Vector4( _norm_width, -_norm_height, 1.0f, 1.0f ));
        _vertices.push_back(Vector4(-_norm_width, -_norm_height, 0.0f, 1.0f ));

        _Buffer.bindBuffer();
        _Buffer.bufferData(atlas::gl::size<Vector4>(_vertices.size()),
            _vertices.data(), GL_STATIC_DRAW);
        _Buffer.unBindBuffer();
    }

    void image::generate_grid()
    {
        _gridVertices.clear();
        _numGridIndicies = ((int)(_norm_width + 1) * 4) + ((int)(_norm_height + 1) * 4) + (int) (abs((int)(_norm_width + 1) - (int)(_norm_height + 1)) * 4) + 4;

        int n = (int)_numGridIndicies / 4;
        for (int i = 0; i < n; ++i)
        {
            //assures that the vertex does not exceed the width or height + 1
            int x = -(_norm_width + 1) + i;
            if (x > _norm_width + 1)
            {
                x = _norm_width + 1;
            }

            int y = -(_norm_height + 1) + i;
            if (y > _norm_height + 1)
            {
                y = _norm_height + 1;
            }
            _gridVertices.push_back(Vector3((int)x, (int)-(_norm_height + 1), 0.01f));
            _gridVertices.push_back(Vector3((int)x, (int)(_norm_height + 1),  0.01f));
            _gridVertices.push_back(Vector3((int)-(_norm_width + 1), y , 0.01f));
            _gridVertices.push_back(Vector3((int)(_norm_width + 1), y, 0.01f));
        }

        //loads data into the buffer array
        _gridBuffer.bindBuffer();
        _gridBuffer.bufferData(atlas::gl::size<atlas::math::Vector>(_numGridIndicies),
            _gridVertices.data(), GL_STATIC_DRAW);
        _gridBuffer.unBindBuffer();
    }

    void image::update_image()
    {
        normalize_dimensions();
        generate_quad();
        generate_grid();
    }

    void image::show_grid(bool b)
    {
        _showGrid = b;
    }

    void image::load_image_filenames()
    {
        HANDLE hFind;
        WIN32_FIND_DATA FindData;

        std::string path = get_images_path() + "/*";
        hFind = FindFirstFile(path.c_str(), &FindData);
        if (strcmp(FindData.cFileName, ".") == 1 && strcmp(FindData.cFileName, "..") == 1 && strcmp(FindData.cFileName, "CMakeLists.txt") == 1)
        {
            _image_names.push_back(FindData.cFileName);
        }

        while (FindNextFile(hFind, &FindData))
        {
            if (strcmp(FindData.cFileName, ".") == 1 && strcmp(FindData.cFileName, "..") == 1 && strcmp(FindData.cFileName, "CMakeLists.txt") == 1)
            {
                _image_names.push_back(FindData.cFileName);
            }
        }

        FindClose(hFind);
    }
}
