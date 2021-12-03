//
// Created by rkieley on 2021-11-25.
//

#ifdef __cplusplus
extern "C" {
#endif

#include "internal_joystick.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

    // Global state shared between compilation units of GLFW
    //
    _GLFWlibraryjoystick _glfw_joystick = { GLFW_FALSE };

    static _GLFWinitconfigjoystick _glfwInitHintsJoystick =
            {
            GLFW_TRUE      // hat buttons
            };

    //static void terminate_joystick(void)
    //{
    //    free(_glfw_joystick.mappings);
    //    _glfw_joystick.mappings = NULL;
    //    _glfw_joystick.mappingCount = 0;
    //}

    //////////////////////////////////////////////////////////////////////////
    //////                        GLFW public API                       //////
    //////////////////////////////////////////////////////////////////////////

//    GLFWAPI int glfwInitJoystick(void)
    int glfwInitJoystick(void)
    {
        if (_glfw_joystick.initialized)
            return GLFW_TRUE;

        memset(&_glfw_joystick, 0, sizeof(_glfw_joystick));
        _glfw_joystick.hints.init = _glfwInitHintsJoystick;

        _glfwInitGamepadMappings();

        _glfw_joystick.initialized = GLFW_TRUE;

        return GLFW_TRUE;
    }

    //////////////////////////////////////////////////////////////////////////
    //////                         GLFW event API                       //////
    //////////////////////////////////////////////////////////////////////////

    // Notifies shared code of an error
    //
    void _glfwInputError(int code, const char* format, ...)
    {
        // TODO: RNK - 12/03/21 - do something useful here
        return;

//        _GLFWerror* error;
//        char description[_GLFW_MESSAGE_SIZE];
//
//        if (format)
//        {
//            va_list vl;
//
//            va_start(vl, format);
//            vsnprintf(description, sizeof(description), format, vl);
//            va_end(vl);
//
//            description[sizeof(description) - 1] = '\0';
//        }
//        else
//        {
//            if (code == GLFW_NOT_INITIALIZED)
//                strcpy(description, "The GLFW library is not initialized");
//            else if (code == GLFW_NO_CURRENT_CONTEXT)
//                strcpy(description, "There is no current context");
//            else if (code == GLFW_INVALID_ENUM)
//                strcpy(description, "Invalid argument for enum parameter");
//            else if (code == GLFW_INVALID_VALUE)
//                strcpy(description, "Invalid value for parameter");
//            else if (code == GLFW_OUT_OF_MEMORY)
//                strcpy(description, "Out of memory");
//            else if (code == GLFW_API_UNAVAILABLE)
//                strcpy(description, "The requested API is unavailable");
//            else if (code == GLFW_VERSION_UNAVAILABLE)
//                strcpy(description, "The requested API version is unavailable");
//            else if (code == GLFW_PLATFORM_ERROR)
//                strcpy(description, "A platform-specific error occurred");
//            else if (code == GLFW_FORMAT_UNAVAILABLE)
//                strcpy(description, "The requested format is unavailable");
//            else if (code == GLFW_NO_WINDOW_CONTEXT)
//                strcpy(description, "The specified window has no context");
//            else
//                strcpy(description, "ERROR: UNKNOWN GLFW ERROR");
//        }
//
//        if (_glfw.initialized)
//        {
//            error = _glfwPlatformGetTls(&_glfw.errorSlot);
//            if (!error)
//            {
//                error = calloc(1, sizeof(_GLFWerror));
//                _glfwPlatformSetTls(&_glfw.errorSlot, error);
//                _glfwPlatformLockMutex(&_glfw.errorLock);
//                error->next = _glfw.errorListHead;
//                _glfw.errorListHead = error;
//                _glfwPlatformUnlockMutex(&_glfw.errorLock);
//            }
//        }
//        else
//            error = &_glfwMainThreadError;
//
//        error->code = code;
//        strcpy(error->description, description);
//
//        if (_glfwErrorCallback)
//            _glfwErrorCallback(code, description);
    }

    float _glfw_fminf(float a, float b)
    {
        if (a != a)
            return b;
        else if (b != b)
            return a;
        else if (a < b)
            return a;
        else
            return b;
    }

    float _glfw_fmaxf(float a, float b)
    {
        if (a != a)
            return b;
        else if (b != b)
            return a;
        else if (a > b)
            return a;
        else
            return b;
    }

#ifdef __cplusplus
}
#endif
