#version 430 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexcoord;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 fTexcoord;
void main()
{
<<<<<<< HEAD
    fTexcoord = vTexcoord;
    gl_Position = projection * view * model * vec4(vPosition, 1.0);
=======
    gl_Position = vec4(vPosition, 1.0);
>>>>>>> tree demo
}
