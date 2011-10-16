
#include "stdafx.h"
#include "WndShaderModify.h"
#include "..\..\frostengine\utility.h"
#include "..\..\frostengine\console.h"
#include "..\..\frostengine\render.h"

#include "gamesetting.h"
#include "wndviewer.h"
#include "animmodifydlg.h"
#include "WndModelShaderTree.h"
#include "WndModel.h"

WndShaderModify::WndShaderModify(void)
{
	m_pAddTextureBtn = NULL;
	m_pDelTextureBtn = NULL;
	m_pAddShaderBtn = NULL;
	m_pDelShaderBtn = NULL;
	m_pComboAlphaTest = NULL;
	m_pComboalphablend = NULL;
	m_pComboColorOP = NULL;
	m_pComboTexIndex = NULL;

	m_pConfirm = NULL;
	m_pCancel = NULL;
	m_pTextureNameEdit = NULL;
	m_pReplaceTextureBtn = NULL;
	m_pTwoSideCheckEnable[0] = NULL;
	m_pTwoSideCheckEnable[1] = NULL;
	m_pNode = 0;
}
WndShaderModify::~WndShaderModify(void)
{

}

BOOL WndShaderModify::Create(const char *pszName,const RECT *pRect,DWORD dwStyle,ui::Wnd *pParent)
{
	ui::Wnd::Create(pszName,pRect,dwStyle,pParent);

	
	//create control
	RECT rect = { 0, 0, 0, 0 };
	m_pAddTextureBtn = new ui::Button;
	m_pAddTextureBtn->Create("��������",&rect, FWS_VISIBLE , this);

	m_pDelTextureBtn = new ui::Button;
	m_pDelTextureBtn->Create("ɾ������",&rect, FWS_VISIBLE , this);

	m_pAddShaderBtn = new ui::Button;
	m_pAddShaderBtn->Create("���Shader",&rect, FWS_VISIBLE, this);
	
	m_pDelShaderBtn = new ui::Button;
	m_pDelShaderBtn->Create("ɾ��Shader",&rect, FWS_VISIBLE, this);

	//m_pComboTwoSide = new ui::Combo;
	//m_pComboTwoSide->Create("�Ƿ�����",&rect, FWS_VISIBLE , this);
	//m_pComboTwoSide->InsertItem("true");
	//m_pComboTwoSide->InsertItem("false");
	//m_pComboTwoSide->SetEditReadOnly(true);
	m_pTwoSideCheckEnable[0] = new ui::CheckBox;
	m_pTwoSideCheckEnable[0]->Create("True",&rect, FWS_VISIBLE | FWS_FILLBKG , this);

	m_pTwoSideCheckEnable[1] = new ui::CheckBox;
	m_pTwoSideCheckEnable[1]->Create("False",&rect, FWS_VISIBLE | FWS_FILLBKG , this);

	m_pComboAlphaTest = new ui::Combo;
	m_pComboAlphaTest->Create("���������Է�ʽ",&rect, FWS_VISIBLE , this);
	m_pComboAlphaTest->InsertItem("none");
	m_pComboAlphaTest->InsertItem("transparent");
	m_pComboAlphaTest->SetEditReadOnly(true);

	m_pComboalphablend = new ui::Combo;
	m_pComboalphablend->Create("��������Ϸ�ʽ",&rect, FWS_VISIBLE , this);
	m_pComboalphablend->InsertItem("additive");
	m_pComboalphablend->InsertItem("alpha");
	m_pComboalphablend->InsertItem("color");
	m_pComboalphablend->InsertItem("reserved");
	m_pComboalphablend->SetEditReadOnly(true);

	m_pComboColorOP = new ui::Combo;
	m_pComboColorOP->Create("��ɫ��ʽ",&rect, FWS_VISIBLE , this);
	m_pComboColorOP->InsertItem("Modulate");
	m_pComboColorOP->InsertItem("Modulate2x");
	m_pComboColorOP->SetEditReadOnly(true);

	m_pComboTexIndex = new ui::Combo;
	m_pComboTexIndex->Create("��������",&rect, FWS_VISIBLE , this);

	m_pTextureNameEdit = new ui::EditBox;
	m_pTextureNameEdit->Create("��������",&rect, FWS_VISIBLE | FWS_FILLBKG | FWS_OUTLINE | FWS_SHADOW, this);

	m_pReplaceTextureBtn = new ui::Button;
	m_pReplaceTextureBtn->Create("�滻����",&rect, FWS_VISIBLE , this);

	m_pConfirm = new ui::Button;
	m_pConfirm->Create("ȷ��/����",&rect, FWS_VISIBLE , this);

	m_pCancel = new ui::Button;
	m_pCancel->Create("ȡ��",&rect, FWS_VISIBLE , this);

	return true;
}

