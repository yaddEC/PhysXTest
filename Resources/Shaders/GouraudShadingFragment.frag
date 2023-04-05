#version 330 core
out vec4 FragColor;

in vec4 color;
in vec2 TexCoord;

uniform sampler2D texture1;

// Main

void main()
{
    vec4 TextureColor = texture(texture1, TexCoord);

    FragColor = color * TextureColor;
}
