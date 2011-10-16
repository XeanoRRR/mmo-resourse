

#pragma once


class LogClient;
class WSTeam;
class CPlayer;
class CFaction;
struct tagFacApplyPerson;
struct tagFacMemInfo;

//! �߼���־�ӿڷ�װ��
//! ����־������װΪ���߼�����Ķ���ָ��
class LogicLogInterface
{
public:
	LogicLogInterface(LogClient *pLogClient);


//! ��־��¼�߼��ӿ�����
//! ����Ľӿڶ������߼�ģ���Լ���ɣ���Ҫ������LogicLogInterface.cpp��
///////////////////////////////////////////////////////////////////////////
public:
	/* --------------------------------------------------------------------
	��־���ã�
	<!--�����齨-->
	<Table name="t340_team_create" log_no="34">
		<item type="date"     head_name=""></item>
		<item type="object"   head_name="team"></item>
		<item type="player"   head_name="sponsor"></item>
		<item type="player"   head_name="participator"></item>
	</Table>
	*/
	//! ��¼�����齨��־
	//! �ӿڶ����ڣ�WSTeam.cpp
	bool LogT340_team_create(WSTeam *pTeam, CPlayer *pSponsor, CPlayer *pParticipator);


	/* --------------------------------------------------------------------
	��־���ã�
	<!--�����ɢ-->
	<Table name="t370_team_disband" log_no="37">
		<item type="date"     head_name=""></item>
		<item type="object"   head_name="team"></item>
	</Table>
	*/
	//! ��¼�����ɢ��־
	//! �ӿڶ����ڣ�WSTeam.cpp
	bool LogT370_team_disband(WSTeam *pTeam);

	/* --------------------------------------------------------------------
	<!--��ᴴ��/��ɢ��־-->
		<Table name="t150_faction_log" log_no="150" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="player"   head_name=""></item>
		<item type="faction"  head_name=""></item>
		<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--�������ͣ�0��������1��ɢ��-->
	</Table>
	*/
	bool LogT150_faction_log(LPCSTR pOprName, const CGUID &pOprGUID, CFaction *pFaction, LONG lAct);

	
	/* --------------------------------------------------------------------
	<!--�����б����/�˳���־-->
	<Table name="t160_faction_join_leave_log" log_no="160" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="player"   head_name=""></item>
		<item type="faction"  head_name=""></item>
		<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--�������ͣ�0�����룻1��ɾ����-->
	</Table>
	*/
	bool LogT160_faction_join_leave_log(const char *pOpterName, const CGUID *pOpterGuid, CFaction *pFaction, LONG lAct);


	/* --------------------------------------------------------------------
	<!--��Ա����/�뿪��־-->
	<Table name="t170_faction_agree_kick_log" log_no="170" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="player"   head_name=""></item>
		<item type="player"   head_name="aim"></item>
		<item type="faction"  head_name=""></item>
		<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--�������ͣ�0�����룻1���뿪��-->
	</Table>
	*/
	bool LogT170_faction_agree_kick_log(const char *pOpterName, const CGUID *pOpterGuid, const char *pAimName, const CGUID *pAimGuid, CFaction *pFaction, LONG lAct);


	/* --------------------------------------------------------------------
	<!--ͷ�α��-->
	<Table name="t230_faction_title_log" log_no="230" log_pos="1">
	<item type="client_head"     head_name=""></item>
	<item type="faction"  head_name=""></item>
	<item type="player"   head_name=""></item>
	<item type="alone"    Item_name="before_title"    data_type="int32"></item><!--֮ǰͷ��-->
	<item type="alone"    Item_name="curr_title"      data_type="int32"></item><!--��ǰͷ��-->
	</Table>
	*/


	/* --------------------------------------------------------------------
	<!--Ȩ�����-->
	<Table name="t240_faction_purview_log" log_no="240" log_pos="1">
	<item type="client_head"     head_name=""></item>
	<item type="faction"  head_name=""></item>
	<item type="player"   head_name=""></item>
	<item type="alone"    Item_name="before_purview"  data_type="int32"></item><!--֮ǰȨ��-->
	<item type="alone"    Item_name="curr_purview"    data_type="int32"></item><!--��ǰȨ��-->
	</Table>
	*/


