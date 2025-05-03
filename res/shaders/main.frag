#version 410 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform sampler2D _texture;
uniform sampler2D shadowMap;
uniform vec3 pelletColor;
uniform float vignetteStrength;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec2 resolution;

float InShadow(vec4 fragPosLightSpace) {
    vec3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w) * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    vec3 lightDir = normalize(lightPos - FragPos);
    float bias = max(0.00005 * (1.0 - dot(normalize(Normal), lightDir)), 0.000005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; x++) {
        for(int y = -1; y <= 1; y++) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    if (projCoords.z > 1.0) shadow = 0.0;

    return shadow;
}

vec4 Posterize(in vec4 frag) {
    float gamma = 0.3f;
    float numColors = 40.0f;

    vec3 c = frag.rgb;
    c = pow(c, vec3(gamma, gamma, gamma));
    c = c * numColors;
    c = floor(c);
    c = c / numColors;
    c = pow(c, vec3(1.0/gamma));
    
    return vec4(c, frag.a);
}

void main()
{
    vec3 lightColor = vec3(1.8, 0.66, 0.66);

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

    float shadow = InShadow(FragPosLightSpace);

    vec3 albedo = texture(_texture, TexCoord).rgb * pelletColor;
	vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * albedo;

    vec2 uv = (gl_FragCoord.xy / resolution);
    float d = length(uv - 0.5);
    float vig = smoothstep(vignetteStrength, 0.35, d);

    float brightness = dot(albedo, vec3(0.2126, 0.7152, 0.0722)); // using albedo instead of result as a "hack" to make sure that lighting calculations like shadow and diffuse don't dampen the HDR value of lights
    if (brightness > 1.0) {
        BrightColor = vec4(albedo, 1.0);
    } else {
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }

    FragColor = Posterize(vec4(result, 1.0)) * vig;
    BrightColor = BrightColor * vig;
}