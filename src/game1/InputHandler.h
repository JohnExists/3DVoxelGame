#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

#include"Settings.h"

#include<tuple>
#include<iostream>
#include<optional>

namespace callback
{
    void updateMouseClick(GLFWwindow *window, int button, int action, int mods);
    void updateCursorPos(GLFWwindow* window, double x, double y);

}

class InputHandler
{

private:

    static GLFWwindow* window;
    static glm::vec2 cursorPosition;
    static int buttonPressed;
    static game::MouseStatus_t status;
    static bool cursorEnabled;
public:
    static void launchInputHandler(GLFWwindow* window);
    static void update();

    static void setCursorPos(double x, double y);
    static std::optional<game::CursorLocation_t> getCursorPos();

    static void enableCursor();

    static void setMouseStatus(int button, int action);
    static game::MouseStatus_t getMouseStatus();

    static bool keyIsPressed(int key);
    static void reset();
private:
};


#endif // INPUTHANDLER_H
