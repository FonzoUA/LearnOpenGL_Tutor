#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, TexCoord).rgba, texture(texture2, TexCoord).rgba, vec4(texture(texture2, TexCoord)).a * 0.6);
}