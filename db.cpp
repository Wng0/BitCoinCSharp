//复制于bitcoin v0.1.5
#include "headers.h"
static CCriticalSection cs_db;
static bool fDbEnvInit=false;
DbEnv dbenv(0);
static map<string, int> mapFileUseCount;
class CDBInit
{
public:
	CDBInit(){}
	~CDBInit()
	{
		if(fDbEnvInit)
		{
			dbenv.close(0);
			fDbEnvInit=false;
		}
	}
}
istance_of_cdbinit;
CDB::CDB(const char* pszFile, const char* pszMode, bool fTxn): pdb(NULL)
{
	int ret;
	if (pszFile==NULL)
		return;
	bool fCreat=strchr(pszMode,'c');
	bool fReadOnly=(!strchr(pszMode,'+')&&!strchr(pszMode,'w'));
	unsigned int nFlags=DB_THREAD;
	if (fCreate)
		nFlags|=DB_CREATE;
	else if (fReadOnly)
		nFlags|=DB_RDONLY;
	if(!fReadOnly||fTxn)
		nFlags|=DB_AUTO_COMMIT;
	CRITICAL_BLOCK(cs_db)
	{
		if (!fDbEnvInit)
		{
			string strAppDir=GetAppDir();
			string strLogDir=strAppDir+"\\database";
			_mkdir(strLogDir.c_str());
			printf("dbenv.open strAppDir=%s\n", strAppDir.c_str());
			dbenv.set_lg_dir(strLogDir.c_str());
			dbenv.set_lg_max(10000000);
			dbenv.set_lk_max_locks(10000);
			dbenv.set_lk_max_objects(10000);
			dbenv.set_errfile(fopen("db.log","a"));
			ret=dbenv.open(strAppDir.c_str(),
					DB_CREATE		|
					DB_INIT_LOCK	|
					DB_INIT_LOG		|
					DB_INIT_MPOOL	|
					DB_INIT_TXN		|
					DB_THREAD		|
					DB_PRIVATE		|
					DB_RECOVER,
					0);
			if (ret>0)
				throw retime_error(strprintf("CDB():error %d opening database environment\n",ret));
			fDbEnvInit=true;
		}
		strFile=pszFile;
		++mapFileUseCount[strFile];
	}
	pdb=new Db(&dbenv,0);
	ret=pdb->open(NULL,
					pszFile,
					"main",
					DB_BTREE,
					nFlags,
					0);
	if (ret>0)
	{
		delete pdb;
		pdb=NULL;
		CRITICAL_BLOCK(cs_db)
			--mapFileUseCount[strFile];
		strFile="";
		throw runtime_error(strprintf("CDB():can't open database file %s, error %d\n",pszFile,ret));
	}
	if (fCreate && !Exists(string("version")))
		WriteVersion(VERSION);
	RandAddSeed();
}
void CDB::Close()
{
	if (!pdb)
		return;
	if(!vTxn.empty())
		vTxn.front()->abort();
	vTxn.clear();
	pdb->close(0);
	delete pdb;
	pdb=NULL;
	dbenv.txn_checkpoint(0,0,0);
	CRITICAL_BLOCK(cs_db)
		--mapFileUseCount[strFile];
	RandAddSeed();
}
void DBFlush(bool fShutdown)
{
	printf("DBFlush(%s)\n",fShutdown?"true":"false");
	CRITICAL_BLOCK(cs_db)
	{
		dbenv.txn_checkpoint(0,0,0);
		map<string, int>::iterator mi=mapFileUseCount.begin();
		while (mi!=mapFileUseCount.end())
		{
			string strFile=(*mi).first;
			int nRefCount=(*mi).second;
			if (nRefCount==0)
			{
				dbenv.lsn_reset(strFile.c_str(),0);
				mapFileUseCount.erase(mi++);
			}
			else
				mi++;
		}
		if(fShutdown)
		{
			char** listp;
			if (mapFileUseCount.empty())
				dbenv.log_archive(&listp, DB_ARCH_REMOVE);
			dbenv.close(0);
			fDbEnvInit=false;
		}
	}
}
bool CTxDB::ReadTxIndex(uint256 hash,CTxIndex& txindex)
{
	assert(!fClient);
	txindex.SetNull();
	return Read(make_pair(string("tx"),hash),txindex);
}
bool CTxDB::UpdateTxIndex(uint256 hash, const CTxIndex& txindex)
{
	assert(!fClient);
	return Write(make_pair(string("tx"),hash),txindex);
}
bool CTxDB::AddTxIndex(const CTransaction& tx,const CDiskTxPos& pos, int nHeight)
{
	assert(!fClient);
	uint256 hash=tx.GetHash();
	CTxIndex txindex(pos, tx.vout.size());
	return Write(make_pair(string("tx"),hash),txindex);
}
bool CTxDB::EraseTxIndex(const CTransaction& tx)
{
	assert(!fClient);
	uint256 hash=tx.GetHash();
	return Erase(make_pair(string("tx"),hash));
}
bool CTxDB::ContainsTx(uint256 hash)
{
	assert(!fClient);
	return Exists(make_pair(string("tx"),hash));
}
bool CTxDB::ReadOwnerTxes(uint160 hash160, int nMinHeight, vector<CTransaction>& vtx)
{
	assert(!fClient);
	vtx.clear();
	Dbc* pcursor=GetCursor();
	if(!pcursor)
		return false;
	unsigned int fFlags=DB_SET_RANGE;
	loop
	{
		CDataStream ssKey;
		if(fFlags==DB_SET_RANGE)
			ssKey<<string("owner")<<hash160<<CDiskTxPos(0,0,0);
		CDataStream ssValue;
		int ret=ReadAtCursor(pcursor,ssKey,ssValue,fFlags);
		fFlags=DB_NEXT;
		if(ret==DB_NOTFOUND)
			break;
		else if (ret!=0)
			return false;
		string strType;
		uint160 hashItem;
		CDiskTxPos pos;
		ssKey>>strType>>hashItem>>pos;
		int nItemHeight;
		ssValue>>nItemHeight;
		if(strType!="owner"||hashItem!=hash160)
			break;
		if(nItemHeight>=nMinHeight)
		{
			vtx.resize(vtx.size()+1);
			if (!vtx.back().ReadFromDisk(pos))
				return false;
		}
	}
	return true;
}
bool CTxDB::ReadDiskTx(uint256 hash, CTransaction& tx, CTxIndex& txindex)
{
	assert(!fClient);
	tx.SetNull();
	if (!ReadTxIndex(hash,txindex))
		return false;
	return (tx.ReadFromDisk(txindex.pos));
}
bool CTxDB::ReadDiskTx(uint256 hash,CTransaction& tx)
{
	CTxIndex txindex;
	return ReadDiskTx(hash, tx, txindex);
}
bool CTxDB::ReadDiskTx(COutPoint outpoint, CTransaction& tx, CTxIndex& txindex)
{
	return ReadDiskTx(outpoint.hash, tx, txindex);
}
bool CTxDB::ReadDiskTx(COutPoint outpoint, CTransaction& tx)
{
	CTxIndex txindex;
	return ReadDiskTx(outpoint.hash,tx,txindex);
}
bool CTxDB::WriteBlockIndex(const CDiskBlockIndex& blockindex)
{
	return Write(make_pair(string("blockindex"),blockindex.GetBlockHash()),blockindex);
}
bool CTxDB::EraseBlockIndex(uint256 hash)
{
	return Erase(make_pair(string("blockindex"),hash));
}
bool CTxDB::ReadHashBestChain(uint256& hashBestChain)
{
	return Read(string("hashBestChain"),hashBestChain);
}
bool CTxDB::WriteHashBestChain(uint256 hashBestChain)
{
	return Write(string("hashBestChain"),hashBestChain);
}
CBlockIndex* InsertBlockIndex(uint256 hash)
{
	if (hash==0)
		return NULL;
	map<uint256, CBlockIndex*>::iterator mi=mapBlackIndex.find(hash);
	if (mi!=mapBlackIndex.end())
		return (*mi).second;
	CBlockIndex* pindexNew=new CBlockIndex();
	if (!pindexNew)
		throw runtime_error("LoadBlockIndex():new CBlockIndex failed");
	mi=mapBlockIndex.insert(make_pair(hash,pindexNew)).first;
	pindexnew->phashBlock=&((*mi).first);
	return pindexNew;
}
bool CTxDB::LoadBlockIndex()
{
	Dbc* pcursor=GetCursor();
	if (!pcursor)
		return false;
	unsigned int fFlags=DB_SET_RANGE;
	loop
	{
		CDataStream ssKey;
		if (fFlags==DB_SET_RANGE)
			ssKey<<make_pair(string("blockindex"),uint256(0));
		CDataStream ssValue;
		int ret=ReadAtCursor(pcursor,ssKey,ssValue,fFlags);
		fFlags=DB_NEXT;
		if (ret==DB_NOTFOUND)
			break;
		else if (ret!=0)
			return false;
		string strType;
		ssKey>>strType;
		if (strType=="blockindex")
		{
			CDiskBlockIndex diskindex;
			ssValue>>diskindex;
			CBlockIndex* pindexNew		=InsertBlockIndex(diskindex.GetBlockHash());
			pindexNew->pprev			=InsertBlockIndex(diskindex.hashPrev);
			pindexNew->pnext			=InsertBlockIndex(diskindex.hashNext);
			pindexNew->nFile			=diskindex.nFile;
			pindexNew->nBlockPos		=diskindex.nBlockPos;
			pindexNew->nHeight			=diskindex.nHeight;
			pindexNew->nVersion			=diskindex.nVersion;
			pindexNew->hashMerkleRoot	=diskindex.hashMerkleRoot;
			pindexNew->nTime			=diskindex.nTime;
			pindexNew->nBits			=diskindex.nBits;
			pindexNew->nNonce			=diskindex.nNonce;
			if (pindexGenesisBlock==NULL&&diskindex.GetBlockHash()==hashGenesisBlock)
				pindexGenesisBlock=pindexNew;
		}
		else
		{
			break;
		}
	}
	if (!ReadHashBestChain(hashBestChain))
	{
		if (pindexGenesisBlock==NULL)
			return true;
		return error("CTxDB::LoadBlockIndex():hashBestChain not found\n");
	}
	if (!mapBlockIndex.count(hashBestChain))
		return error("CTxDB::LoadBlockIndex():blockindex for hashBestChain not found\n");
	pindexBest=mapBlockIndex[hashBestChain];
	nBestHeight=pindexBest->nHeight;
	printf("LoadBlockIndex():hashBestChain=%s height=%d\n",hashBestChain.ToString().substr(0,14).c_str(),nBestHeight);
	return true;
}
bool CAddrDB::WriteAddress(const CAddress& addr)
{
	return Write(make_pair(string("addr"),addr.GetKey()),addr);
}
bool CAddrDB::LoadAddresses()
{
	CRITICAL_BLOCK(cs_mapIRCAddresses)
	CRITICAL_BLOCK(cs_mapAddresses)
	{
		CAutoFile filein=fopen("addr.txt", "rt");
		if (filein)
		{
			try
			{
				char psz[1000];
				while (fgets(psz,sizeof(psz),filein))
				{
					CAddress addr(psz,NODE_NETWORK);
					if (addr.ip!=0)
					{
						AddAddress(*this,addr);
						mapIRCAddresses.insert(make_pair(addr.GetKey().addr));
					}
				}
			}
			catch(...){}
		}





		
