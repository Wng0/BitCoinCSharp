//复制于 Bitcoin v0.1.5
//Wng0@sina.com
//2020-6-4
class COutPoint;
class CInPoint;
class CDiskTxPos;
class CCoinBase;
class CTxIn;
class CTxOut;
class CTransaction;
class CBlock;
class CBlockIndex;
class CWalletTx;
class CKeyItem;
static const unsigned int   MAX_SIZE            = 0x02000000;
static const int64          COIN                = 100000000;
static const int64          CENT                = 1000000;
static const int            COINBASE_MATURITY   = 100;
static const CBigNum        bnProofOfWorkLimit(~uint256(0)>>32);
extern CCriticalSection     cs_main;
extern map<uint256, CBlockIndex*> mapBlockIndex;
extern const uint256        hashGenesisBlock;
extern CBlockIndex*         pindexGenesisBlock;
extern int                  nBestHeight;
extern uint256              hashBestChain;
extern CBlockIndex*         pindexBest;
extern unsigned int         nTransactionsUpdated;
extern string               strSetDataDir;
extern int                  nDropMessagesTest;
extern int                  fGenerateBitcoins;
extern int64                nTransactionFee;
extern CAddress             addrIncoming;
string  GetAppDir();
bool    CheckDiskSpace(int64 nAdditionalBytes=0);
FILE*   OpenBlockFile(unsigned int nFile, unsigned int nBlockPos, const char* pszMode="rb");
FILE*   AppendBlockFile(unsigned int& nFileRet);
bool    AddKey(const CKey& key);
vector<unsigned char>   GenerateNewKey();
bool    AddToWallet(const CWalletTx& wtxIn);
void    ReacceptWalletTransactions();
void    RelayWalletTransactions();
bool    LoadBlockIndex(bool fAllowNew=true);
void    PrintBlockTree();
bool    BitcoinMiner();
bool    ProcessMessages(CNode* pfrom);
bool    ProcessMessage(CNode* pfrom, string strCommand, CDataStream& vRecv);
bool    SendMessages(CNode* pto);
int64   GetBalance();
bool    CreatTransaction(CSript scriptPubKey, int64 nValue, CWalletTx& txNew, int64& nFeeRequiredRet);
bool    CommitTransactionSpent(const CWalletTx& wtxNew);
bool    SendMoney(CScript scriptPubKey, int64 nValue, CWalletTx& wtxNew);
81
