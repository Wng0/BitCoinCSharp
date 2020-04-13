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
	char pchCommand[COMMAND_ 
