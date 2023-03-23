#version 330 core

layout (location = 0) out vec4  color;
//uniform vec3 colour = vec3(0.35, 0.48, 0.95);
//uniform vec3 colour = vec3(0.8, 0.8, 0.8);
uniform vec3 colour = vec3(1.0, 1.0, 1.0);
uniform vec2 light_pos;
uniform int OCTAVES = 4;

uniform float time;
//const float amplitude = 0.125;
//const float frequency = 4;
//const float PI = 3.14159;

in DATA
{
    vec4 position;
    vec2 uv;
    vec4 color;
} fs_in;

float rand(vec2 coord){
    return fract(sin(dot(coord, vec2(56, 78)) * 1000.0) * 1000.0);
}

float noise(vec2 coord){
    vec2 i = floor(coord);
    vec2 f = fract(coord);

    // 4 corners of a rectangle surrounding our point
    float a = rand(i);
    float b = rand(i + vec2(1.0, 0.0));
    float c = rand(i + vec2(0.0, 1.0));
    float d = rand(i + vec2(1.0, 1.0));

    vec2 cubic = f * f * (3.0 - 2.0 * f);

    return mix(a, b, cubic.x) + (c - a) * cubic.y * (1.0 - cubic.x) + (d - b) * cubic.x * cubic.y;
}

float fbm(vec2 coord){
    float value = 0.0;
    float scale = 0.5;

    for(int i = 0; i < OCTAVES; i++){
        value += noise(coord) * scale;
        coord *= 2.0;
        scale *= 0.5;
    }
    return value;
}

void main()
{
    float textBody = 0.2;
    float intensityOn = 1.0 / length(fs_in.position.xy - light_pos);

    vec2 coord = fs_in.uv * 20.0;

    if (fs_in.color.w < textBody)
        discard;

    vec2 motion = vec2( fbm(coord + vec2(time * -0.5, time * 0.5)) );

    float final = fbm(coord + motion);

    color = vec4(colour, final * 0.5);// * intensityOn * 5.0f;
}

