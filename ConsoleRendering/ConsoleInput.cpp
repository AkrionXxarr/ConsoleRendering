/*
* Copyright(c) 2015, Stephen Bloomquist
* All rights reserved.
*/

#include <assert.h>

#include "ConsoleInput.hpp"
#include "MathClasses.h"

using namespace Math;

namespace aki
{
    namespace input
    {
        namespace wincon
        {

            /////////////////////////////
            // Construct & Destruct
            //

            ConsoleInput::ConsoleInput(unsigned int inputBufferSize, HWND consoleWindow)
            {
                this->inputBufferSize = inputBufferSize;
                this->consoleWindow = consoleWindow;
                inputRecords = new INPUT_RECORD[inputBufferSize];

                // Set and clear input buffers
                pressedKeys = new bool[KEYBOARD::END_OF_KEYBOARD];
                mouseActions = new bool[MOUSE_ACTION::END_OF_MOUSE_ACTION];
                pressedMouseButtons = new bool[MOUSE_BUTTON::END_OF_MOUSE_BUTTON];

                for (int i = 0; i < KEYBOARD::END_OF_KEYBOARD; i++)
                {
                    pressedKeys[i] = false;
                }
                for (int i = 0; i < MOUSE_ACTION::END_OF_MOUSE_ACTION; i++)
                {
                    mouseActions[i] = false;
                }
                for (int i = 0; i < MOUSE_BUTTON::END_OF_MOUSE_BUTTON; i++)
                {
                    pressedMouseButtons[i] = false;
                }
                // --

                mousePosition = { 0, 0 };

                inputHandle = GetStdHandle(STD_INPUT_HANDLE);
                assert(inputHandle != INVALID_HANDLE_VALUE);

                // Save the mode so that it may be restored
                !GetConsoleMode(inputHandle, &oldMode);

                !SetConsoleMode(inputHandle, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
            }

            ConsoleInput::~ConsoleInput()
            {
                !SetConsoleMode(inputHandle, oldMode);

                if (inputRecords)
                    delete[] inputRecords;
                if (pressedKeys)
                    delete[] pressedKeys;
                if (pressedMouseButtons)
                    delete[] pressedMouseButtons;
            }


            /////////////////////////////////
            // ConsoleInput operations
            //

            void ConsoleInput::Tick()
            {
                releasedKeys.clear();
                releasedMouseButtons.clear();

                bool curFocus = (GetForegroundWindow() == consoleWindow);
                regainedFocus = (curFocus && !hasFocus);
                hasFocus = curFocus;

                if (hasFocus)
                {
                    DWORD inputCount;

                    for (int i = 0; i < MOUSE_ACTION::END_OF_MOUSE_ACTION; i++)
                        mouseActions[i] = false;

                    !PeekConsoleInput(inputHandle, inputRecords, inputBufferSize, &inputCount);

                    !FlushConsoleInputBuffer(inputHandle);

                    for (unsigned int i = 0; i < inputCount; i++)
                    {
                        INPUT_RECORD input = inputRecords[i];

                        switch (input.EventType)
                        {
                        case KEY_EVENT:
                            KeyEvent(input.Event.KeyEvent);
                            break;

                        case MOUSE_EVENT:
                            MouseEvent(input.Event.MouseEvent);
                            break;
                        }
                    }
                }
                else
                {
                    ClearInputEvents();
                }
            }


            ///////////////////
            // Getters
            //

            /* Keyboard */
            bool ConsoleInput::GetKeyUp(KEYBOARD::TYPE key)
            {
                bool keyMatched = false;

                for (unsigned int i = 0; i < releasedKeys.size(); i++)
                {
                    if (releasedKeys[i] == key)
                    {
                        keyMatched = true;
                        break;
                    }
                }

                return keyMatched;
            }

            bool ConsoleInput::GetKeyDown(KEYBOARD::TYPE key)
            {
                return pressedKeys[key];
            }
            /* ---------------------------------------------------------------- */


            /* Mouse */
            bool ConsoleInput::GetMouseDown(MOUSE_BUTTON::TYPE button)
            {
                return pressedMouseButtons[button];
            }

            bool ConsoleInput::GetMouseUp(MOUSE_BUTTON::TYPE button)
            {
                bool buttonMatched = false;

                for (unsigned int i = 0; i < releasedMouseButtons.size(); i++)
                {
                    if (releasedMouseButtons[i] == button)
                    {
                        buttonMatched = true;
                        break;
                    }
                }

                return buttonMatched;
            }

            bool ConsoleInput::GetMouseAction(MOUSE_ACTION::TYPE action)
            {
                return mouseActions[action];
            }

            COORD ConsoleInput::GetMousePosition()
            {
                return mousePosition;
            }
            /* ---------------------------------------------------------------- */


            //////////////////
            // Utility
            //

            void ConsoleInput::KeyEvent(KEY_EVENT_RECORD keyEvent)
            {
                KEYBOARD::TYPE key = KEYBOARD::NO_KEY;

                switch (keyEvent.wVirtualKeyCode)
                {
                    /* Letter keys */
                case 0x41: key = KEYBOARD::A; break;
                case 0x42: key = KEYBOARD::B; break;
                case 0x43: key = KEYBOARD::C; break;
                case 0x44: key = KEYBOARD::D; break;
                case 0x45: key = KEYBOARD::E; break;
                case 0x46: key = KEYBOARD::F; break;
                case 0x47: key = KEYBOARD::G; break;
                case 0x48: key = KEYBOARD::H; break;
                case 0x49: key = KEYBOARD::I; break;
                case 0x4A: key = KEYBOARD::J; break;
                case 0x4B: key = KEYBOARD::K; break;
                case 0x4C: key = KEYBOARD::L; break;
                case 0x4D: key = KEYBOARD::M; break;
                case 0x4E: key = KEYBOARD::N; break;
                case 0x4F: key = KEYBOARD::O; break;
                case 0x50: key = KEYBOARD::P; break;
                case 0x51: key = KEYBOARD::Q; break;
                case 0x52: key = KEYBOARD::R; break;
                case 0x53: key = KEYBOARD::S; break;
                case 0x54: key = KEYBOARD::T; break;
                case 0x55: key = KEYBOARD::U; break;
                case 0x56: key = KEYBOARD::V; break;
                case 0x57: key = KEYBOARD::W; break;
                case 0x58: key = KEYBOARD::X; break;
                case 0x59: key = KEYBOARD::Y; break;
                case 0x5A: key = KEYBOARD::Z; break;

                    /* Number Keys */
                case 0x30: key = KEYBOARD::NUM_0; break;
                case 0x31: key = KEYBOARD::NUM_1; break;
                case 0x32: key = KEYBOARD::NUM_2; break;
                case 0x33: key = KEYBOARD::NUM_3; break;
                case 0x34: key = KEYBOARD::NUM_4; break;
                case 0x35: key = KEYBOARD::NUM_5; break;
                case 0x36: key = KEYBOARD::NUM_6; break;
                case 0x37: key = KEYBOARD::NUM_7; break;
                case 0x38: key = KEYBOARD::NUM_8; break;
                case 0x39: key = KEYBOARD::NUM_9; break;

                    /* Arrow Keys */
                case VK_LEFT: key = KEYBOARD::ARROW_LEFT; break;
                case VK_RIGHT: key = KEYBOARD::ARROW_RIGHT; break;
                case VK_UP: key = KEYBOARD::ARROW_UP; break;
                case VK_DOWN: key = KEYBOARD::ARROW_DOWN; break;

                    /* Numpad Keys */
                case VK_NUMPAD0: key = KEYBOARD::NUMPAD_0; break;
                case VK_NUMPAD1: key = KEYBOARD::NUMPAD_1; break;
                case VK_NUMPAD2: key = KEYBOARD::NUMPAD_2; break;
                case VK_NUMPAD3: key = KEYBOARD::NUMPAD_3; break;
                case VK_NUMPAD4: key = KEYBOARD::NUMPAD_4; break;
                case VK_NUMPAD5: key = KEYBOARD::NUMPAD_5; break;
                case VK_NUMPAD6: key = KEYBOARD::NUMPAD_6; break;
                case VK_NUMPAD7: key = KEYBOARD::NUMPAD_7; break;
                case VK_NUMPAD8: key = KEYBOARD::NUMPAD_8; break;
                case VK_NUMPAD9: key = KEYBOARD::NUMPAD_9; break;

                case VK_ADD: key = KEYBOARD::NUMPAD_ADD; break;
                case VK_SUBTRACT: key = KEYBOARD::NUMPAD_SUBTRACT; break;
                case VK_MULTIPLY: key = KEYBOARD::NUMPAD_MULTIPLY; break;
                case VK_DIVIDE: key = KEYBOARD::NUMPAD_DIVIDE; break;
                case VK_DECIMAL: key = KEYBOARD::NUMPAD_DECIMAL; break;

                    /* Other Keys */
                case VK_BACK: key = KEYBOARD::BACKSPACE; break;
                case VK_TAB: key = KEYBOARD::TAB; break;
                case VK_INSERT: key = KEYBOARD::INSERT; break;
                case VK_DELETE: key = KEYBOARD::DEL; break;
                case VK_HOME: key = KEYBOARD::HOME; break;
                case VK_END: key = KEYBOARD::END; break;
                case VK_PRIOR: key = KEYBOARD::PAGE_UP; break;
                case VK_NEXT: key = KEYBOARD::PAGE_DOWN; break;
                case VK_APPS: key = KEYBOARD::MENU; break;
                case VK_SNAPSHOT: key = KEYBOARD::PRINT_SCREEN; break;
                case VK_PAUSE: key = KEYBOARD::PAUSE; break;
                case VK_ESCAPE: key = KEYBOARD::ESCAPE; break;

                case VK_OEM_3: key = KEYBOARD::GRAVE; break;
                case VK_SPACE: key = KEYBOARD::SPACE; break;
                case VK_OEM_2: key = KEYBOARD::SLASH_FORWARD; break;
                case VK_OEM_5: key = KEYBOARD::SLASH_BACKWARD; break;
                case VK_OEM_COMMA: key = KEYBOARD::COMMA; break;
                case VK_OEM_PERIOD: key = KEYBOARD::PERIOD; break;
                case VK_OEM_1: key = KEYBOARD::SEMICOLON; break;
                case VK_OEM_7: key = KEYBOARD::APOSTROPHE; break;
                case VK_OEM_MINUS: key = KEYBOARD::HYPHEN; break;
                case VK_OEM_PLUS: key = KEYBOARD::EQUALS; break;
                case VK_OEM_4: key = KEYBOARD::SQUARE_BRACKET_LEFT; break;
                case VK_OEM_6: key = KEYBOARD::SQUARE_BRACKET_RIGHT; break;

                    /* Lock keys */
                case VK_CAPITAL: key = KEYBOARD::CAPS_LOCK; break;
                case VK_NUMLOCK: key = KEYBOARD::NUM_LOCK; break;
                case VK_SCROLL: key = KEYBOARD::SCROLL_LOCK; break;

                    /* F Keys */
                case VK_F1: key = KEYBOARD::F1; break;
                case VK_F2: key = KEYBOARD::F2; break;
                case VK_F3: key = KEYBOARD::F3; break;
                case VK_F4: key = KEYBOARD::F4; break;
                case VK_F5: key = KEYBOARD::F5; break;
                case VK_F6: key = KEYBOARD::F6; break;
                case VK_F7: key = KEYBOARD::F7; break;
                case VK_F8: key = KEYBOARD::F8; break;
                case VK_F9: key = KEYBOARD::F9; break;
                case VK_F10: key = KEYBOARD::F10; break;
                case VK_F11: key = KEYBOARD::F11; break;
                case VK_F12: key = KEYBOARD::F12; break;

                    /* Control Keys */
                case VK_MENU: key = KEYBOARD::ALT; break;
                case VK_CONTROL: key = KEYBOARD::CONTROL; break;
                case VK_SHIFT: key = KEYBOARD::SHIFT; break;
                case VK_LWIN: key = KEYBOARD::SYSTEM_LEFT; break;
                case VK_RWIN: key = KEYBOARD::SYSTEM_RIGHT; break;

                    /* Default */
                default: key = KEYBOARD::UNHANDLED_KEY; break;
                }

                pressedKeys[key] = (keyEvent.bKeyDown != 0);

                if (!keyEvent.bKeyDown)
                    releasedKeys.push_front(key);
            }

            void ConsoleInput::MouseEvent(MOUSE_EVENT_RECORD mouseEvent)
            {
                MOUSE_ACTION::TYPE mouseAction = MOUSE_ACTION::NO_ACTION;
                MOUSE_BUTTON::TYPE mouseButton = MOUSE_BUTTON::NO_BUTTON;
                DWORD mouseButtonFlags = mouseEvent.dwButtonState;

                switch (mouseEvent.dwEventFlags)
                {
                    /* Button pressed or released */
                case 0:
                    mouseAction = MOUSE_ACTION::CLICK;
                    break;

                    /* Actions */
                case DOUBLE_CLICK:
                    mouseAction = MOUSE_ACTION::CLICK_DOUBLE;
                    break;
                case MOUSE_HWHEELED:
                    // HIWORD returns an unsigned short but a signed short
                    // is used to indicate direction.
                    if (short(HIWORD(mouseButtonFlags)) > 0)
                        mouseAction = MOUSE_ACTION::MOUSE_WHEEL_RIGHT;
                    else
                        mouseAction = MOUSE_ACTION::MOUSE_WHEEL_LEFT;
                    break;
                case MOUSE_WHEELED:
                    // HIWORD returns an unsigned short but a signed short
                    // is used to indicate direction.
                    if (short(HIWORD(mouseButtonFlags)) > 0)
                        mouseAction = MOUSE_ACTION::MOUSE_WHEEL_FORWARD;
                    else
                        mouseAction = MOUSE_ACTION::MOUSE_WHEEL_BACKWARD;
                    break;
                case MOUSE_MOVED:
                    mousePosition = mouseEvent.dwMousePosition;
                    mouseAction = MOUSE_ACTION::MOVED;
                    break;
                default:
                    mouseAction = MOUSE_ACTION::UNHANDLED_ACTION;
                    break;
                }

                if (mouseAction == MOUSE_ACTION::CLICK || mouseAction == MOUSE_ACTION::CLICK_DOUBLE)
                {
                    // Extract the bit flags
                    bool leftClick = (mouseButtonFlags & FROM_LEFT_1ST_BUTTON_PRESSED) != 0;
                    bool rightClick = (mouseButtonFlags & RIGHTMOST_BUTTON_PRESSED) != 0;
                    bool middleClick = (mouseButtonFlags & FROM_LEFT_2ND_BUTTON_PRESSED) != 0;


                    if (leftClick)
                    {
                        // Button was or still is pressed
                        pressedMouseButtons[MOUSE_BUTTON::CLICK_LEFT] = true;
                    }
                    else if (!leftClick && pressedMouseButtons[MOUSE_BUTTON::CLICK_LEFT])
                    {
                        // Button was pressed, but is not any longer
                        pressedMouseButtons[MOUSE_BUTTON::CLICK_LEFT] = false;
                        releasedMouseButtons.push_front(MOUSE_BUTTON::CLICK_LEFT);
                    }

                    if (middleClick)
                    {
                        // Button was or still is pressed
                        pressedMouseButtons[MOUSE_BUTTON::CLICK_MIDDLE] = true;
                    }
                    else if (!middleClick && pressedMouseButtons[MOUSE_BUTTON::CLICK_MIDDLE])
                    {
                        // Button was pressed, but is not any longer
                        pressedMouseButtons[MOUSE_BUTTON::CLICK_MIDDLE] = false;
                        releasedMouseButtons.push_front(MOUSE_BUTTON::CLICK_MIDDLE);
                    }

                    if (rightClick)
                    {
                        // Button was or still is pressed
                        pressedMouseButtons[MOUSE_BUTTON::CLICK_RIGHT] = true;
                    }
                    else if (!leftClick && pressedMouseButtons[MOUSE_BUTTON::CLICK_RIGHT])
                    {
                        // Button was pressed, but is not any longer
                        pressedMouseButtons[MOUSE_BUTTON::CLICK_RIGHT] = false;
                        releasedMouseButtons.push_front(MOUSE_BUTTON::CLICK_RIGHT);
                    }
                }

                mouseActions[mouseAction] = true;
            }

            void ConsoleInput::ClearInputEvents()
            {
                for (int i = 0; i < KEYBOARD::END_OF_KEYBOARD; i++)
                {
                    pressedKeys[i] = false;
                }
                for (int i = 0; i < MOUSE_ACTION::END_OF_MOUSE_ACTION; i++)
                {
                    mouseActions[i] = false;
                }
                for (int i = 0; i < MOUSE_BUTTON::END_OF_MOUSE_BUTTON; i++)
                {
                    pressedMouseButtons[i] = false;
                }
            }
        };
    };
};