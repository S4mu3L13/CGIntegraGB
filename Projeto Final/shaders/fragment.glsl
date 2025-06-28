#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 lightDir;
uniform sampler2D texture_diffuse1;

void main() {
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
    vec3 ambient = vec3(0.2);
    vec3 color = (ambient + diffuse) * texture(texture_diffuse1, TexCoords).rgb;
    FragColor = vec4(color, 1.0);
}
