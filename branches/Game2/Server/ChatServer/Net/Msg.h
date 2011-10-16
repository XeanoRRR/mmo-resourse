#ifndef MSG_H
#define MSG_H

class Msg : public CBaseMessage
{
public:
    long DefaultRun();          // ȱʡ����Ϣ��������
    long Run();                 // ����������Ϣ����

    long SendToClient(long);    // ����һ����Ϣ���ͻ����׽���
    long SendAllClient();       // ���͸����пͻ���

    // constructors & destructor
public:
    Msg() : m_SockId(0), m_Ip(0) {}
    explicit Msg(ulong type) : m_SockId(0), m_Ip(0) {Init(type);}
    ~Msg() {UnInit();}

    // operations for data member
public:
    void Reset() {m_SockId=0; m_Ip=0;}
    void SetSocketID(long i) {m_SockId = i;}
    void SetIP(ulong i) {m_Ip = i;}
    long GetSocketID() {return m_SockId;}
    ulong GetIP() {return m_Ip;}
    bool IsValidGsIp();

    // data implementation
private:
    long m_SockId;
    ulong m_Ip;

    // initialize
public:
    static bool Initial(CDataBlockAllocator*, long);
    static bool Release();
    static CBaseMessage* NewMessage();

    // servers
private:
    static CServer* s_NetServer4GS;     // for game server
    static CServer* s_NetServer4Client; // for game client
public:
    static void SetNetServer4GS(CServer*);
    static void SetNetServer4Client(CServer*);
};

#endif//#ifndef MSG_H