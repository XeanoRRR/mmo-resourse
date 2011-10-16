#include "stdafx.h"
#include "../../Engine/Render.h"
#include "Player.h"
#include "MoveShape.h"
#include "FaceActionList.h"
#include "ClientRegion.h"
#include "../GameClient/Game.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
 * ����: ���캯��
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.06.16
 */
CFaceActionList::CFaceActionList(void)
{
}



/*
 * ����: ��������
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.06.16
 */
CFaceActionList::~CFaceActionList(void)
{
	Release();
}



/*
 * ����: ���ݴ�����ַ����õ������̬ID
 * ժҪ: -
 * ����: pszStr - ��̬�����ļ����ڵ�Ŀ¼
 * ����ֵ: �ɹ� - ����ID
 *		   ʧ�� - û�ҵ��򷵻�0
 * ����: lpf
 * ��������: 2008.06.16
 * �޸���־:
 */
DWORD CFaceActionList::GetPlayerFaceActionID(const char * pszStr)
{
	for (size_t st = 0; st < m_vecPlayerFaceAction.size(); ++st)
	{
		if (m_vecPlayerFaceAction[st].sCallStr == pszStr)
			return (DWORD)(st + 1);
	}

	return 0;
}



/*
 * ����: ���ݴ����ID�õ������̬����
 * ժҪ: -
 * ����: dwID - ����ID��1��ʼ
 * ����ֵ: �ɹ� - ���������ָ̬��
 *		   ʧ�� - ����NULL
 * ����: lpf
 * ��������: 2008.06.16
 * �޸���־:
*	2009.04.10 - lpf
*		�޸��˴���ID��ΧԽ���ж�����;
 */
tagFaceAction * CFaceActionList::GetPlayerFaceAction(DWORD dwID)
{
	if ((int)dwID < 1 || dwID > m_vecPlayerFaceAction.size())
		return NULL;

	return &(m_vecPlayerFaceAction[dwID - 1]);
}



/*
 * ����: ���ݴ����ID�õ�������̬����
 * ժҪ: -
 * ����: dwID - ����ID��1��ʼ
 * ����ֵ: �ɹ� - ���ع�����ָ̬��
 *		   ʧ�� - ����NULL
 * ����: lpf
 * ��������: 2008.06.16
 * �޸���־:
*	2009.04.10 - lpf
*		�޸��˴���ID��ΧԽ���ж�����;
 */
tagFaceAction * CFaceActionList::GetMonsterFaceAction(DWORD dwID)
{
	if ((int)dwID < 1 || dwID > m_vecMonsterFaceAction.size())
		return NULL;

	return &(m_vecMonsterFaceAction[dwID - 1]);
}



/*
 * ����: ���ݴ���Ķ����ļ������õ������б�
 * ժҪ: -
 * ����: dwIndex - ��������0��ʼ
 * ����ֵ: �ɹ� - ���ع�����ָ̬��
 *		   ʧ�� - ����NULL
 * ����: lpf
 * ��������: 2008.06.16
 * �޸���־:
*	2009.04.10 - lpf
*		�޸��˴���������ΧԽ���ж�����;
 */
tagActionList * CFaceActionList::GetActionList(DWORD dwIndex)
{
	if ((int)dwIndex < 0 || dwIndex >= m_vecPlayerActionList.size())
		return NULL;

	return &m_vecPlayerActionList[dwIndex];
}



/*
 * ����: ������̬�����ļ�
 * ժҪ: ��̬�����ļ�������,һ���ǹ�����ҵ�,��һ���ǹ��ڹ����,��Ҫ�ֱ�����
 * ����: pszFileDir - ��̬�����ļ����ڵ�Ŀ¼
 * ����ֵ: �ɹ� - true
 *		   ʧ�� - false
 * ����: lpf
 * ��������: 2008.06.16
 * �޸���־:
 *	2008.06.05 - lpf
 *		�޸��˱��鶯������Ҫ��ͼƬ��Դ�ļ����������,����ͬID����Ҷ������߹��ﶯ�����Թ���ͬһ������ͼƬ��Դ�ļ�
 *	2008.11.11 - lpf
 *		�����µ�����,�����˶����б��ļ�����,�޸�����Ӧ���������
 *	2009.03.14 - lpf
 *		������������鶯��ʱ���ڴ�й¶
 */
