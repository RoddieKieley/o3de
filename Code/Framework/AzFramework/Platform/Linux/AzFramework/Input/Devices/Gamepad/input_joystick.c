//
// Created by rkieley on 2021-11-25.
//

#include "internal_joystick.h"
#include "mappings.h"

#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// Internal constants for gamepad mapping source types
#define _GLFW_JOYSTICK_AXIS     1
#define _GLFW_JOYSTICK_BUTTON   2
#define _GLFW_JOYSTICK_HATBIT   3

// Finds a mapping based on joystick GUID
//
static _GLFWmapping* findMapping(const char* guid)
{
    int i;

    for (i = 0;  i < _glfw_joystick.mappingCount;  i++)
    {
        if (strcmp(_glfw_joystick.mappings[i].guid, guid) == 0)
            return _glfw_joystick.mappings + i;
    }

    return NULL;
}

// Checks whether a gamepad mapping element is present in the hardware
//
static GLFWbool isValidElementForJoystick(const _GLFWmapelement* e,
                                          const _GLFWjoystick* js)
                                          {
    if (e->type == _GLFW_JOYSTICK_HATBIT && (e->index >> 4) >= js->hatCount)
        return GLFW_FALSE;
    else if (e->type == _GLFW_JOYSTICK_BUTTON && e->index >= js->buttonCount)
        return GLFW_FALSE;
    else if (e->type == _GLFW_JOYSTICK_AXIS && e->index >= js->axisCount)
        return GLFW_FALSE;

    return GLFW_TRUE;
  }

  // Finds a mapping based on joystick GUID and verifies element indices
  //
  static _GLFWmapping* findValidMapping(const _GLFWjoystick* js)
  {
    _GLFWmapping* mapping = findMapping(js->guid);
    if (mapping)
    {
        int i;

        for (i = 0;  i <= GLFW_GAMEPAD_BUTTON_LAST;  i++)
        {
            if (!isValidElementForJoystick(mapping->buttons + i, js))
                return NULL;
        }

        for (i = 0;  i <= GLFW_GAMEPAD_AXIS_LAST;  i++)
        {
            if (!isValidElementForJoystick(mapping->axes + i, js))
                return NULL;
        }
    }

    return mapping;
  }

  // Parses an SDL_GameControllerDB line and adds it to the mapping list
  //
  static GLFWbool parseMapping(_GLFWmapping* mapping, const char* string)
  {
    const char* c = string;
    size_t i, length;
    struct
    {
        const char* name;
        _GLFWmapelement* element;
    } fields[] =
            {
            { "platform",      NULL },
            { "a",             mapping->buttons + GLFW_GAMEPAD_BUTTON_A },
            { "b",             mapping->buttons + GLFW_GAMEPAD_BUTTON_B },
            { "x",             mapping->buttons + GLFW_GAMEPAD_BUTTON_X },
            { "y",             mapping->buttons + GLFW_GAMEPAD_BUTTON_Y },
            { "back",          mapping->buttons + GLFW_GAMEPAD_BUTTON_BACK },
            { "start",         mapping->buttons + GLFW_GAMEPAD_BUTTON_START },
            { "guide",         mapping->buttons + GLFW_GAMEPAD_BUTTON_GUIDE },
            { "leftshoulder",  mapping->buttons + GLFW_GAMEPAD_BUTTON_LEFT_BUMPER },
            { "rightshoulder", mapping->buttons + GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER },
            { "leftstick",     mapping->buttons + GLFW_GAMEPAD_BUTTON_LEFT_THUMB },
            { "rightstick",    mapping->buttons + GLFW_GAMEPAD_BUTTON_RIGHT_THUMB },
            { "dpup",          mapping->buttons + GLFW_GAMEPAD_BUTTON_DPAD_UP },
            { "dpright",       mapping->buttons + GLFW_GAMEPAD_BUTTON_DPAD_RIGHT },
            { "dpdown",        mapping->buttons + GLFW_GAMEPAD_BUTTON_DPAD_DOWN },
            { "dpleft",        mapping->buttons + GLFW_GAMEPAD_BUTTON_DPAD_LEFT },
            { "lefttrigger",   mapping->axes + GLFW_GAMEPAD_AXIS_LEFT_TRIGGER },
            { "righttrigger",  mapping->axes + GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER },
            { "leftx",         mapping->axes + GLFW_GAMEPAD_AXIS_LEFT_X },
            { "lefty",         mapping->axes + GLFW_GAMEPAD_AXIS_LEFT_Y },
            { "rightx",        mapping->axes + GLFW_GAMEPAD_AXIS_RIGHT_X },
            { "righty",        mapping->axes + GLFW_GAMEPAD_AXIS_RIGHT_Y }
            };

    length = strcspn(c, ",");
    if (length != 32 || c[length] != ',')
    {
        _glfwInputError(GLFW_INVALID_VALUE, NULL);
        return GLFW_FALSE;
    }

    memcpy(mapping->guid, c, length);
    c += length + 1;

    length = strcspn(c, ",");
    if (length >= sizeof(mapping->name) || c[length] != ',')
    {
        _glfwInputError(GLFW_INVALID_VALUE, NULL);
        return GLFW_FALSE;
    }

    memcpy(mapping->name, c, length);
    c += length + 1;

    while (*c)
    {
        // TODO: Implement output modifiers
        if (*c == '+' || *c == '-')
            return GLFW_FALSE;

        for (i = 0;  i < sizeof(fields) / sizeof(fields[0]);  i++)
        {
            length = strlen(fields[i].name);
            if (strncmp(c, fields[i].name, length) != 0 || c[length] != ':')
                continue;

            c += length + 1;

            if (fields[i].element)
            {
                _GLFWmapelement* e = fields[i].element;
                int8_t minimum = -1;
                int8_t maximum = 1;

                if (*c == '+')
                {
                    minimum = 0;
                    c += 1;
                }
                else if (*c == '-')
                {
                    maximum = 0;
                    c += 1;
                }

                if (*c == 'a')
                    e->type = _GLFW_JOYSTICK_AXIS;
                else if (*c == 'b')
                    e->type = _GLFW_JOYSTICK_BUTTON;
                else if (*c == 'h')
                    e->type = _GLFW_JOYSTICK_HATBIT;
                else
                    break;

                if (e->type == _GLFW_JOYSTICK_HATBIT)
                {
                    const unsigned long hat = strtoul(c + 1, (char**) &c, 10);
                    const unsigned long bit = strtoul(c + 1, (char**) &c, 10);
                    e->index = (uint8_t) ((hat << 4) | bit);
                }
                else
                    e->index = (uint8_t) strtoul(c + 1, (char**) &c, 10);

                if (e->type == _GLFW_JOYSTICK_AXIS)
                {
                    e->axisScale = 2 / (maximum - minimum);
                    e->axisOffset = -(maximum + minimum);

                    if (*c == '~')
                    {
                        e->axisScale = -e->axisScale;
                        e->axisOffset = -e->axisOffset;
                    }
                }
            }
            else
            {
                length = strlen(_GLFW_PLATFORM_MAPPING_NAME);
                if (strncmp(c, _GLFW_PLATFORM_MAPPING_NAME, length) != 0)
                    return GLFW_FALSE;
            }

            break;
        }

        c += strcspn(c, ",");
        c += strspn(c, ",");
    }

    for (i = 0;  i < 32;  i++)
    {
        if (mapping->guid[i] >= 'A' && mapping->guid[i] <= 'F')
            mapping->guid[i] += 'a' - 'A';
    }

    _glfwPlatformUpdateGamepadGUID(mapping->guid);
    return GLFW_TRUE;
}

