/**
* @filename:CEGUIManager.h
* @date: 2009/12/17
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: CEGUI�Ĺ�����
*/
#pragma  once

#include <CEGUI.h>
#include "RendererModules/Direct3D9/CEGUIDirect3D9Renderer.h"

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>
#include <string>

#if defined(_WIN32)
#pragma  comment(lib,"dinput8.lib")
#pragma  comment(lib,"dxguid.lib")
#endif

#include "../CEGUIEngine/BaseUIClass.h"
class UIClass::BaseUIClass;

class CEGUIManager
{
	DECLARE_SINGLETON(CEGUIManager);
/////////////////////////////////////////////////
	struct DirectInputState 
	{
		DirectInputState():directInput(0),KeyboardDevice(0)
		{}

		LPDIRECTINPUT8	directInput;
		LPDIRECTINPUTDEVICE8 KeyboardDevice;
	};
public:
	
	///@brief
	///CEGUI���������캯��
	CEGUIManager();

	/// @brief
	/// �����������ͷ�CEGUI DX9��Ⱦ�⣬CEGUI System ��
	~CEGUIManager();

	/// @brief
	/// ��ȡCEGUI��ϵͳ����
	CEGUI::System* GetSystem() { return m_SysCEGUI; }

	/// @brief
	/// ��ʼ��CEGUI
	/// @param pd3ddevice
	/// d3d9�豸ָ��
	/// @return
	/// �ɹ�����true,ʧ�ܷ���false
	bool InitCEGUI(LPDIRECT3DDEVICE9 pd3ddevice);


	/// @brief
	/// �ͷ�CEGUI ʹ�õ���Դ��
	void ReleaseCEGUI();

	/// @brief
	/// ����CEGUI����Ⱦ
	void RenderCEGUI();

	/// @brief
	/// ����CEGUI������ʱ�䣬֡����DInput
	void UpDate();


	/// @brief
	/// ��Ϊ����CGUI��Ⱦ���޸�D3D��Ⱦ��״̬�����ԣ�����Ⱦ��ɺ���Ҫ����D3D����Ⱦ״̬
	void ResetD3DRenderState();

	/// @brief
	/// ��ʼ����Դ��·��
	/// @param szPath
	/// Ĭ��Ϊ"datafiles",ȷ��szPath !=NULL
	void InitiliseResourceDirectories(const char *szPath="datafiles");

	/// @brief
	/// ��ʼ��Ĭ����Դ��:Imageset,Font,Scheme,looknfeel,layout,lua_script,xmlparse,mousecursor
	void InitialiseDefaultResourceGroups();

	/// @brief
	/// �Ƿ�������ʾFPS
	/// @param Enabble
	/// true = ���ã�false = ������
	void EnableDisplayFPS(bool Enable);

	/// @brief
	/// ����֡������
	void DoUpdateFPS();

	/// @brief
	/// ����CEGUI System ʱ�����
	void DoUpdateTime();

	/// @brief
	/// ֡�������¼�������
	/// @param args
	/// CEGUI�¼�����
	/// @return
	/// �ɹ�����true��ʧ�ܷ���false
	bool OverlayHandler(const CEGUI::EventArgs &args);


	/// @brief
	/// ���ؽű��ļ�
	/// @param ScriptFile
	/// �ű��ļ����ƣ�������·��
	void ExecuteScriptFile(const char *ScriptFile);

	/// @brief
	/// ���ؽű��ļ�����
	/// @param��in &vecScrFiles
	/// ��vector<string>����Ľű��ļ���������ã�������Ϊ�����ֵ
	/// @return
	/// 
	void ExecuteScriptFiles(std::vector<std::string>& vecScrFiles);


	/// @brief
	/// ��ʼ��DInput
	/// @param��in hWnd
	/// ����Ĵ��ھ��
	/// @param: in &dis
	/// ��װ��Dinput�����豸�ṹ�壬���ڴ洢�����õ�Dinput����
	/// @return
	/// ��ʼ���ɹ�����true��ʧ�ܷ���false
	static bool InitialiseDirectInput(HWND hWnd,DirectInputState& dis);

	/// @brief
	/// ����Dinput�¼��ĺ���
	/// @param &dis
	/// ��װ��Dinput�����豸�ṹ�������
	static void DoDirectInputEvents(const CEGUIManager::DirectInputState& dis);

