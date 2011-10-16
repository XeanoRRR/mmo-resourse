#pragma once

#include "..\..\frostengine\ui.h"
#include "..\..\frostengine\render.h"
#include "..\..\gameengine\gamemodel.h"
#include "..\..\gameengine\textureprojective.h"
#include "..\..\gameengine\GameSelectRay.h"
#include "light.h"
#include "AppClient\Wing.h"
#include "AppClient\Weapon.h"
#include "AppClient\BindModel.h"

enum CONTROL_CAMEAR_OPERATION{ CCO_NONE,CCO_MOVE,CCO_ROTATE,CCO_SCALE};

enum eModelType
{
	MT_ROLE,	//��ɫģ��
	MT_BIND		//��ģ��
};

class ui::QuickMenu;
class WndEdit;
class WndEditBGColor;
class WndEditLightColor;
class WndAnimModify;
class WndGridMovingSpeedModifyDlg;
class WndSelectLevel;
class WndHotkey;
class WndAbout;
class WndAction;
class WndPart;
class WndLocator;
class WndBindModel;
class WndAnimFrameEx;
class WndGuage;
class GameModel;
class CRoleModel;
class CtrlExplorer;
class CtrlAttribEditor;
class WndModelTexture;
class WndShaderModify;
class WndModelTree;
class WndPartTree;
class WndModelModify;
class WndPartModify;
class WndTrailTree;
class WndTrailModify;
class WndParticleTree;
class WndParticleModify;
//class CGameModelManager;

class WndViewer : public ui::Wnd
{
	friend class WndAction;
	friend class WndLocator;
	friend class WndModel;
	friend class WndAnimModify;
	friend class WndSelectLevel;
	friend class WndModelTexture;
	friend class WndShaderModify;

	render::Camera  * m_pCamera;
	render::Frustum * m_pFrustum;
	SunLight		* m_pSunLight;
	GameSelectRay	  m_ViewfinderRay;

	float					 m_fRotateAngle;
	CONTROL_CAMEAR_OPERATION m_eCamOpType;

	BOOL m_bLightEnable;
	BOOL m_bDisplayWireFrame;
	BOOL m_bShowGrid;
	BOOL m_bShowGridMoving;
	BOOL m_bShowSizeBox;
	BOOL m_bShowPickBox;
	BOOL m_bShowLocator;
	BOOL m_bShowBone;
	BOOL m_bShowNormal;
	BOOL m_bShowCameraInfo;
	BOOL m_bAnimPlaying;
	BOOL m_bAnimLooped; 
	BOOL m_bShowLightArrow;
	BOOL m_bShowLocatorWnd;
	BOOL m_bSwitchBK;					//�л�����ͼƬ����(������ʾ����ͼƬ,������ʾ������ɫ)
	BOOL m_bExAnimFrame;				//����������֡
	BOOL m_bShowViewfinder;				//��ʾȡ����
	BOOL m_bTrail;						//�Ƿ���ʾģ��β��
	BOOL m_bShowUI;						//�Ƿ���ʾUI
	DWORD m_dwAnimOnceLastTime;			//����һ�ε����ʱ��
	DWORD m_dwGridMovingStartTime;
	DWORD m_dwGridTexMovingStartTime;
	float m_fGridMovingSpeed;

	int		   m_iLevelModelIndex;		//��ǰ��ʾ�ȼ���Ϣ��ģ������
	eModelType m_eLevelModelType;		//��ǰ��ʾ�ȼ���Ϣ��ģ������

	int	  m_hCursorHandle;
	DWORD m_dwBK;											//����ͼƬ�ľ��(Ϊ0,��˵��û�м��ر���ͼƬ)
	DWORD m_dwGridTex;										//�ر�������
	render::Interface::tagTextureInfo * m_ptBKTexInfo;		//��ǰ����������Ϣ

    DWORD	 m_dwGroupMenuItemCount;			//��ģ�Ͳ˵�������
	DWORD	 m_dwWeaponLeftMenuItemIndex;		//���������˵�����
	DWORD	 m_dwWeaponRightMenuItemIndex;		//���������˵�����
	DWORD	 m_dwAmbientColor;
	D3DCOLOR m_dwLightDirectionColor;

