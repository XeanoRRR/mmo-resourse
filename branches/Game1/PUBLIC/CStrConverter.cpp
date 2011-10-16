///////////////////////////////////////////////////////////
//  CStrConverter.cpp
//  ʵ�ֶ���	Class CStrConverter
//  �������ڣ�	06-9��-2006 10:37:37
//  ԭʼ���ߣ�	���Ƚ�
///////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CStrConverter.h"


/*
*���ܣ�����ʮ�������ַ���ת��Ϊlong
*������val��Ҫת�����ַ���
*����ֵ��ת����ʮ������
*/		
long CStrConverter::parseStringToDec(const char *val)
{
	int len = (int)strlen(val);
	int i = 0;
	long sum=0;
	int Temp_TiElem = 0;

	while(i<len)
	{
		char t = val[len-i-1];
		if(t>='a' && t<='f')
		{
			Temp_TiElem = t - 87;
		}
		else
		{
			Temp_TiElem = (int)t - 48;
		}	

		sum += Temp_TiElem<<(4*i);					
		++i;
	}
	return sum;
}
/*
*���ܣ������ַ���ת��Ϊfloat
*������val��Ҫת�����ַ���
*����ֵ��ת����float
*/

float CStrConverter::parseFloat(const char *val)
{
	//float f = atof(val);
	return atof(val);
}
/*
*���ܣ������ַ���ת��Ϊlong
*������val��Ҫת�����ַ���
*����ֵ��ת����int
*/			
long  CStrConverter::parseLong(const char *val)
{
	/*int i;
	i = strlen(val);

	if(val[0] == '-')
	{
       return 
	}
	else
	{*/
		return atol(val);
	//}
	
}



/*
*���ܣ������ַ���ת��Ϊint
*������val��Ҫת�����ַ���
*����ֵ��ת����int
*/			
int  CStrConverter::parseInt(const char *val)
{
	return atoi(val);
}

/*
*���ܣ�����ʽת��Ϊlong
*������val��Ҫת�����ַ���
*����ֵ��ת����longֵ
*/
long CStrConverter::parseStyle(const char *val)
{
	if(   !strcmp(val, "SCROLLBAR_LEFT_POS")
		|| !strcmp(val,"STYLE_HORIZONTAL_LABEL")
		|| !strcmp(val,"EDIT_STYLE_NORMAL") 
		|| !strcmp(val,"TEXT_STYLE_NORMAL")
		|| !strcmp(val,"TYPE_NORMAL")
		|| !strcmp(val,"MODE_NULL") )
	{
		return 0;
	}				
	else if(!strcmp(val,"STYLE_VERTICAL_LABEL") 
		|| !strcmp(val,"ALIGNMENT_LABEL_TOP")
		|| !strcmp(val,"SCROLLBAR_RIGHT_POS")
		|| !strcmp(val,"EDIT_STYLE_PASS")
		|| !strcmp(val,"TEXT_STYLE_NUMBER")
		|| !strcmp(val,"TYPE_SCALED")
		|| !strcmp(val,"MODE_NUMBER"))
	{
		return 1;
	}				
	else if( !strcmp(val,"ALIGNMENT_LABEL_LEFT") 
		|| !strcmp(val,"TYPE_TILED")
		|| !strcmp(val,"TEXT_STYLE_UNWCHAR")
		|| !strcmp(val,"MODE_PERCENTAGE")
		|| !strcmp(val,"EC_HAND"))
	{
		return 2;
	}
	else if(!strcmp(val,"ALIGNMENT_LABEL_BOTTOM")
		|| !strcmp(val,"MODE_CHAR"))
	{
		return 3;
	}
	else if(!strcmp(val,"ALIGNMENT_LABEL_RIGHT"))
	{
		return 4;
	}	
	else if (!strcmp(val,"GAP_WEAPON_CATEGORY"))
	{
		return 7;
	}
	else 
	{
		return 0;
	}

}


