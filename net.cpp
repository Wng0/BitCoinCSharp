//Copy from Bitcoin v0.1.5
#include "headers.h"
#include <winsock2.h>
void ThreadMessageHandler2(void* parg);
void ThreadSocketHandler2(void* parg);
void ThreadOpenConnections2(void* parg);
bool fClient=false;
uint64 nLocalServices=(fClient?0:NODE_NETWORK);
CAddress addrLocalHost(0,DEFAULT_PORT,nLocalServices);
CNode nodeLoalHost(INVALID_SOKET,CAddress("127.0.0.1",nLoalServices));
CNode* pnodeLocalHost=&nodeLocalHost;
bool fShutdown=false;
array<bool,10> vfThreadRunning;
vector<CNode*> vNodes;
CCriticalSection cs_vNodes;
map<vector<unsigned char>, CAddress> mapAddresses;
CCriticalSection cs_mapAddresses;
map<CInv, CDataStream> mapRelay;
deque<pair<int64, CInv> vRelayExpiration;
CCriticalSection cs_mapRelay;
map<CInv,int64> mapAlreadyAskedFor;
CAddress addrProxy;
bool ConnectSocket(const CAddress& addrConnect, SOCKET& hSocketRet)
{
	hSocketRet=INVALID_SOCKET;
	SOCKET hSocket=socket(AF_INET,SOCK_STREAM, IPPROO_TCP);
	if (hSocket==INVALID_SOCKET)
		return false;
	bool fRoutable=!(addrConnect.GetByte(3)==10||(addrConnect.GetByte(3)==192&&addrConnect.GetByte(2)==168));
	bool fProxy=(addrProxy.ip&&fRoutable);
	struct sockaddr_in sockaddr=(fProxy?addrProxy.GetSockAdr():addrConnect.GetSockAddr());
	if (connect(hSocket,(struct sockadr*)&sockaddr, sizeof(sockaddr))==SOCKET_ERROR)
	{
		closesocket(hSocket);
		return false;
	}
	if (fProxy)
	{
		printf("Proxy connecting to %s\n", addrConnect.ToString().c_str());
		char pszSocks4IP[]="\4\1\0\0\0\0\0\0user";
		memcpy(pszSocks4IP+2, &addrConnect.port,2);
		memcpy(pszSocks4IP+4, &addrConnect.ip,4);
		char* pszSocks4=pszSocks4IP;
		int nSize=sizeof(pszSocks4IP);
		int ret=send(hSocket, pszSocks4, nSize,0);
		if (ret!=nSize)
		{
			closesocket(hSocket);
			return error("Error sending to proxy\n");
		}
		char pchRet[8];
		if (recv(hSocket, pchRet, 8,0)!=8)
		{
			closesocket(hSocket);
			return error("Error reading proxy response\n");
		}
		if (pchRet[1]!=0x5a)
		{
			cloasesocket(hSocket);
			return error("Proxy returned error %d\n",pchRet[1]);
		}
		printf("Proxy connection established %s\n",addrConnect.ToString().c_str());
	}
	hSocketRet=hSocket;
	return true;
}
bool GetMyExternalIP2(const CAddress& addrConnect, const char* pszGet, const char* pszKeyword, unsigned int& ipRet)
{
	SOCKET hSocket;
	if (!ConnectSocket(addrConnect, hSocket))
		return error("GetMyExternalIP():connection to %s failed\n", addrConnect.ToString().c_str());
	send (hSocket, pszGet, strlen(pszGet),0);
	string strLine;
	while (RecvLine(hSocket, strLine))
	{
		if(strLine.empty())
		{
			loop
			{
				if (!RecvLine(hSocket,strLine))
				{
					closesocket(hSocket);
					return false;
				}
				if (strLine.find(pszKeyword)!=-1)
				{
					strLine=strLine.substr(strLine.find(pszKeyword)+strlen(pszKeyword));
					break;
				}
			}
			closesocket(hSocket);
			if (strLine.find("<"))
				strLine=strLine.substr(0,strLine.find("<"));
			strLine=strLine.substr(strspn(strLine.c_str()," \t\n\r"));
			strLine=wxString(strLine).Trim();
			CAddress addr(string.c_str());
			printf("GetMyExternalIP() received [%s] %s\n", strLine.c_str(),addr.ToString().c_str());
			if (addr.ip==0||!addr.IsRoutable())
				return false;
			ipRet=addr.ip;
			return true;
		}
	}
		closesocket(hSocket);
		return error("GetMyExternalIP():connection cloase\n");
}
bool GetMyExternalIp(unsigned int& ipRet)
{
	CAddress addrConnect;
	char* pszGet;
	char* pszKeyword;
	for (int nLookup=0;nLookup<=1;nLookup++)
	for (int hHost=1;nHost<=2;nHost++)
	{
		if (nHost==1)
		{
			addrConnect=CAddress("70.86.96.218:80");
			if (nLookup==1)
			{
				struct hostent* phostent=gethostbyname("www.ipaddressworld.com");
				if (phostent && phostent->h_addr_list && phostent->h_addr_list[0])
					addrConnect=CAddress(*(u_long*)phostent->h_addr_list[0], htons(80));
			}
			pszGet="GET /ip.php HTTP/1.1\r\n"
			"Host: www.ipaddressworld.com\r\n"
			"User-Agent:Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)\r\n"
			"Connection:close\r\n"
			"\r\n";
			pszKeyword="IP:";
		}
		else if (nHost==2)
		{
			addrConnect=CAddress("208.78.68.70:80");
			if (nLookup==1)
			{
				struct hostent* phostent=gethostbyname("checkip.dyndns.org");
				if (phostent && phostent->h_addr_list && phostent->h_addr_list[0])
					addrConnect=CAddress(*(u_long*)phostent->h_addr_list[0],htons(80));
			}
			pszGet="GET /HTTP/1.1\r\n"
			"Host: checkip.dyndns.org\r\n"
			"User-Agent: Mozilla/4.0(compatible; MIE 7.0; Windows NT 5.1)\r\n:
			"Connection: close\r\n"
			"\r\n";
			pszKeyword="Address:";
		}
		if (GetMyExternalIP2(addrConnect, pszGet,pszKeyword,ipRet))
			return true;
	}
	return false;
}
bool AddAddress(CAddrDB& addrdb, const CAddress& addr)
{
	if (!addr.IsRoutable())
		return false;
	if (addr.ip==addrLocalHost.ip)
		return false;
	CRITICAL_BLOCK(cs_mapAddresses)
	{
		map<vector<unsigned char>, CAddress>::iterator it=mapAddresses.find(addr.GetKey());
		if (it==mapAddresses.end())
		{
			mapAddresses.insert(make_pair(addr.GetKey(),addr));
			addrdb.WriteAddress(addr);
			return true;
		}
		else
		{
			CAddress& addrFound=(*it).second;
			if ((addrFound.nServices| addr.nServices)!=addrFound.nServices)
			{
				addrFound.nServices|=addr.nServices;
				addrdb.WriteAddress(addrFound);
				return true;
			}
		}
	}
	return false;
}
void AbandonRequests(void (*fn)(void*, CDataStream&), void* param1)
{
	CRITICAL_BLOCK(cs_vNodes)
	{
		foreach(CNode* pnode, vNodes)
		{
			CRITICAL_BLOCK(pnode->cs_mapRequests)
			{
				for (map<uint256, CRequestTracker>::iterator mi=pnnode->mapRequests.begin();mi!=pnode->mapRequests.end();)
				{
					CRequestTracker& tracker=(*mi).second;
					if (tracker.fn==fn&& tracker.param1==param1)
						pnode->mapRequests.erase(mi++);
					else
						mi++;
				}
			}
		}
	}
}
bool AnySubscribed(unsigned int nChannel)
{
	if (pnodeLocalHost->IsSubscribed(nChannel))
		return true;
	CRITICAL_BLOCK(cs_vNodes)
	foreach (CNode* pnode, vNodes)
	if (pnode->IsSubscribed(nChannel))
		return true;
	return false;
}
bool CNode::IsSubscribed(unsigned int nChannel)
{
	if (nChannel>=vfSubscribe.size())
		return false;
	return vfSubscribe[nChannel];
}
void CNode::Subscribe(unsigned int nChannel, unsigned int nHops)
{
	if (nChannel>=vfSubscribe.size())
		return;
	if (!AnySubscribed(nChannel))
		CRITICAL_BLOCK(cs_vNodes)
		foreach(CNode* pnode,vNodes)
		if (pnode !=this)
			pnode->PushMessage("subscribe", nChannel,nHops);
}
	vfSubscribe[nChannel]=true;
}
void CNode::CancelSubscribe(unsigned int nChannel)
{
	if (nChannel>=vfSubscribe.size())
		return;
	if (!vfSubscribe[nChannel])
		return;
	vfSubscribe[nChannel]=false;
	if (!AnySubscribed(nChannel))
	{
		CRITICAL_BLOCK(cs_vNodes)
		foreach(CNode* pnode,vNodes)
		if (pnode !=this)
			pnode->PushMessage("sub-cancel", nChannel);
		if (nChannel==MSG_PRODUCT)
			CRITICAL_BLOCK(cs_mapProducts)
			mapProducts.clear();
	}
}
CNode* FindNode(unsigned int ip)
{
	CRITICAL_BLOCK(cs_vNodes)
	{
		foreach(CNode* pnode, vNodes)
		if (pnode->addr.ip==ip)
			return (pnode);
	}
	return NULL;
}
CNode* FindNode(CAddress addr)
{
	CRITICAL_BLOCK(cs_vNodes)
	{
		foreach(CNode* pnode, vNodes)
		if (pnode->addr==addr)
			return (pnode);
	}
	return NULL;
}
CNode* ConnectNode(CAddress addrConnect, int64 nTimeout)
{
	if (addrConnect.ip==addrLocalHost.ip)
		return NULL;
	CNode* pnode=FindNode(addrConnect.ip);
	if(pnode)
	{
		if(nTimeout!=0)
			pnode->AddRef(nTimeout);
		else
			pnode->AddRef();
		return pnode;
	}
	printf("trying %s\n", addrConnect.ToString().C_str());
	SOCKET hSocket;
	if (ConnectSocket(addrConnect, hSocket))
	{
		printf("connected %s\n", addrConnect.ToString().c_str());
		u_long nOne=1;
		if (ioctlsocket(hSocket, FIONBIO,&nOne)==SOCKET_ERROR)
			printf("ConnectSocket(): ioctlsocket nonblocking setting failed, error %d\n", WSAGetLastError());
		CNode* pnode=new CNode(hSocket, addrConnect, false);
		if (nTimeout!=0)
			pnode->AddRef(nTimeout);
		else
			pnode->AddRef();
		CRITICAL_BLOCK(cs_vNodes)
			vNodes.push_back(pnode);
		CRITICAL_BLOCK(cs_mapAddresses)
			mapAddresses[addrConnect.GetKey()].nLastFailed=0;
		return pnode;
	}
	else
	{
		CRITICAL_BLOCK(cs_mapAddresses)
		mapAddresses[addrConnect.GetKey()].nLastFailed=GetTime();
		return NULL;
	}
}
void CNode::Disconnect()
{
	printf("disconnecting node %s\n", addr.ToString().c_str());
	closesocket(hSocket);
	if(!fInbound && nVersion==0)
		CRITICAL_BLOCK(cs_mapAddresses)
		mapAddresses[addr.GetKey()].nLastFailed=GetTime();
	CRITICAL_BLOCK(cs_mapProducts)
	for(map<uint256,CProduct>::iterator mi=mapProducts.begin();mi!=mapProducts.end();)
		AdvertRemoveSource(this,MSG_PRODUCT,0,(*(mi++)).second);
	for(unsigned int nChannel=0;nChannel<vfSubscribe.size();nChannel++)
		if(vfSubscribe[nChannel])
			CancelSubscribe(nChannel);
}





























