#version 410 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 Normal;
in vec3 FragPos;
in vec4 Color;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{   
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

	// ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.75;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 albedo = Color.rgb;
    float alpha = Color.a;
	vec3 result = (ambient * (diffuse + specular)) * albedo;

    float brightness = dot(albedo, vec3(0.2126, 0.7152, 0.0722)); // using albedo instead of result as a "hack" to make sure that lighting calculations like shadow and diffuse don't dampen the HDR value of lights
    if (brightness > 1.0) {
        BrightColor = vec4(albedo, alpha);
    } else {
        BrightColor = vec4(0.0, 0.0, 0.0, alpha);
    }

    FragColor = vec4(result, alpha);
    BrightColor = BrightColor;
}