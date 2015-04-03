/*
* Copyright(c) 2015, Stephen Bloomquist
* All rights reserved.
*/

#include "ConsoleInputExt.hpp"

namespace aki
{
    namespace input
    {
        namespace wincon
        {

            ///////////////////////////////
            // Construct & Destruct
            //
            ConsoleInputExt::ConsoleInputExt(unsigned int inputBufferSize, HWND consoleWindow) : ConsoleInput(inputBufferSize, consoleWindow)
            {
                capsLock = false;
                scrollLock = false;
                numLock = false;
            }

            ConsoleInputExt::~ConsoleInputExt()
            {
            }


            void ConsoleInputExt::Tick()
            {
                ConsoleInput::Tick();

                if (!hasFocus)
                {
                    // Cursor should be locked, and is locked, so unlock it
                    if (lockCursor && cursorIsLocked)
                    {
                        // Given the unique case of this unlock, the unlock is handled manually.
                        if (!cursorLock.Stop())
                        {
                            return;
                        }

                        cursorIsLocked = false;
                        lockCursor = true;
                    }
                }
                else if (regainedFocus)
                {
                    // Cursor should be locked, and isn't locked, so lock it
                    if (lockCursor && !cursorIsLocked)
                    {
                        if (!LockCursor(true))
                            return;
                    }
                }
            }

            bool ConsoleInputExt::LockCursor(bool lock)
            {
                lockCursor = lock;

                if (lock)
                {
                    RECT r;
                    POINT p;
                    if (!GetWindowRect(consoleWindow, &r))
                    {
                        return false;
                    }

                    p.x = r.left + (r.right - r.left) / 2;
                    p.y = r.top + (r.bottom - r.top) / 2;

                    if (!cursorLock.Start(p))
                    {
                        return false;
                    }

                    cursorIsLocked = true;
                }
                else
                {
                    if (!cursorLock.Stop())
                    {
                        return false;
                    }

                    cursorIsLocked = false;
                }

                return true;
            }

