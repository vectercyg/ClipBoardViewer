/*
 * @Descripttion: 
 * @version: 
 * @Author: cuiyonggan
 * @Date: 2022-07-31 08:46:19
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-07-31 16:08:46
 */
#include <Windows.h>
#include <iostream>
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPresInstance, PSTR szCmdLine, int iCmdShow) {
    // 包含窗口类信息， 用于注册窗口
    WNDCLASSEX wndclassex;
    HWND hwnd;
    MSG msg;
    // 窗口类名，无实意
    TCHAR szAppName[] = TEXT("windows-class-name");
    TCHAR wndName[] =TEXT("window-name");

    wndclassex.cbSize=sizeof(WNDCLASSEXA);
    wndclassex.style=CS_VREDRAW|CS_HREDRAW;
    wndclassex.lpfnWndProc=WndProc;
    wndclassex.cbClsExtra=0;
    wndclassex.cbWndExtra=0;
    wndclassex.hInstance=hInstance;
    wndclassex.hIcon=LoadIcon(hInstance, IDI_APPLICATION);
    wndclassex.hCursor=LoadCursor(NULL, IDI_APPLICATION);;
    wndclassex.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclassex.lpszClassName=szAppName;
    wndclassex.lpszMenuName=NULL;
    wndclassex.hIconSm=NULL;
    

    // 注册窗口类
    if(!RegisterClassEx(&wndclassex)){ return 0; }

    // 获得屏幕的长宽
    int cxScreen=GetSystemMetrics(SM_CXSCREEN);
    int cyScreen=GetSystemMetrics(SM_CYSCREEN);

    // 创建窗口
    hwnd = CreateWindowEx(
        0,
        szAppName,
        wndName,
        WS_OVERLAPPEDWINDOW,
        cxScreen/2,
        0,
        cxScreen/2,
        cyScreen/2,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    // 显示窗口并刷新窗口
    ShowWindow(hwnd,1);
    UpdateWindow(hwnd);
    // 接收信息、翻译处理信息、派送信息
    while (GetMessage(&msg,NULL,0,0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
    static HBITMAP hBitmap;
    HDC hdc,hdcMem;
    PAINTSTRUCT ps;
    RECT rect;
    BITMAP Bitmap;
    switch (message) {
        // 相应创建信息
        case WM_CREATE:
            // 设置子窗口、弹出窗口或顶级窗口的大小、位置和 Z 顺序
            SetWindowPos(hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
            // 设置计时器，最后一个参数指向超时后调用的函数指针，如果为空，则发送一个 WM_TIMER 消息
            SetTimer(hwnd,0,1000,NULL);
            return 0;
        
        case WM_TIMER:
            if (hBitmap) {
                // 删除位图，放置内存泄露
                DeleteObject(hBitmap);
                hBitmap=NULL;
            }
            // 打开剪贴板进行检查并防止其他应用程序修改剪贴板内容
            OpenClipboard(hwnd);
            // 从剪贴板中检索指定格式的数据。剪贴板必须先前已打开。
            hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
            CloseClipboard();
            // InvalidateRect函数将一个矩形添加到指定窗口的更新区域。窗口重绘
            InvalidateRect(hwnd,NULL,false);
            return 0;
        
        // 相应窗口重绘
        case WM_PAINT:
            hdc=BeginPaint(hwnd,&ps);
            if(hBitmap)
            {
                GetClientRect(hwnd,&rect);
                // 检索指定图形对象的信息
                GetObject(hBitmap,sizeof(BITMAP),&Bitmap);
                // 创建兼容性设备绘图句柄
                hdcMem=CreateCompatibleDC(hdc);
                // 为兼容性设备绘图句柄选择位图
                SelectObject(hdcMem,hBitmap);
                // 设置缩放格式
                SetStretchBltMode(hdcMem,HALFTONE);
                // 将位图从源矩形复制到目标矩形
                StretchBlt(hdc,0,0,rect.right,rect.bottom,hdcMem,0,0,Bitmap.bmWidth,Bitmap.bmHeight,SRCCOPY);
                // StretchBlt(hdc,0,0,Bitmap.bmWidth,Bitmap.bmHeight,hdcMem,0,0,Bitmap.bmWidth,Bitmap.bmHeight,SRCCOPY);
                DeleteDC(hdcMem);
            }


            EndPaint(hwnd,&ps);
            return 0;

        // 相应窗口销毁信息
        case WM_DESTROY:
        // 向系统指示线程已请求终止（退出），用于窗口关闭时，结束信息处理循环，防止资源浪费
            PostQuitMessage(0);
            return 0;
        
    }
    // 其余交给默认窗口构成处理程序
    return DefWindowProc(hwnd,message,wParam,lParam);
}
