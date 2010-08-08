VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CVBHardwareController"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'/**************************************************************************
'                 版权所有 (c), 2000 - 2010, 超级绿豆
'
' ************************   模 块 名   :CVBHardwareController********************
'版 本 号: V1.0
'作    者: 超级绿豆
'生成日期: 2004年06月04日
'最近修改:
'功能描述:
'函数列表:
'修改历史:
'日    期: 2004年06月04日
'修改人员: 超级绿豆
'修改内容: 生成
'******************************************************************************/
'
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "CVBHardwareController"

Private Type GUID
  Data1 As Long
  Data2 As Integer
  Data3 As Integer
  Data4(7) As Byte
End Type

'WINSETUPAPI
'HDEVINFO
'WINAPI
'SetupDiGetClassDevsA(
'    IN LPGUID ClassGuid,  OPTIONAL
'    IN PCSTR  Enumerator, OPTIONAL
'    IN HWND   hwndParent, OPTIONAL
'    IN DWORD  Flags
'    );
Private Declare Function SetupDiGetClassDevs Lib "setupapi" Alias "SetupDiGetClassDevsA" (ByVal ClassGuid As Long, ByVal Enumerator As Long, ByVal hWndParent As Long, ByVal Flags As Long) As Long

Private Const DIGCF_DEFAULT            As Long = &H1        '// only valid with DIGCF_DEVICEINTERFACE
Private Const DIGCF_PRESENT            As Long = &H2
Private Const DIGCF_ALLCLASSES         As Long = &H4
Private Const DIGCF_PROFILE            As Long = &H8
Private Const DIGCF_DEVICEINTERFACE    As Long = &H10

'WINSETUPAPI
'BOOL
'WINAPI
'SetupDiDestroyDeviceInfoList(
'    IN HDEVINFO DeviceInfoSet
'    );
Private Declare Function SetupDiDestroyDeviceInfoList Lib "setupapi" (ByVal DeviceInfoSet As Long) As Long

'WINSETUPAPI
'BOOL
'WINAPI
'SetupDiEnumDeviceInfo(
'    IN  HDEVINFO         DeviceInfoSet,
'    IN  DWORD            MemberIndex,
'    OUT PSP_DEVINFO_DATA DeviceInfoData
'    );
Private Declare Function SetupDiEnumDeviceInfo Lib "setupapi" (ByVal DeviceInfoSet As Long, ByVal MemberIndex As Long, DeviceInfoData As SP_DEVINFO_DATA) As Long

'typedef struct _SP_DEVINFO_DATA {
'    DWORD cbSize;
'    GUID  ClassGuid;
'    DWORD DevInst;    // DEVINST handle
'    DWORD Reserved;
'} SP_DEVINFO_DATA, *PSP_DEVINFO_DATA;
Private Type SP_DEVINFO_DATA
    cbSize As Long
    ClassGuid As GUID
    DevInst As Long
    Reserved As Long
End Type

'WINSETUPAPI
'BOOL
'WINAPI
'SetupDiGetDeviceRegistryPropertyA(
'    IN  HDEVINFO         DeviceInfoSet,
'    IN  PSP_DEVINFO_DATA DeviceInfoData,
'    IN  DWORD            Property,
'    OUT PDWORD           PropertyRegDataType, OPTIONAL
'    OUT PBYTE            PropertyBuffer,
'    IN  DWORD            PropertyBufferSize,
'    OUT PDWORD           RequiredSize         OPTIONAL
'    );
Private Declare Function SetupDiGetDeviceRegistryProperty Lib "setupapi" Alias "SetupDiGetDeviceRegistryPropertyA" (ByVal DeviceInfoSet As Long, DeviceInfoData As SP_DEVINFO_DATA, ByVal Property As Long, ByRef PropertyRegDataType As Long, ByVal PropertyBuffer As Long, ByVal PropertyBufferSize As Long, RequiredSize As Long) As Long

