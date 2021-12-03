//
// Created by rkieley on 2021-11-25.
//

#ifndef GLFW_GLFW3JOYSTICK_H
#define GLFW_GLFW3JOYSTICK_H

#include "glfw3common.h"


/*! @defgroup joysticks Joysticks
 *  @brief Joystick IDs.
 *
 *  See [joystick input](@ref joystick) for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define GLFW_JOYSTICK_1             0
#define GLFW_JOYSTICK_2             1
#define GLFW_JOYSTICK_3             2
#define GLFW_JOYSTICK_4             3
#define GLFW_JOYSTICK_5             4
#define GLFW_JOYSTICK_6             5
#define GLFW_JOYSTICK_7             6
#define GLFW_JOYSTICK_8             7
#define GLFW_JOYSTICK_9             8
#define GLFW_JOYSTICK_10            9
#define GLFW_JOYSTICK_11            10
#define GLFW_JOYSTICK_12            11
#define GLFW_JOYSTICK_13            12
#define GLFW_JOYSTICK_14            13
#define GLFW_JOYSTICK_15            14
#define GLFW_JOYSTICK_16            15
#define GLFW_JOYSTICK_LAST          GLFW_JOYSTICK_16

/*! @defgroup gamepad_buttons Gamepad buttons
 *  @brief Gamepad buttons.
 *
 *  See @ref gamepad for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define GLFW_GAMEPAD_BUTTON_A               0
#define GLFW_GAMEPAD_BUTTON_B               1
#define GLFW_GAMEPAD_BUTTON_X               2
#define GLFW_GAMEPAD_BUTTON_Y               3
#define GLFW_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define GLFW_GAMEPAD_BUTTON_BACK            6
#define GLFW_GAMEPAD_BUTTON_START           7
#define GLFW_GAMEPAD_BUTTON_GUIDE           8
#define GLFW_GAMEPAD_BUTTON_LEFT_THUMB      9
#define GLFW_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define GLFW_GAMEPAD_BUTTON_DPAD_UP         11
#define GLFW_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define GLFW_GAMEPAD_BUTTON_DPAD_DOWN       13
#define GLFW_GAMEPAD_BUTTON_DPAD_LEFT       14
#define GLFW_GAMEPAD_BUTTON_LAST            GLFW_GAMEPAD_BUTTON_DPAD_LEFT

#define GLFW_GAMEPAD_BUTTON_CROSS       GLFW_GAMEPAD_BUTTON_A
#define GLFW_GAMEPAD_BUTTON_CIRCLE      GLFW_GAMEPAD_BUTTON_B
#define GLFW_GAMEPAD_BUTTON_SQUARE      GLFW_GAMEPAD_BUTTON_X
#define GLFW_GAMEPAD_BUTTON_TRIANGLE    GLFW_GAMEPAD_BUTTON_Y
/*! @} */

/*! @defgroup gamepad_axes Gamepad axes
 *  @brief Gamepad axes.
 *
 *  See @ref gamepad for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define GLFW_GAMEPAD_AXIS_LEFT_X        0
#define GLFW_GAMEPAD_AXIS_LEFT_Y        1
#define GLFW_GAMEPAD_AXIS_RIGHT_X       2
#define GLFW_GAMEPAD_AXIS_RIGHT_Y       3
#define GLFW_GAMEPAD_AXIS_LEFT_TRIGGER  4
#define GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define GLFW_GAMEPAD_AXIS_LAST          GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER
/*! @} */

/*! @addtogroup init
 *  @{ */
/*! @brief Joystick hat buttons init hint.
 *
 *  Joystick hat buttons [init hint](@ref GLFW_JOYSTICK_HAT_BUTTONS).
 */
#define GLFW_JOYSTICK_HAT_BUTTONS   0x00050001

/*! @defgroup hat_state Joystick hat states
 *  @brief Joystick hat states.
 *
 *  See [joystick hat input](@ref joystick_hat) for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define GLFW_HAT_CENTERED           0
#define GLFW_HAT_UP                 1
#define GLFW_HAT_RIGHT              2
#define GLFW_HAT_DOWN               4
#define GLFW_HAT_LEFT               8
#define GLFW_HAT_RIGHT_UP           (GLFW_HAT_RIGHT | GLFW_HAT_UP)
#define GLFW_HAT_RIGHT_DOWN         (GLFW_HAT_RIGHT | GLFW_HAT_DOWN)
#define GLFW_HAT_LEFT_UP            (GLFW_HAT_LEFT  | GLFW_HAT_UP)
#define GLFW_HAT_LEFT_DOWN          (GLFW_HAT_LEFT  | GLFW_HAT_DOWN)
/*! @} */


