//=============================================================================
/**
 *  file: GameClientBind.cpp
 *
 *  Brief:ΪCGameClient���������չ�ű�����
 *
 *  Authtor:wangqiao
 *	
 *	Date:2007-7-25
 */
//=============================================================================


#include "stdafx.h"
#include "../App/GameClient.h"
#include "../App/Player.h"
#include "../App/Monster.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int collect_game_client(lua_State* L)
{
	CGameClient *self = (CGameClient*)tolua_touserdata(L,1,0);
	delete self;
	return 0;
}

int game_client_new(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertable(L,1,"CGameClient",0,&err) ||
		!tolua_isnoobj(L,3,&err) )
	{
		tolua_error(L,"#ferror in function 'client_new'.",&err);
		return 0;
	}
#endif
	CGameClient* ret = new CGameClient();
	tolua_pushusertype(L,(void*)ret,"CGameClient");
	return 1;
}

int game_client_new_local(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertable(L,1,"CGameClient",0,&err) ||
		!tolua_isnoobj(L,3,&err) )
	{
		tolua_error(L,"#ferror in function 'client_new_local'.",&err);
		return 0;
	}
#endif
	CGameClient* ret = new CGameClient();
	tolua_pushusertype_and_takeownership(L,(void*)ret,"CGameClient");
	return 1;
}

int game_client_delete(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CPlayer",0,&err) ||
		!tolua_isnoobj(L,3,&err) )
	{
		tolua_error(L,"#ferror in function 'client_delete'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L,1,0);
	delete self;
	return 0;
}

int game_client_set_region(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err) ||
		!tolua_isusertype(L,2,"CClientRegion",0,&err) ||
		!tolua_isnoobj(L,3,&err) )
	{
		tolua_error(L,"#ferror in function 'game_client_set_region'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L,1,0);
	CClientRegion* pRgn = (CClientRegion*)tolua_tousertype(L,2,0);
	self->SetClientRegion( pRgn );
	return 0;
}

int game_client_get_region(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isstring(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err))
	{
		tolua_error(L,"#ferror in function 'game_client_get_region'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L,1,0);
	CClientRegion* region =(CClientRegion*)self->GetClientRegion();
	if( region )
		tolua_pushusertype(L,region,"CClientRegion");
	else
		tolua_pushboolean(L,false);
	return 1;
}

int game_client_set_main_player(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err) ||
		!tolua_isusertype(L,2,"CPlayer",0,&err) ||
		!tolua_isnoobj(L,3,&err) )
	{
		tolua_error(L,"#ferror in function 'game_client_set_main_player'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L,1,0);
	self->SetMainPlayer((CPlayer*)tolua_tousertype(L,2,0));
	return 0;
}

int game_client_get_main_player(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isstring(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err) )
	{
		tolua_error(L,"#ferror in function 'game_client_get_main_player'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L,1,0);
	CPlayer* player =(CPlayer*)self->GetMainPlayer();
	if( player == NULL ) 
		tolua_pushboolean(L,false);
	else
		tolua_pushusertype(L,player,"CPlayer");
	return 1;
}

int game_client_set_socket_id(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err) ||
		!tolua_isnumber(L,2,0,&err) ||
		!tolua_isnoobj(L,3,&err) )
	{
		tolua_error(L,"#ferror in function 'game_client_set_socket_id'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L,1,0);
	self->SetSocketID((long)tolua_tonumber(L,2,0));
	return 0;
}

int game_client_get_socket_id(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isstring(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err) )
	{
		tolua_error(L,"#ferror in function 'game_client_get_socket_id'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L,1,0);
	tolua_pushnumber(L,(lua_Number)self->GetSocketID());
	return 1;
}

int game_client_set_cdkey(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err) ||
		!tolua_isstring(L,2,0,&err) ||
		!tolua_isnoobj(L,3,&err) )
	{
		tolua_error(L,"#ferror in function 'game_client_set_socket_id'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L,1,0);
	self->SetCDKey(tolua_tostring(L,2,0));
	return 0;
}

int game_client_get_cdkey(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err) ||
		!tolua_isstring(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err) )
	{
		tolua_error(L,"#ferror in function 'game_client_get_cdkey'.",&err);

		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L,1,0);
	tolua_pushstring(L,self->GetCDKey());
	return 1;
}

//=======================================�Զ�Ѱ·=======================================================================
//Ŀ�곡��ID
int gameclient_get_aim_regionid(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err) ||
		!tolua_isstring(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err) )
	{
		tolua_error(L,"#ferror in function 'gameclient_get_aim_regionid'.",&err);
		return 0;
	}
#endif
	CGameClient* self= (CGameClient*)tolua_tousertype(L,1,0);
	tolua_pushnumber(L,self->GetAimRegionID());
	return 1;
}

