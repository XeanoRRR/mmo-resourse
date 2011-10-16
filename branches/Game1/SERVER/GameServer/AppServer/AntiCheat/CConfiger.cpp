#include "StdAfx.h"
#include "CConfiger.h"
#include "..\script\Script.h"
#include "..\Player.h"
#include "..\Region.h"
#include "..\Shape.h"
#include "..\..\GameServer\game.h"


namespace AntiCheat
{
	/// ����Ϣ��������
	void CConfiger::DecodeFromDataBlock(DBReadSet &db )
	{
		m_RightScripts.clear();
		m_WrongScripts.clear();
		db.GetBufferFromByteArray(&m_sys,sizeof(m_sys));

		string RightScriptFile;

		/// ��ȡ������ʩ
		long  RightNum = db.GetLongFromByteArray();
		for (long l=0 ; l<RightNum ; l++)
		{
			int RightScriptKey = db.GetLongFromByteArray();
			CHAR strTemp[256];
			ScriptCfg sc;
			sc.file = db.GetStringFromByteArray(strTemp,256);
			sc.limit = db.GetByteFromByteArray();
			m_RightScripts[RightScriptKey] = sc;
		}

		/// ��ȡ�ͷ���ʩ
		long  WorongNum = db.GetLongFromByteArray();
		for (long l=0 ; l<WorongNum ; l++)
		{
			int WrongScriptKey = db.GetLongFromByteArray();
			CHAR strTemp[256];
			ScriptCfg sc;
			sc.file = db.GetStringFromByteArray(strTemp,256);
			sc.limit = db.GetByteFromByteArray();
			m_WrongScripts[WrongScriptKey] = sc;
		}

		// �������д��б�
		DecodeSensWords( db );
	}

	void CConfiger::DecodeSensWords( DBReadSet &db )
	{
		m_SensWords.clear();
		long size = db.GetLongFromByteArray();
		char Word[512];
		for( long i = 0; i < size; ++ i )
		{
			db.GetStringFromByteArray( Word, sizeof( Word ) );
			if( std::find( m_SensWords.begin(), m_SensWords.end(), Word ) ==
				m_SensWords.end() )
			{
				m_SensWords.push_back( Word );
			}
		}
	}

	/// �ش���ȷ�ű�����
	void CConfiger::OnAnswerRight(const CPlayer *player, int rightTimes, bool bLimit )
	{
		for (SCon_LIST itScript = m_RightScripts.begin();  itScript != m_RightScripts.end() ; itScript ++)
		{
			bool bExecuted = true;
			const ScriptCfg &sc = itScript->second;
			if( bLimit && sc.limit != 0 )
			{
				bExecuted = false;
			}
			if ( bExecuted && itScript->first != 0 && (rightTimes % itScript->first) == 0)
			{
				stRunScript stScript;
				stScript.pszFileName =  (char *)sc.file.c_str();
				stScript.srcShape    =  const_cast<CPlayer*>(player);
				stScript.desShape    =  const_cast<CPlayer*>(player);
				stScript.pRegion	 =  dynamic_cast<CRegion*>(player->GetFather());
				RunScript(&stScript,(char*)GetGame()->GetScriptFileData(stScript.pszFileName));
			}
		}
	}

	/// �ش����ű�����
	void CConfiger::OnAnswerWrong(const CPlayer *player, int wrongTimes )
	{
		for( ScriptsTable::reverse_iterator it = m_WrongScripts.rbegin();
			it != m_WrongScripts.rend(); ++ it )
		{
			// ��Χ���� [ it->first <= wrongTimes < it_next->first ]
			if( it->first != 0 && wrongTimes / it->first > 0 )
			{
				stRunScript stScript;
				stScript.pszFileName =  (char *)it->second.file.c_str();
				stScript.srcShape    =  const_cast<CPlayer*>(player);
				stScript.desShape    =  const_cast<CPlayer*>(player);
				stScript.pRegion	 =  dynamic_cast<CRegion*>(player->GetFather());
				RunScript(&stScript,(char*)(GetGame()->GetScriptFileData(stScript.pszFileName)));

				break;
			}
		}
	}

	bool CConfiger::HasSensWord( const std::string &content )
	{
		for( SensWordsList::const_iterator it = m_SensWords.begin(); it != m_SensWords.end(); ++ it )
		{
			if( content.find( *it ) != std::string::npos )
			{
				return true;
			}
		}

		return false;
	}
}