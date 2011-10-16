//=============================================================================
/**
 *  file: BaseFun.cpp
 *
 *  Brief:�����˽ű��л����ĺ���
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
#include "GlobalScriptTimer.h"
#include "variablelist.h"
#include "../Player.h"
#include "../Monster.h"
#include "../../../setup/LogSystem.h"
#include "../public/StrType.h"
#include "../public/StrUnifyInput.h"


enum CScript::SCRIPTRETURN  CScript::call(const char* CmdStr, char* retStr)
{
    //��ʼΪ����
    CScript::SCRIPTRETURN retFlag  = CScript::SR_ERROR;
    char *str=GetStringParam(CmdStr, 0);
    if (str)
    {
        double paramA = GetIntParam(CmdStr, 1);
        if( ERROR_CODE == paramA ) 
            paramA = 0;
        double paramB = GetIntParam(CmdStr, 2);
		if( ERROR_CODE == paramB )
			paramB = 0;

#ifdef _RUNSTACKINFO_
		char pszStatckInfo[10240]="";
		_snprintf(pszStatckInfo,10240,"call(FileName:%s) Start",str);
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif

#ifdef _SCRIPT_INFO_
		DWORD BeginTime = timeGetTime();
		m_RunTime = 0;
#endif

		CScript* pTempScript = PopFreeScript(str);
		InitChildScriptData(pTempScript);
		// ���ؽű�����
		if( pTempScript->LoadScript(str, (char*)GetGame()->GetScriptFileData(str)) )
        {
            pTempScript->GetVariableList()->AddVar("$ParamA", paramA);
            pTempScript->GetVariableList()->AddVar("$ParamB", paramB);
            pTempScript->GetVariableList()->AddVar("$Ret", 0.0f);
            //��¼�����ӽű�
            EnterChildScript(CST_CALL,pTempScript,"","");
            retFlag = pTempScript->RunStep();
			if(retFlag == CScript::SR_HANG)
			{
#ifdef _RUNSTACKINFO_
				_snprintf(pszStatckInfo,10240,"call(FileName:%s) Hang",str);
				CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
#ifdef _SCRIPT_INFO_
				m_RunTime += timeGetTime() - BeginTime;
#endif
				M_FREE( str, sizeof(char)*MAX_VAR_LEN );
                SetIsHang(true);
                SetHangFunc(pTempScript->GetHangFunc());
				return retFlag;
			}
			//ֻҪ�ýű������𣬾ͼ�¼�뿪�ӽű�
			LeaveChildScript(pTempScript);
			double dbRet = pTempScript->GetVariableList()->GetVarValue("$Ret", 0);
			GetVariableList()->SetVarValue("$Ret",0,dbRet);
		}
#ifdef _RUNSTACKINFO_
		_snprintf(pszStatckInfo,10240,"call(FileName:%s) End",str);
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
#ifdef _SCRIPT_INFO_
		GetGame()->AddScriptInfo(str, timeGetTime() - BeginTime);
		m_RunTime = 0;
#endif
		PushFreeScript(pTempScript);
		M_FREE( str, sizeof(char)*MAX_VAR_LEN );
	}
    return retFlag;
}

//call����չ�汾,���ӽű����ýű������б�
enum CScript::SCRIPTRETURN CScript::callEx(const char* CmdStr, char* retStr)
{
	 //��ʼΪ����
    CScript::SCRIPTRETURN retFlag  = CScript::SR_ERROR;
    char *str=GetStringParam(CmdStr, 0);
    if (str)
    {
#ifdef _RUNSTACKINFO_
		char pszStatckInfo[10240]="";
		_snprintf(pszStatckInfo,10240,"callEx(FileName:%s) Start",str);
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
#ifdef _SCRIPT_INFO_
		DWORD BeginTime = timeGetTime();
		m_RunTime = 0;
#endif
		CScript* pTempScript = PopFreeScript(str);
		InitChildScriptData(pTempScript);
		pTempScript->SetVariableList(GetVariableList());
		// ���ؽű�����
		if( pTempScript->LoadScript(str, (char*)GetGame()->GetScriptFileData(str)) )
        {
            //��¼�����ӽű�
            EnterChildScript(CST_CALLEX,pTempScript,"","");
            retFlag = pTempScript->RunStep();
			if(retFlag == CScript::SR_HANG)
			{
#ifdef _RUNSTACKINFO_
				_snprintf(pszStatckInfo,10240,"callEx(FileName:%s) Hang",str);
				CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
#ifdef _SCRIPT_INFO_
				m_RunTime += timeGetTime() - BeginTime;
#endif
				M_FREE( str, sizeof(char)*MAX_VAR_LEN );
                SetIsHang(true);
                SetHangFunc(pTempScript->GetHangFunc());
				return retFlag;
			}
			//ֻҪ�ýű������𣬾ͼ�¼�뿪�ӽű�
			LeaveChildScript(pTempScript);
		}
#ifdef _RUNSTACKINFO_
		_snprintf(pszStatckInfo,10240,"callEx(FileName:%s) End",str);
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
#ifdef _SCRIPT_INFO_
		GetGame()->AddScriptInfo(str, timeGetTime() - BeginTime);
		m_RunTime = 0;
#endif
		pTempScript->ResetVariableList();
		PushFreeScript(pTempScript);
		M_FREE( str, sizeof(char)*MAX_VAR_LEN );
	}
    return retFlag;
}

enum CScript::SCRIPTRETURN  CScript::For(const char* CmdStr, char* retStr)
{
    // ��ȡ����
    int cmdLen = strlen(CmdStr);

    char* szBeginCmd = GetStringParam(CmdStr, 0);
    if(!szBeginCmd) return CScript::SR_ERROR;
    int szCmdLen = strlen(szBeginCmd);
    szBeginCmd[szCmdLen] = ';';
    szBeginCmd[szCmdLen+1] = '\0';

    char* szMaxCmd = GetStringParam(CmdStr, 1);
    if(!szMaxCmd) 
    {
        M_FREE( szBeginCmd, sizeof(char)*MAX_VAR_LEN );
        return CScript::SR_ERROR;
    }
    szCmdLen = strlen(szMaxCmd);
    szMaxCmd[szCmdLen] = ';';
    szMaxCmd[szCmdLen+1] = '\0';

    char* szCountCmd = GetStringParam(CmdStr, 2);
    if(!szCountCmd) 
    {
        M_FREE( szBeginCmd, sizeof(char)*MAX_VAR_LEN );
        M_FREE( szMaxCmd, sizeof(char)*MAX_VAR_LEN );
        return CScript::SR_ERROR;
    }
    szCmdLen = strlen(szCountCmd);
    szCountCmd[szCmdLen] = ';';
    szCountCmd[szCmdLen+1] = '\0';


    //��ȡFor�ű�����
    char* chr = NULL;
    chr = m_Data;
    long beginPos = 0; // �ӽű���ʼλ��
    long endPos   = 0; // �ӽű�����λ��
    beginPos = m_Point;
    int forBlockNum = 0; // For�������ڲ����ż���

    ReadForCmd(beginPos, endPos);

	// �ӵ�ǰ�ű���������������ѭ���ӽű�����
	char* scriptBuf = (char*)M_ALLOC(sizeof(char)*m_DataLen);
	//memset(scriptBuf, 0, m_DataLen);
    memcpy(scriptBuf, &m_Data[beginPos], (endPos-beginPos+1));
	scriptBuf[(endPos-beginPos+1)+1] = '\0';

    CScript* forScript = BeginForScript(scriptBuf);
    if(NULL == forScript)
	{
		M_FREE( szBeginCmd, sizeof(char)*MAX_VAR_LEN );
		M_FREE( szMaxCmd, sizeof(char)*MAX_VAR_LEN );
		M_FREE( szCountCmd, sizeof(char)*MAX_VAR_LEN );
		M_FREE( scriptBuf, sizeof(char)*m_DataLen );
		return CScript::SR_ERROR;
	}
    CScript::SCRIPTRETURN retFlag = CScript::SR_END;
    // ��Count����ֵ�ı�
	RunLine(szBeginCmd);
	RunLine(szMaxCmd);

#ifdef _RUNSTACKINFO_
	char pszStatckInfo[10240]="";
	_snprintf(pszStatckInfo,10240,"For(beginStr:%s,endStr:%s) Start",szBeginCmd,szMaxCmd);
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
	while(1 == m_Value[0])
	{
		retFlag = CScript::SR_END;
		//���õ�ǰָ��
		forScript->ResetDataPos();
        //��¼�����ӽű�
        EnterChildScript(CST_FOR,forScript,szCountCmd,szMaxCmd);
        retFlag =  forScript->RunStep();
        //ֻҪ�ýű������𣬾ͼ�¼�뿪�ӽű�
        if(retFlag != CScript::SR_HANG)
            LeaveChildScript(forScript);
		//�ű�����
		if(retFlag == CScript::SR_HANG)
		{
#ifdef _RUNSTACKINFO_
			_snprintf(pszStatckInfo,10240,"For(beginStr:%s,endStr:%s) Hang",szBeginCmd,szMaxCmd);
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
			SetIsHang(true);
			SetHangFunc(forScript->GetHangFunc());

			M_FREE( scriptBuf, sizeof(char)*m_DataLen );
			M_FREE( szBeginCmd, sizeof(char)*MAX_VAR_LEN );
			M_FREE( szMaxCmd, sizeof(char)*MAX_VAR_LEN );
			M_FREE( szCountCmd, sizeof(char)*MAX_VAR_LEN );
			return retFlag;
		}
		else if(retFlag == CScript::SR_FORBREAK ||
            retFlag == CScript::SR_RETURN ||
            retFlag == CScript::SR_ERROR)
        {
            if(retFlag == CScript::SR_FORBREAK)
                retFlag = CScript::SR_END;
            break;
        }

        //��retFlag ==  CScript::SR_FORCONTINUE�����������£�����ִ��
        RunLine(szCountCmd);
        RunLine(szMaxCmd);
    }
	EndForScript(forScript);

#ifdef _RUNSTACKINFO_
	_snprintf(pszStatckInfo,10240,"For(beginStr:%s,endStr:%s) End",szBeginCmd,szMaxCmd);
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
	M_FREE( scriptBuf, sizeof(char)*m_DataLen );
	M_FREE( szBeginCmd, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szMaxCmd, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szCountCmd, sizeof(char)*MAX_VAR_LEN );
	return retFlag;
}

double CScript::Goto(const char* CmdStr, char* retStr)
{
	char *str=GetStringParam(CmdStr, 0);
	if (str)
	{
		JumpTo( str );
		M_FREE( str, sizeof(char)*MAX_VAR_LEN );
	}
	return 1;
}
double CScript::If(const char* CmdStr, char* retStr)
{
	CScript *pTemptScript = PopFreeScript(CmdStr);
	InitChildScriptData(pTemptScript);

	long len = lstrlen(CmdStr);
	char* str = (char*)M_ALLOC(sizeof(char)*(len+1));
	memcpy(str, &CmdStr[3], len-4);
	str[len-4] = ';';
	str[len-3] = 0;
	//���ñ���
	pTemptScript->SetVariableList(m_pVariableList);
	pTemptScript->RunLine(str);	
	M_FREE(str, len+1);
	double dbRet = pTemptScript->m_Value[0];
	PushFreeScript(pTemptScript);
	return dbRet;
}

double CScript::wait(const char* CmdStr, char* retStr)
{
	m_nWaitTime=GetIntParam(CmdStr, 0);
	if (m_nWaitTime==INVILID_VALUE || m_nWaitTime==ERROR_CODE)
		return 0;
	//�ű���ִͣ��,�ȴ����ؽ��
	SetIsHang(true);
	SetHangFunc(SR_FUNC_WAIT);

	CGlobalScriptTimer::GetInstance()->RegisterWaitScript(GetID(),m_nWaitTime);
	return 1;
}

double CScript::random(const char* CmdStr, char* retStr)
{
	LONG lrandom = GetIntParam(CmdStr, 0);
	long ret = ::random(lrandom);
	return ret;
}
double CScript::Rgb(const char* CmdStr, char* retStr)
{
	LONG	nRed	= GetIntParam( CmdStr, 0 );
	LONG	nGreen	= GetIntParam( CmdStr, 1 );
	LONG	nBlue	= GetIntParam( CmdStr, 2 );
	DWORD	dwColor	= 0xff000000 | ( (nRed & 0xff) << 16 ) | ( (nGreen & 0xff) << 8 ) | ( nBlue & 0xff ); 
	return	dwColor;
}

double CScript::LocalVar(const char* CmdStr, char* retStr)
{
	// ȡ��һ�������� �������ַ���, �ýű�ϵͳ��֧�������±�Ϊ$var����,��֧��[]Ƕ��
	// �� $Var[$Var1],#strVar[$Var1];
	char* firstVarName = (char*)CmdStr;

	// ��ѭ������һ��������
	long cmdStrLen = strlen(CmdStr);
	long tCurPos = 0;
	while(*firstVarName != '(' && tCurPos < cmdStrLen && *firstVarName != '\0')
	{
		tCurPos++;
		firstVarName++;
	}

	if(*firstVarName == '(') // ����'('
		firstVarName++;

	if(firstVarName)
	{
		char MainVarName[1024];  // ����
		char InnVarName[1024];   // �±����

		// �ֽ��ַ�����һ���������±�
		long tempPos = 0;
		char* tMainPtr = MainVarName;
		char* tInnPtr = InnVarName;
		CheckParamStringForArray(firstVarName, cmdStrLen-tCurPos, &tMainPtr, 1024, &tInnPtr, 1024, tempPos);

		// ��ȡ�ַ���ֵ
		if(MainVarName[0] == '#') // �ַ���
		{
			char* strValue = NULL;
			strValue = GetStringParam(CmdStr, 1);

			if(m_pVariableList && strValue)
			{
				if(InnVarName[0] == '\0') // �����������
				{
					if(strValue[0] != '#') // �ڶ����������Ǳ���
						m_pVariableList->AddVar(MainVarName, strValue);
					else
					{
						char* str2ndValue = (char*)m_pVariableList->GetVarValue(strValue);
						if(str2ndValue)
							m_pVariableList->AddVar(MainVarName, str2ndValue);
					}
				}
				else// ���������
				{
					int lArray = 0;
					if(InnVarName[0] == '$') // �±�����ͨ���������������
					{
						lArray = GetScriptVarValue((CMoveShape*)p_SrcShape, InnVarName, 0);
					}
					else if(InnVarName[0] != '$' && InnVarName[0] != '#') // �±��ǳ���
					{
						lArray = atoi(InnVarName);
					}

					// ȥ��'[' ']'
					for(int i=0; i < lArray; i++)
					{
						char tNum[32];
						char name[1024];
						strcpy_s(name, 1024, MainVarName);
						itoa(i, tNum, 10);
						strcat(name, tNum);
						m_pVariableList->AddVar(name, strValue);
					}
				}
			}

			M_FREE( strValue, sizeof(char)*MAX_VAR_LEN );
		}
		else if(MainVarName[0] == '$') // ��һ�����������ֱ���
		{
			double value = GetIntParam(CmdStr, 1);

			if(m_pVariableList && (value != ERROR_CODE) )
			{
				if(InnVarName[0] == '\0') // �����������
				{
					m_pVariableList->AddVar(MainVarName, value);
				}
				else // ���������
				{
					int lArray = 0;
					if(InnVarName[0] == '$') // �±�����ͨ����
					{
						lArray = GetScriptVarValue((CMoveShape*)p_SrcShape, InnVarName, 0);

					}
					else if(InnVarName[0] != '$' && InnVarName[0] != '#') // �±��ǳ���
					{
						lArray = atoi(InnVarName);
					}

					// ȥ��'[' ']'
					m_pVariableList->AddVar(MainVarName, lArray, value);
				}
			}
		}

		else if(MainVarName[0] == '@') // ����GUID
		{
			double value = GetIntParam(CmdStr, 1);

			if(m_pVariableList && (value != ERROR_CODE) )
			{
				if(InnVarName[0] == '\0') // �����������
				{
					m_pVariableList->AddGuid(MainVarName, NULL_GUID);
				}
				else // ���������
				{
					int lArray = 0;
					if(InnVarName[0] == '$') // �±�����ͨ����
					{
						lArray = GetScriptVarValue((CMoveShape*)p_SrcShape, InnVarName, 0);
					}
					else if(InnVarName[0] != '$' && InnVarName[0] != '#') // �±��ǳ���
					{
						lArray = atoi(InnVarName);
					}

					// ȥ��'[' ']'
					for(int i=0; i < lArray; i++)
					{
						char tNum[32];
						char name[1024];
						strcpy_s(name, 1024, MainVarName);
						itoa(i, tNum, 10);
						strcat(name, tNum);
						m_pVariableList->AddGuid(name, NULL_GUID);
					}
				}	
			}
		}

		//	SAFE_DELETE_ARRAY(firstVarName);
	}
	return 1;
}




double CScript::define(const char* CmdStr, char* retStr)
{
	char* varname=GetStringParam(CmdStr,0);

	if (varname)
	{
		// �����±�ֵ
		int lArray = CVariableList::VAR_NOT_FOUND;
		char MainVarName[256];
		char InnVarName[256];

		// m_Var[num].c_str() ��һ���ַ���Ϊ����������ַ�������ı�ʶ��
		char* MainVarNamePtr = MainVarName;
		char* InnVarNamePtr = InnVarName;
		char* varTypeFlag = (char*)varname;
		long tempPos = 0;

		double ret = CheckParamStringForArray(varTypeFlag, strlen(varname), &MainVarNamePtr, 256, &InnVarNamePtr, 256, tempPos);
		if(1.0f == ret) //����
		{
			if(InnVarNamePtr[0] == '$') // �±��Ǳ���
				lArray = GetScriptVarValue((CMoveShape*)p_SrcShape, InnVarName, 0);
			else
				lArray = atoi(InnVarName);

			if(MainVarNamePtr[0] == '#') // �ַ���
			{
				char* chValue = GetStringParam(CmdStr, 1);
				if(chValue)
				{
					char szNewStrName[128];
					for(int lArrPos=0; lArrPos<lArray; lArrPos++)
					{
						_snprintf(szNewStrName, 128, "%s%d", MainVarNamePtr, lArrPos);
						if(p_SrcShape && (OBJECT_TYPE)p_SrcShape->GetType() == TYPE_PLAYER)
						{
							((CPlayer*)this->GetSrcShape())->GetVariableList()->AddVar(szNewStrName, chValue);
						}
						if(p_SrcShape && (OBJECT_TYPE)p_SrcShape->GetType() == TYPE_MONSTER)
						{
							((CMonster*)this->GetSrcShape())->GetVariableList()->AddVar(szNewStrName, chValue);
						}
					}
					if(p_SrcShape && (OBJECT_TYPE)p_SrcShape->GetType() == TYPE_PLAYER)
					{
						GetGame()->UpdateSavePlayer(dynamic_cast<CPlayer*>(p_SrcShape), SAVE_DETAIL_ATTRVARLIST);
					}
					M_FREE( chValue, sizeof(char)*MAX_VAR_LEN );
					M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
					return true;
				}
			}
			else
			{
				int value = GetIntParam(CmdStr, 1);
				if(value != ERROR_CODE)
				{
					if(p_SrcShape && (OBJECT_TYPE)p_SrcShape->GetType() == TYPE_PLAYER)
					{
						if(MainVarNamePtr[0] == '$') // ��������
							((CPlayer*)this->GetSrcShape())->GetVariableList()->AddVar(MainVarNamePtr, lArray, value);
						else if(MainVarNamePtr[0] == '@') // GUID����
						{
							char szGuidName[128];
							for(int lArrPos=0; lArrPos<lArray; lArrPos++)
							{
								_snprintf(szGuidName, 128, "%s%d", MainVarNamePtr, lArrPos);
								((CPlayer*)this->GetSrcShape())->GetVariableList()->AddGuid(szGuidName, NULL_GUID);
							}
						}
						GetGame()->UpdateSavePlayer(dynamic_cast<CPlayer*>(p_SrcShape), SAVE_DETAIL_ATTRVARLIST);
					}
					if(p_SrcShape && (OBJECT_TYPE)p_SrcShape->GetType() == TYPE_MONSTER)
					{
						if(MainVarNamePtr[0] == '$') // ��������
							((CMonster*)this->GetSrcShape())->GetVariableList()->AddVar(MainVarNamePtr, lArray, value);
						else if(MainVarNamePtr[0] == '@') // GUID����
						{
							char szGuidName[128];
							for(int lArrPos=0; lArrPos<lArray; lArrPos++)
							{
								_snprintf(szGuidName, 128, "%s%d", MainVarNamePtr, lArrPos);
								((CMonster*)this->GetSrcShape())->GetVariableList()->AddGuid(szGuidName, NULL_GUID);
							}
						}
					}
				}
				else
				{
					TestLog(CStringReading::LoadString(IDS_GS_FUNCTION, STR_GS_FUNCTION_DEFINEVARNOTINT));
					M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
					return false;
				}
			}
		}
		else if(0.0f == ret)
		{
			if(varname[0] == '$') // ����
			{
				int value = GetIntParam(CmdStr, 1);
				if(value != ERROR_CODE)
				{
					if(p_SrcShape && (OBJECT_TYPE)p_SrcShape->GetType() == TYPE_PLAYER)
					{
						((CPlayer*)this->GetSrcShape())->GetVariableList()->AddVar(varname, value);
						GetGame()->UpdateSavePlayer((CPlayer*)p_SrcShape, SAVE_DETAIL_ATTRVARLIST);
						M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
						return true;
					}
					if(p_SrcShape && (OBJECT_TYPE)p_SrcShape->GetType() == TYPE_MONSTER)
					{
						((CMonster*)this->GetSrcShape())->GetVariableList()->AddVar(varname, value);
						M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
						return true;
					}
				}
				else
				{
					TestLog(CStringReading::LoadString(IDS_GS_FUNCTION, STR_GS_FUNCTION_DEFINEVARNOTINT));
					M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
					return false;
				}
			}
			else if(varname[0] == '@')
			{
				if(p_SrcShape && (OBJECT_TYPE)p_SrcShape->GetType() == TYPE_PLAYER)
				{
					((CPlayer*)this->GetSrcShape())->GetVariableList()->AddGuid(varname, NULL_GUID);
					GetGame()->UpdateSavePlayer((CPlayer*)p_SrcShape, SAVE_DETAIL_ATTRVARLIST);
					M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
					return true;
				}
				if(p_SrcShape && (OBJECT_TYPE)p_SrcShape->GetType() == TYPE_MONSTER)
				{
					((CMonster*)this->GetSrcShape())->GetVariableList()->AddGuid(varname, NULL_GUID);
					M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
					return true;
				}
			}
			else if(varname[0] == '#')
			{
				char* chValue = GetStringParam(CmdStr, 1);
				if(chValue)
				{
					if(p_SrcShape && (OBJECT_TYPE)p_SrcShape->GetType() == TYPE_PLAYER)
					{
						((CPlayer*)this->GetSrcShape())->GetVariableList()->AddVar(varname, chValue);
					}
					if(p_SrcShape && (OBJECT_TYPE)p_SrcShape->GetType() == TYPE_MONSTER)
					{
						((CMonster*)this->GetSrcShape())->GetVariableList()->AddVar(varname, chValue);
					}
					
					if(p_SrcShape && (OBJECT_TYPE)p_SrcShape->GetType() == TYPE_PLAYER)
					{
						GetGame()->UpdateSavePlayer(dynamic_cast<CPlayer*>(p_SrcShape), SAVE_DETAIL_ATTRVARLIST);
					}
					M_FREE( chValue, sizeof(char)*MAX_VAR_LEN );
					M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
					return true;
				}
			}
		}
		M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
		return true;
	}
	return false;
}
double CScript::undefine(const char* CmdStr, char* retStr)
{
	char* varname=GetStringParam(CmdStr,0);
	if (varname)
	{
		if(p_SrcShape && ((OBJECT_TYPE)p_SrcShape->GetType() == TYPE_PLAYER || (OBJECT_TYPE)p_SrcShape->GetType() == TYPE_MONSTER))
		{
			if(varname[0] == '$')
				static_cast<CMoveShape*>( this->GetSrcShape())->GetVariableList()->RemoveVar(varname);
			else if(varname[0] == '@' 
					||varname[0] == '#' )
			{
				// �����±�ֵ
				int lArray = CVariableList::VAR_NOT_FOUND;
				char MainVarName[256];
				char InnVarName[256];

				// m_Var[num].c_str() ��һ���ַ���Ϊ����������ַ�������ı�ʶ��
				char* MainVarNamePtr = MainVarName;
				char* InnVarNamePtr = InnVarName;
				char* varTypeFlag = (char*)varname;
				long tempPos = 0;

				double ret = CheckParamStringForArray(varTypeFlag, strlen(varname), &MainVarNamePtr, 256, &InnVarNamePtr, 256, tempPos);
				if(1.0f == ret)
				{
					if(InnVarNamePtr[0] == '$') // �±��Ǳ���
						lArray = GetScriptVarValue((CMoveShape*)p_SrcShape, InnVarName, 0);
					else
						lArray = atoi(InnVarName);

					for(int i=0; i<lArray; i++)
					{
						char szStrNewName[128];
						_snprintf(szStrNewName, 128, "%s%d", MainVarNamePtr, i);
						if(MainVarNamePtr[0] == '#')
							static_cast<CMoveShape*>( this->GetSrcShape())->GetVariableList()->RemoveVar(szStrNewName);
						else if(MainVarNamePtr[0] == '@')
							static_cast<CMoveShape*>( this->GetSrcShape())->GetVariableList()->RemoveGuid(szStrNewName);
					}
				}
				else if(0.0f == ret)
				{
					if(varname[0] == '#')
						static_cast<CMoveShape*>( this->GetSrcShape())->GetVariableList()->RemoveVar(varname);
					else if(varname[0] == '@')
						static_cast<CMoveShape*>( this->GetSrcShape())->GetVariableList()->RemoveGuid(varname);
				}
			}
		
			if( p_SrcShape->GetType() == TYPE_PLAYER )
			{
				GetGame()->UpdateSavePlayer((CPlayer*)p_SrcShape, SAVE_DETAIL_ATTRVARLIST);
			}
			M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
			return 1.0f;
		}
	}
	M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
	return 0;
}
double CScript::Ifdef(const char* CmdStr, char* retStr)
{

	char* varname=GetStringParam(CmdStr,0);
	if (varname)
	{
		bool bRet=false;
		if(p_SrcShape && (OBJECT_TYPE)p_SrcShape->GetType() == TYPE_PLAYER)
		{
			bRet = ((CPlayer*)this->GetSrcShape())->GetVariableList()->isExist(varname);
			M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
			return bRet;
		}
	}
	M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
	return 0;
}


double CScript::DestDefine(const char* cmd, char* retStr)
{
	CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(p_DesShape);
	if(!pMoveShape)	return false;

	char* varname=GetStringParam(cmd,0);
	if (varname)
	{
		// �����±�ֵ
		int lArray = CVariableList::VAR_NOT_FOUND;
		char MainVarName[256];
		char InnVarName[256];

		// m_Var[num].c_str() ��һ���ַ���Ϊ����������ַ�������ı�ʶ��
		char* MainVarNamePtr = MainVarName;
		char* InnVarNamePtr = InnVarName;
		char* varTypeFlag = (char*)varname;
		long tempPos = 0;

		double ret = CheckParamStringForArray(varTypeFlag, strlen(varname), &MainVarNamePtr, 256, &InnVarNamePtr, 256, tempPos);
		if(1.0f == ret) //����
		{
			if(InnVarNamePtr[0] == '$') // �±��Ǳ���
				lArray = GetScriptVarValue(pMoveShape, InnVarName, 0);
			else
				lArray = atoi(InnVarName);

			if(MainVarNamePtr[0] == '#') // �ַ���
			{
				char* chValue = GetStringParam(cmd, 1);
				if(chValue)
				{
					char szNewStrName[128];
					for(int lArrPos=0; lArrPos<lArray; lArrPos++)
					{
						_snprintf(szNewStrName, 128, "%s%d", MainVarNamePtr, lArrPos);
						if((OBJECT_TYPE)pMoveShape->GetType() == TYPE_PLAYER)
						{
							((CPlayer*)pMoveShape)->GetVariableList()->AddVar(szNewStrName, chValue);
						}
						if((OBJECT_TYPE)pMoveShape->GetType() == TYPE_MONSTER)
						{
							((CMonster*)pMoveShape)->GetVariableList()->AddVar(szNewStrName, chValue);
						}
					}
					if((OBJECT_TYPE)pMoveShape->GetType() == TYPE_PLAYER)
					{
						GetGame()->UpdateSavePlayer(dynamic_cast<CPlayer*>(pMoveShape), SAVE_DETAIL_ATTRVARLIST);
					}
					M_FREE( chValue, sizeof(char)*MAX_VAR_LEN );
					M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
					return true;
				}
			}
			else
			{
				int value = GetIntParam(cmd, 1);
				if(value != ERROR_CODE)
				{
					if(pMoveShape->GetType() == TYPE_PLAYER)
					{
						if(MainVarNamePtr[0] == '$') // ��������
							((CPlayer*)pMoveShape)->GetVariableList()->AddVar(MainVarNamePtr, lArray, value);
						else if(MainVarNamePtr[0] == '@') // GUID����
						{
							char szGuidName[128];
							for(int lArrPos=0; lArrPos<lArray; lArrPos++)
							{
								_snprintf(szGuidName, 128, "%s%d", MainVarNamePtr, lArrPos);
								((CPlayer*)pMoveShape)->GetVariableList()->AddGuid(szGuidName, NULL_GUID);
							}
						}
						GetGame()->UpdateSavePlayer(dynamic_cast<CPlayer*>(pMoveShape), SAVE_DETAIL_ATTRVARLIST);
					}
					if((OBJECT_TYPE)pMoveShape->GetType() == TYPE_MONSTER)
					{
						if(MainVarNamePtr[0] == '$') // ��������
							((CMonster*)pMoveShape)->GetVariableList()->AddVar(MainVarNamePtr, lArray, value);
						else if(MainVarNamePtr[0] == '@') // GUID����
						{
							char szGuidName[128];
							for(int lArrPos=0; lArrPos<lArray; lArrPos++)
							{
								_snprintf(szGuidName, 128, "%s%d", MainVarNamePtr, lArrPos);
								((CMonster*)pMoveShape)->GetVariableList()->AddGuid(szGuidName, NULL_GUID);
							}
						}
					}
				}
				else
				{
					TestLog(CStringReading::LoadString(IDS_GS_FUNCTION, STR_GS_FUNCTION_DEFINEVARNOTINT));
					M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
					return false;
				}
			}
		}
		else if(0.0f == ret)
		{
			if(varname[0] == '$') // ����
			{
				int value = GetIntParam(cmd, 1);
				if(value != ERROR_CODE)
				{
					if((OBJECT_TYPE)pMoveShape->GetType() == TYPE_PLAYER)
					{
						((CPlayer*)pMoveShape)->GetVariableList()->AddVar(varname, value);
						GetGame()->UpdateSavePlayer((CPlayer*)pMoveShape, SAVE_DETAIL_ATTRVARLIST);
						M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
						return true;
					}
					if((OBJECT_TYPE)pMoveShape->GetType() == TYPE_MONSTER)
					{
						((CMonster*)pMoveShape)->GetVariableList()->AddVar(varname, value);
						M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
						return true;
					}
				}
				else
				{
					TestLog(CStringReading::LoadString(IDS_GS_FUNCTION, STR_GS_FUNCTION_DEFINEVARNOTINT));
					M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
					return false;
				}
			}
			else if(varname[0] == '@')
			{
				if((OBJECT_TYPE)pMoveShape->GetType() == TYPE_PLAYER)
				{
					((CPlayer*)pMoveShape)->GetVariableList()->AddGuid(varname, NULL_GUID);
					GetGame()->UpdateSavePlayer((CPlayer*)pMoveShape, SAVE_DETAIL_ATTRVARLIST);
					M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
					return true;
				}
				if((OBJECT_TYPE)pMoveShape->GetType() == TYPE_MONSTER)
				{
					((CMonster*)pMoveShape)->GetVariableList()->AddGuid(varname, NULL_GUID);
					M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
					return true;
				}
			}
			else if(varname[0] == '#')
			{
				char* chValue = GetStringParam(cmd, 1);
				if(chValue)
				{
					if((OBJECT_TYPE)pMoveShape->GetType() == TYPE_PLAYER)
					{
						((CPlayer*)pMoveShape)->GetVariableList()->AddVar(varname, chValue);
					}
					if((OBJECT_TYPE)pMoveShape->GetType() == TYPE_MONSTER)
					{
						((CMonster*)pMoveShape)->GetVariableList()->AddVar(varname, chValue);
					}
					
					if((OBJECT_TYPE)pMoveShape->GetType() == TYPE_PLAYER)
					{
						GetGame()->UpdateSavePlayer(dynamic_cast<CPlayer*>(pMoveShape), SAVE_DETAIL_ATTRVARLIST);
					}
					M_FREE( chValue, sizeof(char)*MAX_VAR_LEN );
					M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
					return true;
				}
			}
		}
		M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
		return true;
	}
	return false;
}
double CScript::DestUndefine(const char* cmd, char* retStr)
{
	CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(p_DesShape);
	if(!pMoveShape)	return false;

	char* varname=GetStringParam(cmd,0);
	if (varname)
	{
		if(pMoveShape->GetType() == TYPE_PLAYER || pMoveShape->GetType() == TYPE_MONSTER )
		{
			if(varname[0] == '$')
				pMoveShape->GetVariableList()->RemoveVar(varname);
			else if(varname[0] == '@' 
					||varname[0] == '#' )
			{
				// �����±�ֵ
				int lArray = CVariableList::VAR_NOT_FOUND;
				char MainVarName[256];
				char InnVarName[256];

				// m_Var[num].c_str() ��һ���ַ���Ϊ����������ַ�������ı�ʶ��
				char* MainVarNamePtr = MainVarName;
				char* InnVarNamePtr = InnVarName;
				char* varTypeFlag = (char*)varname;
				long tempPos = 0;

				double ret = CheckParamStringForArray(varTypeFlag, strlen(varname), &MainVarNamePtr, 256, &InnVarNamePtr, 256, tempPos);
				if(1.0f == ret)
				{
					if(InnVarNamePtr[0] == '$') // �±��Ǳ���
						lArray = GetScriptVarValue((CMoveShape*)pMoveShape, InnVarName, 0);
					else
						lArray = atoi(InnVarName);

					for(int i=0; i<lArray; i++)
					{
						char szStrNewName[128];
						_snprintf(szStrNewName, 128, "%s%d", MainVarNamePtr, i);
						if(MainVarNamePtr[0] == '#')
							pMoveShape->GetVariableList()->RemoveVar(szStrNewName);
						else if(MainVarNamePtr[0] == '@')
							pMoveShape->GetVariableList()->RemoveGuid(szStrNewName);
					}
				}
				else if(0.0f == ret)
				{
					if(varname[0] == '#')
						pMoveShape->GetVariableList()->RemoveVar(varname);
					else if(varname[0] == '@')
						pMoveShape->GetVariableList()->RemoveGuid(varname);
				}
			}
		
			if( pMoveShape->GetType() == TYPE_PLAYER )
			{
				GetGame()->UpdateSavePlayer((CPlayer*)pMoveShape, SAVE_DETAIL_ATTRVARLIST);
			}
			M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
			return 1.0f;
		}
	}
	M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
	return 0;
}
double CScript::IfDestDef(const char* cmd, char* retStr)
{
	bool bRet = false;
	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_DesShape);
	if(!pPlayer)	return bRet;

	char* varname=GetStringParam(cmd,0);
	if (varname)
	{
		bRet = pPlayer->GetVariableList()->isExist(varname);
	}
	M_FREE( varname, sizeof(char)*MAX_VAR_LEN );
	return bRet;
}


double CScript::Time(const char* CmdStr, char* retStr)
{
	time_t t;
	time(&t);
	tm* ps_Time;
	ps_Time=localtime(&t);
	char* pTimeStr=asctime( ps_Time );


	long result;
	result=ps_Time->tm_year;
	result<<=4;
	result+=ps_Time->tm_mon;
	result<<=5;
	result+=ps_Time->tm_mday;
	result<<=5;
	result+=ps_Time->tm_hour;
	result<<=6;
	result+=ps_Time->tm_min;
	result<<=3;
	result+=ps_Time->tm_wday;
	return result;
}
double CScript::Year(const char* CmdStr, char* retStr)
{
	double result;
	double lTime=GetIntParam(CmdStr, 0);
	if (lTime==ERROR_CODE)
	{	
		//��������,���ص�ǰ���
		time_t t;
		time(&t);
		tm* ps_Time;
		ps_Time=localtime(&t);
		result=ps_Time->tm_year+1900;
	}
	else
	{	
		//������,�Ӳ�������ȡ���
		result=((long)lTime)>>3>>6>>5>>5>>4;
		result+=1900;
	}
	return result;
}
double CScript::Month(const char* CmdStr, char* retStr)
{
	long result;
	double lTime=GetIntParam(CmdStr, 0);
	if (lTime==ERROR_CODE)
	{
		//��������,���ص�ǰ�·�
		time_t t;
		time(&t);
		tm* ps_Time;
		ps_Time=localtime(&t);
		result=ps_Time->tm_mon+1;
	}
	else
	{	
		//������,�Ӳ�������ȡ�·�
		result=((long)lTime)>>3>>6>>5>>5;
		result&=15;
		result++;
	}
	return result;
}
double CScript::Day(const char* CmdStr, char* retStr)
{
	long result;
	double lTime=GetIntParam(CmdStr, 0);
	if (lTime==ERROR_CODE)
	{	
		//��������,���ص�ǰ����
		time_t t;
		time(&t);
		tm* ps_Time;
		ps_Time=localtime(&t);
		result=ps_Time->tm_mday;
	}
	else
	{	
		//������,�Ӳ�������ȡ����
		result=((long)lTime)>>3>>6>>5;
		result&=31;
	}
	return result;
}
double CScript::Hour(const char* CmdStr, char* retStr)
{
	long result;
	double lTime=GetIntParam(CmdStr, 0);
	if (lTime==ERROR_CODE)
	{	
		//��������,���ص�ǰСʱ
		time_t t;
		time(&t);
		tm* ps_Time;
		ps_Time=localtime(&t);
		result=ps_Time->tm_hour;
	}
	else
	{	
		//������,�Ӳ�������ȡСʱ
		result=((long)lTime)>>3>>6;
		result&=31;
	}
	return result;
}
double CScript::Minute(const char* CmdStr, char* retStr)
{
	long result;
	double lTime=GetIntParam(CmdStr, 0);
	if (lTime==ERROR_CODE)
	{	
		//��������,���ص�ǰ����
		time_t t;
		time(&t);
		tm* ps_Time;
		ps_Time=localtime(&t);
		result=ps_Time->tm_min;
	}
	else
	{	
		//������,�Ӳ�������ȡ����
		result=((long)lTime)>>3;
		result&=63;
	}
	return result;
}
double CScript::DayOfWeek(const char* CmdStr, char* retStr)
{
	long result;
	double lTime=GetIntParam(CmdStr, 0);
	if (lTime==ERROR_CODE)
	{	
		//��������,���ص�ǰ���ڼ�
		time_t t;
		time(&t);
		tm* ps_Time;
		ps_Time=localtime(&t);
		result=ps_Time->tm_wday;
	}
	else
	{	
		//������,�Ӳ�������ȡ���ڼ�
		result=lTime;
		result&=7;
	}
	return result;
}
double CScript::HourDiff(const char* CmdStr, char* retStr)
{
	double lTime1=GetIntParam(CmdStr, 0);
	double lTime2=GetIntParam(CmdStr, 1);
	tm Time1;
	tm Time2;
	time_t	tTime1;
	time_t	tTime2;

	//�Ӳ�����ȡ��һ��ʱ��
	Time1.tm_year=((long)lTime1)>>3>>6>>5>>5>>4;
	Time1.tm_mon=(((long)lTime1)>>3>>6>>5>>5)&15;
	Time1.tm_mday=(((long)lTime1)>>3>>6>>5)&31;
	Time1.tm_hour=(((long)lTime1)>>3>>6)&31;
	Time1.tm_min=(((long)lTime1)>>3)&63;
	tTime1=mktime(&Time1);

	if(lTime2==ERROR_CODE)
	{
		//���û�еڶ�����,��ȡ��ǰʱ��
		time(&tTime2);
	}
	else
	{	
		//����еڶ�ʱ��,��ת��Ϊtime_t
		Time2.tm_year=((long)lTime2)>>3>>6>>5>>5>>4;
		Time2.tm_mon=(((long)lTime2)>>3>>6>>5>>5)&15;
		Time2.tm_mday=(((long)lTime2)>>3>>6>>5)&31;
		Time2.tm_hour=(((long)lTime2)>>3>>6)&31;
		Time2.tm_min=(((long)lTime2)>>3)&63;
		tTime2=mktime(&Time2);
	}
	return (int)difftime(tTime2,tTime1)/3600;	//һСʱ3600��
}
double CScript::MinuteDiff(const char* CmdStr, char* retStr)
{
	double lTime1=GetIntParam(CmdStr, 0);
	double lTime2=GetIntParam(CmdStr, 1);
	tm Time1;
	tm Time2;
	time_t	tTime1;
	time_t	tTime2;

	//�Ӳ�����ȡ��һ��ʱ��
	Time1.tm_year=((long)lTime1)>>3>>6>>5>>5>>4;
	Time1.tm_mon=(((long)lTime1)>>3>>6>>5>>5)&15;
	Time1.tm_mday=(((long)lTime1)>>3>>6>>5)&31;
	Time1.tm_hour=(((long)lTime1)>>3>>6)&31;
	Time1.tm_min=(((long)lTime1)>>3)&63;
	Time1.tm_sec=0;
	tTime1=mktime(&Time1);

	if(lTime2==ERROR_CODE)
	{
		//���û�еڶ�����,��ȡ��ǰʱ��
		time(&tTime2);
	}
	else
	{	
		//����еڶ�ʱ��,��ת��Ϊtime_t
		Time2.tm_year=((long)lTime2)>>3>>6>>5>>5>>4;
		Time2.tm_mon=(((long)lTime2)>>3>>6>>5>>5)&15;
		Time2.tm_mday=(((long)lTime2)>>3>>6>>5)&31;
		Time2.tm_hour=(((long)lTime2)>>3>>6)&31;
		Time2.tm_min=(((long)lTime2)>>3)&63;
		Time2.tm_sec=0;
		tTime2=mktime(&Time2);
	}
	return (int)difftime(tTime2,tTime1)/60;	//һ����60��
}

double CScript::Second(const char* CmdStr, char* retStr)
{
	time_t t;
	time(&t);
	tm* ps_Time;
	ps_Time=localtime(&t);
	return ps_Time->tm_sec;
}

double CScript::GetCurTick(const char* cmd, char *retStr )
{
	return ::timeGetTime();
}

double CScript::CheckGuidIsNull(const char* cmd, char* retStr)
{
	char* szGuid = GetStringParam(cmd, 0);
	if(!szGuid) return 0.0f;
	const CGUID &tGuid = GetScriptGuidValue((CMoveShape*)GetSrcShape(), szGuid);
	if(tGuid == NULL_GUID) 
	{
		M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
		return 1.0f;
	}
	M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
	return 0.0f;
}

double CScript::TestLog(const char* CmdStr, char* retStr)
{
	DWORD time = timeGetTime();
	char tt[32];
	ultoa(time, tt, 10);
	long lRet1 = strlen(CmdStr);
	long lRet2 = strlen(tt);
	char* ptr = (char*)M_ALLOC(lRet1 + lRet2 + 2);
	strcpy_s(ptr, lRet1 + lRet2 + 2, tt);
	strcat_s(ptr, lRet1 + lRet2 + 2, ":");
	strcat_s(ptr, lRet1 + lRet2 + 2, CmdStr);
	if (ptr)
	{
		PutDebugString(ptr);
		M_FREE(ptr, lRet1 + lRet2 + 2);
	}
	return 1;
}


double CScript::Int(const char* cmd, char* retStr)
{
	int lValue =GetIntParam(cmd, 0);
	if(lValue == ERROR_CODE)
		return 0;
	return lValue;
}

double CScript::RoundNum(const char* cmd, char* retStr)
{
	double lValue =GetIntParam(cmd, 0);
	if(lValue == ERROR_CODE)
		return 0;
	return Round(lValue);
}
double CScript::GetPow(const char* CmdStr, char* retSt)
{
	double lX=GetIntParam(CmdStr, 0);
	if(ERROR_CODE == lX)
		return -1.0f;

	double lY=GetIntParam(CmdStr, 1);
	if(ERROR_CODE == lY)
		return -1.0f;

	double llRet = std::pow(lX, lY);
	return llRet;
}

//! ��ȡһ���ַ������Ӵ�
//! ����1��Ҫ��ȡ���Ӵ��Ŀ�ʼλ�ã���1��ʼ���������λ�ô����ַ������ȣ�����ʧ�ܣ���Ϊ0���򷵻��ַ������һ���ֽ�, ��Ϊ����-n�򷵻س�ȥ���n���ֽڵ��ַ�����
//! ����2��Ҫ��ȡ���Ӵ��Ľ���λ�ã���1��ʼ���������λ�ô����ַ������ȣ�ת��Ϊ��󳤶ȣ�
//! ����3����ȡ��Ŀ���ַ���
//! ����4�������Ӵ��ı�����
//! ����ֵ��1����ȡ�ɹ���0����ȡʧ��
double CScript::GetSubString(const char* CmdStr, char* retStr)
{
	long BeginPos = GetIntParam(CmdStr, 0);
	long EndPos = GetIntParam(CmdStr, 1);

	char* pSrcStr =GetStringParam(CmdStr, 2);
	if (NULL == pSrcStr)
		return 0.0f;

	DWORD SrcLen = strlen(pSrcStr);
	if(0 == BeginPos)
	{
		BeginPos = SrcLen;
		EndPos = SrcLen;
	}
	else if (0 > BeginPos)
	{
		EndPos = SrcLen + BeginPos;
		BeginPos = 1;
	}
	else if(EndPos > SrcLen)
		EndPos = SrcLen;

	if (BeginPos > SrcLen || EndPos < BeginPos)
	{
		M_FREE( pSrcStr, sizeof(char)*MAX_VAR_LEN );
		return 0.0f;
	}

	char* pDstName =GetStringParam(CmdStr, 3);
	if (NULL == pDstName)
	{
		M_FREE( pSrcStr, sizeof(char)*MAX_VAR_LEN );
		return 0.0f;
	}

	char szRe[MAX_VAR_LEN] = {0};
	memcpy(szRe, pSrcStr + (BeginPos - 1), EndPos - BeginPos + 1);
	int ret = SetScriptVarValue(dynamic_cast<CMoveShape*>(p_SrcShape), pDstName, szRe);


	M_FREE( pSrcStr, sizeof(char)*MAX_VAR_LEN );
	M_FREE( pDstName, sizeof(char)*MAX_VAR_LEN );

	return ret;
}