/*
*���ܣ��������з�ʽ�ַ���ת��Ϊlong
*������val��Ҫת�����ַ���
*����ֵ��ת����longֵ
*/
long CStrConverter::parseRankStyle(const char *val)
{

	if(!strcmp(val,"ALIGNMENT_HORIZONTAL_LEFT|ALIGNMENT_VERTICAL_TOP"))
	{
		return 0;
	}
	else if(!strcmp(val,"ALIGNMENT_HORIZONTAL_LEFT|ALIGNMENT_VERTICAL_CENTER"))
	{
		return 4;
	}                
	else if(!strcmp(val,"ALIGNMENT_HORIZONTAL_LEFT|ALIGNMENT_VERTICAL_BOTTOM"))
	{
		return 8;
	}
	if(!strcmp(val,"ALIGNMENT_HORIZONTAL_CENTER|ALIGNMENT_VERTICAL_TOP"))
	{
		return 1;
	}
	else if(!strcmp(val,"ALIGNMENT_HORIZONTAL_CENTER|ALIGNMENT_VERTICAL_CENTER"))
	{
		return 5;
	}                
	else if(!strcmp(val,"ALIGNMENT_HORIZONTAL_CENTER|ALIGNMENT_VERTICAL_BOTTOM"))
	{
		return 9;
	}
	if(!strcmp(val,"ALIGNMENT_HORIZONTAL_RIGHT|ALIGNMENT_VERTICAL_TOP"))
	{
		return 2;
	}
	else if(!strcmp(val,"ALIGNMENT_HORIZONTAL_RIGHT|ALIGNMENT_VERTICAL_CENTER"))
	{
		return 6;
	}  
	else if(!strcmp(val,"ALIGNMENT_HORIZONTAL_RIGHT|ALIGNMENT_VERTICAL_BOTTOM"))
	{
		return 10;
	} 
	else 
	{
		return 0;
	}


}
/*
*���ܣ������ַ���ת��Ϊbool
*������val��Ҫת�����ַ���
*����ֵ��ת����bool
*/
bool CStrConverter::parseBool(const char *val)
{
	if(!strcmp(val,"true") || !strcmp(val,"TRUE"))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CStrConverter::parseList(char *val,list<long> &lTemp)
{
	if (strlen(val)>0)
	{
		char *pCh;
		pCh = strtok(val,",");
		lTemp.push_back(atoi(pCh));
		while (pCh!=NULL)
		{
			pCh = strtok(NULL,",");
			if (pCh==NULL)
			{
				break;
			}
			lTemp.push_back(atoi(pCh));			
		}
	}

}
const char *CStrConverter::ParseBoolToString(const bool b)
{
	return b==true ? "true":"false";
}

string CStrConverter::GetLeftSplitStr(const std::string &val,size_t SplitPos,string &SplitStrRight)
{
	//�ض�����Ϊ[1,val.length-1]
	if(SplitPos<1 || SplitPos >= val.length())
	{
		SplitStrRight="";
		return val;
	}
	if(!CanSplit(val,SplitPos))
	{
		SplitPos--;
	}
	SplitStrRight = val.substr(SplitPos+1);
	return val.substr(0,SplitPos+1);
}

bool CStrConverter::CanSplit(const std::string &str,size_t pos)
{
	if(pos<1 || pos>=str.length())
		return false;
	if((unsigned char)str[pos] <127 )
	{
		return true;
	}
	int c=1;
	for(size_t i= pos -1; (unsigned char) str[i] >=127 && i>=0;--i)
	{
		++c;
	}
	return c % 2 ==0;
}
//AttackType CStrConverter::parseAttackType(const char *val)
//{
//
//	if (!stricmp(val,"ATTACK_TYPE_PHYSICAL"))
//	{
//		return ATTACK_TYPE_PHYSICAL;
//	}
//	else if (!stricmp(val,"ATTACK_TYPE_MAGERY"))
//	{
//		return ATTACK_TYPE_MAGERY;
//	}
//	else if (!stricmp(val,"ATTACK_TYPE_ELEMENT"))
//	{
//		return ATTACK_TYPE_ELEMENT;
//	}
//	else if (!stricmp(val,"ATTACK_TYPE_SOUL"))
//	{
//		return ATTACK_TYPE_SOUL;
//	}
//	else if (!stricmp(val,"ATTACK_TYPE_POISON"))
//	{
//		return ATTACK_TYPE_POISON;
//	}	
//	else
//	{
//		return ATTACK_TYPE_UNKNOW;
//	}
//}