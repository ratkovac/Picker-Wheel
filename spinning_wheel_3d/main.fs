#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform bool useSectorColors;
uniform int numParts;
uniform vec3 sectorColors[15];
uniform float wheelRotationAngle;

void main()
{
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 finalColor;
    if (useSectorColors) {
        float angle = atan(FragPos.z, FragPos.x);
        if (angle < 0.0) angle += 2.0 * 3.1415926;

        angle = mod(angle + wheelRotationAngle, 2.0 * 3.1415926);

        float sectorAngle = 2.0 * 3.1415926 / float(numParts);
        int sectorIndex = int(angle / sectorAngle);
        finalColor = sectorColors[sectorIndex];
    } else {
        finalColor = objectColor;
    }

    vec3 result = (ambient + diffuse + specular) * finalColor;
    FragColor = vec4(result, 1.0);
}
