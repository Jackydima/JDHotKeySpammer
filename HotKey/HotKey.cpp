// HotKey.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include "HotKey.h"
#include <thread>
#include <atomic>


template <typename... Args>
void println(const char* format, Args... args)
{
    std::fprintf(stdout, format, args...);
    std::fprintf(stdout, "\n");
}


std::thread spamThread;
std::atomic<bool> spamActive = false;
std::atomic<bool> injecting =  false;


int main()
{
    if (RegisterHotKey(NULL, HOTKEYID, MOD_ALT | MOD_NOREPEAT, HOTKEY_VALUE) == FALSE)
    {
        println("Fatal error: Was not able to register hotkeys!");
        return 1;
    }

    if (RegisterHotKey(NULL, QUITKEYID, MOD_ALT, QUITKEY_VALUE) == FALSE)
    {
        UnregisterHotKey(NULL, HOTKEYID);
        println("Fatal error: Was not able to register hotkeys!");
        return 1;
    }

    println("Successfully registered Hotkey for 'ALT + E'");

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0))
    {
        if (msg.message == WM_HOTKEY)
        {
            switch (msg.wParam)
            {
            case HOTKEYID:
                if (injecting)
                    break;
                spamActive = !spamActive;
                println("Switched: %d", spamActive.load());
                if (spamActive) {
                    spamThread = std::thread(ExecSpam);
                }
                else
                {
                    if (spamThread.joinable())
                        spamThread.join();
                }
                
                break;
            case QUITKEYID:
                println("Finished Hotkey Program");

                spamActive = false;
                if (spamThread.joinable())
                {
                    spamThread.join();
                }

                UnregisterHotKey(NULL, HOTKEYID);
                UnregisterHotKey(NULL, QUITKEYID);

                return 0;
                break;
            default:
                break;
            }
        }
    }
}

void ExecSpam()
{
    INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));
    while (spamActive)
    {
        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = HOTKEY_VALUE;
        inputs[0].ki.dwFlags = KEYEVENTF_EXTENDEDKEY;

        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = HOTKEY_VALUE;
        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

        injecting = true;
        UINT result = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
        injecting = false;

        if (result == 0)
        {
            result = GetLastError();
            println("Error sending Input: CODE %d", result);
        }
        Sleep(10);
    }
}