Private Const SPDRP_DEVICEDESC                   As Long = (&H0)        '// DeviceDesc (R/W)
Private Const SPDRP_HARDWAREID                   As Long = (&H1)        '// HardwareID (R/W)
Private Const SPDRP_COMPATIBLEIDS                As Long = (&H2)        '// CompatibleIDs (R/W)
Private Const SPDRP_UNUSED0                      As Long = (&H3)        '// unused
Private Const SPDRP_SERVICE                      As Long = (&H4)        '// Service (R/W)
Private Const SPDRP_UNUSED1                      As Long = (&H5)        '// unused
Private Const SPDRP_UNUSED2                      As Long = (&H6)        '// unused
Private Const SPDRP_CLASS                        As Long = (&H7)        '// Class (R--tied to ClassGUID)
Private Const SPDRP_CLASSGUID                    As Long = (&H8)        '// ClassGUID (R/W)
Private Const SPDRP_DRIVER                       As Long = (&H9)        '// Driver (R/W)
Private Const SPDRP_CONFIGFLAGS                  As Long = (&HA)        '// ConfigFlags (R/W)
Private Const SPDRP_MFG                          As Long = (&HB)        '// Mfg (R/W)
Private Const SPDRP_FRIENDLYNAME                 As Long = (&HC)        '// FriendlyName (R/W)
Private Const SPDRP_LOCATION_INFORMATION         As Long = (&HD)        '// LocationInformation (R/W)
Private Const SPDRP_PHYSICAL_DEVICE_OBJECT_NAME As Long = (&HE)         '// PhysicalDeviceObjectName (R)
Private Const SPDRP_CAPABILITIES                 As Long = (&HF)        '// Capabilities (R)
Private Const SPDRP_UI_NUMBER                    As Long = (&H10)       '// UiNumber (R)
Private Const SPDRP_UPPERFILTERS                 As Long = (&H11)       '// UpperFilters (R/W)
Private Const SPDRP_LOWERFILTERS                 As Long = (&H12)       '// LowerFilters (R/W)
Private Const SPDRP_BUSTYPEGUID                  As Long = (&H13)       '// BusTypeGUID (R)
Private Const SPDRP_LEGACYBUSTYPE                As Long = (&H14)       '// LegacyBusType (R)
Private Const SPDRP_BUSNUMBER                    As Long = (&H15)       '// BusNumber (R)
Private Const SPDRP_ENUMERATOR_NAME              As Long = (&H16)       '// Enumerator Name (R)
Private Const SPDRP_SECURITY                     As Long = (&H17)       '// Security (R/W, binary form)
Private Const SPDRP_SECURITY_SDS                 As Long = (&H18)       '// Security (W, SDS form)
Private Const SPDRP_DEVTYPE                      As Long = (&H19)       '// Device Type (R/W)
Private Const SPDRP_EXCLUSIVE                    As Long = (&H1A)       '// Device is exclusive-access (R/W)
Private Const SPDRP_CHARACTERISTICS              As Long = (&H1B)       '// Device Characteristics (R/W)
Private Const SPDRP_ADDRESS                      As Long = (&H1C)       '// Device Address (R)
Private Const SPDRP_UI_NUMBER_DESC_FORMAT        As Long = (&H1D)       '// UiNumberDescFormat (R/W)
Private Const SPDRP_DEVICE_POWER_DATA            As Long = (&H1E)       '// Device Power Data (R)
Private Const SPDRP_REMOVAL_POLICY               As Long = (&H1F)       '// Removal Policy (R)
Private Const SPDRP_REMOVAL_POLICY_HW_DEFAULT    As Long = (&H20)       '// Hardware Removal Policy (R)
Private Const SPDRP_REMOVAL_POLICY_OVERRIDE      As Long = (&H21)       '// Removal Policy Override (RW)
Private Const SPDRP_INSTALL_STATE                As Long = (&H22)       '// Device Install State (R)
Private Const SPDRP_MAXIMUM_PROPERTY             As Long = (&H23)       '// Upper bound on ordinals


'WINSETUPAPI
'BOOL
'WINAPI
'SetupDiSetClassInstallParamsA(
'    IN HDEVINFO                DeviceInfoSet,
'    IN PSP_DEVINFO_DATA        DeviceInfoData,        OPTIONAL
'    IN PSP_CLASSINSTALL_HEADER ClassInstallParams,    OPTIONAL
'    IN DWORD                   ClassInstallParamsSize
'    );

Private Declare Function SetupDiSetClassInstallParams Lib "setupapi" Alias "SetupDiSetClassInstallParamsA" (ByVal DeviceInfoSet As Long, DeviceInfoData As SP_DEVINFO_DATA, ClassInstallParams As SP_CLASSINSTALL_HEADER, ByVal ClassInstallParamsSize As Long) As Long

'WINSETUPAPI
'BOOL
'WINAPI
'SetupDiCallClassInstaller(
'    IN DI_FUNCTION      InstallFunction,
'    IN HDEVINFO         DeviceInfoSet,
'    IN PSP_DEVINFO_DATA DeviceInfoData OPTIONAL
'    );
Private Declare Function SetupDiCallClassInstaller Lib "setupapi" (ByVal InstallFunction As Long, ByVal DeviceInfoSet As Long, DeviceInfoData As SP_DEVINFO_DATA) As Long

'typedef struct _SP_CLASSINSTALL_HEADER {
'    DWORD       cbSize;
'    DI_FUNCTION InstallFunction;
'} SP_CLASSINSTALL_HEADER, *PSP_CLASSINSTALL_HEADER;
Private Type SP_CLASSINSTALL_HEADER
    cbSize As Long
    InstallFunction As Long
End Type

