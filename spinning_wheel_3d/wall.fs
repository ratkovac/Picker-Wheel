#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform sampler2D texture2; // Uzorak drveta

void main()
{
    // Uzorkujemo boju drveta
    vec3 wallColor = texture(texture2, TexCoord).rgb;
    
    // Ambient komponenta
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;
    
    // Difuzna komponenta (Phong shading)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Spekularna komponenta (Phong shading)
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    // Kombiniramo sve komponente s bojom teksture
    vec3 result = (ambient + diffuse + specular) * wallColor;
    FragColor = vec4(result, 1.0);
}
