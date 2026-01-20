#ifndef GUI_EXTENDED_HPP
#define GUI_EXTENDED_HPP

#include "core/DebugConsole.hpp"
#include <future>
#include <vector>

// Forward declarations for external libraries
struct GLFWwindow;
struct GLFWmonitor;
struct GLFWvidmode;
struct scene;

using GLuint = unsigned int;

namespace Gui {
    extern DebugConsole ScanlineLog;
    extern scene *renderScene;
    // High-level GUI lifecycle
    void setup();
    void update();
    void terminate();

    namespace glfw {
        extern GLFWmonitor *monitor;
        extern const GLFWvidmode *mode;
        extern GLFWwindow *window;

        extern double ASPECT_RATIO;
        extern int IMAGE_WIDTH;
        extern int IMAGE_HEIGHT;
        extern std::vector<unsigned char> pixels;
        extern GLuint renderedImage;

        void setup();
        void terminate();
    } // namespace glfw

    namespace Im {
        // Setup/teardown
        void setup();
        void terminate();

        // Additional GUI logic
        void createDockSpace();
        void defineGui();
    } // namespace Im
} // namespace Gui

// External function used in rendering (must be declared somewhere else)
void bouncingSpheres(std::vector<unsigned char> &pixels);

#endif // GUI_EXTENDED_HPP
