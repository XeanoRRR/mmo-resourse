#ifndef SDOA4CLIENT_H
#define SDOA4CLIENT_H


#include <objbase.h>
#include "windows.h"

#pragma pack(push,1)

// SDOA_SDK_VERSION ��ʾһ�׽����SDK�汾��
#define SDOA_SDK_VERSION                 0x0200

// SDOA ����ֵ
#define SDOA_OK                      0
#define SDOA_FALSE                   1

// �ڴ治��
#define SDOA_INSUFFICIENT_BUFFER     122  

// SDOA��ʼ�����󷢳�����Ϣ
#define SDOA_WM_CLIENT_RUN			0xA604// WM_USER + 0xA204

// ��Ϸͼ����������
#define SDOA_RENDERTYPE_UNKNOWN             0x00
#define SDOA_RENDERTYPE_GDI                 0x01
#define SDOA_RENDERTYPE_DDRAW               0x04
#define SDOA_RENDERTYPE_D3D7                0x08
#define SDOA_RENDERTYPE_D3D8                0x10
#define SDOA_RENDERTYPE_D3D9                0x20
#define SDOA_RENDERTYPE_D3D10               0x40
#define SDOA_RENDERTYPE_OPENGL              0x80

// �ⲿ����Screen״̬ 
#define SDOA_SCREEN_NONE				0		// Screenû�б�����
#define SDOA_SCREEN_NORMAL				1		// ������ʾ״̬
#define SDOA_SCREEN_MINI				2		// ��С��״̬


// �ڲ�IWidgetWindows״̬���� 
#define SDOA_WINDOWSSTATUS_NONE			0		// ����û�б�����
#define SDOA_WINDOWSSTATUS_SHOW			1		// ������ʾ״̬
#define SDOA_WINDOWSSTATUS_HIDE			2		// ��������
// ��������
#define SDOA_WINDOWSTYPE_NONE		0       // δ֪����
#define SDOA_WINDOWSTYPE_TOOLBAR	1		// ������
#define SDOA_WINDOWSTYPE_WIDGET		2		// Widget����
#define SDOA_WINDOWSTYPE_WINDOWS	3		// ��ͨ����

// ��Widget��ѡ��
#define SDOA_OPENWIDGET_DEFAULT			0		// Ĭ��
#define SDOA_OPENWIDGET_NOBARICON		1		// ����Widget�����Զ���ӵ���������

// �ӿ�SetSelfCursor�õ��ĳ���
#define SDOA_CURSOR_AUTO		0	// �Զ�����Ƿ��Ի����ָ��
#define SDOA_CURSOR_SELF		1	// ʼ���Ի������ָ��
#define SDOA_CURSOR_HIDESELF	2	// ʼ�ղ��Ի������ָ��

// ��¼����
#define SDOA_LOGINMETHOD_SSO        0   // ��¼������SSO
#define SDOA_LOGINMETHOD_SDO        1   // ��¼������SDO(CAS)

// ��¼״̬
#define SDOA_LOGINSTATE_UNKNOWN     -1  // δ֪����������ȷ���ȡ״̬�쳣
#define SDOA_LOGINSTATE_NONE        0   // ��û�е��ù���¼����
#define SDOA_LOGINSTATE_OK          1   // ��¼�ɹ�
#define SDOA_LOGINSTATE_CONNECTING  2   // ���ڵ�¼
#define SDOA_LOGINSTATE_FAILED      3   // ��¼ʧ��
#define SDOA_LOGINSTATE_LOGOUT      4   // �ѵǳ�

// ȦȦ������չ��ģʽ
#define SDOA_BARMODE_ICONBOTTOM		0   // ��������ȦȦͼ������
#define SDOA_BARMODE_ICONTOP		1   // ��������ȦȦͼ������

