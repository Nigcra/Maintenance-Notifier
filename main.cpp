// 2025-07-07
// Zweck: Benachrichtigungstool zur Anzeige einer Laufzeitmeldung bei Installationen.
// Statische EXE mit Sciter, ohne Abhängigkeiten zu externen Runtimes.
// Text ist über Parameter übergebbar, Logo ist eingebettet.

#include "sciter-x.h"
#include "sciter-x-window.hpp"
#include <windows.h>
#include <string>

#define IDR_LOGO_PNG 101

class NotificationWindow : public sciter::window {
public:
    NotificationWindow(const std::wstring& message)
        : sciter::window(SW_MAIN | SW_TITLEBAR) {

        std::wstring html = LR"(
            <html>
              <head>
                <style>
                  body { font-family: sans-serif; text-align: center; background: #f0f0f0; padding: 50px; }
                  img { width: 128px; height: 128px; }
                  .text { margin-top: 20px; font-size: 18pt; color: #333; }
                </style>
              </head>
              <body>
                <img src="res:logo.png" />
                <div class="text" id="msg">[TEXT]</div>
              </body>
            </html>
        )";

        size_t pos = html.find(L"[TEXT]");
        if (pos != std::wstring::npos)
            html.replace(pos, 6, message);

        load_html((unsigned char*)html.c_str(), html.length() * sizeof(wchar_t), true);
    }
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    std::wstring message = L"Bitte den PC während der Installation nicht ausschalten.";
    if (argc > 1)
        message = argv[1];

    sciter::archive arc;
    arc.open();

    HRSRC res = FindResource(NULL, MAKEINTRESOURCE(IDR_LOGO_PNG), RT_RCDATA);
    if (res) {
        HGLOBAL hData = LoadResource(NULL, res);
        DWORD size = SizeofResource(NULL, res);
        LPVOID data = LockResource(hData);
        arc.add_data("res:logo.png", data, size);
    }

    sciter::archive::set_global(arc);

    NotificationWindow wnd(message);
    wnd.expand();
    wnd.run();

    return 0;
}
