#version 330 core
out vec4 FragColor;

uniform float time;

void main() {
    // Цвет меняется от зелёного (0,1,0) к фиолетовому (1,0,1) и обратно
    float factor = (sin(time) + 1.0) / 2.0; // от 0 до 1
    vec3 color = mix(vec3(0.0, 1.0, 0.0), vec3(1.0, 0.0, 1.0), factor);
    FragColor = vec4(color, 1.0);
}

