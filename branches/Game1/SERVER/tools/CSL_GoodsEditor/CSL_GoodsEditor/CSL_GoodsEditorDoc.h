// CSL_GoodsEditorDoc.h :  CCSL_GoodsEditorDoc ��Ľӿ�
//


#pragma once

class CCSL_GoodsEditorDoc : public CDocument
{
protected: // �������л�����
	CCSL_GoodsEditorDoc();
	DECLARE_DYNCREATE(CCSL_GoodsEditorDoc)

// ����
public:

// ����
public:

// ��д
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CCSL_GoodsEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


