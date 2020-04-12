//改写自bitcoin 0.1.5
#include "headers.h"
map<uint256,CProduct> mapMyProducts;
map<uint256, CProduct> mapProducts;
CCriticalSection cs_mapProducts;
bool AdvertInsert(const CProduct& product)
{
	uint256 hash=product.GetHash();
	bool fNew=false;
	bool fUpdated=false;
	CRITICAL_BLOCK(cs_mapProducts)
	{
		pair<map<uint256, CProduct>::iterator, bool> item=mapProduct.insert(make_pair(hash,product));
		CProduct* pproduct= &(*(item.first)).second;
		fNew=item.second;
		if (product.nSequence> pproduct->nSequence)
		{
			*pproduct=product;
			fUpdate=true;
		}
	}
	return (fNew||fUpdated);
}
void AdvertErase(const CProduct& product)
{
	uint256 hash=product.GetHash();
	CRITICAL_BLOCK(cs_mapProducts)
	mapProducts.erase(hash);
}
template<typename T> unsigned int Union(T& v1, T& v2)
{
	T vUnion;
	vUnion.reserve(v1.size()+v2.size());
	set_union(v1.begin(),v1.end(),v2.begin(), v2.end(), back_inserter(vUnion));
	unsigned int nAdded= vUnion.size()-v1.size();
	if (nAdded>0)
		v1=vUnion;
	return nAdded;
}
void CUser:: AddAtom(unsigned short nAtom, bool fOrigin)
{
	if (binary_search(vAtomsIn.begin(), vAtomsIn.end(), nAtom)|| find(vAtomsNew.begin(), vAtomsNew.end(), nAtom)!=vAtomsNew.end())
		return;
	if (nAtom==0||fOrigin)
	{
		vector<unsigned short> vTmp(1,nAtom);
		Union(vAtomsIn, vTmp);
		if (fOrigin)
			vAtomsOut.push_back(nAtom);
		return;
	}
	vAtomsNew.push_back(nAtom);
	if (vAtomsNew.size()>=nFlowthroughRate||vAtomsOut.empty())
	{
		vAtomsOut.push_back(vAtomsNew[GetRand(vAtomsNew.size())]);
		sort(vAtomsNew.begin(),vAtomsNew.end());
		Union(vAtomsIn, vAtomsNew);
		vAtomsNew.clear();
	}
}
bool AddAtomsAndPropagate(uint256 hashUserStart, const vector<unsigned short>& vAtoms, bool fOrigin)
{
	CReviewDB reviewdb;
	map<uint256, vector<unsigned short>> pmapPropagate[2];
	pmapPropagate[0][hashUserStart]=vAtoms;
	for (int side=0; !pmapPropagate[side].empty();side=1-side)
	{
		map<uint256, vector<unsigned short> >& mapFrom=pmapPropagate[side];
		map<uint256, vector<unsigned short> >& mapTo=pmapPropagate[1-side];
		for (map<uint256, vector<unsigned short>>::iterator mi=mapFrom.begin();mi!=mapFrom.end();++mi)
		{
			const unit256& hashUser=(*mi).first;
			const vector<unsigned short>& vReceived=(*mi).second;
			CUser user;
			reviewdb.ReadUser(hashUser, user);
			unsigned int nIn=user.vAtomsIn.size();
			unsigned int nNew=user.vAtomsNew.size();
			unsigned int nOut=user.vAtomsOut.size();
			foreach (unsigned short nAtom, vReceived)
				user.AddAtom(nAtom, fOrigin);
			fOrigion=false;
			if (user.vAtomsIn.size()==nIn&&user.vAtomsNew.size()==nNew)
				continue;
			if (user.vAtomsOut.size()>nOut)
				foreach(const uint256& hash, user.vLinksOut)
					mapTo[hash].insert(mapTo[hash].end(),user.vAtomsOut.begin()+nOut,user.vAtomsOut.end());
			if (!reviewdb.WriteUser(hashUser, user))
				return false;
		}
		mapFrom.clear();
	}
	return true;
}
bool CReview::AcceptReview()
{
	nTime=GetTime();
	if (!CKey::Verify(vchPubKeyFrom, GetSigHash(), vchSig))
		return false;
	CReviewDB reviewdb;
	vector<CReview> vReviews;
	reviewdb.ReadReviews(hashTo, vReviews);
	vReviews.push_back(*this);
	if (!reviewdb.WriteReviews(hashTo, vReviews))
		return false;
	CUser user;
	uint256 hashFrom=Hash(vchPubKeyFrom.begin(), vchPubKeyFrom.end());
	reviewdb.ReadUser(hashFrom,user);
	user.vLinksOut.push_back(hashTo);
	if (!reviewdb.WriteUser(hashFrom, user))
		return false;
	reviewdb.Close();
	vector<unsigned short> vZeroAtom(1,0);
	if (!AddAtomsAndPropagate(hashTo, user.vAtomsOut.size()? user.vAtomsOut: vZeroAtom, false))
		return false;
	return true;
}
bool CProduct::CheckSignature()
{
	return (CKey::Verify(vchPubKeyFrom, GetSigHash(), vchSig));
}
bool CProduct:: CheckProduct()
{
	if (!CheckSignature())
		return false;
	if (!mapDetails.empty()||!vOrderForm.empty())
		return false;
	CReviewDB reviewdb("r");
	CUser user;
	reviewdb.ReadUser(GetUserHash(),user);
	nAtoms=user.GetAtomCount();
	reviewdb.Close();
	return true;
}
