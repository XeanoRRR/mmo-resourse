================================================================================
    MICROSOFT �������: RrMMOLauncherServer ��Ŀ����
===============================================================================

Ӧ�ó�������Ϊ�������� RrMMOLauncherServer Ӧ�ó��򡣴�Ӧ�ó��򲻽���ʾʹ�� Microsoft ������Ļ���֪ʶ�����ҿ���Ϊ��дӦ�ó������㡣

���ļ�������� RrMMOLauncherServer Ӧ�ó���ĸ����ļ�������ժҪ��

RrMMOLauncherServer.vcproj
    ����ʹ��Ӧ�ó��������ɵ� VC++ ��Ŀ������Ŀ�ļ���
    �������й������ļ��� Visual C++ �汾����Ϣ���Լ��й�ʹ��Ӧ�ó�����ѡ���ƽ̨�����ú���Ŀ���ܵ���Ϣ��

RrMMOLauncherServer.h
    ����Ӧ�ó������Ҫͷ�ļ���������������Ŀ�ض���ͷ�ļ�(���� Resource.h)�������� CRrMMOLauncherServerApp Ӧ�ó����ࡣ

RrMMOLauncherServer.cpp
    ���ǰ���Ӧ�ó����� CRrMMOLauncherServerApp ����ҪӦ�ó���Դ�ļ���

RrMMOLauncherServer.rc
    ���ǳ���ʹ�õ����� Microsoft Windows ��Դ���б������� RES ��Ŀ¼�д洢��ͼ�ꡢλͼ�͹�ꡣ���ļ�����ֱ���� Microsoft Visual C++ �н��б༭����Ŀ��Դλ�� 2052 �С�

res\RrMMOLauncherServer.ico
    ��������Ӧ�ó���ͼ���ͼ���ļ�����ͼ���������Ҫ��Դ�ļ� RrMMOLauncherServer.rc �С�

res\RrMMOLauncherServer.rc2
    ���ļ����������� Microsoft Visual C++ �༭����Դ����Ӧ�ý���������Դ�༭���༭��������Դ���ڴ��ļ��С�

RrMMOLauncherServer.reg
    ����һ��ʾ�� .reg �ļ�������ʾ�˿�ܽ�Ϊ�����õ�ע�����õ����ࡣ���Խ�������
    ��������Ӧ�ó���һ��ʹ�õ� .reg �ļ����������Խ���ɾ����������Ĭ�ϵ� RegisterShellFileTypes ע�ᡣ

RrMMOLauncherServer.idl
    ���ļ�����Ӧ�ó������Ϳ�Ľӿ���������Դ���롣

/////////////////////////////////////////////////////////////////////////////

��������ܴ���:
    ��Ŀ������׼ MFC ���档

MainFrm.h, MainFrm.cpp
    ��Щ�ļ���������� CMainFrame������������
    CMDIFrameWnd ���������� MDI ��ܹ��ܡ�

res\Toolbar.bmp
    ��λͼ�ļ�����Ϊ����������ƽ��ͼ��
    ��ʼ��������״̬������ CMainFrame ���й���ġ�ʹ����Դ�༭���༭�˹�����λͼ�������� RrMMOLauncherServer.rc �е� IDR_MAINFRAME TOOLBAR ��������ӹ�������ť��
/////////////////////////////////////////////////////////////////////////////

�����ӿ�ܴ���:

ChildFrm.h, ChildFrm.cpp
    ��Щ�ļ����岢ʵ�� CChildFrame �࣬����֧�� MDI Ӧ�ó����е��Ӵ��ڡ�

/////////////////////////////////////////////////////////////////////////////

Ӧ�ó����򵼴���һ���ĵ����ͺ�һ����ͼ:

RrMMOLauncherServerDoc.h��RrMMOLauncherServerDoc.cpp - �ĵ�
    ��Щ�ļ����� CRrMMOLauncherServerDoc �ࡣ�༭��Щ�ļ�������������ĵ����ݲ���ʵ���ļ�����ͼ���(ͨ�� CRrMMOLauncherServerDoc::Serialize)��

RrMMOLauncherServerView.h��RrMMOLauncherServerView.cpp - �ĵ�����ͼ
    ��Щ�ļ����� CRrMMOLauncherServerView �ࡣ
    CRrMMOLauncherServerView �������ڲ鿴 CRrMMOLauncherServerDoc ����

res\RrMMOLauncherServerDoc.ico
    ����ͼ���ļ��������� CRrMMOLauncherServerDoc ��� MDI �Ӵ��ڵ�ͼ�ꡣ��ͼ���������Ҫ��Դ�ļ� RrMMOLauncherServer.rc �С�




/////////////////////////////////////////////////////////////////////////////

��������:

ActiveX �ؼ�
    Ӧ�ó��������ʹ�� ActiveX �ؼ���֧�֡�

��ӡ����ӡԤ��֧��
    Ӧ�ó�������ͨ���� MFC ����� CView ���еĳ�Ա���������������ڴ����ӡ����ӡ���úʹ�ӡԤ������Ĵ��롣

MAPI ֧��
    ���ɵ���Ŀ��������������������ʹ洢�ʼ�����Ĵ��롣

Windows Sockets
    Ӧ�ó���֧��ͨ�� TCP/IP ���罨��ͨ�š�

/////////////////////////////////////////////////////////////////////////////

������׼�ļ�:

StdAfx.h, StdAfx.cpp
    ��Щ�ļ�����������Ϊ RrMMOLauncherServer.pch ��Ԥ����ͷ(PCH)�ļ�����Ϊ StdAfx.obj ��Ԥ���������ļ���

Resource.h
    ���Ǳ�׼ͷ�ļ�������������Դ ID��
    Microsoft Visual C++ ����ȡ�����´��ļ���

/////////////////////////////////////////////////////////////////////////////

����ע��:

Ӧ�ó�����ʹ�á�TODO:��ָʾӦ��ӻ��Զ����Դ���벿�֡�

���Ӧ�ó����ڹ��� DLL ��ʹ�� MFC,����Ҫ���·��� MFC DLL�����Ӧ�ó������������ϵͳ���������ò�ͬ����Ҳ���������·�����Ӧ�ı��ػ���Դ MFC90XXX.DLL��
�й��������������ϸ��Ϣ������� MSDN �ĵ����й����·��� Visual C++ Ӧ�ó���Ĳ��֡�

/////////////////////////////////////////////////////////////////////////////