// Notifies shared code of a joystick connection or disconnection
//
void _glfwInputJoystick(_GLFWjoystick* js, int event)
{
    const int jid = (int) (js - _glfw_joystick.joysticks);

    if (_glfw_joystick.callbacks.joystick)
        _glfw_joystick.callbacks.joystick(jid, event);
}

// Notifies shared code of the new value of a joystick axis
//
void _glfwInputJoystickAxis(_GLFWjoystick* js, int axis, float value)
{
    js->axes[axis] = value;
}

// Notifies shared code of the new value of a joystick button
//
void _glfwInputJoystickButton(_GLFWjoystick* js, int button, char value)
{
    js->buttons[button] = value;
}

// Notifies shared code of the new value of a joystick hat
//
void _glfwInputJoystickHat(_GLFWjoystick* js, int hat, char value)
{
    const int base = js->buttonCount + hat * 4;

    js->buttons[base + 0] = (value & 0x01) ? GLFW_PRESS : GLFW_RELEASE;
    js->buttons[base + 1] = (value & 0x02) ? GLFW_PRESS : GLFW_RELEASE;
    js->buttons[base + 2] = (value & 0x04) ? GLFW_PRESS : GLFW_RELEASE;
    js->buttons[base + 3] = (value & 0x08) ? GLFW_PRESS : GLFW_RELEASE;

    js->hats[hat] = value;
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

// Adds the built-in set of gamepad mappings
//
void _glfwInitGamepadMappings(void)
{
    size_t i;
    const size_t count = sizeof(_glfwDefaultMappings) / sizeof(char*);
    _glfw_joystick.mappings = calloc(count, sizeof(_GLFWmapping));

    for (i = 0;  i < count;  i++)
    {
        if (parseMapping(&_glfw_joystick.mappings[_glfw_joystick.mappingCount], _glfwDefaultMappings[i]))
            _glfw_joystick.mappingCount++;
    }
}

// Returns an available joystick object with arrays and name allocated
//
_GLFWjoystick* _glfwAllocJoystick(const char* name,
                                  const char* guid,
                                  int axisCount,
                                  int buttonCount,
                                  int hatCount)
{
    int jid;
    _GLFWjoystick* js;

    for (jid = 0;  jid <= GLFW_JOYSTICK_LAST;  jid++)
    {
        if (!_glfw_joystick.joysticks[jid].present)
            break;
    }

    if (jid > GLFW_JOYSTICK_LAST)
        return NULL;

    js = _glfw_joystick.joysticks + jid;
    js->present     = GLFW_TRUE;
    js->axes        = calloc(axisCount, sizeof(float));
    js->buttons     = calloc(buttonCount + (size_t) hatCount * 4, 1);
    js->hats        = calloc(hatCount, 1);
    js->axisCount   = axisCount;
    js->buttonCount = buttonCount;
    js->hatCount    = hatCount;

    strncpy(js->name, name, sizeof(js->name) - 1);
    strncpy(js->guid, guid, sizeof(js->guid) - 1);
    js->mapping = findValidMapping(js);

    return js;
}

// Frees arrays and name and flags the joystick object as unused
//
void _glfwFreeJoystick(_GLFWjoystick* js)
{
    free(js->axes);
    free(js->buttons);
    free(js->hats);
    memset(js, 0, sizeof(_GLFWjoystick));
}

GLFWAPI int glfwJoystickPresent(int jid)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);

    _GLFW_REQUIRE_INIT_JOYSTICK_OR_RETURN(GLFW_FALSE);

    if (jid < 0 || jid > GLFW_JOYSTICK_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid joystick ID %i", jid);
        return GLFW_FALSE;
    }

    js = _glfw_joystick.joysticks + jid;
    if (!js->present)
        return GLFW_FALSE;

    return _glfwPlatformPollJoystick(js, _GLFW_POLL_PRESENCE);
}

