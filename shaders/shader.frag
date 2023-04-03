#version 330 core

uniform float aspectRatio;
uniform vec3 camPos;

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

void main() {
    vec2 uv = vec2(v_texCoords.x, v_texCoords.y);
    vec3 color = vec3(0);

    float viewportHeight = 2.0;
    float viewportWidth = aspectRatio * viewportHeight;
    float focalLength = 1.0;

    Ray ray;
    ray.origin = camPos;
    ray.dir = vec3(uv.x*viewportWidth - viewportWidth/2.0, uv.y * viewportHeight-viewportHeight/2.0, 0)+vec3(0,0,-1);

    HitInfo info = hitSphere(ray, vec3(0,0,-1),0.5);
    if(info.didHit){
        color = 0.5*(info.normal + vec3(1,1,1));
    }else {
        float t = 0.5*(normalize(ray.dir).y + 1.0);
        color = (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }



    fragColor = vec4(color, 1);
}
