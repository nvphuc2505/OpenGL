#version 460

in vec3 normal_eye;
in vec3 position_eye;

uniform mat4 view_mat;

vec3 light_position_world = vec3(0.0f, 0.0f, 2.0f);
vec3 Ld = vec3(0.7f, 0.7f, 0.7f);
vec3 Ls = vec3(1.0f, 1.0f, 1.0f);
vec3 La = vec3(0.2f, 0.2f, 0.2f);

vec3 Kd = vec3(1.0f, 0.5f, 0.0f);
vec3 Ks = vec3(1.0f, 1.0f, 1.0f);
vec3 Ka = vec3(1.0f, 1.0f, 1.0f);
float specular_exponent = 100.0f;

out vec4 frag_colour;

void main(){
    // Diffuse light intensity (Id)
    vec3 light_position_eye = vec3(view_mat * vec4(light_position_world, 1.0f));
    vec3 distance_to_light_eye = light_position_eye - position_eye;
    vec3 direction_to_light_eye = normalize(distance_to_light_eye);
    float dot_prod = dot(direction_to_light_eye, normal_eye);
    dot_prod = max(dot_prod, 0.0f);
    vec3 Id = Ld * Kd * dot_prod;

    // Specular light intensity (Is)
    vec3 surface_to_viewer_eye = normalize(-position_eye);

    // vec3 reflection_eye = reflect(-direction_to_light_eye, normal_eye);
    // float dot_prod_specular = dot(reflection_eye, surface_to_viewer_eye);
    // dot_prod_specular = max(dot_prod_specular, 0.0f);
    // float specular_factor = pow(dot_prod_specular, specular_exponent);

    // Blinn Phong
    vec3 half_way_eye = normalize(surface_to_viewer_eye + direction_to_light_eye);
    float dot_prod_specular = max(dot(half_way_eye, normal_eye), 0.0f);
    float specular_factor = pow(dot_prod_specular, specular_exponent);
    vec3 Is = Ls * Ks * specular_factor;

    vec3 Ia = La * Ka;
    
    frag_colour = vec4(Id + Is + Ia, 1.0f);
}