Private Const DIF_SELECTDEVICE                      As Long = &H1
Private Const DIF_INSTALLDEVICE                     As Long = &H2
Private Const DIF_ASSIGNRESOURCES                   As Long = &H3
Private Const DIF_PROPERTIES                        As Long = &H4
Private Const DIF_REMOVE                            As Long = &H5
Private Const DIF_FIRSTTIMESETUP                    As Long = &H6
Private Const DIF_FOUNDDEVICE                       As Long = &H7
Private Const DIF_SELECTCLASSDRIVERS                As Long = &H8
Private Const DIF_VALIDATECLASSDRIVERS              As Long = &H9
Private Const DIF_INSTALLCLASSDRIVERS               As Long = &HA
Private Const DIF_CALCDISKSPACE                     As Long = &HB
Private Const DIF_DESTROYPRIVATEDATA                As Long = &HC
Private Const DIF_VALIDATEDRIVER                    As Long = &HD
Private Const DIF_MOVEDEVICE                        As Long = &HE
Private Const DIF_DETECT                            As Long = &HF
Private Const DIF_INSTALLWIZARD                     As Long = &H10
Private Const DIF_DESTROYWIZARDDATA                 As Long = &H11
Private Const DIF_PROPERTYCHANGE                    As Long = &H12
Private Const DIF_ENABLECLASS                       As Long = &H13
Private Const DIF_DETECTVERIFY                      As Long = &H14
Private Const DIF_INSTALLDEVICEFILES                As Long = &H15
Private Const DIF_UNREMOVE                          As Long = &H16
Private Const DIF_SELECTBESTCOMPATDRV               As Long = &H17
Private Const DIF_ALLOW_INSTALL                     As Long = &H18
Private Const DIF_REGISTERDEVICE                    As Long = &H19
Private Const DIF_NEWDEVICEWIZARD_PRESELECT         As Long = &H1A
Private Const DIF_NEWDEVICEWIZARD_SELECT            As Long = &H1B
Private Const DIF_NEWDEVICEWIZARD_PREANALYZE        As Long = &H1C
Private Const DIF_NEWDEVICEWIZARD_POSTANALYZE       As Long = &H1D
Private Const DIF_NEWDEVICEWIZARD_FINISHINSTALL     As Long = &H1E
Private Const DIF_UNUSED1                           As Long = &H1F
Private Const DIF_INSTALLINTERFACES                 As Long = &H20
Private Const DIF_DETECTCANCEL                      As Long = &H21
Private Const DIF_REGISTER_COINSTALLERS             As Long = &H22
Private Const DIF_ADDPROPERTYPAGE_ADVANCED          As Long = &H23
Private Const DIF_ADDPROPERTYPAGE_BASIC             As Long = &H24
Private Const DIF_RESERVED1                         As Long = &H25
Private Const DIF_TROUBLESHOOTER                    As Long = &H26
Private Const DIF_POWERMESSAGEWAKE                  As Long = &H27
Private Const DIF_ADDREMOTEPROPERTYPAGE_ADVANCED    As Long = &H28
Private Const DIF_UPDATEDRIVER_UI                   As Long = &H29
Private Const DIF_RESERVED2                         As Long = &H30

'typedef struct _SP_PROPCHANGE_PARAMS {
'    SP_CLASSINSTALL_HEADER ClassInstallHeader;
'    DWORD                  StateChange;
'    DWORD                  Scope;
'    DWORD                  HwProfile;
'} SP_PROPCHANGE_PARAMS, *PSP_PROPCHANGE_PARAMS;
Private Type SP_PROPCHANGE_PARAMS
    ClassInstallHeader As SP_CLASSINSTALL_HEADER
    StateChange As Long
    Scope As Long
    HwProfile As Long
End Type
'//
'// Values indicating a change in a device's state
'//
Private Const DICS_ENABLE        As Long = &H1
Private Const DICS_DISABLE       As Long = &H2
Private Const DICS_PROPCHANGE    As Long = &H3
Private Const DICS_START         As Long = &H4
Private Const DICS_STOP          As Long = &H5
'//
'// Values specifying the scope of a device property change
'//
Private Const DICS_FLAG_GLOBAL           As Long = &H1         '// make change in all hardware profiles
Private Const DICS_FLAG_CONFIGSPECIFIC   As Long = &H2         '// make change in specified profile only
Private Const DICS_FLAG_CONFIGGENERAL    As Long = &H4         '// 1 or more hardware profile-specific
                                                               '// changes to follow.

Private GUID_DEVCLASS_1394 As GUID
Private GUID_DEVCLASS_ADAPTER As GUID
Private GUID_DEVCLASS_CDROM As GUID
Private GUID_DEVCLASS_COMPUTER As GUID
Private GUID_DEVCLASS_DECODER As GUID
Private GUID_DEVCLASS_DISKDRIVE As GUID
Private GUID_DEVCLASS_DISPLAY As GUID
Private GUID_DEVCLASS_FDC As GUID
Private GUID_DEVCLASS_GPS As GUID
Private GUID_DEVCLASS_HDC As GUID
Private GUID_DEVCLASS_IMAGE As GUID
Private GUID_DEVCLASS_INFRARED As GUID
Private GUID_DEVCLASS_KEYBOARD As GUID
Private GUID_DEVCLASS_LEGACYDRIVER As GUID
Private GUID_DEVCLASS_MEDIA As GUID
Private GUID_DEVCLASS_MODEM As GUID
Private GUID_DEVCLASS_MONITOR As GUID
Private GUID_DEVCLASS_MOUSE As GUID
Private GUID_DEVCLASS_MTD As GUID
Private GUID_DEVCLASS_MULTIFUNCTION As GUID
Private GUID_DEVCLASS_NET As GUID
Private GUID_DEVCLASS_NETCLIENT As GUID
Private GUID_DEVCLASS_NETSERVICE As GUID
Private GUID_DEVCLASS_NETTRANS As GUID
Private GUID_DEVCLASS_NODRIVER As GUID
Private GUID_DEVCLASS_PARALLE As GUID
Private GUID_DEVCLASS_PCMCIA As GUID
Private GUID_DEVCLASS_PORTS As GUID
Private GUID_DEVCLASS_PRINTER As GUID
Private GUID_DEVCLASS_PRINTERUPGRADE As GUID
Private GUID_DEVCLASS_SCSIADAPTER As GUID
Private GUID_DEVCLASS_SOUND As GUID
Private GUID_DEVCLASS_STILLIMAGE As GUID
Private GUID_DEVCLASS_SYSTEM As GUID
Private GUID_DEVCLASS_TAPEDRIVE As GUID
Private GUID_DEVCLASS_UNKNOWN As GUID
Private GUID_DEVCLASS_VOLUME As GUID

