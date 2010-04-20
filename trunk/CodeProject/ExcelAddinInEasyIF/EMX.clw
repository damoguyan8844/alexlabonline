; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=29
Class1=CEMXApp
LastClass=FloatingGridWnd
NewFileInclude2=#include "EMX.h"
ResourceCount=19
NewFileInclude1=#include "stdafx.h"
Class2=ConfigOptions
LastTemplate=CFrameWnd
Resource1=IDD_WIZ_INSERTPOS
Class3=MatrixSel
Resource2=IDD_GRWZ_GRIDFROMRANGE
Class4=MtxEditGen
Resource3=IDD_PASSWORD_ENTRY
Class5=MatrixEditCol
Resource4=IDD_COLEDIT_GENERAL
Class6=MatrixEditData
Resource5=IDD_MTXEDIT_DATAVALUES
Class7=MatrixEdit
Class8=PasswordEntryDlg
Resource6=IDD_COPY_MATRIX_NAME
Class9=ColPosition
Resource7=IDD_COLEDIT_POSITION
Class10=ColGeneral
Resource8=IDD_COLEDIT_PICKLIST
Class11=ColPickList
Class12=ColumnEdit
Resource9=IDD_MTXEDIT_GENERAL
Class13=DuplicateMatrix
Resource10=IDD_MATRIX_SELECTOR
Class14=FullScreenGrid
Resource11=IDD_MTXEDIT_COLUMNS
Resource12=IDD_GRIDWIZ_CHOOSER
Class15=WizGrSel
Class16=WizGrPos
Class17=GridWiz
Resource13=IDD_CONFIG_OPTS
Class18=WizGrRng
Resource14=IDD_WIZ_MATRIXINFO
Resource15=IDD_MTXEDIT_DATAVALUESFULL
Class19=GridWizChooser
Class20=FLDProps
Class21=GridFromCellWiz
Resource16=IDD_WIZ_SELECTMATRIX
Class22=GFCMatrixDetails
Resource17=IDD_WIZGRID_RANGE
Class23=GFRSetup
Class24=FloatingGridWnd
Class25=CGridList
Resource18=IDD_WIZ_FIELDDEF
Class26=FloatBtn
Class27=FloatingValuesPage
Class28=FloatingThread
Class29=HiddenComWin
Resource19=IDR_FLOATCTXT_MENU

[CLS:CEMXApp]
Type=0
HeaderFile=EMX.h
ImplementationFile=EMX.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CEMXApp

[DLG:IDD_CONFIG_OPTS]
Type=1
Class=ConfigOptions
ControlCount=9
Control1=IDC_STATIC,static,1342312448
Control2=IDC_CONFIG_PATHNAME,edit,1350631552
Control3=IDC_CONFIG_BROWSEPATH,button,1342242816
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,static,1342312448
Control6=IDC_CONFIG_REFRESHINTERVAL,combobox,1344339971
Control7=IDC_CONFIG_RECALCONCHANGE,button,1342242819
Control8=IDOK,button,1342242817
Control9=IDCANCEL,button,1342242816

[CLS:ConfigOptions]
Type=0
HeaderFile=CFGOpts.h
ImplementationFile=CFGOpts.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ConfigOptions

[DLG:IDD_MATRIX_SELECTOR]
Type=1
Class=MatrixSel
ControlCount=9
Control1=IDC_STATIC,static,1342308352
Control2=IDC_MATRIX_TREE,SysTreeView32,1350635575
Control3=IDCANCEL,button,1342242817
Control4=IDC_MATRIX_ADD,button,1342242816
Control5=IDC_MATRIX_PROPERTIES,button,1342242816
Control6=IDC_MATRIX_REMOVE,button,1342242816
Control7=IDC_MATRIX_COPY,button,1342242816
Control8=IDC_STATIC,button,1342177287
Control9=IDC_GRID_NOTES,edit,1352730756

[CLS:MatrixSel]
Type=0
HeaderFile=MtrxSel.h
ImplementationFile=MtrxSel.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_GRID_NOTES

