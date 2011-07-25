#pragma once                    //��//��
#ifndef __MS2DIMAGEDIVIDE_H__   //��//��
#define __MS2DIMAGEDIVIDE_H__   //��//��
//����������������������������������//��
//�ļ����ƣ�Ms2DImageDivide.h       //��
//����������2D���湤��,ͼƬ�ָ�     //��
//ƽ̨��أ�Windows                 //��
//ע    �⣺���༯���� WinRAR ѹ����//��
//          ��, �����ڲ�����ʹ��, ��//��
//          �������κ���ҵ��Ϊ, ��ģ//��
//          �����߱�дĿ�Ľ�Ϊ������//��
//          ��ʹ��, ����ʹ�ñ�ģ����//��
//          �����κ�Υ����Ϊ, �����//��
//          ʹ�������ге�.         //��
//��������������������������������������

//��������������������MS2DEngine����������������
                                            //��
//����������������������������������������������

class CMs2DImageDivide
{
public:
    CMs2DImageDivide(VOID);
    ~CMs2DImageDivide(VOID);

    // ��ͼƬ�ļ�
    BOOL OpenImage(TCHAR* szImageFilePathName);

    // �ر�������Դ
    BOOL CloseAll(VOID);

    // ������ͼƬ�ߴ�
    BOOL SetSubImageSize(NSIZE /*nWidth = INVALID_NID*/, NSIZE /*nHeight = INVALID_NID*/);

    // ���зָ�
    BOOL Divide(BOOL);

