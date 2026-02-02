#version 410 core

out vec4 fragc;

uniform vec3 color;
uniform sampler2D tex;

vec3 lightPosition = vec3(10000.0);

in prop {
    vec3 normal;
    vec3 fragp;
    vec2 uv;
} fs_in;

void main() {

    vec3 col = texture(tex, fs_in.uv).rgb;

    vec3 ambient = col * 0.8;

    vec3 lightDirection = normalize(vec3(1.0));
    float diff = max(dot(fs_in.normal, lightDirection), 0.0);

    vec3 shadowColor = vec3(0.2, 0.2, 0.5);
    vec3 diffuse = mix(shadowColor, vec3(1.0), diff);

    fragc = vec4(col * (ambient + diffuse), 1.0);
}
