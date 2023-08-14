#pragma once
#include<Windows.h>
HWND CreateWidget(HINSTANCE hInst,HWND hParent,RECT rect,int nCmdShow,TCHAR *text,void(*onPaint)(HWND hwnd,void *args),void* args,void* lpParams);
void setBackColor(HWND hwnd,COLORREF color);
void setTextColor(HWND hwnd,COLORREF color);
void setBorderColor(HWND hwnd,COLORREF color);
void setText(HWND hwnd,LPWSTR lpString);
void eventRegister(HWND hwnd,int event,void (*callback)(HWND hwnd,void *args,WPARAM wParam,LPARAM lParam),void *args);