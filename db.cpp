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
		
