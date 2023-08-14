#include"include/Button.h"
typedef struct DRAWSTRUCT{
    COLORREF borderColor;
    COLORREF textColor;
    COLORREF backColor;
}DRAWSTRUCT;
static void OnPaint(HWND hwnd,DRAWSTRUCT *ds){
    PAINTSTRUCT ps;
    HDC hdc;
    HPEN hPen,hPenOld;
    HBRUSH hBr,hBrOld;
    RECT rect;
    int text_len=0;
    wchar_t *text;
    hdc=BeginPaint(hwnd,&ps);
    hPen=CreatePen(PS_SOLID,1,ds->borderColor);
    hBr=CreateSolidBrush(ds->backColor);
    hPenOld=SelectObject(hdc,hPen);
    hBrOld=SelectObject(hdc,hBr);
    GetClientRect(hwnd,&rect);
    Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);
    //绘制文本
    text_len=GetWindowTextLength(hwnd);
    if(text_len){
        text=calloc(text_len+1,sizeof(wchar_t));
        GetWindowText(hwnd,text,text_len+1);
        SetBkMode(hdc,TRANSPARENT);
        SetTextColor(hdc,ds->textColor);
        DrawText(hdc,text,text_len,&rect,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
        free(text);
    }
    SelectObject(hdc,hPenOld);
    SelectObject(hdc,hBrOld);
    DeleteObject(hPen);
    DeleteObject(hBr);
    EndPaint(hwnd,&ps);
}
HWND CreateButton(HINSTANCE hInst,HWND hParent,RECT rect,int nCmdShow,TCHAR *text){
    DRAWSTRUCT *ds=calloc(1,sizeof(DRAWSTRUCT));
    ds->backColor=RGB(0x73,0x73,0x73);
    ds->textColor=RGB(0xff,0xff,0xff);
    ds->borderColor=RGB(0,0,0);
    HWND hwnd=CreateWidget(hInst,hParent,rect,nCmdShow,text,OnPaint,ds,0);
    return hwnd;
}