//=============================================================================
/**
 *  file: ClientViewFun.cpp
 *
 *  Brief:�����˺Ϳͻ��˽������ͻ��˿�����صĽű�����
 *			
 *
 *  Authtor:wangqiao
 *	
 *	Create Datae:2008-11-06
 */
//=============================================================================

#include "StdAfx.h"
#include "mmsystem.h"
#include "script.h"
#include "variablelist.h"
#include "../Player.h"
#include "../serverregion.h"
#include "../../../setup/LogSystem.h"
#include "../public/StrType.h"
#include "../public/StrUnifyInput.h"


double CScript::AddInfo(const char* CmdStr, char* retStr)
{
	char *text=GetStringParam(CmdStr, 0);
	if(text==NULL) return 0;

	char szOutPut[128];
	if(text[0] == '@') // ��GUID����
	{
		// �����±�ֵ
		int lArray = CVariableList::VAR_NOT_FOUND;
		char MainVarName[256];
		char InnVarName[256];

		CGUID tGuid;
		char* MainVarNamePtr = MainVarName;
		char* InnVarNamePtr = InnVarName;
		char* varTypeFlag = (char*)text;
		long tempPos = 0;

		double ret = CheckParamStringForArray(varTypeFlag, strlen(text), &MainVarNamePtr, 256, &InnVarNamePtr, 256, tempPos);
		if(1.0f == ret)
		{
			if(InnVarNamePtr[0] == '$') // �±��Ǳ���
				lArray = GetScriptVarValue((CMoveShape*)p_SrcShape, InnVarName, 0);
			else
				lArray = atoi(InnVarName);

			char szStrNewName[128];
			_snprintf(szStrNewName, 128, "%s%d", MainVarNamePtr, lArray);
			GetScriptGuidValue((CMoveShape*)p_SrcShape, szStrNewName).tostring(szOutPut);
		}
		else if(0.0f == ret)
		{
			GetScriptGuidValue((CMoveShape*)p_SrcShape, text).tostring(szOutPut);
		}
	}

	DWORD color = GetIntParam(CmdStr, 1);
	DWORD bkColor = 0;

	if (color==ERROR_CODE)
	{
		color = 0xffffffff;
	}
	else
	{
		bkColor = GetIntParam(CmdStr, 2);
		if (bkColor==ERROR_CODE)
		{
			bkColor = 0;
		}
	}

	char* strContent=NULL;
	if(text[0] == '@')
		strContent=szOutPut;
	else
		strContent=text;
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer)
	{
		pPlayer->SendNotifyMessage(strContent,color,bkColor,eNOTIFYPOS_LEFT_BOTTOM);
	}

	M_FREE( text, sizeof(char)*MAX_VAR_LEN );
	return 1;
}


double CScript::AddDestInfo(const char* CmdStr, char* retStr)
{
	char *text=GetStringParam(CmdStr, 0);
	if(text==NULL) return 0;

	char szOutPut[128];
	if(text[0] == '@') // ��GUID����
	{
		// �����±�ֵ
		int lArray = CVariableList::VAR_NOT_FOUND;
		char MainVarName[256];
		char InnVarName[256];

		CGUID tGuid;
		char* MainVarNamePtr = MainVarName;
		char* InnVarNamePtr = InnVarName;
		char* varTypeFlag = (char*)text;
		long tempPos = 0;

		double ret = CheckParamStringForArray(varTypeFlag, strlen(text), &MainVarNamePtr, 256, &InnVarNamePtr, 256, tempPos);
		if(1.0f == ret)
		{
			if(InnVarNamePtr[0] == '$') // �±��Ǳ���
				lArray = GetScriptVarValue((CMoveShape*)p_SrcShape, InnVarName, 0);
			else
				lArray = atoi(InnVarName);

			char szStrNewName[128];
			_snprintf(szStrNewName, 128, "%s%d", MainVarNamePtr, lArray);
			GetScriptGuidValue((CMoveShape*)p_SrcShape, szStrNewName).tostring(szOutPut);
		}
		else if(0.0f == ret)
		{
			GetScriptGuidValue((CMoveShape*)p_SrcShape, text).tostring(szOutPut);
		}
	}

	DWORD color = GetIntParam(CmdStr, 1);
	DWORD bkColor = 0;

	if (color==ERROR_CODE)
	{
		color = 0xffffffff;
	}
	else
	{
		bkColor = GetIntParam(CmdStr, 2);
		if (bkColor==ERROR_CODE)
		{
			bkColor = 0;
		}
	}

	char* strContent=NULL;
	if(text[0] == '@')
		strContent=szOutPut;
	else
		strContent=text;
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_DesShape);
	if(pPlayer)
	{
		pPlayer->SendNotifyMessage(strContent,color,bkColor,eNOTIFYPOS_LEFT_BOTTOM);
	}

	M_FREE( text, sizeof(char)*MAX_VAR_LEN );
	return 1;
}

double CScript::AddInfoByIndex(const char* cmd, char* retStr)
{
	DWORD dwTextType =GetIntParam(cmd, 0);
	DWORD dwTextIndex= GetIntParam(cmd, 1);
	if (dwTextType==INVILID_VALUE || dwTextType== ERROR_CODE|| dwTextIndex==INVILID_VALUE || dwTextIndex== ERROR_CODE)
		return INVILID_VALUE;

	DWORD color = GetIntParam(cmd, 2);
	DWORD bkColor = 0;

	if (color==ERROR_CODE)
	{
		color = 0xffffffff;
	}
	else
	{
		bkColor = GetIntParam(cmd, 3);
		if (bkColor==ERROR_CODE)
		{
			bkColor = 0;
		}
	}


	/*�޸ĵ�ʱ�������ִ�����ʽ������
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer)
	{
	pPlayer->SendNotifyMessage()
	}*/


	//SAFE_DELETE_ARRAY(text);

	return 1;
}

double CScript::GameMessage(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;

	DelPlayerTalkBoxScript(m_guSrcID, SR_FUNC_GAMEMESSAGE);

	//�ȴ�����
	SetIsHang(true);
	SetHangFunc(SR_FUNC_GAMEMESSAGE);

	char *text = GetStringParam(CmdStr, 0);
	if(text == NULL) return 0;

	double lType = GetIntParam(CmdStr, 1);

	CMessage msg(MSG_S2C_OTHER_GAMEMESSAGE);
	msg.Add(text);
	msg.Add((long)lType);
	msg.Add(GetID());
	msg.SendToPlayer(p_SrcShape->GetExID());

	M_FREE( text, sizeof(char)*MAX_VAR_LEN );
	return 1;
}
double CScript::TalkBox(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;

	DelPlayerTalkBoxScript(m_guSrcID, SR_FUNC_TALKBOX);
	//�ȴ�����
	SetIsHang(true);
	SetHangFunc(SR_FUNC_TALKBOX);

	char* info = GetStringParam(CmdStr, 0);
	if (info==NULL) return 0;

	char* button_1		=GetStringParam(CmdStr,1);
	BYTE  b1_useable	=(BYTE)GetIntParam(CmdStr,2);
	char* button_2		=GetStringParam(CmdStr,3);
	BYTE b2_useable		=(BYTE)GetIntParam(CmdStr,4);

	LONG InterfaceID	=GetIntParam(CmdStr,5);
	LONG IsCloseWindow	=GetIntParam(CmdStr,6);
	LONG PicID			=GetIntParam(CmdStr,7);
	LONG PicX			=GetIntParam(CmdStr,8);
	LONG PicY			=GetIntParam(CmdStr,9);	
	LONG MusicID		=GetIntParam(CmdStr,10);
	LONG SoundID		=GetIntParam(CmdStr,11);
	//
	if(InterfaceID==INVILID_VALUE ||InterfaceID==ERROR_CODE )
		InterfaceID=1;
	if(  IsCloseWindow==INVILID_VALUE||IsCloseWindow==ERROR_CODE )
		IsCloseWindow	=0;
	if(PicID ==INVILID_VALUE || PicID == ERROR_CODE )
		PicID=0;
	if(  PicX ==INVILID_VALUE ||PicX == ERROR_CODE )
		PicX=0;
	if(PicY ==INVILID_VALUE ||PicY == ERROR_CODE )
		PicY=0;	
	if(MusicID ==INVILID_VALUE||MusicID == ERROR_CODE )
		MusicID	=0;
	if(SoundID ==INVILID_VALUE || SoundID == ERROR_CODE )
		SoundID	=0;

	// ���ͶԻ���Ϣ
	CMessage msg(MSG_S2C_OTHER_OPENTALKBOX);
	msg.Add((LONG)0);
	msg.Add(GetID());
	msg.Add(info);	

	if (button_1)
		msg.Add(button_1);
	else
		msg.Add("");
	msg.Add(b1_useable);
	if (button_2)
		msg.Add(button_2);
	else
		msg.Add("");
	msg.Add(b2_useable);

	msg.Add(InterfaceID);
	msg.Add((BYTE)IsCloseWindow);
	msg.Add(PicID);
	msg.Add(PicX);	
	msg.Add(PicY);	
	msg.Add(SoundID);	
	msg.Add(MusicID);
	msg.SendToPlayer(p_SrcShape->GetExID());

	M_FREE( info, sizeof(char)*MAX_VAR_LEN );
	M_FREE( button_1, sizeof(char)*MAX_VAR_LEN );
	M_FREE( button_2, sizeof(char)*MAX_VAR_LEN );

	return 1;
}

