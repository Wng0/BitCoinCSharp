//重写Bitcoin v0.1.5.
static const char* pszBase58="123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
inline string EncodeBase58(const unsigned char* pbegin, const unsigned char* pend)
{
	CAutoBN_CTX 	pctx;
	CBigNum 	bn58	=58;
	CBigNum 	bn0	=0;
	vector<unsigned char> vchTmp(pend-pbegin+1,0);		//转换大结尾为小结尾？以正数结尾。
	reverse_copy(pbegin,pend,vchTmp.begin());
	CBigNum 	bn;
	bn.setvch(vchTmp);
	string 		str;
	str.reserve((pend-pbegin)*138/100+1);
	CBigNum 	dv;
	CBigNum 	rem;	

	while (bn>bn0)
	{
		if (!BN_div(&dv,&rem,&bn,&bn58,pctx)) 		
			throw bignum_error("EncodeBase58:BN_div failed");		
		bn	=dv;
		unsigned int c=rem.getulong();
		str	+=pszBase58[c];
	}
	for (const unsigned char* p=pbegin; p<pend&&*p==0;p++)	//首字母0改写为code58中的0	
		str+=pszBase58[0];	
	reverse(str.begin(),str.end());
	return str;	
}
inline string EncodeBase58(const vector<unsigned char>& vch)
{
	return EncodeBase58(&vch[0],&vch[0]+vch.size());
}
inline bool DecodeBase58(const char* psz, vector<unsigned char>&vchRet)
{
	CAutoBN_CTX 	pctx;
	vchRet.clear();
	CBigNum		bn58	=58;
	CBigNum		bn	=0;
	CBigNum		bnChar;
	while (isspace(*psz))	
		psz++;	
	for (const char*p=psz; *p;p++)				//转换大写字母为大写数字
	{
		const char* p1=strchr(pszBase58, *p);
		if(P1==NULL)
		{
			while (isspace(*p))
				p++;
			if (*P !='\0')
				return false;
			break;
		}
		bnChar.setulong(p1-pszBase58);
		if (!BN_mul(&bn,&bn,&bn58,pctx))
			throw bignum_error("DecodeBase58: BN_mul failed");
		bn+=bnChar;
	}
	vector<unsigned char> vchTmp=bn.getvch();
	if (vchTmp.size()>=2 && vchTmp.end()[-1]== 0 && vchTmp.end()[-2]>=0x80)
		vchTmp.erase(vchTmp.end()-1);
	int nLeadingZeros=0;
	for (const char*p = psz; *p == pszBase58[0]; p++)
		nLeadingZeros++;
	vchRet.assign(nLeadingZeros + vchTmp.size(),0);
	reverse_copy(vchTmp.begin(), vchTmp.end(),vchRet.end() -vchTmp.size());
	return true;
}
inline bool DecodeBase58(const string&str, vector<unsigned char> & vchRet)
{
	return DecodeBase58(str.c_str(),vchRet);
}
inline string EncodeBase58Check(const vector<unsigned char>&vchIn)
{
	vector<unsigned char> vch(vchIn);
	uint256 hash=Hash(vch.begin(),vch.end());
	vch.insert(vch.end(),(unsigned char*)&bash, (unsigned char*)&hash+4);
	return EncodeBase58(vch);
}
inline bool DecodeBase58Check(const char* psz, vector<unsigned char>&vchRet)
{
	if (!DecodeBase58(psz, vchRet))
		return false;
	if (vchRet.size()<4)
	{
		vchRet.clear();
		return false;
	}
	uint256 hash=Hash(vchRet.begin(),vchRet.end()-4);
	if (memcmp(&hash,&vchRet.end()[-4],4)!=0)
	{
		vchRet.clear();
		return false;
	}
	vchRet.resize(vchRet.size()-4);
	return true;
}
inline bool DecodeBase58Check(const string& str, vector<unsigned char>& vchRet)
{
	return DecodeBase58Check(str.c_str(),vchRet);
}
static const unsigned char ADDRESSVERSION=0;
inline string Hash160ToAddress(unit160 hash160)
{
	vector<unsigned char> vch(1, ADDRESSVERSION);
	vch.insert(vch.end(), UBEGIN(hash160), UEND(hash160));
	return EncodeBase58Check(vch);
}
inline bool AddressToHash160(const char* psz, unit160&hash160Ret)
{
	Vector<unsigned char> vch;
	if (!DecodeBase58Check(psz,vch))
		return false;
	if (vch.empty())
		return false;
	unsigned char nVersion=vch[0];
	if (vch.size()!=sizeof(hash160Ret)+1)
		return false;
	memcpy(&hash160Ret, &vch[1],sizeof (hash160Ret));
	return (nVersion<=ADDRESSVERSION);
}
inline bool AddressToHash160(const string& str, unit160& hash160Ret)
{
	return AddressToHash160(str.c_str(), hash160Ret);
}
inline bool IsValidBitcoinAddress(const char* psz)
{
	uint160 hash160;
	return AddressToHash160(psz, hash160);
}
inline bool isValidBitcoinAddress(const string&str)
{
	return IsValidBitcoinAddress(str.c_str());
}
inline string PubKeyToAddress(const vector<unsigned char>& vchPubKey)
{
	return Hash160ToAddress(Hash160(vchPubkey));
}