	ui::MenuBar	  * m_pMenuBar;
	ui::QuickMenu * m_pQuickMenu;	//��ݲ˵�

	D3DXVECTOR3		m_vRolePos;
	CDisplayModel * m_pModel;		//��ǰ��ʾ�Ľ�ɫ��ģ��
	CDisplayModel * m_pRefModel;	//��ʾ��ɫ�ο�ģ��

	std::vector<CRoleModel *> m_vRoleModels;			//��ɫģ�Ͷ�̬����
	std::vector<CBindModel *> m_vBindModels;			//��ģ�Ͷ�̬����

	WndAnimModify				* m_pWndAnimModify;
	WndGridMovingSpeedModifyDlg * m_pWndGridMovingSpeedModify;
	WndHotkey					* m_pWndHotkeyList;
	WndAbout					* m_pWndAbout;
	WndEditBGColor				* m_pWndEditBKColor;
	WndEditLightColor			* m_pWndEditLightColor;
	WndSelectLevel				* m_pWndSelectLevel;
	WndAction					* m_pWndAction;
	WndPart						* m_pWndPart;
	WndModel		  			* m_pWndModels;
	WndLocator		  			* m_pWndLocator;
	WndAnimFrameEx	  			* m_pWndAnimFrameEx;
	WndGuage		  			* m_pWndGuage;
	WndModelTexture	  			* m_pWndModelTexture;
	WndShaderModify   			* m_pWndShaderModify;
	WndModelTree	  			* m_pWndModelTree;
	WndPartTree		  			* m_pWndPartTree;
	WndModelModify    			* m_pWndModelModify;
	WndPartModify	  			* m_pWndPartModify;
	WndTrailTree	  			* m_pWndTrailTree;
	WndTrailModify	  			* m_pWndTrailModify;
	WndParticleTree	  			* m_pWndParticleTree;
	WndParticleModify 			* m_pWndParticleModify;
	DWORD m_dwStartTime;
	DWORD m_dwCurFrame;

	AnimInfo m_cAnimInfo;			//��ɫģ�Ͷ�����Ϣ
	AnimInfo m_cAnimInfoRef;		//�ο�ģ�Ͷ�����Ϣ

	// ����
	char  m_szPathNameTmp[_MAX_PATH];				//��ǰ�򿪵�ģ���ļ���
	char  m_szViewframeDis[20];
	float m_fYRot;
	bool  m_bKeyState[256];							//���ڼ��̵������ļ���״̬

	// ����֡�������
	DWORD			   m_dwAnimTimeLength;
	DWORD			   m_dwAnimFrameIndex;
	DWORD			   m_dwAnimFPS;
	char			 * m_szpFileFormat;
	D3DXIMAGE_FILEFORMAT m_IFF;
	LPDIRECT3DSURFACE9 m_pOldRenderTarget;
	LPDIRECT3DSURFACE9 m_pOldZStencil;
	LPDIRECT3DTEXTURE9 m_pRenderTexture;
	LPDIRECT3DSURFACE9 m_pDepthStencilSurface;

	D3DXVECTOR3		TargetPos;
public:
	WndViewer(void);
	~WndViewer(void);

	BOOL Create(const char *pszName, const RECT *pRect, DWORD dwStyle, ui::Wnd *pParent);
	void Destroy();

	void Update();
	void Render(void);
	int  GetFps(void);

	void MenuBarProc(void);
	void CreateMenuBar(void);
	void DestroyMenuBar(void);
	void QuickMenuProc(void);				//��ݲ˵��¼�����

	void ShowCursor(void);
	void ShowFps(void);
	void ShowAxis(void);
	void ShowGrid(void);
	void ShowInfo(void);
	void DisplayTextureProjective();
	void GetBuffer(int x,int z,DWORD size,D3DXVECTOR3* posbuff);