/*! @} */

/*! @brief The function pointer type for joystick configuration callbacks.
 *
 *  This is the function pointer type for joystick configuration callbacks.
 *  A joystick configuration callback function has the following signature:
 *  @code
 *  void function_name(int jid, int event)
 *  @endcode
 *
 *  @param[in] jid The joystick that was connected or disconnected.
 *  @param[in] event One of `GLFW_CONNECTED` or `GLFW_DISCONNECTED`.  Future
 *  releases may add more events.
 *
 *  @sa @ref joystick_event
 *  @sa @ref glfwSetJoystickCallback
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup input
 */
typedef void (* GLFWjoystickfun)(int jid, int event);

/*! @brief Gamepad input state
 *
 *  This describes the input state of a gamepad.
 *
 *  @sa @ref gamepad
 *  @sa @ref glfwGetGamepadState
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
typedef struct GLFWgamepadstate
{
    /*! The states of each [gamepad button](@ref gamepad_buttons), `GLFW_PRESS`
     *  or `GLFW_RELEASE`.
     */
    unsigned char buttons[15];
    /*! The states of each [gamepad axis](@ref gamepad_axes), in the range -1.0
     *  to 1.0 inclusive.
     */
    float axes[6];
} GLFWgamepadstate;

/*! @brief Returns whether the specified joystick is present.
 *
 *  This function returns whether the specified joystick is present.
 *
 *  There is no need to call this function before other functions that accept
 *  a joystick ID, as they all check for presence before performing any other
 *  work.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @return `GLFW_TRUE` if the joystick is present, or `GLFW_FALSE` otherwise.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref joystick
 *
 *  @since Added in version 3.0.  Replaces `glfwGetJoystickParam`.
 *
 *  @ingroup input
 */
GLFWAPI int glfwJoystickPresent(int jid);

/*! @brief Returns the values of all axes of the specified joystick.
 *
 *  This function returns the values of all axes of the specified joystick.
 *  Each element in the array is a value between -1.0 and 1.0.
 *
 *  If the specified joystick is not present this function will return `NULL`
 *  but will not generate an error.  This can be used instead of first calling
 *  @ref glfwJoystickPresent.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @param[out] count Where to store the number of axis values in the returned
 *  array.  This is set to zero if the joystick is not present or an error
 *  occurred.
 *  @return An array of axis values, or `NULL` if the joystick is not present or
 *  an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned array is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified joystick is
 *  disconnected or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref joystick_axis
 *
 *  @since Added in version 3.0.  Replaces `glfwGetJoystickPos`.
 *
 *  @ingroup input
 */
GLFWAPI const float* glfwGetJoystickAxes(int jid, int* count);

/*! @brief Returns the state of all buttons of the specified joystick.
 *
 *  This function returns the state of all buttons of the specified joystick.
 *  Each element in the array is either `GLFW_PRESS` or `GLFW_RELEASE`.
 *
 *  For backward compatibility with earlier versions that did not have @ref
 *  glfwGetJoystickHats, the button array also includes all hats, each
 *  represented as four buttons.  The hats are in the same order as returned by
 *  __glfwGetJoystickHats__ and are in the order _up_, _right_, _down_ and
 *  _left_.  To disable these extra buttons, set the @ref
 *  GLFW_JOYSTICK_HAT_BUTTONS init hint before initialization.
 *
 *  If the specified joystick is not present this function will return `NULL`
 *  but will not generate an error.  This can be used instead of first calling
 *  @ref glfwJoystickPresent.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @param[out] count Where to store the number of button states in the returned
 *  array.  This is set to zero if the joystick is not present or an error
 *  occurred.
 *  @return An array of button states, or `NULL` if the joystick is not present
 *  or an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned array is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified joystick is
 *  disconnected or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref joystick_button
 *
 *  @since Added in version 2.2.
 *  @glfw3 Changed to return a dynamic array.
 *
 *  @ingroup input
 */
GLFWAPI const unsigned char* glfwGetJoystickButtons(int jid, int* count);

