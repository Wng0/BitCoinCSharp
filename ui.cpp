//复制于bitcoin v0.1.5
#include "headers.h"
#ifdef _MSC_VER
#include <crtdbg.h>
#endif
DEFINE_EVENT_TYPE(wxEVT_CROSSTHREADCALL)
DEFINE_EVENT_TYPE(wxEVT_REPLY1)
DEFINE_EVENT_TYPE(wxEVT_REPLY2)
DEFINE_EVENT_TYPE(wxEVT_REPLY3)
DEFINE_EVENT_TYPE(wxEVT_TABLEADDED)
DEFINE_EVENT_TYPE(wxEVT_TABLEUPDATED)
DEFINE_EVENT_TYPE(wxEVT_TABLEDELETED)
CMainFrame* pframeMain=NULL;
map<string, string> mapAddressBook;
void ThreadRequestProductDetails(void* parg);
void ThreadRandSendTest(void* parg);
bool fRandSendTest=false;
void RandSend();
extern int g_isPainting;
void HandleCtrlA(wxKeyEvent& event)
{
    43
}
bool Is24HourTime()
{
    51
}
string DateStr(int64 nTime)
{
    60
}
string DateTimeStr(int64 nTime)
{
    64
}
wxString GetItemText(wxListCtrl* listCtrl, int nIndex, int nColumn)
{
    75
}
int InsertLine(wxListCtrl* listCtrl, const wxString& str0, const wxString& str1)
{
    86
}
int InsertLine(wxListCtrl* listCtrl, const wxString& str0, const wxString& str1, const wxString& str2, const wxString& str3, const wxString& str4)
{
    93
}
int InsertLine(wxListCtrl* listCtrl, void* pdata, const wxString& str0, const wxString& str1, const wxString& str2, const wxString& str3, const wxString& str4)
{
    103
}
void SetSelection(wxListCtrl* listCtrl, int nIndex)
{
    115
}
int GetSelection(wxListCtrl* listCtrl)
{
    122
}
string HtmlEscape(const char* psz, bool fMultiLine=false)
{
    131
}
string HtmlEscape(const string& str, bool fMultiLine=false)
{
    162
}
void AddToMyProducts(CProduct product)
{
    168
}
set <void*> setCallbackAvailable;
CCriticalSection cs_setCallbackAvailable;
void AddCallbackAvailable(void* p)
{
    192
}
void RemoveCallbackAvailable(void* p)
{
    198
}
bool IsCallbackAvailable(void* p)
{
    204
}

