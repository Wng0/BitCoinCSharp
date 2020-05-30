//复制于bitcoin v0.1.5
//wng0@sina.com
#include <limits.h>
#include <string>
#if defined(_MSC_VER)||defined(__BORLANDC__)
typedef __int64 int64;
typedef unsigned _int64 uint64;
#else
typedef long long int64;
typedef unsigned long long uint64;
#endif
#if defined(__MSC_VER)&&_MSC_VER <1300
#define for if (false); else for 
#endif
inline int Testuint256AdHoc(vector<string> vArg);
template<unsigned int BITS>
class base_uint
{
protected:
    enum{WIDTH=BITS/32};
    unsigned int ph[WIDTH];
public:
    bool operator!() const
    {
        for (int i=0;i<WIDTH;i++)
            if (pn[i]!=0)
                return false;
        return true;        
    }
    const base_uint operator~() const
    {
        base_uint ret;
        for (int i=0; i<WIDTH; i++)
            ret.pn[i]=~pn[i];
        return ret;
    }
    const base_uint operator-() const
    {
        base_uint ret;
        for (int i=0; i<WIDTH; i++)
            ret.pn[i]=~pn[i];
        ret++;
        return ret;
    }
    base_uint& operator=(uint64 b)
    {
        pn[0]=(unsigned int)b;
        pn[1]=(unsigned int)(b>>32);
        for (int i=2; i<WIDTH; i++)
            pn[i]=0;
        return *this;
    }
    base_uint& operator^=(const base_uint& b)
    {
        for (int i=0; i<WIDTH; i++)
            pn[i]^=b.pn[i];
        return *this;
    }
    base_uint& operator&=(const base_uint& b)
    {
        for (int i=0; i<WIDTH; i++)
            pn[i]&=b.pn[i];
        return *this;
    }
    base_uint& operator|=(const base_uint& b)
    {
        for (int i=0; i<WIDTH; i++)
            pn[i]|=b.pn[i];
        return *this;
    }
    base_uint& operator^=(uint64 b)
    {
        pn[0]=(unsigned int)b;
        pn[1]=(unsigned int)(b>>32);
        return *this;
    }
    base_uint& operator&=(uint64 b)
    {
        pn[0]=(unsigned int)b;
        pn[1]=(unsigned int)(b>>32);
        return *this;
    }    
    base_uint& operator|=(uint64 b)
    {
        pn[0]=(unsigned int)b;
        pn[1]=(unsigned int)(b>>32);
        return *this;
    }
    //110
    base_uint& operator<<=(unsigned int shift)
    {
        base_uint a(*this);
        for (int i=0;i<WIDTH; i++)
            
    }

}