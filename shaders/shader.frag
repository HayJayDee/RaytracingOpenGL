#version 410 core

#define NUM_SPHERES 3

struct Sphere {
    vec3 pos;
    vec3 color;
    float radius;
};

uniform float aspectRatio;
uniform vec3 camPos;
uniform mat4 viewMat;

layout(std140) uniform Scene {
    Sphere spheres[NUM_SPHERES];
};

in vec2 v_texCoords;
out vec4 fragColor;

struct Ray {
    vec3 origin;
    vec3 dir;
};

struct HitInfo {
    bool didHit;
    float dst;
    vec3 hitPoint;
    vec3 normal;
    int index;
};

HitInfo hitSphere(Ray ray, vec3 center, float radius) {
    vec3 oc = ray.origin - center;
    float a = length(ray.dir)*length(ray.dir);
    float half_b = dot(oc, ray.dir);
    float c = length(oc)*length(oc) - radius * radius;
    float disc = half_b*half_b - a*c;
    if(disc >= 0){
        float dst = (-half_b-sqrt(disc))/a;

        if(dst >= 0) {
            HitInfo info;
            info.didHit = true;
            info.dst = dst;
            info.hitPoint = ray.origin + ray.dir * dst;
            info.normal = normalize(info.hitPoint - center);
            return info;
        }
    }
    HitInfo info;
    info.didHit = false;
    return info;
}

HitInfo calcHit(Ray ray) {
    HitInfo closestHit;
    closestHit.didHit = false;

    for(int i = 0; i < NUM_SPHERES; i++){
        Sphere sphere = spheres[i];
        HitInfo info = hitSphere(ray, sphere.pos.xyz, sphere.radius);
        info.index = i;

        if(!closestHit.didHit && info.didHit){
            closestHit = info;
            continue;
        }
        if(info.didHit && info.dst < closestHit.dst){
            closestHit = info;
        }
    }
    return closestHit;
}

void main() {
    vec2 uv = vec2(v_texCoords.x, v_texCoords.y);
    vec3 color = vec3(0);

    float viewportHeight = 2.0;
    float viewportWidth = aspectRatio * viewportHeight;
    float focalLength = 1.0;

    Ray ray;
    ray.origin = camPos;
    vec3 dir = vec3(uv.x*viewportWidth - viewportWidth/2.0, uv.y * viewportHeight-viewportHeight/2.0, 0)+vec3(0,0,-1);
    ray.dir = (viewMat * vec4(dir, 1.0)).xyz;

    HitInfo info = calcHit(ray);
    if(info.didHit){
        color = info.normal;
    }else {
        float t = 0.5*(normalize(ray.dir).y + 1.0);
        color = (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }

    fragColor = vec4(color, 1);
}