	/* --------------------------------------------------------------------
	<!--�������-->
	<Table name="t250_faction_master_log" log_no="250" log_pos="1">
	<item type="client_head"     head_name=""></item>
	<item type="faction"  head_name=""></item>
	<item type="player"   head_name="old"></item>
	<item type="player"   head_name="new"></item>
	</Table>
	*/
	bool LogT250_faction_master_log(CFaction *pFaction, tagFacMemInfo *pOld, tagFacMemInfo *pNew);
			
	/*
	<!--================================================================-->
	<!--��ᾭ��仯-->
	<Table name="t260_faction_experience_log" log_no="260" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="faction"  head_name=""></item>
		<item type="alone"    Item_name="before_exp"      data_type="int32"></item><!--֮ǰ����-->
		<item type="alone"    Item_name="curr_exp"        data_type="int32"></item><!--��ǰ����-->
	</Table>
	*/
	bool LogT260_faction_experience_log(CFaction *pFaction, long lBeforeExp);
	
	/*
	<!--================================================================-->
	<!--���ȼ��仯-->
	<Table name="t270_faction_level_log" log_no="270" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="faction"  head_name=""></item>
		<item type="alone"    Item_name="before_level"    data_type="int32"></item><!--֮ǰ�ȼ�-->
		<item type="alone"    Item_name="curr_level"      data_type="int32"></item><!--��ǰ�ȼ�-->
		<item type="alone"    Item_name="level_type"      data_type="int32"></item><!--�ȼ����� 0=���� 1=���� 2=�ֿ� 3=���� 4���� 5=����-->
	</Table>
	*/
	bool LogT270_faction_level_log(CFaction *pFaction, long lCurrLevel, long lLvlType);

	/* --------------------------------------------------------------------
	*/

	/* --------------------------------------------------------------------
	<!--��ϵ�˲�����־-->
	<Table name="t450_linkman_opt_log" log_no="50" log_pos="1">
	<item type="client_head"     head_name=""></item>
	<item type="player"   head_name="opter"></item><!--������-->
	<item type="player"   head_name="aim"></item><!--����Ŀ��-->
	<item type="alone"    Item_name="opt_type"      data_type="int32"></item><!--�������ͣ�1=��ӡ�2=ɾ��-->
	<item type="alone"    Item_name="group_id"      data_type="int32"></item><!--����������ϵ�����ID-->
	</Table>
	*/
	bool logT450_linkman_opt_log(CPlayer *pOpter, const CGUID &AimGuid, const char *pAimName, long lOptType, long lGroupID);


	/* --------------------------------------------------------------------
	<!--�ͻ�����������־-->
	<Table name="t550_client_in_game_log" log_no="550" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="alone"    Item_name="cdkey"         data_type="str128"></item><!--IP-->
		<item type="alone"    Item_name="ip"            data_type="int32"></item><!--CDKEY-->
		<item type="player"   head_name=""></item>
		<item type="pos"      head_name=""></item>
		<item type="alone"    Item_name="region_guid"   data_type="int32"></item><!--level-->
		<item type="alone"    Item_name="level"         data_type="int32"></item><!--level-->
		<item type="alone"    Item_name="gold"          data_type="int32"></item><!--gold-->
		<item type="alone"    Item_name="flag"          data_type="int32"></item><!--silver-->
		<item type="alone"    Item_name="flag"          data_type="int32"></item><!--�������ͣ�1=���ߡ�2=����-->
		<item type="alone"    Item_name="online_time"   data_type="int32"></item><!--�˴β������߷��������������ͣ��˴�Ϊ0���������ͣ��˴���ֵ-->
	</Table>
	*/
	bool logT550_client_in_game_log(CPlayer *pPlayer, LONG lState);

private:

	//!							�õ���ǰʱ���ַ�����00-00-00 00:00:00��ʽ��
	void						GetCurrTimeString(string &strTime);


private:

	//!							��־�ͻ��˶���
	LogClient					*m_pLogClient;

};