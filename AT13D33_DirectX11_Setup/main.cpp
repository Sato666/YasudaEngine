//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include    "main.h"
#include    "Application.h"
#include    <assimp/version.h>
#include    <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif

int wmain(int argc, wchar_t** argv, wchar_t** evnp)
{
// メモリリークチェック
#if defined(DEBUG) || defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif//defined(DEBUG) || defined(_DEBUG)

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    // Assimpのバージョンをコンソールに表記
    std::cout << "assimp version ( major,minor,reveision) "
        << aiGetVersionMajor() << ","
        << aiGetVersionMinor() << ","
        << aiGetVersionRevision() << std::endl;

    // アプリケーション実行
    Application app(SCREEN_WIDTH, SCREEN_HEIGHT);
    app.Run();

    return 0;
}