bool CFaceActionList::LoadFaceAction(const char * pszFileDir)
{
	string				sFileName  = pszFileDir;
	CRFile			  * pRFile	   = NULL;
	stringstream		streamPLayer;
	stringstream		streamMonster;
	tagFaceAction		stPlayerTmp;
	tagFaceAction		stMonsterTmp;
	render::Interface * pInterface = render::Interface::GetInstance();

	// ���������̬�ļ�
	pRFile = rfOpen((sFileName + "/PlayerAction.ini").c_str());
	if (pRFile == NULL)
	{
		rfClose(pRFile);
		return false;
	}
	pRFile->ReadToStream(streamPLayer);
	rfClose(pRFile);

	// ���֮ǰ�����ͼƬ��Դ
	//for (size_t st = 0; st < m_vecPlayerFaceAction.size(); ++st)
	//	pInterface->DestroyTexture(m_vecPlayerFaceAction[st].dwFaceFileHandle);

	m_vecPlayerFaceAction.clear();

	// ���������̬��Ϣ
	while (ReadTo(streamPLayer, "*"))
	{
		streamPLayer>>stPlayerTmp.dwID
					>>stPlayerTmp.sCallStr
					>>stPlayerTmp.sNote

					>>stPlayerTmp.sActionFileName
					>>stPlayerTmp.bPlayInWater

					//>>stPlayerTmp.dwFaceCount
					//>>stPlayerTmp.sFaceFileName
					//>>stPlayerTmp.dwFaceStartIndex
					//>>stPlayerTmp.dwFaceEndIndex
					//>>stPlayerTmp.dwFaceInterval

					>>stPlayerTmp.sShowTextTarget
					>>stPlayerTmp.sShowTextNoTarget;

		//if (stPlayerTmp.sAction != "0")
		//	stPlayerTmp.sAction.erase(0, 1);

/*		if (stPlayerTmp.sFaceFileName != "0")
		{
			bool bIsFound = false;

			// ��ѯ�Ƿ��Ѿ������˸ñ�������
			for (DWORD dw = 0; dw < m_vecPlayerFaceAction.size(); ++dw)
			{
				if (m_vecPlayerFaceAction[dw].sFaceFileName == stPlayerTmp.sFaceFileName)
				{
					bIsFound = true;
					stPlayerTmp.dwFaceFileHandle = m_vecPlayerFaceAction[dw].dwFaceFileHandle;
					break;
				}
			}

			if (!bIsFound)
				stPlayerTmp.dwFaceFileHandle = pInterface->CreateTexture((string("pictures/face/") + stPlayerTmp.sFaceFileName + ".tga").c_str());
		}else
			stPlayerTmp.dwFaceFileHandle = 0;*/

		stPlayerTmp.dwFaceFileHandle = 0;

		if (stPlayerTmp.sActionFileName != "0")
		{
			bool bIsFound = false;

			// ��ѯ�Ƿ��Ѿ������˸ö����б�
			for (DWORD dw = 0; dw < m_vecPlayerFaceAction.size(); ++dw)
			{
				if (m_vecPlayerFaceAction[dw].sActionFileName == stPlayerTmp.sActionFileName)
				{
					bIsFound = true;
					stPlayerTmp.iActionIndex = m_vecPlayerFaceAction[dw].iActionIndex;
					break;
				}
			}

			if (!bIsFound)
			{
				string sFileTmp = sFileName + "/";
				sFileTmp += stPlayerTmp.sActionFileName;
				sFileTmp += ".ini";
				if (LoadActionList(sFileTmp))
					stPlayerTmp.iActionIndex = (int)m_vecPlayerActionList.size() - 1;
				else
				{
					stPlayerTmp.sActionFileName = "0";
					stPlayerTmp.iActionIndex = -1;
				}
			}
		}else
			stPlayerTmp.iActionIndex = -1;

		m_vecPlayerFaceAction.push_back(stPlayerTmp);
	}

	// ���������̬�ļ�
	pRFile = rfOpen((sFileName + "/MonsterAction.ini").c_str());
	if (pRFile == NULL)
	{
		rfClose(pRFile);
		return false;
	}
	pRFile->ReadToStream(streamMonster);
	rfClose(pRFile);

	// ���֮ǰ�����ͼƬ��Դ
	for (size_t st = 0; st < m_vecMonsterFaceAction.size(); ++st)
		pInterface->DestroyTexture(m_vecMonsterFaceAction[st].dwFaceFileHandle);

	m_vecMonsterFaceAction.clear();

	// ������̬��Ϣ��������Ӧ��ͼƬ��Դ
	while (ReadTo(streamMonster, "*"))
	{
		streamMonster>>stMonsterTmp.dwID
					 >>stMonsterTmp.sNote

					 >>stMonsterTmp.sActionFileName

					 >>stMonsterTmp.dwFaceCount
					 >>stMonsterTmp.sFaceFileName
					 >>stMonsterTmp.dwFaceStartIndex
					 >>stMonsterTmp.dwFaceEndIndex
					 >>stMonsterTmp.dwFaceInterval

					 >>stMonsterTmp.sShowTextTarget
					 >>stMonsterTmp.sShowTextNoTarget;

		//if (stMonsterTmp.sActionFileName != "0")
			//stMonsterTmp.sActionFileName.erase(0, 1);

		if (stMonsterTmp.sFaceFileName != "0")
		{
			bool bIsFound = false;

			// ��ѯ�Ƿ��Ѿ������˸ñ�������(����ı���ͼƬ�ļ����ܺ����һ��)
			for (DWORD dw = 0; dw < m_vecPlayerFaceAction.size(); ++dw)
			{
				if (m_vecPlayerFaceAction[dw].sFaceFileName == stMonsterTmp.sFaceFileName)
				{
					bIsFound = true;
					stMonsterTmp.dwFaceFileHandle = m_vecPlayerFaceAction[dw].dwFaceFileHandle;
					break;
				}
			}

			if (!bIsFound)
			{
				// ��ѯ�Ƿ��Ѿ������˸ñ�������(Ȼ����������������ļ�)
				for (DWORD dw = 0; dw < m_vecMonsterFaceAction.size(); ++dw)
				{
					if (m_vecMonsterFaceAction[dw].sFaceFileName == stMonsterTmp.sFaceFileName)
					{
						bIsFound = true;
						stMonsterTmp.dwFaceFileHandle = m_vecMonsterFaceAction[dw].dwFaceFileHandle;
						break;
					}
				}
			}

			if (!bIsFound)
				stMonsterTmp.dwFaceFileHandle = pInterface->CreateTexture((string("pictures/face/") + stMonsterTmp.sFaceFileName + ".tga").c_str());
		}else
			stMonsterTmp.dwFaceFileHandle = 0;

		if (stMonsterTmp.sActionFileName != "0")
		{
			bool bIsFound = false;

			// ��ѯ�Ƿ��Ѿ������˸ö����б�
			for (DWORD dw = 0; dw < m_vecPlayerFaceAction.size(); ++dw)
			{
				if (m_vecPlayerFaceAction[dw].sActionFileName == stMonsterTmp.sActionFileName)
				{
					bIsFound = true;
					stMonsterTmp.iActionIndex = m_vecPlayerFaceAction[dw].iActionIndex;
					break;
				}
			}

			if (!bIsFound)
			{
				for (DWORD dw = 0; dw < m_vecMonsterFaceAction.size(); ++dw)
				{
					if (m_vecMonsterFaceAction[dw].sActionFileName == stMonsterTmp.sActionFileName)
					{
						bIsFound = true;
						stMonsterTmp.iActionIndex = m_vecMonsterFaceAction[dw].iActionIndex;
						break;
					}
				}
			}

			if (!bIsFound)
			{
				string sFileTmp = sFileName + "/";
				sFileTmp += stMonsterTmp.sActionFileName;
				sFileTmp += ".ini";
				if (LoadActionList(sFileTmp))
					stMonsterTmp.iActionIndex = (int)m_vecPlayerActionList.size() - 1;
				else
				{
					stMonsterTmp.sActionFileName = "0";
					stMonsterTmp.iActionIndex = -1;
				}
			}
		}else
			stMonsterTmp.iActionIndex = -1;

		m_vecMonsterFaceAction.push_back(stMonsterTmp);
	}

	return true;
}



