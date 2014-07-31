// camera.h : main header file for the CAMERA application
//

#if !defined(AFX_CAMERA_H__BF088772_61DD_42C6_ADB8_335257E15372__INCLUDED_)
#define AFX_CAMERA_H__BF088772_61DD_42C6_ADB8_335257E15372__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "Camera.h"

/////////////////////////////////////////////////////////////////////////////
// CCameraApp:
// See camera.cpp for the implementation of this class
//

class CCameraApp : public CWinApp
{
public:
	CCameraApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCameraApp)
	public:
	virtual BOOL InitInstance();
		virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCameraApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bATLInited;
private:
	BOOL InitATL();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMERA_H__BF088772_61DD_42C6_ADB8_335257E15372__INCLUDED_)