[DLG:IDD_MTXEDIT_GENERAL]
Type=1
Class=MtxEditGen
ControlCount=18
Control1=IDC_STATIC,static,1342312448
Control2=IDC_GEN_NAME,edit,1350631552
Control3=IDC_STATIC,static,1342312448
Control4=IDC_GEN_CATEGORY,combobox,1344340226
Control5=IDC_STATIC,static,1342312448
Control6=IDC_GEN_AUTHOR,edit,1350631552
Control7=IDC_STATIC,static,1342312448
Control8=IDC_GEN_FILEPATH,static,1342308352
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,button,1342177287
Control11=IDC_GEN_REQPSWACCESS,button,1342242819
Control12=IDC_GEN_PSWACCESS_LBL,static,1342312448
Control13=IDC_GEN_PSWACCESS,edit,1350631584
Control14=IDC_GEN_REQPSWMODIFY,button,1342242819
Control15=IDC_GEN_PSWMODIFY_LBL,static,1342312448
Control16=IDC_GEN_PSWMODIFY,edit,1350631584
Control17=IDC_STATIC,button,1342177287
Control18=IDC_GEN_NOTES,edit,1350635716

[CLS:MtxEditGen]
Type=0
HeaderFile=MtxEdGen.h
ImplementationFile=MtxEdGen.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=MtxEditGen

[DLG:IDD_MTXEDIT_COLUMNS]
Type=1
Class=MatrixEditCol
ControlCount=10
Control1=IDC_STATIC,button,1342177287
Control2=IDC_COLS_LIST,SysListView32,1350664205
Control3=IDC_COLS_ADD,button,1342242816
Control4=IDC_COLS_PROPERTIES,button,1342242816
Control5=IDC_COLS_REMOVE,button,1342242816
Control6=IDC_COLS_MOVEUP,button,1342242816
Control7=IDC_COLS_MOVEDOWN,button,1342242816
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,static,1342312448
Control10=IDC_COLS_RETURNTYPE,combobox,1344339971

[CLS:MatrixEditCol]
Type=0
HeaderFile=MtxEdCol.h
ImplementationFile=MtxEdCol.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=IDC_COLS_LIST

[DLG:IDD_MTXEDIT_DATAVALUES]
Type=1
Class=MatrixEditData
ControlCount=5
Control1=IDC_GRID_PLACEHOLDER,static,1350696960
Control2=IDC_DATAVALUES_INSERTROW,button,1342242944
Control3=IDC_DATAVALUES_DELETEROW,button,1342242944
Control4=IDC_DATAVALUES_WILDCARD,button,1342242944
Control5=IDC_DATAVALUES_FULLSCREEN,button,1342242944

[CLS:MatrixEditData]
Type=0
HeaderFile=MtxEdDat.h
ImplementationFile=MtxEdDat.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=IDC_DATAVALUES_DELETEROW

[CLS:MatrixEdit]
Type=0
HeaderFile=MtxEdit.h
ImplementationFile=MtxEdit.cpp
BaseClass=CPropertySheet
Filter=W
VirtualFilter=hWC
LastObject=MatrixEdit

[DLG:IDD_PASSWORD_ENTRY]
Type=1
Class=PasswordEntryDlg
ControlCount=4
Control1=IDC_STATIC,static,1342312448
Control2=IDC_PSW_PASSWORD,edit,1350631584
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[CLS:PasswordEntryDlg]
Type=0
HeaderFile=PswDlg.h
ImplementationFile=PswDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_PSW_PASSWORD

[DLG:IDD_COLEDIT_POSITION]
Type=1
Class=ColPosition
ControlCount=4
Control1=IDC_STATIC,button,1342177287
Control2=IDC_COLEDIT_APPEND,button,1342177289
Control3=IDC_COLEDIT_INSERT,button,1342177289
Control4=IDC_COLEDIT_INSERTLIST,listbox,1352728833

[CLS:ColPosition]
Type=0
HeaderFile=ColPos.h
ImplementationFile=ColPos.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=ColPosition

[DLG:IDD_COLEDIT_GENERAL]
Type=1
Class=ColGeneral
ControlCount=10
Control1=IDC_STATIC,static,1342312448
Control2=IDC_COLEDIT_COLNAME,edit,1350631552
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342312448
Control5=IDC_COLEDIT_DATATYPE,combobox,1344339971
Control6=IDC_STATIC,static,1342312448
Control7=IDC_COLEDIT_COMPAREOP,combobox,1344339971
Control8=IDC_COLEDIT_COMPARECOL,button,1342242819
Control9=IDC_STATIC,button,1342177287
Control10=IDC_COLEDIT_NOTES,edit,1350635716

