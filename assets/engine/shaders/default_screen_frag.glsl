#version 460 core


out vec4 FragColor;
in  vec2 TexCoords;


uniform float     exposure;
uniform sampler2D screenTexture;


void main()
{

    float gamma  = 2.2;
    vec3  color  = texture(screenTexture, TexCoords).rgb;
    vec3  mapped = vec3(1.0) - exp(-color * exposure);


    mapped = pow(mapped, vec3(1.0 / gamma));


    FragColor = vec4(mapped, 1.0);

}