/*! @brief Returns the state of all hats of the specified joystick.
 *
 *  This function returns the state of all hats of the specified joystick.
 *  Each element in the array is one of the following values:
 *
 *  Name                  | Value
 *  ----                  | -----
 *  `GLFW_HAT_CENTERED`   | 0
 *  `GLFW_HAT_UP`         | 1
 *  `GLFW_HAT_RIGHT`      | 2
 *  `GLFW_HAT_DOWN`       | 4
 *  `GLFW_HAT_LEFT`       | 8
 *  `GLFW_HAT_RIGHT_UP`   | `GLFW_HAT_RIGHT` \| `GLFW_HAT_UP`
 *  `GLFW_HAT_RIGHT_DOWN` | `GLFW_HAT_RIGHT` \| `GLFW_HAT_DOWN`
 *  `GLFW_HAT_LEFT_UP`    | `GLFW_HAT_LEFT` \| `GLFW_HAT_UP`
 *  `GLFW_HAT_LEFT_DOWN`  | `GLFW_HAT_LEFT` \| `GLFW_HAT_DOWN`
 *
 *  The diagonal directions are bitwise combinations of the primary (up, right,
 *  down and left) directions and you can test for these individually by ANDing
 *  it with the corresponding direction.
 *
 *  @code
 *  if (hats[2] & GLFW_HAT_RIGHT)
 *  {
 *      // State of hat 2 could be right-up, right or right-down
 *  }
 *  @endcode
 *
 *  If the specified joystick is not present this function will return `NULL`
 *  but will not generate an error.  This can be used instead of first calling
 *  @ref glfwJoystickPresent.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @param[out] count Where to store the number of hat states in the returned
 *  array.  This is set to zero if the joystick is not present or an error
 *  occurred.
 *  @return An array of hat states, or `NULL` if the joystick is not present
 *  or an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned array is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified joystick is
 *  disconnected, this function is called again for that joystick or the library
 *  is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref joystick_hat
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI const unsigned char* glfwGetJoystickHats(int jid, int* count);

/*! @brief Returns the name of the specified joystick.
 *
 *  This function returns the name, encoded as UTF-8, of the specified joystick.
 *  The returned string is allocated and freed by GLFW.  You should not free it
 *  yourself.
 *
 *  If the specified joystick is not present this function will return `NULL`
 *  but will not generate an error.  This can be used instead of first calling
 *  @ref glfwJoystickPresent.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @return The UTF-8 encoded name of the joystick, or `NULL` if the joystick
 *  is not present or an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned string is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified joystick is
 *  disconnected or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref joystick_name
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup input
 */
GLFWAPI const char* glfwGetJoystickName(int jid);

/*! @brief Returns the SDL compatible GUID of the specified joystick.
 *
 *  This function returns the SDL compatible GUID, as a UTF-8 encoded
 *  hexadecimal string, of the specified joystick.  The returned string is
 *  allocated and freed by GLFW.  You should not free it yourself.
 *
 *  The GUID is what connects a joystick to a gamepad mapping.  A connected
 *  joystick will always have a GUID even if there is no gamepad mapping
 *  assigned to it.
 *
 *  If the specified joystick is not present this function will return `NULL`
 *  but will not generate an error.  This can be used instead of first calling
 *  @ref glfwJoystickPresent.
 *
 *  The GUID uses the format introduced in SDL 2.0.5.  This GUID tries to
 *  uniquely identify the make and model of a joystick but does not identify
 *  a specific unit, e.g. all wired Xbox 360 controllers will have the same
 *  GUID on that platform.  The GUID for a unit may vary between platforms
 *  depending on what hardware information the platform specific APIs provide.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @return The UTF-8 encoded GUID of the joystick, or `NULL` if the joystick
 *  is not present or an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned string is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified joystick is
 *  disconnected or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref gamepad
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI const char* glfwGetJoystickGUID(int jid);

/*! @brief Sets the user pointer of the specified joystick.
 *
 *  This function sets the user-defined pointer of the specified joystick.  The
 *  current value is retained until the joystick is disconnected.  The initial
 *  value is `NULL`.
 *
 *  This function may be called from the joystick callback, even for a joystick
 *  that is being disconnected.
 *
 *  @param[in] jid The joystick whose pointer to set.
 *  @param[in] pointer The new value.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @sa @ref joystick_userptr
 *  @sa @ref glfwGetJoystickUserPointer
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI void glfwSetJoystickUserPointer(int jid, void* pointer);

/*! @brief Returns the user pointer of the specified joystick.
 *
 *  This function returns the current value of the user-defined pointer of the
 *  specified joystick.  The initial value is `NULL`.
 *
 *  This function may be called from the joystick callback, even for a joystick
 *  that is being disconnected.
 *
 *  @param[in] jid The joystick whose pointer to return.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @sa @ref joystick_userptr
 *  @sa @ref glfwSetJoystickUserPointer
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI void* glfwGetJoystickUserPointer(int jid);

/*! @brief Returns whether the specified joystick has a gamepad mapping.
 *
 *  This function returns whether the specified joystick is both present and has
 *  a gamepad mapping.
 *
 *  If the specified joystick is present but does not have a gamepad mapping
 *  this function will return `GLFW_FALSE` but will not generate an error.  Call
 *  @ref glfwJoystickPresent to check if a joystick is present regardless of
 *  whether it has a mapping.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @return `GLFW_TRUE` if a joystick is both present and has a gamepad mapping,
 *  or `GLFW_FALSE` otherwise.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_ENUM.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref gamepad
 *  @sa @ref glfwGetGamepadState
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI int glfwJoystickIsGamepad(int jid);

/*! @brief Sets the joystick configuration callback.
 *
 *  This function sets the joystick configuration callback, or removes the
 *  currently set callback.  This is called when a joystick is connected to or
 *  disconnected from the system.
 *
 *  For joystick connection and disconnection events to be delivered on all
 *  platforms, you need to call one of the [event processing](@ref events)
 *  functions.  Joystick disconnection may also be detected and the callback
 *  called by joystick functions.  The function will then return whatever it
 *  returns if the joystick is not present.
 *
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(int jid, int event)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWjoystickfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref joystick_event
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup input
 */