struct AppInfo {
	DWORD		cbSize;				// = sizeof(AppInfo)
	int			nAppID;				// ʢ��ͳһ��ϷID
	LPCWSTR		pwcsAppName;		// ��Ϸ����
	LPCWSTR		pwcsAppVer;		   	// ��Ϸ�汾
	int			nRenderType;		// ��Ϸͼ���������� (ȡֵSDOA_RENDERTYPE_UNKNOWN..SDOA_RENDERTYPE_OPENGL)
	int			nMaxUser;			// ��Ϸ��ͬʱ��һ̨��������Ϸ�����������ͨ��Ϊ1������:KOFӦ����2��
	int			nAreaId;			// ��Ϸ��ID	(ע�⣺������ʱ��-1)
	int			nGroupId;           // ��Ϸ��ID	(ע�⣺������ʱ��-1)
};


struct RoleInfo{
	DWORD		cbSize;                 // sizeof(RoleInfo)
	LPCWSTR		pwcsRoleName;			// �ǳ�
	int			nSex;                   // �Ա�(0: Ů; 1: ��)
	// more
};

struct SDOAWinProperty{
	int nWinType;				// ��������(ֻ���ڱ�ʶ�������ͣ�������Ч)
	int nStatus;				// ����״̬
	// ������λ�á������Ϣ
	int nLeft;
	int nTop;
	int nWidth;
	int nHeight;	
};


struct LoginResult {
	DWORD  cbSize;			// sizeof(LoginResult)��Ϊ�Ժ������ṩ����
	LPCSTR szSessionId;		// ���ں�̨��֤��token��
	LPCSTR szSndaid;		// �û�ID
	LPCSTR szIdentityState;	// �Ƿ�����ʶ��0δ���꣬1����
	LPCSTR szAppendix;		// �����ֶΣ�����

};

 struct LoginMessage {
   DWORD  dwSize;
   int    nErrorCode;   // ��¼������صĴ�����,�����ο�����ϸ��������pbstrContent
   BSTR*  pbstrTitle;	// ��ʾ���⣬���Ե���SysReallocString�޸����е����ݣ�ע�⣬�ص������IGW������SysFreeString�ͷ��ѷ�����ڴ�
   BSTR*  pbstrContent;	// ��ʾ���ݣ����Ե���SysReallocString�޸����е����ݣ�ע�⣬�ص������IGW������SysFreeString�ͷ��ѷ�����ڴ�
 };

// LPLOGINCALLBACKPROC�лص�����ErrorCodeֵ����
#define SDOA_ERRORCODE_OK               0    // ��¼�ɹ�
#define SDOA_ERRORCODE_CANCEL           -1   // �û���ȡ���ر��˵�¼�Ի���
#define SDOA_ERRORCODE_UILOST           -2   // ���ضԻ���UI�ļ�ȱʧ����Ч
#define SDOA_ERRORCODE_FAILED           -3   // ��¼ʧ��
#define SDOA_ERRORCODE_UNKNOWN          -4   // δ֪����
#define SDOA_ERRORCODE_INVALIDPARAM     -5   // ������Ч
#define SDOA_ERRORCODE_UNEXCEPT         -6   // δ֪�쳣
#define SDOA_ERRORCODE_ALREAYEXISTS     -7   // �Ѿ�����
#define SDOA_ERRORCODE_SHOWMESSAGE      -10  // ��ʾ��ʾ�Ի���


// ��¼�ص�����
// nErrorCode:     ��¼���ش������
// pLoginResult:   ��¼���ؽ����ֻ�� nErrorCode = SDOA_ERRORCODE_OK ʱ����Ч
// nUserData:      �ڵ���ShowLoginDialogʱ������û�����
// nReserved:      �������ݲ�ʹ�ã�Ŀǰ����Ϊ0(������ShowLoginDialog�д����Reserved����)
// ����ֵ:        ΪTRUE���رնԻ���ΪFALSE���ر�
//
// 2009.03.30
// ��չ���ܣ���nErrorCode = ERRORCODE_SHOWMESSAGEʱ��Reserved����LoginMessage*���
// ����TRUEʱ��IGW�����ٵ����Ի�����Ϸ�������е�����ʾ�Ի����
// ����FALSEʱ��IGW���������Ի��򣬿��Ե���SysReallocString�޸�Title,Content���е����ݣ��Ա���Ϸ��ʾ�Զ���Ĵ�����ʾ
//              ע�⣺�ص������IGW������SysFreeString�ͷ��ѷ�����ڴ�
typedef BOOL (CALLBACK *LPLOGINCALLBACKPROC)(int nErrorCode, const LoginResult* pLoginResult, int nUserData, int nReserved);