[CLS:ColGeneral]
Type=0
HeaderFile=ColGen.h
ImplementationFile=ColGen.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=IDC_COLEDIT_COLNAME

[DLG:IDD_COLEDIT_PICKLIST]
Type=1
Class=ColPickList
ControlCount=4
Control1=IDC_PICKLIST_USEPICKLIST,button,1342242819
Control2=IDC_PICKLIST_RESTRICT2LIST,button,1342242819
Control3=IDC_PICKLIST_CHOICES_LBL,static,1342308352
Control4=IDC_PICKLIST_CHOICES,edit,1352732804

[CLS:ColPickList]
Type=0
HeaderFile=ColPList.h
ImplementationFile=ColPList.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=ColPickList
VirtualFilter=idWC

[CLS:ColumnEdit]
Type=0
HeaderFile=ColEdit.h
ImplementationFile=ColEdit.cpp
BaseClass=CPropertySheet
Filter=W
VirtualFilter=hWC
LastObject=ColumnEdit

[DLG:IDD_COPY_MATRIX_NAME]
Type=1
Class=DuplicateMatrix
ControlCount=4
Control1=IDC_STATIC,static,1342312448
Control2=IDC_COPYMATRIX_NAME,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[CLS:DuplicateMatrix]
Type=0
HeaderFile=DupMtx.h
ImplementationFile=DupMtx.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=DuplicateMatrix

[DLG:IDD_MTXEDIT_DATAVALUESFULL]
Type=1
Class=FullScreenGrid
ControlCount=5
Control1=IDC_GRID_PLACEHOLDER,static,1350696960
Control2=IDC_DATAVALUES_INSERTROW,button,1342242944
Control3=IDC_DATAVALUES_DELETEROW,button,1342242944
Control4=IDC_DATAVALUES_WILDCARD,button,1342242944
Control5=IDCANCEL,button,1342242816

[CLS:FullScreenGrid]
Type=0
HeaderFile=FSGrid.h
ImplementationFile=FSGrid.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=FullScreenGrid

[DLG:IDD_WIZ_SELECTMATRIX]
Type=1
Class=WizGrSel
ControlCount=3
Control1=IDC_STATIC,static,1342179342
Control2=IDC_STATIC,static,1342308352
Control3=IDC_MATRIX_TREE,SysTreeView32,1350635575

[CLS:WizGrSel]
Type=0
HeaderFile=WizGrSel.h
ImplementationFile=WizGrSel.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=IDC_MATRIX_TREE
VirtualFilter=idWC

[DLG:IDD_WIZ_INSERTPOS]
Type=1
Class=WizGrPos
ControlCount=5
Control1=IDC_STATIC,static,1342179342
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342177280
Control4=IDC_STATIC,static,1342312448
Control5=IDC_INSPOS_CELL,edit,1350631552

[CLS:WizGrPos]
Type=0
HeaderFile=WizGrPos.h
ImplementationFile=WizGrPos.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=IDC_INSPOS_CELL

[CLS:GridWiz]
Type=0
HeaderFile=GridWiz.h
ImplementationFile=GridWiz.cpp
BaseClass=CPropertySheet
Filter=W
VirtualFilter=hWC
LastObject=GridWiz

[DLG:IDD_WIZGRID_RANGE]
Type=1
Class=WizGrRng
ControlCount=4
Control1=IDC_STATIC,static,1342179342
Control2=IDC_STATIC,static,1342308352
Control3=IDC_WIZRANGE_LIST,listbox,1352728833
Control4=IDC_WIZRANGE_REMOVE,button,1342242816

[CLS:WizGrRng]
Type=0
HeaderFile=WizGrRng.h
ImplementationFile=WizGrRng.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=WizGrRng

[DLG:IDD_GRIDWIZ_CHOOSER]
Type=1
Class=GridWizChooser
ControlCount=7
Control1=IDC_STATIC,button,1342177287
Control2=IDC_GRIDWIZ_CGCOLUMN,button,1342186505
Control3=IDC_GRIDWIZ_INSGRID,button,1342186505
Control4=IDC_GRIDWIZ_CGIMPORT,button,1342186505
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816
Control7=IDC_GRIDWIZ_INDICATOR,static,1342179342

