; 脚本由 Inno Setup 脚本向导 生成！
; 有关创建 Inno Setup 脚本文件的详细资料请查阅帮助文档！

[Setup]
AppName=FreeEIM
AppVerName=FreeEIM 2.0
AppPublisher=FEIM Studios
AppPublisherURL=http://www.freeeim.com/
AppSupportURL=http://www.freeeim.com/
AppUpdatesURL=http://www.freeeim.com/
DefaultDirName={pf}\FreeEIM
DefaultGroupName=FreeEIM 沟通专家
LicenseFile=C:\Documents and Settings\skon\桌面\setup\Readme.txt
OutputDir=C:\Documents and Settings\skon\桌面\setup
OutputBaseFilename=setup
SetupIconFile=C:\Documents and Settings\skon\桌面\20080410-FreeEIM\res\EIM02.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "chinese"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Documents and Settings\skon\桌面\setup\freeeim.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Documents and Settings\skon\桌面\setup\Readme.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Documents and Settings\skon\桌面\setup\eimseh.dll"; DestDir: "{app}"; Flags: ignoreversion
; 注意: 不要在任何共享系统文件上使用“Flags: ignoreversion”

[INI]
Filename: "{app}\freeeim.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.freeeim.com/"

[Icons]
Name: "{group}\FreeEIM"; Filename: "{app}\freeeim.exe"
Name: "{group}\{cm:ProgramOnTheWeb,FreeEIM}"; Filename: "{app}\freeeim.url"
Name: "{group}\{cm:UninstallProgram,FreeEIM}"; Filename: "{uninstallexe}"
Name: "{userdesktop}\FreeEIM"; Filename: "{app}\freeeim.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\FreeEIM"; Filename: "{app}\freeeim.exe"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\freeeim.exe"; Description: "{cm:LaunchProgram,FreeEIM}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\freeeim.url"