	/// @brief
	/// ���Dinput�豸
	/// @param &dis
	/// ��װ��Dinput�����豸�ṹ��
	static void CleanupDirectInput(CEGUIManager::DirectInputState& dis);

	/// @brief
	/// ��װ�õ�CEGUI�Լ���windows��Ϣ������
	/// @param message
	/// @param wParam
	/// @param lParam
	/// @return
	/// ���û�д���ָ������Ϣ���ͷ���false�������������true
	bool ParseMsg(UINT message,WPARAM wParam,LPARAM lParam);


	/// @brief
	/// ��ȡ��װ��Dinput�ṹ��Ա����������
	DirectInputState& GetDirectInputState(){ return m_DirectInputState; }


	/// @brief
	/// ������Ⱦǰ��D3D״̬
	/// @remark
	/// ��ΪCEGUI��Ⱦ��ı�D3D�豸����Ⱦ״̬��������ȾCEGUIǰҪ������Ⱦ״̬���Ա����CEGUI��Ⱦ�󣬻ָ�D3D��״̬
	/// �˺�����Ҫ��ResetRenderSate()�ɶԳ���
	void SaveRenderState();

	/// @brief
	/// �ָ�D3D��CEGUI��Ⱦǰʱ��״̬
	/// @remark
    /// ��ΪCEGUI��Ⱦ��ı�D3D�豸����Ⱦ״̬���������CEGUI��Ⱦ��Ҫ�ָ�D3D��״̬
	/// �˺�����Ҫ��SaveRenderState()�ɶԳ���
	void ResetRenderState();

	CEGUI::Window* CreateRootWnd();
	CEGUI::Window* GetRootWindow(){return m_RootWindow;};
	void RegisterUIClassInList();
	UIClass::BaseUIClass* GetUIClass(const CEGUI::String ClassName);
/////////////////////////////////////////////////
protected:

/////////////////////////////////////////////////
private:

	
	bool						m_ShowFPS;///< �Ƿ���ʾ֡��
	DWORD						m_LastFrameTime;///< ��һ֡��ʱ��
	DWORD						m_FPS_LastTime;
	int							m_FPS_Frames;
	int							m_FPS_Value;
	char						m_FPS_TextBuffer[16];///< ������ʾ֡�����ַ���
	CEGUI::GeometryBuffer		*m_FPS_geometry;///< ������ʾ֡���Ļ�����

	CEGUI::Direct3D9Renderer	*m_RenderCEGUI; ///< CEGUI DX9 ��Ⱦ��
	CEGUI::System				*m_SysCEGUI;    ///< CEGUI ϵͳ��

	DirectInputState		m_DirectInputState;///< DirectInput �豸����ṹ��

	typedef std::map<const CEGUI::String,UIClass::BaseUIClass*>UIClassList;

	UIClassList m_UIClassList;
	CEGUI::Window* m_RootWindow;
	/////////////////////////////////////////////////
	/**
	*���±������ڴ洢CEGUI��Ⱦ֮ǰ��D3D�豸��Ⱦ״̬
	*ʹ���߲���ϸ��,ֻ��ɶ�ʹ��SaveRenderState()��ResetRenderState()��������
	*/
	LPDIRECT3DDEVICE9 m_d3ddevice;
	DWORD	m_FVF;
	IDirect3DVertexShader9		*m_VertexShader; ///< ����shader
	IDirect3DPixelShader9		*m_PixelShader; ///< ����shader
	DWORD m_LIGHTING;
	DWORD m_FOGENABLE;
	DWORD m_ZENABLE;
	DWORD m_ALPHATESTENABLE;
	DWORD m_CULLMODE;
	DWORD m_FILLMODE;

	DWORD m_SCISSORTESTENABLE;
	DWORD m_ZWRITENABLE;

	
	DWORD m_SAMPADDRESSU;
	DWORD m_SAMPADDRESSV;

	
	DWORD m_COLORARG1;
	DWORD m_COLORARG2;
	DWORD m_COLOROP;

	DWORD m_ALPHAARG1;
	DWORD m_ALPHAARG2;
	DWORD m_ALPHAOP;

	DWORD m_MINFILTER;
	DWORD m_MAGFILTER;

	DWORD m_TSS1_COLOROP;

	DWORD m_RS_ALPHABLENDENABLE;
	DWORD m_RS_SRCBLEND;
	DWORD m_RS_DESTBLEND;
	/////////////////////////////////////////////////

};