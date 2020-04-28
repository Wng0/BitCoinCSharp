// Bitcoin v 0.1.5重写
#include<vector>
#include<map>
#include<boost/type_traits/is_fundamental.hpp>
#if defined(_MSC_VER)||defined(__BORLANDC___)
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
typedef long long int64;
typedef unsigned long long uint64;
#endif
#if defined(_MSC_VER)&& _MSC_VER <1300
#define for if (false); else for
#endif
class CScript;
class CDataStream;
class CAutoFile;
static const int VERSION=105;
enum
{
	SER_NETWORK		=(1<<0),
	SER_DISK		=(1<<1),
	SER_GETHASH		=(1<<2),
	SER_SKIPSIG		=(1<<16),
	SER_BLOCKHEADERONLY=(1<<17),
};
#define IMPLEMENT_SERIALIZE(statements) \
	unsigned int GetSerializeSize(int nType=0, int nVersion=VERSION) const \
{											\
	CSerActionGetSerializeSize ser_action;	\
	const bool fGetSize	=true;				\	
	const bool fWrite=false;				\
	const bool fRead=false;					\	
	unsigned int nSerSize=0;				\
	ser_streamplaceholder s;				\
	s.nType=nType;							\
	s.nVersion=nVersion;					\
	{statements}							\
	return nSerSize;						\
}											\
template<typename Stream>					\
void Serialize(Stream& s, int nType=0, int nVersion=VERSION) const \
{											\
	CSerActionSerialize ser_action;	\
	const bool fGetSize	=false;				\	
	const bool fWrite=true;				\
	const bool fRead=false;					\	
	unsigned int nSerSize=0;				\
	{statements}							\
}
template<typename Stream>					\
void Unserialize(Stream& s, int nType=0, int nVersion=VERSION)  \
{											\
	CSerActionUnserialize ser_action;	\
	const bool fGetSize	=false;				\	
	const bool fWrite=false;				\
	const bool fRead=true;					\	
	unsigned int nSerSize=0;				\
	{statements}							\
}
# define READWRITE(obj) (nSerSize+=::SerReadWrite(s,(obj), nType, nVersion, ser_action))
#define WRITEDATA(s, obj) s.write((char*)&(obj), sizeof (obj))	
#define READDATA(s, obj) s.read((char*)&(obj), sizeof (obj))
inline unsigned int GetSerializeSize(char a, int, int=0)
{ return sizeof (a);}
inline unsigned int GetSerializeSize(signed char a, int, int=0)
{ return sizeof (a);}
inline unsigned int GetSerializeSize(unsigned char a, int, int=0)
{ return sizeof (a);}
inline unsigned int GetSerializeSize(signed short a, int, int=0)
{ return sizeof (a);}
inline unsigned int GetSerializeSize(unsigned short a, int, int=0)
{ return sizeof (a);}
inline unsigned int GetSerializeSize(signed int a, int, int=0)
{ return sizeof (a);}
inline unsigned int GetSerializeSize(unsigned int a, int, int=0)
{ return sizeof (a);}
inline unsigned int GetSerializeSize(signed long a, int, int=0)
{ return sizeof (a);}
inline unsigned int GetSerializeSize(unsigned long a, int, int=0)
{ return sizeof (a);}
inline unsigned int GetSerializeSize(int64 a, int, int=0)
{ return sizeof (a);}
inline unsigned int GetSerializeSize(uint64 a, int, int=0)
{ return sizeof (a);}
inline unsigned int GetSerializeSize(float a, int, int=0)
{ return sizeof (a);}
inline unsigned int GetSerializeSize(double a, int, int=0)
{ return sizeof (a);}
template <typename Stream> inline void Serialize(Stream& s, char a, int, int=0) {WRITEDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, signed char a, int, int=0) {WRITEDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, unsigned char a, int, int=0) {WRITEDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, signed short a, int, int=0) {WRITEDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, unsigned short a, int, int=0) {WRITEDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, signed int a, int, int=0) {WRITEDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, unsigned int a, int, int=0) {WRITEDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, signed long a, int, int=0) {WRITEDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, unsigned long a, int, int=0) {WRITEDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, int64 a, int, int=0) {WRITEDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, uint64 a, int, int=0) {WRITEDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, float a, int, int=0) {WRITEDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, double a, int, int=0) {WRITEDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, char& a, int, int=0) {READDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, signed char& a, int, int=0) {READDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, signed short& a, int, int=0) {READDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, unsigned short& a, int, int=0) {READDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, signed int& a, int, int=0) {READDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, unsigned int& a, int, int=0) {READDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, signed long& a, int, int=0) {READDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, unsigned long& a, int, int=0) {READDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, int64& a, int, int=0) {READDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, uint64& a, int, int=0) {READDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, float& a, int, int=0) {READDATA(s, a);}
template <typename Stream> inline void Serialize(Stream& s, double& a, int, int=0) {READDATA(s, a);}
inline unsigned int GetSerializedSize(bool a, int, int=0) {return sizeof(char);}
template<typename Stream> inline void Serialize(Stream& s, bool a, int, int=0) {char f=a; WRITEDATA(s,f);}
template<typename Stream> inline void Unserialize(Stream& s, bool& a, int, int=0) {char f; READDATA(s,f); a=f;}
inline unsigned int GetSizeOfCompactSize(uint64 nSize)
{
	if (nSize <UCHAR_MAX -2) return sizeof(unsigned char);
	else if (nSize<=USHRT_MAX) return sizeof(unsigned char) + sizeof(unsigned short);
	else if (nSize<=UINT_MAX) return sizeof (unsigned char) + sizeof (unsigned int);
	else return sizeof (unsigned char)+sizeof (uint64);
}
template<typename Stream> void WriteCompactSize(Stream& os, uint64 nSize)
{
	if (nSize<UCHAR_MAX -2)
	{
		unsigned char chSize=nSize;
		WRITEDATA(os, chSize);
	}
	else if (nSize<=USHRT_MAX)
	{
		unsigned char chSize=UCHAR_MAX-2;
		unsigned short xSize=nSize;
		WRITEDATA(os, chSize);
		WRITEDATA(os, xSize);
	}
	else if (nSize<=UINT_MAX)
	{
		unsigned char chSize=UCHAR_MAX-1;
		unsigned int xSize=nSize;
		WRITEDATA(os, chSize);
		WRITEDATA(os, xSize);
	}
	else 
	{
		unsigned char chSize=UCHAR_MAX;

		WRITEDATA(os, chSize);
		WRITEDATA(os, nSize);
	}
	return;
}
template<typename Stream> uint64 ReadCompactSize(Stream& is)
{
	unsigned char chSize;
	READDATA(is, chSize);
	if (chSize<UCHAR_MAX -2)
	{
		return chSize;
	}
	else if (chSize==UCHAR_MAX-2)
	{
		
		unsigned short nSize;
		READDATA(is, nSize);
		return nSize;
	}
	else if (chSize==UCHAR_MAX-1)
	{
		unsigned int nSize;
		READDATA(is, nSize);
		return nSize;
	}
	else 
	{
		uint64 nSize;
		READDATA(is, nSize);
		return nSize;
	}
}
#define FLATDATA(obj)	REF(CFlatData((char*)&(obj),(char*)&(obj)+sizeof(obj)))
class CFlatData
{
protected:
	char* pbegin;
	char* pend;
public:
	CFlatData(void* pbeginIn, void* pendIn): pbegin((char*)pbeginIn),pend((char*)pendIn){}
	char* begin(){return pbegin;}
	const char* begin() const {return pbegin;}
	char* end() {return pend;}
	const char* end() const{return pend;}
	unsigned int GetSerializeSize(int, int=0) const
	{
		return pend-pbegin;
	}
	template<typename Stream> void Serialize(Stream& s, int, int=0) const
	{
		s.write(pbegin,pend-pbegin);
	}
	template<typename Stream>
	void Unserialize(Stream& s, int, int-0)
	{
		s.read(pbegin,pend-pbegin);
	}
};
template<std:: size_t LEN>
class CFixedFieldString
{
protected:
	const string* pcstr;
	string* pstr;
public:
	explicit CFixedFieldString(const string& str): pcstr(&str), pstr(NULL){}
	explicit CFixedFieldString (string& str): pcstr(&str), pstr(&str) {}
	unsigned int GetSerializeSize(int, int=0) const
	{
		return LEN;
	}
	template<typename Stream>
	void Serialize(Stream& s, int, int=0) const
	{
		char pszBuf[LEN];
		strncpy(pszBuf, pcstr->c_str(),LEN);
		s.write(pszBuf,LEN);
	}
	template<typename Stream>
	void Unserialize(Stream& s, int, int=0)
	{
		if (pstr==NULL)
			throw std::ios_base::failure("CFixedFieldString::Unserialize:trying to unserialize to const string");
		char pszBuf[LEN+1];
		s.read(pszBuf,LEN);
		pszBuf[LEN]='\0';
		*pstr=pszBuf;
	}
};
template<typename C> unsigned int GetSerializeSize(const basic_string<C>& str, int, int=0);
template<typename Stream, typename C> void Serialize(Stream& os, const basic_string<C>& str, int, int=0);
template<typename Stream, typename C> void UnSerialize(Stream& is, basic_string<C>& str, int, int=0);
template<typename T, typename A> unsigned int GetSerializeSize_impl(const std::vector<T,A>& v, int nType, int nVersion, const boost::true_type&);
template<typename T, typename A> unsigned int GetSerializeSize_impl(const std::vector<T,A>& v, int nType, int nVersion, const boost::false_type&);
template<typename T, typename A> inline unsigned int GetSerializeSize(const std::vector<T,A>& v, int nType, int nVersion=VERSION);
template<typename Stream, typename T, typename A> void Serialize_impl(Stream& os, const std::vector<T,A>& v, int nType, int nVersion, const boost::true_type&);
template<typename Stream, typename T, typename A> void Serialize_impl(Stream& os, const std::vector<T,A>& v, int nType, int nVersion, const boost::false_type&);
template<typename Stream, typename T, typename A> inline void Serialize(Stream& os, const std::vector<T,A>& v, int nType, int nVersion=VERSION);
template<typename Stream, typename T, typename A> void Unserialize_impl(Stream& is, std::vector<T,A>& v, int nType, int nVersion, const boost::true_type&);
template<typename Stream, typename T, typename A> void Unserialize_impl(Stream& is, std::vector<T,A>& v, int nType, int nVersion, const boost::false_type&);
template<typename Stream, typename T, typename A> inline void Unserialize(Stream& os, std::vector<T,A>& v, int nType, int nVersion=VERSION);
extern inline unsigned int GetSerializeSize(const CScript& v, int nType, int nVersion=VERSION);
template<typename Stream> void Serialize(Stream& os, const CScript& v, int nType, int nVersion=VERSION);
template<typename Stream> void Unserialize(Stream& is, CScript& v, int nType, int nVersion=VERSION);
template<typename K, typename T> unsigned int GetSerializeSize(const std::pair<K,T>& item, int nType, int nVersion=VERSION);
template<typename Stream, typename K, typename T> void Serialize(Stream& os, const std:: pair<K, T>& item, int nType, int nVersion=VERSION);
template<typename Stream, typename K, typename T> void Unserialize(Stream& is, std::pair<K, T>& item, int nType, int nVersion=VERSION);
template<typename K, typename T, typename Pred, typename A> unsigned int GetSerializeSize(const std::map<K,T,Pred, A>& m, int nType, int nVersion=VERSION);
template<typename Stream, typename K, typename T, typename Pred, typename A> void Serialize(Stream& os, const std:: map<K, T, Pred,A>& m, int nType, int nVersion=VERSION);
template<typename Stream, typename K, typename T, typename Pred, typename A> void Unserialize(Stream& is, std::map<K, T,Pred,A>& m, int nType, int nVersion=VERSION);
template<typename K, typename Pred, typename A> unsigned int GetSerializeSize(const std::set<K,Pred, A>& m, int nType, int nVersion=VERSION);
template<typename Stream, typename K, typename Pred, typename A> void Serialize(Stream& os, const std:: set<K,Pred,A>& m, int nType, int nVersion=VERSION);
template<typename Stream, typename K, typename Pred, typename A> void Unserialize(Stream& is, std::set<K, Pred,A>& m, int nType, int nVersion=VERSION);
template <typename T> inline unsigned int GetSerializeSize(const T& a, long nType, int nVersion=VERSION)
{
	return a.GetSerializeSize((int)nType,nVersion);
}
template <typename Stream, typename T> inline void Serialize (Stream& os, const T& a, long nType, int nVersion=VERSION)
{
	a.Serialize(os, (int)nType,nVersion);
}
template <typename Stream, typename T> inline void Unserialize (Stream& is, T& a, long nType, int nVersion=VERSION)
{
	a.Unserialize(is, (int)nType,nVersion);
}
template<typename C> unsigned int GetSerializeSize(const basic_string<C>&str,int,int)
{
	return GetSizeOfCompactSize(str.size())+str.size()*sizeof (str[0]);
}
template<typename Stream, typename C> 
void Serialize(Stream& os, const basic_string<C>& str, int, int)
{
	WriteCompactSize(os, str.size());
	if (!str.empty())
		os.write((char*)& str[0], str.size()* sizeof(str[0]));
}
template<typename Stream, typename C>
void Unserialize(Stream& is, basic_string<C>& str, int, int)
{
	unsigned int nSize=ReadCompactSize(is);
	str.resize(nSize);
	if (nSize1=0)
		is.read((char*)& str[0], nSize* sizeof(str[0]));
}
template<typename T, typename A> unsigned int GetSerializeSize_impl(const std::vector<T,A>& v, int nType, in nVersion, const boost::true_type&)
{
	return (GetSizeOfCompactSize(v.size())+v.size()*sizeof(T));
}
template<typename T, typename A> unsigned int GetSerializeSize_impl(const std::vector<T,A>& v, int nType, in nVersion, const boost::false_type&)
{
	unsigned int nSize=GetSizeOfCompactSize(v.size());
	for (typename std::vector<T,A>::const_iterator vi= v.begin(); vi!=v.end(); ++vi)
		nSize+=GetSerializeSize((*vi), nType, nVersion);
	return nSize;	
}
template<typename T, typename A> inline unsigned int GetSerializeSize(const std::vector<T,A>& v, int nType, in nVersion)
{
	return GetSerializeSize_impl(v, nType, nVersion, boost::is_fundamental<T>());
}
template<typename Stream, typename T, typename A> void Serialize_impl(Stream& os, const std::vector<T,A>& v, int nType, in nVersion, const boost::true_type&)
{
	WriteCompactSize(os, v.size());
	if (!v.empty())
		os.write((char*)& v[0], v.size()* sizeof(T));
}
template<typename Stream, typename T, typename A> void Serialize_impl(Stream& os, const std::vector<T,A>& v, int nType, in nVersion, const boost::false_type&)
{
	WriteCompactSize(os, v.size());
	for (typename std::vector<T, A>::const_iterator vi=v.begin(); vi!=v.end();++vi)
		::Serialize(os, (*vi), nType, nVersion);
}
template<typename Stream, typename T, typename A> void Serialize(Stream& os, const std::vector<T,A>& v, int nType, in nVersion)
{
	Serialize_impl(os, v, nType, nVersion, boost::is_fundamental<T>());
}
template<typename Stream, typename T, typename A> void Unserialize_impl(Stream& is, std::vector<T,A>& v, int nType, in nVersion, const boost::true_type&)
{
	v.clear();
	unsigned int nSize=ReadCompactSize(is);
	unsigned int i=0;
	while (i<nSize)
	{
		unsigned int blk=min(nSize-i,1+4999999/sizeof(T));
		v.resize(i+blk);
		is.read((char*)&v[i],blk* sizeof(T));
		i+=blk;
	}
}
template<typename Stream, typename T, typename A> void Unserialize_impl(Stream& is, std::vector<T,A>& v, int nType, in nVersion, const boost::false_type&)
{
	v.clear();
	unsigned int nSize=ReadCompactSize(is);
	unsigned int i=0;
	unsigned int nMid=0;
	while (nMid<nSize)
	{
		nMid+=5000000/sizeof(T);
		if (nMid>nSize)
			nMid=nSize;
		v.resize(nMid);
		for (;i<nMid;i++)
			Unserialize(is,v[i],nType,nVersion);
	}
}
template<typename Stream, typename T, typename A> inline void Unserialize(Stream& is, std::vector<T,A>& v, int nType, in nVersion)
{
	Unserialize_impl(is, v, nType, nVersion, boost::is_fundamental<T>());
}
inline unsigned int GetSerializeSize(const CScript& v, int nType, int nVersion)
{
	return GetSerializeSize((const vector<unsigned char>&)v, nType, nVersion);
}
template<typename Stream> void Serialize(Stream& os, const CScript& v, int nType, int nVersion)
{
	Serialize(os, (const vector<unsigned char>&)v, nType, nVesion);
}
template<typename Stream> void Unserialize(Stream& is, CScript& v, int nType, int nVersion)
{
	Unserialize(is, (vector<unsigned char>&)v, nType, nVesion);
}
template<typename K, typename T> unsigned int GetSerializeSize(const std::pair<K,T>& item, int nType, int nVersion)
{
	return GetSerializeSize(item.first, nType, nVersion)+GetSerializeSize(item.second,nType,nVersion);
}
template<typename Stream, typename K, typename T> void Serialize(Stream& os, const std::pair<K,T>& item, int nType, int nVersion)
{
	Serialize(os, item.first, nType,nVersion);
	Serialize(os, item.second, nType,nVersion);
}
template<typename Stream, typename K, typename T> void Unserialize(Stream& is, std::pair<K,T>& item, int nType, int nVersion)
{
	Unserialize(is, item.first, nType,nVersion);
	Unserialize(is, item.second, nType,nVersion);
}
template<typename K, typename T, typename Pred, typename A>
unsigned int GetSerializeSize(const std::map<K,T,Pred,A>& m, int nType, int nVersion)
{
	unsigned int nSize=GetSizeOfCompactSize(m.size());
	for (typename std::map<K,T,Pred,A>::const_iterator mi=m.begin();mi!=m.end();++mi)
		nSize+=GetSerializeSize((*mi),nType,nVersion);
	return nSize;
}
template<typename Stream, typename K, typename T, typename Pred, typename A>
unsigned Serialize(Stream& os, const std::map<K,T,Pred,A>& m, int nType, int nVersion)
{
	WriteCompactSize(os,m.size());
	for (typename std::map<K,T,Pred,A>::const_iterator mi=m.begin();mi!=m.end();++mi)
		Serialize(os, (*mi),nType,nVersion);
}
template<typename Stream, typename K, typename T, typename Pred, typename A>
unsigned Unserialize(Stream& is, std::map<K,T,Pred,A>& m, int nType, int nVersion)
{
	m.clear();
	unsigned int nSize= ReadCompactSize(is);
	typename std::map<K, T, Pred, A>::iterator mi=m.begin();
	for (unsigned int i=0; i<nSize; i++)
	{
		pair<K,T>item;
		Unserialize(is, item, nType, nVersion);
		mi=m.insert(mi,item);
	}
}
template<typename K, typename Pred, typename A>
unsigned int GetSerializeSize(const std::map<K,Pred,A>& m, int nType, int nVersion)
{
	unsigned int nSize=GetSizeOfCompactSize(m.size());
	for (typename std::set<K,Pred,A>::const_iterator it=m.begin();it!=m.end();++it)
		nSize+=GetSerializeSize((*it),nType,nVersion);
	return nSize;
}
template<typename Stream, typename K, typename Pred, typename A>
unsigned Serialize(Stream& os, const std::map<K,Pred,A>& m, int nType, int nVersion)
{
	WriteCompactSize(os,m.size());
	for (typename std::set<K,Pred,A>::const_iterator it=m.begin();it!=m.end();++it)
		Serialize(os, (*it),nType,nVersion);
}
template<typename Stream, typename K, typename Pred, typename A>
unsigned Unserialize(Stream& is, std::set<K,Pred,A>& m, int nType, int nVersion)
{
	m.clear();
	unsigned int nSize= ReadCompactSize(is);
	typename std::set<K, Pred, A>::iterator mi=m.begin();
	for (unsigned int i=0; i< nSize; i++)
	{
		K key;
		Unserialize(is, item, nType, nVersion);
		it=m.insert(it, key);
	}
}
class CSerActionGetSerializeSize{};
class CSerActionSerialize{};
class CSerActionUnserlialize{};
template <typename Stream, typename T>
inline unsigned int SerReadWrite(Stream& s, const T& obj, int nType, int nVersion, CSerActionGetSerializeSize ser_action)
{
	return::GetSerializeSize(obj, nType, nVersion);
}
template <typename Stream, typename T>
inline unsigned int SerReadWrite(Stream& s, const T& obj, int nType, int nVersion, CSerActionSerialize ser_action)
{
	::Serialize(s, obj, nType, nVersion);
	return 0;
}
template <typename Stream, typename T>
inline unsigned int SerReadWrite(Stream& s, T& obj, int nType, int nVersion, CSerActionUnserialize ser_action)
{
	::Unserialize(s, obj, nType, nVersion);
	return 0;
}
struct ser_streamplaceholder
{
	int nType;
	int nVersion;
};
template<typename T>
struct secure_allocator::public std::allocator<T>
{
	typedef std::allocator<T> base;
	typedef typename base::size_type size_type;
	typedef typename base::defference_type difference_type;
	typedef typename base::pointer pointer;
	typedef typename base::const_pointer const_pointer;
	typedef typename base::reference reference;
	typedef typename base::const_reference const_reference;
	typedef typename base::value_type value_type;
	secure_allocator() throw(){}
	secure_allocator(const secure_allocator& a) throw():base(a){}
	~secure_allocator() throw(){}
	template <typename _Other> struct rebind
	{
		typedef secure_allocator<_Other> other;
	};
	void deallocate(T* p, std::size_t n)
	{
		if (p!=NULL)
			memset(p,0, sizeof(T)*n);
		allocator<T>::deallocate(p,n);
	}
};
//696





























































































	
	