GLFWAPI const float* glfwGetJoystickAxes(int jid, int* count)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);
    assert(count != NULL);

    *count = 0;

    _GLFW_REQUIRE_INIT_JOYSTICK_OR_RETURN(NULL);

    if (jid < 0 || jid > GLFW_JOYSTICK_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid joystick ID %i", jid);
        return NULL;
    }

    js = _glfw_joystick.joysticks + jid;
    if (!js->present)
        return NULL;

    if (!_glfwPlatformPollJoystick(js, _GLFW_POLL_AXES))
        return NULL;

    *count = js->axisCount;
    return js->axes;
}

GLFWAPI const unsigned char* glfwGetJoystickButtons(int jid, int* count)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);
    assert(count != NULL);

    *count = 0;

    _GLFW_REQUIRE_INIT_JOYSTICK_OR_RETURN(NULL);

    if (jid < 0 || jid > GLFW_JOYSTICK_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid joystick ID %i", jid);
        return NULL;
    }

    js = _glfw_joystick.joysticks + jid;
    if (!js->present)
        return NULL;

    if (!_glfwPlatformPollJoystick(js, _GLFW_POLL_BUTTONS))
        return NULL;

    if (_glfw_joystick.hints.init.hatButtons)
        *count = js->buttonCount + js->hatCount * 4;
    else
        *count = js->buttonCount;

    return js->buttons;
}