// Ӧ�ýӿ�
typedef interface ISDOAApp ISDOAApp;
typedef ISDOAApp *PSDOAApp, *LPSDOAApp;

MIDL_INTERFACE("C8187484-1C5F-48DF-8497-2136A72A5733")
ISDOAApp : public IUnknown
{
public:
	// ��ʾ��¼�Ի���
	STDMETHOD_(int,ShowLoginDialog)(THIS_ LPLOGINCALLBACKPROC fnCallback, int nUserData, int nReserved) PURE;

	STDMETHOD_(void,ModifyAppInfo)(THIS_ const AppInfo* pAppInfo) PURE;
	STDMETHOD_(void,Logout)(THIS) PURE;
	STDMETHOD_(int,GetLoginState)(THIS_ int nLoginMethod) PURE;
	STDMETHOD_(void,SetRoleInfo)(THIS_ const RoleInfo* pRoleInfo) PURE;
	

	// ��ʾ��ֵ�Ի��� 
	STDMETHOD_(int,ShowPaymentDialog)(THIS_ LPCWSTR pwcsSrc) PURE;

	// ����SDOA UI���
	STDMETHOD_(int,GetScreenStatus)(THIS) PURE;
	STDMETHOD_(void,SetScreenStatus)(THIS_ int nValue) PURE;
	STDMETHOD_(bool,GetScreenEnabled)(THIS) PURE;
	STDMETHOD_(void,SetScreenEnabled)(THIS_ bool bValue) PURE;
	STDMETHOD_(bool,GetTaskBarPosition)(THIS_ PPOINT ptPosition) PURE;
	STDMETHOD_(bool,SetTaskBarPosition)(THIS_ const PPOINT ptPosition) PURE;
	STDMETHOD_(bool,GetFocus)(THIS) PURE;
	STDMETHOD_(void,SetFocus)(THIS_ bool bValue) PURE;
	STDMETHOD_(bool,HasUI)(THIS_ const PPOINT ptPosition) PURE;
	
	STDMETHOD_(int,GetTaskBarMode)(THIS) PURE;
	STDMETHOD_(void,SetTaskBarMode)(THIS_ int nBarMode) PURE;
	STDMETHOD_(int,GetSelfCursor)(THIS) PURE;
	STDMETHOD_(void,SetSelfCursor)(THIS_ int nValue) PURE;

	// �������
	STDMETHOD_(int,OpenWidget)(THIS_ LPCWSTR pwcsWidgetNameSpace) PURE;
	STDMETHOD_(int,OpenWidgetEx)(THIS_ LPCWSTR pwcsWidgetNameSpace, LPCWSTR pwcsParam, int nFlag) PURE;
	STDMETHOD_(int,CloseWidget)(THIS_ LPCWSTR pwcsWidgetNameSpace) PURE;
	STDMETHOD_(int,ExecuteWidget)(THIS_ LPCWSTR pwcsWidgetNameSpaceOrGUID, LPCWSTR pwcsParam) PURE;
	STDMETHOD_(int,WidgetExists)(THIS_ LPCWSTR pwcsWidgetNameSpaceOrGUID) PURE;
	STDMETHOD_(int,OpenWindow)(THIS_ LPCWSTR pwcsWinType, LPCWSTR pwcsWinName, LPCWSTR pwcsSrc, int nLeft, int nTop, int nWidth, int nHeight, LPCWSTR pwcsMode) PURE;
	STDMETHOD_(int,CloseWindow)(THIS_ LPCWSTR pwcsWinName) PURE;

	STDMETHOD_(int,GetWinProperty)(THIS_ LPCWSTR pwcsWinName, SDOAWinProperty* pWinProperty) PURE;
	STDMETHOD_(int,SetWinProperty)(THIS_ LPCWSTR pwcsWinName, SDOAWinProperty* pWinProperty) PURE;
};


