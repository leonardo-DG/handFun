// cameraDlg.h : header file
//

#if !defined(AFX_CAMERADLG_H__F61943C8_378D_4EB3_B6B0_DF618AB8B43A__INCLUDED_)
#define AFX_CAMERADLG_H__F61943C8_378D_4EB3_B6B0_DF618AB8B43A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "vfw.h"
#include "Hand.h"
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CCameraDlg dialog

class CCameraDlg : public CDialog
{
	
// Construction
public:
	CCameraDlg(CWnd* pParent = NULL);	// standard constructor
// Dialog Data
	//{{AFX_DATA(CCameraDlg)
	enum { IDD = IDD_CAMERA_DIALOG };
	CButton	m_start;
	CButton	m_read;
	//}}AFX_DATA
	static CListBox m_listboxresult; //定义成静态的 这样在回调函数中 便可以通过 dlg类 显示识别结果！
	static CStatic	m_hand;
	static SYSTEMTIME ti;//系统时间
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCameraDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	HWND hWndCap;//视频捕获窗口的句柄

	HICON m_hIcon; //图标句柄

	int SaveBitmapToFile(HBITMAP hBitmap, LPSTR lpFileName);//图像保存

	HBITMAP CopyScreenToBitmap(LPRECT lpRect);//采集图像

	bool readRESULT(CString lpname);//发音

	static void showHand(CString name);//显示当前手势

	// Generated message map functions
	//{{AFX_MSG(CCameraDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);  //afx_msg 占位符
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnRead();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//BOOL capSetCallbackOnVideoStream(HWND hWndCap,VIDEOHDR videohead);

#endif // !defined(AFX_CAMERADLG_H__F61943C8_378D_4EB3_B6B0_DF618AB8B43A__INCLUDED_)
