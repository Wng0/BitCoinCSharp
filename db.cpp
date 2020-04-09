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
		