/*
 * ����: ������Ҷ�������
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.11.04
 * �޸���־:
 *	2008.11.17 - lpf
 *		�ж�����������б������ļ���û����ЧID��λ����"#",��ֱ�ӷ���
 */
bool CFaceActionList::LoadActionList(const string & sFilePath)
{
	stringstream  stream;
	tagActionDec  stActionDec;
	tagActionList stActionList;

	CRFile * pActionFile = rfOpen(sFilePath.c_str());
	if (pActionFile == NULL)
	{
		rfClose(pActionFile);
		return false;
	}

	pActionFile->ReadToStream(stream);
	rfClose(pActionFile);

	// �������Ч��Ϣ
	string sLocator;
	if (ReadTo(stream, "#"))
	{
		stream	>>	stActionList.dwIdEffect
				>>	stActionList.dwStartTime
				>>	stActionList.bIsLoop
				>>	sLocator;
	}else
	{
		rfClose(pActionFile);
		return false;
	}

	if (sLocator == "WIND")					//�ؿ�
		stActionList.eLocator = MAKEFOURCC('0','0','0','0');
	else if (sLocator == "HEAD")			//ͷ��
		stActionList.eLocator = MAKEFOURCC('0','0','0','1');
	else if (sLocator == "FOOTCENTER")		//�ŵ�����
		stActionList.eLocator = MAKEFOURCC('0','0','0','2');
	else if (sLocator == "LEFTHAND")		//����
		stActionList.eLocator = MAKEFOURCC('0','0','0','3');
	else if (sLocator == "RIGHTHAND")		//����
		stActionList.eLocator = MAKEFOURCC('0','0','0','4');
	else									//Ĭ��Ϊ�ؿ�
		stActionList.eLocator = MAKEFOURCC('0','0','0','0');

	while (ReadTo(stream, "*"))
	{
		stream	>>	stActionDec.sAction
				>>	stActionDec.iLoopCount;

		if (stActionDec.sAction != "0")
			stActionDec.sAction.erase(0, 1);

		stActionList.m_vecActionDecList.push_back(stActionDec);
	}

	m_vecPlayerActionList.push_back(stActionList);

	return true;
}