//! ����ͬTalkBox
//! ��TalkBox�ĵ�һ������֮�������������ַ���������һ�����ڱ�ʾ�Ի��������ͼƬ��һ����ʾ�Ի��򱳾�ͼƬ
double CScript::PicTalkBox(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;

	DelPlayerTalkBoxScript(m_guSrcID, SR_FUNC_TALKBOX);
	//�ȴ�����
	SetIsHang(true);
	SetHangFunc(SR_FUNC_TALKBOX);

	char* info = GetStringParam(CmdStr, 0);
	if (info==NULL) return 0;

	//! ������ͼƬ
	char *pTopPicPath = GetStringParam(CmdStr, 1);
	//! ����ͼƬ
	char *pBackPicPath = GetStringParam(CmdStr, 2);


	char* button_1		=GetStringParam(CmdStr,3);
	BYTE  b1_useable	=(BYTE)GetIntParam(CmdStr,4);
	char* button_2		=GetStringParam(CmdStr,5);
	BYTE b2_useable		=(BYTE)GetIntParam(CmdStr,6);

	LONG InterfaceID	=GetIntParam(CmdStr,7);
	LONG IsCloseWindow	=GetIntParam(CmdStr,8);
	LONG PicID			=GetIntParam(CmdStr,9);
	LONG PicX			=GetIntParam(CmdStr,10);
	LONG PicY			=GetIntParam(CmdStr,11);	
	LONG MusicID		=GetIntParam(CmdStr,12);
	LONG SoundID		=GetIntParam(CmdStr,13);
	//
	if(InterfaceID==INVILID_VALUE ||InterfaceID==ERROR_CODE )
		InterfaceID=1;
	if(  IsCloseWindow==INVILID_VALUE||IsCloseWindow==ERROR_CODE )
		IsCloseWindow	=0;
	if(PicID ==INVILID_VALUE || PicID == ERROR_CODE )
		PicID=0;
	if(  PicX ==INVILID_VALUE ||PicX == ERROR_CODE )
		PicX=0;
	if(PicY ==INVILID_VALUE ||PicY == ERROR_CODE )
		PicY=0;	
	if(MusicID ==INVILID_VALUE||MusicID == ERROR_CODE )
		MusicID	=0;
	if(SoundID ==INVILID_VALUE || SoundID == ERROR_CODE )
		SoundID	=0;

	// ���ͶԻ���Ϣ
	CMessage msg(MSG_S2C_OTHER_OPEN_PIC_TALKBOX);
	msg.Add((LONG)0);
	msg.Add(GetID());
	msg.Add(info);

	if (NULL != pTopPicPath)
		msg.Add(pTopPicPath);
	else
		msg.Add("");
	if (NULL != pBackPicPath)
		msg.Add(pBackPicPath);
	else
		msg.Add("");
	if (NULL != button_1)
		msg.Add(button_1);
	else
		msg.Add("");
	msg.Add(b1_useable);
	if (NULL != button_2)
		msg.Add(button_2);
	else
		msg.Add("");

	msg.Add(b2_useable);
	msg.Add(InterfaceID);
	msg.Add((BYTE)IsCloseWindow);
	msg.Add(PicID);
	msg.Add(PicX);	
	msg.Add(PicY);	
	msg.Add(SoundID);	
	msg.Add(MusicID);

	msg.SendToPlayer(p_SrcShape->GetExID());

	M_FREE( info, sizeof(char)*MAX_VAR_LEN );
	M_FREE( pTopPicPath, sizeof(char)*MAX_VAR_LEN );
	M_FREE( pBackPicPath, sizeof(char)*MAX_VAR_LEN );
	M_FREE( button_1, sizeof(char)*MAX_VAR_LEN );
	M_FREE( button_2, sizeof(char)*MAX_VAR_LEN );

	return 1;
}

double CScript::EndTalkBox(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;
	// ���ͶԻ���Ϣ
	CMessage msg(MSG_S2C_OTHER_OPENTALKBOX);
	msg.Add((LONG)0);
	msg.Add(GetID());
	msg.Add("");			// ����
	msg.Add((BYTE)0);
	msg.SendToPlayer(p_SrcShape->GetExID());
	return 1;
}
double CScript::TalkBoxByIndex(const char* cmd, char* retStr)
{
	if(p_SrcShape == NULL) return 0;

	DelPlayerTalkBoxScript(m_guSrcID, SR_FUNC_TALKBOX);
	//�ȴ�����
	SetIsHang(true);
	SetHangFunc(SR_FUNC_TALKBOX);

	LONG TextType = GetIntParam(cmd, 0);
	if (TextType==INVILID_VALUE || TextType== ERROR_CODE)
		return INVILID_VALUE;

	LONG TextIndex = GetIntParam(cmd, 1);
	if (TextIndex==INVILID_VALUE || TextIndex== ERROR_CODE)
		return INVILID_VALUE;

	char* button_1		=GetStringParam(cmd,2);
	BYTE  b1_useable	=(BYTE)GetIntParam(cmd,3);
	char* button_2		=GetStringParam(cmd,4);
	BYTE  b2_useable	=(BYTE)GetIntParam(cmd,5);

	LONG InterfaceID	=GetIntParam(cmd,6);
	LONG IsCloseWindow	=GetIntParam(cmd,7);
	LONG PicID			=GetIntParam(cmd,8);
	LONG PicX			=GetIntParam(cmd,9);
	LONG PicY			=GetIntParam(cmd,10);	
	LONG MusicID		=GetIntParam(cmd,11);
	LONG SoundID		=GetIntParam(cmd,12);

	if(InterfaceID==INVILID_VALUE ||InterfaceID==ERROR_CODE )
		InterfaceID=1;
	if(  IsCloseWindow==INVILID_VALUE||IsCloseWindow==ERROR_CODE )
		IsCloseWindow=0;
	if(PicID ==INVILID_VALUE || PicID == ERROR_CODE )
		PicID=0;
	if(  PicX ==INVILID_VALUE ||PicX == ERROR_CODE )
		PicX=0;
	if(PicY ==INVILID_VALUE ||PicY == ERROR_CODE )
		PicY=0;	
	if(MusicID ==INVILID_VALUE||MusicID == ERROR_CODE )
		MusicID	=0;
	if(SoundID ==INVILID_VALUE || SoundID == ERROR_CODE )
		SoundID	=0;

	// ���ͶԻ���Ϣ
	CMessage msg(MSG_S2C_OTHER_OPENTALKBOX);
	msg.Add((LONG)1);
	msg.Add(GetID());
	msg.Add(TextType);
	msg.Add(TextIndex);			// ����
	//
	if (button_1)
		msg.Add(button_1);
	else
		msg.Add("");
	msg.Add(b1_useable);
	if (button_2)
		msg.Add(button_2);
	else
		msg.Add("");
	msg.Add(b2_useable);
	//
	msg.Add(InterfaceID);
	msg.Add((BYTE)IsCloseWindow);
	msg.Add(PicID);
	msg.Add(PicX);	
	msg.Add(PicY);	
	msg.Add(MusicID);
	msg.Add(SoundID);

	msg.SendToPlayer(p_SrcShape->GetExID());
	M_FREE( button_1, sizeof(char)*MAX_VAR_LEN );
	M_FREE( button_2, sizeof(char)*MAX_VAR_LEN );

		return 1;
}

double CScript::StoryTalkBox(const char* cmd, char* retStr)
{
	if(p_SrcShape == NULL) return 0;

	DelPlayerTalkBoxScript(m_guSrcID, SR_FUNC_TALKBOX);
	//�ȴ�����
	SetIsHang(true);
	SetHangFunc(SR_FUNC_TALKBOX);

	char* info = GetStringParam(cmd, 0);
	if (info==NULL) return 0;

	BOOL PointContinu	=GetIntParam(cmd,1);
	BOOL EndTalkBox		=GetIntParam(cmd,2);
	LONG SoundID		=GetIntParam(cmd,3);
	LONG MusicID		=GetIntParam(cmd,4);
	LONG HeadID			=GetIntParam(cmd,5);
	char* NpcName		=GetStringParam(cmd,6);
	LONG FaceID_1		=GetIntParam(cmd,7);
	LONG FaceTimes_1	=GetIntParam(cmd,8);
	LONG FaceID_2		=GetIntParam(cmd,9);
	LONG FaceTimes_2	=GetIntParam(cmd,10);
	LONG FaceID_3		=GetIntParam(cmd,11);
	LONG FaceTimes_3	=GetIntParam(cmd,12);
	LONG FaceID_4		=GetIntParam(cmd,13);
	LONG FaceTimes_4	=GetIntParam(cmd,14);
	LONG FaceID_5		=GetIntParam(cmd,15);
	LONG FaceTimes_5	=GetIntParam(cmd,16);

	if(PointContinu==INVILID_VALUE ||PointContinu==ERROR_CODE )
		PointContinu=0;
	if(EndTalkBox==INVILID_VALUE ||EndTalkBox==ERROR_CODE )
		EndTalkBox=0;
	if(SoundID ==INVILID_VALUE || SoundID == ERROR_CODE )
		SoundID	=0;
	if(MusicID ==INVILID_VALUE||MusicID == ERROR_CODE )
		MusicID	=0;
	if(HeadID ==INVILID_VALUE || HeadID== ERROR_CODE )
		HeadID	=0;
	if(FaceID_1  ==INVILID_VALUE ||FaceID_1== ERROR_CODE )
		FaceID_1=0;
	if( FaceTimes_1  ==INVILID_VALUE ||  FaceTimes_1  == ERROR_CODE )
		FaceTimes_1	=0;
	if(FaceID_2 ==INVILID_VALUE || FaceID_2   == ERROR_CODE )
		FaceID_2=0;		
	if(FaceTimes_2   ==INVILID_VALUE ||  FaceTimes_2  == ERROR_CODE )
		FaceTimes_2	=0;
	if(  FaceID_3  ==INVILID_VALUE || FaceID_3   == ERROR_CODE )
		FaceID_3=0;		
	if(  FaceTimes_3  ==INVILID_VALUE ||  FaceTimes_3  == ERROR_CODE )
		FaceTimes_3	=0;
	if(  FaceID_4  ==INVILID_VALUE ||  FaceID_4  == ERROR_CODE )
		FaceID_4=0;		
	if(   FaceTimes_4 ==INVILID_VALUE ||  FaceTimes_4  == ERROR_CODE )
		FaceTimes_4	=0;
	if(  FaceID_5  ==INVILID_VALUE || FaceID_5   == ERROR_CODE )
		FaceID_5=0;		
	if(  FaceTimes_5  ==INVILID_VALUE ||  FaceTimes_5  == ERROR_CODE )
		FaceTimes_5	=0;

	// ���ͶԻ���Ϣ
	CMessage msg(MSG_S2C_OTHER_OPENSTORYTALKBOX);
	msg.Add((LONG)0);
	msg.Add(GetID());
	msg.Add(info);
	msg.Add((BYTE)PointContinu);	
	msg.Add((BYTE)EndTalkBox);	

	msg.Add(SoundID);	
	msg.Add(MusicID);
	msg.Add(HeadID);
	if (NpcName)
		msg.Add(NpcName);
	else
		msg.Add("");
	msg.Add(FaceID_1);
	msg.Add(FaceTimes_1	);
	msg.Add(FaceID_2);
	msg.Add(FaceTimes_2	);
	msg.Add(FaceID_3);
	msg.Add(FaceTimes_3	);
	msg.Add(FaceID_4);
	msg.Add(FaceTimes_4	);
	msg.Add(FaceID_5);
	msg.Add(FaceTimes_5);
	//
	msg.SendToPlayer(p_SrcShape->GetExID());

	M_FREE( info, sizeof(char)*MAX_VAR_LEN );
	M_FREE( NpcName, sizeof(char)*MAX_VAR_LEN );
	return 1;

}
double CScript::StoryTalkBoxByIndex(const char* cmd, char* retStr)
{
	if(p_SrcShape == NULL) return 0;

	DelPlayerTalkBoxScript(m_guSrcID, SR_FUNC_TALKBOX);
	//�ȴ�����
	SetIsHang(true);
	SetHangFunc(SR_FUNC_TALKBOX);

	LONG TextType = GetIntParam(cmd, 0);
	if (TextType==INVILID_VALUE || TextType== ERROR_CODE)
		return INVILID_VALUE;

	LONG TextIndex = GetIntParam(cmd, 1);
	if (TextIndex==INVILID_VALUE || TextIndex== ERROR_CODE)
		return INVILID_VALUE;

	BOOL PointContinu	=GetIntParam(cmd,2);
	BOOL EndTalkBox		=GetIntParam(cmd,3);

	LONG SoundID		=GetIntParam(cmd,4);
	LONG MusicID		=GetIntParam(cmd,5);
	LONG HeadID			=GetIntParam(cmd,6);
	char* NpcName		=GetStringParam(cmd,7);
	LONG FaceID_1		=GetIntParam(cmd,8);
	LONG FaceTimes_1	=GetIntParam(cmd,9);
	LONG FaceID_2		=GetIntParam(cmd,10);
	LONG FaceTimes_2	=GetIntParam(cmd,11);
	LONG FaceID_3		=GetIntParam(cmd,12);
	LONG FaceTimes_3	=GetIntParam(cmd,13);
	LONG FaceID_4		=GetIntParam(cmd,14);
	LONG FaceTimes_4	=GetIntParam(cmd,15);
	LONG FaceID_5		=GetIntParam(cmd,16);
	LONG FaceTimes_5	=GetIntParam(cmd,17);

	if(PointContinu==INVILID_VALUE ||PointContinu==ERROR_CODE )
		PointContinu=0;
	if(EndTalkBox==INVILID_VALUE ||EndTalkBox==ERROR_CODE )
		EndTalkBox=0;
	if(SoundID ==INVILID_VALUE || SoundID == ERROR_CODE )
		SoundID	=0;
	if(MusicID ==INVILID_VALUE||MusicID == ERROR_CODE )
		MusicID	=0;
	if(HeadID ==INVILID_VALUE || HeadID== ERROR_CODE )
		HeadID	=0;
	if(FaceID_1  ==INVILID_VALUE ||FaceID_1== ERROR_CODE )
		FaceID_1=0;
	if( FaceTimes_1  ==INVILID_VALUE ||  FaceTimes_1  == ERROR_CODE )
		FaceTimes_1	=0;
	if(FaceID_2 ==INVILID_VALUE || FaceID_2   == ERROR_CODE )
		FaceID_2=0;		
	if(FaceTimes_2   ==INVILID_VALUE ||  FaceTimes_2  == ERROR_CODE )
		FaceTimes_2	=0;
	if(  FaceID_3  ==INVILID_VALUE || FaceID_3   == ERROR_CODE )
		FaceID_3=0;		
	if(  FaceTimes_3  ==INVILID_VALUE ||  FaceTimes_3  == ERROR_CODE )
		FaceTimes_3	=0;
	if(  FaceID_4  ==INVILID_VALUE ||  FaceID_4  == ERROR_CODE )
		FaceID_4=0;		
	if(   FaceTimes_4 ==INVILID_VALUE ||  FaceTimes_4  == ERROR_CODE )
		FaceTimes_4	=0;
	if(  FaceID_5  ==INVILID_VALUE || FaceID_5   == ERROR_CODE )
		FaceID_5=0;		
	if(  FaceTimes_5  ==INVILID_VALUE ||  FaceTimes_5  == ERROR_CODE )
		FaceTimes_5	=0;

	// ���ͶԻ���Ϣ
	CMessage msg(MSG_S2C_OTHER_OPENSTORYTALKBOX);
	msg.Add((LONG)1);
	msg.Add(GetID());
	msg.Add(TextType);
	msg.Add(TextIndex);			
	msg.Add((BYTE)PointContinu);	
	msg.Add((BYTE)EndTalkBox);	

	msg.Add(SoundID);	
	msg.Add(MusicID);
	msg.Add(HeadID);
	if (NpcName)
		msg.Add(NpcName);
	else
		msg.Add("");
	msg.Add(FaceID_1);
	msg.Add(FaceTimes_1	);
	msg.Add(FaceID_2);
	msg.Add(FaceTimes_2	);
	msg.Add(FaceID_3);
	msg.Add(FaceTimes_3	);
	msg.Add(FaceID_4);
	msg.Add(FaceTimes_4	);
	msg.Add(FaceID_5);
	msg.Add(FaceTimes_5);
	//
	msg.SendToPlayer(p_SrcShape->GetExID());
	M_FREE( NpcName, sizeof(char)*MAX_VAR_LEN );
	return 1;
}

double CScript::EndPixyTalkBox(const char* cmd, char* retStr)
{
	if(p_SrcShape == NULL) return 0;
	// ���ͶԻ���Ϣ
	CMessage msg(MSG_S2C_OTHER_OPENPIXYTALKBOX);
	msg.Add((LONG)0);
	msg.Add(GetID());
	msg.Add("");			// ����
	msg.Add((BYTE)0);
	msg.SendToPlayer(p_SrcShape->GetExID());
	return 1.0f;
}
double CScript::PixyTalkBox(const char* cmd, char* retStr)
{
	if(p_SrcShape == NULL) return 0;

	DelPlayerTalkBoxScript(m_guSrcID, SR_FUNC_TALKBOX);
	//�ȴ�����
	SetIsHang(true);
	SetHangFunc(SR_FUNC_TALKBOX);

	char* info = GetStringParam(cmd, 0);
	if (info==NULL) return 0;


	LONG SoundID		=GetIntParam(cmd,1);
	LONG HeadID			=GetIntParam(cmd,2);

	LONG FaceID_1		=GetIntParam(cmd,3);
	LONG FaceTimes_1	=GetIntParam(cmd,4);
	LONG FaceID_2		=GetIntParam(cmd,5);
	LONG FaceTimes_2	=GetIntParam(cmd,6);
	LONG FaceID_3		=GetIntParam(cmd,7);
	LONG FaceTimes_3	=GetIntParam(cmd,8);
	LONG FaceID_4		=GetIntParam(cmd,9);
	LONG FaceTimes_4	=GetIntParam(cmd,10);
	LONG FaceID_5		=GetIntParam(cmd,11);
	LONG FaceTimes_5	=GetIntParam(cmd,12);

	if (SoundID==INVILID_VALUE || SoundID== ERROR_CODE)
	{
		SoundID=0;
	}

	//1
	if (HeadID==INVILID_VALUE || HeadID== ERROR_CODE)
	{
		HeadID=0;
		FaceID_1=0;
		FaceTimes_1=0;
		FaceID_2=0;
		FaceTimes_2=0;
		FaceID_3=0;	
		FaceTimes_3=0;
		FaceID_4=0;	
		FaceTimes_4=0;
		FaceID_5=0;	
		FaceTimes_5=0;
	}
	else
	{
		if (FaceID_1==INVILID_VALUE || FaceID_1== ERROR_CODE)
		{
			FaceID_1=0; 
		}
		if (FaceTimes_1==INVILID_VALUE || FaceTimes_1==ERROR_CODE)
		{
			FaceTimes_1=0;
		}

		if (FaceID_2==INVILID_VALUE || FaceID_2== ERROR_CODE)
		{
			FaceID_2=0; 
		}
		if (FaceTimes_2=INVILID_VALUE || FaceTimes_2==ERROR_CODE)
		{
			FaceTimes_2=0;
		}

		if (FaceID_3==INVILID_VALUE || FaceID_3== ERROR_CODE)
		{
			FaceID_3=0; 
		}
		if (FaceTimes_3==INVILID_VALUE || FaceTimes_3==ERROR_CODE)
		{
			FaceTimes_3=0;
		}

		if (FaceID_4==INVILID_VALUE || FaceID_4== ERROR_CODE)
		{
			FaceID_4=0; 
		}
		if (FaceTimes_4==INVILID_VALUE || FaceTimes_4==ERROR_CODE)
		{
			FaceTimes_4=0;
		}

		if (FaceID_5==INVILID_VALUE || FaceID_5== ERROR_CODE)
		{
			FaceID_5=0; 
		}
		if (FaceTimes_5==INVILID_VALUE || FaceTimes_5==ERROR_CODE)
		{
			FaceTimes_5=0;
		}
	}

	// ���ͶԻ���Ϣ
	CMessage msg(MSG_S2C_OTHER_OPENPIXYTALKBOX);
	msg.Add((LONG)0);
	msg.Add(GetID());
	msg.Add(info);
	msg.Add(SoundID);	
	msg.Add(HeadID);
	msg.Add(FaceID_1);
	msg.Add(FaceTimes_1);
	msg.Add(FaceID_2);
	msg.Add(FaceTimes_2);
	msg.Add(FaceID_3);
	msg.Add(FaceTimes_3);
	msg.Add(FaceID_4);
	msg.Add(FaceTimes_4);
	msg.Add(FaceID_5);
	msg.Add(FaceTimes_5);

	msg.SendToPlayer(p_SrcShape->GetExID());

	M_FREE( info, sizeof(char)*MAX_VAR_LEN );
	return 1;
}

double CScript::PixyTalkBoxByIndex(const char* cmd, char* retStr)
{
	if(p_SrcShape == NULL) return 0;

	DelPlayerTalkBoxScript(m_guSrcID, SR_FUNC_TALKBOX);
	//�ȴ�����
	SetIsHang(true);
	SetHangFunc(SR_FUNC_TALKBOX);

	LONG TextType = GetIntParam(cmd, 0);
	if (TextType==INVILID_VALUE || TextType== ERROR_CODE)
		return INVILID_VALUE;

	LONG TextIndex = GetIntParam(cmd, 1);
	if (TextIndex==INVILID_VALUE || TextIndex== ERROR_CODE)
		return INVILID_VALUE;


	LONG SoundID		=GetIntParam(cmd,2);
	LONG HeadID			=GetIntParam(cmd,3);

	LONG FaceID_1		=GetIntParam(cmd,4);
	LONG FaceTimes_1	=GetIntParam(cmd,5);
	LONG FaceID_2		=GetIntParam(cmd,6);
	LONG FaceTimes_2	=GetIntParam(cmd,7);
	LONG FaceID_3		=GetIntParam(cmd,8);
	LONG FaceTimes_3	=GetIntParam(cmd,9);
	LONG FaceID_4		=GetIntParam(cmd,10);
	LONG FaceTimes_4	=GetIntParam(cmd,11);
	LONG FaceID_5		=GetIntParam(cmd,12);
	LONG FaceTimes_5	=GetIntParam(cmd,13);

	if (SoundID==INVILID_VALUE || SoundID== ERROR_CODE)
	{
		SoundID=0;
	}

	if (HeadID==INVILID_VALUE || HeadID== ERROR_CODE || HeadID==0)
	{
		HeadID=0;
		FaceID_1=0;
		FaceTimes_1=0;
		FaceID_2=0;
		FaceTimes_2=0;
		FaceID_3=0;	
		FaceTimes_3=0;
		FaceID_4=0;	
		FaceTimes_4=0;
		FaceID_5=0;	
		FaceTimes_5=0;
	}
	else
	{
		if (FaceID_1==INVILID_VALUE || FaceID_1== ERROR_CODE)
		{
			FaceID_1=0; 
		}
		if (FaceTimes_1==INVILID_VALUE || FaceTimes_1==ERROR_CODE)
		{
			FaceTimes_1=0;
		}

		if (FaceID_2==INVILID_VALUE || FaceID_2== ERROR_CODE)
		{
			FaceID_2=0; 
		}
		if (FaceTimes_2=INVILID_VALUE || FaceTimes_2==ERROR_CODE)
		{
			FaceTimes_2=0;
		}

		if (FaceID_3==INVILID_VALUE || FaceID_3== ERROR_CODE)
		{
			FaceID_3=0; 
		}
		if (FaceTimes_3==INVILID_VALUE || FaceTimes_3==ERROR_CODE)
		{
			FaceTimes_3=0;
		}

		if (FaceID_4==INVILID_VALUE || FaceID_4== ERROR_CODE)
		{
			FaceID_4=0; 
		}
		if (FaceTimes_4==INVILID_VALUE || FaceTimes_4==ERROR_CODE)
		{
			FaceTimes_4=0;
		}

		if (FaceID_5==INVILID_VALUE || FaceID_5== ERROR_CODE)
		{
			FaceID_5=0; 
		}
		if (FaceTimes_5==INVILID_VALUE || FaceTimes_5==ERROR_CODE)
		{
			FaceTimes_5=0;
		}
	}
	// ���ͶԻ���Ϣ
	CMessage msg(MSG_S2C_OTHER_OPENPIXYTALKBOX);
	msg.Add((LONG)1);
	msg.Add(GetID());
	msg.Add(TextType);
	msg.Add(TextIndex);
	msg.Add(SoundID);	
	msg.Add(HeadID);
	msg.Add(FaceID_1);
	msg.Add(FaceTimes_1);
	msg.Add(FaceID_2);
	msg.Add(FaceTimes_2);
	msg.Add(FaceID_3);
	msg.Add(FaceTimes_3);
	msg.Add(FaceID_4);
	msg.Add(FaceTimes_4);
	msg.Add(FaceID_5);
	msg.Add(FaceTimes_5);

	msg.SendToPlayer(p_SrcShape->GetExID());
	return 1;
}

double CScript::PlayEffectByIndex(const char* cmd, char* retStr)
{

	DWORD dwIndex = GetIntParam( cmd, 0 );
	if (dwIndex==INVILID_VALUE || dwIndex==ERROR_CODE )
	{
		return 0.0;
	}

	CPlayer* player= dynamic_cast<CPlayer*>(p_SrcShape);
	if (player==NULL)
	{
		return 0.0;
	}

	CMessage msg(MSG_S2C_OTHER_EFFECT);
	msg.Add(dwIndex);
	msg.SendToPlayer(player->GetExID());
	return 0;
}
double CScript::PlayerTalk(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;
	CPlayer* pPlayer= dynamic_cast<CPlayer*>(p_SrcShape);
	if (pPlayer== NULL ) return 0;

	char* pTalk = GetStringParam( CmdStr, 0 );
	if( pTalk )
	{
		CMessage msg( MSG_S2C_OTHER_TALK );
		msg.Add((BYTE)0);
		msg.Add( static_cast<long>(CHAT_NORMAL) );
		msg.Add( static_cast<long>(TYPE_PLAYER) );
		msg.Add( pPlayer -> GetExID() );
		msg.Add((long)0);
		msg.Add( const_cast<char*>( pPlayer -> GetName() ) );
		msg.Add( const_cast<char*>(pTalk) );
		msg.SendToAround(pPlayer); 

		M_FREE( pTalk, sizeof(char)*MAX_VAR_LEN );
		return 1;
	}
	return 0;
}

double CScript::PlayerTalkByIndex(const char* cmd, char* retStr)
{
	CPlayer* pPlayer= dynamic_cast<CPlayer*>(p_SrcShape);
	if (pPlayer== NULL ) return 0;

	LONG TextType = GetIntParam(cmd,0);
	LONG TextIndex = GetIntParam( cmd, 1 );
	if( TextIndex!=INVILID_VALUE && TextIndex!=ERROR_CODE )
	{
		CMessage msg( MSG_S2C_OTHER_TALK );
		msg.Add((BYTE)1);
		msg.Add( static_cast<long>(CHAT_NORMAL) );
		msg.Add( static_cast<long>(TYPE_PLAYER) );
		msg.Add( pPlayer -> GetExID() );
		msg.Add((long)0);
		msg.Add( const_cast<char*>( pPlayer -> GetName() ) );
		msg.Add(TextType);
		msg.Add(TextIndex);
		msg.SendToAround( pPlayer); 
		return 1;
	}
	return 0;
}

double CScript::PackGlint(const char* cmd, char* retStr)
{
	CPlayer* pPlayer= dynamic_cast<CPlayer*>(p_SrcShape);
	if (pPlayer== NULL) return 0;

	pPlayer->PackGlint();
	return 1;
}


double CScript::PostPlayerInfo(const char* CmdStr, char* retStr)
{
	if(p_SrcShape==NULL) return 0;

	char* strContend = GetStringParam(CmdStr,0);
	if(strContend == NULL)
	{
		return 0;
	}
	DWORD dwFrColor = static_cast<DWORD>(GetIntParam(CmdStr, 1));
	DWORD dwBkColor = static_cast<DWORD>(GetIntParam(CmdStr, 2));

	if (dwFrColor==ERROR_CODE)
		dwFrColor = 0xffffffff;

	if (dwBkColor==ERROR_CODE)
		dwBkColor = 0xff000000;

	CMessage msg(MSG_S2C_OTHER_ADDPLAYERINFO);
	msg.Add((LONG)0);
	msg.Add(dwFrColor);
	msg.Add(dwBkColor);
	msg.Add(strContend);
	msg.SendToPlayer(p_SrcShape->GetExID());

	M_FREE( strContend, sizeof(char)*MAX_VAR_LEN );
	return 1;
}

double CScript::PostDestPlayerInfo(const char* CmdStr, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_DesShape);
	if(pPlayer==NULL) return 0;

	char* strContend = GetStringParam(CmdStr,0);
	if(strContend == NULL)
	{
		return 0;
	}
	DWORD dwFrColor = static_cast<DWORD>(GetIntParam(CmdStr, 1));
	DWORD dwBkColor = static_cast<DWORD>(GetIntParam(CmdStr, 2));

	if (dwFrColor==ERROR_CODE)
		dwFrColor = 0xffffffff;

	if (dwBkColor==ERROR_CODE)
		dwBkColor = 0xff000000;

	CMessage msg(MSG_S2C_OTHER_ADDPLAYERINFO);
	msg.Add((LONG)0);
	msg.Add(dwFrColor);
	msg.Add(dwBkColor);
	msg.Add(strContend);
	msg.SendToPlayer(pPlayer->GetExID());

	M_FREE( strContend, sizeof(char)*MAX_VAR_LEN );
	return 1;
}

double CScript::PostPlayerInfoByIndex(const char* CmdStr, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer==NULL) return 0;

	LONG TextType = GetIntParam(CmdStr,0);
	if(TextType == INVILID_VALUE || TextType== ERROR_CODE)
	{
		return INVILID_VALUE;
	}

	LONG TextIndex = GetIntParam(CmdStr,1);
	if(TextIndex == INVILID_VALUE || TextIndex== ERROR_CODE)
	{
		return INVILID_VALUE;
	}
	DWORD dwFrColor = static_cast<DWORD>(GetIntParam(CmdStr, 2));
	DWORD dwBkColor = static_cast<DWORD>(GetIntParam(CmdStr, 3));

	if (dwFrColor==ERROR_CODE)
		dwFrColor = 0xffffffff;

	if (dwBkColor==ERROR_CODE)
		dwBkColor = 0xff000000;

	CMessage msg(MSG_S2C_OTHER_ADDPLAYERINFO);
	msg.Add((LONG)1);
	msg.Add(dwFrColor);
	msg.Add(dwBkColor);
	msg.Add(TextType);
	msg.Add(TextIndex);
	msg.SendToPlayer(pPlayer->GetExID());

	return 1;
}

double CScript::PostDestPlayerInfoByIndex(const char* CmdStr, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_DesShape);
	if(pPlayer==NULL) return 0;

	LONG TextType = GetIntParam(CmdStr,0);
	if(TextType == INVILID_VALUE || TextType== ERROR_CODE)
	{
		return INVILID_VALUE;
	}

	LONG TextIndex = GetIntParam(CmdStr,1);
	if(TextIndex == INVILID_VALUE || TextIndex== ERROR_CODE)
	{
		return INVILID_VALUE;
	}
	DWORD dwFrColor = static_cast<DWORD>(GetIntParam(CmdStr, 2));
	DWORD dwBkColor = static_cast<DWORD>(GetIntParam(CmdStr, 3));

	if (dwFrColor==ERROR_CODE)
		dwFrColor = 0xffffffff;

	if (dwBkColor==ERROR_CODE)
		dwBkColor = 0xff000000;

	CMessage msg(MSG_S2C_OTHER_ADDPLAYERINFO);
	msg.Add((LONG)1);
	msg.Add(dwFrColor);
	msg.Add(dwBkColor);
	msg.Add(TextType);
	msg.Add(TextIndex);
	msg.SendToPlayer(pPlayer->GetExID());

	return 1;
}


double CScript::ShowString(const char* cmd, char* retStr)
{
	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(NULL == pPlayer) return -1;

	LONG lScreenPosX = GetIntParam(cmd, 0);
	LONG lScreenPosY = GetIntParam(cmd, 1);

	char* pString = GetStringParam( cmd, 2 );
	if(NULL == pString) return -1;

	LONG lIsShow = GetIntParam(cmd, 3);
	CMessage msg(MSG_S2C_OTHER_SHOW_STRING);
	msg.Add(lIsShow);
	if(0 != lIsShow)
	{
		msg.Add(lScreenPosX);
		msg.Add(lScreenPosY);
		msg.Add(pString);
	}
	msg.SendToPlayer(pPlayer->GetExID(), false);

	M_FREE( pString, sizeof(char)*MAX_VAR_LEN );
	return 1;
}

