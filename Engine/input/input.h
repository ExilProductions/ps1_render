#include <renderer.h>
#include <math.h>
#include <array>
#include <GLFW/glfw3.h>

enum class InputEvent
{
    KEY_PRESS,
    KEY_RELEASE,
    MOUSE_MOVE,
    MOUSE_BUTTON_PRESS,
    MOUSE_BUTTON_RELEASE,
};

enum class InputKey
{
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_ENTER,
    KEY_BACKSPACE,
    KEY_TAB,
    KEY_SHIFT,
    KEY_CONTROL,
    KEY_ALT,
    KEY_CAPS_LOCK,
    KEY_SPACE,
    KEY_ESCAPE,
    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_MIDDLE,
    MOUSE_BUTTON_4,
    MOUSE_BUTTON_5,
    MOUSE_BUTTON_6,
    MOUSE_BUTTON_7,
    MOUSE_BUTTON_8,
};

class InputManager
{
private:
    GLFWwindow* window;
    Vec2 scroll_delta;
    Vec2 last_mouse_position;
    std::array<bool, 400> previous_key_states;
    std::array<bool, 8> previous_mouse_button_states;

    int get_key_code(InputKey key);
    int get_mouse_button_code(InputKey key);

public:
    InputManager(GLFWwindow* win);
    bool is_pressed(InputKey key);
    bool was_pressed(InputKey key);
    bool was_released(InputKey key);
    Vec2 get_mouse_position();
    Vec2 get_mouse_delta();
    Vec2 get_scroll_delta();
    void update();
};