#pragma once
#include <raylib.h>
#include <string>

/// @brief TYPE OF ELEMENT
enum TYPE : int
{
  null = 999,
  liquid = 0,
  water,
  lava,

  gas = 100,
  steam,

  moving_solid = 200,
  sand,
  dirt,
  snow,

  unmovable_solid = 300,
  brick,
  ice,
};


struct Cube
{
  Vector3 pos = {0,0,0};
  bool empty = true;
  bool hidden = false;
  TYPE celltype = null;
  int stationary = 0;
};








struct shader_data {
  const char* instance_shader_fs =
R""""(
#version 330

  // Input vertex attributes (from vertex shader)
  in vec3 fragPosition;
  in vec2 fragTexCoord;
  //in vec4 fragColor;
  in vec3 fragNormal;

  // Input uniform values
  uniform sampler2D texture0;
  uniform vec4 colDiffuse;

  // Output fragment color
  out vec4 finalColor;

  // NOTE: Add here your custom variables

#define     MAX_LIGHTS              4
#define     LIGHT_DIRECTIONAL       0
#define     LIGHT_POINT             1

  struct Light {
    int enabled;
    int type;
    vec3 position;
    vec3 target;
    vec4 color;
  };

  // Input lighting values
  uniform Light lights[MAX_LIGHTS];
  uniform vec4 ambient;
  uniform vec3 viewPos;

  void main() {
    // Texel color fetching from texture sampler
    vec4 texelColor = texture(texture0, fragTexCoord);
    vec3 lightDot = vec3(0.0);
    vec3 normal = normalize(fragNormal);
    vec3 viewD = normalize(viewPos - fragPosition);
    vec3 specular = vec3(0.0);

    // NOTE: Implement here your fragment shader code

    for (int i = 0; i < MAX_LIGHTS; i++) {
      if (lights[i].enabled == 1) {
        vec3 light = vec3(0.0);

        if (lights[i].type == LIGHT_DIRECTIONAL) {
          light = -normalize(lights[i].target - lights[i].position);
        }

        if (lights[i].type == LIGHT_POINT) {
          light = normalize(lights[i].position - fragPosition);
        }

        float NdotL = max(dot(normal, light), 0.0);
        lightDot += lights[i].color.rgb * NdotL;

        float specCo = 0.0;
        if (NdotL > 0.0)
          specCo = pow(max(0.0, dot(viewD, reflect(-(light), normal))),
                       16.0); // 16 refers to shine
        specular += specCo;
      }
    }

    finalColor =
        (texelColor * ((colDiffuse + vec4(specular, 1.0)) * vec4(lightDot, 1.0)));
    finalColor += texelColor * colDiffuse;

    // Gamma correction
    finalColor = pow(finalColor, vec4(1.0 / 2.2));
  }
)"""";
  const char* instance_shader_vs =
    R""""(
#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
//in vec4 vertexColor;      // Not required

in mat4 instanceTransform;

// Input uniform values
uniform mat4 mvp;
uniform mat4 matNormal;

// Output vertex attributes (to fragment shader)
out vec3 fragPosition;
out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 fragNormal;

// NOTE: Add here your custom variables

void main()
{
    // Compute MVP for current instance
    mat4 mvpi = mvp*instanceTransform;

    // Send vertex attributes to fragment shader
    fragPosition = vec3(mvpi*vec4(vertexPosition, 1.0));
    fragTexCoord = vertexTexCoord;
    //fragColor = vertexColor;
    fragNormal = normalize(vec3(matNormal*vec4(vertexNormal, 1.0)));

    // Calculate final vertex position
    gl_Position = mvpi*vec4(vertexPosition, 1.0);
}
)"""";
};