int gameclient_set_aim_regionid(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err) ||
		!tolua_isnumber(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err) )
	{
		tolua_error(L,"#ferror in function 'gameclient_set_aim_regionid'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L,1,0);
	self->SetAimRegionID((long)tolua_tonumber(L,2,0));
	return 0;
}
//��Ƕ��Ƭ���
//��ȡ����Ƕ����ʱ��SessID
int get_enchase_sessionguid(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err) ||
		!tolua_isstring(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err) )
	{
		tolua_error(L,"#ferror in function 'get_enchase_sessionguid'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L,1,0);
	const CGUID guid = self->GetEnchaseSessionGuid();

	tolua_pushusertype(L, (void*)&guid, "CGUID");
	return 1;
}
//��ȡ��ȡ����Ƕ����ʱ��lplayerID
int get_enchase_playerid(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err) ||
		!tolua_isstring(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err) )
	{
		tolua_error(L,"#ferror in function 'get_enchase_playerid'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L,1,0);
	long num = (long)self->GetEnchasePlayerID();
	num = num << 8;										//���տͻ�������
	tolua_pushnumber(L, num);
	return 1;
}
//��ȡ�⿨ʱ�Ƿ�װ��������ȥ
int get_enchase_is_equip_putdown(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err) ||
		!tolua_isnoobj(L,2,&err) )
	{
		tolua_error(L,"#ferror in function 'get_enchase_is_equip_putdown'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L,1,0);
	CGoods *goods = (CGoods*)self->GetEnchaseGoods();
	if (goods != NULL)
	{
		tolua_pushboolean(L, true);
	}
	else
	{
		tolua_pushboolean(L, false);
	}
	return 1;
}

//��ȡ�⿨ʱ�Ƿ�Ƭ������ȥ
int get_enchase_is_card_putdown(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err) ||
		!tolua_isnumber(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err) )
	{
		tolua_error(L,"#ferror in function 'get_enchase_is_card_putdown'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L,1,0);
	int i = (int)tolua_tonumber(L, 2, 0);
	CGoods *card = (CGoods*)self->GetEnchaseCard(i);
	if (card != NULL)
	{
		tolua_pushboolean(L, true);
	}
	else
	{
		tolua_pushboolean(L, false);
	}
	return 1;
}
//��ȡ��Ƭ���Ϻ��msg���õ�AimConPos
int get_enchase_AimConPos_for_msg(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err) ||
		!tolua_isstring(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err) )
	{
		tolua_error(L,"#ferror in function 'get_enchase_AimConPos_for_msg'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L,1,0);
	long num = (long)self->GetEnchaseAimConPosForMsg();
	tolua_pushnumber(L, num);
	return 1;
}

/*
	�������
	:��ȡ��ǰ�򿪽����type
*/
int gameclient_get_open_page_type(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error e;
	if(!tolua_isusertype(L,1,"CGameClient",0,&e)||
		!tolua_isstring(L,2,0,&e)||
		!tolua_isnoobj(L,3,&e))
	{
		tolua_error(L,"#ferror in function 'gameclient_get_open_page_type'.",&e);
		return 0;
	}
#endif
	CGameClient* pClient = (CGameClient*)tolua_tousertype(L,1,0);
	long lType = (long)pClient->GetCurOpenPageType();
	tolua_pushnumber(L,lType);
	return 1;
}
//----------------------------------------------------------------

/*********************************
װ������
**********************************/
//�õ���������
int player_get_upgrade_stuff(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isstring(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err))
	{
		tolua_error(L,"#ferror in function 'player_get_upgrade_stuff'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L,1,0);
	tagUpgradeStuff* pStuff = self->GetUpgradeStuff();
	ZeroMemory( pStuff, sizeof(tagUpgradeStuff) );
	tolua_pushuserdata(L,pStuff);
	return 1;
}
int player_get_upgrade_stuff_size(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isstring(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err))
	{
		tolua_error(L,"#ferror in function 'player_get_upgrade_stuff_size'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L,1,0);
	tagUpgradeStuff* pStuff = self->GetUpgradeStuff();
	long lSize = sizeof(*pStuff);
	tolua_pushnumber(L,lSize);
	return 1;
}
//�����������
int player_upgrade_stuff_add_goods(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isuserdata(L,2,0,&err)||
		!tolua_isnumber(L,3,0,&err) ||
		!tolua_isnoobj(L,4,&err))
	{
		tolua_error(L,"#ferror in function 'player_upgrade_stuff_add_goods'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L,1,0);
	tagGoods* ptagGoods = (tagGoods*)tolua_touserdata(L,2,0);
	long lIndex = (long)tolua_tonumber(L,3,0);

	if( self && ptagGoods )
	{
		tagUpgradeStuff* pStuff = self->GetUpgradeStuff();
		if( lIndex>=0 && lIndex<5)
		{
			//pStuff->arrContainID[lIndex] = ptagGoods->lType;
			//pStuff->arrGoodsGuid[lIndex] = ptagGoods->pGoods->GetExID();
			//pStuff->arrGoodsPos[lIndex] = ptagGoods->lPos;
		}
	}
	return 0;
}

//�����ȡ��ǰ�����ϵ�һ������
// by tanhaowen 2008.12.11
int gameclient_get_random_obj(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isstring(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err))
	{
		tolua_error(L,"#ferror in function 'gameclient_get_random_obj'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L, 1, 0);
	
	list<CBaseObject*>::iterator it = self->GetClientRegion()->GetObjectList()->begin();		//��ȡbaseobjlist
	long obj_size	= self->GetClientRegion()->GetObjectList()->size();							//��ȡbaseobjlist�Ĵ�С
	
	if ( obj_size >0 )
	{	
	long random_num = rand()%obj_size;															//���ݴ�С������һ������С��size
	long num        = 0;																		//������	

	for( ;it != self->GetClientRegion()->GetObjectList()->end(); ++it )
	{
		if (random_num == num)
		{
			tolua_pushusertype(L, (void*)(*it), "CBaseObject");
			break;
			return 1;
		}
		num++;
	}
	
	}

	return 1;	
}

//�����ȡ��ǰ�����ϵ�һ�������������
// by tanhaowen 2008.12.16
int gameclient_get_random_objlist(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isnoobj(L,2,&err))
	{
		tolua_error(L,"#ferror in function 'gameclient_get_random_objlist'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L, 1, 0);

	list<CBaseObject*> *listBaseObj = self->GetClientRegion()->GetObjectList();		//��ȡbaseobjlist
	
	if (listBaseObj)
	{	
		long lSize = listBaseObj->size();	
		long lNum  = 0;
		//ȡ�ñ���Ԫ�ظ���
		list<CBaseObject*>::iterator it = listBaseObj->begin();
		for ( ; it!= listBaseObj->end(); ++it)
		{
			lNum++;
			tolua_pushusertype(L, (void*)(*it), "CBaseObject");						//ѹ������ջ
			if ( lNum == 10 )
			{
				break;
		}
		}

		return lNum;	
	}
	else
	{
		return 0;
	}
}

//��ǰ�����Ƿ��ڳ���������
int gameclient_is_find_shape(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isusertype(L,2,"CGUID",0,&err)||
		!tolua_isnoobj(L,3,&err))
	{
		tolua_error(L,"#ferror in function 'gameclient_is_find_shape'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L, 1, 0);

	list<CBaseObject*> *listBaseObj = self->GetClientRegion()->GetObjectList();		//��ȡbaseobjlist
	const CGUID guid = *(CGUID*)tolua_tousertype(L, 2, 0);
	if ( listBaseObj )
	{	
		long lSize = listBaseObj->size();											//ȡ�ñ���Ԫ�ظ���
		list<CBaseObject*>::iterator it = listBaseObj->begin();
		for ( ; it != listBaseObj->end(); ++it)
		{
			if ( guid == (*it)->GetExID() )
			{
				tolua_pushboolean(L, true);
				return 1;
			}
		}

		tolua_pushboolean(L, false);
		return 1;
	}
	else
	{
		return 0;
	}
}

//��췽����һ������
int gameclient_get_monster(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isstring(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err))
	{
		tolua_error(L,"#ferror in function 'gameclient_get_monster'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L, 1, 0);
	list<CBaseObject*> *listBaseObj = self->GetClientRegion()->GetObjectList();		//��ȡbaseobjlist
	if ( listBaseObj && listBaseObj->size()>1 )
	{
		list<CBaseObject*>::iterator it = listBaseObj->begin();
		for ( ; it!=listBaseObj->end(); ++it)
		{
			if ( (*it)->GetType()==600 )
			{
				CMonster *monster = (CMonster*)(*it);
				if ( monster->GeteNpcType() != 0  && monster->GeteNpcType() != 3)
				{
					tolua_pushusertype(L, (void*)(*it), "CBaseObject");
//					self->GetMainPlayer()->SetSelectedShapeGuid((*it)->GetExID());
					break;
				}
			}
		}
		return 1;
	}
	return 0;
}

//���Ҿ���player 2���������ڵ�goods
int gameclient_get_pickup_goods(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isnoobj(L,2,&err))
	{
		tolua_error(L,"#ferror in function 'gameclient_get_pickup_goods'.",&err);
		return 0;
	}
#endif
	CGameClient* self = (CGameClient*)tolua_tousertype(L, 1, 0);
	CPlayer *pPlayer  = self->GetMainPlayer();
	if ( !self || !pPlayer )
	{
		return 0;
	}
	list<CBaseObject*> *listBaseObj = self->GetClientRegion()->GetObjectList();		//��ȡbaseobjlist
	if ( listBaseObj && listBaseObj->size()>1 )
	{	
		long lGoodsSize = 0;
		
		list<CBaseObject*>::iterator it = listBaseObj->begin();
		for ( ; it!=listBaseObj->end(); ++it)
		{	
			
			if ( (*it)->GetType()==700 && pPlayer->RealDistance((CShape*)(*it)) <= 2)
			{
				lGoodsSize++;
				tolua_pushusertype(L, (void*)(*it), "CBaseObject");
			}
		}
		return lGoodsSize;
	}
	return 0;
}


//---------------------------------------------------------
// ���� [12/22/2008 Square]
int gameclient_get_temp_id_by_index(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isnumber(L,2,0,&err) ||
		//!tolua_isnumber(L,3,0,&err) ||
		!tolua_isnoobj(L,3,&err))
	{
		tolua_error(L,"#ferror in function 'gameclient_get_temp_id_by_index'.",&err);
		return 0;
	}
#endif
	CGameClient* pClient = (CGameClient*)tolua_tousertype(L,1,0);
	long lIndex = (long)tolua_tonumber(L,2,0);
	long lReionID = pClient->GetTempID(lIndex);
	tolua_pushnumber(L,lReionID);
	return 1;
}
int gameclient_get_region_guid_by_index(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isnumber(L,2,0,&err) ||
		!tolua_isnoobj(L,3,&err))
	{
		tolua_error(L,"#ferror in function 'gameclient_get_region_guid_by_index'.",&err);
		return 0;
	}
#endif
	CGameClient* pClient = (CGameClient*)tolua_tousertype(L,1,0);
	long lIndex = (long)tolua_tonumber(L,2,0);
	const CGUID& guid = pClient->GetRegionGuid(lIndex);
	tolua_pushusertype(L,(void*)&guid,"CGUID");
	return 1;
}

int gameclient_get_line_num(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isstring(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err))
	{
		tolua_error(L,"#ferror in function 'gameclient_get_line_num'.",&err);
		return 0;
	}
#endif
	CGameClient* pClient = (CGameClient*)tolua_tousertype(L,1,0);
	long lLineNum = pClient->GetLineNum();
	tolua_pushnumber(L,lLineNum);
	return 1;
}
int gameclient_get_line_id(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isstring(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err))
	{
		tolua_error(L,"#ferror in function 'gameclient_get_line_id'.",&err);
		return 0;
	}
#endif
	CGameClient* pClient = (CGameClient*)tolua_tousertype(L,1,0);
	long lLineID = pClient->GetLineID();
	tolua_pushnumber(L,lLineID);
	return 1;
}
// gameclient_decord_line_from_datablcok [12/23/2008 Square]
int gameclient_decord_line_from_datablcok(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isuserdata(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err))
	{
		tolua_error(L,"#ferror in function 'gameclient_decord_line_from_datablcok'.",&err);
		return 0;
	}
#endif
	CGameClient* pClient = (CGameClient*)tolua_tousertype(L,1,0);
	DBReadSet* pData = (DBReadSet*)tolua_touserdata(L,2,0);
	pClient->DecordLineFromDataBlock(*pData);
	return 0;
}

//(x,y,size)�������Ƿ���shape
int gameclient_get_space_shape(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;

	if(!tolua_isusertype(L,1,"CGameClient",0,&err) ||
		!tolua_isnumber(L,2,0,&err)||
		!tolua_isnumber(L,3,0,&err)||
		!tolua_isnumber(L,4,0,&err)||
		!tolua_isnumber(L,5,0,&err)||
		!tolua_isusertype(L,6,"CShape",0,&err)||
		!tolua_isnoobj(L,7,&err) )
	{
		tolua_error(L,"#ferror in function 'gameclient_get_space_shape'.",&err);
		return 0;
	}
#endif
	CGameClient *self = (CGameClient*)tolua_tousertype(L,1,0);
	if( self != NULL )
	{
		long x = (long)tolua_tonumber(L,2,0);
		long y = (long)tolua_tonumber(L,3,0);
		long size = (long)tolua_tonumber(L,4,0);
		OBJECT_TYPE eType = (OBJECT_TYPE)(long)tolua_tonumber(L,5,0);
		CShape* pShape = (CShape*)tolua_tousertype(L,6,"CShape");
		if( pShape != NULL )
		{
			CShape* pFindShape = (CShape*)self->GetShapeFromSpace(x,y,size,eType,pShape);
			if ( pFindShape != NULL )
			{
				tolua_pushusertype(L,(void*)pFindShape,"CShape");
				return 1;
			}
		}
	}
	tolua_pushboolean(L,false);
	return 1;
}

//��ȡ��ǰ�Ƿ����PingSrv 2009.7.27
int gameclient_get_is_can_pingserver(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isstring(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err))
	{
		tolua_error(L,"#ferror in function 'gameclient_get_is_can_pingserver'.",&err);
		return 0;
	}
#endif
	CGameClient* pClient = (CGameClient*)tolua_tousertype(L,1,0);
	if ( pClient )
	{
		tolua_pushboolean(L, pClient->GetIsCanPingSrv());
		return 1;
	}
	
	return 0;
}

//���ü��ʱ��
int gameclient_set_pingserver_blanktime(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isnumber(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err))
	{
		tolua_error(L,"#ferror in function 'gameclient_set_pingserver_blanktime'.",&err);
		return 0;
	}
#endif
	CGameClient* pClient = (CGameClient*)tolua_tousertype(L,1,0);
	if ( pClient )
	{
		pClient->SetBlankPingTime(tolua_tonumber(L, 2, 0));
	}
	return 0;
}

