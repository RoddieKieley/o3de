/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "glfw3common.h"
#include "internal_joystick.h"
#include "posix_time.h"
#include "linux_joystick.h"
#include "init_joystick.h"

#include <AzFramework/Input/Devices/Gamepad/InputDeviceGamepad.h>
#include <AzFramework/Input/Buses/Notifications/RawInputNotificationBus_Platform.h>

GLFWAPI int glfwInitJoystick();

////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: RNK: 12/03/21 - Fill in values here to map channels to glfw input
// As per *_Apple.mm atm
namespace
{
    using namespace AzFramework;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! Digital button bitmasks
    const AZ::u32 DIGITAL_BUTTON_MASK_DU    = 0x0001;
    const AZ::u32 DIGITAL_BUTTON_MASK_DD    = 0x0002;
    const AZ::u32 DIGITAL_BUTTON_MASK_DL    = 0x0004;
    const AZ::u32 DIGITAL_BUTTON_MASK_DR    = 0x0008;

    const AZ::u32 DIGITAL_BUTTON_MASK_L1    = 0x0010;
    const AZ::u32 DIGITAL_BUTTON_MASK_R1    = 0x0020;
    const AZ::u32 DIGITAL_BUTTON_MASK_L3    = 0x0040;
    const AZ::u32 DIGITAL_BUTTON_MASK_R3    = 0x0080;

    const AZ::u32 DIGITAL_BUTTON_MASK_A     = 0x0100;
    const AZ::u32 DIGITAL_BUTTON_MASK_B     = 0x0200;
    const AZ::u32 DIGITAL_BUTTON_MASK_X     = 0x0400;
    const AZ::u32 DIGITAL_BUTTON_MASK_Y     = 0x0800;

    const AZ::u32 DIGITAL_BUTTON_MASK_PAUSE = 0x1000;
    const AZ::u32 DIGITAL_BUTTON_MASK_SELECT = 0x2000;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! Map of digital button ids keyed by their button bitmask
    const AZStd::unordered_map<AZ::u32, const InputChannelId*> GetDigitalButtonIdByBitMaskMap()
    {
        const AZStd::unordered_map<AZ::u32, const InputChannelId*> map =
                {
                { DIGITAL_BUTTON_MASK_DU,       &InputDeviceGamepad::Button::DU },
                { DIGITAL_BUTTON_MASK_DD,       &InputDeviceGamepad::Button::DD },
                { DIGITAL_BUTTON_MASK_DL,       &InputDeviceGamepad::Button::DL },
                { DIGITAL_BUTTON_MASK_DR,       &InputDeviceGamepad::Button::DR },

                { DIGITAL_BUTTON_MASK_L1,       &InputDeviceGamepad::Button::L1 },
                { DIGITAL_BUTTON_MASK_R1,       &InputDeviceGamepad::Button::R1 },
                { DIGITAL_BUTTON_MASK_L3,       &InputDeviceGamepad::Button::L3 },
                { DIGITAL_BUTTON_MASK_R3,       &InputDeviceGamepad::Button::R3 },

                { DIGITAL_BUTTON_MASK_A,        &InputDeviceGamepad::Button::A },
                { DIGITAL_BUTTON_MASK_B,        &InputDeviceGamepad::Button::B },
                { DIGITAL_BUTTON_MASK_X,        &InputDeviceGamepad::Button::X },
                { DIGITAL_BUTTON_MASK_Y,        &InputDeviceGamepad::Button::Y },

                { DIGITAL_BUTTON_MASK_PAUSE,    &InputDeviceGamepad::Button::Start },
                { DIGITAL_BUTTON_MASK_SELECT,   &InputDeviceGamepad::Button::Select }
                };
        return map;
    }

