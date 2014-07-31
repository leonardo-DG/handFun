#if !defined(AFX_STATICHAND_H__104FC5D9_CC31_4CC3_B49F_755C1BBCB223__INCLUDED_)
#define AFX_STATICHAND_H__104FC5D9_CC31_4CC3_B49F_755C1BBCB223__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticHand.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticHand dialog
/*
	对静态手势图片的识别处理
*/
class CStaticHand : public CDialog
{
// Construction
public:
	CBrush m_brBk;
	HICON m_hIcon; //图标句柄
	CStaticHand(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CStaticHand)
	enum { IDD = IDD_DIALOG_STATIC };
	CStatic	m_staticresult;
	CListBox	m_result;
	CStatic	m_statichand;
	//}}AFX_DATA
	CString n;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticHand)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStaticHand)
	virtual BOOL OnInitDialog();
	afx_msg void OnOpen();
	afx_msg void OnButton2();
	afx_msg void OnSaveResult();
	afx_msg void OnButton3();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButton4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICHAND_H__104FC5D9_CC31_4CC3_B49F_755C1BBCB223__INCLUDED_)