//��ȡPingSrv����ֵ
int gameclient_get_pingserver_val(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isstring(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err))
	{
		tolua_error(L,"#ferror in function 'gameclient_get_pingserver_val'.",&err);
		return 0;
	}
#endif
	CGameClient* pClient = (CGameClient*)tolua_tousertype(L,1,0);
	
	if ( pClient )
	{
		tolua_pushnumber(L, pClient->GetPingSrvSendVal());
		return 1;
	}
	
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ��ȡ��ǰDriveGameClient��MappingFile�߳�Id
// by MartySa 2009.2.3
int gameclient_get_mapfile_uID(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isstring(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err))
	{
		tolua_error(L,"#ferror in function 'gameclient_get_mapfile_uID'.",&err);
		return 0;
	}
#endif
	CGameClient* pClient = (CGameClient*)tolua_tousertype(L,1,0);
	unsigned uID		 = pClient->GetMemServeruID();
	tolua_pushnumber(L, uID);
	return 1;
}

//��ȡ��ǰPlayer�Ƿ����ƶ�
int gameclient_get_player_is_move(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isstring(L,2,0,&err)||
		!tolua_isnoobj(L,3,&err))
	{
		tolua_error(L,"#ferror in function 'gameclient_get_player_is_move'.",&err);
		return 0;
	}
