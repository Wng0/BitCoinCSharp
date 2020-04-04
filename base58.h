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
	
