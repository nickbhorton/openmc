#version 450 core

layout (local_size_x = 1, local_size_y = 1) in;
layout (std430, binding=0) buffer Positions {
    vec4 positions[];
};
layout (location = 0) uniform float t;
layout (location = 1) uniform uint xd;
layout (location = 2) uniform uint zd;

#define M_PI 3.1415926
#define f 20.69e9
#define D 0.1
#define R0 0.15
#define c 3e8

float lambda = c/f;
float w0 = sqrt((lambda * sqrt(D*(R0-D)))/M_PI);
float zc = (M_PI * w0*w0)/(lambda);
float k = 2.0 * M_PI / lambda;

float atan2(float y, float x) {
    bool s = (abs(x) > abs(y));
    return mix(M_PI/2.0 - atan(x,y), atan(y,x), s);
}

// beam radius
float w(float z) {
    return w0 * sqrt(1.0 + (z/zc)*(z/zc));
}

// radius of curvature
float R(float z) {
    return z + zc*zc/z;
}

// phase shift
float phase(float z) {
    return atan2(z,zc);
}

void main() {
    uint xi = gl_GlobalInvocationID.x;
    uint zi = gl_GlobalInvocationID.y;
    uint idx = xi * xd + zi;

    float r = (float(xi) / xd) - 0.5;
    float z = (float(zi) / zd) - 0.5;

    float scale_r = 10.0;
    float scale_z = 5.0;

    vec3 p;
    p.x = 100.0 * r;
    p.z = 100.0 * z;

    r = r / scale_r;
    z = z / scale_z;
    float E_re = (w0 / w(z)) * exp(- (r*r)/(w(z) * w(z))) * cos(k * (z + ((r*r)/(2.0 * R(z))) - phase(z)/k));
    float E_im = (w0 / w(z)) * exp(- (r*r)/(w(z) * w(z))) * -sin(k * (z + ((r*r)/(2.0 * R(z))) - phase(z)/k));
    p.y = E_re * cos(k * c * t*1e-10 * (1.0/60.0));

    positions[idx].xyz = p;
}
