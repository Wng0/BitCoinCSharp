// 复制于 bitcoin v 0.1.5
extern bool RecvLine(SOCKET hSocket, string& strLine);
extern void ThreadIRCSeed(void* parg);
extern bool fRestartIRCSeed;
extern map<vector<unsigned char>, CAddress> mapIRCAddresses;
extern CCriticalSection cs_mapIRCAddresses;
