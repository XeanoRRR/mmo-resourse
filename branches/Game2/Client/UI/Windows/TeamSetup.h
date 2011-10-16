/**
* @filename:TeamSetup.h
* @date: 2010/6/7
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 

* @purpose: �����������
*/

///����������ù�����
class TeamSetup
{
public:
	///
	TeamSetup();
	///
	~TeamSetup();
	///��setup/UISetup/Team.ini��ȡ����
	bool ReadSetup();
	///
	int GetInterval() { return interval; }
	///
	int GetMaxItem() const { return maxItem;}
	///
	int GetCurItemNum() const { return CurItemNum; }
	///
	void ResetCurItemNum() { CurItemNum = 0; }
	///
	void CurItemNumAdded() { CurItemNum++ ;}
	///
	CEGUI::Vector2& GetPagePos()  { return pagePos; }
	///
	CEGUI::Vector2& GetHeadPos()  { return headWndOffset; }
	///
	CEGUI::Vector2& GetHpPos()  { return HpWndOffset; }
	///
	CEGUI::Vector2& GetMpPOs()  { return MpWndOffset; }
	///
	CEGUI::Vector2& GetPlayerPos() { return PlayerWndOffset;}
	///
	CEGUI::Vector2& GetLevelPos() { return LevelWndOffset; }
	
	///
	float GetItemHeight() const { return itemSize.d_height; }
	///
	float GetItemWidth() const { return itemSize.d_width; }
	///
	CEGUI::UVector2 GetItemSize() const { return CEGUI::UVector2(cegui_absdim(itemSize.d_width),
		cegui_absdim(itemSize.d_height)); }
	///
	CEGUI::UVector2 GetHeadSize() const { return CEGUI::UVector2(cegui_absdim(headWndSize.d_width),
		cegui_absdim(headWndSize.d_height)); }
	///
	CEGUI::UVector2 GetHpSize() const { return CEGUI::UVector2(cegui_absdim(HpWndSize.d_width),
		cegui_absdim(HpWndSize.d_height)); }
	///
	CEGUI::UVector2 GetMpSize() const { return CEGUI::UVector2(cegui_absdim(MpWndSize.d_width),
		cegui_absdim(MpWndSize.d_height)); }
	///
	CEGUI::UVector2 GetPlayerSize() const { return CEGUI::UVector2(cegui_absdim(PlayerWndSize.d_width),
		cegui_absdim(PlayerWndSize.d_height)); }
	///
	CEGUI::UVector2 GetLevelSize() const { return CEGUI::UVector2(cegui_absdim(LevelWndSize.d_width),
		cegui_absdim(LevelWndSize.d_height)); }
	///
	const char* GetItemWndType() const { return ItemWndType; }
	///
	const char* GetItemWndName() const { return ItemWndName; }
	///
	const char* GetHeadWndType() const { return headWndType; }
	///
	const char* GetHeadWndName() const { return headWndName; }
	///
	const char* GetMpWndType() const { return MpWndType; }
	///
	const char* GetMpWndName() const { return MpWndName; }
	///
	const char* GetHPWndType() const { return HpWndType; }
	///
	const char* GetHPWndName() const { return HpWndName; }

	///
	const char* GetPlayerWndName() const { return PlayerWndName; }
	///
	const char* GetPlayerWndType() const { return PlayerWndType; }
	///
	const char* GetLevelWndName() const { return LevelWndName; }
	///
	const char* GetLevelWndType() const { return LevelWndType; }
private:
	uint maxItem;				///�����ʾ��Ա��
	uint CurItemNum;			///��ǰitem��Ŀ
	CEGUI::Vector2   pagePos;	///���ڳ�ʼλ��
	CEGUI::Size		 itemSize;	///item�Ĵ�С
	int interval;				///Item�������

	char ItemWndName[256];	///��Ŀ���ڵ�����ǰ׺
	char ItemWndType[256];	///��Ŀ��������

	char headWndType[256];	///ͷ�񴰿���������
	char headWndName[256];  ///ͷ�񴰿�����
	CEGUI::Size	headWndSize; ///ͷ�񴰿ڵĴ�С
	CEGUI::Vector2  headWndOffset;///ͷ�񴰿ڵ�λ��ƫ��

	char HpWndType[256];
	char HpWndName[256];
	CEGUI::Size HpWndSize;
	CEGUI::Vector2 HpWndOffset;

	char MpWndType[256];
	char MpWndName[256];
	CEGUI::Size MpWndSize;
	CEGUI::Vector2 MpWndOffset;

	char PlayerWndName[256];
	char PlayerWndType[256];
	CEGUI::Size PlayerWndSize;
	CEGUI::Vector2 PlayerWndOffset;

	char LevelWndName[256];
	char LevelWndType[256];
	CEGUI::Size LevelWndSize;
	CEGUI::Vector2 LevelWndOffset;
};