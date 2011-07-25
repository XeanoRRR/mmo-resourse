
// MsScriptEditorDoc.h : CMsScriptEditorDoc ��Ľӿ�
//


#pragma once


class CMsScriptEditorDoc : public CDocument
{
protected: // �������л�����
	CMsScriptEditorDoc();
	DECLARE_DYNCREATE(CMsScriptEditorDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

    static void LUACALL LuaHookCallBackFun(lua_State *L, lua_Debug *ar);

// ʵ��
public:
	virtual ~CMsScriptEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
    TCHAR*  GetTextBuff(VOID);

    INT     MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType);

protected:

    CMsLuaManager*  m_pCMsLuaManager;
    TCHAR           m_szText[MAX_PATH];

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

	// ���ɵ� OLE ����ӳ�亯��

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