Private Const INVALID_HANDLE_VALUE As Long = -1
Private Const ERROR_INSUFFICIENT_BUFFER As Long = 122

Private Const FILE_READ_DATA = (&H1)                     '  file pipe
Private Const FILE_WRITE_DATA = (&H2)                    '  file pipe

Private Const FILE_DEVICE_BEEP                 As Long = &H1
Private Const FILE_DEVICE_CD_ROM               As Long = &H2
Private Const FILE_DEVICE_CD_ROM_FILE_SYSTEM   As Long = &H3
Private Const FILE_DEVICE_CONTROLLER           As Long = &H4
Private Const FILE_DEVICE_DATALINK             As Long = &H5
Private Const FILE_DEVICE_DFS                  As Long = &H6
Private Const FILE_DEVICE_DISK                 As Long = &H7
Private Const FILE_DEVICE_DISK_FILE_SYSTEM     As Long = &H8
Private Const FILE_DEVICE_FILE_SYSTEM          As Long = &H9
Private Const FILE_DEVICE_INPORT_PORT          As Long = &HA
Private Const FILE_DEVICE_KEYBOARD             As Long = &HB
Private Const FILE_DEVICE_MAILSLOT             As Long = &HC
Private Const FILE_DEVICE_MIDI_IN              As Long = &HD
Private Const FILE_DEVICE_MIDI_OUT             As Long = &HE
Private Const FILE_DEVICE_MOUSE                As Long = &HF
Private Const FILE_DEVICE_MULTI_UNC_PROVIDER   As Long = &H10
Private Const FILE_DEVICE_NAMED_PIPE           As Long = &H11
Private Const FILE_DEVICE_NETWORK              As Long = &H12
Private Const FILE_DEVICE_NETWORK_BROWSER      As Long = &H13
Private Const FILE_DEVICE_NETWORK_FILE_SYSTEM As Long = &H14
Private Const FILE_DEVICE_NULL                 As Long = &H15
Private Const FILE_DEVICE_PARALLEL_PORT        As Long = &H16
Private Const FILE_DEVICE_PHYSICAL_NETCARD     As Long = &H17
Private Const FILE_DEVICE_PRINTER              As Long = &H18
Private Const FILE_DEVICE_SCANNER              As Long = &H19
Private Const FILE_DEVICE_SERIAL_MOUSE_PORT    As Long = &H1A
Private Const FILE_DEVICE_SERIAL_PORT          As Long = &H1B
Private Const FILE_DEVICE_SCREEN               As Long = &H1C
Private Const FILE_DEVICE_SOUND                As Long = &H1D
Private Const FILE_DEVICE_STREAMS              As Long = &H1E
Private Const FILE_DEVICE_TAPE                 As Long = &H1F
Private Const FILE_DEVICE_TAPE_FILE_SYSTEM     As Long = &H20
Private Const FILE_DEVICE_TRANSPORT            As Long = &H21
Private Const FILE_DEVICE_UNKNOWN              As Long = &H22
Private Const FILE_DEVICE_VIDEO                As Long = &H23
Private Const FILE_DEVICE_VIRTUAL_DISK         As Long = &H24
Private Const FILE_DEVICE_WAVE_IN              As Long = &H25
Private Const FILE_DEVICE_WAVE_OUT             As Long = &H26
Private Const FILE_DEVICE_8042_PORT            As Long = &H27
Private Const FILE_DEVICE_NETWORK_REDIRECTOR   As Long = &H28
Private Const FILE_DEVICE_BATTERY              As Long = &H29
Private Const FILE_DEVICE_BUS_EXTENDER         As Long = &H2A
Private Const FILE_DEVICE_MODEM                As Long = &H2B
Private Const FILE_DEVICE_VDM                  As Long = &H2C
Private Const FILE_DEVICE_MASS_STORAGE         As Long = &H2D
Private Const FILE_DEVICE_SMB                  As Long = &H2E
Private Const FILE_DEVICE_KS                   As Long = &H2F
Private Const FILE_DEVICE_CHANGER              As Long = &H30
Private Const FILE_DEVICE_SMARTCARD            As Long = &H31
Private Const FILE_DEVICE_ACPI                 As Long = &H32
Private Const FILE_DEVICE_DVD                  As Long = &H33
Private Const FILE_DEVICE_FULLSCREEN_VIDEO     As Long = &H34
Private Const FILE_DEVICE_DFS_FILE_SYSTEM      As Long = &H35
Private Const FILE_DEVICE_DFS_VOLUME           As Long = &H36

