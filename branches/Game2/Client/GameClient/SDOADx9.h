#ifndef SDOADX9_H
#define SDOADX9_H


#include <d3d9.h>

typedef interface ISDOADx9 ISDOADx9;
typedef ISDOADx9 *PSDOADx9, *LPSDOADx9;

MIDL_INTERFACE("59B789AB-BEE9-4C68-AA23-BCA9AB1A3E50")
ISDOADx9 : public IUnknown
{
public:
	/* ��ʼ��d3d�豸 */
	STDMETHOD_(HRESULT,Initialize)(THIS_ IDirect3DDevice9* pDev9, D3DPRESENT_PARAMETERS* pParams, bool bHookGameWnd) PURE;	
	/* ��Ⱦ�����ڲ��Ѿ�����BeginScene��EndScene��ȷ����EndScene֮������� */
	STDMETHOD_(HRESULT,Render)(THIS) PURE;	
	/* ȷ����BeginScene��EndScene֮�����RenderEx */
	STDMETHOD_(HRESULT,RenderEx)(THIS) PURE;	
	/* �ͷ�d3d�����Դ */
	STDMETHOD_(HRESULT,Finalize)(THIS) PURE;
	/* �����豸 */
	STDMETHOD_(void,OnDeviceReset)(THIS_ D3DPRESENT_PARAMETERS* pParams) PURE;
	/* �豸��ʧ */
	STDMETHOD_(void,OnDeviceLost)(THIS) PURE;
	/* ������Ϣ */
	STDMETHOD_(HRESULT,OnWindowProc)(THIS_ HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lResult) PURE;
};

#endif /* SDOADX9_H */