/*
 * ����: �ͷ���̬�б�
 * ժҪ: �ú�������ע�����,���ͷ���Դ
 *		 �ڸ��������������Ҳ�����˸ú���
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.06.16
 * �޸���־:
 *	2008.11.11 - lpf
 *		�����������̬�����б���ͷ�
 */
void CFaceActionList::Release()
{
	render::Interface * pInterface = render::Interface::GetInstance();

	// ��������ͼƬ��Դ
	for (size_t st = 0; st < m_vecPlayerFaceAction.size(); ++st)
		pInterface->DestroyTexture(m_vecPlayerFaceAction[st].dwFaceFileHandle);
	m_vecPlayerFaceAction.clear();

	for (size_t st = 0; st < m_vecPlayerActionList.size(); ++st)
		m_vecPlayerActionList[st].m_vecActionDecList.clear();
	m_vecPlayerActionList.clear();

	for (size_t st = 0; st < m_vecMonsterFaceAction.size(); ++st)
		pInterface->DestroyTexture(m_vecMonsterFaceAction[st].dwFaceFileHandle);
	m_vecMonsterFaceAction.clear();
}



/*
 * ����: �趨��̬����
 * ժҪ: ͨ���ú��������Զ��趨��̬��������ǰ���趨,�趨�Ķ������Զ�������趨��ʼ������̬����
 * ����: dwFaceActionID - ��Ҫ���ŵ���̬����ID
 *		 pSourceObj		- Դ�����߶���
 *		 lSourceObjType - Դ�����߶�������
 *		 pTargetObj		- Ŀ�겥�Ŷ���
 *		 lTargetObjType - Ŀ�겥�Ŷ�������
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.06.16
 * �޸���־:
 *	2008.11.11 - lpf
 *		�����������̬�������Ź��������
 *	2008.11.28 - lpf
 *		�������������״̬���ܲ�����̬������������
 *		����ҵ���Լ�������̬����ʱ��������Ϣ��Ĭ��Ϊû��Ŀ������
 *	2008.12.29 - lpf
 *		���������ս��״̬���ܲ�����̬������������
 *	2009.03.25 - lpf
 *		�������Ʒ������µĳ�Ա����IsCanPlay()��
 *	2009.04.03 - lpf
 *		��"û��˵����Ϣ���ж�"��������,����Ҵ��ڷ���״̬����Ҫ������Ϣ�����
 *	2009.07.01 - lk
 *		������̬�����ͽ�����̬�����Ķ�������ң�����������˸�����Ϣ������ʾ���ֱ�ɡ������ߡ�
 */