double CScript::Weather(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;

	double lID = GetIntParam( CmdStr, 0 );
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( p_SrcShape -> GetFather() );
	if( pRegion )
	{
		pRegion -> ChangeWeather( lID );
	}
	return 1;
}
double CScript::PlayEffect(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;

	double lEffectID = GetIntParam(CmdStr, 0);
	//�Ƿ�������λ��
	long lInRegion = static_cast<long>(GetIntParam(CmdStr, 1));
	if(lInRegion != ERROR_CODE && lInRegion != INVILID_VALUE)
	{		
		double lX = GetIntParam(CmdStr, 2);
		double lY = GetIntParam(CmdStr, 3);
		long lBindPos=GetIntParam(CmdStr,4);
		if (lBindPos == ERROR_CODE)
			lBindPos = 2;

		if(lInRegion==0)
		{
			lX = p_SrcShape->GetTileX();
			lY = p_SrcShape->GetTileY();
		}


		CMessage msg(MSG_S2C_RGN_PLAYEFFECT);
		msg.Add((BYTE)lInRegion);
		msg.Add((long)lEffectID);
		if(p_SrcShape)
		{
			msg.Add(p_SrcShape->GetType());
			msg.Add(p_SrcShape->GetExID());
		}
		else
		{
			msg.Add((long)0);
			msg.Add(NULL_GUID);
		}		
		msg.Add((float)lX+.5f);
		msg.Add((float)lY+.5f);		
		msg.Add(lBindPos);
		msg.SendToAround((CServerRegion*)GetRegion(), lX, lY);
		return 1;		

	}
	return 0;
}
double CScript::PlayAction(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;

	WORD wAction = (WORD)GetIntParam(CmdStr, 0);

	CMessage msg(MSG_S2C_RGN_PLAYACTION);
	msg.Add(((CPlayer*)this->GetSrcShape())->GetType());
	msg.Add(((CPlayer*)this->GetSrcShape())->GetExID());
	msg.Add(wAction);
	msg.SendToAround((CServerRegion*)GetRegion(), ((CPlayer*)this->GetSrcShape())->GetTileX(), ((CPlayer*)this->GetSrcShape())->GetTileY());
	return 1;
}

double CScript::PlaySound(const char* CmdStr, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer || !m_pRegion)	return 0;

	char* strFile = GetStringParam(CmdStr, 0);
	int  nToAround = GetIntParam(CmdStr, 1);
	if (nToAround == ERROR_CODE)
	{
		nToAround = 0;
	}

	if (strFile)
	{
		CMessage msg(MSG_S2C_RGN_PLAYSOUND);
		msg.Add(strFile);
		//ֻ�����Լ�
		if(nToAround == 0 && pPlayer)
			msg.SendToPlayer( pPlayer -> GetExID() );
		else
			msg.SendToAround((CServerRegion*)GetRegion(), pPlayer->GetTileX(), pPlayer->GetTileY());

		M_FREE( strFile, sizeof(char)*MAX_VAR_LEN );
	}
	return 1;
}

double CScript::Play3DSound(const char* cmd, char* retStr/* =NULL */)
{
	char* strFile = GetStringParam(cmd,0);
	long lLoop = GetIntParam(cmd,1);
	long lPosX = GetIntParam(cmd,2);
	long lPosY = GetIntParam(cmd,3);
	long lPosZ = GetIntParam(cmd,4);
	if (strFile)
	{
		if (lLoop != ERROR_CODE && 
			lPosX != ERROR_CODE &&
			lPosY != ERROR_CODE &&
			lPosZ != ERROR_CODE)
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(GetSrcShape());	
			if (pPlayer)
			{
				CMessage msg(MSG_S2C_RGN_PLAY3DSOUND);
				msg.Add(strFile);
				msg.Add(lLoop);
				msg.Add(lPosX);
				msg.Add(lPosY);
				msg.Add(lPosZ);
				msg.SendToPlayer(pPlayer->GetExID());
			}
			
		}
		M_FREE( strFile, sizeof(char)*MAX_VAR_LEN );
	}

	return 1;
}

double CScript::PlayBkMusic(const char* cmd, char* retStr/* =NULL */)
{
	char *szPath = GetStringParam(cmd,0);
	long lTimes = GetIntParam(cmd,1);
	if (szPath)
	{
		if (lTimes != ERROR_CODE)
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(GetSrcShape());
			if (pPlayer)
			{
				CMessage msg(MSG_S2C_RGN_PLAYMUSIC);
				msg.Add(szPath);
				msg.Add(lTimes);
				msg.SendToPlayer(pPlayer->GetExID());
			}
		}
		M_FREE( szPath, sizeof(char)*MAX_VAR_LEN );
	}
	return 1;
}

double CScript::StopBkMusic(const char* cmd, char* retStr/* =NULL */)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(GetSrcShape());
	if (pPlayer)
	{
		CMessage msg(MSG_S2C_RGN_STOPMUSIC);
		msg.SendToPlayer(pPlayer->GetExID());
	}
	return 1;
}

double CScript::AddGoodToShortCutBar(const char* cmd, char* retStr)
{
	int nRet = 0;

	char* name=GetStringParam(cmd,0);
	if( name )
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
		if(pPlayer)
		{
			CGoods * pGoods = pPlayer->FindGoodInPack(name);
			//�������и���Ʒ
			if(pGoods)
			{
				long  nBarNo = static_cast<long>(GetIntParam(cmd, 1));
				if( nBarNo != ERROR_CODE && nBarNo != INVILID_VALUE)
				{
					pPlayer->SetHotKey(nBarNo, pGoods->GetBasePropertiesIndex());

					CMessage msg(MSG_S2C_SETHOTKEY);
					msg.Add((char)HOTKEY_SET_SUCCEED);
					msg.Add((BYTE)nBarNo);
					msg.Add(pGoods->GetBasePropertiesIndex());
					msg.SendToPlayer(pPlayer->GetExID());	
					nRet = 1;
				}
			}
		}
		M_FREE( name, sizeof(char)*MAX_VAR_LEN );
	}
	return nRet;
}

double CScript::AddSkillToShortCutBar(const char* cmd, char* retStr)
{
	long nRet = 0;
	DWORD dwSkillID = static_cast<DWORD>(GetIntParam(cmd, 0));
	if( dwSkillID != ERROR_CODE && dwSkillID != INVILID_VALUE )
	{
		long  nBarNo = static_cast<long>(GetIntParam(cmd, 1));
		if( nBarNo != ERROR_CODE && nBarNo != INVILID_VALUE)
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
			if( pPlayer && pPlayer->CheckSkill(dwSkillID) )
			{
				dwSkillID = dwSkillID|0x80000000;
				pPlayer->SetHotKey(nBarNo,dwSkillID);

				CMessage msg(MSG_S2C_SETHOTKEY);
				msg.Add((char)HOTKEY_SET_SUCCEED);
				msg.Add((BYTE)nBarNo);
				msg.Add(dwSkillID);
				msg.SendToPlayer(pPlayer->GetExID());

				nRet = 1;
			}
		}
	}
	return nRet;
}

double CScript::TransforCamera(const char* cmd, char* retStr)
{
	int nRet = 0;
	//�仯��ʽ
	long lTransforMode = static_cast<long>(GetIntParam(cmd, 0));
	if( lTransforMode != ERROR_CODE && lTransforMode != INVILID_VALUE )
	{
		long lParam1 = static_cast<long>(GetIntParam(cmd, 1));
		long lParam2 = static_cast<long>(GetIntParam(cmd, 2));
		long lParam3 = static_cast<long>(GetIntParam(cmd, 3));
		if( lParam1 != ERROR_CODE && lParam1 != INVILID_VALUE && 
			lParam2 != ERROR_CODE && lParam2 != INVILID_VALUE &&
			lParam3 != ERROR_CODE && lParam3 != INVILID_VALUE)
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
			if( pPlayer )
			{
				CMessage msg(MSG_S2C_OTHER_TRANSFORCAMERA);
				msg.Add(lTransforMode);
				msg.Add(lParam1);
				msg.Add(lParam2);
				msg.Add(lParam3);
				msg.SendToPlayer(pPlayer->GetExID());
				nRet = 1;
			}
		}
	}
	return nRet;
}

double CScript::OpenReliveWindow(const char* cmd, char* retStr)
{
    double dRet = 0.0;

	BYTE bType = GetIntParam(cmd,0);
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	
	if (pPlayer && bType !=ERROR_CODE && bType!=INVILID_VALUE)
	{	
		DelPlayerTalkBoxScript(m_guSrcID, SR_FUNC_RELIVEWINDOW);
		//�ȴ�����
		SetIsHang(true);
		SetHangFunc(SR_FUNC_RELIVEWINDOW);

		pPlayer->SetReliveScript(GetID());

		CMessage msg(MSG_S2C_OTHER_OPEN_RELIVEWINDOW);
		msg.Add(GetID());
		msg.Add(bType);										//0��ֻ�лسǸ��1Ϊ����ԭ�ظ���
		msg.SendToPlayer(pPlayer->GetExID());

		dRet = 1.0;
	}
	return dRet;
}

