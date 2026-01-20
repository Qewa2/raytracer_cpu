#include "Application/gui.hpp"
#include "core/DebugConsole.hpp"
#include "scenes/S_bouncingSpheres.hpp"
#include "scenes/S_checkeredCheckeredSpheres.hpp"
#include "scenes/S_checkeredSpheres.hpp"
#include "scenes/S_perlinSpheres.hpp"

#include <chrono>
#include <future>
#include <iostream>

// GLFW
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <vector>

// ImGui
#include "dependencies/imgui/backends/imgui_impl_glfw.h"
#include "dependencies/imgui/backends/imgui_impl_opengl3.h"
#include "dependencies/imgui/imgui.h"
#include "imgui_internal.h"

namespace Gui {
    scene *renderScene;
    DebugConsole ScanlineLog;

    namespace glfw {

        // Setting up Glfw monitor
        GLFWmonitor *monitor;
        const GLFWvidmode *mode;

        GLFWwindow *window;

        // Rendered Image
        double ASPECT_RATIO = 16.0f / 9.0f;
        int IMAGE_WIDTH = 1920 / 2;
        int IMAGE_HEIGHT = IMAGE_WIDTH * (1 / ASPECT_RATIO); // 1080
        std::vector<unsigned char> pixels(IMAGE_WIDTH *IMAGE_HEIGHT * 3, 0);
        GLuint renderedImage;

        void glfwErrorCallback(int error, const char *description) {
            fprintf(stderr, "Error: %s\n", description);
        }

        void setup() {
            glfwSetErrorCallback(glfwErrorCallback);
            if (!glfwInit()) {
                std::cerr << "GLFW didn't initialize" << '\n';
                return;
            }

            monitor = glfwGetPrimaryMonitor();
            mode = glfwGetVideoMode(monitor);
            window = glfwCreateWindow(mode->width, mode->height, "raytracer",
                                      NULL, NULL);

            if (!window) {
                glfwTerminate();
                std::cerr << "window didn't initialize" << '\n';
                return;
            }

            glfwMakeContextCurrent(window);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glGenTextures(1, &renderedImage);
            glBindTexture(GL_TEXTURE_2D, renderedImage);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // Allocate empty Texture with windowSize
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, IMAGE_WIDTH, IMAGE_HEIGHT, 0,
                         GL_RGB, GL_UNSIGNED_BYTE, nullptr);

            // Swap interval
            glfwSwapInterval(1);
        }