    // ����ͼƬ���������
    BOOL SaveSubImagePack(TCHAR* /*szPackFilePathName*/);

private:
    CxImage*    m_pMainImage;       // ��ͼƬ����
    CxImage*    m_pSubImage;        // ��ͼƬ����
    DWORD       m_dwSubImageCount;  // ��ͼƬ����
    DWORD       m_dwSubImageWidth;  // ��ͼƬ���
    DWORD       m_dwSubImageHeight; // ��ͼƬ�߶�
};
//
//void CGameClientView::OnMapConversion()
//{
//    CMsImageSplitManager* m_pCMsImageSplitManager;    // ͼƬ������
//    ACHAR    szFileNameOnly[MAX_PATH] = {0};
//
//    m_pCMsImageSplitManager = new CMsImageSplitManager;
//    if (!m_pCMsImageSplitManager)
//    {
//        ASSERT(m_pCMsImageSplitManager);
//    }
//
//    CFileDialog ImageFileDlg(TRUE, NULL, NULL, 0, "λͼ�ļ�|*.bmp||");
//    ImageFileDlg.DoModal();
//    CString strImageFilePathName = ImageFileDlg.GetPathName();
//    CString strImageFileName = ImageFileDlg.GetFileName();
//    if (0 == strImageFilePathName.GetLength() || 0 == strImageFileName.GetLength())
//    {
//        return;
//    }
//    strcpy_s(szFileNameOnly, MAX_PATH, strImageFileName.GetString());
//
//    m_pCMsImageSplitManager->OpenImageFile(strImageFilePathName.GetString());
//    m_pCMsImageSplitManager->SplitImageFile();
//
//    TCHAR szRarPathName[MAX_PATH] = {0};
//    TCHAR szMapPathName[MAX_PATH] = {0};
//    TCHAR szNewMapPathName[MAX_PATH] = {0};
//    strcpy(szRarPathName, m_szCurrentPath);
//    strcpy(szMapPathName, m_szCurrentPath);
//    strcpy(szNewMapPathName, m_szCurrentPath);
//    strcat(szRarPathName, "/Map/rar");
//    strcat(szMapPathName, "/Map/NewMap/");
//    strcat(szNewMapPathName, "/Map/NewMap.ROmap");
//    ::CreateDirectoryA(szMapPathName, NULL);
//    m_pCMsImageSplitManager->SaveAllSubImage(szMapPathName);
//
//    DWORD dwFileNameLen = strlen(szFileNameOnly);
//    if ('.' == szFileNameOnly[dwFileNameLen-4])
//    {
//        szFileNameOnly[dwFileNameLen-4] = '\0';
//    }
//    else if ('.' == szFileNameOnly[dwFileNameLen-5])
//    {
//        szFileNameOnly[dwFileNameLen-5] = '\0';
//    }
//
//    this->CreateFileFromRes(IDR_WINRAR_EXE, "exe", szRarPathName);
//
//    PROCESS_INFORMATION ProcessInfo;  // ������Ϣ
//    STARTUPINFOA StartInfo;
//    ZeroMemory(&StartInfo, sizeof(StartInfo));
//    StartInfo.cb = sizeof(StartInfo);
//
//    ::SetCurrentDirectoryA(szMapPathName);
//    TCHAR szCurrentDir[MAX_PATH] = {0};
//    ::CreateProcessA(szRarPathName,
//        "-afzip -s -dw -m1 a -r NewMap.ROmap ./NewMap/*.dat",
//        NULL, NULL, FALSE, 0, NULL, NULL, &StartInfo, &ProcessInfo);
//    ::SetCurrentDirectoryA(szCurrentDir);
//    ::WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
//    //::MoveFile("E:/����Ŀ¼/dev/common/tools/Client_Sample/Map/NewMap.ROmap", "E:/����Ŀ¼/dev/common/tools/Client_Sample/Map/World.ROmap");
//    ::DeleteFileA(szRarPathName);
//    //::DeleteFileA(szNewMapPathName);
//
//    HANDLE hIniFile = ::CreateFileA(
//        m_szIniPathName,
//        FILE_READ_DATA,
//        0,
//        NULL,
//        OPEN_EXISTING,
//        0,
//        NULL);
//    if (INVALID_HANDLE_VALUE == hIniFile)
//    {
//        AfxMessageBox(_T("��/Map/World.ini�����ļ�ʧ��!"));
//        return;
//    }
//    else
//    {
//        ::CloseHandle(hIniFile);
//        hIniFile = INVALID_HANDLE_VALUE;
//        ACHAR szTemp[MAX_PATH] = {0};
//
//        m_pCMsImageSplitManager->GetMainImageSize(m_dwMapSizeWidth, m_dwMapSizeHeight);
//        itoa(m_dwMapSizeWidth, szTemp, 10);
//        ::WritePrivateProfileStringA("MapInfo", "MapWidth", szTemp, m_szIniPathName);
//
//        itoa(m_dwMapSizeHeight, szTemp, 10);
//        ::WritePrivateProfileStringA("MapInfo", "MapHeight", szTemp, m_szIniPathName);
//
//        itoa(m_pCMsImageSplitManager->GetSubImageCount(), szTemp, 10);
//        ::WritePrivateProfileStringA("MapInfo", "ImageCount", szTemp, m_szIniPathName);
//
//        itoa(m_pCMsImageSplitManager->GetSubImageCountByX(), szTemp, 10);
//        ::WritePrivateProfileStringA("MapInfo", "ImageCountByX", szTemp, m_szIniPathName);
//
//        itoa(m_pCMsImageSplitManager->GetSubImageCountByY(), szTemp, 10);
//        ::WritePrivateProfileStringA("MapInfo", "ImageCountByY", szTemp, m_szIniPathName);
//
//        DWORD dwSubMapSizeWidth, dwSubMapSizeHeight;
//        ACHAR szItemName[MAX_PATH] = {0};
//        for (DWORD i = 0; i < m_pCMsImageSplitManager->GetSubImageCount(); i++)
//        {
//            m_pCMsImageSplitManager->GetSubImageSize(i, dwSubMapSizeWidth, dwSubMapSizeHeight);
//
//            sprintf_s(szItemName, MAX_PATH, "X_%d", i);
//            itoa(dwSubMapSizeWidth, szTemp, 10);
//            ::WritePrivateProfileStringA("SizeArray", szItemName, szTemp, m_szIniPathName);
//
//            sprintf_s(szItemName, MAX_PATH, "Y_%d", i);
//            itoa(dwSubMapSizeHeight, szTemp, 10);
//            ::WritePrivateProfileStringA("SizeArray", szItemName, szTemp, m_szIniPathName);
//        }
//    }
//
//    if (m_pCMsImageSplitManager)
//    {
//        delete m_pCMsImageSplitManager;
//        m_pCMsImageSplitManager = NULL;
//    }
//}
//
//// ����Դ����ȡ�ļ�
//BOOL CGameClientView::CreateFileFromRes(INT ID, TCHAR* strResType, TCHAR* szResFilePath)
//{
//    // �������ͷ���ֵ
//    BOOL bRet = TRUE;
//
//    // ��Դ���
//    HRSRC hRes = NULL;
//
//    TCHAR szResID[MAX_PATH] = {0};
//    // ��ȡID���ַ�����ʽ
//
//    _stprintf_s(szResID, MAX_PATH, _T("#%d"), ID);
//
//    // ����ID�����Ͳ�����Դ
//    hRes = ::FindResource(::GetModuleHandle(NULL), szResID, strResType);
//    ::GetLastError();
//
//    // װ����Դ
//    HGLOBAL hLoadRes = ::LoadResource(theApp.m_hInstance, hRes);
//
//    // ������Դ
//    LPVOID  lpLockRes = ::LockResource(hLoadRes);
//
//    // ������Դ�ļ�
//    HANDLE hResFile = ::CreateFile(
//        szResFilePath,
//        GENERIC_WRITE,
//        0,
//        NULL,
//        CREATE_ALWAYS,
//        0,
//        NULL
//        );
//
//    // ��ȡ��Դ�ߴ�
//    DWORD nResSize = ::SizeofResource(NULL, hRes);
//
//    // ��Դд���ļ�
//    bRet = ::WriteFile(hResFile, lpLockRes, nResSize, &nResSize, NULL);
//
//    // �ر��ļ����
//    bRet = ::CloseHandle(hResFile);
//    hResFile = NULL;
//
//    // ���سɹ�
//    return TRUE;
//}
//
//void* CGameClientView::LoadZFile(CONST ACHAR* MapFileName, DWORD dwImageIndex, OUT DWORD& dwSize)
//{
//    ACHAR    szName[MAX_PATH];
//    ACHAR    szZipName[MAX_PATH];
//    unzFile zip;
//    unz_file_info file_info;
//    INT     done;
//    BYTE*   ptr;
//
//    _stprintf_s(szName, MAX_PATH, _T("%04d.dat"), dwImageIndex);
//
//    zip = unzOpen(MapFileName);
//    done = unzGoToFirstFile(zip);
//    //done = unzGoToNextFile(zip);
//    for (DWORD i = 0; i < dwImageIndex; i++)
//    {
//        done = unzGoToNextFile(zip);
//        if (done != UNZ_OK)
//        {
//            AfxMessageBox(_T("�������޴��ļ�"));
//            return NULL;
//        }
//    }
//    unzGetCurrentFileInfo(zip, &file_info, szZipName, sizeof(szZipName), NULL, 0, NULL, 0);
//    dwSize = file_info.uncompressed_size;
//    ACHAR* pTemp = strchr(szZipName, '/');
//    if (!pTemp)
//    {
//        return NULL;
//    }
//    pTemp++;
//    if(!strcmp(szName, pTemp))
//    {
//        if(unzOpenCurrentFilePassword(zip, NULL) != UNZ_OK)
//        {
//            unzClose(zip);
//            return NULL;
//        }
//
//        ptr = new BYTE[dwSize];
//        if(!ptr)
//        {
//            unzCloseCurrentFile(zip);
//            unzClose(zip);
//            return NULL;
//        }
//
//        if(unzReadCurrentFile(zip, ptr, dwSize) < 0)
//        {
//            unzCloseCurrentFile(zip);
//            unzClose(zip);
//            delete[] ptr;
//            ptr = NULL;
//            return NULL;
//        }
//        unzCloseCurrentFile(zip);
//        unzClose(zip);
//        return ptr;
//    }
//    else
//    {
//        AfxMessageBox(_T("�����ļ�����"));
//        return NULL;
//    }
//    return NULL;
//}
//
//VOID CGameClientView::FreeZFile(void* pData)
//{
//    if (pData)
//    {
//        delete[] pData;
//    }
//}
//
//void CGameClientView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
//{
//    m_bFreeShow = TRUE;
//    SCROLLINFO ScrollInfo;
//    this->GetScrollInfo(SB_HORZ, &ScrollInfo);
//    switch (nSBCode)
//    {
//    case SB_LINELEFT:
//        {
//            ScrollInfo.nPos--;
//        }break;
//    case SB_LINERIGHT:
//        {
//            ScrollInfo.nPos++;
//        }break;
//    case SB_LEFT:
//        {
//            ScrollInfo.nPos-=10;
//        }break;
//    case SB_RIGHT:
//        {
//            ScrollInfo.nPos+=10;
//        }break;
//    case SB_PAGELEFT:
//        {
//            ScrollInfo.nPos -= m_dwMapSizeWidth/100;
//        }break;
//    case SB_PAGERIGHT:
//        {
//            ScrollInfo.nPos += m_dwMapSizeWidth/100;
//            m_dwMapSizeHeight;
//        }break;
//    case SB_THUMBPOSITION:
//        {
//            ScrollInfo.nPos = nPos;
//        }break;
//    default:
//        {
//            ;
//        }
//    }
//    this->InvalidateRect(NULL, TRUE);
//    this->SetScrollInfo(SB_HORZ, &ScrollInfo);
//}
//
//void CGameClientView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
//{
//    m_bFreeShow = TRUE;
//    SCROLLINFO ScrollInfo;
//    this->GetScrollInfo(SB_VERT, &ScrollInfo);
//    switch (nSBCode)
//    {
//    case SB_LINEUP:
//        {
//            ScrollInfo.nPos--;
//        }break;
//    case SB_LINEDOWN:
//        {
//            ScrollInfo.nPos++;
//        }break;
//    case SB_TOP:
//        {
//            ScrollInfo.nPos-=10;
//        }break;
//    case SB_BOTTOM:
//        {
//            ScrollInfo.nPos+=10;
//        }break;
//    case SB_PAGEUP:
//        {
//            ScrollInfo.nPos -= m_dwMapSizeWidth/100;
//        }break;
//    case SB_PAGEDOWN:
//        {
//            ScrollInfo.nPos += m_dwMapSizeWidth/100;
//            m_dwMapSizeHeight;
//        }break;
//    case SB_THUMBPOSITION:
//        {
//            ScrollInfo.nPos = nPos;
//        }break;
//    default:
//        {
//            ;
//        }
//    }
//    this->InvalidateRect(NULL, TRUE);
//    this->SetScrollInfo(SB_VERT, &ScrollInfo);
//}
//
//BOOL CGameClientView::InitSubMap(VOID)
//{
//    HANDLE hIniFile = ::CreateFileA(
//        m_szIniPathName,
//        FILE_READ_DATA,
//        0,
//        NULL,
//        OPEN_EXISTING,
//        0,
//        NULL);
//    if (INVALID_HANDLE_VALUE == hIniFile)
//    {
//        AfxMessageBox(_T("��/Map/World.ini�����ļ�ʧ��!"));
//        return FALSE;
//    }
//    else
//    {
//        ::CloseHandle(hIniFile);
//        hIniFile = INVALID_HANDLE_VALUE;
//
//        ACHAR szTemp[MAX_PATH] = {0};
//
//        // ��ȡͼƬ����
//        if(::GetPrivateProfileStringA("MapInfo", "ImageCount", _T(""), szTemp, MAX_PATH, m_szIniPathName))
//        {
//            m_dwImageCount = atoi(szTemp);
//        }
//        else
//        {
//            AfxMessageBox(_T("��ȡ/Map/World.ini->MapInfo->ImageCountʧ��!"));
//            return FALSE;
//        }
//
//        // ��ȡ�����������
//        if(::GetPrivateProfileStringA("MapInfo", "MapWidth", _T(""), szTemp, MAX_PATH, m_szIniPathName))
//        {
//            m_dwMapSizeWidth = atoi(szTemp);
//        }
//        else
//        {
//            AfxMessageBox(_T("��ȡ/Map/World.ini->MapInfo->MapWidthʧ��!"));
//            return FALSE;
//        }
//
//        // ��ȡ���������߶�
//        if(::GetPrivateProfileStringA("MapInfo", "MapHeight", _T(""), szTemp, MAX_PATH, m_szIniPathName))
//        {
//            m_dwMapSizeHeight = atoi(szTemp);
//        }
//        else
//        {
//            AfxMessageBox(_T("��ȡ/Map/World.ini->MapInfo->MapHeightʧ��!"));
//            return FALSE;
//        }
//
//        // ��ȡ����ͼƬ����
//        if(::GetPrivateProfileStringA("MapInfo", "ImageCountByX", _T(""), szTemp, MAX_PATH, m_szIniPathName))
//        {
//            m_dwSubImageCountByX = atoi(szTemp);
//        }
//        else
//        {
//            AfxMessageBox(_T("��ȡ/Map/World.ini->MapInfo->ImageCountByXʧ��!"));
//            return FALSE;
//        }
//
//        // ��ȡ����ͼƬ����
//        if(::GetPrivateProfileStringA("MapInfo", "ImageCountByX", _T(""), szTemp, MAX_PATH, m_szIniPathName))
//        {
//            m_dwSubImageCountByY = atoi(szTemp);
//        }
//        else
//        {
//            AfxMessageBox(_T("��ȡ/Map/World.ini->MapInfo->ImageCountByXʧ��!"));
//            return FALSE;
//        }
//
//        m_lpSizeArray = new SIZE[m_dwImageCount];
//        if (!m_lpSizeArray)
//        {
//            AfxMessageBox(_T("�ڴ治��"));
//            return FALSE;
//        }
//        for (DWORD i = 0; i < m_dwImageCount; i++)
//        {
//            ACHAR szItemName[MAX_PATH] = {0};
//            sprintf_s(szItemName, MAX_PATH, "X_%d", i);
//            // ��ȡ����ͼƬ����
//            if(::GetPrivateProfileStringA("SizeArray", szItemName, _T(""), szTemp, MAX_PATH, m_szIniPathName))
//            {
//                m_lpSizeArray[i].cx = atoi(szTemp);
//            }
//            else
//            {
//                ::MessageBoxA(m_hWnd, "��ȡ/Map/World.ini->SizeArrayʧ��!", szItemName, MB_OK);
//                return FALSE;
//            }
//
//            sprintf_s(szItemName, MAX_PATH, "Y_%d", i);
//            // ��ȡ����ͼƬ����
//            if(::GetPrivateProfileStringA("SizeArray", szItemName, _T(""), szTemp, MAX_PATH, m_szIniPathName))
//            {
//                m_lpSizeArray[i].cy = atoi(szTemp);
//            }
//            else
//            {
//                ::MessageBoxA(m_hWnd, "��ȡ/Map/World.ini->SizeArrayʧ��!", szItemName, MB_OK);
//                return FALSE;
//            }
//        }
//    }
//
//    SCROLLINFO ScrollInfo;
//    this->GetScrollInfo(SB_VERT, &ScrollInfo);
//    ScrollInfo.nMax = m_dwMapSizeHeight;
//    this->SetScrollInfo(SB_VERT, &ScrollInfo);
//
//    this->GetScrollInfo(SB_HORZ, &ScrollInfo);
//    ScrollInfo.nMax = m_dwMapSizeWidth;
//    this->SetScrollInfo(SB_HORZ, &ScrollInfo);
//
//
//    m_pbmpMap = new CBitmapEx[m_dwImageCount];
//    if (!m_pbmpMap)
//    {
//        ASSERT(m_pbmpMap);
//    }
//
//    DWORD dwImageSize;
//    VOID* pImage = NULL;
//    TCHAR szMapPathName[MAX_PATH] = {0};
//    strcpy(szMapPathName, m_szCurrentPath);
//    strcat(szMapPathName, "/Map/World.Romap");
//
//    for (DWORD i = 0; i < m_dwImageCount; i++)
//    {
//        VOID* pImage = this->LoadZFile(szMapPathName, i, dwImageSize);
//        if (pImage)
//        {
//            IPicture* pPicture = m_pbmpMap[i].LoadFromBuffer((BYTE*)pImage, dwImageSize);
//            pPicture->get_Handle((OLE_HANDLE*)&m_pbmpMap[i].m_hObject);
//            this->FreeZFile(pImage);
//            pImage = NULL;
//        }
//    }
//    return TRUE;
//}
//
//
//#pragma once                        //��//��
//#ifndef __MSIMAGESPLITMANAGER_H__   //��//��
//#define __MSIMAGESPLITMANAGER_H__   //��//��
////��������������������������������������//��
////�ļ����ƣ�MsImageSplitManager.h       //��
////����������ͼƬ��ֹ�����              //��
////ƽ̨��أ�WINDOWS                     //��
////������������������������������������������
//
//// �ӳ���ͼƬ��С�ߴ�
//#define SUB_MAP_LEAST_SIZE  400
//
//class CMsImageSplitManager
//{
//public:
//
//    // ���졢����
//    CMsImageSplitManager(DWORD dwSubMapSize = SUB_MAP_LEAST_SIZE);
//    virtual ~CMsImageSplitManager(VOID);
//
//    BOOL    OpenImageFile(CONST TCHAR* szMapFileName);    // ��ͼƬ
//    //VOID    ZoomOutMainImage(FLOAT fZoom);          // �Ŵ���ͼƬ
//    DWORD   SplitImageFile(VOID);                   // ���ͼƬ
//    VOID    CloseAllImage(VOID);                    // �ر�����ͼƬ
//    DWORD   GetSplitImageSize(VOID){return m_dwSubImageSize;}
//    VOID    GetMainImageSize(DWORD& dwWidth, DWORD& dwHeight);
//    VOID    SaveAllSubImage(TCHAR* szFilePath);
//    DWORD   GetSubImageCount(VOID){return m_dwSubImageCount;}
//    DWORD   GetSubImageCountByX(VOID){return m_dwSubImageCountByX;}
//    DWORD   GetSubImageCountByY(VOID){return m_dwSubImageCountByY;}
//    VOID    GetSubImageSize(DWORD dwIndex, DWORD& dwWidth, DWORD& dwHeight);
//
//    // ������ͼƬָ������Ŀ��ͼƬָ��λ��
//    BOOL    CopyImageInMainImageRect(CxImage* pDesImage, LPRECT lpSrcRc, LONG X, LONG Y, BOOL bCopyAlpha = TRUE);
//
//    // ����ͼƬ
//    BOOL DrawMainImage(HDC hDc, LONG X, LONG Y);
//
//    // ������������ͼƬ
//    BOOL DrawSubImage(HDC hDc, DWORD dwIndex, LONG X, LONG Y);
//
//private:
//
//    CxImage*    m_pMainImage;           // ��ͼƬ����
//    CxImage*    m_pSubImage;            // ��ͼƬ����
//    LPSIZE      m_lpSizeArray;          // ÿ����ͼƬ�ߴ�
//    DWORD       m_dwSubImageSize;       // ��ͼƬ�ߴ�
//    DWORD       m_dwSubImageCountByX;   // ÿ��������ͼƬ����
//    DWORD       m_dwSubImageCountByY;   // ÿ��������ͼƬ����
//    DWORD       m_dwSubImageCount;      // ��ͼƬ����
//};
//
//#include "StdAfx.h"
//#include "xImage/ximage.h"
//#include "MsImageSplitManager.h"
//
//CMsImageSplitManager::CMsImageSplitManager(DWORD dwSubMapSize)
//: m_pSubImage               (NULL)
//, m_dwSubImageSize          (dwSubMapSize)
//, m_dwSubImageCount         (0)
//, m_lpSizeArray             (NULL)
//{
//    if (dwSubMapSize < SUB_MAP_LEAST_SIZE)
//    {
//        dwSubMapSize = SUB_MAP_LEAST_SIZE;
//    }
//    m_pMainImage = new CxImage;
//    if (!m_pMainImage)
//    {
//        ::MessageBox(::GetActiveWindow(), _T("�ڴ治��"), _T("ʧ��"), MB_OK);
//    }
//}
//
//CMsImageSplitManager::~CMsImageSplitManager(VOID)
//{
//
//    this->CloseAllImage();
//    if (m_pMainImage)
//    {
//        m_pMainImage->Destroy();
//        delete m_pMainImage;
//        m_pMainImage = NULL;
//    }
//}
//
//// ��ͼƬ
//BOOL CMsImageSplitManager::OpenImageFile(CONST TCHAR* szMapFileName)
//{
//    BOOL bRet = m_pMainImage->Load(szMapFileName, CXIMAGE_SUPPORT_BMP);
//    if (!bRet)
//    {
//        ::MessageBox(::GetActiveWindow(), szMapFileName, _T("�򿪳���ͼƬʧ��"), MB_OK);
//    }
//    return bRet;
//}
//
////// �Ŵ���ͼƬ
////VOID CMsImageSplitManager::ZoomOutMainImage(FLOAT fZoom)
////{
////    CxImage* pImage = new CxImage;
////    if (!pImage)
////    {
////        ::MessageBox(::GetActiveWindow(), _T("�ڴ治��"), _T("ʧ��"), MB_OK);
////    }
////
////    DWORD dwDesWidth = (DWORD)(m_pMainImage->GetWidth() * fZoom);
////    DWORD dwDesHeight = (DWORD)(m_pMainImage->GetHeight() * fZoom);
////    pImage->Create(dwDesWidth, dwDesHeight, m_pMainImage->GetBpp(), m_pMainImage->GetType());
////    pImage->Save(_T("{884e200c-217d-11da-b2a4-000e7bbb2b09-884e200b-217d-11da-b2a4-000e7bbb2b09}"), CXIMAGE_FORMAT_BMP);
////    delete pImage;
////    pImage = NULL;
////
////    HANDLE hImage = ::LoadImage(NULL,
////        _T("{884e200c-217d-11da-b2a4-000e7bbb2b09-884e200b-217d-11da-b2a4-000e7bbb2b09}"),
////        IMAGE_BITMAP,
////        LR_DEFAULTSIZE,
////        LR_DEFAULTSIZE,
////        LR_LOADFROMFILE
////        );
////
////    HWND hWnd = ::GetActiveWindow();
////    HDC hMainDc = ::GetDC(hWnd);
////    HDC hDc = ::CreateCompatibleDC(hMainDc);
////    ::SelectObject(hDc, hImage);
////    m_pMainImage->Draw(hDc, 0, 0, dwDesWidth, dwDesHeight);
////
////    //m_pMainImage->Draw()
////
////    delete m_pMainImage;
////    m_pMainImage = NULL;
////    m_pMainImage = new CxImage;
////    if (!m_pMainImage)
////    {
////        ::MessageBox(::GetActiveWindow(), _T("�ڴ治��"), _T("ʧ��"), MB_OK);
////    }
////    m_pMainImage->CreateFromHANDLE(hDc);
////
////    ::DeleteDC(hDc);
////    ::ReleaseDC(hWnd, hMainDc);
////}
//
//// ���ͼƬ
//DWORD CMsImageSplitManager::SplitImageFile(VOID)
//{
//    //if (fZoom != 1.0f)
//    //{
//    //    this->ZoomOutMainImage(fZoom);
//    //}
//
//    // ��ȡ�ָ�������ͼƬ����
//    m_dwSubImageCountByX = m_pMainImage->GetWidth() / m_dwSubImageSize + 1;
//    
//    // ��ȡ�ָ��������ͼƬ����
//    m_dwSubImageCountByY = m_pMainImage->GetHeight() / m_dwSubImageSize + 1;
//
//    // ������ͼƬ�ܸ���
//    m_dwSubImageCount = m_dwSubImageCountByX * m_dwSubImageCountByY;
//
//    // ����������һ����ͼƬ��Ҫ��ʾ�Ŀ��
//    DWORD dwLastSubImageWidthByX    = SUB_MAP_LEAST_SIZE - (m_dwSubImageCountByX * SUB_MAP_LEAST_SIZE - m_pMainImage->GetWidth());
//    
//    // �����������һ����ͼƬ��Ҫ��ʾ�ĸ߶�
//    DWORD dwLastSubImageHeightByY   = SUB_MAP_LEAST_SIZE - (m_dwSubImageCountByY * SUB_MAP_LEAST_SIZE - m_pMainImage->GetHeight());
//
//    // ������ͼƬ
//    m_pSubImage = new CxImage[m_dwSubImageCount];
//    if (!m_pSubImage)
//    {
//        ::MessageBox(::GetActiveWindow(), _T("�����ڴ�ʧ��"), _T("�ڴ治��"), MB_OK);
//        return -1;
//    }
//
//    // ������ͼƬƫ������
//    m_lpSizeArray = new SIZE[m_dwSubImageCount];
//    if (!m_lpSizeArray)
//    {
//        ::MessageBox(::GetActiveWindow(), _T("�����ڴ�ʧ��"), _T("�ڴ治��"), MB_OK);
//        return -1;
//    }
//
//    // ��ȡͼƬ��ɫ���
//    DWORD dwImageBpp    = m_pMainImage->GetBpp();
//    DWORD dwImageType   = m_pMainImage->GetType();
//
//
//    // ��ȡ
//    for (DWORD dwIndex = 0; dwIndex < m_dwSubImageCount; dwIndex++)
//    {
//        DWORD dwCurrentSubImageWidth = m_dwSubImageSize;
//        DWORD dwCurrentSubImageHeight = m_dwSubImageSize;
//
//        // ��������һ����ͼƬ
//        if ((dwIndex+1) == m_dwSubImageCount)
//        {
//            // ������ͼƬ
//            m_pSubImage[dwIndex].Create(dwLastSubImageWidthByX, dwLastSubImageHeightByY, dwImageBpp, dwImageType);
//            dwCurrentSubImageWidth = dwLastSubImageWidthByX;
//            dwCurrentSubImageHeight = dwLastSubImageHeightByY;
//        }
//
//        // ����Ǻ������һ����ͼƬ
//        else if (0 != dwIndex && 0 == (dwIndex+1)%(m_dwSubImageCountByX))
//        {
//            // ������ͼƬ
//            m_pSubImage[dwIndex].Create(dwLastSubImageWidthByX, m_dwSubImageSize, dwImageBpp, dwImageType);
//            dwCurrentSubImageWidth = dwLastSubImageWidthByX;
//        }
//
//        // ������������һ����ͼƬ
//        else if ((dwIndex+1) > ((m_dwSubImageCountByY - 1) * m_dwSubImageCountByX))
//        {
//            // ������ͼƬ
//            m_pSubImage[dwIndex].Create(m_dwSubImageSize, dwLastSubImageHeightByY, dwImageBpp, dwImageType);
//            dwCurrentSubImageHeight = dwLastSubImageHeightByY;
//        }
//
//        // ������ͼƬ
//        else
//        {
//            // ������ͼƬ
//            m_pSubImage[dwIndex].Create(m_dwSubImageSize, m_dwSubImageSize, dwImageBpp, dwImageType);
//        }
//
//
//        m_lpSizeArray[dwIndex].cx = dwCurrentSubImageWidth;
//        m_lpSizeArray[dwIndex].cy = dwCurrentSubImageHeight;
//
//        RECT  rc = {0,0,0,0};
//        rc.left     = (dwIndex%m_dwSubImageCountByX)*m_dwSubImageSize;
//        rc.top      = (dwIndex/m_dwSubImageCountByX)*m_dwSubImageSize;
//        rc.right    = rc.left + dwCurrentSubImageWidth;
//        rc.bottom   = rc.top  + dwCurrentSubImageHeight;
//        this->CopyImageInMainImageRect(&m_pSubImage[dwIndex], &rc, 0, 0, FALSE);
//    }
//    // ������ͼƬ����
//    return m_dwSubImageCount;
//}
//
//// �ر�����ͼƬ
//VOID CMsImageSplitManager::CloseAllImage(VOID)
//{
//    // �ͷ���ͼƬ����
//    if (m_pSubImage)
//    {
//        for (DWORD i = 0; i < m_dwSubImageCount; i++)
//        {
//            m_pSubImage[i].Destroy();
//
//        }
//        delete[] m_pSubImage;
//        m_pSubImage = NULL;
//    }
//
//    // �ͷ�ƫ������
//    if (m_lpSizeArray)
//    {
//        delete[] m_lpSizeArray;
//        m_lpSizeArray = NULL;
//    }
//}
//
//// ����ͼƬ
//BOOL CMsImageSplitManager::DrawMainImage(HDC hDc, LONG X, LONG Y)
//{
//    // ����ͼƬ
//    //FLOAT fDrawCX = fZoom * m_pMainImage->GetWidth();
//    //FLOAT fDrawCY = fZoom * m_pMainImage->GetHeight();
//    INT nRet = m_pMainImage->Draw(hDc, X, Y);
//    if (nRet)
//    {
//        return TRUE;
//    }
//    else
//    {
//        return FALSE;
//    }
//}
//
//// ������������ͼƬ
//BOOL CMsImageSplitManager::DrawSubImage(HDC hDc, DWORD dwIndex, LONG X, LONG Y)
//{
//    if (dwIndex >= m_dwSubImageCount)
//    {
//        ::MessageBox(::GetActiveWindow(), _T("ָ������ͼƬ������"), _T("��ʾ"), MB_OK);
//        return FALSE;
//    }
//    if (!m_pSubImage)
//    {
//        ::MessageBox(::GetActiveWindow(), _T("���ô���"), _T("��ʾ"), MB_OK);
//        return FALSE;
//    }
//
//    //LONG lDrawCX = /*fZoom * */m_pSubImage[dwIndex].GetWidth();
//    //LONG lDrawCY = /*fZoom * */m_pSubImage[dwIndex].GetHeight();
//
//    
//    LONG lRealDrawX = m_lpSizeArray[dwIndex].cx/* * fZoom*/;
//    LONG lRealDrawY = m_lpSizeArray[dwIndex].cy/* * fZoom*/;
//
//    INT nRet = m_pSubImage[dwIndex].Draw(hDc, X+lRealDrawX, Y+lRealDrawY);
//
//    if (nRet)
//    {
//        return TRUE;
//    }
//    else
//    {
//        return FALSE;
//    }
//}
//
//// ������ͼƬָ������Ŀ��ͼƬָ��λ��
//BOOL CMsImageSplitManager::CopyImageInMainImageRect(CxImage* pDesImage, LPRECT lpSrcRc, LONG DesX, LONG DesY, BOOL bCopyAlpha)
//{
//    if (!pDesImage)
//    {
//        return FALSE;
//    }
//    DWORD dwMainImageWidth = m_pMainImage->GetWidth();        // ��ͼƬ���
//    DWORD dwMainImageHeight = m_pMainImage->GetHeight();      // ��ͼƬ�߶�
//    DWORD dwSrcBeginX = lpSrcRc->left;                      // ת����ͼƬ��ʼλ��X
//    DWORD dwSrcBeginY = dwMainImageHeight - lpSrcRc->bottom;// ת����ͼƬ��ʼλ��Y
//    DWORD dwCopyWidth = lpSrcRc->right - lpSrcRc->left;     // ���ƿ��
//    DWORD dwCopyHeight = lpSrcRc->bottom - lpSrcRc->top;    // ���Ƹ߶�
//
//    RGBQUAD rgba;
//    for (DWORD x = 0; x < dwCopyWidth; x++)
//    {
//        for (DWORD y = 0; y < dwCopyHeight; y++)
//        {
//            rgba = m_pMainImage->GetPixelColor(dwSrcBeginX+x, dwSrcBeginY+y, bCopyAlpha?true:false);
//            pDesImage->SetPixelColor(DesX+x, DesY+y, rgba, bCopyAlpha?true:false);
//        }
//    }
//    return TRUE;
//}
//
//VOID CMsImageSplitManager::GetMainImageSize(DWORD& dwWidth, DWORD& dwHeight)
//{
//    dwWidth = m_pMainImage->GetWidth();
//    dwHeight = m_pMainImage->GetHeight();
//}
//
//VOID CMsImageSplitManager::SaveAllSubImage(TCHAR* szFilePath)
//{
//    TCHAR szSubImageName[MAX_PATH] = {0};
//    for (DWORD i = 0; i < m_dwSubImageCount; i++)
//    {
//        _stprintf_s(szSubImageName, "%s%04d.dat", szFilePath, i);
//        m_pSubImage[i].Save(szSubImageName, CXIMAGE_FORMAT_BMP);
//    }
//}
//
//VOID CMsImageSplitManager::GetSubImageSize(DWORD dwIndex, DWORD& dwWidth, DWORD& dwHeight)
//{
//    dwWidth = m_lpSizeArray[dwIndex].cx;
//    dwHeight = m_lpSizeArray[dwIndex].cy;
//}

//����������������������
#endif//#pragma once  ��
//����������������������

