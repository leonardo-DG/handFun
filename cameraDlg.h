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
	static CListBox m_listboxresult; //����ɾ�̬�� �����ڻص������� �����ͨ�� dlg�� ��ʾʶ������
	static CStatic	m_hand;
	static SYSTEMTIME ti;//ϵͳʱ��
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCameraDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	HWND hWndCap;//��Ƶ���񴰿ڵľ��

	HICON m_hIcon; //ͼ����

	int SaveBitmapToFile(HBITMAP hBitmap, LPSTR lpFileName);//ͼ�񱣴�

	HBITMAP CopyScreenToBitmap(LPRECT lpRect);//�ɼ�ͼ��

	bool readRESULT(CString lpname);//����

	static void showHand(CString name);//��ʾ��ǰ����

	// Generated message map functions
	//{{AFX_MSG(CCameraDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);  //afx_msg ռλ��
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