    // TODO: RNK: 12/03/21 - Determine where this should really live
    // Wait for data to arrive using select
    // This avoids blocking other threads via the per-display Xlib lock that also
    // covers GLX functions
    //
    static GLFWbool waitForEvent(double* timeout)
    {
        fd_set fds;
        const int fd = 0;//ConnectionNumber(_glfw.x11.display);
        int count = fd + 1;

#if defined(__linux__)
        if (_glfw_joystick.linjs.inotify > fd)
            count = _glfw_joystick.linjs.inotify + 1;
#endif
        for (;;)
        {
            FD_ZERO(&fds);
            FD_SET(fd, &fds);
#if defined(__linux__)
            if (_glfw_joystick.linjs.inotify > 0)
                FD_SET(_glfw_joystick.linjs.inotify, &fds);
#endif

            if (timeout)
            {
                const long seconds = (long) *timeout;
                const long microseconds = (long) ((*timeout - seconds) * 1e6);
                struct timeval tv = { seconds, microseconds };
                const uint64_t base = _glfwPlatformGetTimerValue();

                const int result = select(count, &fds, NULL, NULL, &tv);
                const int error = errno;

                *timeout -= (_glfwPlatformGetTimerValue() - base) /
                        (double) _glfwPlatformGetTimerFrequency();

                if (result > 0)
                    return GLFW_TRUE;
                if ((result == -1 && error == EINTR) || *timeout <= 0.0)
                    return GLFW_FALSE;
            }
            else if (select(count, &fds, NULL, NULL, NULL) != -1 || errno != EINTR)
                return GLFW_TRUE;
        }
    }
} // namespace

namespace AzFramework
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //! Platform specific implementation for Linux game-pad input devices
    class InputDeviceGamepadLinux : public InputDeviceGamepad::Implementation
                                  , public RawInputNotificationBusLinux::Handler
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////////////
        // Allocator
        AZ_CLASS_ALLOCATOR(InputDeviceGamepadLinux, AZ::SystemAllocator, 0);

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! Constructor
        //! \param[in] inputDevice Reference to the input device being implemented
        InputDeviceGamepadLinux(InputDeviceGamepad& inputDevice);

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! Destructor
        ~InputDeviceGamepadLinux() override;

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \ref AzFramework::InputDeviceGamepad::Implementation::IsConnected
        bool IsConnected() const override;

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \ref AzFramework::InputDeviceGamepad::Implementation::SetVibration
        void SetVibration(float leftMotorSpeedNormalized, float rightMotorSpeedNormalized) override;

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \ref AzFramework::InputDeviceGamepad::Implementation::TickInputDevice
        void TickInputDevice() override;

        ////////////////////////////////////////////////////////////////////////////////////////////
        //! \ref AzFramework::RawInputNotificationsLinux::OnRawInputEvent
        void OnRawInputEvent(const AInputEvent* rawInputEvent) override;

        ////////////////////////////////////////////////////////////////////////////////////////////
        // Variables
        RawGamepadState m_rawGamepadState;        //!< The last known raw game-pad state
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    AZ::u32 InputDeviceGamepad::GetMaxSupportedGamepads()
    {
        // ToDo: Return the maximum number of supported gamepads
        return 1;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    InputDeviceGamepad::Implementation* InputDeviceGamepad::Implementation::Create(
        InputDeviceGamepad& inputDevice)
    {
        glfwInitJoystick();
        _glfwInitJoysticksLinux();

        return aznew InputDeviceGamepadLinux(inputDevice);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    InputDeviceGamepadLinux::InputDeviceGamepadLinux(InputDeviceGamepad& inputDevice)
        : InputDeviceGamepad::Implementation(inputDevice)
        , m_rawGamepadState(GetDigitalButtonIdByBitMaskMap())
    {

    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    InputDeviceGamepadLinux::~InputDeviceGamepadLinux()
    {
        _glfwTerminateJoysticksLinux();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    bool InputDeviceGamepadLinux::IsConnected() const
    {
        // ToDo: Figure out how/whether we can determine the availability of this game-pad,
        // and implement this function (along with dispatching connect/disconnect events)
        return false;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    void InputDeviceGamepadLinux::SetVibration(float leftMotorSpeedNormalized,
                                                 float rightMotorSpeedNormalized)
    {
        // ToDo: Implement force-feedback if supported?
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    void InputDeviceGamepadLinux::TickInputDevice()
    {
        double timeout = 0.1; // 0.1 seconds?

        if (!waitForEvent(&timeout))
            return;// GLFW_FALSE;

#if defined(__linux__)
        _glfwDetectJoystickConnectionLinux();
#endif
        // ToDo: Process raw game-pad input and update input channels

        // Check for a controller being connected?
        // If we just connected -> connect event (BroadcastIputDeviceConnectedEvent() from *_Apple.mm ?)

        // Check for a controller being disconnected
        // If we just disconnected -> reset data for the disconnected controller -> disconnect event

        // Any controllers currently connected? If not bail

        // Got at least on controller...
        // foreach controller update RawGamepadState

        ProcessRawGamepadState(m_rawGamepadState);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    void InputDeviceGamepadLinux::OnRawInputEvent(const AInputEvent* rawInputEvent)
    {
        // ToDo: Process raw game-pad input events
    }
} // namespace AzFramework
