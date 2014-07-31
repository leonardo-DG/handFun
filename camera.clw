; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCameraDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "camera.h"
LastPage=0

ClassCount=5
Class1=CCameraApp
Class2=CAboutDlg
Class3=CCameraDlg
Class4=CHelpDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDD_HELP
Resource3=IDD_CAMERA_DIALOG
Class5=CStaticHand
Resource4=IDD_DIALOG_STATIC

[CLS:CCameraApp]
Type=0
BaseClass=CWinApp
HeaderFile=camera.h
ImplementationFile=camera.cpp
LastObject=CCameraApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=cameraDlg.cpp
ImplementationFile=cameraDlg.cpp
LastObject=CAboutDlg

[CLS:CCameraDlg]
Type=0
BaseClass=CDialog
HeaderFile=cameraDlg.h
ImplementationFile=cameraDlg.cpp
LastObject=IDC_BUTTON4
Filter=D
VirtualFilter=dWC

[CLS:CHelpDlg]
Type=0
BaseClass=CDialog
HeaderFile=HelpDlg.h
ImplementationFile=HelpDlg.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CAMERA_DIALOG]
Type=1
Class=CCameraDlg
ControlCount=11
Control1=IDC_BUTTON1,button,1342242816
Control2=IDC_STATIC,static,1342177287
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC01,static,1342308353
Control6=IDC_READ,button,1342242816
Control7=IDC_STATIC1,static,1342177294
Control8=IDC_STATIC,static,1342308352
Control9=IDC_LISTRESULT,listbox,1352728833
Control10=IDC_BUTTON3,button,1342242816
Control11=IDC_BUTTON4,button,1342242816

[DLG:IDD_HELP]
Type=1
Class=CHelpDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[DLG:IDD_DIALOG_STATIC]
Type=1
Class=CStaticHand
ControlCount=13
Control1=IDC_STATICHAND,static,1342177294
Control2=IDC_OPEN,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_LIST1,listbox,1352728835
Control5=IDC_STATICRESULT,static,1342177294
Control6=IDC_STATIC,static,1342177287
Control7=IDC_STATIC,static,1342177287
Control8=IDC_BUTTON1,button,1342242816
Control9=IDC_BUTTON3,button,1342242816
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,button,1342177287
Control12=IDC_STATIC,button,1342177287
Control13=IDC_BUTTON4,button,1342242816

[CLS:CStaticHand]
Type=0
HeaderFile=StaticHand.h
ImplementationFile=StaticHand.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CStaticHand