// ��Ϸʵ�ֵ��������ƻص�
// ע������ֵ����Ч��Χ��0-100
typedef DWORD (CALLBACK *LPGETAUDIOSOUNDVOLUME)();
typedef void  (CALLBACK *LPSETAUDIOSOUNDVOLUME)(DWORD nNewVolume);
typedef DWORD (CALLBACK *LPGETAUDIOEFFECTVOLUME)();
typedef void  (CALLBACK *LPSETAUDIOEFFECTVOLUME)(DWORD nNewVolume);

// ͨ�����ݴ���ӿ�
class ICommonChannel
{
public:
	// ��������������Ϊ����Delphi�Ľӿڶ����ã���C++�в���Ҫʵ�֣�����Ҫ�����ڴ�λ��
	virtual HRESULT _stdcall QueryInterface(REFIID riid, void **ppvObject) = 0;
	virtual ULONG _stdcall AddRef(void) = 0;
	virtual ULONG _stdcall Release(void) = 0;

	virtual void _stdcall SendData(BSTR Request, BSTR* Response) = 0;
	virtual void _stdcall Set_RevertChannel(ICommonChannel *pReverter) = 0;
	virtual ICommonChannel* _stdcall Get_RevertChannel(void) = 0;
	virtual void _stdcall Close(void) = 0;
	virtual void _stdcall Get_ChannelType(LPSTR szChannelType, DWORD nBufferLen) = 0; // max BufferLen = 64
};
typedef ICommonChannel *PCommonChannel;

typedef BOOL (WINAPI *LPCREATECHANNEL)(LPCSTR szChannelType, ICommonChannel **pChannel);


typedef interface ISDOAAppUtils ISDOAAppUtils;
typedef ISDOAAppUtils *PSDOAAppUtils, *LPSDOAAppUtils;

MIDL_INTERFACE("1170C2F9-28AD-4EA8-8392-E9A219C8FF65")
ISDOAAppUtils : public IUnknown
{
public:
	// ��������
	// ��������������ػص�
	STDMETHOD_(void,SetAudioSetting)(THIS_ 
		LPGETAUDIOSOUNDVOLUME  fnGetAudioSoundVolume,
		LPSETAUDIOSOUNDVOLUME  fnSetAudioSoundVolume,
		LPGETAUDIOEFFECTVOLUME fnGetAudioEffectVolume,
		LPSETAUDIOEFFECTVOLUME fnSetAudioEffectVolume ) PURE;
	// ֪ͨSDOA��Ϸ�����б仯                          
	STDMETHOD_(void,NodifyAudioChanged)(THIS) PURE;	
	// ���ô�������ͨ���Ļص�
	STDMETHOD_(void,SetCreateChannelCallback)(THIS_ const LPCREATECHANNEL fnCreateChannel) PURE;	
};

// �첽�ص�
typedef void (CALLBACK *LPQUERYCALLBACK)(int nRetCode, int nUserData);

typedef interface ISDOAClientService ISDOAClientService;
typedef ISDOAClientService *PSDOAClientService, *LPSDOAClientService;

MIDL_INTERFACE("AF56D291-823A-41AA-85A0-EBE5C6163425")
ISDOAClientService : public IUnknown
{
public:
	STDMETHOD_(int, SetValue)(THIS_ const BSTR bstrKey, const BSTR bstrValue ) PURE;
	STDMETHOD_(int, Query)(THIS_ const BSTR bstrService) PURE;	
	STDMETHOD_(int, AsyncQuery)(THIS_ const BSTR bstrService, LPQUERYCALLBACK fnCallback, int nUserData) PURE;	
	STDMETHOD_(int, GetValue)(THIS_ const BSTR bstrKey, BSTR* bstrValue) PURE;	
};

/* ע��igwInitialize��dwSdkVersion����ʹ�ú� SDOA_SDK_VERSION */
typedef int  (WINAPI* LPigwInitialize)(DWORD dwSdkVersion, const AppInfo* pAppInfo);
typedef bool (WINAPI* LPigwGetModule)(REFIID riid, void** intf);
typedef int  (WINAPI* LPigwTerminal)();

#pragma pack(pop)
#endif /* SDOA4CLIENT_H */