Private Const METHOD_BUFFERED                  As Long = 0
Private Const METHOD_IN_DIRECT                 As Long = 1
Private Const METHOD_OUT_DIRECT                As Long = 2
Private Const METHOD_NEITHER                   As Long = 3

Private Const FILE_ANY_ACCESS                  As Long = 0
Private Const FILE_READ_ACCESS                 As Long = FILE_READ_DATA  '/* file & pipe */
Private Const FILE_WRITE_ACCESS                As Long = FILE_WRITE_DATA '/* file & pipe */

Private Const IOCTL_STORAGE_BASE As Long = FILE_DEVICE_MASS_STORAGE

Private Enum STORAGE_BUS_TYPE
    BusTypeUnknown = 0
    BusTypeScsi
    BusTypeAtapi
    BusTypeAta
    BusType1394
    BusTypeSsa
    BusTypeFibre
    BusTypeUsb
    BusTypeRAID
    BusTypeMaxReserved = &H7F
End Enum

Private Enum STORAGE_QUERY_TYPE
    PropertyStandardQuery = 0
    PropertyExistsQuery
    PropertyMaskQuery
    PropertyQueryMaxDefined
End Enum

Private Enum STORAGE_PROPERTY_ID
    StorageDeviceProperty = 0
    StorageAdapterProperty
End Enum

Private Type STORAGE_PROPERTY_QUERY
    PropertyId As STORAGE_PROPERTY_ID
    QueryType As STORAGE_QUERY_TYPE
    AdditionalParameters(0) As Byte
End Type

Private Type STORAGE_DEVICE_DESCRIPTOR
    Version As Long
    Size As Long
    DeviceType As Byte
    DeviceTypeModifier As Byte
    RemovableMedia As Byte
    CommandQueueing As Byte
    VendorIdOffset As Long
    ProductIdOffset As Long
    ProductRevisionOffset As Long
    SerialNumberOffset As Long
    BusType As STORAGE_BUS_TYPE
    RawPropertiesLength As Long
    RawDeviceProperties(0) As Byte
End Type

Private Declare Function GetDriveType Lib "kernel32" Alias "GetDriveTypeA" (ByVal nDrive As String) As Long
Private Declare Function CreateFile Lib "kernel32" Alias "CreateFileA" (ByVal lpFileName As String, ByVal dwDesiredAccess As Long, ByVal dwShareMode As Long, lpSecurityAttributes As SECURITY_ATTRIBUTES, ByVal dwCreationDisposition As Long, ByVal dwFlagsAndAttributes As Long, ByVal hTemplateFile As Long) As Long
Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long
Private Declare Function DeviceIoControl Lib "kernel32" (ByVal hDevice As Long, ByVal dwIoControlCode As Long, lpInBuffer As Any, ByVal nInBufferSize As Long, lpOutBuffer As Any, ByVal nOutBufferSize As Long, lpBytesReturned As Long, lpOverlapped As OVERLAPPED) As Long

Private Const DRIVE_CDROM = 5
Private Const DRIVE_FIXED = 3
Private Const DRIVE_RAMDISK = 6
Private Const DRIVE_REMOTE = 4
Private Const DRIVE_REMOVABLE = 2

Private Const GENERIC_READ = &H80000000

Private Const FILE_SHARE_READ = &H1
Private Const FILE_SHARE_WRITE = &H2
Private Const OPEN_EXISTING = 3

Private Type SECURITY_ATTRIBUTES
        nLength As Long
        lpSecurityDescriptor As Long
        bInheritHandle As Long
End Type

Private Type OVERLAPPED
        Internal As Long
        InternalHigh As Long
        offset As Long
        OffsetHigh As Long
        hEvent As Long
End Type

Private Declare Function GetLogicalDrives Lib "kernel32" () As Long

'**************************************************************************
'*                                暴露的接口                              *
'**************************************************************************
Public Event EnumDevicesProc(ByVal lIndex As Long, ByVal sDeviceName As String, ByVal sDeviceClassName As String, ByVal sDeviceFriendlyName As String, ByVal sDeviceEnumeratorName As String, ByVal sDeviceClassGuid As String)