double CScript::OpenReLiveByGoodsWindow(const char* cmd, char* retStr)
{

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);

	if( pPlayer )
	{
		long lRet    = GetIntParam(cmd, 0);
		long lIndex = static_cast<long>(GetIntParam(cmd, 1));	
		long lCostNum = static_cast<long>(GetIntParam(cmd,2));

		if ( lRet!=ERROR_CODE && lRet!=INVILID_VALUE 
			&&   lIndex!=ERROR_CODE && lIndex!=INVILID_VALUE 		
			&&   lCostNum!=ERROR_CODE && lCostNum!=INVILID_VALUE )
		{
			DelPlayerTalkBoxScript(m_guSrcID, SR_FUNC_RELIVEGOODWINDOW);
			//�ȴ�����
			SetIsHang(true);
			SetHangFunc(SR_FUNC_RELIVEGOODWINDOW);

			CMessage msg(MSG_S2C_PLAYER_BUYRELIVEITEM_RESULT);
			msg.Add(GetID());								//�ű�guid
			msg.Add(lRet);									//�򿪴������ͣ�1�����������Ĵ��ڣ�0�ǲ���������
			msg.Add(lIndex);								//��Ʒ����ֵ			
			msg.Add(lCostNum);								//������Ʒ����
			msg.SendToPlayer(pPlayer->GetExID());
			return 1.0;
		}
	}
	
	return 0.0;
}
double CScript::OperaInterface(const char* cmd, char* retStr)
{
	int nRet = 0;
	//�仯��ʽ
	long lOperaMode = static_cast<long>(GetIntParam(cmd, 0));
	if( lOperaMode != ERROR_CODE && lOperaMode != INVILID_VALUE )
	{
		char* InterfaceName=GetStringParam(cmd,1);
		if( InterfaceName )
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
			if( pPlayer )
			{
#ifdef _DEBUG
				OutputDebugString("\n================���͹رս�����Ϣ=========================\n");
#endif
				CMessage msg(MSG_S2C_OTHER_OPERAINTERFACE);
				msg.Add(lOperaMode);
				msg.Add(InterfaceName);
				msg.SendToPlayer(pPlayer->GetExID());
				nRet = 1;
			}
			M_FREE( InterfaceName, sizeof(char)*MAX_VAR_LEN );
		}
	}
	return nRet;
}

//������ť�İ�����ʾ
double CScript::HelpHint(const char* cmd, char* retStr)
{
	int nRet = 0;
	char* pszScriptName=GetStringParam(cmd,0);
	if( pszScriptName )
	{
		long lFutureDelTime = static_cast<long>(GetIntParam(cmd, 1));
		if( lFutureDelTime != ERROR_CODE && lFutureDelTime != INVILID_VALUE )
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
			if( pPlayer )
			{
				//�����ʾ
				nRet = pPlayer->AddHelpHint(pszScriptName,lFutureDelTime);
			}
		}
		M_FREE( pszScriptName, sizeof(char)*MAX_VAR_LEN );
	}
	return nRet;
}

//���������ʾ
double CScript::InterfaceHint(const char* cmd, char* retStr)
{
	int nRet = 0;
	char* pszScriptName=GetStringParam(cmd,0);
	if( pszScriptName )
	{
		long lFutureDelTime = static_cast<long>(GetIntParam(cmd, 1));
		if( lFutureDelTime != ERROR_CODE && lFutureDelTime != INVILID_VALUE )
		{
			char* pszInterfaceName=GetStringParam(cmd,2);
			if( pszInterfaceName )
			{
				long lX = static_cast<long>(GetIntParam(cmd, 3));
				long lY = static_cast<long>(GetIntParam(cmd, 4));
				long lWidth = static_cast<long>(GetIntParam(cmd, 5));
				long lHeight = static_cast<long>(GetIntParam(cmd, 6));
				if(lX != ERROR_CODE && lX != INVILID_VALUE &&
					lY != ERROR_CODE && lY != INVILID_VALUE &&
					lWidth != ERROR_CODE && lWidth != INVILID_VALUE &&
					lHeight != ERROR_CODE && lHeight != INVILID_VALUE)
				{
					CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
					if( pPlayer )
					{
						//�����ʾ
						nRet = pPlayer->AddInterfaceHint(pszScriptName,lFutureDelTime,pszInterfaceName,lX,lY,lWidth,lHeight);
					}
				}
				M_FREE( pszInterfaceName, sizeof(char)*MAX_VAR_LEN );
			}
		}
		M_FREE( pszScriptName, sizeof(char)*MAX_VAR_LEN );
	}
	return nRet;
}


double CScript::PlayVideo(const char* CmdStr, char* retStr)
{
	double ret = 0;	
	char* strFileName = GetStringParam(CmdStr,0);
	long  lLoop = GetIntParam(CmdStr,1);
	long  lX	= GetIntParam(CmdStr,2);
	long  lY	= GetIntParam(CmdStr,3);
	long  lWidth = GetIntParam(CmdStr,4);
	long  lHeight = GetIntParam(CmdStr,5);
	if (p_SrcShape!=NULL && strFileName!=NULL   && lLoop!=ERROR_CODE && lLoop!=INVILID_VALUE 
		&& lX!=ERROR_CODE && lX!=INVILID_VALUE 
		&& lY!=ERROR_CODE && lY!=INVILID_VALUE
		&& lWidth!=ERROR_CODE && lWidth!=INVILID_VALUE
		&& lHeight!=ERROR_CODE && lHeight!=INVILID_VALUE)
	{
		CMessage msg(MSG_S2C_OTHER_PLAY_VIDEO);
		msg.Add(strFileName);
		msg.Add(lLoop);
		msg.Add(lX);
		msg.Add(lY);
		msg.Add(lWidth);
		msg.Add(lHeight);
		msg.SendToPlayer(p_SrcShape->GetExID());
		ret = 1;
	}

	M_FREE( strFileName, sizeof(char)*MAX_VAR_LEN );
	return ret;
}
double CScript::StopVideo(const char* CmdStr, char* retStr)
{
	double ret = 0;
	long lIndex = GetIntParam(CmdStr,0);
	if (p_SrcShape!=NULL && lIndex!=ERROR_CODE && lIndex!=INVILID_VALUE)
	{
		CMessage msg(MSG_S2C_OTHER_STOP_VIDEO);
		msg.Add(lIndex);
		msg.SendToPlayer(p_SrcShape->GetExID());
		ret=1;
	}

	return ret;
}

// �޸Ľű�����
double CScript::UpdateScriptVarValue(const char* CmdStr, char* retStr)
{
	//��ʼΪ����
	CScript::SCRIPTRETURN retFlag  = CScript::SR_ERROR;
	char *str=GetStringParam(CmdStr, 0);
	if (str)
	{
		char *szValue = GetStringParam(CmdStr, 1);
		if(szValue && p_SrcShape)
		{
			DelPlayerTalkBoxScript(m_guSrcID, SR_FUNC_UPDATEVARVALUE);

			//�ȴ�����, �յ���Ӧ��Ϣ�����нű�
			SetIsHang(true);
			SetHangFunc(SR_FUNC_UPDATEVARVALUE);

			// ��¼Ҫ�޸ĵı�����
			SetClientUpdateVarName(szValue);

			CMessage msg(MSG_S2C_OTHER_UPDATEVARVALUEOPEN);
			msg.Add(GetID());
			msg.Add(str);
			msg.Add(szValue);
			msg.SendToPlayer(p_SrcShape->GetExID());			
		}
		M_FREE( szValue, sizeof(char)*MAX_VAR_LEN );	
		M_FREE( str, sizeof(char)*MAX_VAR_LEN );
	}
	return 1.0f;
}