GLFWAPI GLFWjoystickfun glfwSetJoystickCallback(GLFWjoystickfun callback);

/*! @brief Adds the specified SDL_GameControllerDB gamepad mappings.
 *
 *  This function parses the specified ASCII encoded string and updates the
 *  internal list with any gamepad mappings it finds.  This string may
 *  contain either a single gamepad mapping or many mappings separated by
 *  newlines.  The parser supports the full format of the `gamecontrollerdb.txt`
 *  source file including empty lines and comments.
 *
 *  See @ref gamepad_mapping for a description of the format.
 *
 *  If there is already a gamepad mapping for a given GUID in the internal list,
 *  it will be replaced by the one passed to this function.  If the library is
 *  terminated and re-initialized the internal list will revert to the built-in
 *  default.
 *
 *  @param[in] string The string containing the gamepad mappings.
 *  @return `GLFW_TRUE` if successful, or `GLFW_FALSE` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_VALUE.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref gamepad
 *  @sa @ref glfwJoystickIsGamepad
 *  @sa @ref glfwGetGamepadName
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI int glfwUpdateGamepadMappings(const char* string);

/*! @brief Returns the human-readable gamepad name for the specified joystick.
 *
 *  This function returns the human-readable name of the gamepad from the
 *  gamepad mapping assigned to the specified joystick.
 *
 *  If the specified joystick is not present or does not have a gamepad mapping
 *  this function will return `NULL` but will not generate an error.  Call
 *  @ref glfwJoystickPresent to check whether it is present regardless of
 *  whether it has a mapping.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @return The UTF-8 encoded name of the gamepad, or `NULL` if the
 *  joystick is not present, does not have a mapping or an
 *  [error](@ref error_handling) occurred.
 *
 *  @pointer_lifetime The returned string is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified joystick is
 *  disconnected, the gamepad mappings are updated or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref gamepad
 *  @sa @ref glfwJoystickIsGamepad
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI const char* glfwGetGamepadName(int jid);

/*! @brief Retrieves the state of the specified joystick remapped as a gamepad.
 *
 *  This function retrieves the state of the specified joystick remapped to
 *  an Xbox-like gamepad.
 *
 *  If the specified joystick is not present or does not have a gamepad mapping
 *  this function will return `GLFW_FALSE` but will not generate an error.  Call
 *  @ref glfwJoystickPresent to check whether it is present regardless of
 *  whether it has a mapping.
 *
 *  The Guide button may not be available for input as it is often hooked by the
 *  system or the Steam client.
 *
 *  Not all devices have all the buttons or axes provided by @ref
 *  GLFWgamepadstate.  Unavailable buttons and axes will always report
 *  `GLFW_RELEASE` and 0.0 respectively.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @param[out] state The gamepad input state of the joystick.
 *  @return `GLFW_TRUE` if successful, or `GLFW_FALSE` if no joystick is
 *  connected, it has no gamepad mapping or an [error](@ref error_handling)
 *  occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_ENUM.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref gamepad
 *  @sa @ref glfwUpdateGamepadMappings
 *  @sa @ref glfwJoystickIsGamepad
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI int glfwGetGamepadState(int jid, GLFWgamepadstate* state);

#endif //GLFW_GLFW3JOYSTICK_H
