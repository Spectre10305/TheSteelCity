#version 460 core


float near = 0.1; 
float far  = 1.0;


float LinearizeDepth(float depth) 
{

    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	

}


void main()
{

    gl_FragDepth = gl_FragCoord.z;

}