/**
* @filename:ShopCitySys.h
* @date: 2010/7/19
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: �̳ǵ��߼���ģ��
*/

class CMessage;
class ShopCityMsgMgr;

//#define SAFEDELETE(op)         if (op) { delete op;op=NULL;}

///�̳���ĵ��������߼��������
class SCGData
{
public:
	///�̳�����̵�����
	enum eSCType{
		ST_XINGZUAN = 0,	///����
		ST_DIANQUAN,		///��ȯ
		ST_WEIMIAN,			///λ��
		ST_NUM				///������Ŀ
	};
	/// ��Ʒ���ڵ��̵�����
	enum eSType
	{
		/// ������Ʒ
		TABTYPE_HOT	 = 0,
		/// ҩƷ�̵�
		TABTYPE_YAOPIN,
		/// �ճ��ӻ�
		TABTYPE_ASSISTANT,
		/// װ������
		TABTYPE_EQUIP,
		/// ʱװ��Ʒ
		TABTYPE_FASHION,
		/// ��������
		TABTYPE_NUM,
	};
	///��Ʒ����Ϣ�Ĳ��ֽṹ
	struct tagGDPTA{
		uint type;
		uint index;
	};
	///������Ʒ��Ϣ�ṹ
	struct tagGD 
	{
		uint type;///����
		uint index;///���
		uint price;///�۸�
		uint num;///����
		uint discount;///����
		string timelimit;///��ʱ
		long numlimit;///����
		long buylimit;///�޹�
	};
	///�����¼
	struct tagBRec{
		std::string buytime;///����ʱ��
		uint pricenum;///��������
		uint priceType;///��������
		uint goodsIdx;///��ƷID
		uint buynum;///��������
		tagBRec(){
			buytime = "";
		};
		~tagBRec(){
			buytime = "";
		};
	};

	typedef std::vector<tagGD>VecGD;
	//typedef std::vector<tagGD*>VecGDPtr;
	typedef std::map<eSCType,VecGD> MapSCGD;///������Ʒ
	//typedef std::map<eSCType,VecGDPtr>MapSCGDPtr;

	typedef std::vector<tagBRec> VecSCGBRec;///�������й����¼

	typedef std::vector<tagGDPTA>VecGDPTA;
	typedef std::map<eSCType,VecGDPTA> MapTop10A;///������Ʒ

	typedef MapTop10A MapNewestA;///��Ʒ�Ƽ�

	typedef MapTop10A MapSBT10;///�����������

	typedef std::map<std::string,VecGDPTA> MapStrGGDTPA;
	
	typedef std::map<eSCType,MapStrGGDTPA> MapGuideDataA; ///����

	typedef std::map<std::string,VecGDPTA> MapStrFilDTPA;
	typedef std::map<eSType,MapStrFilDTPA> MapUFLDTPA;
	typedef std::map<eSCType,MapUFLDTPA> MapFLDTA;///ɸѡ

	SCGData();
	~SCGData();

	///��ȡ��Ʒ�Ƽ��б�
	MapNewestA& GetNewestVec() { return m_newestMapA;}

	///��ȡ������Ʒ����
	MapSCGD& GetAllGoodsList() {return m_goodsDTMap;}

	///��ȡ���˹�����ϸ��¼
	VecSCGBRec& GetBuyLogList() { return m_buyRecordVec;}

	///��ȡ������Ʒ�б�
	MapTop10A& GetTop10List() {return m_Top10MapA;}
	///��ȡ�����б�
	MapGuideDataA& GetGuideList() { return m_GuideMapA;}
	///��ȡɸѡ�б�
	MapFLDTA& GetFilterList() { return m_FilterMapA;}
	///��ȡ�����������򵥼�¼
	//VecSelfBuyTop10A& GetSelfBuyTop10() { return m_SelfLatestBuyTop10VecA;}
	MapSBT10& GetSelfBuyTop10() { return m_SelfLatestBuyTop10MapA; }

	/// @brief
	/// �����̳����ͣ��̵����ͷ��ض�Ӧ����Ʒ����Vec
	/// @param
	/// 
	/// @return
	/// 
	void GetVecGoodsData(eSCType scType,eSType sType,VecGD& vecGD);

