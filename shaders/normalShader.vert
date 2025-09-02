#version 410

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

out vec2 TexCoord;
out vec3 FragPos;
out mat3 TBN;
out vec3 GouraudLight;
out vec3 GouraudAlbedo;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 color;
uniform vec3 ambientColor;
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 cameraPosition;

uniform int illuminationModel;

uniform mat4 lightSpaceMatrix;

void main() {
    vec3 T = normalize(mat3(model) * aTangent);
    vec3 N = normalize(mat3(model) * aNormal);
    vec3 B = cross(N, T);

    TBN = mat3(T, B, N);
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoord = aTexCoord;

    gl_Position = projection * view * model * vec4(aPos, 1.0f);

    GouraudLight = vec3(0.0);
    GouraudAlbedo = vec3(1.0);

    if (illuminationModel == 1) {
        vec3 albedo = (color.r != -1.0 && color.g != -1.0 && color.b != -1.0) ? color : vec3(1.0);
        GouraudAlbedo = albedo;

        vec3 normal = normalize(mat3(model) * aNormal);
        vec3 lightDirection = normalize(-lightDir);
        vec3 viewDir = normalize(cameraPosition - FragPos);
        vec3 halfwayDir = normalize(lightDirection + viewDir);

        float NdotL = max(dot(normal, lightDirection), 0.0);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

        vec3 ambient = ambientColor;
        vec3 diffuse = NdotL * lightColor;
        vec3 specular = spec * lightColor;

        GouraudLight = ambient + diffuse + specular;
    }

    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}
