// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once

#pragma warning(disable:4018;disable:4244;disable:4267;disable:4311;disable:4312;disable:4311;disable:4996)

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����

// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�
#include <afx.h>
//#include "../../public/debugnew.h"
#include "lua.hpp"
#include "../../../DefPublic.h"
#include "../../../ServerPublic/ServerPublic.h"
#include "../../../Common/GoodsDef.h"
#include "../../../Common/PlayerDef.h"
#include "../../../Common/MonsterDef.h"
#include "../../../Common/DepotDef.h"
#include "../../../Common/OrganizingDef.h"
#include "../../../Common/BounsDef.h"
#include "../../../Common/SkillRelated.h"

#include "../Utility/PlayerDef.h"
#include "../Utility/GoodsExDef.h"
#include "../../../Nets/MemServer.h"

// ��׼ģ���
#include <list>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <deque>
#include <set>
using namespace std;

#include <hash_map>
using namespace stdext;