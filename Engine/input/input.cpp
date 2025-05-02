#include "input.h"
#include <algorithm>

InputManager::InputManager(GLFWwindow* win) : window(win), scroll_delta(0.0f, 0.0f) {
    std::fill(previous_key_states.begin(), previous_key_states.end(), false);
    std::fill(previous_mouse_button_states.begin(), previous_mouse_button_states.end(), false);
    glfwSetWindowUserPointer(window, this);
    glfwSetScrollCallback(window, [](GLFWwindow* w, double xoffset, double yoffset) {
        InputManager* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(w));
        manager->scroll_delta.x += static_cast<float>(xoffset);
        manager->scroll_delta.y += static_cast<float>(yoffset);
    });
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    last_mouse_position = Vec2(static_cast<float>(x), static_cast<float>(y));
}

int InputManager::get_key_code(InputKey key) {
    switch (key) {
        case InputKey::KEY_A: return GLFW_KEY_A;
        case InputKey::KEY_B: return GLFW_KEY_B;
        case InputKey::KEY_C: return GLFW_KEY_C;
        case InputKey::KEY_D: return GLFW_KEY_D;
        case InputKey::KEY_E: return GLFW_KEY_E;
        case InputKey::KEY_F: return GLFW_KEY_F;
        case InputKey::KEY_G: return GLFW_KEY_G;
        case InputKey::KEY_H: return GLFW_KEY_H;
        case InputKey::KEY_I: return GLFW_KEY_I;
        case InputKey::KEY_J: return GLFW_KEY_J;
        case InputKey::KEY_K: return GLFW_KEY_K;
        case InputKey::KEY_L: return GLFW_KEY_L;
        case InputKey::KEY_M: return GLFW_KEY_M;
        case InputKey::KEY_N: return GLFW_KEY_N;
        case InputKey::KEY_O: return GLFW_KEY_O;
        case InputKey::KEY_P: return GLFW_KEY_P;
        case InputKey::KEY_Q: return GLFW_KEY_Q;
        case InputKey::KEY_R: return GLFW_KEY_R;
        case InputKey::KEY_S: return GLFW_KEY_S;
        case InputKey::KEY_T: return GLFW_KEY_T;
        case InputKey::KEY_U: return GLFW_KEY_U;
        case InputKey::KEY_V: return GLFW_KEY_V;
        case InputKey::KEY_W: return GLFW_KEY_W;
        case InputKey::KEY_X: return GLFW_KEY_X;
        case InputKey::KEY_Y: return GLFW_KEY_Y;
        case InputKey::KEY_Z: return GLFW_KEY_Z;
        case InputKey::KEY_0: return GLFW_KEY_0;
        case InputKey::KEY_1: return GLFW_KEY_1;
        case InputKey::KEY_2: return GLFW_KEY_2;
        case InputKey::KEY_3: return GLFW_KEY_3;
        case InputKey::KEY_4: return GLFW_KEY_4;
        case InputKey::KEY_5: return GLFW_KEY_5;
        case InputKey::KEY_6: return GLFW_KEY_6;
        case InputKey::KEY_7: return GLFW_KEY_7;
        case InputKey::KEY_8: return GLFW_KEY_8;
        case InputKey::KEY_9: return GLFW_KEY_9;
        case InputKey::KEY_UP: return GLFW_KEY_UP;
        case InputKey::KEY_DOWN: return GLFW_KEY_DOWN;
        case InputKey::KEY_LEFT: return GLFW_KEY_LEFT;
        case InputKey::KEY_RIGHT: return GLFW_KEY_RIGHT;
        case InputKey::KEY_ENTER: return GLFW_KEY_ENTER;
        case InputKey::KEY_BACKSPACE: return GLFW_KEY_BACKSPACE;
        case InputKey::KEY_TAB: return GLFW_KEY_TAB;
        case InputKey::KEY_SHIFT: return GLFW_KEY_LEFT_SHIFT;
        case InputKey::KEY_CONTROL: return GLFW_KEY_LEFT_CONTROL;
        case InputKey::KEY_ALT: return GLFW_KEY_LEFT_ALT;
        case InputKey::KEY_CAPS_LOCK: return GLFW_KEY_CAPS_LOCK;
        case InputKey::KEY_SPACE: return GLFW_KEY_SPACE;
        case InputKey::KEY_ESCAPE: return GLFW_KEY_ESCAPE;
        default: return -1;
    }
}

int InputManager::get_mouse_button_code(InputKey key) {
    switch (key) {
        case InputKey::MOUSE_LEFT: return GLFW_MOUSE_BUTTON_LEFT;
        case InputKey::MOUSE_RIGHT: return GLFW_MOUSE_BUTTON_RIGHT;
        case InputKey::MOUSE_MIDDLE: return GLFW_MOUSE_BUTTON_MIDDLE;
        case InputKey::MOUSE_BUTTON_4: return GLFW_MOUSE_BUTTON_4;
        case InputKey::MOUSE_BUTTON_5: return GLFW_MOUSE_BUTTON_5;
        case InputKey::MOUSE_BUTTON_6: return GLFW_MOUSE_BUTTON_6;
        case InputKey::MOUSE_BUTTON_7: return GLFW_MOUSE_BUTTON_7;
        case InputKey::MOUSE_BUTTON_8: return GLFW_MOUSE_BUTTON_8;
        default: return -1;
    }
}

bool InputManager::is_pressed(InputKey key) {
    int key_code = get_key_code(key);
    if (key_code != -1) {
        return glfwGetKey(window, key_code) == GLFW_PRESS;
    }
    int button = get_mouse_button_code(key);
    if (button != -1) {
        return glfwGetMouseButton(window, button) == GLFW_PRESS;
    }
    return false;
}

bool InputManager::was_pressed(InputKey key) {
    int key_code = get_key_code(key);
    if (key_code != -1) {
        bool current = glfwGetKey(window, key_code) == GLFW_PRESS;
        bool previous = previous_key_states[key_code];
        return current && !previous;
    }
    int button = get_mouse_button_code(key);
    if (button != -1) {
        bool current = glfwGetMouseButton(window, button) == GLFW_PRESS;
        bool previous = previous_mouse_button_states[button];
        return current && !previous;
    }
    return false;
}

bool InputManager::was_released(InputKey key) {
    int key_code = get_key_code(key);
    if (key_code != -1) {
        bool current = glfwGetKey(window, key_code) == GLFW_PRESS;
        bool previous = previous_key_states[key_code];
        return !current && previous;
    }
    int button = get_mouse_button_code(key);
    if (button != -1) {
        bool current = glfwGetMouseButton(window, button) == GLFW_PRESS;
        bool previous = previous_mouse_button_states[button];
        return !current && previous;
    }
    return false;
}

Vec2 InputManager::get_mouse_position() {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return Vec2(static_cast<float>(x), static_cast<float>(y));
}

Vec2 InputManager::get_mouse_delta() {
    Vec2 current = get_mouse_position();
    Vec2 delta = current - last_mouse_position;
    last_mouse_position = current;
    return delta;
}

Vec2 InputManager::get_scroll_delta() {
    Vec2 delta = scroll_delta;
    scroll_delta = Vec2(0.0f, 0.0f);
    return delta;
}

void InputManager::update() {
    for (int key = 0; key < 400; ++key) {
        previous_key_states[key] = (glfwGetKey(window, key) == GLFW_PRESS);
    }
    for (int button = 0; button < 8; ++button) {
        previous_mouse_button_states[button] = (glfwGetMouseButton(window, button) == GLFW_PRESS);
    }
}