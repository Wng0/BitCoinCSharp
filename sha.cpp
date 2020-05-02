//copy from Bitcoin v0.1.5
#include<assert.h>
#include<memory.h>
#include "sha.h"
namespace CryptoPP
{
#define blk0(i) (W[i]=data[i])
#define blk1(i) (W[i&15]=rotlFixed(W(i+13)&15]^W[(i+8)&15]^W[(i+2)&15]^W[i&15],1))
void SHA1::InitState(HashWordType* state)
{
	state[0]=0x67452301L;
	state[1]=0xEFCDAB89L;
	state[2]=0x98BADCFEL;
	state[3]=0x10325476L;
	state[4]=0xC3D2E1F0L;
}
#define f1(x,y,z)(z^(x&(y^z)))
#define f2(x,y,z)(x^y^z)
#define f3(x,y,z)((x&y)|(z&x|y)))
#define f4(x,y,z)(x^y^z)
#define R0(v,w,x,y,z,i) z+=f1(w,x,y)+blk0(i)+0x5A827999+rotlFixed(v,5);w=rotlFixed(w,30);
#define R1(v,w,x,y,z,i) z+=f1(w,x,y)+blk1(i)+0x5A827999+rotlFixed(v,5);w=rotlFixed(w,30);
#define R2(v,w,x,y,z,i) z+=f2(w,x,y)+blk1(i)+0x6ED9EBA1+rotlFixed(v,5);w=rotlFixed(w,30);
#define R3(v,w,x,y,z,i) z+=f3(w,x,y)+blk1(i)+0x8F1BBCDC+rotlFixed(v,5);w=rotlFixed(w,30);
#define R4(v,w,x,y,z,i) z+=f4(w,x,y)+blk1(i)+0xCA62C1D6+rotlFixed(v,5);w=rotlFixed(w,30);
void SHA1::Transform(word32* state, const word32* data)
{
	word32 W[16];
	word32 a = state[0];		
	word32 b = state[1];		
	word32 c = state[2];		
	word32 d = state[3];		
	word32 e = state[4];
	R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3); R0(b,c,d,e,a, 4);
	R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7); R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9);
	R0(a,b,c,d,e,10); R0(e,a,b,c,d,11); R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14);
	R0(a,b,c,d,e,15); R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
	R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23); R2(b,c,d,e,a,24);
	R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27); R2(c,d,e,a,b,28); R2(b,c,d,e,a,29);
	R2(a,b,c,d,e,30); R2(e,a,b,c,d,31); R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34);
	R2(a,b,c,d,e,35); R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
	R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43); R3(b,c,d,e,a,44);
	R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47); R3(c,d,e,a,b,48); R3(b,c,d,e,a,49);
	R3(a,b,c,d,e,50); R3(e,a,b,c,d,51); R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54);
	R3(a,b,c,d,e,55); R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
	R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63); R4(b,c,d,e,a,64);
	R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67); R4(c,d,e,a,b,68); R4(b,c,d,e,a,69);
	R4(a,b,c,d,e,70); R4(e,a,b,c,d,71); R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74);
	R4(a,b,c,d,e,75); R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);
	state[0]+=a;
	state[1]+=b;
	state[2]+=c;
	state[3]+=d;
	state[4]+=e;
}
void SHA224::InitState(HashWordType* state)
{
	static const word32 s[8]={0xc1059ed8, 0x367cd507, 0x3070dd17,0xf70e5939, 0xffc00b31,0x68581511,0x64f98fa7,0xbefa4fa4};
	memcpy(state,s,sizeof(s));
}
void SHA256::InitState(HashWordType* state)
{



