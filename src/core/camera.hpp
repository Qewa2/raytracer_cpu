#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "core/color.hpp"
#include "core/vec3.hpp"
#include "hittable/bvh.hpp"
#include "hittable/hittable.hpp"
#include "hittable/hittableList.hpp"

#include <fstream>
#include <string>
#include <vector>

using std::string;

class camera {
  public:
    camera();
    camera(const string &ImagePath);

    std::vector<unsigned char> *pixels;

    double AspectRatio = 16.0 / 9.0;
    int ImageWidth = 1920;
    int SamplesPerPixel = 10;
    int maxDepth = 10;

    double vfov = 90; // Vertical viewing angle
    point3 lookFrom = point3(0, 0, 0);
    point3 lookAt = point3(0, 0, -1);
    vec3 vup = vec3(0, 1, 0);

    double defocusAngle = 0;
    double focusDist = 10;
    bool useBvh = true;

    void render(const hittableList &world);

  private:
    string imagePath;
    std::ofstream Image;
    int ImageHeight;
    double pixelSamplesScale;
    point3 center;
    point3 pixel00_loc;
    vec3 pixelDelta_u;
    vec3 pixelDelta_v;
    vec3 u, v, w; // u = camera right vector, v = camera up vector, w = opposit
                  // direction of lookAt (-w)
    vec3 defocusDisk_u; // defocusDisk horizontal radius
    vec3 defocusDisk_v; // defocusDisk vertical radius

    void initialize();

    ray getRay(int i, int j) const;

    vec3 sampleSquare() const;

    point3 defocusDiskSample() const;

    color rayColor(const ray &r, int depth, const hittable &world) const;
};

#endif // CAMERA_HPP
