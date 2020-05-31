//复制于bitcoin v0.1.5
#if defined(_MSC_VER)||defined(__BORLANDC__)
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
typedef long long int64;
typedef unsigned long long uint64;
#endif
#if defined(_MSC_VER)&& _MSC_VER <1300
#define for if (false); else for
#endif
#ifndef _MSC_VER
#define __forceinline inline
#endif
#define foreach         BOOST_FOREACH
#define loop            for (;;)
#define BEGIN(a)        ((char*)&(a))
#define END(a)          ((char*)&((&a))[1]))
#define UBEGIN(a)       ((unsigned char*)&(a))
#define UEND(a)         ((unsigned char*)&((&a))[1]))
#define ARRAYLEN(array) (sizeof(array)/sizeof((array)[0]))
#ifdef _WINDOWS
#define printf          OutputDebugStringF
#endif
#ifdef  snprintf
#undef  snprintf
#endif
#define snprintf my_snprintf
#ifdef  PRId64
#if defined(_MSC_VER)||defined(__BORLANDC__)||defined(__MSVCRT__)
#define PRId64  "I64d"
#define PRId64  "I64u"
#define PRIx64  "I64x"
#else
#define PRId64  "lld"
#define PRIu64  "llu"
#define PRIx64  "llx"
#endif
#endif
#define PAIRTYPE(t1,t2) Pair<t1,t2>
template<typename T>
inline T& REF(const T& val)
{
    return (T&) val;
}
extern bool fDebug;
void RandAddSeed(bool fPerfmon=false);
int my_snprintf(char* buffer, size_t limit, const char* format, ...);
string strprintf(const char* format,...);
bool error(const char* format, ...);
void PrintException(std::exception* pex, const char* pszThread);
void ParseString(const string& str, char c, vector<string>& v);
string FormatMoney(int64 n, bool fPlus=false);
bool ParseMoney(const char* pszIn, int64& nRet);
bool FileExists(const char* psz);
int GetFilesize(FILE* file);
uint64 GetRand(uint64 nMax);
int64 GetTime();
int64 GetAdjustedTime();
void AddTimeData(unsigned int ip, int64 nTime);
class CCriticalSection
{
protected:
    CRITICAL_SECTION cs;
public:
    char* pszFile;
    int nLine;
    explicit CCriticalSection() {initializeCriticalSection(&cs);}
    ~CCriticalSection(){DeleteCriticalSection(&cs);}
    void Enter(){EnterCriticalSection(&cs);}
    void Leave(){LeaveCriticalSection(&cs);}
    bool TryEnter(){return TryEnterCriticalSection(&cs);}
    CRITICAL_SECTION* operator(){return &cs;}
};
class CCriticalBlock
{
protected:
    CRITICAL_SECTION* pcs;
public:
    CCriticalBlock(CRITICAL_SECTION& csIn){pcs=&csIn;EnterCriticalSection(pcs);}
    CCriticalBlock(CCriticalSection& csIn){pcs=&csIn;EnterCriticalSection(pcs);}
    ~CCriticalBlock(){LeaveCriticalSection(pcs);}
};
#define CRITICAL_BLOCK(cs)\
    for (bool fcriticalblockonce=true; fcriticalblockonce; assert(("break caught by CRITICAL_BLOCK!", !fcriticalblockone)),fcriticalblockone=false)\
    for (CCriticalBlock criticalblock(cs); fcriticalblockonce && (cs.pszFile=__FILE__, cs.nLine=__LINE__,true); fcriticalblockonce=false, cs.pszFile=NULL, cs.nLine=0)
class CTryCriticalBlock
{
protected:
    CRITICAL_SECTION* pcs;
public:
    CTryCriticalBlock(CRITICAL_SECTION& csIn){pcs=(TryEnterCriticalSection(&csIn)?&csIn:NULL);}
    CTryCriticalBlock(CCriticalSection& csIn){pcs=(TryEnterCriticalSection(&csIn)?&csIn:NULL);}
    ~CTryCriticalBlock(){if(pcs)LeaveCriticalSection(pcs);}
    bool Entered(){return pcs!=NULL;}    
};
#define TRY_CRITICAL_BLOCK(cs)\
    for (bool fcriticalblockonce=true; fcriticalblockonce; assert(("break caught by TRY_CRITICAL_BLOCK!", !fcriticalblockone)),fcriticalblockone=false)\
    for (CTryCriticalBlock criticalblock(cs); fcriticalblockonce && (fcriticalblockonce=criticalblock.Entered()&&(cs.pszFile=__FILE__, cs.nLine=__LINE__,true); fcriticalblockonce=false, cs.pszFile = NULL, cs.nLine=0)