        void terminate() {
            glfwDestroyWindow(window);
            glfwTerminate();
        }

    } // namespace glfw

    namespace Im {
        // Window Flags
        ImGuiWindowFlags windowFlags = 0;
        ImGuiWindowFlags windowFlagsMainWindow = 0;

        void setup() {
            // ImGui Context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();

            ImGuiIO &io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

            // ImGui Style
            ImGui::StyleColorsDark();

            // Setting up glfw and OpenGl3 backends
            ImGui_ImplGlfw_InitForOpenGL(Gui::glfw::window, true);
            ImGui_ImplOpenGL3_Init();

            // windowFlags |= ImGuiWindowFlags_NoMove;
            // windowFlags |= ImGuiWindowFlags_NoResize;

            windowFlagsMainWindow |= ImGuiWindowFlags_NoMove;
            windowFlagsMainWindow |= ImGuiWindowFlags_NoCollapse;
            windowFlagsMainWindow |= ImGuiWindowFlags_NoTitleBar;
            // windowFlagsMainWindow |= ImGuiWindowFlags_NoResize;
        }

        void createDockSpace() {
            // Setup viewport
            const ImGuiViewport *mainViewport = ImGui::GetMainViewport();

            // ImGui Dockspace
            ImGuiID dockSpaceID;
            {
                dockSpaceID = ImGui::GetID("MainDockSpace");
                ImGui::SetNextWindowSize(
                    ImVec2(Gui::glfw::mode->width, Gui::glfw::mode->height));
                ImGui::SetNextWindowPos(ImVec2(0, 0));
                ImGui::Begin("raytracer Main Window", NULL,
                             windowFlagsMainWindow);

                // Create the actual Dockspace
                ImGui::DockSpace(dockSpaceID, ImVec2(0, 0),
                                 ImGuiDockNodeFlags_None);

                ImGui::End();
            }

            // Docking Space
            static bool hasExecuted = false;
            if (!hasExecuted) {
                hasExecuted = true;

                // Removing existing Layout
                ImGui::DockBuilderRemoveNode(dockSpaceID);
                ImGui::DockBuilderAddNode(dockSpaceID,
                                          ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockSpaceID,
                                              ImGui::GetMainViewport()->Size);

                // Left Part and remainder
                ImGuiID dockIDLeft;
                ImGuiID dockIDMain;
                ImGui::DockBuilderSplitNode(dockSpaceID, ImGuiDir_Left, 0.2f,
                                            &dockIDLeft, &dockIDMain);

                // Right and Center
                ImGuiID dockIDRight;
                ImGuiID dockIDCenter;
                ImGui::DockBuilderSplitNode(dockIDMain, ImGuiDir_Right, 0.3f,
                                            &dockIDRight, &dockIDCenter);

                // Top and Bottom
                ImGuiID dockIDTop;
                ImGuiID dockIDBottom;
                ImGui::DockBuilderSplitNode(dockIDCenter, ImGuiDir_Up, 0.6f,
                                            &dockIDTop, &dockIDBottom);

                // Assign the Docks to Windows
                ImGui::DockBuilderDockWindow("Scene", dockIDLeft);
                ImGui::DockBuilderDockWindow("Dear ImGui Demo", dockIDTop);
                ImGui::DockBuilderDockWindow("Output", dockIDTop);
                ImGui::DockBuilderDockWindow("Debug", dockIDBottom);
                ImGui::DockBuilderDockWindow("Render", dockIDRight);

                ImGui::DockBuilderFinish(dockSpaceID);
            }
        }

        void defineGui() {
            {
                static bool isRendering = false;
                static std::future<void> renderFuture;

                ImGui::Begin("Render", NULL, windowFlags);

                // Rendering Button
                if (ImGui::Button("render")) {
                    if (!isRendering) {
                        isRendering = true;
                        renderFuture = std::async(std::launch::async, [] {
                            // S_perlinSpheres().render();
                            renderScene->render();
                        });
                    }
                }

                // While rendering show Text
                if (isRendering) {
                    ImGui::SameLine();
                    ImGui::Text("Rendering ...");
                }

                // if finished rendering show image
                if (isRendering && renderFuture.valid() &&
                    renderFuture.wait_for(std::chrono::milliseconds(0)) ==
                        std::future_status::ready) {
                    renderFuture.get();

                    glBindTexture(GL_TEXTURE_2D, Gui::glfw::renderedImage);
                    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                                    Gui::glfw::IMAGE_WIDTH,
                                    Gui::glfw::IMAGE_HEIGHT, GL_RGB,
                                    GL_UNSIGNED_BYTE, Gui::glfw::pixels.data());

                    // End of rendering
                    isRendering = false;
                }

                // Bool if stuff is allowed to change
                static bool isEditable = true;
                static bool RenderAfterChange = false;
                // ImGui::Checkbox("Edit", &isEditable);
                if (isEditable) {
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x *
                                         0.5f);
                    // ImGui::Checkbox("Render after Change",
                    // &RenderAfterChange); ImGui::DragFloat("Vertical Fov",
                    // &renderScene->cam.vfov);
                    float vfov_f = static_cast<float>(renderScene->cam.vfov);
                    if (ImGui::DragFloat("Vertical Fov", &vfov_f, 0.1f, 1.0f,
                                         180.0f)) {
                        renderScene->cam.vfov = static_cast<double>(vfov_f);
                    }
                    ImGui::DragInt("Rays per Pixel",
                                   &renderScene->cam.SamplesPerPixel, 1, 1);
                    ImGui::DragInt("Amount of Light Bounces",
                                   &renderScene->cam.maxDepth, 1, 1);
                    ImGui::PopItemWidth();
                }
                ImGui::End();
            }

            // RenderView
            {
                ImGui::Begin("Output", NULL, windowFlags);
                int WindowWidth = ImGui::GetWindowSize().x;
                ImGui::Image(
                    (void *)(intptr_t)Gui::glfw::renderedImage,
                    ImVec2(WindowWidth, (WindowWidth) * (1 / (16.0f / 9.0f))),
                    ImVec2(0, 1), ImVec2(1, 0));
                ImGui::End();
            }

            // Debug Window/Terminal
            {
                ImGui::Begin("Debug", NULL, windowFlags);
                ImGui::Text("Debug Prints:");
                ImGui::Text("Monitor Width: %d", Gui::glfw::mode->width);
                ImGui::Text("Monitor Height: %d", Gui::glfw::mode->height);
                if (Gui::renderScene) {
                    ImGui::Text("renderScene exists");
                } else {
                    ImGui::Text("renderScene doesn't exist");
                }

                // static std::string DebugText;
                ImGui::Text("Rendering: %s", ScanlineLog.LastLog().c_str());
                ImGui::End();
            }

            // Placeholder
            {
                ImGui::Begin("Scene");
                ImGui::Text("Placeholder");
                ImGui::End();
            }
        }

        void terminate() {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }
    } // namespace Im

    void setup() {
        Gui::glfw::setup();
        Gui::Im::setup();
    }

    void update() {
        // Poll for and process events
        glfwPollEvents();

        // Start the ImGUi Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Actual Gui Here
        Im::createDockSpace();
        Im::defineGui();

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(Gui::glfw::window);
    }

    void terminate() {
        Gui::Im::terminate();
        Gui::glfw::terminate();
    }
} // namespace Gui
