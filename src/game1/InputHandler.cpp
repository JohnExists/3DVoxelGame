#include "InputHandler.h"

GLFWwindow* InputHandler::window = nullptr;
glm::vec2 InputHandler::cursorPosition = glm::vec2(0.0f, 0.0f);
game::MouseStatus_t InputHandler::status = game::MouseStatus_t(-1, -1);
bool InputHandler::cursorEnabled = false;


void InputHandler::launchInputHandler(GLFWwindow* window)
{
    InputHandler::window = window;
}

void InputHandler::update()
{
    glfwSetMouseButtonCallback(window, callback::updateMouseClick);
    glfwSetCursorPosCallback(window, callback::updateCursorPos);
}

void InputHandler::enableCursor()
{
    cursorEnabled = true;    
}

std::optional<game::CursorLocation_t> InputHandler::getCursorPos()
{
    if(!cursorEnabled) return std::nullopt;
    return std::make_optional(InputHandler::cursorPosition);    
}

void InputHandler::setCursorPos(double x, double y)
{
    InputHandler::cursorPosition = glm::vec2(x, y);    
}

game::MouseStatus_t InputHandler::getMouseStatus()
{
    return InputHandler::status;
}

void InputHandler::setMouseStatus(int button, int action)
{
    InputHandler::status = game::MouseStatus_t(button, action);
}

bool InputHandler::keyIsPressed(int key)
{
    return glfwGetKey(window, key) == GLFW_PRESS;
}

void callback::updateCursorPos(GLFWwindow* window, double x, double y)
{
    InputHandler::enableCursor();
    InputHandler::setCursorPos(static_cast<float>(x), static_cast<double>(y));
}

void callback::updateMouseClick(GLFWwindow *window, int button, int action, int mods)
{
    InputHandler::setMouseStatus(button, action);
}

void InputHandler::reset()
{
    InputHandler::status = game::MouseStatus_t(-1, -1);
}