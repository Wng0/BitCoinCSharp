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
class CDiskTxPos
{
public: 
	unsigned int nFile;
	unsigned int nBlockPos;
	unsigned int nTxPos;
	CDiskTxPos()
	{
		SetNull();
	}
	CDiskTxPos(unsigned int nFileIn, unsigned int nBlockPosIn, unsigned int nTxPosIn)
	{
		nFile=nFileIn;
		nBlockPos= nBlockPosIn;
		nTxPos=nTxPosIn;
	}
	IMPLEMENT_SERIALIZE( READWRITE(FLATDATA(*this));)
	void SetNull() { nFile=-1; nBlockPos=0;nTxPos=0;}
	bool InNull() const { return (nFile==-1);}
	friend bool operator==(const CDiskTxPos& a, const CDiskTxPos& b)
	{
		return (a.nFile==b.nFile&&a.nBlockPos==b.nBlockPos &&
				a.nTxPos==b.nTxPos);
	}
	friend bool operator!=(const CDiskTxPos& a, const CDiskTxPos& b)
	{
		return !(a==b);
	}
	string ToString() const
	{
		if (IsNull())
			return strprintf("null");
		else 
			return strprintf("(nFile=%d, nBlockPos=%d, nTxPos=%d)", nFile, nBlockPos, nTxPos);
	}
	void print()const
	{
		printf("%s", ToString().c_str());
	}
};
class CInPoint
{
public:
	CTransaction* ptx;
	unsigned int n;
	CInPoint(){SetNull();}
	CInPoint(CTransaction* ptxIn, unsigned int nIn){ptx=ptxIn;n=nIn;}
	void SetNull(){ptx=NULL;n=-1;}
	bool IsNull() const{return (ptx==NULL&&n==-1);}
};
class COutPoint
{
public:
	uint256 hash;
	unsigned int n;
	COutPoint() {SetNull();}
	COutPoint(uint256 hashIn, unsigned int nIn){hash=hashIn;n=nIn;}
	IMPLEMENT_SERIALIZE(READWRITE(FLATDATA(*this));)
	void SetNull(){hash=0;n=-1;}
	bool IsNull() const { return (hash==0&&n==-1);}
	friend bool operator<(const COutPoint& a, const COutPoint& b)
	{
		return (a.hash<b.hash||(a.hash==b.hash&&a.n<b.n));
	}
	friend bool operator==(const COutPoint& a, const COutPoint& b)
	{
		return (a.hash==b.hash&&a.n==b.n);
	}
	friend bool operator!=(const COutPoint& a, const COutPoint& b)
	{
		return !(a==b);
	}
	string ToString() const
	{
		return strprintf("COutPoint(%s, %d)", hash.ToString().substr(0,6).c_str(),n);
	}
	void print() const
	{
		printf("%s\n", ToString().c_str());
	}
};
class CTxIn
{
public:
	COutPoint prevout;
	CScript scriptSig;
	unsigned int nSequence;
	CtxIn()
	{
		nSequence=UINT_MAX;
	}
	explicit CTxIn(COutPoint prevoutIn, CScript scriptSigIn=CScript(),
	unsigned int nSequenceIn=UINT_MAX)
	{
		preout=prevoutIn;
		scriptSig=scriptSigIn;
		nSequence=nSequenceIn;
	}
	CTxIn(uint256 hashPrevTx, unsigned int nOut, CScript scriptSigIn=CScript(), unsigned int nSequenceIn=UINT_MAX)
	{
		prevout=COutPoint(hashPrevTx, nOut);
		scriptSig=scriptSigIn;
		nSequence=nSequenceIn;
	}
	IMPLEMENT_SERIALIZE
	(
		READWRITE(prevout);
		READWRITE(scriptSig);
		READWRITE(nSequence);
	)
	bool IsFinal() const
	{
		return (nSequence==UNIT_MAX);
	}
	friend bool operator==(const CTxIn& a, const CTxIn& b)
	{
		return (a.prevout==b.prevout&&a.scriptSig==b.scriptSig&&
				a.nSequence==b.nSequence);
	}
	friend bool operator!=(const CTxIn& a, const CTxIn& b)
	{
		return !(a==b);
	}
	string ToString() const
	{
		string str;
		str+=strPrintf("CTxIn(");
		str+=prevout.ToString();
		if (prevout.IsNull())
			str+=strprintf(",coinbase %s", HexStr(scriptSig.begin(),scriptSig.end(),false).c_str());
		else
			str+=strprintf(", scriptSig=%s", scriptSig.ToString().substr(0,24).c_str());
		if (nSequence!=UINT_MAX)
			str+=strprintf(",nSequnce=%u", nSequence);
		str+=")";
		return str;
	}
	void print() const
	{
		prinf("%s\n", ToString().c_str());
	}
	bool IsMine() const;
	int64 GetDebit() const;
};
class CTxOut
{
public:
	int64 nValue;
	CSript scriptPubKey;
public:
	CTxOut()
	{
		SetNull();
	}
	CTxOut(int64 nValueIn, CSript scriptPubKeyIn)
	{
		nValue=nValueIn;
		scriptPubKey=scriptPubKeyIn;
	}
	IMPLEMENT_SERIALIZE
	(
		READWRITE(nValue);
		READWRITE(scriptPubKey);
	)
	void SetNull()
	{
		nValue=-1;
		scriptPubKey.clear();
	}
	bool IsNull()
	{
		return (nValue==-1);
	}
	uint256 GetHash() const
	{
		return SerializeHash(*this);
	}
	bool IsMine() const
	{
		return::IsMine(scriptPubKey);
	}
	int 64 GetCredit() const
	{
		if(IsMine())
			return nValue;
		return 0;
	}
	friend bool operator==(const CTxOut& a, const CTxOut& b)
	{
		return (a.nValue==b.nValue&& a.scriptPubKey==b.scriptPubKey);
	}
	friend bool operator!=(const CTxOut& a, const CTxOut& b)
	{
		return !(a==b);
	}
	string ToString() const
	{
		if (scriptPubKey.size()<6)
			return "CTxOut(error)";
		return strprintf("CTxOut(nValue=%I64d.%08I64d,scriptPubKey=%s)", nValue/COIN, nValue % COIN, scriptPubKey.ToString().substr(0,24).c_str());
	}
	void print() const
	{
		printf("%s\n",ToSTring().C_str());
	}
};
class CTransaction
{
public:
	int nVersion;
	vector<CTxIn> vin;
	vector<CTxOut> vout;
	int nLockTime;
	CTransaction()
	{
		SetNull();
	}
	IMPLEMENT_SERIALIZE
	(
		READWRITE(this->nVersion);
		nVersion=this->nVersion;
		READWRITE(vin);
		READWRITE(vout);
		READWRITE(nLockTime);
	)
	void SetNull()
	{
		nVersion=1;
		vin.clear();
		vout.clear();
		nLockTime=0;
	}
	bool IsNull() const
	{
		return (vin.empty()&& vout.empty());
	}
	uint256 GetHash() const
	{
		return SerializeHash(*this);
	}
	bool IsFinal() const
	{
		if (nLockTime==0||nLockTime<nBestHeight)
			return true;
		foreach(const CTxIn& txin, vin)
			if (!txin.IsFinal())
				return false;
		return true;
	}
	bool IsNewerThan(const CTransaction& old) const
	{
		if (vin.size()!=old.vin.size())
			return false;
		for (int i=0;i<vin.size();i++)
			if (vin[i].prevout!=old.vin[i].prevout)
				return false;
		bool fNewer=false;
		unsigned int nLowest=UINT_MAX;
		for (int i=0; i<vin.size();i++)
		{
			if(vin[i].nSequence!=old.vin[i].nSequence)
			{
				if (vin[i].nSequence<=nLowest)
				{
					fNewer=false;
					nLowest=vin[i].nSequence;
				}
				if (old.vin[i].nSequence<nLowest)
				{
					fNewer=true;
					nLowest=old.vin[i].nSequence;
				}
			}
		}
		return fNewer;
	}
	bool IsCoinBase() const
	{
		return (vin.size()==1&&vin[0].prevout.IsNull());
	}
	bool CheckTransaction() const
	{
		if (vin.empty()||vout.empty())
			return error("CTransaction::CheckTransaction():vin or vout empty");
		foreach(const CTxOut& txout,vout)
			if (txout.nValue<0)
				return error("CTransaction::CheckTransaction(): txout.nValue negative");
		if(IsCoinBase())
		{
			if (vin[0].scriptSig.size()<2||vin[0].scriptSig.size()>100)
				return error("CTransaction::CheckTransaction():coinbase script size");
		}
		else
		{
			foreach(const CTxIn& txin, vin)
				if (txin.prevout.IsNull())
					return error("CTransaction::CheckTransaction():prervout is null");
		}
		return true;
	}
	bool IsMine() const
	{
		foreach(const CTxOut& txout, vout)
			if (txout.IsMine())
				return true;
		return false;
	}
	int64 GetDebit() const
	{
		int64 nDebit=0;
		foreach (const CTxIn& txin, vin)
			nDebit+=txin.GetDebit();
		return nDebit;
	}
	int64 GetCredit() const
	{
		int64 nCredit=0;
		foreach(const CTxOut& txout,vout)
			nCredit+=txout.GetCredit();
		return nCredit;
	}
	int64 GetValueOut() const
	{
		int64 nValueOut=0;
		foreach(const CTxOut& txout, vout)
		{
			if (txout.nValue<0)
				throw runtime error("CTransaction::GetValueOut():negative value");
			nValueOut+=txout.nValue;
		}
		return nValueOut;
	}
	int64 GetMinFee(bool fDiscount=false) const
	{
		unsigned int nBytes=::GetSerializeSize(*this, SER_NETWORK);
		int64 nMinFee=(1+(int64)nBytes/1000)*CENT;
		if (fDiscount && nBytes <10000)
			nMinFee=0;
		if (nMinFee<CENT)
			foreach (const CTxOut& txout, vout)
				if (txout.nValue<CENT)
					nMinFee=CENT;
		return nMinFee;
	}
	bool ReadFromDisk(CDiskTxPos pos, FILE** pfileRet=NULL)
	{
		CAutoFile filein=OpenBlockFile(pos.nFile,0,pfileRet?"rb+":"rb");
		if (!filein)
			return error("CTransaction::ReadFromDisk():OpenBlockFilefailed");
		if (fseek(filein, pos.nTxPos, SEEK_SET)!=0)
			return error("CTransaction::ReadFromDisk():fsee failed");
		filein>>*this;
		if (pfileRet)
		{
			if (fseek(filein, pos.nTxPos, SEEK_SET)!=0)
				return error("CTransaction::ReadFromDisk():second fseek failed");
			*pfileRet=filein.release();
		}
		return true;
	}
	friend bool operator==(const CTransaction& a, const CTransaction& b)
	{
		return (a.nVersion==b.nVersion&&
				a.vin==b.vin&&
				a.vout==b.vout&&
				a.nLockTime==b.nLockTime);
	}
	friend bool operator!=(const CTransaction& a, const CTransaction& b)
	{
		return!(a==b);
	}
	string ToString() const
	{
		string str;
		str+=strprintf("CTransaction(hash=%s,ver=%d,vin.size=%d,vout.size=%d,nLockTime=%d)\n",
			GetHash().ToString().substr(0,6).c_str(),
			nversion,
			vin.size(),
			vout.size(),
			nLockTime);
		for (int i=0;i<vin.size();i++)
			str+="    "+vin[i].ToString()+"\n";

		for (int i=0;i<vout.size();i++)
			str+="    "+vout[i].ToString()+"\n";
		return str;
	}
	void print() const
	{
		printf("%s",ToString().c_str());
	}
	bool DisconnectInputs(cTxDB& txdb);
	bool ConnectInputs(CTxDB& txdb,map<uint256,CTxIndex>& mapTestPool,
	586











































