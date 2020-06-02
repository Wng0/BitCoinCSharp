//复制于bitcoin v0.1.5
#include "headers.h"
bool fDebug = false;
static HANDLE* lock_cs;
void win32_locking_callback(int mode, int type, const char* file, int line)
{
    if (mode& CRYPTO_LOCK)
        WaitForSingleObject(lock_cs[type],INFINITE);
    else
        ReleaseMutex(lock_cs[type]);
}
class CInit
{
public:
    CInit()
    {
        lock_cs=(HANDLE*)OPENSSL_malloc(CRYPTO_num_locks()*sizeof(HANDLE));
        for (int i=0;i<CRYPTO_num_locks();i++)
            lock_cs[i]=CreateMutex(NULL, FALSE,NULL);
        CRYPTO_set_locking_callback(win32_locking_callback);
        RAND_screen();
        RandAddSeed(true);
    }
    ~CInit()
    {
        CRYPTO_set_locking_callback(NULL);
        for (int i=0;i<CRYPTO_num_locks();i++)
            CloseHandle(lock_cs[i]);
        OPENSSL_free(lock_cs);
    }
}
instance_of_cinit;
void RandAddSeed(bool fPerfmon)
{
    LARGE_INTEGER PerformanceCount;
    QueryPerformanceCounter(&PerformanceCount);
    RAND_add(&PerformanceCount, sizeof (PerformanceCount), 1.5);
    memset(&PerformanceCount, 0, sizeof (PerformanceCount));
    static int64 nLastPerfmon;
    if (fPerfmon||GetTime()>nLastPerfmon+5*60)
    {
        nLastPerfmon=GetTime();
        unsigned char pdata[250000];
        memset(pdata,0,sizeof(pdata));
        unsigned long nSize=sizeof(pdata);
        long ret=RegQueryValueEx(HKEY_PERFORMANCE_DATA,"Global",NULL, NULL, pdata, &nSize);
        RegCloseKey(HKEY_PERFORMANCE_DATA);
        if(ret==ERROR_SUCCESS)
        {
            uint256 hash;
            SHA256 (pdata, nSize,(unsigned char*)&hash);
            RAND_add(&hash, sizeof(hash),min(nSize/500.0,(double)sizeof(hash)));
            hash=0;
            memset(pdata,0,nSize);
            time_t nTime;
            time(&nTime);
            struct tm* ptmTime=gmtime(&nTime);
            char pszTime[200];
            strftime(pszTime, sizeof(pszTime),"%x %H:%M:%S",ptmTime);
            printf("%s RandAddSeed() got %d bytes of performance data\n", pszTime, nSize);            
        }
    }
}
int my_snprintf(char* buffer, size_t limit, const char* format,...)
{
    if(limit==0)
        return 0;
    va_list arg_ptr;
    va_start (arg_ptr, format);
    int ret=_vsnprintf(buffer, limit, format, arg_ptr);
    va_end (arg_ptr);
    if (ret<0||ret>=limit)
    {
        ret=limit-1;
        buffer[limit-1]=0;        
    }
    return ret;
}
string strprintf(const char* format,...)
{
    char buffer[50000];
    char* p=buffer;
    int limit=sizeof(buffer);
    int ret;
    loop
    {
        va_list arg_ptr;
        va_start(arg_ptr,format);
        ret=_vsnprintf(p,limit,format, arg_ptr);
        va_end(arg_ptr);
        if(ret>=0&&ret<limit)
            break;
        if(p!=buffer)
            delete p;
        limit*=2;
        p=new char[limit];
        if(p==NULL)
            throw std::bad_alloc();
    }
#ifdef _MSC_VER
    if (p==buffer)
        return string(p, p+ret);
#endif
    string str(p, p+ret);
    if(p!=buffer)
        delete p;
    return str;
} 154