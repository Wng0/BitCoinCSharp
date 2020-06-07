//复制与Bitcoin v0.1.5
//Wng0@sina.com
//2020-6-7
#include "headers.h"
#include "sha.h"
CCriticalSection cs_main;
map<uint256, CTransaction> mapTransactions;
CCriticalSection cs_mapTransactions;
unsigned int nTransactionsUpdated=0;
map<COutPoint, CInPoint> mapNextTx;
map<uint256, CBlockIndex*> mapBlockIndex;
const uint256 hashGenesisBlock("0x000000000019d6689c085ae165831e934ff7     63ae46a2a6c172b3f1b60a8ce26f");
CBlockIndex* pindexGenesisBlock=NULL;
int nBestHeight=-1;
uint256 hashBestChain=0;
CBlockIndex* pindexBest=NULL;
map<uint256, CBlock*> mapOrphanBlocks;
multimap<uint256, CBlock*> mapOrphanBlocksByPrev;
map<uint256, CWalletTx> mapWallet;
vector<pair<uint256, bool>>vWalletUpdated;
CCriticalSection cs_mapWallet;
map<vector<unsigned char>,CPrivKey> mapKeys;
map<uint160, vector<unsigned char>>mapPubKeys;
CCriticalSection cs_mapKeys;
CKey keyUser;
string strSetDataDir;
int nDropMessagesTest=0;
int fGenerateBitcoins;
int64 nTransactionFee=0;
CAddress addrIncoming;
bool AddKey(const CKey& key)
{
	CRITICAL_BLOCK(cs_mapKeys)
	{
		mapKeys[key.GetPubKey()]=key.GetPrivKey();
		mapPubKeys[Hash160(key.GetPubKey())]=key.GetPubKey();
	}
	return CWalletDB().WriteKey(key.GetPubKey(), key.GetPrivKey());
}
vector <unsigned char> GenerateNewKey()
{
	CKey key;
	key.MakeNewKey();
	if (!AddKey(key))
		throw runtime_error("GenerateNewKey():AddKey failed\n");
	return key.GetPubKey();
}
bool AddToWallet(const CWalletTx& wtxIn)
{
	uint256 hash=wtxIn.GetHash();
	CRITICAL_BLOCK(cs_mapWallet)
	{
		pair<map<uint256, CWalletTx>::iterator,bool>ret= mapWallet.insert ( make_pair( hash, wtxIn));
		CWalletTx& wtx=(*ret.first).second;
		bool fInsertedNew=ret.second;
		if (fInsertedNew)
			wtx.nTimeReceived=GetAdjustedTime();
		printf("AddToWallet %s  %s\n", wtxIn.GetHash().ToString().substr(0, 6).c_str(), fInsertedNew ? "new" : "update");
		if (!fInsertedNew)
		{
			bool fUpdated=false;
			if (wtxIn.hashBlock!=0&& wtxIn.hashBlock != wtx.hashBlock)
			{
				wtx.hashBlock=wtxIn.hashBlock;
				fUpdate=true;
			}
			if (wtxIn.nIndex!=-1 && (wtxIn.vMerleBranch != wtx.vMerkleBranch || wtxIn.nIndex != wtx.nIndex))
			{
				wtx.vMerkleBranch=wtxIn.vMerkleBranch;
				wtx.nIndex=wtxIn.nIndex;
				fUpdated=true;
			}
			if (wtxIn.fFromMe && wtxIn.fFromMe !=wtx.fFromMe)
			{
				wtx.fFromMe=wtxIn.fFromMe;
				fUpdated=true;
			}
			if (wtxIn.fSpent && wtxIn.fSpent != wtx.fSpent)
			{
				wtx.fSpent=wtxIn.fSpent;
				fUpdated=true;
			}
			if (!fUpdated)
				return true;
		}
		if (!wtx.WriteToDisk())
			return false;
		vWalletUpdated.push_back(make_pair(hash, fInsertedNew));
	}
	MainFrameRepaint();
	return true;
}
bool AddToWalletIfMine(const CTransaction& tx, const CBlock* pblock)
{
	if(tx.IsMine()||mapWallet.count(tx.GetHash()))
	{
		CWalletTx wtx(tx);
		if (pblock)
			wtx.SetMerkleBranch(pblock);
		return AddToWallet(wtx);
	}
	return true;
}
bool EraseFromWallet(uint256 hash)
{
	CRITICAL_BLOCK(cs_mapWallet)
	{
		if (mapWallet.erase(hash))
			CWalletDB().EraseTx(hash);
	}
	return true;
}
void AddOrphanTx(const CDataStream& vMsg)
{
	CTransaction tx;
	CDataStream(vMsg)>>tx;
	uint256 hash = tx.GetHash();
	if (mapOrphanTransactions.count(hash))
		return;
	CDataStream* pvMsg=mapOrphanTransactions[hash]=new CDataStream (vMsg);
	193
}
void EraseOrphanTx(uint256 hash)
{
199
}
bool CTxIn::IsMine() const
{
233
}
int64 CTxIn::GetDebit() const
{
249
}
int64 CWalletTx::GetTxTime() const
{
265
}
int CMerkleTx::SetMerkleBranch(const CBlock* pblock)
{
	288
}
void CWalletTx::AddSupportingTransactions(CTxDB& txdb)
{
341
}
bool CTransaction::AcceptTransaction(CTxDB& txdb, bool fCheckInputs, bool* pfMissingInputs)
{
407
}
bool CTransaction::AddToMemoryPool()
{
483
}
bool CTransaction::RemoveFromMemoryPool()
{
499
}
int CMerkleTx::GetDepthInMainChain() const
{
517
}
int CMerkleTx::GetBlocksToMaturity() const
{
542
}
bool CMerkleTx::AcceptTransaction(CTxDB& txdb, bool fCheckInputs)
{
550
}
bool CWalletTx::AcceptWalletTransaction(CTxDB& txdb, bool fCheckInputs)
{
566
}
void ReacceptWalletTransactions()
{
586
}
void CWalletTx::RelayWalletTransaction(CTxDB& txdb)
{
	foreach(const CMerkleTx& tx, vtxPrev)
	602
}
void RelayWalletTransactions()
{
622
}
void CBlock::ReadFromDisk(const CBlockIndex* pblockindex, bool fReadTransactions)
{
664
}
uint256 GetOrphanRoot(const CBlock* pblock)
{
670
}
int64 CBlock::GetBlockValue(int64 nFees) const
{
677
}
unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast)
{
687
}
bool CTransaction::DisconnectInputs(CTxDB& txdb)
{
741
}
bool CTransaction::ConnectInputs(CTxDB& txdb, map<uint256, CTxIndex>& mapTestPool, CDiskTxPos posThisTx, int nHeight, int64& nFees, bool fBlock, bool fMiner, int64 nMinFee)
{
774
}
bool CTransaction::ClientConnectInputs()
{
875
}
bool CBlock::DisconnectBlock(CTxDB& txdb, CBlockIndex* pindex)
{
920
}
bool CBlock::ConnectBlock(CTxDB& txdb, CBlockIndex* pindex)
{
939
}
bool Reorganize(CTxDB& txdb, CBlockIndex* pindexNew)
{
976
}
bool CBlock::AddToBlockIndex(unsigned int nFile, unsigned int nBlockPos)
{
1074
}
bool CBlock::CheckBlock() const
{
1156
}
bool CBlock::AcceptBlock()
{
1194
}
bool ProcessBlock(CNode* pfrom, CBlock* pblock)
{
1240
}
template<typename Stream>
bool ScanMessageStart(Stream& s)
{
1308
}
string GetAppDir()
{
1348
}
bool CheckDiskSpace(int64 nAdditionalBytes)
{
1383
}
FILE* OpenBlockFile(unsigned int nFile, unsigned int nBlockPos, const char* pszMode)
{
1409
}
static unsigned int nCurrentBlockFile=1;
FILE* AppendBlockFile(unsigned int& nFileRet)
{
1429
}
bool LoadBlockIndex(bool fAllowNew)
{
1448
}
void PrintBlockTree()
{
1526
}
bool AlreadyHave(CTxDB& txdb, const CInv& inv)
{
1622
}
bool ProcessMessage(CNode* pfrom)
{
1641
}
bool ProcessMesage(CNode* pfrom, string strCommand, CDataStream& vRecv)
{
1720
}
bool SendMessages(CNode* pto)
{
2088
}
int FormatHashBlocks(void* pbuffer, unsigned int len)
{
2170
}
using CryptoPP::ByteReverse;
static int detectlittleendian=1;
void BlockSHA256(const void* pin, unsigned int nBlocks, void* pout)
{
2188
}
bool BitcoinMiner()
{
2215
}
int64 GetBalance()
{
2418
}
bool SelectCoins(int64 nTargetValue, set<CWalletTx*>& setCoinsRet)
{
2442
}
bool CreateTransaction(CScript scriptPubKey, int64 nValue, CWalletTx& wtxNew, int64& nFeeRequiredRet)
{
2546
}
bool CommitTransactionSpent(const CWalletTx& wtxNew)
{
2629
}
bool SendMoney(CScript scriptPubKey, int64 nValue, CWalletTx& wtxNew)
{
2658
}