#endif
	CGameClient* pClient = (CGameClient*)tolua_tousertype(L,1,0);
	bool isMove			 =	pClient->GetPlayerIsMove();
	tolua_pushboolean(L, isMove);
	return 1;
}
//---------------------------------------------------------------------------
//  [8/11/2009 Square]
int gameclient_add_object(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isusertype(L,2,"CBaseObject",0,&err)||
		!tolua_isnoobj(L,3,&err))
	{
		tolua_error(L,"#ferror in function 'gameclient_get_player_is_move'.",&err);
		return 0;
	}
#endif
	CGameClient* pClient = (CGameClient*)tolua_tousertype(L,1,0);
	CBaseObject* pObject = (CBaseObject*)tolua_tousertype(L,2,0);
	if( pClient != NULL && 
		pObject != NULL )
	{
		pClient->AddObjectToClientRgn( pObject );
	}
	return 0;
}

int gameclient_remove_all_object(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err)||
		!tolua_isnoobj(L,2,&err))
	{
		tolua_error(L,"#ferror in function 'gameclient_remove_all_object'.",&err);
		return 0;
	}
#endif
	CGameClient* pClient = (CGameClient*)tolua_tousertype(L,1,0);
	pClient->RemoveAllObject();
	return 0;
}

//  [8/11/2009 Square]
int gameclient_find_object(lua_State* L)
{
#ifdef _DEBUG
	tolua_Error err;
	if(!tolua_isusertype(L,1,"CGameClient",0,&err) ||
		!tolua_isnumber(L,2,0,&err) ||
		!tolua_isusertype(L,3,"CGUID",0,&err)||
		!tolua_isnoobj(L,4,&err))
	{
		tolua_error(L,"&ferror paramter in funciton 'gameclient_find_object'.",&err);
		return 0;
	}
#endif
	CGameClient *self = (CGameClient *)tolua_tousertype(L,1,0);
	if( self != NULL)
	{
		long lType = (long)tolua_tonumber(L,2,0);
		CGUID* guid = (CGUID *)tolua_tousertype(L,3,0);
		if( guid != NULL )
		{
			CBaseObject* pObject = self->FindObjectFromClientRgn(lType,*guid);
			if( pObject!=NULL)
			{
				string strType;
				if(lType == TYPE_PLAYER)
					strType = "CPlayer";
				else if( lType == TYPE_MONSTER )
					strType = "CMonster";
				else if( lType == TYPE_GOODS )
					strType = "CGoods";
				else if( lType == TYPE_COLLECTION )
				{
					tolua_pushusertype(L,pObject,"CShape");
					return 1;
				}
				else
				{
					tolua_pushboolean(L,false);
					return 1;
				}
				tolua_pushusertype(L,pObject,strType.c_str());
				return 1;
			}
			else
			{
				tolua_pushboolean(L,false);
				return 1;
			}
		}
	}
	tolua_pushboolean(L,false);
	return 1;
}

