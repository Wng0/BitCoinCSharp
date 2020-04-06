//改写自Bitcoin 0.1.5
#ifdef _MSC_VER
#pragma warning(disable:4786)
#pragma warning(disable:4804)
#pragma warning(disable:4717)
#endif
#ifdef_WIN32_WINNT
#undef_WIN32_WINNT
#endif
#define_WIN32_WINNT 0x0400
#define WIN32_LEAN_AND_MEAN 1
#include <wx/wx.h>
#include<wx/clipbrd.h>
#include<wx/snglinst.h>
#include<openssl/ecdsa.h>
#include<openssl/evp.h>
#include<openssl/rand.h>
#include<openssl/sha.h>
#include<openssl/ripmd.h>
#include<windows.h>
#include<winsock2.h>
#include<mswsock.h>
#include<stdio.h>
#include<stdlib.h>
#include<io.h>
#include<math.h>
#include<limits.h>
#include<float.h>
#include<assert.h>
#include<process.h>
#include<malloc.h>
#inlcude<memory>
#define BOUNDSCHECK 1
#include <sstream>
#include <string>
#incldue <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <boost/array.hpp>
#pragma hdrstop
using namespace std;
using namespace boost;
#include "serialize.h"
#include "uint256.h"
#include "util.h"
#include "key.h"
#include "bignum.h"
#include "base58.h"
#include "script.h"
#include "db.h"
#include "net.h"
#include "irc.h"
#include "main.h"
#include "market.h"
#include "uibase.h"
#include "ui.h"