
// WindowsSystemRegisterDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "WindowsSystemRegister.h"
#include "WindowsSystemRegisterDlg.h"
#include "afxdialogex.h"
#include "RejestrFunkcje.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWindowsSystemRegisterDlg dialog



CWindowsSystemRegisterDlg::CWindowsSystemRegisterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WINDOWSSYSTEMREGISTER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWindowsSystemRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_strTree);
	DDX_Control(pDX, IDC_EDIT1, konsolaTextBox);
	DDX_Control(pDX, IDC_LIST3, listView);
	DDX_Control(pDX, IDC_CHECK1, CheckBox1);
	DDX_Control(pDX, IDC_EDIT2, editBoxMainKey);
	DDX_Control(pDX, IDC_BUTTON1, editBoxMainKey);
	DDX_Control(pDX, IDC_BUTTON2, addButton);
	DDX_Control(pDX, IDC_BUTTON3, deleteButton);
}

BEGIN_MESSAGE_MAP(CWindowsSystemRegisterDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(IDC_BUTTON1, &CWindowsSystemRegisterDlg::OnBnClickedButton1)


	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CWindowsSystemRegisterDlg::OnNMClickTree1)

	//ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE1, &CWindowsSystemRegisterDlg::OnTvnItemexpandedTree1)
	ON_BN_CLICKED(IDC_CHECK1, &CWindowsSystemRegisterDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, &CWindowsSystemRegisterDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CWindowsSystemRegisterDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CWindowsSystemRegisterDlg::OnBnClickedButton3)
	ON_WM_DESTROY()

	ON_COMMAND(AFX_IDB_MINIFRAME_MENU, &CWindowsSystemRegisterDlg::OnAfxIdbMiniframeMenu)
END_MESSAGE_MAP()


// CWindowsSystemRegisterDlg message handlers

BOOL CWindowsSystemRegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (!CzytajPolozenieIWielkoscOknaZRejestru(this->m_hWnd, L"Software\\TEST\\Okno")) { MessageBox(L"Pierwsze uruchomienie programu"); }

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	CheckBox1.SetCheck((int)CzyIstniejeZapisAutostart(HKEY_CURRENT_USER, AfxGetAppName()));
	konsolaTextBox.SetWindowTextW(L"ROOT");

	//tu dodaje do drzewa
	//TODO: refactor to function init main key of register
	m_strTree.InsertItem(L"HKEY_CLASSES_ROOT", TVI_ROOT);
	m_strTree.InsertItem(L"HKEY_CURRENT_USER", TVI_ROOT);
	m_strTree.InsertItem(L"HKEY_LOCAL_MACHINE", TVI_ROOT);
	m_strTree.InsertItem(L"HKEY_USERS", TVI_ROOT);
	m_strTree.InsertItem(L"HKEY_CURRENT_CONFIG", TVI_ROOT);

	//List with value init
	listView.InsertColumn(
		0,              // Rank/order of item
		L"Name",         // Caption for this header
		LVCFMT_LEFT,    // Relative position of items under header
		100);          // Width of items under header
	listView.InsertColumn(1, L"Type", LVCFMT_CENTER, 180);
	listView.InsertColumn(2, L"Data", LVCFMT_CENTER, 100);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWindowsSystemRegisterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWindowsSystemRegisterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWindowsSystemRegisterDlg::OnBnClickedButton1()
{
	if (MessageBox(L"Do you want close application?",
		L"System Register", MB_ICONQUESTION | MB_YESNO) == IDYES)
		this->PostMessage(WM_CLOSE); //lepiej u¿yæ metody EndDialog
	else this->SetWindowText(L"Zamkniêcie anulowane");
	OutputDebugStringW(L"Test butona 1");
}



void CWindowsSystemRegisterDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	ZapiszPolozenieIWielkoscOknaWRejestrze(this->m_hWnd, L"Software\\TEST\\Okno");
	// TODO: Add your message handler code here
}


void CWindowsSystemRegisterDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
}

CString  CWindowsSystemRegisterDlg::getSelectedInTree(CString& hMain)
{
	CString pathToRegister = L"";
	CString hMainKey = L"";

	HTREEITEM hItem = m_strTree.GetSelectedItem();
	do {

		if (m_strTree.GetParentItem(hItem))
		{
			CString temp = pathToRegister;
			pathToRegister = m_strTree.GetItemText(hItem) + L"\\" + temp;
		}
		else
		{
			hMainKey = m_strTree.GetItemText(hItem);
		}

		hItem = m_strTree.GetParentItem(hItem);

	} while (hItem != nullptr);
	hItem = m_strTree.GetSelectedItem();
	hMain = hMainKey;
	if (!m_strTree.GetChildItem(hItem)) {//zapobiega podwajaniu sie
		//QUERy key wywo³anie dodaæ póŸniej do osobnje funkcji
		//TODO: wywoluje sie ale replikuje w nie tej galezi co powinno pradopodobnie xle coœ przekazuje
		HKEY hTestKey;
		//CString text = konsolaTextBox.GetWindowTextW();
		//RegOpenKeyEx otwiera rejestr
		if (hMain != L"")
		{
			/*HKEY HRootKey = helper.Hr
			if (HRootKey != NULL) {
				if (RegOpenKeyEx(HRootKey,
					pathToRegister,
					0,
					KEY_READ,
					&hTestKey) == ERROR_SUCCESS
					)
				{
					QueryKey(hTestKey, m_strTree, m_strTree.GetSelectedItem(), listView);
				}
				RegCloseKey(hTestKey);
			}*/
		}
	}
	hPath = pathToRegister;
	return pathToRegister;
}






void CWindowsSystemRegisterDlg::OnNMClickTree1(NMHDR* pNMHDR, LRESULT* pResult)
{

	konsolaTextBox.SetWindowTextW(getSelectedInTree(hKeyMain));
	editBoxMainKey.SetWindowTextW(hKeyMain);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CWindowsSystemRegisterDlg::OnNMDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult)
{

	konsolaTextBox.SetWindowTextW(getSelectedInTree(hKeyMain));
	editBoxMainKey.SetWindowTextW(hKeyMain);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
//TODO tutaj jest akcja gdy rozwijam drzewo

//void CWindowsSystemRegisterDlg::OnTvnItemexpandedTree1(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
//	
//	OutputDebugStringW(L"My output string.");
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}


void CWindowsSystemRegisterDlg::OnBnClickedCheck1()
{
	wchar_t sciezkaPliku[MAX_PATH]; GetModuleFileName(NULL, sciezkaPliku, MAX_PATH);
	if (CheckBox1.GetCheck())
		ZapiszAutostart(HKEY_CURRENT_USER, AfxGetAppName(), sciezkaPliku);
	else
		UsunAutostart(HKEY_CURRENT_USER, AfxGetAppName());
}


void CWindowsSystemRegisterDlg::OnBnClickedButton2()
{

	if (hKeyMain != L"")
	{
		HKEY rootKey = HKEY_CURRENT_CONFIG;
		if (hKeyMain == "HKEY_CLASSES_ROOT") {
			rootKey = HKEY_CLASSES_ROOT;
		}
		else if (hKeyMain == "HKEY_CURRENT_USER")
		{
			rootKey = HKEY_CURRENT_USER;
		}
		else if (hKeyMain == "HKEY_LOCAL_MACHINE")
		{
			rootKey = HKEY_LOCAL_MACHINE;
		}
		else
		{
			rootKey = HKEY_USERS;
		}

		konsolaTextBox.GetWindowTextW(hPath);
		//RegAddnode(rootKey, hPath);
		OutputDebugStringW(L"Wartosc klucza glownego: " + hKeyMain + L" path: " + hPath + L"\n");

	}

	// TODO: Add your control notification handler code here
}


void CWindowsSystemRegisterDlg::OnBnClickedButton3()
{
	if (hKeyMain != L"")
	{
		HKEY rootKey = HKEY_CURRENT_CONFIG;
		if (hKeyMain == "HKEY_CLASSES_ROOT") {
			rootKey = HKEY_CLASSES_ROOT;
		}
		else if (hKeyMain == "HKEY_CURRENT_USER")
		{
			rootKey = HKEY_CURRENT_USER;
		}
		else if (hKeyMain == "HKEY_LOCAL_MACHINE")
		{
			rootKey = HKEY_LOCAL_MACHINE;
		}
		else
		{
			rootKey = HKEY_USERS;
		}
		RegDelnode(rootKey, hPath);

		OutputDebugStringW(L"Wartosc klucza glownego: " + hKeyMain + L" path: " + hPath + L"\n");

	}
	// TODO: Add your control notification handler code here
}


void CWindowsSystemRegisterDlg::OnAfxIdbMiniframeMenu()
{
	// TODO: Add your command handler code here
}
