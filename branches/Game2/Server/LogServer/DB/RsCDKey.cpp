#include "StdAfx.h"
#include ".\rscdkey.h"
#include "..\..\nets\basemessage.h"
#include "..\..\nets\MySocket.h"
#include "..\..\server\loginserver\loginserver\game.h"
#include "../../public/Date.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRsCDKey::CRsCDKey(void)
{
}

CRsCDKey::~CRsCDKey(void)
{
}

//����:	���CDKEY�Ƿ���Ч
//����:	�ʺţ�����
//����:
//		LOGIN_OK				�ɹ�
//		LOGINDB_ERROR			���ݿ�򿪴���
//		LOGIN_CDKEYDISABLE		�˺�ͣ��
//		LOGIN_BEBANED			�ʺŽ���ʱ��
//		LOGIN_PASSWORDERROR		�������
//		LOGIN_NOTCDKEY			�ʺŲ�����
//��ע:
//		�������3������������룬����1���Ӹ��˻�
int CRsCDKey::CDKeyIsValidate(DWORD dwIP ,const char *cdkey,vector<BYTE>& password, char *szOutBanTime)
{
	//SQL���İ�ȫ�Լ��,���ܺ���',=,''�ȵ�
	string strCheck(cdkey);
	long lPos = (long)strCheck.find_first_of("'");
	if(lPos >= 0)
		return LOGIN_NOTCDKEY;
	lPos = (long)strCheck.find_first_of("=");
	if(lPos >= 0)
		return LOGIN_NOTCDKEY;
	lPos = (long)strCheck.find_first_of(" ");
	if(lPos >= 0)
		return LOGIN_NOTCDKEY;

	// ����ʺ��Ƿ����
	char sql[256];
	_ConnectionPtr cn;
	_RecordsetPtr rs;

	try
	{
		CreateCn(cn);
		CreateRs(rs);

		OpenCn(cn);
		sprintf(sql,"SELECT * FROM CSL_CDKEY WHERE cdkey='%s'", cdkey); //cdkey is PK
		OpenRs(sql, rs, cn/*m_Connection*/);

		// �ʻ�������
		if(rs->GetadoEOF()) 
		{
			ReleaseRs(rs);
			ReleaseCn(cn);
			return LOGIN_NOTCDKEY;
		}
		// �ʻ�����
		else 
		{
			// ����ʺ��Ƿ񱻽�ֹ
//			char currtime[20];
//			char bantime[20];
			SYSTEMTIME ct,bt;

			GetLocalTime(&ct);
			tagTime cTime,bTime;
			memcpy(&cTime,&ct,sizeof(tagTime));

			// 2004-12-31 14:16:1
			//wsprintf(currtime, "%d-%d-%d %d:%d:%d", ct.wYear,ct.wMonth,ct.wDay,ct.wHour,ct.wMinute,ct.wSecond);
			
			// 1999-1-1 1:1:1
			VariantTimeToSystemTime(double( rs->GetCollect("ban_time") ) ,&bt);


			memcpy(&bTime,&bt,sizeof(tagTime));

			
			//wsprintf(bantime,"%s",);

			if(cTime < bTime)
			{
				ReleaseRs(rs);
				ReleaseCn(cn);
				sprintf(szOutBanTime, "%d��%d��%d�� %d��%d��",
					bTime.Year(), bTime.Month(), bTime.Day(), bTime.Hour(), bTime.Minute());
				return LOGIN_BEBANED; 
			}

			string strMD5;
			if( GetPassWord(rs,strMD5) == false )
			{
				ReleaseRs(rs);
				ReleaseCn(cn);
				return LOGINDB_ERROR; // ���ݿ��������  
			}
			char strDBMD5[50];
			strcpy(strDBMD5,strMD5.c_str());
			strupr(strDBMD5);

			//�Ѷ���������ת�����ַ���
			char strClientMD5[50];
			static char hexDigits[] = {'0', '1', '2', '3', '4', '5', '6', '7',
				'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
			BYTE* pByte = &password[0];
			for(int i=0;i < 16;i++)
			{
				int bValue = password[i];
				int bValue1 = bValue>>4;
				int bValue2 = bValue&0x0F;
				if(bValue1 >= 0 && bValue1 <= 15 && bValue2 >= 0 && bValue2 <= 15)
				{
					strClientMD5[i*2] = hexDigits[bValue1];
					strClientMD5[i*2+1] = hexDigits[bValue2];
				}
			}
			strClientMD5[32]='\0';
			strupr(strClientMD5);
			
			if( strcmp(strDBMD5,strClientMD5) != 0 ) // ���벻��
			{
				// �������+1
				map<string,int>::iterator it=GetGame()->GetPWErrorMap()->find(string(cdkey));
				if(it!=GetGame()->GetPWErrorMap()->end())
				{
					//3�δ�������������ø��ʺ�
					if(it->second>=2)
					{
						CDKeyBan(cdkey,1);
						GetGame()->GetPWErrorMap()->erase(it);
					}
					else 
					{
						it->second++;
					}
				}
				else
				{
					GetGame()->GetPWErrorMap()->insert(map<string,int>::value_type(string(cdkey),1));
				}

				ReleaseRs(rs);
				ReleaseCn(cn);
				return  LOGIN_PASSWORDERROR;
			}
			else	//������ȷ
			{
				/*
				 * 	������IP��⣬�Ƿ�������ҵ�¼
				 */

				// ���ip�Ƿ�����
				if( !IPIsAllowed(dwIP) )
				{
					return LOGIN_IP_FORBID;
				}

				// ���ip�Ƿ񱻽�ֹ
				if( IPIsForbidded(dwIP) )
				{
					return LOGIN_IP_FORBID;
				}

				
				/*
				 * 	����IP�б��⣬�Ƿ�����GM��¼
				 */
				if( !IsBetweenIP(cdkey, dwIP) )	{
					ReleaseRs(rs);
					ReleaseCn(cn);
					return LOGIN_CDKEYDISABLE;
				}


				//��������������б��¼
				map<string,int>::iterator it=GetGame()->GetPWErrorMap()->find(string(cdkey));
				if(it!=GetGame()->GetPWErrorMap()->end())
				{
					GetGame()->GetPWErrorMap()->erase(it);
				}

				ReleaseRs(rs);
				ReleaseCn(cn);
				return  LOGIN_OK;			// ƥ��
			}
			
		}
	}
	catch(_com_error &e)
	{
		ReleaseRs(rs);
		ReleaseCn(cn);
		PrintErr("RsCDKey cdkey is validate eorror", e);
		return LOGINDB_ERROR; // ���ݿ�������� 
	}
}

//������	�����ʺ�
//������	�ʺš�ʱ�䣨��λ�����ӣ�
bool CRsCDKey::CDKeyBan(const char* cdkey,long lminute)
{
	char sql[256];
	_ConnectionPtr cn;

	if(!CreateCn(cn)) return false;
	if(!OpenCn(cn)) return false;

	sprintf(sql,"UPDATE CSL_CDKEY SET ban_time=DATEADD(minute,%d,GETDATE()) WHERE cdkey='%s'",
		lminute,cdkey);
	if(!ExecuteCn(sql , cn/*m_Connection*/))
	{
		ReleaseCn(cn);
		AddLogText("RcCDKey cdkey ban error");
		return false;
	}
	ReleaseCn(cn);
	return true;
}

bool CRsCDKey::GetPassWord(_RecordsetPtr& rs,BYTE* pByte)
{
	try
	{
		long lSize = rs->GetFields()->GetItem("password")->ActualSize;
		if(lSize == 16)
		{
			_variant_t varian_t1 = rs->GetCollect("password");
			BYTE* pBuf = NULL;
			SafeArrayAccessData(varian_t1.parray,(void**)&pBuf);
			SafeArrayUnaccessData(varian_t1.parray);
			if(pBuf)
			{
				memcpy(pByte,pBuf,16);
				return true;
			}
		}
		
	}
	catch (_com_error &e)
	{
		PrintErr("Get PassWordeorror", e);
		return false;
	}
	return false;
}


bool CRsCDKey::GetPassWord(_RecordsetPtr& rs,string& strMD5)
{
	try
	{
		strMD5 = (_bstr_t)rs->GetCollect("password");
		if(strMD5.length() != 32)
		{
			return false;
		}
		return true;
	}
	catch (_com_error &e)
	{
		PrintErr("Get PassWordeorror", e);
		return false;
	}
	return false;
}

bool CRsCDKey::IPIsAllowed(DWORD dwIP)
{
	if( !GetGame()->GetSetup()->bCheckAllowIP )
		return true;

	// ����IP�ĸ��ֽڵ�˳��
	BYTE bt1 = LOBYTE(LOWORD(dwIP));
	BYTE bt2 = HIBYTE(LOWORD(dwIP));
	BYTE bt3 = LOBYTE(HIWORD(dwIP));
	BYTE bt4 = HIBYTE(HIWORD(dwIP));
	dwIP = MAKELONG(MAKEWORD(bt4, bt3), MAKEWORD(bt2, bt1));

	// ���IP�Ƿ���������ҵ��
	char sql[256];

	_ConnectionPtr cn;
	_RecordsetPtr rs;

	try
	{
		CreateCn(cn);
		CreateRs(rs);

		OpenCn(cn);
		sprintf(sql,"SELECT count(*) as exp1 FROM ip_allow where 0x%x >= int_begin and 0x%x <= int_end", dwIP, dwIP);
		OpenRs(sql, rs, cn/*m_Connection*/);

		if( !rs->GetadoEOF() )
		{
			long lCount = rs->GetCollect("exp1");
			if (lCount > 0)
			{
				ReleaseRs(rs);
				ReleaseCn(cn);
				return true;
			}
		}
	}
	catch(_com_error &e)
	{
		PrintErr("check ip allow error", e);
	}

	
	ReleaseRs(rs);
	ReleaseCn(cn);
	return false;
}

bool CRsCDKey::IPIsForbidded(DWORD dwIP)
{
	if( !GetGame()->GetSetup()->bCheckForbidIP )
		return false;

	// ����IP�ĸ��ֽڵ�˳��
	BYTE bt1 = LOBYTE(LOWORD(dwIP));
	BYTE bt2 = HIBYTE(LOWORD(dwIP));
	BYTE bt3 = LOBYTE(HIWORD(dwIP));
	BYTE bt4 = HIBYTE(HIWORD(dwIP));
	dwIP = MAKELONG(MAKEWORD(bt4, bt3), MAKEWORD(bt2, bt1));

	// ���IP�Ƿ��ֹ������ҵ��
	char sql[256];

	_ConnectionPtr cn;
	_RecordsetPtr rs;

	try
	{
		CreateCn(cn);
		CreateRs(rs);

		OpenCn(cn);
		sprintf(sql,"SELECT count(*) as exp1 FROM ip_forbid where 0x%x >= int_begin and 0x%x <= int_end", dwIP, dwIP);
		OpenRs(sql, rs, cn/*m_Connection*/);

		if( !rs->GetadoEOF() )
		{
			long lCount = rs->GetCollect("exp1");
			if (lCount > 0)
			{
				ReleaseRs(rs);
				ReleaseCn(cn);
				return true;
			}
		}
	}
	catch(_com_error &e)
	{
		PrintErr("check ip allow error", e);
	}


	ReleaseRs(rs);
	ReleaseCn(cn);
	return false;
}

bool CRsCDKey::IsBetweenIP(const char *szCdkey, DWORD dwIP) {
	
	if( !GetGame()->GetSetup()->bCheckBetweenIP ) return true;

	char sql[256];
	unsigned char ipv4[4] = {0,0,0,0};

	ipv4[0] = LOBYTE(LOWORD(dwIP)); 
	ipv4[1] = HIBYTE(LOWORD(dwIP));
	ipv4[2] = LOBYTE(HIWORD(dwIP));
	ipv4[3] = HIBYTE(HIWORD(dwIP));
	_ConnectionPtr cn;
	_RecordsetPtr rs;

	try {
		CreateCn(cn);
		CreateRs(rs);

		OpenCn(cn);
		sprintf(sql,"SELECT * FROM ip_list WHERE cdkey='%s'", szCdkey);
		OpenRs(sql, rs, cn/*m_Connection*/);

		// �����������ip�б��У���ֱ�ӽ�ȥ��
		if( rs->GetadoEOF() ) {
			ReleaseRs(rs);
			ReleaseCn(cn);
			return true;
		}
		// ���ip�б���ڣ�����Ҫƥ���б� ��ûһ��ip�ķ�Χ��
		// ����������1��ƥ���������룬���в�ƥ����������롣
		else {
			bool bIsLessEqual; // is <=
			bool bIsMoreEqual; // is >=
			unsigned long ipb;
			unsigned char ipv4b[4] = {0,0,0,0};
			
			unsigned long ipe;
			unsigned char ipv4e[4] = {0,0,0,0};

			while( !rs->GetadoEOF() ) {

				ipb = STR2UL( (_bstr_t)(rs->GetCollect("ip_begin")) );

				ipe = STR2UL( (_bstr_t)(rs->GetCollect("ip_end")) );
				
				ipv4b[0] = LOBYTE(LOWORD(ipb)); 
				ipv4b[1] = HIBYTE(LOWORD(ipb));
				ipv4b[2] = LOBYTE(HIWORD(ipb));
				ipv4b[3] = HIBYTE(HIWORD(ipb));

				ipv4e[0] = LOBYTE(LOWORD(ipe)); 
				ipv4e[1] = HIBYTE(LOWORD(ipe));
				ipv4e[2] = LOBYTE(HIWORD(ipe));
				ipv4e[3] = HIBYTE(HIWORD(ipe));

				// �жϴ���
				bIsMoreEqual = false;
				bIsLessEqual = false;

				// �ȼ��  ��ǰIP�Ƿ���ڵ�����ʼIP
				if( ipv4[0] == ipv4b[0] ) {
					if( ipv4[1] == ipv4b[1] ) {
						if( ipv4[2] == ipv4b[2] ) {
							if( ipv4[3] >= ipv4b[3] ) {
								bIsMoreEqual = true;
							}
						}
						else if( ipv4[2] > ipv4b[2] ) {
							bIsMoreEqual = true;
						}
					}
					else if( ipv4[1] > ipv4b[1] ) {
						bIsMoreEqual = true;
					}
				}
				else if( ipv4[0] > ipv4b[0] ) {
					bIsMoreEqual = true;
				}
				// �ڼ������ ��ǰIP�Ƿ�С�ڵ��ڽ���IP
				if(bIsMoreEqual) {
					if( ipv4[0] == ipv4e[0] ) {
						if( ipv4[1] == ipv4e[1] ) {
							if( ipv4[2] == ipv4e[2] ) {
								if( ipv4[3] <= ipv4e[3] ) {
									bIsLessEqual = true;
								}
							}
							else if( ipv4[2] < ipv4e[2] ) {
								bIsLessEqual = true;
							}
						}
						else if( ipv4[1] < ipv4e[1] ) {
							bIsLessEqual = true;
						}
					}
					else if( ipv4[0] < ipv4e[0] ) {
						bIsLessEqual = true;
					}
				}

				// �����ǰip���б��ڣ��������ȥ��
				if( bIsLessEqual && bIsMoreEqual ) {
					ReleaseRs(rs);
					ReleaseCn(cn);
					return true;
				}

				rs->MoveNext();
			}
		}
	}
	catch(_com_error &e) {
		PrintErr("check ip list error", e);
	}

	ReleaseRs(rs);
	ReleaseCn(cn);
	return false;
}
