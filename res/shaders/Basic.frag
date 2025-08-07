#version 330 core
#define PI 3.14159265358979323846

in vec2 v_TexCoord;
in vec4 pos;

uniform sampler2D u_Tex;
uniform uint time;

out vec4 color;

float screenWidth = 1.0;

float rand(vec2 c);
float noise(vec2 p, float freq);
float pNoise(vec2 p, int res);

void main() {
    float mult = (int(floor(time / 40u)) % 10) / 10.0;
    vec2 rOffset = vec2(0.01, 0.01) * mult;
    vec2 gOffset = vec2(0.5, 0.2) + mult;
    vec2 bOffset = vec2(0.8, 0.1) * mult;
    vec4 polyShader = vec4(
            pNoise(pos.xy + rOffset, 1), 
            pNoise(pos.xy + gOffset, 1),
            pNoise(pos.xy + bOffset, 1),
            1.0);
    // color = texture(u_Tex, v_TexCoord) + vec4(0.5, 0.5, 0.3, 1.0);
    // color = texture(u_Tex, v_TexCoord) + polyShader;
    color = texture(u_Tex, v_TexCoord) + polyShader;
};

float rand(vec2 c) {
    return fract(sin(dot(c.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float noise(vec2 p, float freq) {
    float unit = screenWidth / freq;
    vec2 ij = floor(p / unit);
    vec2 xy = mod(p, unit) / unit;
    //xy = 3.*xy*xy-2.*xy*xy*xy;
    xy = .5 * (1. - cos(PI * xy));
    float a = rand((ij + vec2(0., 0.)));
    float b = rand((ij + vec2(1., 0.)));
    float c = rand((ij + vec2(0., 1.)));
    float d = rand((ij + vec2(1., 1.)));
    float x1 = mix(a, b, xy.x);
    float x2 = mix(c, d, xy.x);
    return mix(x1, x2, xy.y);
}

float pNoise(vec2 p, int res) {
    float persistance = .5;
    float n = 0.;
    float normK = 0.;
    float f = 4.;
    float amp = 1.;
    int iCount = 0;
    for (int i = 0; i < 50; i++) {
        n += amp * noise(p, f);
        f *= 2.;
        normK += amp;
        amp *= persistance;
        if (iCount == res) break;
        iCount++;
    }
    float nf = n / normK;
    return nf * nf * nf * nf;
}
