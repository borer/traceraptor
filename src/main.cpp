#include <Logger.h>
#include <string>

#include "Image.h"
#include "Vec3.h"
#include "Ray.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "UtilMath.h"
#include "Renderer.h"

using namespace traceraptor;

int main(int argc, char* argv[]) {
	std::string fileName = (argc < 2) ? "output.png" : argv[1];

    const int width = 200;
    const int height = 100;
    const int ns = 10;
    const int MAX_RAY_BOUNCE = 50;

    Vec3 lookfrom(13,2,3);
    Vec3 lookat(0,0,0);
    float dist_to_focus = 10.0;
    float aperture = 0.1;

    Camera camera(lookfrom, lookat, Vec3(0,1,0), 20, float(width)/float(height), aperture, dist_to_focus);

    Renderer renderer(width, height, ns, MAX_RAY_BOUNCE);
    renderer.render_random_scene(camera, fileName, 4);

    return 0;
}