//---------------------------------------------------------------------------
void game_client_reg_types (lua_State* L)
{
	tolua_usertype(L,"CGameClient");
}
//��baseobject��
int  game_client_open(lua_State* L)
{
	tolua_open(L);
	game_client_reg_types(L);
	tolua_module(L,NULL,0);
	tolua_beginmodule(L,NULL);
	{
		tolua_module(L,"Game",0);
		tolua_beginmodule(L,"Game");
		{
			tolua_cclass(L,"GameClient","CGameClient","",collect_game_client);
			tolua_beginmodule(L,"GameClient");
			{
				tolua_function(L,"decord_line_from_data_block",	gameclient_decord_line_from_datablcok);
				tolua_function(L,"space_shape",		gameclient_get_space_shape);			//һ����(x,y,size)�ϵ�size�������Ƿ���shape
				//��CPlayer��ĺ���
				tolua_function(L,"new",			game_client_new);
				tolua_function(L,"new_local",	game_client_new_local);
				tolua_function(L,".call",		game_client_new_local);
				tolua_function(L,"delete",		game_client_delete);
				//  [8/11/2009 Square]
				tolua_function(L,"add_object",	gameclient_add_object);
				tolua_function(L,"find_object",	gameclient_find_object);
				tolua_function(L,"remove_all_object", gameclient_remove_all_object);
				//�ͻ��˱���
				tolua_variable(L,"player_ptr",	game_client_get_main_player,	game_client_set_main_player);
				tolua_variable(L,"region_ptr",	game_client_get_region,			game_client_set_region);
				tolua_variable(L,"socketid",	game_client_get_socket_id,		game_client_set_socket_id);
				tolua_variable(L,"cdkey",		game_client_get_cdkey,			game_client_set_cdkey);
				//Ŀ���
				tolua_variable(L,"aim_regionid", gameclient_get_aim_regionid,gameclient_set_aim_regionid);
				//�������
				tolua_variable(L,"openpage_type",	gameclient_get_open_page_type,	NULL);
				//��Ƕ���
				tolua_variable(L,"enchase_sessid",		get_enchase_sessionguid, NULL);		//��ȡ��Ƕ����guid
				tolua_variable(L,"enchase_playerid",	get_enchase_playerid,	 NULL);		//��ȡ��Ƕ�������guid
				tolua_variable(L,"enchase_conpos",		get_enchase_AimConPos_for_msg,	 NULL);		//��ȡ��Ƕ����ſ�S2C�е�AimConPos
				tolua_function(L,"is_echequip_putdown",	get_enchase_is_equip_putdown);//��ȡ�⿨װ���Ƿ����
				tolua_function(L,"is_echcard_putdown",	get_enchase_is_card_putdown);	//��ȡ�⿨��Ƭ�Ƿ����

				//װ������
				tolua_variable(L,"upgrade_stuff",			player_get_upgrade_stuff,		NULL);
				tolua_variable(L,"upgrade_stuff_size",		player_get_upgrade_stuff_size,	NULL);
				tolua_function(L,"add_upgrade_stuff",		player_upgrade_stuff_add_goods);

				//�����ȡ��ǰ������һ��guid
				tolua_variable(L,"get_rand_obj", gameclient_get_random_obj, NULL);
				//�����õ�ǰ����һ�������ObjectList
				tolua_function(L, "get_rand_objlist", gameclient_get_random_objlist);//, NULL);
				// ���� [12/22/2008 Square]
				tolua_variable(L,"line_num"	,gameclient_get_line_num,	NULL);
				tolua_variable(L,"line_id",	gameclient_get_line_id,	NULL);
				tolua_function(L,"rgn_id",	gameclient_get_temp_id_by_index);
				tolua_function(L,"rgn_guid",	gameclient_get_region_guid_by_index);
				//// ��ȡ��ǰDriveGameClient��MappingFile�߳�Id
				tolua_variable(L,"mapfile_id",	gameclient_get_mapfile_uID,	NULL);
				//gameclient_get_player_is_move
				tolua_variable(L,"is_player_move",	gameclient_get_player_is_move,	NULL);
				//��ǰ����shape�Ƿ��ڳ���������
				tolua_function(L,"is_find_shape",	gameclient_is_find_shape);
				//gameclient_get_monster
				tolua_variable(L,"get_monster",		gameclient_get_monster, NULL);
				//ȡ�þ������2���������ڵ�goods
				//gameclient_get_pickup_goods
				tolua_function(L,"get_pickup_goods", gameclient_get_pickup_goods);

				//PingSrv
				tolua_variable(L,"is_can_pingsrv",		gameclient_get_is_can_pingserver, NULL);				
				tolua_variable(L,"get_pingsrv_sendval",	gameclient_get_pingserver_val, NULL);
				tolua_function(L,"set_pingsrv_timers",	gameclient_set_pingserver_blanktime);
			}
			tolua_endmodule(L);
		}
		tolua_endmodule(L);
	}
	tolua_endmodule(L);
	return 1;
}