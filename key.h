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
























































