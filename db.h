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