[CLS:GridWizChooser]
Type=0
HeaderFile=GridWizChooser.h
ImplementationFile=GridWizChooser.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=GridWizChooser

[DLG:IDD_WIZ_FIELDDEF]
Type=1
Class=FLDProps
ControlCount=12
Control1=IDC_WIZFLD_KEYFIELD,button,1342242819
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,static,1342312448
Control4=IDC_WIZFLD_FIELDTYPE,combobox,1344339971
Control5=IDC_STATIC,static,1342312448
Control6=IDC_WIZFLD_OPERATION,combobox,1344339971
Control7=IDC_STATIC,static,1342312448
Control8=IDC_WIZFLD_CAPTION,edit,1350631552
Control9=IDC_STATIC,static,1342312448
Control10=IDC_WIZFLD_DATATYPE,combobox,1344339971
Control11=IDC_WIZFLD_COLINDICATOR,SysTabControl32,1476428032
Control12=IDC_STATIC,static,1342308353

[CLS:FLDProps]
Type=0
HeaderFile=FLDProps.h
ImplementationFile=FLDProps.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=IDC_WIZFLD_KEYFIELD

[CLS:GridFromCellWiz]
Type=0
HeaderFile=GridFromCellWiz.h
ImplementationFile=GridFromCellWiz.cpp
BaseClass=CPropertySheet
Filter=W
VirtualFilter=hWC
LastObject=GridFromCellWiz

[DLG:IDD_WIZ_MATRIXINFO]
Type=1
Class=GFCMatrixDetails
ControlCount=8
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342312448
Control3=IDC_WIZGRIDINFO_GRIDNAME,edit,1350631552
Control4=IDC_STATIC,static,1342312448
Control5=IDC_WIZGRIDINFO_RETURNTYPE,combobox,1344339971
Control6=IDC_STATIC,static,1342312448
Control7=IDC_WIZGRIDINFO_DEFVALUE,edit,1350631552
Control8=IDC_WIZGRIDINFO_LAUNCHEDITOR,button,1342242819

[CLS:GFCMatrixDetails]
Type=0
HeaderFile=GFCMatrixDetails.h
ImplementationFile=GFCMatrixDetails.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=GFCMatrixDetails
VirtualFilter=idWC

[DLG:IDD_GRWZ_GRIDFROMRANGE]
Type=1
Class=GFRSetup
ControlCount=5
Control1=IDC_STATIC,static,1342312448
Control2=IDC_GWFR_MATRIXNAME,edit,1350631552
Control3=IDC_GWFR_FIRSTROWCOLHEADERS,button,1342242819
Control4=IDOK,button,1342242817
Control5=IDCANCEL,button,1342242816

[CLS:GFRSetup]
Type=0
HeaderFile=GFRSetup.h
ImplementationFile=GFRSetup.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=GFRSetup

[CLS:FloatingGridWnd]
Type=0
HeaderFile=FlotGrid.h
ImplementationFile=FlotGrid.cpp
BaseClass=CMiniFrameWnd
Filter=T
VirtualFilter=fWC
LastObject=FloatingGridWnd

[CLS:CGridList]
Type=0
HeaderFile=GridList.h
ImplementationFile=GridList.cpp
BaseClass=CListBox
Filter=W
VirtualFilter=bWC
LastObject=CGridList

[MNU:IDR_FLOATCTXT_MENU]
Type=1
Class=?
CommandCount=0

[CLS:FloatBtn]
Type=0
HeaderFile=FloatBtn.h
ImplementationFile=FloatBtn.cpp
BaseClass=CButton
Filter=W
LastObject=FloatBtn

[CLS:FloatingValuesPage]
Type=0
HeaderFile=FlotValP.h
ImplementationFile=FlotValP.cpp
BaseClass=CMiniFrameWnd
Filter=T
VirtualFilter=fWC
LastObject=FloatingValuesPage

[CLS:FloatingThread]
Type=0
HeaderFile=FlotThrd.h
ImplementationFile=FlotThrd.cpp
BaseClass=CWinThread
Filter=N
LastObject=FloatingThread

[CLS:HiddenComWin]
Type=0
HeaderFile=HComWin.h
ImplementationFile=HComWin.cpp
BaseClass=CFrameWnd
Filter=T
VirtualFilter=fWC
LastObject=HiddenComWin

