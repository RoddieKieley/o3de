/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzFramework/Input/Devices/Gamepad/InputDeviceGamepad.h>
#include <AzFramework/Input/Buses/Notifications/RawInputNotificationBus_Platform.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
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
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    AZ::u32 InputDeviceGamepad::GetMaxSupportedGamepads()
    {
        // ToDo: Return the maximum number of supported gamepads
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    InputDeviceGamepad::Implementation* InputDeviceGamepad::Implementation::Create(
        InputDeviceGamepad& inputDevice)
    {
        return aznew InputDeviceGamepadLinux(inputDevice);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    InputDeviceGamepadLinux::InputDeviceGamepadLinux(InputDeviceGamepad& inputDevice)
        : InputDeviceGamepad::Implementation(inputDevice)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    InputDeviceGamepadLinux::~InputDeviceGamepadLinux()
    {
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
        // ToDo: Process raw game-pad input and update input channels
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    void InputDeviceGamepadLinux::OnRawInputEvent(const AInputEvent* rawInputEvent)
    {
        // ToDo: Process raw game-pad input events
    }
} // namespace AzFramework
