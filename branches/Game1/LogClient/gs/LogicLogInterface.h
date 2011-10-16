

#pragma once


class LogClient;
class GSTeam;
class CRegion;
class CPlayer;
class CGoods;
class CMonster;
class CMoveShape;
class CServerRegion;

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
	<!--��Ա����-->
	<Table name="t350_team_join" log_no="35">
		<item type="date"     head_name=""></item>
		<item type="pos"      head_name=""></item>
		<item type="object"   head_name="team"></item>
		<item type="player"   head_name=""></item>
	</Table>
	*/
	//! ��¼��Ա������־
	bool LogT350_team_join(GSTeam *pTeam, CPlayer *pPlayer);


	/* --------------------------------------------------------------------
	��־���ã�
	<!--��Ա�뿪-->
		<Table name="t360_team_leave" log_no="36">
		<item type="date"     head_name=""></item>
		<item type="pos"      head_name=""></item>
		<item type="object"   head_name="team"></item>
		<item type="player"   head_name=""></item>
	</Table>
	*/
	//! ��¼��Ա�뿪��־
	bool LogT360_team_leave(GSTeam *pTeam, CPlayer *pPlayer);

	/* --------------------------------------------------------------------
	<!--��Ʒ�ֽ���־-->
		<Table name="t380_goods_disassemble_log" log_no="38" log_pos="1">
			<item type="client_head"     head_name=""></item>
			<item type="player"   head_name=""></item>
			<item type="goods"    head_name=""></item><!--��Ʒ-->
			<item type="alone"    Item_name="opt_id"            data_type="guid"></item><!--�ֽ������ID��-->
			<item type="alone"    Item_name="opt_type"          data_type="int32"></item><!--0=ɾ���ģ�1=��ӵ�-->
			<item type="alone"    Item_name="change_type"       data_type="int32"></item><!--1=�������Ʒ��2=�޸�����-->
		</Table>
	*/
	bool logT380_goods_disassemble_log(CPlayer *pPlayer, CGoods *pGoods, long lAddType, const CGUID& guid, BOOL bIsAdd);


	/* --------------------------------------------------------------------
	<!--��Ʒ�ϳ���־-->
		<Table name="t390_goods_synthesize_log" log_no="39" log_pos="1">
			<item type="client_head"     head_name=""></item>
			<item type="player"   head_name=""></item>
			<item type="goods"    head_name=""></item><!--��Ʒ-->
			<item type="alone"    Item_name="opt_id"            data_type="guid"></item><!--�ֽ������ID��-->
			<item type="alone"    Item_name="opt_type"          data_type="int32"></item><!--0=ɾ���ģ�1=��ӵ�-->
			<item type="alone"    Item_name="change_type"       data_type="int32"></item><!--1=�������Ʒ��2=�޸�����-->
		</Table>
	*/
	bool logT390_goods_synthesize_log(CPlayer *pPlayer, CGoods *pGoods, long lAddType, const CGUID& guid, BOOL bIsAdd);


	/* --------------------------------------------------------------------
	<!--��Ʒ������־-->
		<Table name="t400_goods_upgrade_log" log_no="40" log_pos="1">
			<item type="client_head"     head_name=""></item>
			<item type="player"   head_name=""></item>
			<item type="alone"    Item_name="opt_id"            data_type="guid"></item>  <!--������ID��-->
			<item type="goods"    head_name=""></item><!--����������Ʒ-->
			<item type="alone"    Item_name="old_level"      data_type="int32"></item><!--����ǰ�ȼ�-->
			<item type="alone"    Item_name="new_level"      data_type="int32"></item><!--������ȼ�-->
		</Table>
	*/
	bool logT400_goods_upgrade_log(CPlayer *pPlayer, const CGUID& guid, CGoods *pGoods, long lOldLevel, long lNewLevel);


	/* --------------------------------------------------------------------
	<!--��Ʒħ����־-->
		<Table name="t410_goods_plugevil_log" log_no="41" log_pos="1">
			<item type="client_head"     head_name=""></item>
			<item type="player"   head_name=""></item>
			<item type="alone"    Item_name="opt_id"            data_type="guid"></item>  <!--������ID��-->
			<item type="goods"    head_name=""></item><!--����������Ʒ-->
			<item type="alone"    Item_name="item_index"     data_type="int32"></item> <!--�޸ĵ�ħ����λ-->
			<item type="alone"    Item_name="old_level"      data_type="int32"></item><!--����ǰ�ȼ�-->
			<item type="alone"    Item_name="new_level"      data_type="int32"></item><!--������ȼ�-->
		</Table>
	*/
	bool logT410_goods_plugevil_log(CPlayer *pPlayer, const CGUID& guid, CGoods *pGoods, long lItemIndex, long lOldLevel, long lNewLevel);

	/* --------------------------------------------------------------------
	<!--ħ�����ϻ����־-->
		<Table name="t420_evil_stuff_synthesize_log" log_no="42" log_pos="1">
			<item type="client_head"     head_name=""></item>
			<item type="player"   head_name=""></item>
			<item type="goods"    head_name=""></item><!--���ϳɵ���Ʒ-->
			<item type="alone"    Item_name="add_type"      data_type="int32"></item><!--1=�������Ʒ��2=�޸�����-->
			<item type="alone"    Item_name="get_num"      data_type="int32"></item><!--1=�������Ʒ��2=�޸�����-->
		</Table>
	*/
	bool logT420_evil_stuff_synthesize_log(CPlayer *pPlayer, CGoods *pGoods, long lAddType, long lGetNum);

	/* --------------------------------------------------------------------
	<!--��Ʒ�󶨺Ͷ�����־-->
	<Table name="t421_goods_bind_and_frost" log_no="421" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="player"   head_name=""></item>
		<item type="goods"    head_name=""></item><!--����������Ʒ-->
		<item type="goods"    head_name=""></item><!--���ĵ���Ʒ-->
		<item type="alone"    Item_name="opt_type"          data_type="int32"></item><!--0=�󶨣�1=����󶨣� 2=���ᣬ3=�������-->
	</Table>
	*/
	bool logT421_goods_bind_and_frost(CPlayer *pPlayer, CGoods *pAim, CGoods *pStuff, long lType);

	/*
	<!--�Ǹ�ְҵ������־-->
	<Table name="t422_goods_tailor_system" log_no="422" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="player"   head_name=""></item>
		<item type="goods"    head_name="aim"></item><!--����������Ʒ-->
	</Table>
	*/
	bool logT422_goods_tailor_system(CPlayer *pPlayer, CGoods *pGoods);

	/* --------------------------------------------------------------------
	<!--������־-->
	<Table name="t430_chat_log" log_no="430" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="player"   head_name="sender"></item>
		<item type="alone"	  Item_name="_accepter_name" data_type="str128"></item>
		<item type="alone"	  Item_name="_chat_text" 	data_type="str512"></item>
		<item type="alone"    Item_name="region_id" data_type="int32"></item>
		<item type="alone"    Item_name="msg_type"      data_type="int32"></item>
		<!--
		��Ϣ����
		# 1. ��Ļ
		#	2. ����
		#	3. ���
		#	4. ����
		#	5. ���
		#	6. ����
		#	7. GMָ��
		-->
	</Table>*/
	bool logT430_chat_log(CPlayer* pPlayer,const char* pAccepterName,const char* pChatText,long lType);

	/* <!--GM������־-->
	  <Table name="t431_GM_log" log_no="431" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="player"   head_name="GM"></item>
		<item type="alone"    Item_name="region_id" data_type="int32"></item>
		<item type="alone"	  Item_name="cmd_line" data_type="str128"></item>
	  </Table>*/

	bool logT431_GM_log(CPlayer* pPlayer,const char* pCommandLine);

	/*
	<!--================================================================-->
	<!--�ֿ������־-->
	<Table name="t060_goods_log_depository" log_no="60" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="player"   head_name=""></item>
		<item type="goods"    head_name=""></item>
		<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--���ͣ�0���棻1��ȡ��-->
		<item type="alone"    Item_name="goods_num"       data_type="int32"></item><!--��Ʒ����-->
	</Table>
	*/
	bool logT060_goods_log_depository(CPlayer *pPlayer,DWORD dwGoodsId,const CGUID& goodsGuid,const char* strGoodsName,DWORD dwGoodsNum, long lActType);


	/*
	<!--================================================================-->
	<!--���в�����־-->
	<Table name="t070_goods_log_bank" log_no="70" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="player"   head_name=""></item>
		<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--���ͣ�0���棻1��ȡ��-->
		<item type="alone"    Item_name="number"          data_type="int32"></item><!--����-->
	</Table>
	*/
	bool logT070_goods_log_bank(CPlayer *pPlayer, long lActType, long lNum);

	/*
	<!--��һ�þ���-->
	<Table name="t080_player_getexp_log" log_no="80" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="player"   head_name=""></item>
		<item type="pos"      head_name=""></item>
		<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--���Ŀ�����ͣ�0�����1��NPC��3�����帣����-->
		<item type="alone"    Item_name="delta_exp"       data_type="int32"></item><!--�仯�ľ���ֵ-->
		<item type="alone"    Item_name="delta_expMul"    data_type="int32"></item><!--�仯�ľ��鱶��-->
		<item type="alone"    Item_name="old_exp"         data_type="int32"></item><!--�仯ǰ�ľ���ֵ-->
		<item type="alone"    Item_name="old_expMul"	  data_type="int32"></item><!--�仯ǰ�ľ��鱶��-->
		<item type="alone"    Item_name="new_exp"         data_type="int32"></item><!--�仯��ľ���ֵ-->
		<item type="alone"    Item_name="new_expMul"	  data_type="int32"></item><!--�仯��ľ��鱶��-->
		<item type="alone"    Item_name="src_depict"      data_type="str128"></item><!--������Դ��˵��-->
	</Table>
	*/
	bool logT080_player_getexp_log(CPlayer *pPlayer, DWORD dwType, LONG lChangeExp, LONG lChangeExpMul, LONG lOldExp, LONG lOldExpMul, LONG lNewExp, LONG lNewExpMul, const char *pSrcDepict);


	/*<!--��һ�þ���-->
	<Table name="t081_player_GetOccuExp_log" log_no="81" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="player"   head_name=""></item>
		<item type="pos"      head_name=""></item>
		<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--���Ŀ�����ͣ�0�����1��NPC��-->
		<item type="alone"    Item_name="cur_occu"        data_type="int32"></item><!--��ǰְҵ-->
		<item type="alone"    Item_name="delta_exp"       data_type="int32"></item><!--�仯�ľ���ֵ-->
		<item type="alone"    Item_name="old_exp"         data_type="int32"></item><!--�仯ǰ�ľ���ֵ-->
		<item type="alone"    Item_name="new_exp"         data_type="int32"></item><!--�仯��ľ���ֵ-->
		<item type="alone"    Item_name="src_depict"      data_type="str128"></item><!--������Դ��˵��-->
	</Table>*/
	bool logT081_player_getoccuexp_log(CPlayer *pPlayer, DWORD dwType,LONG byOccu, LONG lChangeExp, LONG lOldExp, LONG lNewExp, const char *pSrcDepict);


	/*
	<!--���������־-->
	<Table name="t100_player_level_log" log_no="100" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="pos"      head_name=""></item>
		<item type="player"   head_name=""></item>
		<item type="alone"    Item_name="curr_exp"        data_type="int32"></item><!--����ֵ����-->
		<item type="alone"    Item_name="last_level"      data_type="int32"></item><!--֮ǰ����-->
		<item type="alone"    Item_name="curr_level"      data_type="int32"></item><!--�����󼶱�-->
		<item type="alone"    Item_name="src_depict"      data_type="str128"></item><!--������˵��-->
	</Table>
	*/
	bool logT100_player_level_log(CPlayer *pPlayer, LONG lLastLevel, const char *pSrcDepict);


	/*
	<!--���ְҵ�ȼ�������־-->
	<Table name="t101_player_Occulevel_log" log_no="101" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="pos"      head_name=""></item>
		<item type="player"   head_name=""></item>
		<item type="alone"    Item_name="curr_occu"       data_type="int32"></item><!--��ǰְҵ-->
		<item type="alone"    Item_name="curr_exp"        data_type="int32"></item><!--����ֵ����-->
		<item type="alone"    Item_name="last_level"      data_type="int32"></item><!--֮ǰ����-->
		<item type="alone"    Item_name="curr_level"      data_type="int32"></item><!--�����󼶱�-->
		<item type="alone"    Item_name="src_depict"      data_type="str128"></item><!--������˵��-->
	</Table>
	*/
	bool logT101_player_Occulevel_log(CPlayer *pPlayer, LONG lLastLevel, const char *pSrcDepict);


	/*
	<!--��Ʒ������־-->
		<Table name="t010_goods_log_trade" log_no="10" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="pos"      head_name=""></item>
		<item type="player"   head_name="sponsor"></item>
		<item type="player"   head_name="participator"></item>
		<item type="goods"    head_name=""></item>
		<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--�������ͣ�0����ͨ��1�����̵꣩-->
		<item type="alone"    Item_name="goods_price"     data_type="int32"></item><!--��Ʒ�۸�-->
		<item type="alone"    Item_name="goods_num"       data_type="int32"></item><!--��Ʒ����-->
	</Table>
	*/
	bool logT010_goods_log_trade(CPlayer* pPlayer1,CPlayer* pPlayer2,DWORD dwGoodsId,const CGUID& goodsGuid,const char* strGoodsName,long lActType,long lPrice,long lNum,DWORD dwMoneyType);


	/*
	<!--================================================================-->
	<!--���PK��־-->
	<Table name="t130_player_attack_log" log_no="130" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="pos"      head_name=""></item>
		<item type="player"   head_name="sponsor"></item>
		<item type="player"   head_name="participator"></item>
	</Table>
	*/
	bool logT130_player_attack_log(CPlayer *pSponsor, CPlayer *pParticipator);


	/*
	<!--================================================================-->
	<!--���������־-->
	<Table name="t140_player_pkdie_log" log_no="140" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="pos"      head_name=""></item>
		<item type="player"   head_name="decedent"></item>
		<item type="objcet"   head_name="attacker"></item>
	</Table>
	*/
	bool logT140_player_pkdie_log(CPlayer *pPlayer, CMoveShape *pMoveShape);

	/*
	<Table name="t020_goods_log_pickup" log_no="20" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="pos"      head_name=""></item>
		<item type="player"   head_name=""></item>
		<item type="goods"    head_name=""></item>
		<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--�������ͣ�0��ʰȡ��1��������-->
		<item type="alone"    Item_name="goods_num"       data_type="int32"></item><!--��Ʒ����-->
	</Table>
	*/
	bool logT020_goods_log_pickup(CPlayer* pPlayer,DWORD dwGoodsId,const CGUID& goodsGuid,const char* strGoodsName,long lActType,long lNum);


	/*
	<!--��Ʒ��Ƕ��־-->
	<Table name="t021_goods_log_enchase" log_no="21" log_pos="1">
	<item type="client_head"     head_name=""></item>
	<item type="pos"      head_name=""></item>
	<item type="player"   head_name=""></item>
	<item type="goods"    head_name=""></item>
	<item type="alone"    Item_name="act_type"        data_type="int32"></item>
	<!--�������ͣ�0���⣻1����-->
	<item type="alone"    Item_name="hole_pos"       data_type="int32"></item>
	<!--��λ��-->
	<item type="alone"    Item_name="card_id"       data_type="int32"></item>
	<!--��ID-->
	<item type="alone"    Item_name="card_name"       data_type="str128"></item>
	<!--������-->
	</Table>
	*/
	bool logT021_goods_log_enchase(CPlayer* pPlayer,DWORD dwGoodsId,const CGUID& goodsGuid,const char* strGoodsName,long lActType,DWORD dwHolePos,
		DWORD dwCardId,const char* strCardName,DWORD dwReserveCard);


	/*
	<!--��Ʒ������־-->
	<Table name="t022_goods_log_consume" log_no="22" log_pos="1">
	<item type="client_head"     head_name=""></item>
	<item type="pos"      head_name=""></item>
	<item type="player"   head_name=""></item>
	<item type="goods"    head_name=""></item>    
	<item type="alone"    Item_name="num"  data_type="int32"></item>
	<!--����-->
	<item type="alone"    Item_name="container_id"       data_type="int32"></item>
	<!--container id-->
	<item type="alone"    Item_name="container_pos"       data_type="int32"></item>
	<!--container pos-->
	</Table>
	*/
	bool logT022_goods_log_consume(CPlayer* pPlayer,DWORD dwGoodsId,const CGUID& goodsGuid,const char* strGoodsName,DWORD dwNum,DWORD dwUsedNum,DWORD dwPlace,DWORD dwPos);

	/*
	<!--��Ʒ������־-->
		<Table name="t023_goods_log_repair" log_no="23" log_pos="1">
		<item type="client_head"     head_name=""></item>    
		<item type="player"   head_name=""></item>
		<item type="goods"    head_name=""></item>
		<item type="alone" Item_name="act_type"        data_type="int32"></item>
		</Table>*/
	bool logT023_goods_log_repair(CPlayer* pPlayer,DWORD dwGoodsId,const CGUID& goodsGuid,int nActType);

	/*
	<!--��Ʒ�ƶ���־-->
		<Table name="t024_goods_log_mov" log_no="24" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="player"   head_name=""></item>
		<item type="goods"    head_name=""></item>
		<item type="alone" Item_name="act_type"        data_type="int32"></item>
		<item type="alone" Item_name="mov_ret"        data_type="int32"></item>
		<item type="alone" Item_name="src_container"   data_type="int32"></item>
		<item type="alone" Item_name="src_pos"   data_type="int32"></item>
		<item type="alone" Item_name="dst_container"   data_type="int32"></item>
		<item type="alone" Item_name="dst_pos"   data_type="int32"></item>
		</Table>*/
	bool logT024_goods_log_mov(CPlayer* pPlayer,DWORD dwGoodsId,const CGUID& goodsGuid,int nActType,int nMovRet,int nSrc_container,int nSrc_pos,int nDst_container,int nDst_pos);

	/*
	<!--��Ʒunlock��־-->
	<Table name="t025_goods_log_unlock" log_no="25" log_pos="1">
	<item type="client_head"     head_name=""></item>
	<item type="player"   head_name=""></item>    
	<item type="alone" Item_name="ewt"        data_type="int32"></item>
	<item type="alone" Item_name="session_guid"        data_type="guid"></item>
	<item type="alone" Item_name="goods_guid"   data_type="guid"></item>
	<item type="alone" Item_name="pos"   data_type="int32"></item>
	<item type="alone" Item_name="plug_id"   data_type="int32"></item>    
	</Table>
	*/
	bool logT025_goods_log_unlock(CPlayer* pPlayer,int nEwt,int nPos,int nPlugId);
	

	/*
	<!--������Ʒ��ȡ��־-->
	<Table name="t026_goods_log_bouns" log_no="26" log_pos="1">
	<item type="client_head"     head_name=""></item>
	<item type="player"   head_name=""></item>   
	<item type="goods"    head_name=""></item> 
	<item type="alone"    Item_name="goods_num"       data_type="int32"></item><!--��Ʒ����-->
	<item type="alone"    Item_name="bouns_id"       data_type="int32"></item><!--�������-->
	</Table>
	*/
	bool logT026_goods_log_bouns(CPlayer* pPlayer, DWORD dwGoodsId, const CGUID& goodsGuid, const char* strGoodsName, int nGoodsNum, int nBounsID);

	/*
	<!--��NPC����Ʒ������־-->
	<Table name="t030_goods_log_npc" log_no="30" log_pos="1">
	<item type="client_head"     head_name=""></item>
	<item type="pos"      head_name=""></item>
	<item type="player"   head_name=""></item>
	<item type="goods"    head_name=""></item>
	<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--�������ͣ�0�������1���������-->
	<item type="alone"    Item_name="goods_price"     data_type="int32"></item><!--��Ʒ�۸�-->
	<item type="alone"    Item_name="goods_num"       data_type="int32"></item><!--��Ʒ����-->
	</Table>
	*/

	/*
	<Table name="t027_goods_log_pickup" log_no="20" log_pos="1">
	<item type="client_head"     head_name=""></item>
	<item type="pos"      head_name=""></item>
	<item type="player"   head_name=""></item>
	<item type="goods"    head_name=""></item>
	<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--�������ͣ�0��ʰȡ��1��������-->
	<item type="alone"    Item_name="goods_num"       data_type="int32"></item><!--��Ʒ����-->
	</Table>
	*/
	bool logT027_goods_log_pickup(CPlayer* pPlayer,DWORD dwGoodsId,const CGUID& goodsGuid,const char* strGoodsName,long lActType,long lNum);

	bool logT030_goods_log_npc(CPlayer* pPlayer,DWORD dwGoodsId,const CGUID& goodsGuid,const char* strGoodsName,long lActType,long lPrice,long lNum);

	/*<!--��NPCͨ��λ�澫��������־-->
	<Table name="t031_goods_wm_npc" log_no="31" log_pos="1">	
	<item type="client_head"     head_name=""></item>
	<item type="player"   head_name=""></item>
	<item type="goods"    head_name=""></item>  
	<item type="alone"	  Item_name="Goods_Num"		  data_type="int32"></item><!--��ȡ��Ʒ������-->
	<item type="alone"	  Item_name="wm_Num"		  data_type="int32"></item><!--λ������-->
	</Table>*/
	bool logT031_goods_wm_npc(CPlayer* pPlayer,DWORD dwGoodsID,const CGUID& guid,LONG lGoodsNum,LONG lWmNum);
	
	/*
	<!--��Ʒ��ʧ��־-->
	<Table name="t040_goods_log_disappear" log_no="40" log_pos="1">
	<item type="client_head"     head_name=""></item>
	<item type="player"   head_name=""></item>
	<item type="goods"    head_name=""></item>
	<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--���ͣ�0���𻵣�1��������ʧ��-->
	</Table>
	*/
	bool logT040_goods_log_disappear(CPlayer* pPlayer,CGoods* pGoods,long lActType);

	/*
	<!--��Ʒ������־-->
	<Table name="t050_goods_log_fall" log_no="50" log_pos="1">
	<item type="client_head"     head_name=""></item>
	<item type="pos"      head_name=""></item>
	<item type="player"   head_name=""></item>
	<item type="goods"    head_name=""></item>
	</Table>
	*/
	bool logT050_goods_log_fall(CPlayer* pPlayer,CGoods* pGoods);

	/*
	<!--================================================================-->
	<!--�л�������־-->
		<Table name="t440_change_map_log" log_no="440" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="player"   head_name=""></item>
		<item type="alone"    Item_name="src_region_id"      data_type="int32"></item>
		<item type="alone"    Item_name="aim_region_id"      data_type="int32"></item>
		<item type="alone"    Item_name="switch_type"      data_type="int32"></item>
		<!--
		�л�����
		# 1. ������
		#	2. ��������
		#	3. ����������
		-->
	</Table>
	*/
	bool logT440_change_map_log(CPlayer *pPlayer, LONG lSrcRegionID, LONG lAimRegionID, LONG lType);

	//<!--�������-->
	//	<Table name="t460_quest_opt_log" log_no="460" log_pos="1">
	//	<item type="client_head"     head_name=""></item>
	//	<item type="player"   head_name=""></item>
	//	<!--����Ŀ��-->
	//	<item type="alone"    Item_name="quest_id"      data_type="int32"></item><!--��������id-->
	//	<item type="alone"    Item_name="opt_type"      data_type="int32"></item><!-- �������ͣ�1=��ӡ�2=��� 3=ɾ��--> 
	//	</Table>


	bool logT460_quest_log(CPlayer* pPlayer,DWORD dwQuestID,DWORD dwQuestOperType);


	/*
	<!--�ͽ�����ˢ�²���-->
	<Table name="t461_mercquest_log" log_no="461" log_pos="1">
	<item type="client_head"     head_name=""></item>
	<item type="player"   head_name=""></item>
	<item type="pos"      head_name=""></item>
	<!--����Ŀ��-->
	<item type="alone"    Item_name="usegoods"       data_type="int32"></item><!--�Ƿ�ʹ����Ʒ��1�ǣ�0��-->
	<item type="alone"    Item_name="quest_id1"      data_type="int32"></item><!--ˢ��������1-->
	<item type="alone"    Item_name="quest_id2"      data_type="int32"></item><!--ˢ��������2-->
	<item type="alone"    Item_name="quest_id3"      data_type="int32"></item><!--ˢ��������3-->
	<item type="alone"    Item_name="quest_id4"      data_type="int32"></item><!--ˢ��������4-->
	</Table>
	*/
	bool logT461_mercquest_log(CPlayer* pPlayer,DWORD bUsegoods,DWORD quest1,DWORD quest2,DWORD quest3,DWORD quest4);


	//<!--AI�쳣��¼-->
	//<Table name="t470_AI_opt_log" log_no="470" log_pos="1">
	//<item type="client_head"     head_name=""></item>
	//<item type="object"   head_name=""></item>
	//<!--����Ŀ��-->
	//<item type="alone"    Item_name="opt_type"      data_type="int32"></item><!--�������ͣ��쳣����-->
	//</Table>
	
	bool logT470_AIExc_log(CMonster* pMonster,DWORD dwExcType);

	/*
	<!--��ȯ������־-->
	<Table name="t501_Ticket_consume_log" log_no="501" log_pos="1">
	<item type="client_head"     head_name=""></item>    
	<item type="player"   head_name=""></item>
	<item type="goods"    head_name=""></item>    
	<item type="alone"    Item_name="goods_price"     data_type="int32"></item><!--��Ʒ�۸�-->
	<item type="alone"    Item_name="goods_num"       data_type="int32"></item><!--��Ʒ����-->
	</Table>
	*/
	bool logT501_Ticket_consume_log(CPlayer* pPlayer,DWORD dwGoodsId,const CGUID& goodsGuid,const char* strGoodsName,DWORD dwPrice,DWORD dwGoodsNum,DWORD dwMoney,DWORD dwTicket,DWORD dwActType);

	bool logT502_Ticket_gain_log(CPlayer* pPlayer,int nGainNum);

	/*<!--��һ�ȡ��־-->
	  <Table name="t503_Gold_gain_log" log_no="503" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="player"   head_name=""></item>    
		<item type="alone"    Item_name="old_num"       data_type="int32"></item>
		<item type="alone"    Item_name="new_num"       data_type="int32"></item>
		<item type="alone"    Item_name="flag"          data_type="int32"></item>
	  </Table>*/
	bool logT503_Gold_gain_log(CPlayer* pPlayer,int nOldNum,int nNewNum,int nFlag);

	bool logT504_weimian_log(CPlayer* pPlayer,int nNum,int nActType,int nFlag);


	/*<!--ͨ�����������ȡ��ȯ��ȡ��־-->
		<Table name="t513_Ticket_gain_log" log_no="513" log_pos="1">
		<item type="client_head"     head_name=""></item>
		<item type="player"   head_name=""></item>
		<item type="alone"    Item_name="gain_num"       data_type="int32"></item>
		<item type="alone"    Item_name="return_ratio"   data_type="float"></item>
		<item type="alone"    Item_name="ticket_num"     data_type="int32"></item>
	</Table>*/
	bool logT513_Ticket_gain_log(CPlayer* pPlayer,int nGainNum,float fRatio);





	bool logT521_snda_url_send_log(CPlayer* pPlayer,const char* szOrderId,int nTotalAmount,
		const char* szGoodsInfo);

	bool logT522_snda_ticket_consume_log(CPlayer* pPlayer,const char* szOrderId,const char* szSendTime,
		const char* szGoodsInfo,const char* payErrorMsg,int retcode);

	/*
	<!--��ȡ�ʼ���־-->
		<Table name="t600_receive_mail_log" log_no="600" log_pos="1">
		<item type="client_head"     head_name=""></item>    
		<item type="player"   head_name=""></item>
		<item type="alone"    Item_name="mail_guid"            data_type="guid"></item><!--�ʼ�guid-->
		<item type="alone"	  Item_name="writer"			   data_type="str128"></item><!--������-->
		<item type="alone"	  Item_name="type"			   	   data_type="int32"></item><!--�ʼ�����-->
		<item type="alone"	  Item_name="gold"			   	   data_type="int32"></item><!--��Ǯ--> 
		<item type="alone"	  Item_name="goodsnum"			   data_type="int32"></item><!--��Ʒ����-->
		<item type="alone"	  Item_name="incgoodsnum"		   data_type="int32"></item><!--��ֵ��Ʒ����-->    
		<item type="alone"    Item_name="flag"          	   data_type="int32"></item><!--�������ͣ�1=������ȡ��2=���߽���-->
		</Table>
	*/
	bool logT600_Receive_mail_log(CPlayer *pPlayer,const CGUID &MailGuid,const char *strWriter,int nType,long lGold,long lGoodsNum,long lIncGoodsNum,int nFlag);
	/*
	<Table name="t601_send_mail_log" log_no="601" log_pos="1">
	<item type="client_head"     head_name=""></item>    
	<item type="alone"    Item_name="mail_guid"            data_type="guid"></item><!--�ʼ�guid-->
	<item type="alone"	  Item_name="receiver"			   data_type="str128"></item><!--������-->
	<item type="alone"	  Item_name="writer"			   data_type="str128"></item><!--������-->
	<item type="alone"	  Item_name="type"			   	   data_type="int32"></item><!--�ʼ�����-->
	<item type="alone"	  Item_name="gold"			   	   data_type="int32"></item><!--��Ǯ--> 
	<item type="alone"	  Item_name="goodsnum"			   data_type="int32"></item><!--��Ʒ����-->
	<item type="alone"	  Item_name="incgoodsnum"		   data_type="int32"></item><!--��ֵ��Ʒ����-->    	
	</Table>
	*/
	bool logT601_Send_mail_log(const CGUID &MailGuid,const char *strReceiver,const char *strWriter,int nType,long lGold,long lGoodsNum,long lIncGoodsNum,const char *strGoodsInfo);

	/*<!--���ʼ���ȡ��Ʒ��־-->
		<Table name="t602_getgoods_mail_log" log_no="602" log_pos="1">
		<item type="client_head"     head_name=""></item>    
		<item type="alone"    Item_name="mail_guid"            data_type="guid"></item><!--�ʼ�guid-->
		<item type="alone"	  Item_name="receiver"			   data_type="str128"></item><!--������-->
		<item type="alone"	  Item_name="writer"			   data_type="str128"></item><!--������-->
		<item type="goods"    head_name=""></item>
		<item type="alone"	  Item_name="type"			   	   data_type="int32"></item><!--�ʼ�����-->
		<item type="alone"	  Item_name="gold"			   	   data_type="int32"></item><!--��Ǯ--> 
		<item type="alone"	  Item_name="goodsnum"			   data_type="int32"></item><!--��Ʒ����-->
		<item type="alone"	  Item_name="incgoodsnum"		   data_type="int32"></item><!--��ֵ��Ʒ����-->    
		</Table>
		*/
	bool logT602_GetGoods_mail_log(const CGUID &MailGuid,const char *strReceiver,const char *strWriter,DWORD dwGoodsId,const CGUID& goodsGuid,const char* strGoodsName,int nType,long lGold,long lGoodsNum,long lIncGoodsNum);
	/*<!--���ʼ���ȡ�����־-->
		<Table name="t603_getgold_mail_log" log_no="603" log_pos="1">
		<item type="client_head"     head_name=""></item>    
		<item type="alone"    Item_name="mail_guid"            data_type="guid"></item><!--�ʼ�guid-->
		<item type="alone"	  Item_name="receiver"			   data_type="str128"></item><!--������-->
		<item type="alone"	  Item_name="writer"			   data_type="str128"></item><!--������-->    
		<item type="alone"	  Item_name="type"			   	   data_type="int32"></item><!--�ʼ�����-->
		<item type="alone"	  Item_name="gold"			   	   data_type="int32"></item><!--��Ǯ--> 
		<item type="alone"	  Item_name="goodsnum"			   data_type="int32"></item><!--��Ʒ����-->
		<item type="alone"	  Item_name="incgoodsnum"		   data_type="int32"></item><!--��ֵ��Ʒ����-->    
		</Table>
	*/
	bool logT603_GetGold_mail_log(const CGUID &MailGuid,const char *strReceiver,const char *strWriter,int nType,long lGold,long lGoodsNum,long lIncGoodsNum);
	/*
	<!--��ȡ�ʼ���־-->
		<Table name="t604_read_mail_log" log_no="604" log_pos="1">
		<item type="client_head"     head_name=""></item>    
		<item type="player"   head_name=""></item>
		<item type="alone"    Item_name="mail_guid"            data_type="guid"></item><!--�ʼ�guid-->		
		<item type="alone"	  Item_name="type"			   	   data_type="int32"></item><!--�ʼ�����-->
		<item type="alone"	  Item_name="gold"			   	   data_type="int32"></item><!--��Ǯ--> 
		<item type="alone"	  Item_name="goodsnum"			   data_type="int32"></item><!--��Ʒ����-->
		<item type="alone"	  Item_name="incgoodsnum"		   data_type="int32"></item><!--��ֵ��Ʒ����-->    
		<item type="alone"    Item_name="flag"          	   data_type="int32"></item><!--�������ͣ�1=�ɹ���0=ʧ��-->
		</Table>
	*/
	bool logT604_read_mail_log(CPlayer *pPlayer,const CGUID &MailGuid,int nType,long lGold,long lGoodsNum,long lIncGoodsNum,int nFlag);
	/*
	<!--ɾ���ʼ���־-->
	<Table name="t605_delete_mail_log" log_no="605" log_pos="1">
	<item type="client_head"     head_name=""></item> 
	<item type="alone"    Item_name="mail_guid"            data_type="guid"></item><!--�ʼ�guid-->   
	<item type="alone"	  Item_name="receiver"			   data_type="str128"></item><!--������-->
	<item type="alone"	  Item_name="writer"			   data_type="str128"></item><!--������-->    
	<item type="alone"	  Item_name="type"			   	   data_type="int32"></item><!--�ʼ�����-->
	<item type="alone"	  Item_name="gold"			   	   data_type="int32"></item><!--��Ǯ--> 
	<item type="alone"	  Item_name="goodsnum"			   data_type="int32"></item><!--��Ʒ����-->
	<item type="alone"	  Item_name="incgoodsnum"		   data_type="int32"></item><!--��ֵ��Ʒ����-->    
	</Table>*/
	bool logT605_delete_mail_log(const CGUID &MailGuid,const char *strReceiver,const char *strWriter,int nType,long lGold,long lGoodsNum,long lIncGoodsNum);

	/*
	<!--���������NPC��������־-->
	<Table name="t700_business_npc_log" log_no="700" log_pos="1">
		<item type="client_head" head_name=""></item>
		<item type="pos" head_name=""></item>
		<item type="player" head_name=""></item>
		<item type="goods" head_name=""></item>
		<item type="alone" Item_name="act_type" data_type="int32"></item> <!--�������ͣ�1����2����-->
		<item type="alone" Item_name="goods_amount" data_type="int32"></item> <!--��������-->
		<item type="alone" Item_name="trade_spot" data_type="int32"></item> <!--ó�׵�ID-->
		<item type="alone" Item_name="trade_money" data_type="int32"></item> <!--���׵�Ǯ-->
	</Table>
	*/
	bool logT700_business_npc( CPlayer *pPlayer, CGoods *pGoods, int iTradeAmount, 
		int iTradeMoney, int iActType, int iTradeSpot );
	/*
	<!--================================================================-->
	<!--�ɼ���־-->
	<Table name="t055_collect" log_no="55" log_pos="1">
	<item type="client_head"     head_name=""></item>
	<item type="pos"      head_name=""></item>
	<item type="player"   head_name=""></item>
	<item type="goods"    head_name=""></item>
	<item type="alone"    Item_name="opt_id"          data_type="guid"></item><!--����ID-->
	<item type="alone"    Item_name="goods_num"       data_type="int32"></item><!--��Ʒ����-->
	</Table>
	*/
	bool logT055_collect( CPlayer* pPlayer,CGoods* pGoods,const CGUID& guid, int nAmount );

	/*
	<!--================================================================-->
	<!--������־-->
	<Table name="t056_facture" log_no="56" log_pos="1">
	<item type="client_head"     head_name=""></item>
	<item type="player"   head_name=""></item>
	<item type="goods"    head_name=""></item>
	<item type="alone"    Item_name="opt_id"          data_type="guid"></item><!--����ID-->
	<item type="alone"    Item_name="act_type"          data_type="int32"></item><!--��������,0��ɾ��,1�����-->
	<item type="alone"    Item_name="goods_num"       data_type="int32"></item><!--��Ʒ����-->
	</Table>
	*/
	bool logT056_facture( CPlayer* pPlayer,CGoods* pGoods,const CGUID& guid,int nType,int nAmount );


	/*
	<!--================================================================-->
	<!--ÿ��ͼ�ϵ��������¼-->
	<Table name="t800_player_num_in_region" log_no="800" log_pos="1">
		<item type="alone" Item_name="gs_id" data_type="int32"></item>
		<item type="client_head" head_name=""></item>
		<item type="alone" Item_name="region_name" data_type="int32"></item>
		<item type="alone" Item_name="region_id" data_type="int32"></item>
		<item type="alone" Item_name="region_guid" data_type="int32"></item>
		<item type="alone" Item_name="player_num" data_type="int32"></item> <!--�������-->
	</Table>
	*/

	bool logT800_player_num_in_region(LONG lGsID, CServerRegion *pCServerRegion);

	/*
	<!--�������־��¼��T���߼��ս����־-->
	<Table name="t900_anticheat_player_log" log_no="900" log_pos="1">
	<item type="client_head" head_name=""></item>			<!-- ʱ�� -->
	<item type="pos" head_name=""></item>				<!-- λ�� -->
	<item type="player" head_name=""></item>			<!-- ��� -->	
	<item type="alone" Item_name="log_type" data_type="str128"></item> <!-- �������� -->
	<item type="alone" Item_name="ACC_WRONG" data_type="int32"></item> <!--�ۼ����ֵ-->
	<item type="alone" Item_name="CUR_WRONG" data_type="int32"></item> <!--��ǰ���ֵ-->
	<item type="alone" Item_name="CUR_BONUS" data_type="int32"></item> <!--�����ۼƽ�������-->
	</Table>
	*/
	bool logT900_anticheat_player_log( CPlayer * player,  const char *log_type, 
		int acc_wrong, int cur_wrong, int acc_bonus );

	/*
	<!--�������־��¼�����������־-->
	<Table name="t901_anticheat_player_log" log_no="901" log_pos="1">
	<item type="client_head" head_name=""></item>			<!-- ʱ�� -->
	<item type="pos" head_name=""></item>				<!-- λ�� -->
	<item type="player" head_name=""></item>			<!-- ��� -->	
	<item type="alone" Item_name="oper_type" data_type="str128"></item> <!-- ��������:ANSWER, TIMEOUT, REFUSED, EXIT -->
	<item type="alone" Item_name="ACC_WRONG" data_type="int32"></item> <!--�ۼ����ֵ-->
	<item type="alone" Item_name="CUR_WRONG" data_type="int32"></item> <!--��ǰ���ֵ-->
	<item type="alone" Item_name="CUR_RIGHT" data_type="int32"></item> <!--��ǰ������Դ���-->
	<item type="alone" Item_name="CUR_BONUS" data_type="int32"></item> <!--�����ۼƽ�������-->
	<item type="alone" Item_name="PK" data_type="int32"></item> <!--��ǰPKֵ-->
	<item type="alone" Item_name="RHAnswered" data_type="int32"></item> <!--�Ƿ��ڻƽ�ʱ�������ȷ��-->
	</Table>
	*/
	bool logT901_anticheat_answer_log( CPlayer *player, const char *log_type, 
		int acc_wrong, int cur_wrong, int cur_right, int acc_bonus, int pk, bool RHAnswered );

	bool logT902_player_progress_chg_log(CPlayer* pPlayer,int nOldProgress,int nNewProgress);

   /*
    *   <!-- �����־ -->
    *   <Table name="t910_player_marriage_log" log_no="910" log_pos="1">
    *   <item type="client_head" head_name="" />    
    *   <item type="player" head_name="oper" /> <!-- ������� -->
    *   <item type="player" head_name="aim" />  <!-- ������� -->
    *   <item type="alone" Item_name="state" data_type="int32" />  <!-- ״̬��1��ʾ��飬0��ʾ��� -->
    *   </Table>
    */
    bool logT910_player_marriage_log(CPlayer* pOperPlayer, const CGUID* pAimGuid, long nState);



private:
	//!							�õ���ǰʱ���ַ�����00-00-00 00:00:00��ʽ��
	void						GetCurrTimeString(string &strTime);

private:
	//!							��־�ͻ��˶���
	LogClient					*m_pLogClient;

};