double CScript::SendExpAction(const char* CmdStr, char* retStr/* =NULL */)
{
	double dRet = 0;
	DWORD dwActionID = GetIntParam(CmdStr,0);
	DWORD dwSrcType = GetIntParam(CmdStr,1);
	char*  strSrcGuid = GetStringParam(CmdStr,2); 
	CGUID  guidDest;
	CGUID  guidSrc;
	if (strSrcGuid!=NULL)
	{
		guidSrc = GetScriptGuidValue((CMoveShape*)p_SrcShape, strSrcGuid);
	}
	else
	{
		guidSrc = p_SrcShape->GetExID();
	}
	DWORD dwDestType = GetIntParam(CmdStr,3);
	char*  strDestGuid = GetStringParam(CmdStr,4); 
	if (strDestGuid!=NULL)
	{
		guidDest = GetScriptGuidValue((CMoveShape*)p_SrcShape, strDestGuid);
	}

	if (dwDestType==ERROR_CODE || dwDestType==INVILID_VALUE)
	{
		dwDestType = 0;
	}
	if (guidDest== CGUID::GUID_INVALID)
	{
		guidDest = NULL_GUID;
	}

	if(dwActionID!=ERROR_CODE && dwSrcType!=ERROR_CODE && guidSrc!= CGUID::GUID_INVALID && this->m_pRegion != NULL)
	{
		CMoveShape *pMoveShape = dynamic_cast<CMoveShape*>( m_pRegion->FindChildObject(OBJECT_TYPE(dwSrcType), guidSrc ) );
		CMessage msg(MSG_S2C_OTHER_EXPATC);
		msg.Add(dwActionID);
		msg.Add(dwSrcType);
		msg.Add(guidSrc);
		msg.Add(dwDestType);
		msg.Add(guidDest);
		msg.SendToAround(pMoveShape);
		dRet = 1.0;
	}
	M_FREE( strSrcGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( strDestGuid, sizeof(char)*MAX_VAR_LEN );
	return dRet;
}

	double CScript::GetPlayerAllProperties(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;

	CHAR* szName = GetStringParam( CmdStr, 0 );
	if( szName )
	{
		CPlayer* pPlayer = GetGame() -> FindPlayer( szName );
		if( pPlayer )
		{
			//##�ҵ������
			CHAR szText[1024 * 4] = {0};

			//##�����ɫ����״̬
			_snprintf( szText, 1024 * 4, "����:%s [ְҵ:%d], �ȼ� %d, ���� %d, HP %u/%u, MP %u/%u, RP %u/%u, ����HP %u, ����MP %u, ����RP %u, PK�� %u, ɱ���� %u, ��Ǯ:%d", 
				pPlayer -> GetName(),
				pPlayer->GetOccupation(),
				pPlayer->GetLevel(),
				pPlayer->GetExp(),
				pPlayer->GetHP(),
				pPlayer->GetMaxHP(),
				pPlayer->GetMP(),
				pPlayer->GetMaxMP(),
				pPlayer->GetEnergy(),
				pPlayer->GetMaxEnergy(),
				pPlayer->GetBaseMaxHP(),
				pPlayer->GetBaseMaxMP(),
				pPlayer->GetBaseMaxEnergy(),
				pPlayer->GetPkValue(),
				pPlayer->GetPkCount(),
				pPlayer->GetMoney()),

				//##���͸��ͻ���
				((CPlayer*)p_SrcShape) -> SendNotifyMessage( szText, 0xffffff00 , 0, eNOTIFYPOS_LEFT_BOTTOM);


			//##�����ɫ�Ļ�������
			_snprintf( szText, 1024 * 4, "����������%u���������ݣ�%u���������ʣ�%u�������ǻۣ�%u��������������%u - %u����������������%u�������������ޣ�%u����������һ�����ʣ�%u��������������%u����������������%u�����������ٶȣ�%u������Ԫ�صֿ�����%u�����������ָ��ٶȣ�%u�����������ָ��ٶȣ�%u", 
				pPlayer->GetStrenth(),
				pPlayer->GetAgility(),
				pPlayer->GetConsti(),
				pPlayer->GetWisdom(),
				pPlayer->GetMinAtk(),
				pPlayer->GetMaxAtk(),
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0);

			//##���͸��ͻ���
			((CPlayer*)p_SrcShape) -> SendNotifyMessage( szText, 0xffffffff , 0, eNOTIFYPOS_LEFT_BOTTOM);



			//##�����ɫ�ĵ�ǰ����
			_snprintf( szText, 1024 * 4, "������%u�����ݣ�%u�����ʣ�%u��������%u����������%u - %u�����У�%u���������ޣ�%u������һ����%u����������%u������������%u�������ٶȣ�%u��Ԫ�صֿ�����%u�������ָ��ٶȣ�%u�������ָ��ٶȣ�%u�����ӷ�����������%u������������%d��Ӳֱ����������%u������������������%u�������˺����ԣ�%u", 
				pPlayer->GetStrenth(),
				pPlayer->GetAgility(),
				pPlayer->GetConsti(),
				pPlayer->GetAgility(),
				pPlayer->GetMinAtk(),
				pPlayer->GetMaxAtk(),
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0);

			//##���͸��ͻ���
			((CPlayer*)p_SrcShape) -> SendNotifyMessage( szText, 0xffff00ff , 0, eNOTIFYPOS_LEFT_BOTTOM);
		}

		M_FREE( szName, sizeof(char)*MAX_VAR_LEN );
	}

	return 1;
}
double CScript::GetPlayerAllVariables(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;

	CHAR* szName = GetStringParam( CmdStr, 0 );
	if( szName )
	{
		CPlayer* pPlayer = GetGame() -> FindPlayer( szName );
		if( pPlayer )
		{
			//##�ҵ������
			CHAR szText[10240] = {0};
			ZeroMemory( szText, sizeof(szText) );

			CVariableList* pVarList = pPlayer->GetVariableList();

			if (pVarList)
			{
				CVariableList::varlistitr itr = pVarList->VarListBegin();

				for(; itr != pVarList->VarListEnd(); itr++)
				{
					// �򵥱���
					if( itr->second->Array == 0 )
					{
						_snprintf(szText, 10240, "%s = %d", itr->first.c_str(), (int)(itr->second->Value));
						((CPlayer*)p_SrcShape) -> SendNotifyMessage( szText, 0xff00ff00, 0, eNOTIFYPOS_LEFT_BOTTOM );
					}
					else if( itr->second->Array > 0)	// �������
					{
						for(int j = 0; j < itr->second->Array; j++)
						{
							_snprintf(szText, 10240, "%s[%d] = %d", itr->first.c_str(), j, itr->second->Value[j]);
							((CPlayer*)p_SrcShape) -> SendNotifyMessage( szText, 0xff00ff00, 0, eNOTIFYPOS_LEFT_BOTTOM );
						}
					}
					else
					{	//�ַ�������
						_snprintf(szText, 10240, "%s = \"%s\"", itr->first.c_str(), itr->second->strValue);
						((CPlayer*)p_SrcShape) -> SendNotifyMessage( szText, 0xff00ff00, 0, eNOTIFYPOS_LEFT_BOTTOM );
					}
				}
			}
		}

		M_FREE( szName, sizeof(char)*MAX_VAR_LEN );
	}

	return 1;
}

double CScript::ViewPlayerAllVariableList(const char* cmd, char* retStr)
{
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer)
	{
		char szVarInfo[5120];
		_snprintf(szVarInfo, 5120, "All Normal Var Num: %d", pPlayer->GetVariableList()->GetVarNum());
		pPlayer->SendNotifyMessage(szVarInfo, 0xFFFFFFFF, 0x0, eNOTIFYPOS_LEFT_BOTTOM);

		CVariableList::VarList* pVarList = pPlayer->GetVariableList()->GetVarList();
		if(pVarList)
		{
			CVariableList::varlistitr varItr = pVarList->begin();
			for(; varItr != pVarList->end(); varItr++)
			{
				if(varItr->second->Array < 0)
					_snprintf(szVarInfo, 5120, "%s : %s", varItr->first.c_str(), varItr->second->strValue);
				else if(varItr->second->Array == 0)
				{
					_snprintf(szVarInfo, 5120, "%s : %f", varItr->first.c_str(), *((double*)varItr->second->strValue));
				}
				else if(varItr->second->Array > 0)
				{
					_snprintf(szVarInfo, 5120, "%s : ", varItr->first.c_str());

					char szArray[1024];
					for(int i=0; i<varItr->second->Array; i++)
					{
						gcvt(varItr->second->Value[i], 10, szArray);
						strcat(szVarInfo, szArray);
						strcat(szVarInfo, ";");
					}
				}
				pPlayer->SendNotifyMessage(szVarInfo, 0xFFFFFFFF, 0x0, eNOTIFYPOS_LEFT_BOTTOM);
			}
		}

		_snprintf(szVarInfo, 5120, "All GUID Var Num: %d", pPlayer->GetVariableList()->GetGuidNum());
		pPlayer->SendNotifyMessage(szVarInfo, 0xFFFFFFFF, 0x0, eNOTIFYPOS_LEFT_BOTTOM);

		CVariableList::GuidList* pGuidList = pPlayer->GetVariableList()->GetGuidList();
		if(pGuidList)
		{
			CVariableList::GuidList::iterator guidItr = pGuidList->begin();
			for(; guidItr != pGuidList->end(); guidItr++)
			{
				char szGuid[128];
				guidItr->second->tostring(szGuid);
				_snprintf(szVarInfo, 128, "%s : %s", guidItr->first.c_str(), szGuid);
				pPlayer->SendNotifyMessage(szVarInfo, 0xFFFFFFFF, 0x0, eNOTIFYPOS_LEFT_BOTTOM);
			}
		}
	}

	return 0.0f;
}

