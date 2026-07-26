[Setup]
AppName=Scrabble.c
AppVersion=1.0.0
AppPublisher=Sonoth Amin
; 1. Creates 'C:\Program Files\Scrabble.c' directory
DefaultDirName={autopf}\Scrabble.c
DefaultGroupName=Scrabble.c
OutputDir=Output
OutputBaseFilename=Scrabble_Setup
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
SetupIconFile=src\icon.ico

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[Files]
Source: "bin\release\Scrabble.exe"; DestDir: "{app}"; Flags: ignoreversion
; Source: "bin\release\*.dll"; DestDir: "{app}"; Flags: ignoreversion skipifsilent
Source: "resources\*"; DestDir: "{app}\resources"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
;Create Start Menu entry
Name: "{group}\Scrabble.c"; Filename: "{app}\Scrabble.exe"; IconFilename: "{app}\Scrabble.exe"
Name: "{group}\Uninstall Scrabble.c"; Filename: "{uninstallexe}"

; 5. Optionally add shortcut to Desktop
Name: "{autodesktop}\Scrabble.c"; Filename: "{app}\Scrabble.exe"; IconFilename: "{app}\Scrabble.exe"; Tasks: desktopicon

[Run]
; 5. Optionally launch game when installer completes
Filename: "{app}\Scrabble.exe"; Description: "{cm:LaunchProgram,Scrabble.c}"; Flags: nowait postinstall skipifsilent