	/// @brief
	/// ���ݸ�������Ʒ���ֽṹ��������Ʒ��ϸ����VEC
	/// @param
	/// 
	/// @return
	/// 
	void GetTop10InGoodsData(eSCType compType,VecGD& vecGD);

	/// @brief
	/// ���ݸ�������Ʒ�̳ǣ��̵꣬��ɸѡ�ı���������Ʒ��ϸ����
	/// @param
	/// 
	/// @return
	/// 
	void GetFilterInGoodsData(eSCType sctype,eSType stype,string filterstr,VecGD& vecGD);
	/// @brief
	/// ���ݸ������̳ǣ��̵�͵����ı���������Ʒ��ϸ����
	/// @param
	/// 
	/// @return
	/// 
	void GetGuideInGoodsData(eSCType sctype,string guidstr,VecGD& vecGD);
	/// @brief
	/// ������Ʒ�������̳����ͻ�ȡ�������Ʒ��������
	/// @param
	/// 
	/// @return
	/// 
	tagGD* GetGoodsData(uint index,eSCType shopcitytype);

protected:
private:
	friend class ShopCityMsgMgr;
	///�����̳���������
	void DecodeFromDataBlock(CMessage* pMsg);
	/// ���빺���¼����
	void DecodeBuyLogDataBlock(CMessage* pMsg);
	/// ��������
	void Update(CMessage* pMsg);
	/// @brief
	/// ������Ʒ�̵����ͣ�ID�� �̳����ͻ�ȡһ����Ʒ���ݽṹ
	/// @param
	/// 
	/// @return
	/// 
	tagGD* GetGoodsData(eSType goodsType,uint index,eSCType shoptype);
	/// @brief
	/// ��������б�����
	/// @param
	/// 
	/// @return
	/// 
	void Clear();
	//��������̵���Ʒ����
	void ClearAllGoodsList();
	//��ո��˹�����ϸ��¼
	void ClearBuyLogList();
	//���������Ʒ�б�
	void ClearTop10List();
	//�����Ʒ�Ƽ��б�
	void ClearNewestList();
	//��յ����б�
	void ClearGuideList();
	//���ɸѡ�б�
	void ClearSelectList();
	//��ո����������򵥼�¼
	void ClearSelfLatestBuyList();

	MapSCGD m_goodsDTMap;///�����̵����Ͷ����������Ʒ��Ϣ�б�
	//MapSCGDPtr m_goodsDTMapPtr;
	VecSCGBRec m_buyRecordVec;		///���˹�����Ʒ��¼��������ϸ�Ĺ���ʱ���

	MapTop10A			m_Top10MapA;		///������Ʒ

	MapNewestA			m_newestMapA;		///��Ʒ�Ƽ�

	MapGuideDataA		m_GuideMapA;		///����

	MapFLDTA      		m_FilterMapA;				///ɸѡ

	MapSBT10			m_SelfLatestBuyTop10MapA;	///������������¼��ֻ��������ƷId��type
};
///�̳ǵ���Ϣ������
class ShopCityMsgMgr
{
public:
	/// @brief
	/// ���캯��,���̳����ݽ�����new����
	/// @param
	/// 
	/// @return
	/// 
	ShopCityMsgMgr();
	/// @brief
	/// ��������,delete �̳�����
	/// @param
	/// 
	/// @return
	/// 
	~ShopCityMsgMgr();
	/// @brief
	/// �л�ҳ�濪���
	/// @param
	/// 
	/// @return
	/// 
	void SwitchPage();
	/// @brief
	/// ���շ����������̳Ǵ򿪽��淴������Ϣ
	/// @param 
	/// 
	/// @return
	/// 
	void Receive_PageOpen(CMessage* pMsg);
	/// @brief
	/// ���շ����������̳ǹر���Ϣ
	/// @param
	/// 
	/// @return
	/// 
	void Receive_PageClose(CMessage* pMsg);

	/// @brief
	/// ���չ�����
	/// @param
	/// 
	/// @return
	/// 
	void Receive_BuyResult(CMessage* pMsg);

	/// @brief
	/// ���ո�����Ϣ
	/// @param
	/// 
	/// @return
	/// 
	void Receive_Updata(CMessage* pMsg);