Public Function EnumDevices(Optional lClassName As Long, Optional fEnumerateInterfaces As Boolean = False) As Boolean
    On Error GoTo Error_Handler
    Dim hDevInfo As Long
    If fEnumerateInterfaces Then
    Else
        hDevInfo = SetupDiGetClassDevs(0, 0, 0, DIGCF_PRESENT + DIGCF_ALLCLASSES)
        If hDevInfo <> INVALID_HANDLE_VALUE Then
            Dim DeviceInfoData  As SP_DEVINFO_DATA
            Dim i As Long
            Dim bDevInfo() As Byte
            Dim sDeviceDescription As String, sDeviceClassName As String, sDeviceEnumeratorName As String, sDeviceFriendlyName As String, sDeviceClassGuid As String
            DeviceInfoData.cbSize = Len(DeviceInfoData)
            i = 0
            Do While (SetupDiEnumDeviceInfo(hDevInfo, i, DeviceInfoData) <> 0)
                sDeviceDescription = ""
                sDeviceClassName = ""
                sDeviceEnumeratorName = ""
                sDeviceFriendlyName = ""
                sDeviceClassGuid = ""
                
                If GetDeviceRegInfo(hDevInfo, DeviceInfoData, SPDRP_DEVICEDESC, bDevInfo()) Then
                    sDeviceDescription = StrConv(bDevInfo, vbUnicode)
                End If
                
                If GetDeviceRegInfo(hDevInfo, DeviceInfoData, SPDRP_CLASS, bDevInfo()) Then
                    sDeviceClassName = StrConv(bDevInfo, vbUnicode)
                End If
                                
                If GetDeviceRegInfo(hDevInfo, DeviceInfoData, SPDRP_ENUMERATOR_NAME, bDevInfo()) Then
                    sDeviceEnumeratorName = StrConv(bDevInfo, vbUnicode)
                End If
                                                
                If GetDeviceRegInfo(hDevInfo, DeviceInfoData, SPDRP_FRIENDLYNAME, bDevInfo()) Then
                    sDeviceFriendlyName = StrConv(bDevInfo, vbUnicode)
                End If
                                                
                If GetDeviceRegInfo(hDevInfo, DeviceInfoData, SPDRP_CLASSGUID, bDevInfo()) Then
                    sDeviceClassGuid = StrConv(bDevInfo, vbUnicode)
                End If
                                                
                RaiseEvent EnumDevicesProc(i, sDeviceDescription, sDeviceClassName, sDeviceFriendlyName, sDeviceEnumeratorName, sDeviceClassGuid)
                
                i = i + 1
            Loop
                        
            Call SetupDiDestroyDeviceInfoList(hDevInfo)
        End If
    End If
    Exit Function
Error_Handler:
End Function

Public Function SetDeviceState(ByVal lDeviceIndex As Long, ByVal sDeviceClassGuid As String, Optional ByVal fEnabled As Boolean = True) As Boolean
    Dim hDevInfo As Long
    hDevInfo = SetupDiGetClassDevs(0, 0, 0, DIGCF_PRESENT + DIGCF_ALLCLASSES)
    If hDevInfo <> INVALID_HANDLE_VALUE Then
        Dim DeviceInfoData  As SP_DEVINFO_DATA
        Dim bDevInfo() As Byte
        
        DeviceInfoData.cbSize = Len(DeviceInfoData)
        If SetupDiEnumDeviceInfo(hDevInfo, lDeviceIndex, DeviceInfoData) <> 0 Then
            If GetDeviceRegInfo(hDevInfo, DeviceInfoData, SPDRP_CLASSGUID, bDevInfo()) Then
                If sDeviceClassGuid = StrConv(bDevInfo, vbUnicode) Then
                    Dim PropChangeParams  As SP_PROPCHANGE_PARAMS
                    
                    With PropChangeParams
                        .ClassInstallHeader.cbSize = Len(.ClassInstallHeader)
                        .ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE
                        .Scope = DICS_FLAG_GLOBAL
                        .StateChange = IIf(fEnabled, DICS_ENABLE, DICS_DISABLE)
                        .HwProfile = 0
                    End With
                    
                    If SetupDiSetClassInstallParams(hDevInfo, DeviceInfoData, PropChangeParams.ClassInstallHeader, Len(PropChangeParams)) <> 0 Then
                        With PropChangeParams
                            .ClassInstallHeader.cbSize = Len(.ClassInstallHeader)
                            .ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE
                            .Scope = DICS_FLAG_CONFIGSPECIFIC
                            .StateChange = IIf(fEnabled, DICS_ENABLE, DICS_DISABLE)
                            .HwProfile = 0
                        End With
                        If SetupDiSetClassInstallParams(hDevInfo, DeviceInfoData, PropChangeParams.ClassInstallHeader, Len(PropChangeParams)) <> 0 Then
                            If SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, hDevInfo, DeviceInfoData) <> 0 Then
                                SetDeviceState = True
                            End If
                        End If
                    End If
                End If
            End If
        End If
                    
        Call SetupDiDestroyDeviceInfoList(hDevInfo)
    End If
    Exit Function
Error_Handler:
End Function

