; Script for Inno Setup

[Setup]
AppName=live-app
AppVersion=1.0.0
DefaultDirName={pf}\live-app
DefaultGroupName=live-app
UninstallDisplayIcon={app}\live-app.exe
Compression=lzma2
SolidCompression=yes
WizardStyle=modern

[Files]
Source: "..\src\LiveApp.UI.CSharp\bin\Release\net6.0-windows\*.dll"; DestDir: "{app}"
Source: "..\src\LiveApp.UI.CSharp\bin\Release\net6.0-windows\*.exe"; DestDir: "{app}"
Source: "..\src\LiveApp.Core.Cpp\x64\Release\*.dll"; DestDir: "{app}"

[Icons]
Name: "{group}\live-app"; Filename: "{app}\LiveApp.UI.CSharp.exe"
Name: "{commondesktop}\live-app"; Filename: "{app}\LiveApp.UI.CSharp.exe"

[Run]
Filename: "{app}\LiveApp.UI.CSharp.exe"; Description: "Launch live-app"; Flags: nowait postinstall skipifsilent
