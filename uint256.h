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
            pn[i]=0;
        int k=shift/32;
        shift = shift % 32;
        for (int i=0; i< WIDTH; i++)
        {
            if (i+k+1< WIDTH && shift !=0)
                pn[i+k+1] |=(a.pn[i]>>(32-shift));
            if (i+k<WIDTH)
                pn[i+k]|=(a.pn[i]<<shift);                
        }
        return *this;            
    }
    base_uint& operator>>=(unsigned int shift)
    {
        base_uint a(*this);
        for (int i=0;i<WIDTH; i++)
            pn[i]=0;
        int k=shift/32;
        shift = shift % 32;
        for (int i=0; i< WIDTH; i++)
        {
            if (i-k-1>=0 && shift !=0)
                pn[i-k-1] |=(a.pn[i]<<(32-shift));
            if (i-k>=0)
                pn[i-k] |= (a.pn[i]>>shift);                
        }
        return *this;            
    }
    base_uint& operator+=(const base_uint& b)
    {
        uint64 carry =0;
        for (int i=0;i<WIDTH;i++)
        {
            uint64 n=carry +pn[i]+b.pn[i];
            pn[i]=n& 0xffffffff;
            carry=n>>32;            
        }
        return *this;
    }
    base_uint& operator-=(const base_uint& b)
    {
        *this+=-b;
        return * this;
    }
    base_uint& operator+=(uint64 b64)
    {
        base_uint b;
        b=b64;
        *this+=b;
        return *this;
    }
    base_uint& operator-=(uint64 b64)
    {
        base_uint b;
        b=b64;
        *this+=-b;
        return *this;
    }
    base_uint& operator++()
    {
        int i=0;
        while (++pn[i]==0&&i<WIDTH-1)
        {
            i++;
        }
        return *this;        
    }
    const base_uint operator++(int)
    {
        const base_uint ret=*this;
        ++(*this);
        return ret;
    }
    base_uint& operator--()
    {
        int i=0;
        while (--pn[i]==-1&&i<WIDTH-1)
        {
            i++;
        }
        return *this;        
    }
    const base_uint operator--(int)
    {
        const base_uint ret=*this;
        --(*this);
        return ret;
    }
    friend inline bool operator<(const base_uint& a, const base_uint& b)
    {
        for (int i=base_uint::WIDTH-1;i>=0;i--)
        {
            if (a.pn[i]<b.pn[i])
                return true;
            else if(a.pn[i]>b.pn[i])
                return false;            
        }
        return false;
    }
    friend inline bool operator<=(const base_uint& a, const base_uint& b)
    {
        for (int i=base_uint::WIDTH-1;i>=0;i--)
        {
            if (a.pn[i]<b.pn[i])
                return true;
            else if(a.pn[i]>b.pn[i])
                return false;            
        }
        return false;
    }

}