void WndShaderModify::Destroy()
{
	SAFEDESTROY(m_pAddTextureBtn);
	SAFEDESTROY(m_pDelTextureBtn);
	SAFEDESTROY(m_pAddShaderBtn);
	SAFEDESTROY(m_pDelShaderBtn);
	SAFEDESTROY(m_pTwoSideCheckEnable[0]);
	SAFEDESTROY(m_pTwoSideCheckEnable[1]);
	SAFEDESTROY(m_pComboAlphaTest);
	SAFEDESTROY(m_pComboalphablend);
	SAFEDESTROY(m_pComboColorOP);
	SAFEDESTROY(m_pComboTexIndex);
	SAFEDESTROY(m_pConfirm);
	SAFEDESTROY(m_pCancel);
	SAFEDESTROY(m_pTextureNameEdit);
	SAFEDESTROY(m_pReplaceTextureBtn);
	ui::Wnd::Destroy();
}

void WndShaderModify::CaculateWhichToDisplay()
{
	//���ݽڵ������Щ�ؼ���ʾ���޸�
	if(!m_pNode)
		return;
	m_pAddTextureBtn->ShowWnd(false);
	m_pDelTextureBtn->ShowWnd(false);
	m_pAddShaderBtn->ShowWnd(false);
	m_pDelShaderBtn->ShowWnd(false);
	m_pComboAlphaTest->ShowWnd(false);
	m_pComboalphablend->ShowWnd(false);
	m_pComboColorOP->ShowWnd(false);
	m_pComboTexIndex->ShowWnd(false);
	m_pTextureNameEdit->ShowWnd(false);
	m_pConfirm->ShowWnd(false);
	m_pCancel->ShowWnd(false);
	m_pReplaceTextureBtn->ShowWnd(false);
	m_pTwoSideCheckEnable[0]->ShowWnd(false);
	m_pTwoSideCheckEnable[1]->ShowWnd(false);

	const char * name = m_pNode->GetName();		// ��ȡ����
	const char * parentName = "";
	const char * value = m_pNode->GetValue();
	char strPostfix[10];
	if(strcmp(value,"") != 0 && strlen(value)>= 4)
	{
		string temp(value);
		strcpy(strPostfix,temp.substr(temp.length()-4,temp.length()).c_str());
	}
	ui::TreeView::Node *pParentNode = m_pNode->GetParent();
	ui::TreeView::Node *pGrandNode = NULL;
	if(pParentNode)
	{
		parentName = pParentNode->GetName();		//��ȡ���ڵ�����
		pGrandNode = pParentNode->GetParent();
	}
	RECT rcWorkArea;
	memcpy(&rcWorkArea,&m_tWindowInfo.rcRect,sizeof(RECT));
	rcWorkArea.bottom = 150;

	RECT rcControl;
	rcControl.left		= rcWorkArea.left + 8;
	rcControl.right		= rcControl.left + 160;
	rcControl.top		= rcWorkArea.top + 32;
	rcControl.bottom	= rcControl.top + 25; 

	RECT rectConfirm;
	rectConfirm.left	= rcWorkArea.left + 8;
	rectConfirm.right	= rectConfirm.left + 70;
	rectConfirm.top		= rcWorkArea.top + 95;
	rectConfirm.bottom	= rectConfirm.top + 20;
	m_pConfirm->SetWndRect(&rectConfirm);

	RECT rectCancel;
	rectCancel.left = rectConfirm.right + 20;
	rectCancel.right = rectCancel.left + 70;
	rectCancel.top = rcWorkArea.top + 95;
	rectCancel.bottom = rectCancel.top + 20;
	m_pCancel->SetWndRect(&rectCancel);

	if(strcmp(name,"layer0") == 0 && strcmp(parentName,"texture") == 0)	//�������/ɾ���������һ��λ�ã�
	{
		m_pAddTextureBtn->SetWndRect(&rcControl);
		m_pAddTextureBtn->ShowWnd(true);
		RECT delRc;
		delRc.left = rcControl.left;
		delRc.right = delRc.left + 160;
		delRc.top = rcControl.top + 50;
		delRc.bottom = delRc.top + 25;
		m_pDelTextureBtn->SetWndRect(&delRc);
		m_pDelTextureBtn->ShowWnd(true);
		SetWndRect(&rcWorkArea);
		ShowWnd(true);
		return;
	}
	else if(strcmp(parentName,"layer0") == 0 && pGrandNode && strcmp(pGrandNode->GetName(),"texture") == 0) //�༭����
	{
		SetWndRect(&rcWorkArea);
		m_pTextureNameEdit->SetWndRect(&rcControl);
		m_pTextureNameEdit->ShowWnd(true);
		m_pTextureNameEdit->SetEditText(value);
		m_pConfirm->ShowWnd(true);
		m_pCancel->ShowWnd(true);

		RECT rcBtn;
		rcBtn.left = rcControl.left;
		rcBtn.top = rcControl.bottom + 7;
		rcBtn.right = rcControl.right;
		rcBtn.bottom = rcBtn.top + 25;
		m_pReplaceTextureBtn->SetWndRect(&rcBtn);
		m_pReplaceTextureBtn->ShowWnd(true);
		ShowWnd(true);
		return;		
	}
	else if(strcmp(name,"shader") == 0 && strcmp(parentName,"main") == 0)	//���/ɾ��һ��Shader��
	{
		SetWndRect(&rcWorkArea);
		m_pAddShaderBtn->SetWndRect(&rcControl);
		m_pAddShaderBtn->ShowWnd(true);
		RECT rcDel;
		rcDel.left = rcControl.left;
		rcDel.right = rcDel.left + 160;
		rcDel.top = rcControl.top + 50;
		rcDel.bottom = rcDel.top + 25;
		m_pDelShaderBtn->SetWndRect(&rcDel);
		m_pDelShaderBtn->ShowWnd(true);
		ShowWnd(true);
		return;
	}
	else if(strcmp(name,"twoside") == 0)	//�޸��Ƿ�˫��
	{	
		SetWndRect(&rcWorkArea);
		RECT rcChe;
		rcChe.left = rcControl.left;
		rcChe.top = rcControl.bottom + 8;
		rcChe.right = rcControl.right;
		rcChe.bottom = rcChe.top + 25;
		m_pTwoSideCheckEnable[0]->SetWndRect(&rcControl);
		m_pTwoSideCheckEnable[1]->SetWndRect(&rcChe);
		m_pTwoSideCheckEnable[0]->ShowWnd(true);
		m_pTwoSideCheckEnable[1]->ShowWnd(true);

		if(strcmp(value,"true") == 0)
		{
			m_pTwoSideCheckEnable[0]->SetChecked(true);
			m_pTwoSideCheckEnable[1]->SetChecked(false);
		}
		else
		{
			m_pTwoSideCheckEnable[0]->SetChecked(false);
			m_pTwoSideCheckEnable[1]->SetChecked(true);
		}
		m_pConfirm->ShowWnd(true);
		m_pCancel->ShowWnd(true);
		ShowWnd(true);
		return;
	}
	else if(strcmp(name,"alphatest") == 0)	//�޸İ��������Է�ʽ
	{
		SetWndRect(&rcWorkArea);
		m_pComboAlphaTest->SetWndRect(&rcControl);
		m_pComboAlphaTest->ShowWnd(true);
		m_pComboAlphaTest->SetEditText(value);
		m_pConfirm->ShowWnd(true);
		m_pCancel->ShowWnd(true);
		ShowWnd(true);
		return;
	}
	else if(strcmp(name,"alphablend") == 0)	//�޸İ�������Ϸ�ʽ
	{
		SetWndRect(&rcWorkArea);
		m_pComboalphablend->SetWndRect(&rcControl);
		m_pComboalphablend->ShowWnd(true);
		m_pComboalphablend->SetEditText(value);
		m_pConfirm->ShowWnd(true);
		m_pCancel->ShowWnd(true);
		ShowWnd(true);
		return;
	}
	else if(strcmp(name,"ColorOp") == 0)		//�޸���ɫ������ʽ
	{
		SetWndRect(&rcWorkArea);
		m_pComboColorOP->SetWndRect(&rcControl);
		m_pComboColorOP->ShowWnd(true);
		m_pComboColorOP->SetEditText(value);
		m_pConfirm->ShowWnd(true);
		m_pCancel->ShowWnd(true);
		ShowWnd(true);
		return;
	}
	else if(strcmp(name,"tex") == 0)		//�޸���������
	{
		SetWndRect(&rcWorkArea);
		m_pComboTexIndex->SetWndRect(&rcControl);
		m_pComboTexIndex->Clear();

		WndViewer  * pWndViewer	= (WndViewer *)GetParent();
		ui::TreeView *pTreeView = pWndViewer->GetWndModelShaderTree()->GetTextureTreeView();
		ui::TreeView::Node *pRootNode = pTreeView->GetRootNode();
		ui::TreeView::Node *pTextNode = pTreeView->GetNodeByName(pRootNode,"layer0");
		std::vector<ui::TreeView::Node*> ::iterator iter = pTextNode->GetChildList()->begin();
		for(;iter!=pTextNode->GetChildList()->end();++iter)
		{
			ui::TreeView::Node *pNode = *iter;
			const char *name = pNode->GetName();
			m_pComboTexIndex->InsertItem(name);
		}
		m_pComboTexIndex->ShowWnd(true);
		m_pComboTexIndex->SetEditText(value);
		m_pConfirm->ShowWnd(true);
		m_pCancel->ShowWnd(true);
		ShowWnd(true);
		return;
	}
	else
	{
		ShowWnd(false);
		return;
	}
	
}
void WndShaderModify::SetCurNode(ui::TreeView::Node *pNode)
{
	if(pNode == m_pNode)
		return;
	m_pNode = pNode;
}
void WndShaderModify::Update()
{
	ui::Wnd::UpdateDefault();

	DWORD &dwStyle = m_tWindowInfo.dwStyle;
	ui::Manager::_tagINPUTINFO *pInputInfo = ui::Manager::GetInstance()->GetInputInfoBuffer();
	if (dwStyle & FWS_VISIBLE && !(dwStyle & FWS_DISABLE))
	{
		render::Interface		   * pInterface	= render::Interface::GetInstance();
		if(pInputInfo->eType == UIMT_MS_BTNDOWN && pInputInfo->dwData & MK_LBUTTON)
		{
			if(!m_pNode)
				return;
			WndViewer  * pWndViewer	= (WndViewer *)GetParent();
			ui::TreeView *pTreeView = pWndViewer->GetWndModelShaderTree()->GetTextureTreeView();
			const char * name = m_pNode->GetName();		// ��ȡ����
			const char * parentName = "";
			const char * value = m_pNode->GetValue();
			char strPostfix[10];
			if(strcmp(value,"") != 0 && strlen(value)>= 4)
			{
				string temp(value);
				strcpy(strPostfix,temp.substr(temp.length()-4,temp.length()).c_str());
			}
			ui::TreeView::Node *pParentNode = m_pNode->GetParent();
			ui::TreeView::Node *pGrandNode = NULL;
			if(pParentNode)
			{
				parentName = pParentNode->GetName();		//��ȡ���ڵ�����
				pGrandNode = pParentNode->GetParent();
			}
			RECT rect;
			//���ȡ��
			m_pCancel->GetWndRect(&rect);
			if (::PtInRect(&rect,pInputInfo->ptMouse) && m_pCancel->GetStyle())		
			{
				ShowWnd(false);
			}

			//���һ������
			m_pAddTextureBtn->GetWndRect(&rect);
			if (::PtInRect(&rect,pInputInfo->ptMouse) && m_pAddTextureBtn->GetStyle())		
			{
				OPENFILENAME ofn;								// common dialog box structure
				static char szFile[_MAX_PATH] = "";		// buffer for file name

				// Initialize OPENFILENAME
				ZeroMemory(&ofn, sizeof(OPENFILENAME));
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = pInterface->GetWndHandle();
				ofn.lpstrFile = szFile;
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = "�����ļ�\0*.*\0tga�ļ�\0*.tga\0dds�ļ�\0*.dds\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = "";
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_FILEMUSTEXIST;

				// Display the Open dialog box. 
				static char szOldDirectory[_MAX_PATH];
				GetCurrentDirectory(_MAX_PATH ,szOldDirectory);
				char strName[128] = "";
				if(GetOpenFileName(&ofn))
				{
					strcpy(strName , utility::File::GetFileName(szFile));
				}
				if(strcmp(strName,"") != 0)
				{
					ui::TreeView::Node *pNode = new ui::TreeView::Node;
					pNode->SetValue(strName);
					sprintf(strName,"%d",m_pNode->GetChildCount());
					pNode->SetName(strName);
					pNode->SetParent(m_pNode);
					pTreeView->AppendChildNode(m_pNode,pNode);
				}
				ShowWnd(false);
				//���±����ļ�
				////////////////////////
				pWndViewer->GetWndModelShaderTree()->SaveToShaderFile();
			}
			//ɾ��һ������
			m_pDelTextureBtn->GetWndRect(&rect);
			if (::PtInRect(&rect,pInputInfo->ptMouse) && m_pDelTextureBtn->GetStyle())		
			{
				pTreeView->DeleteLastNode(m_pNode);
				ShowWnd(false);
				//���±����ļ�
				////////////////////////
				pWndViewer->GetWndModelShaderTree()->SaveToShaderFile();
			}
			//�滻һ������
			m_pReplaceTextureBtn->GetWndRect(&rect);
			if (::PtInRect(&rect,pInputInfo->ptMouse) && m_pReplaceTextureBtn->GetStyle())		
			{
				
				OPENFILENAME ofn;								// common dialog box structure
				static char szFile[_MAX_PATH] = "";		// buffer for file name

				// Initialize OPENFILENAME
				ZeroMemory(&ofn, sizeof(OPENFILENAME));
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = pInterface->GetWndHandle();
				ofn.lpstrFile = szFile;
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = "�����ļ�\0*.*\0tga�ļ�\0*.tga\0dds�ļ�\0*.dds\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = "";
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_FILEMUSTEXIST;

				// Display the Open dialog box. 
				static char szOldDirectory[_MAX_PATH];
				GetCurrentDirectory(_MAX_PATH ,szOldDirectory);
				char strName[128] = "";
				if(GetOpenFileName(&ofn))
				{
					strcpy(strName , utility::File::GetFileName(szFile));
				}
				if(strcmp(strName,"") != 0)
				{
					m_pNode->SetValue(strName);
				}
				//���±����ļ�
				////////////////////////
				pWndViewer->GetWndModelShaderTree()->SaveToShaderFile();
			}
			//���һ��Shader��
			m_pAddShaderBtn->GetWndRect(&rect);
			if(::PtInRect(&rect,pInputInfo->ptMouse)&& m_pAddShaderBtn->GetStyle() )
			{
				//����shader���ڵ����洴���ӽڵ�
				char strName[128];
				ui::TreeView::Node *pNewShaderNode = new ui::TreeView::Node;
				sprintf(strName,"sdr%d",m_pNode->GetChildCount());
				pNewShaderNode->SetName(strName);
				pNewShaderNode->SetValue("");
				pNewShaderNode->SetParent(m_pNode);
				pTreeView->AppendChildNode(m_pNode,pNewShaderNode);
				
				//������Ҷ�ӽڵ�
				ui::TreeView::Node *pCldNode_stream = new ui::TreeView::Node;
				pCldNode_stream->SetName("stream");
				pCldNode_stream->SetValue("posdifspctex0");
				pCldNode_stream->SetParent(pNewShaderNode);
				pTreeView->AppendChildNode(pNewShaderNode,pCldNode_stream);

				ui::TreeView::Node *pCldNode_twoside = new ui::TreeView::Node;
				pCldNode_twoside->SetName("twoside");
				pCldNode_twoside->SetValue("true");
				pCldNode_twoside->SetParent(pNewShaderNode);
				pTreeView->AppendChildNode(pNewShaderNode,pCldNode_twoside);

				ui::TreeView::Node *pCldNode_alphatest = new ui::TreeView::Node;
				pCldNode_alphatest->SetName("alphatest");
				pCldNode_alphatest->SetValue("none");
				pCldNode_alphatest->SetParent(pNewShaderNode);
				pTreeView->AppendChildNode(pNewShaderNode,pCldNode_alphatest);

				ui::TreeView::Node *pCldNode_alphablend = new ui::TreeView::Node;
				pCldNode_alphablend->SetName("alphablend");
				pCldNode_alphablend->SetValue("additive");
				pCldNode_alphablend->SetParent(pNewShaderNode);
				pTreeView->AppendChildNode(pNewShaderNode,pCldNode_alphablend);

				ui::TreeView::Node *pCldNode_layer0 = new ui::TreeView::Node;
				pCldNode_layer0->SetName("layer0");
				pCldNode_layer0->SetValue("");
				pCldNode_layer0->SetParent(pNewShaderNode);
				pTreeView->AppendChildNode(pNewShaderNode,pCldNode_layer0);
				//////////����Layer0������ӽڵ�///////////////////////////	
				ui::TreeView::Node *pCldNode_ColorOp = new ui::TreeView::Node;
				pCldNode_ColorOp->SetName("ColorOp");
				pCldNode_ColorOp->SetValue("Modulate");
				pCldNode_ColorOp->SetParent(pCldNode_layer0);
				pTreeView->AppendChildNode(pCldNode_layer0,pCldNode_ColorOp);

				ui::TreeView::Node *pCldNode_ColorArg1 = new ui::TreeView::Node;
				pCldNode_ColorArg1->SetName("ColorArg1");
				pCldNode_ColorArg1->SetValue("Texture");
				pCldNode_ColorArg1->SetParent(pCldNode_layer0);
				pTreeView->AppendChildNode(pCldNode_layer0,pCldNode_ColorArg1);

				ui::TreeView::Node *pCldNode_ColorArg2 = new ui::TreeView::Node;
				pCldNode_ColorArg2->SetName("ColorArg2");
				pCldNode_ColorArg2->SetValue("Diffuse");
				pCldNode_ColorArg2->SetParent(pCldNode_layer0);
				pTreeView->AppendChildNode(pCldNode_layer0,pCldNode_ColorArg2);

				ui::TreeView::Node *pCldNode_AlphaOp = new ui::TreeView::Node;
				pCldNode_AlphaOp->SetName("ColorArg2");
				pCldNode_AlphaOp->SetValue("Diffuse");
				pCldNode_AlphaOp->SetParent(pCldNode_layer0);
				pTreeView->AppendChildNode(pCldNode_layer0,pCldNode_AlphaOp);

				ui::TreeView::Node *pCldNode_AlphaArg1 = new ui::TreeView::Node;
				pCldNode_AlphaArg1->SetName("AlphaArg1");
				pCldNode_AlphaArg1->SetValue("Texture");
				pCldNode_AlphaArg1->SetParent(pCldNode_layer0);
				pTreeView->AppendChildNode(pCldNode_layer0,pCldNode_AlphaArg1);

				ui::TreeView::Node *pCldNode_AlphaArg2 = new ui::TreeView::Node;
				pCldNode_AlphaArg2->SetName("AlphaArg2");
				pCldNode_AlphaArg2->SetValue("Diffuse");
				pCldNode_AlphaArg2->SetParent(pCldNode_layer0);
				pTreeView->AppendChildNode(pCldNode_layer0,pCldNode_AlphaArg2);

				ui::TreeView::Node *pCldNode_tex = new ui::TreeView::Node;
				pCldNode_tex->SetName("tex");
				pCldNode_tex->SetValue("0");
				pCldNode_tex->SetParent(pCldNode_layer0);
				pTreeView->AppendChildNode(pCldNode_layer0,pCldNode_tex);

				///////////////////////////////////////////////////////////
				ui::TreeView::Node *pCldNode_layer1 = new ui::TreeView::Node;
				pCldNode_layer1->SetName("layer1");
				pCldNode_layer1->SetValue("");
				pCldNode_layer1->SetParent(pNewShaderNode);
				pTreeView->AppendChildNode(pNewShaderNode,pCldNode_layer1);

				ui::TreeView::Node *pCldNode_layer1_ColorOp = new ui::TreeView::Node;
				pCldNode_layer1_ColorOp->SetName("ColorOp");
				pCldNode_layer1_ColorOp->SetValue("Disable");
				pCldNode_layer1_ColorOp->SetParent(pCldNode_layer1);
				pTreeView->AppendChildNode(pCldNode_layer1,pCldNode_layer1_ColorOp);

				ui::TreeView::Node *pCldNode_layer1_AlphaOp = new ui::TreeView::Node;
				pCldNode_layer1_AlphaOp->SetName("AlphaOp");
				pCldNode_layer1_AlphaOp->SetValue("Disable");
				pCldNode_layer1_AlphaOp->SetParent(pCldNode_layer1);
				pTreeView->AppendChildNode(pCldNode_layer1,pCldNode_layer1_AlphaOp);

				ui::TreeView::Node *pCldNode_layer1_tex = new ui::TreeView::Node;
				pCldNode_layer1_tex->SetName("tex");
				pCldNode_layer1_tex->SetValue("0");
				pCldNode_layer1_tex->SetParent(pCldNode_layer1);
				pTreeView->AppendChildNode(pCldNode_layer1,pCldNode_layer1_tex);
				ShowWnd(false);
				//���±����ļ�
				////////////////////////
				pWndViewer->GetWndModelShaderTree()->SaveToShaderFile();
			}

			///ɾ�����һ��Shader��
			m_pDelShaderBtn->GetWndRect(&rect);
			if(::PtInRect(&rect,pInputInfo->ptMouse) && m_pDelShaderBtn->GetStyle())
			{
				//�ҵ����һ��shader
				ui::TreeView::Node *pShaderNode = (*(m_pNode->GetChildList()))[m_pNode->GetChildCount()-1];
				ui::TreeView::Node *pLayer0 = (*(pShaderNode->GetChildList()))[4];
				ui::TreeView::Node *pLayer1 = (*(pShaderNode->GetChildList()))[5];

				for(DWORD i = 0; i < pLayer1->GetChildCount(); i++)
				{
					pTreeView->DeleteLastNode(pLayer1);
				}
				for (DWORD i = 0; i < pLayer0->GetChildCount(); i++)
				{
					pTreeView->DeleteLastNode(pLayer0);
				}

				for (DWORD i = 0; i < pShaderNode->GetChildCount(); i++)
				{
					pTreeView->DeleteLastNode(pShaderNode);
				}
				pTreeView->DeleteLastNode(m_pNode);
				ShowWnd(false);
				//���±����ļ�
				////////////////////////
				pWndViewer->GetWndModelShaderTree()->SaveToShaderFile();
			}
			
			//CheckBox Click
			m_pTwoSideCheckEnable[0]->GetWndRect(&rect);
			if (::PtInRect(&rect,pInputInfo->ptMouse) && m_pTwoSideCheckEnable[0]->GetStyle())
			{
				m_pTwoSideCheckEnable[1]->SetChecked(m_pTwoSideCheckEnable[0]->IsChecked());
			}
			m_pTwoSideCheckEnable[1]->GetWndRect(&rect);
			if (::PtInRect(&rect,pInputInfo->ptMouse) && m_pTwoSideCheckEnable[1]->GetStyle())
			{
				m_pTwoSideCheckEnable[0]->SetChecked(m_pTwoSideCheckEnable[1]->IsChecked());
			}
			

			m_pConfirm->GetWndRect(&rect);
			if (::PtInRect(&rect,pInputInfo->ptMouse))		//���ȷ������
			{
				if(strcmp(parentName,"layer0") == 0 && pGrandNode && strcmp(pGrandNode->GetName(),"texture") == 0)	//�޸������ļ���
				{
					char strValue[128];
					m_pTextureNameEdit->GetEditText(strValue,127);
					if (strcmp(strValue,"") == 0)
					{
						return;
					}
					char strPostfix[10];
					if(strcmp(strValue,"") != 0 && strlen(strValue)>= 4)
					{
						string temp(strValue);
						strcpy(strPostfix,temp.substr(temp.length()-4,temp.length()).c_str());
					}
					else
						strcpy(strPostfix," ");
					if (strcmp(strPostfix,".dds") != 0)
					{
						if(strcmp(strPostfix,".tga") != 0)
						{
							MessageBox(NULL,"ͼƬ��ʽ����","����",MB_OK);
							return;
						}
					}
					m_pNode->SetValue(strValue);
				}
				else if(m_pTwoSideCheckEnable[0]->IsChecked())				//�޸��Ƿ�˫��
				{
					m_pNode->SetValue("true");
				}
				else if (m_pTwoSideCheckEnable[1]->IsChecked())
				{
					m_pNode->SetValue("false");
				}
				else if(strcmp(name,"alphatest") == 0)		//�޸İ��������Է�ʽ
				{
					char strValue[128];
					m_pComboAlphaTest->GetEditText(strValue,127);
					if (strcmp(strValue,"") == 0)
					{
						return;
					}
					m_pNode->SetValue(strValue);
				}
				else if(strcmp(name,"alphablend") == 0)		//�޸İ�������Ϸ�ʽ
				{
					char strValue[128];
					m_pComboalphablend->GetEditText(strValue,127);
					if (strcmp(strValue,"") == 0)
					{
						return;
					}
					m_pNode->SetValue(strValue);
				}
				else if(strcmp(name,"ColorOp") == 0)		//�޸���ɫ������ʽ
				{
					char strValue[128];
					m_pComboColorOP->GetEditText(strValue,127);
					if (strcmp(strValue,"") == 0)
					{
						return;
					}
					m_pNode->SetValue(strValue);
				}
				else if(strcmp(name,"tex") == 0)			//�޸���������
				{
					char strValue[128];
					m_pComboTexIndex->GetEditText(strValue,127);
					if (strcmp(strValue,"") == 0)
					{
						return;
					}
					m_pNode->SetValue(strValue);
				}
				ShowWnd(false);
				////////////////////////
				//���±����ļ�
				////////////////////////
				pWndViewer->GetWndModelShaderTree()->SaveToShaderFile();
			}
		}
		ui::Wnd::UpdateChild();
	}
}

