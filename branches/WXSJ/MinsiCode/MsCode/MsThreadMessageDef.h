#pragma once                        //��//��
#ifndef __MSTHREADMESSAGEDEF_H__    //��//��
#define __MSTHREADMESSAGEDEF_H__    //��//��
//��������������������������������������//��
//�ļ����ƣ�MsThreadMessageDef.h        //��
//�����������߳���Ϣ����                //��
//ƽ̨��أ�WINDOWS                     //��
//������������������������������������������

enum E_MSM
{
    MSM_NOTHING = 0,                                // WPARAM 0                     LPARAM 0        ����֪ͨ
    // �ͻ���֪ͨ
    MSM_CLIENT_SOCKET_ON_ENTER  = WM_USER + 0x0001, // WPARAM CMsServerSocket*      LPARAM 0        �ͻ����׽�������֪ͨ    W(�������׽���)     L(0)
    MSM_CLIENT_SOCKET_ON_EXIT,                      // WPARAM CMsServerSocket*      LPARAM 0        �ͻ����׽��ֶϿ�֪ͨ    W(�������׽���)     L(0)

    // ������֪ͨ
    MSM_SERVER_SOCKET_ON_ENTER  = WM_USER + 0x0011, // WPARAM CMsClientSocket*      LPARAM 0        �������׽�������֪ͨ    W(�ͻ����׽���)     L(0)
    MSM_SERVER_SOCKET_ON_EXIT,                      // WPARAM CMsClientSocket*      LPARAM 0        �������׽��ֶϿ�֪ͨ    W(�ͻ����׽���)     L(0)

    // ��ɶ˿�֪ͨ
    MSM_IOCP_UPDATE             = WM_USER + 0x0101, // WPARAM S_IOCP_UPDATE*        LPARAM 0        ��ɶ˿ڸ���֪ͨ        W(����֪ͨ�ṹ��)   L(0)
    MSM_IOCP_EXIT_OR_ERROR,                         // WPARAM ULONG_PTR             LPARAM 0        ��ɶ˿ڶϿ������֪ͨ  W(��ɶ˿�֪ͨKEY)  L(0)
    MSM_IOCP_POST_QUEUED_COMPLETION_STATUS,         // WPARAM CMsSocket*            LPARAM 0        �ⲿͶ��һ�������Ϣ    W(�׽���)           L(0)

    // ��Ϣ�߳�֪ͨ
    MSM_MSG_THREAD_STOP         = WM_USER + 0x0151, // WPARAM 0                     LPARAM 0        ֪ͨ�߳��˳�            W(0)                L(0)
    MSM_MSG_THREAD_ON_RUN,                          // WPARAM 0                     LPARAM 0        ֪ͨ�߳��Ѿ�����        W(0)                L(0)
    MSM_MSG_THREAD_ON_SUSPEND,                      // WPARAM 0                     LPARAM 0        ֪ͨ�߳��Ѿ�����        W(0)                L(0)

    // �׽�����Ϣ֪ͨ
    MSM_MSG_SOCKET_EXECUTE,                         // WPARAM CMsSocket*            LPARAM 0        ֪ͨ�׽�����Ҫִ��      W(�׽���)           L(0)

    // �׽�����Ϣ֪ͨ
    MSM_DLL_NOTIFY_HWND,                            // WPARAM hWnd                  LPARAM 0        ֪ͨ��Ϸƽ̨���ھ��    W(DllGame��Ϸ����)  L(0)
    MSM_DLL_EXIT,                                   // WPARAM HMODULE               LPARAM 0        ֪ͨ��Ϸƽ̨DllGame�˳� W(DllGameʵ�����)  L(0)

    // ������Ϣ
    MSM_INIT_MESSAGE,                               // WPARAM 0                     LPARAM 0        ֪ͨ���ڳ�ʼ��          W(0)                L(0)
    MSM_QUIT_MESSAGE,                               // WPARAM 0                     LPARAM 0        ֪ͨ������Ϣ�߳��˳�    W(0)                L(0)
    MSM_SYS_ICON_MESSAGE,                           // WPARAM 0                     LPARAM 0        ϵͳ������Ϣ֪ͨ        W(0)                L(0)

    // �߳���Ϣ���ֵ
    MSM_MAX_COUNT,                                  // WPARAM 0                     LPARAM 0        ����֪ͨ
};

//����������������������
#endif//#pragma once//��
//����������������������
