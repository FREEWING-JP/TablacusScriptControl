#import <msscript.ocx>
using namespace MSScriptControl;

#define DISPID_TE_ITEM  0x3fffffff
#define DISPID_TE_COUNT 0x3ffffffe
#define DISPID_TE_INDEX 0x3ffffffd
#define DISPID_TE_MAX TE_VI

class CteDispatch : public IDispatch, public IEnumVARIANT
{
public:
	STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	//IDispatch
	STDMETHODIMP GetTypeInfoCount(UINT *pctinfo);
	STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo);
	STDMETHODIMP GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
	STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr);
	//IEnumVARIANT
	STDMETHODIMP Next(ULONG celt, VARIANT *rgVar, ULONG *pCeltFetched);
	STDMETHODIMP Skip(ULONG celt);
	STDMETHODIMP Reset(void);
	STDMETHODIMP Clone(IEnumVARIANT **ppEnum);

	CteDispatch(IDispatch *pDispatch, int nMode);
	~CteDispatch();

	DISPID		m_dispIdMember;
	int			m_nIndex;
	IActiveScript *m_pActiveScript;
private:
	LONG		m_cRef;
	IDispatch	*m_pDispatch;
	int			m_nMode;//0: Clone 1:collection 2:ScriptDispatch
};

class CTScriptControl : public IScriptControl
{
public:
	STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	//IDispatch
	STDMETHODIMP GetTypeInfoCount(UINT *pctinfo);
	STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo);
	STDMETHODIMP GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
	STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr);
	//IScriptContotol
	STDMETHODIMP get_Language(BSTR * pbstrLanguage);
	STDMETHODIMP put_Language(BSTR bstrLanguage);
	STDMETHODIMP get_State(enum ScriptControlStates * pssState);
	STDMETHODIMP put_State(enum ScriptControlStates ssState);
	STDMETHODIMP put_SitehWnd(long hwnd);
	STDMETHODIMP get_SitehWnd(long * phwnd);
	STDMETHODIMP get_Timeout(long * plMilleseconds);
	STDMETHODIMP put_Timeout(long lMilleseconds);
	STDMETHODIMP get_AllowUI(VARIANT_BOOL * pfAllowUI);
	STDMETHODIMP put_AllowUI(VARIANT_BOOL fAllowUI);
	STDMETHODIMP get_UseSafeSubset(VARIANT_BOOL * pfUseSafeSubset);
	STDMETHODIMP put_UseSafeSubset(VARIANT_BOOL pfUseSafeSubset);
	STDMETHODIMP get_Modules(struct IScriptModuleCollection ** ppmods);
	STDMETHODIMP get_Error(struct IScriptError ** ppse);
	STDMETHODIMP get_CodeObject(IDispatch ** ppdispObject);
	STDMETHODIMP get_Procedures(struct IScriptProcedureCollection ** ppdispProcedures);
	STDMETHODIMP raw__AboutBox();
	STDMETHODIMP raw_AddObject(BSTR Name,IDispatch * Object,VARIANT_BOOL AddMembers);
	STDMETHODIMP raw_Reset();
	STDMETHODIMP raw_AddCode(BSTR Code);
	STDMETHODIMP raw_Eval(BSTR Expression,VARIANT * pvarResult);
	STDMETHODIMP raw_ExecuteStatement(BSTR Statement);
	STDMETHODIMP raw_Run(BSTR ProcedureName,SAFEARRAY ** Parameters,VARIANT * pvarResult);

	CTScriptControl();
	~CTScriptControl();

	HRESULT Exec(BSTR Expression,VARIANT * pvarResult, DWORD dwFlags);
	HRESULT ParseScript(LPOLESTR lpScript, LPOLESTR lpLang, IDispatchEx *pdex, IUnknown *pOnError, IDispatch **ppdisp, IActiveScript **ppas, VARIANT *pvarResult, DWORD dwFlags);
	VOID Clear();
private:
	LONG   m_cRef;
	BSTR m_bsLang;
	IActiveScript *m_pActiveScript;
	IDispatch *m_pJS;
	IDispatch *m_pObject;
	IDispatchEx *m_pObjectEx;
	IDispatch *m_pCode;
	HWND m_hwnd;
};

class CteActiveScriptSite : public IActiveScriptSite, public IActiveScriptSiteWindow
{
public:
	STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	//ActiveScriptSite
	STDMETHODIMP GetLCID(LCID *plcid);
	STDMETHODIMP GetItemInfo(LPCOLESTR pstrName,
                           DWORD dwReturnMask,
                           IUnknown **ppiunkItem,
                           ITypeInfo **ppti);
	STDMETHODIMP GetDocVersionString(BSTR *pbstrVersion);
	STDMETHODIMP OnScriptError(IActiveScriptError *pscripterror);
	STDMETHODIMP OnStateChange(SCRIPTSTATE ssScriptState);
	STDMETHODIMP OnScriptTerminate(const VARIANT *pvarResult,const EXCEPINFO *pexcepinfo);
	STDMETHODIMP OnEnterScript(void);
	STDMETHODIMP OnLeaveScript(void);
	//IActiveScriptSiteWindow
	STDMETHODIMP GetWindow(HWND *phwnd);
	STDMETHODIMP EnableModeless(BOOL fEnable);

	CteActiveScriptSite(IUnknown *punk, IUnknown *pOnError, CTScriptControl *pSC);
	~CteActiveScriptSite();
public:
	LONG		m_cRef;
	IDispatchEx	*m_pDispatchEx;
	IDispatch *m_pOnError;
	CTScriptControl *m_pSC;
};

class CTScriptControlFactory : public IClassFactory
{
public:
	STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	
	STDMETHODIMP CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject);
	STDMETHODIMP LockServer(BOOL fLock);
};

const CLSID CLSID_JScriptChakra       = {0x16d51579, 0xa30b, 0x4c8b, { 0xa2, 0x76, 0x0f, 0xf4, 0xdc, 0x41, 0xe7, 0x55}}; 

extern void LockModule(BOOL bLock);
extern BOOL CreateRegistryKey(HKEY hKeyRoot, LPTSTR lpszKey, LPTSTR lpszValue, LPTSTR lpszData);