void WndShaderModify::Render()
{
	DWORD &dwStyle = m_tWindowInfo.dwStyle;
	if (dwStyle & FWS_VISIBLE)
	{
		ui::Wnd::RenderDefault();	
		if(m_pNode)
		{
			string sTitle = " �༭:";
			sTitle	 += m_pNode->GetName();
			if(strcmp(m_pNode->GetValue(),"")!=0)
			{
				sTitle += " = ";
				sTitle += m_pNode->GetValue();
			}
			render::Interface *pInterface = render::Interface::GetInstance();
			render::Interface::Layer2D *pLayer2D = pInterface->GetLayer2D();

			RECT &rc = m_tWindowInfo.rcRect;

			int iX = rc.left, iY = rc.top;//,iW = rc.right - rc.left + 1,;
		
			int iTextLen = (int)sTitle.size();

			pLayer2D->TextOut(iX + 1 + 4,iY + 1 + 2,sTitle.c_str(),iTextLen,FWC_BLACK);
			pLayer2D->TextOut(iX - 1+ 4,iY - 1+ 2,sTitle.c_str(),iTextLen,FWC_BLACK);
			pLayer2D->TextOut(iX + 1+ 4,iY - 1+ 2,sTitle.c_str(),iTextLen,FWC_BLACK);
			pLayer2D->TextOut(iX - 1+ 4,iY + 1+ 2,sTitle.c_str(),iTextLen,FWC_BLACK);
			pLayer2D->TextOut(iX + 1+ 4,iY+ 2,sTitle.c_str(),iTextLen,FWC_BLACK);
			pLayer2D->TextOut(iX - 1+ 4,iY+ 2,sTitle.c_str(),iTextLen,FWC_BLACK);
			pLayer2D->TextOut(iX+ 4,iY + 1+ 2,sTitle.c_str(),iTextLen,FWC_BLACK);
			pLayer2D->TextOut(iX+ 4,iY - 1+ 2,sTitle.c_str(),iTextLen,FWC_BLACK);
			pLayer2D->TextOut(iX+ 4,iY+ 2,sTitle.c_str(),iTextLen,FWC_WHITE);	
		}
		ui::Wnd::RenderChild();	
	} 
}