	/// @brief
	/// ���չ�����־
	/// @param
	/// 
	/// @return
	/// 
	void Receive_LogResponse(CMessage* pMsg);
	/// @brief
	/// �����̳�ͨ��
	/// @param
	/// 
	/// @return
	/// 
	void Receive_AfficheResponse(CMessage* pMsg);

	/// @brief
	/// 
	/// @param
	/// 
	/// @return
	/// 
	const char* GetAffiche();

	/// @brief
	/// �����̳�����
	/// @param
	/// 
	/// @return
	/// 
	SCGData* GetShopCityGoodsData(){ return dt;}

	/// @brief
	/// 
	/// @param
	/// 
	/// @return
	/// 
	uint GetPlayerXinZuan();
	/// @brief
	/// 
	/// @param
	/// 
	/// @return
	/// 
	uint GetPlayerDianQuan();
	/// @brief
	/// 
	/// @param
	/// 
	/// @return
	/// 
	uint GetPlayerWeiMian();
	/// @brief
	/// 
	/// @param
	/// 
	/// @return
	/// 
	void SetPageState(bool IsOpen);
	/// @brief
	/// 
	/// @param
	/// 
	/// @return
	/// 
	static int GetPageInfo(int ShopCityIdx,int ShopIndex);

	/// @brief
	/// 
	/// @param
	/// 
	/// @return
	/// 
	static void SetPageInfo(int ShopCityIdx,int ShopIndex,int InPage);
	/// @brief
	/// ��ȡUI����Դ����,0.�̳��̵꣬1.ɸѡ 2.����
	/// @param
	/// 
	/// @return
	/// 
	static uint GetStateUpdateUIByType(){return stateUpdateUIbyType;}

	/// @brief
	/// ����UI����Դ����,0.�̳��̵꣬1.ɸѡ 2.����
	/// @param
	/// 
	/// @return
	/// 
	static void SetStateUpdateUIByType(uint i){ stateUpdateUIbyType = i ;}
	/// @brief
	/// ���͹�����Ϣ
	/// @param goodsID
	/// ��ƷID
	/// @param buyNum
	/// ��������
	/// @param buyMode
	/// ����ʱ�����ָ��ѣ����̵�����һ��
	/// @return
	/// 
	void Send_BuyQuery(uint goodsID = 0,uint buyNum = 0,uint buyMode = 0);
	/// @brief
	/// ���ͻ�ȡ������־
	/// @param
	/// 
	/// @return
	/// 
	void Send_GetBuyLog();

	/// @brief
	/// ���ͻ�ȡ�̳ǹ���
	/// @param
	/// 
	/// @return
	/// 
	void Send_GetAffiche();

	/// @brief
	/// ���͹ر��̳���Ϣ
	/// @param
	/// 
	/// @return
	/// 
	void Send_CloseShop(void);
private:
	/// @brief
	/// �����������������̳ǽ���
	/// @param
	/// 
	/// @return
	/// 
	void Send_PageOpen();

	/// @brief
	/// ���͹�ϵ�̳�
	/// @param
	/// 
	/// @return
	/// 
	void Send_PageClose();

	/// @brief
	/// �ж�����Ƿ��Ѿ�����,�Ƿ���true��������CEGUI��װ�õ�messagebox���񷵻�false
	/// @param
	/// 
	/// @return
	/// 
	bool IsPlayerDied();
	/// @brief
	/// ���ýӿ�
	/// @param
	/// 
	/// @return
	/// 
	void DoMsgBoxWhenDied();
	

	bool IsPageOpen;		///��ʶ�����Ƿ��(���߼�����ϣ����������޹�)

	SCGData* dt;	///�̳���Ʒ������
	std::string Affiche;	///�̳ǹ�����Ϣ�ַ���
	
	static int arrPGInfo[3][4];///�����Ӧ�����ҳ��Ϣ[�̳�������Ŀ][�̵�������Ŀ]

	static uint stateUpdateUIbyType;///��ʶUI����Դ����,�Է����ڷ�ҳʱ��������:0.�̳��̵꣬1.ɸѡ 2.����
};