	void SetupTransform(void);
	void SetMenuBarItemCheck(void);
	void SetQuickMenuVisible(bool bIsVis)		{ bIsVis ? m_pQuickMenu->BeginPopUp() : m_pQuickMenu->EndPopUp(); }		//�趨��ݲ˵��Ƿ�ɼ�
	void SwitchQuickMenuVisible(void);			//�л���ݲ˵��Ƿ�ɼ�
	void MoveQuickMenu(POINT & ptDes)			{ m_pQuickMenu->MoveWndTo(ptDes.x, ptDes.y); }							//�ƶ���ݲ˵�

	CDisplayModel * GetModel()					{ return m_pModel; }
	CDisplayModel * GetCurrentSelBindModel();	//�õ���ǰѡ��İ�ģ��
	AnimInfo	  * GetAnimInfo()				{ return &m_cAnimInfo; }
	void			GetModelName(string & sModelName, const string & sModelPathName);
	void			SetCurOperFileName(const char * pszFileName)		{ strcpy(m_szPathNameTmp, pszFileName); }			//�趨��ǰ�������ļ�·����

	float GetGridMovingSpeed()					{ return m_fGridMovingSpeed; }
	void SetGridMovingSpeed(float f)			{ m_fGridMovingSpeed = f; }

	void SetLevelModelIndex(int iIndex)			{ m_iLevelModelIndex = iIndex; }
	void SetLevelModelType(eModelType eType)	{ m_eLevelModelType = eType; }

	render::Camera  *GetCamera(){return m_pCamera;}

	void HotkeyProc(void);
	void CalculateNormal(void);
	void SaveNormal(void);
	void ExportCamera(void);
	void ImportCamera(const char *szFileName);
	void ExportAnimationFrame();									//������������֡
	void SaveVertexColor(void);
	void ResetCamera(void);
	void CreateTemplateModelFile();
	void CheckFile();

	// ��ģ���ļ�������
	void OpenMainModelFile(const char * pszFileName);				//����ģ���ļ�
	void OpenBindModelFile(const char * pszFileName);				//�򿪰�ģ���ļ�
	void OpenRefModelFile(const char * pszFileName);				//�򿪲ο�ģ���ļ�
	void OpenBKPicFile(const char * pszFileName);					//�򿪱���ͼƬ�ļ�
	void OpenGridTexFile(const char * pszFileName);					//�򿪵ر�����

	// �����˵����ڲ��̶������Ĳ˵���
	void CreateMainModelMenu(const char * pszFileName);				//������ģ�Ͳ˵�
	void CreateActionMenu(const char * pszFileName);				//���������˵�
	void CreatePartList(const char * pszFileName);					//���������б�˵�
	void CreateBindModelMenu(CBindModel * pBindModel);				//�������˵���ģ����Ŀ

	bool KeyClick(BYTE * pbyKeyBuffer, int iScanCodes);				//�����̰��������¼�
	WndModel *GetWndModel()		{return m_pWndModels;}				//��ȡģ�ʹ���
	WndShaderModify *GetWndShaderModify()	{return m_pWndShaderModify;}	//��ȡSHADER�޸Ĵ���
	WndModelTexture *GetWndModelShaderTree(){return m_pWndModelTexture;}	//��ȡShader���δ���
	WndPartTree		*GetWndPartTree()		{return m_pWndPartTree;}		//��ȡpart���δ���
	WndModelTree	*GetWndModelTree()		{return m_pWndModelTree;}		//��ȡmodel���δ���
	WndModelModify  *GetWndModelModify()	{return m_pWndModelModify;}		//��ȡmodel�޸Ĵ���
	WndPartModify   *GetWndPartModify()		{return m_pWndPartModify;}		//��ȡpart�޸Ĵ���
	WndTrailTree	*GetWndTrailTree()		{return m_pWndTrailTree;}		//��ȡTrail���δ���
	WndTrailModify  *GetWndTrailModify()	{return m_pWndTrailModify;}		//��ȡTrail�޸Ĵ���
	WndParticleTree *GetWndParticleTree()	{return m_pWndParticleTree;}	//��ȡPariticle���δ���
	WndParticleModify *GetWndParticleModify() {return m_pWndParticleModify;}	//��ȡParticle�޸Ĵ���
	WndSelectLevel * GetWndSelectLevel()	{ return m_pWndSelectLevel; }	//��ȡ����ѡ�����
};