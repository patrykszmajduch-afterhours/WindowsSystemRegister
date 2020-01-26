
// WindowsSystemRegisterDlg.h : header file
//

#pragma once
#include "pch.h"
#include "CRejestr.h"
#include <cstring>
#include <cstring>
#include <string>


// CWindowsSystemRegisterDlg dialog
class CWindowsSystemRegisterDlg : public CDialogEx
{
// Construction
public:
	CWindowsSystemRegisterDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINDOWSSYSTEMREGISTER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton4();
	CTreeCtrl m_strTree;
	CString  getSelectedInTree(CString& hMain);
	
	CEdit konsolaTextBox;
	
	
	afx_msg void OnNMClickTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);

private:
	CString hKeyMain=L"";
	CString hPath;

public:
	CListCtrl listView;
	CButton CheckBox1;
	afx_msg void OnBnClickedCheck1();
	CEdit editBoxMainKey;
	CButton addButton;
	CButton deleteButton;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
