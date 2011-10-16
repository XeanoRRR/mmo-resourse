#pragma once

class CBitmapX;
class CShape;

// ģ�͹�����
class CPicList
{
public:
	enum ePicType
	{
		PT_STATE_ICON,			// ״̬ͼ��
		PT_SKILL_ICON,			// ����ͼ��
		PT_SKILL_ICON_GRAY,		// ����ͼ���ɫ
		PT_COUNTRY_ICON,		// �������ֱ��ϵĹ��ұ��
		PT_PLAYER_COUNTRY_ICON,	// �������Ĺ��ұ��
		PT_GOODS_ICON,			// ��������Ʒͼ��
		//--------------07.9.10liuke�������ͼ��------------
		PT_QUEST_ICON,			// �����ͼƬͼ��
		PT_MONSTER_ICON,		// ����ĳƺ�ͼ��
		PT_MINIMAP_ICON,		// С��ͼ�ϵ�����ͼ��
		PT_TALKBOX_ICON,		// talkbox�еĸ��ӱ���ͼ��
		PT_TEXT_ICON,			// talkbox�е�ͼ��
		//--------------------------------------------------
		PT_MONSTER_HEAD_ICON,	// ����ͷ��ͼƬ
		PT_TSGOODS_ICON,		// ��ֵ�̵���ƷͼƬ
		PT_PET_ICON,			// ����ͷ��ͼƬ
	};

	CPicList(void);
	virtual ~CPicList(void);

	// ģ�ͽṹ
	struct tagBitmap
	{
		long		lType;			// ����
		long		lGraphicsID;	// ͼƬ���
		DWORD		lUseCount;		// ʹ�õĴ���
		CBitmapX	*pBitmap;		// ģ��
		tagBitmap()
		{
			lType		= -1;
			lGraphicsID = 0;
			lUseCount	= 0;
			pBitmap		= NULL;
		}
	};

protected:
	list<tagBitmap>	m_listPic;					// ģ���б�

public:
	tagBitmap* Load(long lType, long lGraphicsID, bool bFind = true);		// ����һ����Դ( ������֮ǰ�Ƿ�Ҫ����Ҫ�������ͼ���Ƿ���� )
	BOOL Release(long lType, long lGraphicsID);		// �ͷ�һ����Դ
	BOOL ReleaseAll(void);							// ጷ������YԴ

	// ��ʾ
	BOOL Display(long lType,long lGraphicsID,long x, long y, DWORD color = 0xffffffff, RECT *rectClip=NULL);
	// ������ʾ
	BOOL Display(long lType,long lGraphicsID,long x, long y, long w, long h, DWORD color = 0xffffffff);

	// ��ȡ
	tagBitmap* GetBitmap(long lType, long lGraphicsID);	// ��ȡ

	tagBitmap* GetBitmap(CBitmapX* pBitmapX);			// ��ȡ

    //��ȡͼƬ·���ļ���
    const char * GetPicFilePathName(long lType, long lGraphicsID);
};
