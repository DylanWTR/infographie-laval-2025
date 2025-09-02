#version 410 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

uniform bool  enableToneMapping;
uniform float toneMappingExposure;

void main() {
    vec3 color = texture(skybox, TexCoords).rgb;

    if (enableToneMapping) {
        vec3 hdrColor = color * toneMappingExposure;
        color = hdrColor / (hdrColor + vec3(1.0));
    }

    FragColor = vec4(color, 1.0);
}
