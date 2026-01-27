#include <windows.h>

int WINAPI WinMain(HINSTANCE hIstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){

int result = MessageBox(NULL,"Welcome to the C-playground","Welcome", MB_OK | MB_ICONINFORMATION);

if (result == IDYES){
// clicked yes
}else {
// clicked no
}
return 0;


}
