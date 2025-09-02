#version 410

#define PI 3.14159265359
#define MAX_POINT_LIGHTS 32

struct PointLight {
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in mat3 TBN;
in vec3 GouraudLight;
in vec3 GouraudAlbedo;
in vec4 FragPosLightSpace;

uniform bool disableNormalMapping;
uniform bool enableToneMapping;
uniform float toneMappingExposure;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

uniform vec3 color;
uniform vec3 ambientColor;
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform float roughness;
uniform float metallic;
uniform int filterType;

uniform vec3 cameraPosition;

uniform samplerCube skybox;
uniform float reflectionStrength;

uniform vec3 spotPos;
uniform vec3 spotDir;
uniform float cutOff;
uniform float outerCutOff;

uniform int numPointLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform sampler2D shadowMap;
uniform int illuminationModel;

float calculateShadow(vec4 fragPosLightSpace, vec3 normal, vec3 direction) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0)
        return 0.0;

    float shadow = 0.0;
    float bias = max(0.05 * (1.0 - dot(normal, direction)), 0.005);
    float currentDepth = projCoords.z;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    return shadow / 25.0;
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

vec3 CalculatePointLights(vec3 N, vec3 V, vec3 albedo, vec3 F0, float roughness, float metallic) {
    vec3 totalPointLight = vec3(0.0);
    
    for (int i = 0; i < numPointLights; i++) {
        PointLight light = pointLights[i];
        vec3 L = normalize(light.position - FragPos);
        vec3 H = normalize(V + L);
        float distance = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        
        float NDF = DistributionGGX(N, H, roughness);       
        float G = GeometrySmith(N, V, L, roughness);      
        vec3 F = F0 + (1.0 - F0) * pow(1.0 - max(dot(H, V), 0.0), 5.0);  // Fixed this line
        
        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
        vec3 specular = numerator / denominator;
        
        vec3 kD = (vec3(1.0) - F) * (1.0 - metallic);
        vec3 diffuse = kD * albedo / PI;
        
        float NdotL = max(dot(N, L), 0.0);        
        totalPointLight += (diffuse + specular) * light.color * NdotL * attenuation;
    }
    
    return totalPointLight;
}

void main() {
    vec3 lightDirection = normalize(-lightDir);
    vec3 viewDirection = normalize(cameraPosition - FragPos);
    vec3 halfwayDir = normalize(lightDirection + viewDirection);

    vec3 normal = TBN[2];
    if (!disableNormalMapping) {
        normal = texture(texture_normal1, TexCoord).rgb;
        normal = normalize(normal * 2.0 - 1.0);
        normal = normalize(TBN * normal);
    }

    vec3 albedo = texture(texture_diffuse1, TexCoord).rgb;
    if (color.r != -1.0 && color.g != -1.0 && color.b != -1.0)
        albedo = color;

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    float NdotV = max(dot(normal, viewDirection), 0.0);
    vec3 fresnel = F0 + (1.0 - F0) * pow(1.0 - NdotV, 5.0);
    vec3 kD = (1.0 - fresnel) * (1.0 - metallic);

    // Spotlight calculations
    vec3 spotToFrag = normalize(FragPos - spotPos);
    float theta = dot(spotToFrag, normalize(-spotDir));
    float epsilon = cutOff - outerCutOff;
    float spotIntensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);
    float NdotL_spot = max(dot(normal, -spotToFrag), 0.0);
    vec3 spotHalfway = normalize(-spotToFrag + viewDirection);
    float spotSpecStrength = pow(max(dot(normal, spotHalfway), 0.0), (1.0 - roughness) * 128.0);
    vec3 spotDiffuse = kD * albedo / PI * NdotL_spot;
    vec3 spotSpecular = spotSpecStrength * fresnel * lightColor;
    vec3 spotlight = (spotDiffuse + spotSpecular) * spotIntensity;

    // Point lights calculation
    vec3 pointLighting = CalculatePointLights(normal, viewDirection, albedo, F0, roughness, metallic);

    // Base lighting model
    vec3 baseLighting = vec3(0.0);
    if (illuminationModel == 0) { // Lambert
        float NdotL = max(dot(normal, lightDirection), 0.0);
        baseLighting = albedo * lightColor * NdotL;
    } else if (illuminationModel == 1) { // Gouraud
        vec3 textureAlbedo = texture(texture_diffuse1, TexCoord).rgb;
        vec3 chosenAlbedo = (color.r != -1.0 && color.g != -1.0 && color.b != -1.0) ? GouraudAlbedo : textureAlbedo;
        baseLighting = chosenAlbedo * GouraudLight;
    } else if (illuminationModel == 4) { // PBR
        vec3 H = normalize(viewDirection + lightDirection);
        float NDF = DistributionGGX(normal, H, roughness);
        float G = GeometrySmith(normal, viewDirection, lightDirection, roughness);
        vec3 F = fresnel;
        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * NdotV * max(dot(normal, lightDirection), 0.0) + 0.001;
        vec3 specular = numerator / denominator;
        vec3 diffuse = kD * albedo / PI;
        baseLighting = (diffuse + specular) * lightColor * max(dot(normal, lightDirection), 0.0);
    }

    // Shadows and final composition
    float shadow = calculateShadow(FragPosLightSpace, normal, lightDirection);
    vec3 ambient = albedo * ambientColor;
    vec3 finalColor = ambient + baseLighting * (1.0 - shadow) + spotlight + pointLighting;

    // Reflection mapping
    vec3 I = normalize(FragPos - cameraPosition);
    vec3 R = reflect(I, normalize(normal));
    vec3 reflectionColor = texture(skybox, R).rgb;
    vec3 reflectionMapped = reflectionColor / (reflectionColor + vec3(1.0));
    reflectionMapped *= (1.0 - roughness);
    float reflectionFactor = clamp(fresnel.r * reflectionStrength, 0.0, 1.0);
    finalColor = mix(finalColor, reflectionMapped, reflectionFactor);

    // Post-processing effects
    if (filterType == 1) { // Grayscale
        float gray = dot(finalColor, vec3(0.299, 0.587, 0.114));
        finalColor = vec3(gray);
    } else if (filterType == 2) { // Inverted
        finalColor = vec3(1.0) - finalColor;
    } else if (filterType == 3) { // Posterize
        float levels = 8.0;
        finalColor = floor(finalColor * levels) / levels;
    }

    // Tone mapping
    if (enableToneMapping) {
        vec3 hdrColor = finalColor * toneMappingExposure;
        finalColor = hdrColor / (hdrColor + vec3(1.0));
    }

    FragColor = vec4(finalColor, 1.0);
}