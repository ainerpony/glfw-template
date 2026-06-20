#version 330 core
out vec4 FragColor;

uniform vec2 u_resolution;
uniform float u_time;

vec4 exampleCircle()
{
    vec2 FC = gl_FragCoord.xy;
    vec2 r = u_resolution;
    vec4 fragColor;
    float d = distance(0.5*r, FC);

    if (d <= 300) {
        fragColor = vec4((FC.x/r.x), 0.0, (FC.y/r.y), 0.0)*1.25;
    }

    return fragColor;
}

vec4 exampleTri()
{
    /* Shader by XorDev: https://x.com/XorDev/status/1956070340414558241 */

    vec4 o;
    float t = u_time;
    vec2 r = u_resolution, FC = gl_FragCoord.xy;
    vec2 p = (FC.xy*2.-r)/r.y, b = vec2(.58*clamp(.4-p.y,0.,.8),.4), v = p-clamp(p,-b,b), l; l += length(v);

    o = tanh((cos(atan(p.y,p.x)+t+vec4(0,2,4,0))+1.1)/l.y/3e1)*min(l*r*.2,1.).y;
    return o;
}

void main()
{
    FragColor = exampleTri();
}