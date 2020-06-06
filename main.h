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
	bool ConnectInputs(CTxDB& txdb,map<uint256,CTxIndex>& mapTestPool, CDiskTxPos posThisTx, int nHeight, int64& nFees, bool fBlock, bool fMiner, int64 nMinFee=0);
	bool ClientConnectInputs();
	bool AcceptTransaction(CTxDB& txdb, bool fCheckInputs=true, bool* pfMissingInputs=NULL);
	bool AcceptTransaction(bool fCheckInputs=true, bool* pfMissingInputs=NULL)
	{
		CTxDB txdb("r");
		return AcceptTransaction(txdb, fCheckInputs, pfMissingInputs);
	}
protected:
	bool AddToMemoryPool();
public:
	bool RemoveFromMemoryPool();
};
Class CMerkleTx: public CTransaction
{
public:
	uint256 hashBlock;
	vector<uint256> vMerkleBranch;
	int nIndex;
	mutable bool fMerkleVerified;
	CMerkleTx()
	{
		Init();
	}
	CMerkleTx(const CTransaction& txIn):CTransaction(txIn)
	{
		Init();
	}
	void Init()
	{
		hashBlock=0;
		nIndex=-1;
		fMerkleVerified=false;
	}
	int64 GetCredit() const
	{
		if(IsCoinBase() && GetBlocksToMaturity()>0)
			return 0;
		return CTransaction::GetCredit();
	}
	IMPLEMENT_SERIALIZE
	(
		nSerSize+=SerReadWrite(s, *(CTransaction*)this,nType,nVersion, ser_action);
		nVersion=this->nVersion;
		READWRITE(hashBlock);
		READWRITE(vMerkleBranch);
		READWRITE(nIndex);
	)
	int SetMerkleBranch(const CBlock* pblock=NULL);
	int GetDepthInMainChain() const;
	bool IsInMainChain() const {return GetDepthInMainChain()>0;}
	int GetBlocksToMaturity() const;
	bool AcceptTransaction(CTxDB& txdb, bool fCheckInputs=true);
	bool AcceptTransaction(){CTxDB txdb("r");return AcceptTransaction(txdb);}
};
class CWalletTx:public CMerkleTx
{
public:
	vector<CMerkleTx> vtxPrev;
	map<string, string> mapValue;
	vector<pair<string,string>>vOrderForm;
	unsigned int fTimeReceivedIsTxTime;
	unsigned int nTimeReceived;
	char fFromMe;
	char fSpent;
	mutable unsigned int nTimeDisplayed;
	CWalletTx()
	{
		Init();
	}
	CWalletTx(const CMerkleTx& txIn):CMerkleTx(txIn)
	{
		Init();
	}
	CWalletTx(const CTransaction& txIn):CMerkleTx(txIn)
	{
		Init();
	}
	void Init()
	{
		fTimeReceivedIsTxTime=false;
		nTimeReceived=0;
		fFromMe=false;
		fSpent=false;
		nTimeDisplayed=0;
	}	
	IMPLEMENT_SERIALIZE
	(
		nSerSize+=SerReadWrite(s, *(CMerkleTx*)this,nType,nVersion, ser_action);
		nVersion=this->nVersion;
		READWRITE(vtxPrev);
		READWRITE(mapValue);
		READWRITE(vOrderForm);
		READWRITE(fTimeReceivedIsTxTime);
		READWRITE(nTimeReceived);
		READWRITE(fFromMe);
		READWRITE(fSpent);		
	)
	bool WriteToDisk()
	{
		return CWalletDB().WriteTx(GetHash(),*this);
	}
	int64 GetTxTime() const;
	void AddSupportingTransactions(CTxDB& txdb);
	bool AcceptWalletTransaction(CTxDB& txdb, bool fCheckInputs=true);
	bool AcceptWalletTRansaction(){CTxDB txdb("r");return AcceptWalletTransaction(txdb);}
	void RelayWalletTransaction(CTxDB& txdb);
	void RelayWalletTransaction(){CTxDB txdb("r"); RelayWalletTransaction(txdb);}
};
class CTxIndex
{
public:
	CDiskTxPos pos;
	vector<CDiskTxpos> vSpent;
	CTxIndex()
	{
		SetNull();
	}
	CTxIndex(const CDiskTxPos& posIn, unsigned int nOutputs)
	{
		pos=posIn;
		vSpent.resize(nOutputs);
	}
	IMPLEMENT_SERIALIZE
	(
		if(!(nType & SER_GETHASH))
			READWRITE(nVersion);
		READWRITE(pos);
		READWRITE(vSpent);
	)
	void SetNull()
	{
		pos.SetNull();
		vSpent.clear();
	}
	bool IsNull()
	{
		return pos.IsNull();
	}
	friend bool operator==(const CTxIndex& a, const CTxIndex&b)
	{
		if (a.pos!=b.pos||a.vSpent.size()!=b.vSpent.size())
			return false;
		for (int i=0;i<a.vSpent.size();i++)
			if (a.vSpent[i]!=b.vSpent[i])
				return false;
		return true;
	}
	friend bool operator!=(const CTxIndex& a, const CTxIndex& b)
	{
		return !(a==b);
	}
};
class CBlock
{
public:
	int vVersion;
	uint256 hashPrevBlock;
	uint256 hashMerkleRoot;
	unsigned int nTime;
	unsigned int nBits;
	unsigned int nNonce;
	vector<CTransaction> vtx;
	mutable vector<uint256> vMerkleTree;
	CBlock()
	{
		SetNull();
	}
	IMPLEMENT_SERIALIZE
	(
		READWRITE(this->nVersion);
		nVersion=this->nVersion;
		READWRITE(hashPrevBlock);
		READWRITE(hashMerkleRoot);
		READWRITE(nTime);
		READWRITE(nBits);
		READWRITE(nNonce);
		if (!(nType&(SER_GETHASH|SER_BLOCKHEADERONLY)))
			READWRITE(vtx);
		else if (fRead)
			const_cast<CBlock*>(this)->vtx.clear();
	)
	void SetNull()
	{
		nVersion=1;
		hashPrevBlock=0;
		hashMerkleRoot=0;
		nTime=0;
		nBits=0;
		nNonce=0;
		vtx.clear();
		vMerkleTree.clear();
	}
	bool IsNull() const
	{
		return (nBits==0);
	}
	uint256 GetHash() const
	{
		return Hash(BEGIN(nVersion),END(nNonce));
	}
	uint256 BuildMerkleTree() const
	{
		vMerkleTree.clear();
		foreach(const CTransaction& tx, vtx)
			vMerkleTree.push_back(tx.GetHash());
		int j=0;
		for (int nSize=vtx.size();nSize>1;nSize=(nSize+1)/2)
		{
			for (int i=0;i<nSize;i+=2)
			{
				int i2=min(i+1,nSize-1);
				vMerkleTree.push_back(Hash(BEGIN(vMerkleTree[j+i], END(vMerkleTree[j+i], BEGIN(vMerkleTree[j+i2]), END(vMerkleTree[j+i2])));
			}
			j+=nSize;
		}
		vector<uint256> GetMerkleBranch(int nIndex) const
		{
			if (vMerkleTree.empty())
				BuildMerkleTree();
			vector<uint256> vMerkleBranch;
			int j=0;
			for (int nSize=vtx.size(); nSize>1; nSize=(nSize+1)/2)
			{
				int i=min(nIndex^1, nSize-1);
				vMerkleBranch.push_back(vMerkleTree[j+i]);
				nIndex>>=1;
				j+= nSize;
			}
			return vMerkleBranch;
		}913	
			
		
	
	











































