#include "Application/gui.hpp"
#include "dependencies/stb/stb_image.h"
#include "dependencies/stb/stb_image_write.h"
#include "scenes/S_bouncingSpheres.hpp"
#include "scenes/S_checkeredCheckeredSpheres.hpp"
#include "scenes/S_checkeredMetalSpheres.hpp"
#include "scenes/S_checkeredSpheres.hpp"
#include "scenes/S_earth.hpp"
#include "scenes/S_perlinSpheres.hpp"
#include "scenes/scene.hpp"
// GLFW
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>
#include <vector>

int IMAGE_WIDTH = 1920 / 4;
int IMAGE_HEIGHT = 1080 / 4;

using std::make_shared;
using std::string;
using std::vector;

int main() {

    Gui::setup();

    S_bouncingSpheres S_bouncingSpheresInstance;
    S_perlinSpheres S_perlinSpheresInstance;
    S_checkeredSpheres S_checkeredSpheresInstance;
    S_checkeredCheckeredSpheres S_checkeredCheckeredSpheresInstance;
    Gui::renderScene = &S_bouncingSpheresInstance;

    // Main Loop
    while (!glfwWindowShouldClose(Gui::glfw::window)) {
        Gui::update();
    }

    Gui::terminate();

    return 0;
}
