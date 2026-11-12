#version 460 core


//================================================================


struct Material
{

    sampler2D diffuse;
    vec3      specular;
    float     shininess;
    
};


struct PointLight
{

    vec3  position;
    float constant;
    float linear;
    float quadratic;
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;

};


//================================================================


in      vec3      Normal;
in      vec2      TexCoord;
in      vec3      FragPos;
out     vec4      FragColor;


uniform vec3       cameraPos;
uniform Material   mat;
#define MAX_LIGHTS_NUM 16
uniform PointLight pointLights[MAX_LIGHTS_NUM];
uniform int        lightsNum;


//================================================================


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{

    vec3 lightDir = normalize(light.position - fragPos);


    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);


    // specular shading
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);


    // attenuation
    float dist        = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));  


    // combine results
    vec3 ambient  = light.ambient         * vec3(texture(mat.diffuse,  TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(mat.diffuse,  TexCoord));
    vec3 specular = light.specular * spec * mat.specular;


    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;


    return (ambient + diffuse + specular);

}


//================================================================


float LinearizeDepth(float depth) 
{

    float near = 0.1; 
    float far  = 100.0; 


    float z = depth * 2.0 - 1.0; 
    return (2.0 * near * far) / (far + near - z * (far - near));	

}


//================================================================


void main()
{

    vec3 objectColor = texture(mat.diffuse, TexCoord).rgb;
    vec3 viewDir     = normalize(cameraPos - FragPos);
    vec3 result      = vec3(0.0);


    for(int i = 0; i < lightsNum; i++)
    {

        result += CalcPointLight(pointLights[i], Normal, FragPos, viewDir); 

    }
        

    //float depth = LinearizeDepth(gl_FragCoord.z) / 100.0;
    //FragColor = vec4(vec3(depth), 1.0);

    
    FragColor = vec4(result * objectColor, 1.0);

}


//================================================================