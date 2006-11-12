#if OS_WIN

# define WIN32_LEAN_AND_MEAN
# include <windows.h>

// Use WinXP-style controls
# if _MSC_VER >= 1400 // (can't be bothered to implement this for VC7.1...)
#  pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='X86' publicKeyToken='6595b64144ccf1df'\"")
# endif

# define ATLASDLLIMPEXP extern "C" __declspec(dllimport)
#else
# define ATLASDLLIMPEXP extern "C"
#endif

#include "AtlasUI/Misc/DLLInterface.h"

#if OS_WIN
int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPTSTR, int) {
#else
int main() {
#endif
	Atlas_StartWindow(L"FileConverter");
	return 0;
}
