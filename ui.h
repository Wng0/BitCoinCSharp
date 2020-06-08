//复制于Bitcoin v0.1.5
DECLARE_EVENT_TYPE(wxEVT_CROSSTHREADCALL,-1)
DECLARE_EVENT_TYPE(wxEVT_REPLY1, -1)
DECLARE_EVENT_TYPE(wxEVT_REPLY2, -1)
DECLARE_EVENT_TYPE(wxEVT_REPLY3, -1)
DECLARE_EVENT_TYPE(wxEVT_TABLEADDED, -1)
DECLARE_EVENT_TYPE(wxEVT_TABLEUPDATED, -1)
DECLARE_EVENT_TYPE(wxEVT_TABLEDELETED, -1)
enum
{
    UICALL_ADDORDER=1,
    UICALL_UPDATEORDER,
};
extern void HandleCtrlA(wxKeyEvent& event);
extern string DateTimeStr(int nTime);
extern string FormatTxStatus(const CWalletTx& wtx);
extern void CrossThreadCall(int nID, void* pdata);
extern void MainFrameRepaint();
extern void Shutdown(void* parg);
class CMainFrame: public CMainFrameBase
{
    38
};
class CTxDetailsDialog: public CTxDetailsDialogBase
{
    87
};
class COptionDialog: public COptionsDialogBase
{
    103
};
class CAboutDialog : public CAboutDialogBase
{
    118
};
class CSendDialog: public CSendDialogBase
{
    131
};
class CSendingDialog: public CSendingDialogBase
{
    150
};
void SendingDialogStartTransfer(void* parg);
void SendingDialogOnReply2(void* parg, CDataStream& vRecv);
void SendingDialogOnReply3(void* parg, CDataStream& vRecv);
class CYourAddressDialog: public CYourAddressDialogBase
{
    192
};
class CAddressBookDialog: public CAddressBookDialogBase
{
    217
};
class CProductsDialog : public CProductsDialogBase
{
    244
};
class CEditProductDialog: public CEditProductDialogBase
{
    263
};
class CViewProductDialog: public CViewProductDialogBase
{
    312
};
class CViewOrderDialog: public CViewOrderDialogBase
{
    341
}
class CEditReviewDialog: public CEditReviewDialogBase
{
    357
}
class CGetTextFromUserDialog : public CGetTextFromUserDialogBase
{
    376
}
