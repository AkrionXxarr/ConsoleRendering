/*
* Copyright(c) 2015, Stephen Bloomquist
* All rights reserved.
*/

#pragma once

#include <process.h>

#include "ConsoleInput.hpp"

namespace aki
{
    namespace input
    {
        namespace wincon
        {
            class CursorLock
            {
                friend class ConsoleInputExt;

            private:
                CursorLock();
                ~CursorLock();

                static unsigned int __stdcall Run(void* params);

                bool Start(POINT center);
                bool Stop();

                POINT GetDelta();

            private:
                POINT delta;
                POINT center;
                CRITICAL_SECTION pointCritical;
                CRITICAL_SECTION runCritical;
                HANDLE handle;
                bool running;
            };

            //////////////////////////////////////////////////////////////////////////////
            // An extension on the base functionality of ConsoleInput
            //
            class ConsoleInputExt : public ConsoleInput
            {
            public:
                /* Construct & Destruct */
                ConsoleInputExt(unsigned int inputBufferSize, HWND consoleWindow);
                virtual ~ConsoleInputExt();

                /* ConsoleInputExt operations */
                virtual void Tick();

                bool LockCursor(bool lock);

                /* Getters */
                char GetCharacter();

                KEYBOARD::TYPE GetMostRecentKey();
                MOUSE_BUTTON::TYPE GetMostRecentMouseButton();

                bool GetAnyKeyUp();
                bool GetAnyKeyDown();

                bool GetAnyMouseUp();
                bool GetAnyMouseDown();

                POINT GetDelta();

            protected:
                KEYBOARD::TYPE mostRecentKey;
                MOUSE_BUTTON::TYPE mostRecentMouseButton;

                bool capsLock, scrollLock, numLock;

            private:
                CursorLock cursorLock;
                bool lockCursor;
                bool cursorIsLocked;
            };
        };
    };
};