GLFWAPI const unsigned char* glfwGetJoystickHats(int jid, int* count)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);
    assert(count != NULL);

    *count = 0;

    _GLFW_REQUIRE_INIT_JOYSTICK_OR_RETURN(NULL);

    if (jid < 0 || jid > GLFW_JOYSTICK_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid joystick ID %i", jid);
        return NULL;
    }

    js = _glfw_joystick.joysticks + jid;
    if (!js->present)
        return NULL;

    if (!_glfwPlatformPollJoystick(js, _GLFW_POLL_BUTTONS))
        return NULL;

    *count = js->hatCount;
    return js->hats;
}

GLFWAPI const char* glfwGetJoystickName(int jid)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);

    _GLFW_REQUIRE_INIT_JOYSTICK_OR_RETURN(NULL);

    if (jid < 0 || jid > GLFW_JOYSTICK_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid joystick ID %i", jid);
        return NULL;
    }

    js = _glfw_joystick.joysticks + jid;
    if (!js->present)
        return NULL;

    if (!_glfwPlatformPollJoystick(js, _GLFW_POLL_PRESENCE))
        return NULL;

    return js->name;
}

GLFWAPI const char* glfwGetJoystickGUID(int jid)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);

    _GLFW_REQUIRE_INIT_JOYSTICK_OR_RETURN(NULL);

    if (jid < 0 || jid > GLFW_JOYSTICK_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid joystick ID %i", jid);
        return NULL;
    }

    js = _glfw_joystick.joysticks + jid;
    if (!js->present)
        return NULL;

    if (!_glfwPlatformPollJoystick(js, _GLFW_POLL_PRESENCE))
        return NULL;

    return js->guid;
}

GLFWAPI void glfwSetJoystickUserPointer(int jid, void* pointer)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);

    _GLFW_REQUIRE_INIT_JOYSTICK();

    js = _glfw_joystick.joysticks + jid;
    if (!js->present)
        return;

    js->userPointer = pointer;
}

GLFWAPI void* glfwGetJoystickUserPointer(int jid)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);

    _GLFW_REQUIRE_INIT_JOYSTICK_OR_RETURN(NULL);

    js = _glfw_joystick.joysticks + jid;
    if (!js->present)
        return NULL;

    return js->userPointer;
}

GLFWAPI GLFWjoystickfun glfwSetJoystickCallback(GLFWjoystickfun cbfun)
{
    _GLFW_REQUIRE_INIT_JOYSTICK_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(_glfw_joystick.callbacks.joystick, cbfun);
    return cbfun;
}

GLFWAPI int glfwUpdateGamepadMappings(const char* string)
{
    int jid;
    const char* c = string;

    assert(string != NULL);

    _GLFW_REQUIRE_INIT_JOYSTICK_OR_RETURN(GLFW_FALSE);

    while (*c)
    {
        if ((*c >= '0' && *c <= '9') ||
            (*c >= 'a' && *c <= 'f') ||
            (*c >= 'A' && *c <= 'F'))
        {
            char line[1024];

            const size_t length = strcspn(c, "\r\n");
            if (length < sizeof(line))
            {
                _GLFWmapping mapping = {{0}};

                memcpy(line, c, length);
                line[length] = '\0';

                if (parseMapping(&mapping, line))
                {
                    _GLFWmapping* previous = findMapping(mapping.guid);
                    if (previous)
                        *previous = mapping;
                    else
                    {
                        _glfw_joystick.mappingCount++;
                        _glfw_joystick.mappings =
                            realloc(_glfw_joystick.mappings,
                                    sizeof(_GLFWmapping) * _glfw_joystick.mappingCount);
                        _glfw_joystick.mappings[_glfw_joystick.mappingCount - 1] = mapping;
                    }
                }
            }

            c += length;
        }
        else
        {
            c += strcspn(c, "\r\n");
            c += strspn(c, "\r\n");
        }
    }

    for (jid = 0;  jid <= GLFW_JOYSTICK_LAST;  jid++)
    {
        _GLFWjoystick* js = _glfw_joystick.joysticks + jid;
        if (js->present)
            js->mapping = findValidMapping(js);
    }

    return GLFW_TRUE;
}

