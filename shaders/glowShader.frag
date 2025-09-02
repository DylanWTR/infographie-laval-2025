#version 330 core
out vec4 FragColor;
uniform vec3 color;

void main() {
    float alpha = abs(sin(gl_FragCoord.x * 0.02 + gl_FragCoord.y * 0.02));
    FragColor = vec4(color, 0.6 + 0.4 * alpha);
}