void CFaceActionList::SetupFaceAction(DWORD dwFaceActionID, CMoveShape * pSourceObj, long lSourceObjType, CMoveShape * pTargetObj, long lTargetObjType)
{
	string			sChatMsg;
	tagFaceAction * stpFaceAction = NULL;

	// ���Դ����Ϊ����ֱ�ӷ���
	if (pSourceObj == NULL)
		return;

	// �жϲ��õ���Ҫ���ŵ���̬����
	if (lSourceObjType == TYPE_PLAYER)
		stpFaceAction = GetPlayerFaceAction(dwFaceActionID);
	else if (lSourceObjType == TYPE_MONSTER)
		stpFaceAction = GetMonsterFaceAction(dwFaceActionID);
	else
		return; 

	// �жϱ��鶯�������Ƿ�õ���
	if (stpFaceAction == NULL)
		return;

	// �趨���鶯��
	if (stpFaceAction->dwFaceFileHandle != 0 && lSourceObjType != TYPE_PLAYER)
		pSourceObj->SetupFaceAnim(stpFaceAction);

	// �趨����
	if (stpFaceAction->sActionFileName != "0")
	{
		//if (pSourceObj->GetAction() == CShape::ACT_STAND || pSourceObj->GetAction() == CShape::ACT_IDLE)
		pSourceObj->SetupActionAnim(stpFaceAction);
	}

	// ����������Ϣ
	if (pTargetObj == NULL || pTargetObj == pSourceObj)
	{
		sChatMsg = stpFaceAction->sShowTextNoTarget;
	}else
		sChatMsg = stpFaceAction->sShowTextTarget;

	// û��˵����Ϣ���ж�
	if (sChatMsg == "0" || ((CPlayer *)pSourceObj)->IsPlayerShopOpened())
		return;

	string strSouName = "";
	if(pSourceObj)
		strSouName = pSourceObj->GetName();
	string strTargName = "";
	if(pTargetObj)
		strTargName = pTargetObj->GetName();
	// ������̬�����ͽ�����̬�����Ķ�������ң�����������˸�����Ϣ������ʾ���ֱ�ɡ������ߡ�
	CPlayer * pMainPlayer = GetGame()->GetMainPlayer();
	if(pMainPlayer)
	{
		if(lSourceObjType == TYPE_PLAYER && pMainPlayer != pSourceObj)
		{
			// ����
			if(pMainPlayer->IsTeammate(pSourceObj->GetExID()))
			{
				tagWSMemberInfo * stInfo = pMainPlayer->GetWsMemberInfo(pSourceObj->GetExID());
				if(stInfo)
					strSouName = stInfo->szName;
			}
		}
		if(lTargetObjType == TYPE_PLAYER && pMainPlayer != pTargetObj)
		{
			// ����
			if(pMainPlayer->IsTeammate(pTargetObj->GetExID()))
			{
				tagWSMemberInfo * stInfo = pMainPlayer->GetWsMemberInfo(pTargetObj->GetExID());
				if(stInfo)
					strTargName = stInfo->szName;
			}
		}
	}

	// ����������Ϣ
	while (true)
	{
		size_t st = sChatMsg.find("%");
		if (st == string::npos)
			break;

		if (sChatMsg[st + 1] == 'M')
			sChatMsg.replace(st, 2, strSouName.c_str());
		else if (sChatMsg[st + 1] == 'T')
			sChatMsg.replace(st, 2, strTargName.c_str());
	}

	// ���������Ϣ����ҳ�������б���
	//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(sChatMsg.c_str());
}



