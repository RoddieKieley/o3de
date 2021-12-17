//
// Created by rkieley on 2021-11-25.
//

#ifndef GLFW_INTERNAL_JOYSTICK_H
#define GLFW_INTERNAL_JOYSTICK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "glfw3joystick.h"
#include "glfw3common.h"
#include "linux_joystick.h"
#include "posix_time.h"
#include <stdint.h>
#include <stdlib.h>

#define _GLFW_POLL_PRESENCE     0
#define _GLFW_POLL_AXES         1
#define _GLFW_POLL_BUTTONS      2
#define _GLFW_POLL_ALL          (_GLFW_POLL_AXES | _GLFW_POLL_BUTTONS)

//typedef struct _GLFWlibraryjoystick     _GLFWlibraryjoystick;
typedef struct _GLFWinitconfigjoystick  _GLFWinitconfigjoystick;

typedef struct _GLFWmapelement  _GLFWmapelement;
typedef struct _GLFWmapping     _GLFWmapping;
typedef struct _GLFWjoystick    _GLFWjoystick;


//// Gamepad mapping element structure
////
//struct _GLFWmapelement
//{
//    uint8_t         type;
//    uint8_t         index;
//    int8_t          axisScale;
//    int8_t          axisOffset;
//};
//
//// Gamepad mapping structure
////
//struct _GLFWmapping
//{
//    char            name[128];
//    char            guid[33];
//    _GLFWmapelement buttons[15];
//    _GLFWmapelement axes[6];
//};
//
// Joystick structure
//
//struct _GLFWjoystick
//{
//    GLFWbool        present;
//    float*          axes;
//    int             axisCount;
//    unsigned char*  buttons;
//    int             buttonCount;
//    unsigned char*  hats;
//    int             hatCount;
//    char            name[128];
//    void*           userPointer;
//    char            guid[33];
//    _GLFWmapping*   mapping;
//
//    // This is defined in the joystick API's joystick.h
//    _GLFW_PLATFORM_JOYSTICK_STATE;
//};

//// Initialization configuration
////
//// Parameters relating to the initialization of the library
////
//struct _GLFWinitconfigjoystick
//{
//    GLFWbool      hatButtons;
//    //    struct {
//    //        GLFWbool  menubar;
//    //        GLFWbool  chdir;
//    //    } ns;
//};

//struct _GLFWlibraryjoystick
//typedef struct _GLFWlibraryjoystick
//{
//    GLFWbool            initialized;
//
//    struct {
//        _GLFWinitconfigjoystick init;
//    } hints;
//
//    _GLFWjoystick       joysticks[GLFW_JOYSTICK_LAST + 1];
//    _GLFWmapping*       mappings;
//    int                 mappingCount;
//
//    struct {
//        uint64_t        offset;
//        // This is defined in the platform's time.h
//        _GLFW_PLATFORM_LIBRARY_TIMER_STATE;
////        _GLFWtimerPOSIX posix;
//    } timer;
//
//    struct {
////        GLFWmonitorfun  monitor;
//        GLFWjoystickfun joystick;
//    } callbacks;
//
//    // This is defined in the platform's joystick.h
//    _GLFW_PLATFORM_LIBRARY_JOYSTICK_STATE;
//} _GLFWlibraryjoystick;

// Global state shared between compilation units of GLFW
//
//extern _GLFWlibraryjoystick _glfw_joystick;
//_GLFWlibraryjoystick _glfw_joystick = { GLFW_FALSE };
// Checks for whether the library has been initialized
#define _GLFW_REQUIRE_INIT_JOYSTICK()                         \
if (!_glfw_joystick.initialized)                          \
{                                                \
_glfwInputError(GLFW_NOT_INITIALIZED, NULL); \
return;                                      \
}

#define _GLFW_REQUIRE_INIT_JOYSTICK_OR_RETURN(x)              \
if (!_glfw_joystick.initialized)                          \
{                                                \
_glfwInputError(GLFW_NOT_INITIALIZED, NULL); \
return x;                                    \
}

int _glfwPlatformPollJoystick(_GLFWjoystick* js, int mode);
void _glfwPlatformUpdateGamepadGUID(char* guid);

void _glfwInputJoystick(_GLFWjoystick* js, int event);
void _glfwInputJoystickAxis(_GLFWjoystick* js, int axis, float value);
void _glfwInputJoystickButton(_GLFWjoystick* js, int button, char value);
void _glfwInputJoystickHat(_GLFWjoystick* js, int hat, char value);

void _glfwInitGamepadMappings(void);
_GLFWjoystick* _glfwAllocJoystick(const char* name,
                                  const char* guid,
                                  int axisCount,
                                  int buttonCount,
                                  int hatCount);
void _glfwFreeJoystick(_GLFWjoystick* js);

//static void terminate_joystick(void)
//{
//    free(_glfw_joystick.mappings);
//    _glfw_joystick.mappings = NULL;
//    _glfw_joystick.mappingCount = 0;
//}


#ifdef __cplusplus
}
#endif

#endif //GLFW_INTERNAL_JOYSTICK_H