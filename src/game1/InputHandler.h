#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

#include"Settings.h"

#include<tuple>
#include<iostream>
#include<optional>

// The functions called when the user interacts with
// mouse or keyboard
namespace callback
{
    void updateMouseClick(GLFWwindow *window, int button, int action, int mods);
    void updateCursorPos(GLFWwindow* window, double x, double y);

}

class InputHandler
{

private:

    // The window for the input handler
    static GLFWwindow* window;
    // The x, y location of the mouse pointer and cursor values
    static game::MouseStatus_t status;
    static glm::vec2 cursorPosition;
    static bool cursorEnabled;
    // The button pressed
    static int buttonPressed;
public:
    /**
     * Initializes the window for input usage
     * 
     * @param window The window where all the inputs are taking place
     */
    static void launchInputHandler(GLFWwindow* window);

    /**
     * Called every frame to check for mouse or keyboard
     * input from the user
     */
    static void update();

    /**
     * Move the position of the mouse pointer in the window
     * 
     * @param x The new x-position
     * @param y The new y-position
     */
    static void setCursorPos(double x, double y);
    /**
     * @returns The current x, y vector for the mouse pointers position
     */
    static std::optional<game::CursorLocation_t> getCursorPos();

    /**
     * Allows the mouse cursor to be visible
     */
    static void enableCursor();
    /**
     * Sets the current values of the mouse
     * 
     * @param button Which button status is being updated (LEFT, RIGHT, SCROLL, etc)
     * @param action What is the new status of that button (PRESSED, RELEASED, etc)
     */
    static void setMouseStatus(int button, int action);
    /**
     * @returns the current button pressed and the state of that button
     */
    static game::MouseStatus_t getMouseStatus();

    /**
     * Checks if a certain key is pressed
     * 
     * @param key The key that is being checked
     * @returns Whether the key has been pressed or not
     */
    static bool keyIsPressed(int key);

    /**
     * Resets all the values for the mouse in
     * the input handler
     */
    static void reset();
private:
};


#endif // INPUTHANDLER_H
