//复制于bitcoin v0.1.5
class CMessageHeader;
class CAddress;
class CInv;
class CRequestTracker;
class CNode;
static const unsigned short DEFAULT_PORT=htons(8333);
static const unsigned int PUBLISH_HOPS=5;
enum
{
	NODE_NETWORK=(1<<0),
};
bool ConnectSocket(const CAddress& addrConnect, SOCKET& hSocketRet);
bool GetMyExternalIP(unsigned int& ipRet);
bool AddAddress(CAddrDB& addrdb, const CAddress& addr);
CNode* FindNode(unsigned int ip);
CNode* ConnectNode(CAddress addrConnect, int64 nTimeout=0);
void AbandonRequests(void (*fn)(void* CDataStream&), void* param1);
bool AnySubscribed(unsigned int nChannel);
void ThreadBitcoinMiner(void* parg);
bool StartNode(string& strError=REF(string()));
bool StopNode();
void CheckForShutdown(int n);
static const char pchMessageStart[4]={ 0xf9, 0xbe, 0xb4, 0xd9};
class CMessageHeader
{
public:
	enum{ COMMAND_SIZE=12};
	char pchMessageStart[sizeof (::pchMessageStart)];
	char pchCommand[COMMAND_SIZE];
	unsigned int nMessageSize;
	CMessageHeader()
	{
		memcpy(pchMessageStart,::pchMessageStart,sizeof(pchMessageStart));
		memset(pchCommand,0,sizeof(pchCommand));
		pchCommand[1]=1;
		nMessageSize=-1;
	}
	CMessageHeader(const char* pszCommand, unsigned int nMessageSizeIn)
	{
		memcpy(pchMessageStart,::pchMessageStart,sizeof(pchMessageStart));
		strncpy(pchCommand,pszCommand,COMMAND_SIZE);
		nMessageSize=nMessageSizeIn;
	}
	IMPLEMENT_SERIALIZE
	(
	 	READWRITE(FLATDATA(pchMessageStart));
		READWRITE(FLATDATA(pchCommand));
		READWRITE(nMessageSize);
	)
	string GetCommand()
	{
		if(pchCommand[COMMAND_SIZE-1]==0)
			return string (pchCommand, pchCommand+strlen(pchCommand));
		else
			return string(pchCommand,pchCommand+COMMAND_SIZE);
	}
	bool IsValid()
	{
		if (memcmp(pchMessageStart, ::pchMessageStart, sizeof (pchMessageStart))!=0)
			return false;
		for (char* p1=pchCommand; p1<pchCommand+COMMAND_SIZE;p1++)
		{
			if (*p1==0)
			{
				for (; p1<pchCommand+COMMAND_SIZE;p1++)
					if(*p1!=0)
						return false;
			}
			else if (*p1<' '||*p1>0x7E)
				return false;
		}
		if (nMessageSize>0x10000000)
		{
			printf("CMessageHeader::IsValid():nMessageSize too large %u\n", nMessageSize);
			return false;
		}
		return true;
	}
};
static const unsigned char pchIPv4[12]={0,0,0,0,0,0,0,0,0,0,0xff,0xff};
class CAddress
{
public:
	uint64 nServices;
	unsigned char pchReserved[12];
	unsigned int ip;
	unsinged short port;
	unsigned in nTime;
	unsigned int nLastFailed;
	CAddress()
	{
		nServices=0;
		memcpy(pchReserved, pchIPv4,sizeof(pchReserved));
		ip=0;
		port=DEFAULT_PORT;
		nTime=GetAdjustedTime();
		nLastFailed=0;
	}
	CAddress(unsigned int ipIn, unsigned short portIn=DEFAULT_PORT,uint64 nServicesIn=0)
	{
		nServices=nServicesIn;
		memcpy(pchReserved, pchIPv4, sizeof(pchReserved));
		ip=ipIn;
		port=portIn;
		nTime=GetAdjustedTime();
		nLastFailed=0;
	}
	explicit CAddress(const struct sockaddr_in& sockaddr, uint64 nServicesIn=0)
	{
		nServices=nServicesIn;
		memcpy(pchReserved, pchIPv4, sizeof(pchReserved));
		ip=sockaddr.sin_addr.s_addr;
		port=sockaddr.sin_port;
		nTime=GetAdjustedTime();
		nLastFailed=0;
	}
	explicit CAddress(const char* pszIn, uint64 nServicesIn=0)
	{
		nServices=nServicesIn;
		memcpy(pchReserved, pchIPv4,sizeof(pchReserved));
		ip=0;
		port=DEFAULT_PORT;
		nTime=GetAdjustedTime();
		nLastFailed=0;
		char psz[100];
		if(strlen(pszIn)>ARRAYLEN(psz)-1)
			return;
		strcpy(psz,pszIn);
		unsigned int a,b,c,d,e;
		if (sscanf(psz,"%u.%u.%u.%u.%u",&a,&b,&c,&d,&e)<4)
			return;
		char* pszPort=strchr(psz,':');
		if (pszPort)
		{
			*pszPort++='\0';
			port=htons(atoi(pszPort));
		}
		ip=inet_addr(psz);
	}
	IMPLEMENT_SERIALIZE
	(
	 	if (nType & SER_DISK)
		{
			READWRITE(nVersion);
			READWRITE(nTime);
		}
		READWRITE(nServices);
		READWRITE(FLATDATA(pchReserved));
		READWRITE(ip;
		READWRITE(port);
	)
	friend inline bool operator==(const CAddress& a, const CAddress& b)
	{
		return (memcmp(a.pchReserved, b.pchReserved, sizeof(a.pchReserved))==0 && 
				a.ip==b.ip&& 
				a.port==b.port);
	}
	friend inline bool operator<(const CAddress& a, const CAddress& b)
	{
		int ret=memcmp(a.pchReserved, b.pchReserved, sizeof(a.pchReserved));
		if (ret<0)
			return true;
		else if (ret==0)
		{
			if (ntohl(a.ip)<ntohl(b.ip))
				return true;
			else if (a.ip==b.ip)
				return ntohs(a.port)<ntohs(b.port);
		}
		return false;
	}
	vector<unsigned char> GetKey() const
	{
		CDataStream ss;
		ss.reserve(18);
		ss<< FLATDATA(pchReserved)<<ip<<port;
		#if defined(_MSC_VER)&&_MSC_VER<1300
		return vector<unsigned char>((unsigned char*)&ss.begin()[0],(unsigned char*)&ss.end()[0]);
		#else
		return vector<unsigned char>(ss.begin(),ss.end());
		#endif
	}
	struct sockaddr_in GetSockAddr() const
	{
		struct sockaddr_in sockaddr;
		sockaddr.sin_family=AF_INET;
		sockaddr.sin_addr.s_addr=ip;
		sockaddr.sin_port=port;
		return sockaddr;
	}
	bool IsIP4() const
	{
		return (memcmp(pchReserved, pchIPv4, sizeof(pchIPv4))==0);
	}
	bool IsRoutable() const
	{
		return !(GetByte(3)==10||(GetByte(3)==192&& GetByte(2)==168||GetByte(3)==127||GetByte(3)==0);
	}
	unsigned char GetByte(int n) const
	{
		return ((unsigned char*)&ip)[3-n];
	}
	string ToStringIPPort() const
	{
		return strprintf("%u.%u.%u.%u.%u",GetByte(3),GetByte(2),GetByte(1),GetByte(0),ntohs(port));
	}
	string ToStringIP() const
	{
		return strprintf("%u.%u.%u.%u",GetByte(3),GetByte(2),GetByte(1),GetByte(0));
	}
	string ToString() const
	{
		return strprintf("%u.%u.%u.%u.%u",GetByte(3),GetByte(2),GetByte(1),GetByte(0),ntohs(port));
	}
	string print() const
	{
		printf("CAddress(%s)\n", ToString().c_str());
	}
};
enum
{
	MSG_TX=1,
	MSG_BLOCK,
	MSG_REVIEW,
	MSG_PRODUCT,
	MSG_TABLE,
};
static const char* ppszTypeName[]=
{
	"ERROR",
	"tx",
	"block",
	"review",
	"product",
	"table",
};
class CInv
{
public:
	int type;
	uint256 hash;
	CInv()
	{
		type=0;
		hash=0;
	}
	CInv(int typeIn, const uint256& hashIn)
	{
		type=typeIn;
		hash=hashIn;
	}
	CInv(const string& strType, const uint256& hashIn)
	{
		int i;
		for (i=1;i<ARRAYLEN(ppszTypeName); i++)
		{
			if (strType==ppszTypeName[i])
			{
				type=i;
				break;
			}
		}
		if (i==ARRAYLEN(ppszTypeName))
			throw std::out_of_range(strprintf("CInv::CInv(string, uint256):unknown type '%s'", strType.c_str()));
		hash=hashIn;
	}
	IMPLEMENT_SERIALIZE
	{
		READWRITE(type);
		READWRITE(hash);
	}
	friend inline bool operator<(const CInv& a, const CInv& b)
	{
		return (a.type<b.type|| (a.type==b.type&&a.hash<b.hash));
	}
	bool IsKnownType() const
	{
		return (type>=1&& type<ARRAYLEN(ppszTypeName));
	}
	const char* GetCommand() const
	{
		if (!IsKnownType())
			throw std::out_of_range(strprintf("CInv::GetCommand():type=%unknown type", type));
		return ppszTypeName[type];
	}
	string ToString() const
	{
		return strprintf("%s %s", GetCommand(), hash.ToString().substr(0,14).c_str());
	}
	void print() const
	{
		printf("CInv(%s)\n", ToString().c_str());
	}
};
class CRequestTracker
{
public:
	void (*fn)(void*, CDataStream&);
	void* param1;
	explicit CRequestTracker(void (*fnIn)(void*, CDataStream&)=NULL, void* param1In=NULL)
	{
		fn=fnIn;
		param1=param1In;
	}
	bool IsNull()
	{
		return fn=NULL;
	}
};
extern bool fClient;
extern uint256 nLocalServices;
extern CAddress addrLocalHost;
extern CNode* pnodeLocalHost;
extern bool fShutdown;
extern array<bool, 10> vfThreadRunning;
extern vector<CNode*> vNodes;
extern CCriticalSection cs_vNodes;
extern map<vector<unsigned char>, CAddress> mapAddresses;
extern CCriticalSection cs_mapAddresses;
extern map<CInv, CDataStream>mapRelay;
extern deque<pair<int64,CInv>>vRelayExpiration;
extern CCriticalSection cs_mapRelay;
extern map<CInv, int64> mapAlreadyAskedFor;
extern CAddress addrProxy;




