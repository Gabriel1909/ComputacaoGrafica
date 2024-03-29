#version 450

in vec3 finalColor;
in vec3 scaledNormal;
in vec3 fragPos;
in vec2 texCoord;

uniform float ka;
uniform float kd;
uniform float ks;
uniform float q;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 cameraPos;

out vec4 color;

uniform sampler2D colorBuffer;

void main() {

    vec3 ambient =  lightColor * ka;

    vec3 N = normalize(scaledNormal);
    vec3 L = normalize(lightPos - fragPos);
    float diff = max(dot(N, L),0.0);
    vec3 diffuse = diff * lightColor * kd;
    
    vec3 R = reflect(-L,N);
    vec3 V = normalize(cameraPos - fragPos);
    float spec = pow(max(dot(R,V),0.0),q);
    vec3 specular = spec * ks * lightColor;
    
    vec4 texColor = texture(colorBuffer,texCoord);
    vec3 result = (ambient + diffuse) * vec3(texColor) + specular;

    color = vec4(result, 1.0f);
}
