//复制于bitcoin 0.1.5
class key_error:public std::runtime_error
{
public:
	explicit key_error(const std::string& str):std::runtime_error(str){}
};
typedef vector<unsigned char, secure_allocator<unsigned char> >CPrivKey;
class CKey
{
protected:
	EC_KEY* pkey;
public:
	CKey()
	{
		pkey=EC_KEY_new_by_curve_name(NID_secp256k1);
		if (pkey==NULL)
			throw key_error("CKey::CKey():EC_KEY_new_by_curve_name failed");
	}
	CKey(const CKey& b)
	{
		pkey=EC_Key_dup(b.pkey);
		if (pkey==NULL)
			throw key_error("CKey::CKey(const CKey&):EC_KEY_dup failed");
	}
	CKey& operator=(const CKey& b)
	{
		if (!EC_KEY_copy(pkey,b.pkey))
			throw key_error("CKey::operator=(const CKey&): EC_KEY_copy failed");
		return (*this);
	}
	~CKey()
	{
		EC_KEY_free(pkey);
	}
	void MakeNewKey()
	{
		if (!EC_KEY_generate_key(pkey))
			throw key_error("CKey::MakeNewKey():EC_KEY_generate_key failed");
	}
	bool setPrivKey(const CPrivKey& vchPrivKey)
	{
		const unsigned char* pbegin = &vchPrivKey[0];
		if (!d2i_ECPrivateKey(&pkey, &pbegin, vchPrivKey.size()))
			return false;
		return true;
	}
	CPrivKey GetPrivKey() const
	{
		unsigned int nSize=i2d_ECPrivateKey(pkey,NULL);
		if (!nSize)
			throw key_error("CKey::GetPrivKey():i2d_ECPrivateKey failed");
		CPrivKey vchPrivKey(nSize,0);
		unsigned char* pbegin=&vchPrivKey[0];
		if (i2d_ECPrivateKey(pkey,&pbegin)!=nSize)
			throw key_error("CKey::GetPrivKey():i2d_ECPrivateKey returned unexpected size");
		return vchPrivKey;
	}
	bool SetPubKey(const vector<unsigned char>& vchPubKey)
	{
		const unsigned char* pbegin=&vchPubKey[0];
		if (!o2i_ECPublicKey(&pkey,&pbegin,vchPubKey.size()))
			return false;
		return true;
	}
	vector<unsigned char> GetPubKey() const
	{
		unsigned int nSize=i2o_ECPublicKey(pkey,NULL);
		if (!nSize)
			throw key_error("CKey::GetPubKey():i20_ECPublicKey failed");
		vector<unsigned char> vchPubKey(nSize,0);
		unsigned char* pbegin=&vchPubKey[0];
		if (i2o_ECPublicKey(pkey,&pbegin)!=nSize)
			throw key_error("CKey::GetPubKey():i2o_ECPublicKey retruned unexpected size");
		return vchPubKey;
	}
	bool Sign(uint256 hash, vector<unsigned char>& vchSig)
	{
		vchSig.clear();
		unsigned char pchSig[10000];
		unsigned int nSize=0;
		if (!ECDSA_sign(0, (unsigned char*)&hash,sizeof(hash),pchSig, &nSize, pkey))
			return false;
		vchSig.resize(nSize);
		memcpy(&vchSig[0],pchSig, nSize);
		return true;
	}
	bool Verify(uint256 hash, const vector<unsigned char>& vchSig)
	{
		if (ECDSA_verify(0,(unsigned char*)&hash, sizeof (hash), &vchSig[0], vchSig.size(),pkey)!=1)
			return false;
		return true;
	}
	static bool Sign(const CPrivKey& vchPrivKey, uint256 hash, vector<unsigned char>& vchSig)
	{
		CKey key;
		if (!key.SetPrivKey(vchPrivKey))
			return false
		return key.Sign(hash, vchSig);
	}
	static bool Verify(const vector<unsigned char>& vchPubKey, uint256 hash, const vector<unsigned char>& vchsig)
	{
		CKey key;
		if (!key.SetPubKey(vchPubKey))
			return false;
		return key.Verify(hash, vchSig);
	}
};

























































