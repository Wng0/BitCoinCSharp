//重写bitcoin 0.1.5
#include <db_cxx.h>
class CTransaction;
class CTxIndex;
class CDiskBlockIndex;
class CDiskTxPos;
class COutPoint;
class CUser;
class CReview;
class CAddress;
class CWalletTx;
extern map<string, string> mapAddressBook;
extern bool fClient;
extern DbEnv dbenv;
extern void DBFlush(bool fShutdown);
class CDB
{
protacted:
	DB* pdb;
	string strFile;
	vector<DbTxn*> vTxn;
	explicit CDB(const char* pszFile, const char* pszMode="r+", bool fTxn=false);
	~CDB() {Close();}
public:
	void Close();
private:
	CDB(const CDB&);
	void operator=(const CDB&);
protacted:
	template<typename K, typename T>
	bool Read(const K& key, T& value);
	{
		if (!pdb)
			return false;
		CDataStream ssKey(SER_DISK);
		ssKey.reserve(1000);
		ssKey<<key;
		Dbt datKey(&ssKey[0],ssKey.size());
		Dbt datValue;
		datValue.set_flags(DB_DBT_MALLOC);
		int ret=pdb->get(GetTxn(),&datKey,&datValue,0);
		memset(datKey.get_data(),0,datKey.get_size());
		if (datValue.get_data()==NULL)
			return false;
		CDataStream ssValue((char*)datValue.get_data(), (char*)datValue.get_data()+datValue.get_size(),SER_DISK);
		ssValue>>value;
		memset(datValue.get_data(),0,datValue.get_size());
		free(datValue.get_data());
		return(ret==0);
	}
	template<typename K, typename T>
	bool Write(const K& key, const T& value, bool fOverwrite=true)
	{
		if (!pdb)
			return false;
		CDataStream ssKey(SER_DISK);
		ssKey.reserve(1000);
		ssKey<<key;
		Dbt datKey(&ssKey[0],ssKey.size());
		CDataStream ssValue(SER_DISK);
		ssValue.reserve(10000);
		ssValue<<value;
		Dbt datValue(&ssValue[0],ssValue.size());
		int ret=pdb->put(GetTxn(),&datKey, &datValue,(fOverwrite?0:DB_NOOVERWRITE));
		memset(datKey.get_data(),0,datKey.get_size());
		memset(datValue.get_data(),0,datValue.get_size());
		return (ret==0);
	}
	template<typename K>
	bool Erase(const K& key)
	{
		if (!pdb)
			return false;
		CDataStream ssKey(SER_DISK);
		ssKey.reserve(1000);
		ssKey<<Key;
		Dbt datKey(&ssKey[0],ssKey.size());
		int ret =pdb->del(GetTxn(),&datKey,0);
		memset(datKey.get_data(),0,datKey.get_size());
		return (ret==0||ret==DB_NOTFOUND);
	}
	template<typename K> bool Exists(const K& key)
	{
		if(!pdb)
			return false;
		CDataStream ssKey(SER_DISK);
		ssKey.reserve(1000);
		ssKey<<Key;
		Dbt datKey(&ssKey[0],ssKey.size());
		int ret=pdb->exists(GetTxn(),&dtaKey,0);
		memset(datKey.get_data(),0,datKey.get_size());
		return (ret==0);
	}
	Dbc* GetCursor()
	{
		if(!pdb)
			return NULL;
		Dbc* pcursor=NULL;
		int ret=pdb->cursor(NULL, &pcursor,0);
		if (ret!=0)
			return NULL;
		return pcursor;
	}
	
	int ReadAtCursor(Dbc* pcursor,CDataStream& ssKey, CDataStream& ssValue, unsigned int fFlags=DB_NEXT)
	{
		Dbt datKey;
		if (fFlag==DB_SET|| fFlags==DB_SET_RANGE|| fFlags==DB_GET_BOTH || fFlags==DB_GET_BOTH_RANGE)
		{
			datKey.set_data(&ssKey[0]);
			datKey.set_size(ssKey.size());
		}
		Dbt datValue;
		if(fFlags==DB_GET_BOTH||fFlags==DB_GET_BOTH_RANGE)
		{
			datValue.set_data(&ssValue[0]);
			datValue.set_size(ssValue.size());
		}
		datKey.set_flags(DB_DBT_MALLOC);
		datValue.set_flags(DB_DBT_MALLOC);
		int ret=pcursor->get(&datKey, &datValue,fFlags);
		if (ret!=0)
			return ret;
		else if (datKey.get_data()==NULL||datValue.get_data()==NULL)
			return 99999;
		ssKey.SetType(SER_DISK);
		ssKey.clear();
		ssKey.write((char*)datkey.get_data(),datKey.get_size());
		ssValue.SetType(SER_DISK);
		ssValue.clear();
		ssValue.write((char*)datValue.get_data(),datValue.get_size());
		memset(datKey.get_data(),0,datKey.get_size());
		memset(datValue.get_data(),0,datValue.get_size());
		free(datKey.get_data());
		free(datValue.get_data());
		return 0;
	}
	DbTxn* GetTxn()
	{
		if (!vTxn.empty())
			return vTxn.back();
		else
			return NULL;
	}
public:
	bool TxnBegin()
	{
		if(!pdb)
			return false;
		DbTxn* ptxn=NULL;
		int ret=dbenv.txn_begin(GetTxn(),&ptxn,0);
		if (!ptxn||ret!=0)
			return false;
		vTxn.push_back(ptxn);
		return true;
	}
	bool TxnCommit()
	{
		if (!pdb)
			return false;
		if (vTxn.empty())
			return false;
		int ret=vTxn.back()->commit(0);
		vTxn.pop_back();
		return (ret==0);
	}
	bool TxnAbort()
	{
		if (!pdb)
			return false;
		if(vTxn.empty())
			return false;
		int ret=vTxn.back()->abort();
		vTxn.pop_back();
		return(ret==0);
	}
	bool ReadVersion(int & nVersion)
	{
		nVersion=0;
		return Read(string("version"),nVersion);
	}
	bool WriteVersion(int vVersion)
	{
		return Write(string("version"),nVersion);
	}
};
class CTxDB:public CDB
{
public:
	CTxDB(const char* pszMode="r+",bool fTxn=false):CDB(!fClient ?"blkindex.dat":NULL,pszMode,fTxn){}
private:
	CTxDB(const CTxDB&);
	void operator=(const CTxDB&);
public:
	bool ReadTxIndex(uint256 hash, CTxIndex& txindex);
	bool UpdateTxIndex(uint256 hash, const CTxIndex& txindex);
	bool AddTxIndex(const CTransaction& tx, const CDiskTxPos& pos,int nHeight);
	bool EraseTxIndex(const CTransaction& tx);
	bool ContainsTx(uint256 hash);
	bool ReadOwnerTxes(uint160 hash160,int nHeight, vector<CTransaction>& vtx);
	bool ReadDiskTx(uint256 hash, CTransaction& tx, CTxIndex& txindex);
	bool ReadDiskTx(uint256 hash, CTransaction& tx);
	bool ReadDiskTx(COutPoint outpoint, CTransaction& tx, CTxIndex& txindex);
	bool ReadDiskTx(COutPoint outpoint, CTransaction& tx);
	bool WriteBlockIndex(const CDiskBlockIndex& blockindex);
	bool EraseBlackIndex(uint256 hash);
	bool ReadHashBestChain(unit256& hashBestChain);
	bool WriteHashBestChain(uint256 hashBestChain);
	bool LoadBlockIndex();
};


