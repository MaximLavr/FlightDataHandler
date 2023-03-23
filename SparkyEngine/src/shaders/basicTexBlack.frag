#version 330 core

layout (location = 0) out vec4  color;

uniform vec4 colour;
uniform vec2 light_pos;
uniform vec2 light_pos1;
uniform vec2 light_pos_star1;
uniform vec2 light_pos_star2;
uniform vec2 light_pos_star3;
uniform vec2 light_pos_star4;
uniform vec2 light_pos_star5;
uniform vec2 light_pos_star6;
uniform vec2 light_pos_star7;
uniform vec2 light_pos_star8;
uniform vec2 light_pos_star9;
uniform vec2 light_pos_star10;
uniform vec2 light_pos_star11;
uniform vec2 light_pos_star12;
uniform vec2 light_pos_star13;
uniform vec2 light_pos_star14;
uniform vec2 light_pos_star15;
uniform vec2 light_pos_star16;
uniform vec2 light_pos_star17;
uniform int text;

in DATA
{
    vec4 position;
    vec2 uv;
    float tid;
    vec4 color;
} fs_in;


uniform sampler2D textures[32];
uniform sampler2D tex;

void main()
{
    float intensityOn = 2.5 / length(fs_in.position.xy - light_pos);
    float intensityOn1 = 5.5 / length(fs_in.position.xy - light_pos1);

    float intensity_star1 = 0.02 / length(fs_in.position.xy - light_pos_star1);
    float intensity_star2 = 0.02 / length(fs_in.position.xy - light_pos_star2);
    float intensity_star3 = 0.02 / length(fs_in.position.xy - light_pos_star3);
    float intensity_star4 = 0.02 / length(fs_in.position.xy - light_pos_star4);
    float intensity_star5 = 0.02 / length(fs_in.position.xy - light_pos_star5);
    float intensity_star6 = 0.02 / length(fs_in.position.xy - light_pos_star6);
    float intensity_star7 = 0.02 / length(fs_in.position.xy - light_pos_star7);
    float intensity_star8 = 0.02 / length(fs_in.position.xy - light_pos_star8);
    float intensity_star9 = 0.02 / length(fs_in.position.xy - light_pos_star9);
    float intensity_star10 = 0.02 / length(fs_in.position.xy - light_pos_star10);
    float intensity_star11 = 0.02 / length(fs_in.position.xy - light_pos_star11);
    float intensity_star12 = 0.02 / length(fs_in.position.xy - light_pos_star12);
    float intensity_star13 = 0.02 / length(fs_in.position.xy - light_pos_star13);
    float intensity_star14 = 0.02 / length(fs_in.position.xy - light_pos_star14);
    float intensity_star15 = 0.02 / length(fs_in.position.xy - light_pos_star15);
    float intensity_star16 = 0.02 / length(fs_in.position.xy - light_pos_star16);
    float intensity_star17 = 0.02 / length(fs_in.position.xy - light_pos_star17);

    float intensityOff = 1;
    float highIntensity = 6.0f;
    vec4 texColor = fs_in.color;
    float textBody = 0.2;
    vec4 shadow = vec4(0, 0, 0, 0.8f);

    if (fs_in.tid > 0.0f)
    {
        if (fs_in.tid == 1.0f)
        texColor = fs_in.color * texture(textures[0], fs_in.uv);
        if (fs_in.tid == 2.0f)
        texColor = fs_in.color * texture(textures[1], fs_in.uv);
        if (fs_in.tid == 3.0f)
        texColor = fs_in.color * texture(textures[2], fs_in.uv);
        if (fs_in.tid == 4.0f)
        texColor = fs_in.color * texture(textures[3], fs_in.uv);
        if (fs_in.tid == 5.0f)
        texColor = fs_in.color * texture(textures[4], fs_in.uv);
        if (fs_in.tid == 6.0f)
        texColor = fs_in.color * texture(textures[5], fs_in.uv);
        if (fs_in.tid == 7.0f)
        texColor = fs_in.color * texture(textures[6], fs_in.uv);
        if (fs_in.tid == 8.0f)
        texColor = fs_in.color *  texture(textures[7], fs_in.uv);
        if (fs_in.tid == 9.0f)
        texColor = fs_in.color *  texture(textures[8], fs_in.uv);
        if (fs_in.tid == 10.0f)
        texColor = fs_in.color *  texture(textures[9], fs_in.uv);
    }

    if (texColor.w < textBody) {
        discard;
    }

    vec4 color1 = vec4(0.33, 0.15, 0.82, 1.0);

    color = texColor;
    /*color = texColor * color1 * intensityOn + texColor * intensityOn1 * color1
    + texColor * intensity_star1
    + texColor * intensity_star2
    + texColor * intensity_star3
    + texColor * intensity_star4
    + texColor * intensity_star5
    + texColor * intensity_star6
    + texColor * intensity_star7
    + texColor * intensity_star8
    + texColor * intensity_star9
    + texColor * intensity_star10
    + texColor * intensity_star11
    + texColor * intensity_star12
    + texColor * intensity_star13
    + texColor * intensity_star14
    + texColor * intensity_star15
    + texColor * intensity_star16
    + texColor * intensity_star17
    ; // * color1;///* color1;*/
}