            char ConsoleInputExt::GetCharacter()
            {
#define t(keyCode, hex) if(GetKeyDown(keyCode)) { return hex; }

                bool shift = GetKeyDown(KEYBOARD::SHIFT);

                /* Numpad keys */
                t(KEYBOARD::NUMPAD_0, 0x30); // 0
                t(KEYBOARD::NUMPAD_1, 0x31); // 1
                t(KEYBOARD::NUMPAD_2, 0x32); // 2
                t(KEYBOARD::NUMPAD_3, 0x33); // 3
                t(KEYBOARD::NUMPAD_4, 0x34); // 4
                t(KEYBOARD::NUMPAD_5, 0x35); // 5
                t(KEYBOARD::NUMPAD_6, 0x36); // 6
                t(KEYBOARD::NUMPAD_7, 0x37); // 7
                t(KEYBOARD::NUMPAD_8, 0x38); // 8
                t(KEYBOARD::NUMPAD_9, 0x39); // 9

                t(KEYBOARD::NUMPAD_ADD, 0x2B); // +
                t(KEYBOARD::NUMPAD_SUBTRACT, 0x2D); // -
                t(KEYBOARD::NUMPAD_MULTIPLY, 0x2A); // *
                t(KEYBOARD::NUMPAD_DIVIDE, 0x2F); // /
                t(KEYBOARD::NUMPAD_DECIMAL, 0x2E); // .

                /* Other Keys */
                t(KEYBOARD::BACKSPACE, 0x08);
                t(KEYBOARD::TAB, 0x09);
                t(KEYBOARD::SPACE, 0x20);

                if (shift)
                {
                    /* Letter keys */
                    t(KEYBOARD::A, 0x41); // A
                    t(KEYBOARD::B, 0x42); // B
                    t(KEYBOARD::C, 0x43); // C
                    t(KEYBOARD::D, 0x44); // D
                    t(KEYBOARD::E, 0x45); // E
                    t(KEYBOARD::F, 0x46); // F
                    t(KEYBOARD::G, 0x47); // G
                    t(KEYBOARD::H, 0x48); // H
                    t(KEYBOARD::I, 0x49); // I
                    t(KEYBOARD::J, 0x4A); // J
                    t(KEYBOARD::K, 0x4B); // K
                    t(KEYBOARD::L, 0x4C); // L
                    t(KEYBOARD::M, 0x4D); // M
                    t(KEYBOARD::N, 0x4E); // N
                    t(KEYBOARD::O, 0x4F); // O
                    t(KEYBOARD::P, 0x50); // P
                    t(KEYBOARD::Q, 0x51); // Q
                    t(KEYBOARD::R, 0x52); // R
                    t(KEYBOARD::S, 0x53); // S
                    t(KEYBOARD::T, 0x54); // T
                    t(KEYBOARD::U, 0x55); // U
                    t(KEYBOARD::V, 0x56); // V
                    t(KEYBOARD::W, 0x57); // W
                    t(KEYBOARD::X, 0x58); // X
                    t(KEYBOARD::Y, 0x59); // Y
                    t(KEYBOARD::Z, 0x5A); // Z

                    /* Number keys */
                    t(KEYBOARD::NUM_0, 0x29); // )
                    t(KEYBOARD::NUM_1, 0x21); // !
                    t(KEYBOARD::NUM_2, 0x40); // @
                    t(KEYBOARD::NUM_3, 0x23); // #
                    t(KEYBOARD::NUM_4, 0x24); // $
                    t(KEYBOARD::NUM_5, 0x25); // %
                    t(KEYBOARD::NUM_6, 0x5E); // ^
                    t(KEYBOARD::NUM_7, 0x26); // &
                    t(KEYBOARD::NUM_8, 0x2A); // *
                    t(KEYBOARD::NUM_9, 0x28); // (

                    /* Other keys */
                    t(KEYBOARD::GRAVE, 0x7E); // ~
                    t(KEYBOARD::SLASH_FORWARD, 0x3F); // ?
                    t(KEYBOARD::SLASH_BACKWARD, 0x7C); // |
                    t(KEYBOARD::COMMA, 0x3C); // <
                    t(KEYBOARD::PERIOD, 0x3E); // >
                    t(KEYBOARD::SEMICOLON, 0x3A); // :
                    t(KEYBOARD::APOSTROPHE, 0x22); // "
                    t(KEYBOARD::HYPHEN, 0x5F); // _
                    t(KEYBOARD::EQUALS, 0x2B); // +
                    t(KEYBOARD::SQUARE_BRACKET_LEFT, 0x7B); // {
                    t(KEYBOARD::SQUARE_BRACKET_RIGHT, 0x7D); // }
                }
                else
                {
                    /* Letter keys */
                    t(KEYBOARD::A, 0x61); // a
                    t(KEYBOARD::B, 0x62); // b
                    t(KEYBOARD::C, 0x63); // c
                    t(KEYBOARD::D, 0x64); // d
                    t(KEYBOARD::E, 0x65); // e
                    t(KEYBOARD::F, 0x66); // f
                    t(KEYBOARD::G, 0x67); // g
                    t(KEYBOARD::H, 0x68); // h
                    t(KEYBOARD::I, 0x69); // i
                    t(KEYBOARD::J, 0x6A); // j
                    t(KEYBOARD::K, 0x6B); // k
                    t(KEYBOARD::L, 0x6C); // l
                    t(KEYBOARD::M, 0x6D); // m
                    t(KEYBOARD::N, 0x6E); // n
                    t(KEYBOARD::O, 0x6F); // o
                    t(KEYBOARD::P, 0x70); // p
                    t(KEYBOARD::Q, 0x71); // q
                    t(KEYBOARD::R, 0x72); // r
                    t(KEYBOARD::S, 0x73); // s
                    t(KEYBOARD::T, 0x74); // t
                    t(KEYBOARD::U, 0x75); // u
                    t(KEYBOARD::V, 0x76); // v
                    t(KEYBOARD::W, 0x77); // w
                    t(KEYBOARD::X, 0x78); // x
                    t(KEYBOARD::Y, 0x79); // y
                    t(KEYBOARD::Z, 0x7A); // z

                    /* Number keys */
                    t(KEYBOARD::NUM_0, 0x30); // 0
                    t(KEYBOARD::NUM_1, 0x31); // 1
                    t(KEYBOARD::NUM_2, 0x32); // 2
                    t(KEYBOARD::NUM_3, 0x33); // 3
                    t(KEYBOARD::NUM_4, 0x34); // 4
                    t(KEYBOARD::NUM_5, 0x35); // 5
                    t(KEYBOARD::NUM_6, 0x36); // 6
                    t(KEYBOARD::NUM_7, 0x37); // 7
                    t(KEYBOARD::NUM_8, 0x38); // 8
                    t(KEYBOARD::NUM_9, 0x39); // 9

                    /* Other keys */
                    t(KEYBOARD::GRAVE, 0x60); // `
                    t(KEYBOARD::SLASH_FORWARD, 0x2F); // /
                    t(KEYBOARD::SLASH_BACKWARD, 0x5C); // \ 
                    t(KEYBOARD::COMMA, 0x2C); // ,
                    t(KEYBOARD::PERIOD, 0x2E); // .
                    t(KEYBOARD::SEMICOLON, 0x3B); // ;
                    t(KEYBOARD::APOSTROPHE, 0x27); // '
                    t(KEYBOARD::HYPHEN, 0x2D); // -
                    t(KEYBOARD::EQUALS, 0x3D); // =
                    t(KEYBOARD::SQUARE_BRACKET_LEFT, 0x5B); // [
                    t(KEYBOARD::SQUARE_BRACKET_RIGHT, 0x5D); // ]
                }



                return 0;
            }

            KEYBOARD::TYPE ConsoleInputExt::GetMostRecentKey()
            {
                return mostRecentKey;
            }

            MOUSE_BUTTON::TYPE ConsoleInputExt::GetMostRecentMouseButton()
            {
                return mostRecentMouseButton;
            }

            bool ConsoleInputExt::GetAnyKeyUp()
            {
                return (releasedKeys.size() > 0);
            }

            bool ConsoleInputExt::GetAnyKeyDown()
            {
                bool keyIsDown = false;

                for (int i = 0; i < KEYBOARD::END_OF_KEYBOARD; i++)
                {
                    if (pressedKeys[i])
                    {
                        keyIsDown = true;
                        break;
                    }
                }

                return keyIsDown;
            }

            bool ConsoleInputExt::GetAnyMouseUp()
            {
                return (releasedMouseButtons.size() > 0);
            }

            bool ConsoleInputExt::GetAnyMouseDown()
            {
                bool mouseIsDown = false;

                for (int i = 0; i < MOUSE_BUTTON::END_OF_MOUSE_BUTTON; i++)
                {
                    if (pressedMouseButtons[i])
                    {
                        mouseIsDown = true;
                        break;
                    }
                }

                return mouseIsDown;
            }

            POINT ConsoleInputExt::GetDelta()
            {
                return cursorLock.GetDelta();
            }
        };
    };
};