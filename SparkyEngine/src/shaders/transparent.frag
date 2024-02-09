#version 330 core

layout (location = 0) out vec4  color;

uniform vec4 colour;
uniform vec4 light_color;
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

    float intensityOff = 1;
    float highIntensity = 6.0f;
    vec4 texColor = fs_in.color;
    float textBody = 0.9;

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

    //if (fs_in.color.w < textBody) {
      //  discard;
    //}

    color = texColor;
}

