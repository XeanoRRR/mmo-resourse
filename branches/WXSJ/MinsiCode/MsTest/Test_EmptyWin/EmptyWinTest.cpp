#include "Precompiled.h"
#include "EmptyWinTest.h"

//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = ::GetDC(hWnd);

	switch (message)
	{
    case WM_CREATE:
        {
            ;
        }break;
    case WM_XBUTTONDOWN:
        {
            ::TextOut(hdc, 0, 0, _T("                                                            "), (INT)_tcslen(_T("                                                            ")));
            if (XBUTTON1 == GET_XBUTTON_WPARAM(wParam))
            {
                ::TextOut(hdc, 0, 0, _T("�����������չ��(һ)"), (INT)_tcslen(_T("�����������չ��(һ)")));
            }
            else if (XBUTTON2 == GET_XBUTTON_WPARAM(wParam))
            {
                ::TextOut(hdc, 0, 0, _T("�����������չ��(��)"), (INT)_tcslen(_T("�����������չ��(��)")));
            }
            else
            {
                ::TextOut(hdc, 0, 0, _T("�����������չ��(δ֪)"), (INT)_tcslen(_T("�����������չ��(δ֪)")));
            }
        }break;
    case WM_XBUTTONUP:
        {
            ::TextOut(hdc, 0, 0, _T("                                                            "), (INT)_tcslen(_T("                                                            ")));
            if (XBUTTON1 == GET_XBUTTON_WPARAM(wParam))
            {
                ::TextOut(hdc, 0, 0, _T("�ɿ��������չ��(һ)"), (INT)_tcslen(_T("�����������չ��(һ)")));
            }
            else if (XBUTTON2 == GET_XBUTTON_WPARAM(wParam))
            {
                ::TextOut(hdc, 0, 0, _T("�ɿ��������չ��(��)"), (INT)_tcslen(_T("�����������չ��(��)")));
            }
            else
            {
                ::TextOut(hdc, 0, 0, _T("�ɿ��������չ��(δ֪)"), (INT)_tcslen(_T("�����������չ��(δ֪)")));
            }
        }break;
    case WM_LBUTTONDOWN:
        {
            ::TextOut(hdc, 0, 0, _T("                                                            "), (INT)_tcslen(_T("                                                            ")));
            ::TextOut(hdc, 0, 0, _T("������������"), (INT)_tcslen(_T("������������")));
        }break;
    case WM_LBUTTONUP:
        {
            ::TextOut(hdc, 0, 0, _T("                                                            "), (INT)_tcslen(_T("                                                            ")));
            ::TextOut(hdc, 0, 0, _T("�ɿ���������"), (INT)_tcslen(_T("�ɿ���������")));
        }break;
    case WM_RBUTTONDOWN:
        {
            ::TextOut(hdc, 0, 0, _T("                                                            "), (INT)_tcslen(_T("                                                            ")));
            ::TextOut(hdc, 0, 0, _T("����������Ҽ�"), (INT)_tcslen(_T("����������Ҽ�")));
        }break;
    case WM_RBUTTONUP:
        {
            ::TextOut(hdc, 0, 0, _T("                                                            "), (INT)_tcslen(_T("                                                            ")));
            ::TextOut(hdc, 0, 0, _T("�ɿ�������Ҽ�"), (INT)_tcslen(_T("�ɿ�������Ҽ�")));
        }break;
    case WM_MBUTTONDOWN:
        {
            ::TextOut(hdc, 0, 0, _T("                                                            "), (INT)_tcslen(_T("                                                            ")));
            ::TextOut(hdc, 0, 0, _T("����������м�"), (INT)_tcslen(_T("����������м�")));
        }break;
    case WM_MBUTTONUP:
        {
            ::TextOut(hdc, 0, 0, _T("                                                            "), (INT)_tcslen(_T("                                                            ")));
            ::TextOut(hdc, 0, 0, _T("�ɿ�������м�"), (INT)_tcslen(_T("�ɿ�������м�")));
        }break;
    case WM_MOUSEWHEEL:
        {
            ::TextOut(hdc, 0, 0, _T("                                                            "), (INT)_tcslen(_T("                                                            ")));
            short nWheel = ((signed short)(HIWORD(wParam))) / WHEEL_DELTA;
            if (nWheel > 0)
            {
                ::TextOut(hdc, 0, 0, _T("�������Ϲ�"), (INT)_tcslen(_T("�������Ϲ�")));
            }
            else if (nWheel < 0)
            {
                ::TextOut(hdc, 0, 0, _T("�������¹�"), (INT)_tcslen(_T("�������¹�")));
            }
            else
            {
                ::TextOut(hdc, 0, 0, _T("������զ��?"), (INT)_tcslen(_T("������զ��?")));
            }
        }break;
    case WM_PAINT:
        {
            hdc = ::BeginPaint(hWnd, &ps);
            ::EndPaint(hWnd, &ps);
        }break;
	case WM_DESTROY:
        {
            ::PostQuitMessage(0);
        }break;
	default:
        return ::DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