Public Function EjectMedia(ByVal sDriveLetter As String, Optional ByVal fEject As Boolean = True) As Long
    Dim hDevice As Long
    Dim ut As SECURITY_ATTRIBUTES
    Dim uto As OVERLAPPED
    Dim utDevDesc As STORAGE_DEVICE_DESCRIPTOR
    hDevice = CreateFile("\\.\" & sDriveLetter, GENERIC_READ, FILE_SHARE_READ Or FILE_SHARE_WRITE, ut, OPEN_EXISTING, 0, 0)
    If hDevice <> -1 Then
        If fEject Then
            EjectMedia = DeviceIoControl(hDevice, IOCTL_STORAGE_EJECT_MEDIA, ByVal 0, 0, ByVal 0, 0, ByVal 0, uto)
        Else
            EjectMedia = DeviceIoControl(hDevice, IOCTL_STORAGE_LOAD_MEDIA, ByVal 0, 0, ByVal 0, 0, ByVal 0, uto)
        End If
        Call CloseHandle(hDevice)
    End If
End Function

Public Function GetDriveBusType(ByVal sDriveLetter As String) As String
    Dim hDevice As Long
    Dim ut As SECURITY_ATTRIBUTES
    Dim utDevDesc As STORAGE_DEVICE_DESCRIPTOR
    hDevice = CreateFile("\\.\" & sDriveLetter, GENERIC_READ, FILE_SHARE_READ Or FILE_SHARE_WRITE, ut, OPEN_EXISTING, 0, 0)
    If hDevice <> -1 Then
        utDevDesc.Size = LenB(utDevDesc)
        Call GetDisksProperty(hDevice, utDevDesc)
        Select Case utDevDesc.BusType
            Case BusType1394
                GetDriveBusType = "1394"
            Case BusTypeAta
                GetDriveBusType = "Ata"
            Case BusTypeAtapi
                GetDriveBusType = "Atapi"
            Case BusTypeFibre
                GetDriveBusType = "Fibre"
            Case BusTypeRAID
                GetDriveBusType = "RAID"
            Case BusTypeScsi
                GetDriveBusType = "Scsi"
            Case BusTypeSsa
                GetDriveBusType = "Ssa"
            Case BusTypeUsb
                GetDriveBusType = "Usb"
            Case BusTypeUnknown
                GetDriveBusType = "未知"
            Case Else
        End Select
        Call CloseHandle(hDevice)
    End If
End Function

Public Function TellDriveType(ByVal sDriveLetter As String) As String
    
    Select Case GetDriveType(sDriveLetter)
        Case 0
            TellDriveType = "驱动器类型无法确定"
        Case 1
            TellDriveType = "驱动器根目录不存在"
        Case DRIVE_CDROM
            TellDriveType = "光盘驱动器"
        Case DRIVE_FIXED
            TellDriveType = "固定驱动器"
        Case DRIVE_RAMDISK
            TellDriveType = "RAM盘"
        Case DRIVE_REMOTE
            TellDriveType = "远程（网络）驱动器"
        Case DRIVE_REMOVABLE
            If UCase$(Left$(sDriveLetter, 1)) = "A" Or UCase$(Left$(sDriveLetter, 1)) = "B" Then
                TellDriveType = "软盘"
            Else
                TellDriveType = "其他"
            End If
            TellDriveType = "可移动驱动器 - " & TellDriveType
        Case Else
            TellDriveType = "未知"
    End Select
    TellDriveType = TellDriveType & " - " & GetDriveBusType(sDriveLetter) & "总线"
End Function

Public Function GetDriveNames(Optional cbo As ComboBox) As String()
    On Error Resume Next
    Dim asDrives() As String
    Dim lDriveBits As Long
    Dim i As Long, lCnt As Long
    Dim fMissing As Boolean
    fMissing = cbo Is Nothing
    
    lCnt = -1
    lDriveBits = GetLogicalDrives()
    For i = 1 To 26
        If (lDriveBits And 2 ^ (i - 1)) <> 0 Then
            lCnt = lCnt + 1
            ReDim asDrives(lCnt)
            asDrives(lCnt) = Chr$(65 + i - 1) & ":"
            If Not fMissing Then
                cbo.AddItem asDrives(lCnt)
            End If
        End If
    Next
    GetDriveNames = asDrives
End Function
'**************************************************************************
'*                                暴露的接口                              *
'**************************************************************************


'**************************************************************************
'******************************** 私有函数 ********************************
'**************************************************************************
Private Function CTL_CODE(ByVal lDeviceType As Long, ByVal lFunction As Long, ByVal lMethod As Long, ByVal lAccess As Long) As Long
    CTL_CODE = (lDeviceType * 2 ^ 16&) Or (lAccess * 2 ^ 14&) Or (lFunction * 2 ^ 2) Or (lMethod)
End Function

Private Function IOCTL_STORAGE_QUERY_PROPERTY() As Long
    IOCTL_STORAGE_QUERY_PROPERTY = CTL_CODE(IOCTL_STORAGE_BASE, &H500, METHOD_BUFFERED, FILE_ANY_ACCESS)
End Function

Private Function IOCTL_STORAGE_EJECT_MEDIA() As Long
    IOCTL_STORAGE_EJECT_MEDIA = CTL_CODE(IOCTL_STORAGE_BASE, &H202, METHOD_BUFFERED, FILE_READ_ACCESS)
End Function

Private Function IOCTL_STORAGE_LOAD_MEDIA() As Long
    IOCTL_STORAGE_LOAD_MEDIA = CTL_CODE(IOCTL_STORAGE_BASE, &H203, METHOD_BUFFERED, FILE_READ_ACCESS)
End Function

Private Function GetDisksProperty(ByVal hDevice As Long, utDevDesc As STORAGE_DEVICE_DESCRIPTOR) As Boolean
    Dim ut As OVERLAPPED
    Dim utQuery As STORAGE_PROPERTY_QUERY
    Dim lOutBytes As Long
    With utQuery
        .PropertyId = StorageDeviceProperty
        .QueryType = PropertyStandardQuery
    End With
    GetDisksProperty = DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY, utQuery, LenB(utQuery), utDevDesc, LenB(utDevDesc), lOutBytes, ut)
End Function

Private Function GetDeviceRegInfo(ByVal hDevInfo As Long, DeviceInfoData As SP_DEVINFO_DATA, ByVal lPropertyName As Long, bDevInfo() As Byte) As Boolean
    Dim lBufferSize As Long
    Dim lRegDataType As Long
    Call SetupDiGetDeviceRegistryProperty(hDevInfo, DeviceInfoData, lPropertyName, lRegDataType, 0, 0, lBufferSize)
    If Err.LastDllError = ERROR_INSUFFICIENT_BUFFER Then
        ReDim bDevInfo(lBufferSize * 2 - 1)
        Call SetupDiGetDeviceRegistryProperty(hDevInfo, DeviceInfoData, lPropertyName, lRegDataType, VarPtr(bDevInfo(0)), lBufferSize, ByVal 0)
        GetDeviceRegInfo = True
    End If
End Function

Friend Sub DEFINE_GUID(udtGuid As GUID, ByVal Data1 As Long, Data2 As Integer, Data3 As Integer, Data4_0 As Byte, Data4_1 As Byte, Data4_2 As Byte, Data4_3 As Byte, Data4_4 As Byte, Data4_5 As Byte, Data4_6 As Byte, Data4_7 As Byte)
    With udtGuid
        .Data1 = Data1
        .Data2 = Data2
        .Data3 = Data3
        .Data4(0) = Data4_0
        .Data4(1) = Data4_1
        .Data4(2) = Data4_2
        .Data4(3) = Data4_3
        .Data4(4) = Data4_4
        .Data4(5) = Data4_5
        .Data4(6) = Data4_6
        .Data4(7) = Data4_7
    End With
End Sub

Private Sub InitGuids()
    Call DEFINE_GUID(GUID_DEVCLASS_1394, &H6BDD1FC1, &H810F, &H11D0, &HBE, &HC7, &H8, &H0, &H2B, &HE2, &H9, &H2F)
    Call DEFINE_GUID(GUID_DEVCLASS_ADAPTER, &H4D36E964, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_CDROM, &H4D36E965, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_COMPUTER, &H4D36E966, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_DECODER, &H6BDD1FC2, &H810F, &H11D0, &HBE, &HC7, &H8, &H0, &H2B, &HE2, &H9, &H2F)
    Call DEFINE_GUID(GUID_DEVCLASS_DISKDRIVE, &H4D36E967, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_DISPLAY, &H4D36E968, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_FDC, &H4D36E969, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_GPS, &H6BDD1FC3, &H810F, &H11D0, &HBE, &HC7, &H8, &H0, &H2B, &HE2, &H9, &H2F)
    Call DEFINE_GUID(GUID_DEVCLASS_HDC, &H4D36E96A, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_IMAGE, &H6BDD1FC4, &H810F, &H11D0, &HBE, &HC7, &H8, &H0, &H2B, &HE2, &H9, &H2F)
    Call DEFINE_GUID(GUID_DEVCLASS_INFRARED, &H6BDD1FC5, &H810F, &H11D0, &HBE, &HC7, &H8, &H0, &H2B, &HE2, &H9, &H2F)
    Call DEFINE_GUID(GUID_DEVCLASS_KEYBOARD, &H4D36E96B, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_LEGACYDRIVER, &H8ECC055D, &H47F, &H11D1, &HA5, &H37, &H0, &H0, &HF8, &H75, &H3E, &HD1)
    Call DEFINE_GUID(GUID_DEVCLASS_MEDIA, &H4D36E96C, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_MODEM, &H4D36E96D, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_MONITOR, &H4D36E96E, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_MOUSE, &H4D36E96F, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_MTD, &H4D36E970, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_MULTIFUNCTION, &H4D36E971, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_NET, &H4D36E972, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_NETCLIENT, &H4D36E973, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_NETSERVICE, &H4D36E974, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_NETTRANS, &H4D36E975, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_NODRIVER, &H4D36E976, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_PARALLE, &H811FC6A5, &HF728, &H11D0, &HA5, &H37, &H0, &H0, &HF8, &H75, &H3E, &HD1)
    Call DEFINE_GUID(GUID_DEVCLASS_PCMCIA, &H4D36E977, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_PORTS, &H4D36E978, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_PRINTER, &H4D36E979, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_PRINTERUPGRADE, &H4D36E97A, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_SCSIADAPTER, &H4D36E97B, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_SOUND, &H4D36E97C, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_STILLIMAGE, &H6BDD1FC6, &H810F, &H11D0, &HBE, &HC7, &H8, &H0, &H2B, &HE2, &H9, &H2F)
    Call DEFINE_GUID(GUID_DEVCLASS_SYSTEM, &H4D36E97D, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_TAPEDRIVE, &H6D807884, &H7D21, &H11CF, &H80, &H1C, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_UNKNOWN, &H4D36E97E, &HE325, &H11CE, &HBF, &HC1, &H8, &H0, &H2B, &HE1, &H3, &H18)
    Call DEFINE_GUID(GUID_DEVCLASS_VOLUME, &H71A27CDD, &H812A, &H11D0, &HBE, &HC7, &H8, &H0, &H2B, &HE2, &H9, &H2F)
End Sub

Private Sub Class_Initialize()
    Call InitGuids
End Sub


'**************************************************************************
'******************************** 私有函数 ********************************
'**************************************************************************