/*
 * ����: ����Ƿ���Բ���ָ������̬����
 * ժҪ: -
 * ����: dwFaceActionID - ��Ҫ���ŵ���̬����ID
 *		 pSourceObj		- Դ�����߶���
 *		 lSourceObjType - Դ�����߶�������
 * ����ֵ: true  - ���Բ���
 *		   false - ���ܲ���
 * ����: lpf
 * ��������: 2009.03.25
 * �޸���־:
*	2009.04.10 - lpf
*		��"�õ������̬������Ϣ"�����˷���ָ��Ϊ�յ��ж�;
 */
bool CFaceActionList::IsCanPlay(DWORD dwFaceActionID, CMoveShape * pSourceObj, long lSourceObjType)
{
	tagFaceAction * stpFaceAction = NULL;

	// �жϲ��õ���Ҫ���ŵ���̬����
	if (lSourceObjType == TYPE_PLAYER)
	{
		CPlayer * pPlayer = dynamic_cast<CPlayer *>(pSourceObj);
		if (pPlayer)
		{
			if (pPlayer->GetState() == CShape::STATE_FIGHT)			//ս��״̬���Ų���
			{
				//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
				//													CStringReading::LoadText(eIDS_CONTROL, eIDS_CONTROL_CANTFACEACTION_AT_FIGHT),
				//													D3DCOLOR_ARGB(255, 255, 0, 0));
				return false;
			}if (pPlayer->IsRide())		//����״̬���Ų���
			{
				//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
				//													CStringReading::LoadText(eIDS_CONTROL, eIDS_CONTROL_CANTFACEACTION_ON_RIDE),
				//													D3DCOLOR_ARGB(255, 255, 0, 0));
				return false;
			}else if (pPlayer->GetIntonateKey() != 0)		//����״̬���Ų���
			{
				//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
				//													CStringReading::LoadText(eIDS_CONTROL, eIDS_CONTROL_CANTFACEACTION_AT_CURRENT),
				//													D3DCOLOR_ARGB(255, 255, 0, 0));
				return false;
			}
		}else
			return false;

		// �õ������̬������Ϣ
		stpFaceAction = GetPlayerFaceAction(dwFaceActionID);
		if (stpFaceAction == NULL)
			return false;

		if (!stpFaceAction->bPlayInWater && (GetGame()->GetRegion()->GetSpaceType() == RST_Warter) )			//ˮ��״̬�����ж�
		{
			//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
			//													CStringReading::LoadText(eIDS_CONTROL, eIDS_CONTROL_CANTFACEACTION_IN_WATER),
			//													D3DCOLOR_ARGB(255, 255, 0, 0));
			return false;
		}
	}

	return true;
}