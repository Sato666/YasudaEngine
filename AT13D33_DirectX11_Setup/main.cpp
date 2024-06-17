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
// ���������[�N�`�F�b�N
#if defined(DEBUG) || defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif//defined(DEBUG) || defined(_DEBUG)

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    // Assimp�̃o�[�W�������R���\�[���ɕ\�L
    std::cout << "assimp version ( major,minor,reveision) "
        << aiGetVersionMajor() << ","
        << aiGetVersionMinor() << ","
        << aiGetVersionRevision() << std::endl;

    // �A�v���P�[�V�������s
    Application app(SCREEN_WIDTH, SCREEN_HEIGHT);
    app.Run();

    return 0;
}
