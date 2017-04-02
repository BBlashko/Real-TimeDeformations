#version 430 core
in vec2 fTexcoord;

uniform vec4 colour;
uniform sampler2D tex;

out vec4 fragColor;

void main()
{
	fragColor = texture(tex, fTexcoord) * colour;
}