GLFWAPI int glfwJoystickIsGamepad(int jid)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);

    _GLFW_REQUIRE_INIT_JOYSTICK_OR_RETURN(GLFW_FALSE);

    if (jid < 0 || jid > GLFW_JOYSTICK_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid joystick ID %i", jid);
        return GLFW_FALSE;
    }

    js = _glfw_joystick.joysticks + jid;
    if (!js->present)
        return GLFW_FALSE;

    if (!_glfwPlatformPollJoystick(js, _GLFW_POLL_PRESENCE))
        return GLFW_FALSE;

    return js->mapping != NULL;
}

GLFWAPI const char* glfwGetGamepadName(int jid)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);

    _GLFW_REQUIRE_INIT_JOYSTICK_OR_RETURN(NULL);

    if (jid < 0 || jid > GLFW_JOYSTICK_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid joystick ID %i", jid);
        return NULL;
    }

    js = _glfw_joystick.joysticks + jid;
    if (!js->present)
        return NULL;

    if (!_glfwPlatformPollJoystick(js, _GLFW_POLL_PRESENCE))
        return NULL;

    if (!js->mapping)
        return NULL;

    return js->mapping->name;
}

GLFWAPI int glfwGetGamepadState(int jid, GLFWgamepadstate* state)
{
    int i;
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);
    assert(state != NULL);

    memset(state, 0, sizeof(GLFWgamepadstate));

    _GLFW_REQUIRE_INIT_JOYSTICK_OR_RETURN(GLFW_FALSE);

    if (jid < 0 || jid > GLFW_JOYSTICK_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid joystick ID %i", jid);
        return GLFW_FALSE;
    }

    js = _glfw_joystick.joysticks + jid;
    if (!js->present)
        return GLFW_FALSE;

    if (!_glfwPlatformPollJoystick(js, _GLFW_POLL_ALL))
        return GLFW_FALSE;

    if (!js->mapping)
        return GLFW_FALSE;

    for (i = 0;  i <= GLFW_GAMEPAD_BUTTON_LAST;  i++)
    {
        const _GLFWmapelement* e = js->mapping->buttons + i;
        if (e->type == _GLFW_JOYSTICK_AXIS)
        {
            const float value = js->axes[e->index] * e->axisScale + e->axisOffset;
            // HACK: This should be baked into the value transform
            // TODO: Bake into transform when implementing output modifiers
            if (e->axisOffset < 0 || (e->axisOffset == 0 && e->axisScale > 0))
            {
                if (value >= 0.f)
                    state->buttons[i] = GLFW_PRESS;
            }
            else
            {
                if (value <= 0.f)
                    state->buttons[i] = GLFW_PRESS;
            }
        }
        else if (e->type == _GLFW_JOYSTICK_HATBIT)
        {
            const unsigned int hat = e->index >> 4;
            const unsigned int bit = e->index & 0xf;
            if (js->hats[hat] & bit)
                state->buttons[i] = GLFW_PRESS;
        }
        else if (e->type == _GLFW_JOYSTICK_BUTTON)
            state->buttons[i] = js->buttons[e->index];
    }

    for (i = 0;  i <= GLFW_GAMEPAD_AXIS_LAST;  i++)
    {
        const _GLFWmapelement* e = js->mapping->axes + i;
        if (e->type == _GLFW_JOYSTICK_AXIS)
        {
            const float value = js->axes[e->index] * e->axisScale + e->axisOffset;
            state->axes[i] = _glfw_fminf(_glfw_fmaxf(value, -1.f), 1.f);
        }
        else if (e->type == _GLFW_JOYSTICK_HATBIT)
        {
            const unsigned int hat = e->index >> 4;
            const unsigned int bit = e->index & 0xf;
            if (js->hats[hat] & bit)
                state->axes[i] = 1.f;
            else
                state->axes[i] = -1.f;
        }
        else if (e->type == _GLFW_JOYSTICK_BUTTON)
            state->axes[i] = js->buttons[e->index] * 2.f - 1.f;
    }

    return GLFW_TRUE;
}
