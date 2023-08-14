#include<Windows.h>
#include<stdlib.h>
#include"include/Widget.h"
// #include"include/Button.h"
#include"include/Edit.h"
void LButtonDown(){
    MessageBox(NULL,TEXT("Hello"),TEXT(""),0);
}
void LButtonDown2(){
    MessageBox(NULL,TEXT("Hello World!"),TEXT(""),0);
}
int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrev,LPSTR lpCmdLine,int nCmdShow){
    RECT wndRect={CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT+500,CW_USEDEFAULT+400};
    RECT btnRect={10,10,100,50};
    RECT edtRect={100,100,400,150};
    RECT edtRect2={100,200,400,250};
    HWND hwnd=CreateWidget(hInst,NULL,wndRect,nCmdShow,TEXT("hELLO"),NULL,NULL,0);
    HWND hEdit=CreateEdit(hInst,hwnd,edtRect,0,NULL);
    HWND hEdit2=CreateEdit(hInst,hwnd,edtRect2,0,TEXT("≤È’“"));
    MSG msg;
    while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}