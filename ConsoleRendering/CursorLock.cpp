/*
* Copyright(c) 2015, Stephen Bloomquist
* All rights reserved.
*/

#include <sstream>
#include "ConsoleInputExt.hpp"

namespace aki
{
    namespace input
    {
        namespace wincon
        {
            CursorLock::CursorLock()
            {
                delta = { 0, 0 };
                center = { 0, 0 };
                running = false;

                InitializeCriticalSection(&pointCritical);
                InitializeCriticalSection(&runCritical);
                handle = INVALID_HANDLE_VALUE;
            }

            CursorLock::~CursorLock()
            {
                DeleteCriticalSection(&pointCritical);
                DeleteCriticalSection(&runCritical);
                handle = INVALID_HANDLE_VALUE;
            }

            unsigned int __stdcall CursorLock::Run(void* params)
            {
                POINT p;
                CursorLock* cl = (CursorLock*)params;

                // Set the cursor before starting in order to prevent grabbing deltas 
                // from wherever the mouse originated when the lock occured.
                EnterCriticalSection(&cl->pointCritical);
                SetCursorPos(cl->center.x, cl->center.y);
                LeaveCriticalSection(&cl->pointCritical);

                for (;;)
                {
                    GetCursorPos(&p);

                    EnterCriticalSection(&cl->pointCritical);
                    cl->delta.x += p.x - cl->center.x;
                    cl->delta.y += p.y - cl->center.y;

                    SetCursorPos(cl->center.x, cl->center.y);
                    LeaveCriticalSection(&cl->pointCritical);

                    EnterCriticalSection(&cl->runCritical);
                    if (!cl->running) break; // Kill the loop
                    LeaveCriticalSection(&cl->runCritical);
                }
                LeaveCriticalSection(&cl->runCritical);

                return 0;
            }

            bool CursorLock::Start(POINT c)
            {
                EnterCriticalSection(&runCritical);
                if (!running)
                {
                    center = c;
                    running = true;
                    handle = HANDLE(_beginthreadex(0, 0, &CursorLock::Run, (void*)this, 0, 0));

                    if (handle == 0)
                    {
                        std::stringstream sstream;
                        int errNum = errno;
                        int dosErrNum = _doserrno;

                        sstream << "Thread creation failed. (errno: ";
                        sstream << errNum << ") (_doserrno: " << dosErrNum;
                    }
                }
                LeaveCriticalSection(&runCritical);

                return true;
            }

            bool CursorLock::Stop()
            {
                EnterCriticalSection(&runCritical);
                if (running)
                {
                    running = false;
                    LeaveCriticalSection(&runCritical);

                    WaitForSingleObject(handle, INFINITE);

                    if (!CloseHandle(handle))
                    {
                        return false;
                    }

                    handle = INVALID_HANDLE_VALUE;
                }

                return true;
            }

            POINT CursorLock::GetDelta()
            {
                EnterCriticalSection(&pointCritical);
                POINT p;
                p.x = delta.x;
                p.y = delta.y;
                delta.x = 0;
                delta.y = 0;
                LeaveCriticalSection(&pointCritical);

                return p;
            }
        };
    };
};