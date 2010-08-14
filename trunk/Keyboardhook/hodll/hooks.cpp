#include "hooks.h"

__declspec(dllexport) LRESULT CALLBACK KeyboardProc(
                            int nCode, 
                            WPARAM wParam, 
                            LPARAM lParam)
{
	MessageBox(0,"ff","ff",MB_OK);

	return 0;
}
