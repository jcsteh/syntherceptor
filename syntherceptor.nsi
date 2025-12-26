; Syntherceptor
; NSIS installer script
; Copyright 2025 James Teh
; License: GNU General Public License version 2.0

; We don't really need MUI2, but without it, the uninstaller is corrupt when
; built on GitHub Actions. See:
; https://stackoverflow.com/questions/75525495/nsis-installer-produces-an-app-that-cant-run-this-app-cant-run-on-your-pc-t
!include "MUI2.nsh"

Name "Syntherceptor"
OutFile "${OUTFILE}"
InstallDir "$PROGRAMFILES64\syntherceptor"
RequestExecutionLevel admin

Page directory
!insertmacro MUI_PAGE_INSTFILES

!insertmacro MUI_UNPAGE_INSTFILES

Section "Install"
	SetOutPath "$INSTDIR"

	CreateDirectory "$INSTDIR\x86"
	CreateDirectory "$INSTDIR\x64"

	File /oname=$INSTDIR\x86\syntherceptor.dll "build\x86\syntherceptor.dll"
	File /oname=$INSTDIR\x86\nvdaControllerClient.dll "deps\nvdaControllerClient\x86\nvdaControllerClient.dll"

	File /oname=$INSTDIR\x64\syntherceptor.dll "build\x86_64\syntherceptor.dll"
	File /oname=$INSTDIR\x64\nvdaControllerClient.dll "deps\nvdaControllerClient\x64\nvdaControllerClient.dll"

	WriteUninstaller "$INSTDIR\Uninstall.exe"

	; Add/Remove Programs entry
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Syntherceptor" "DisplayName" "Syntherceptor"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Syntherceptor" "UninstallString" "$INSTDIR\Uninstall.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Syntherceptor" "InstallLocation" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Syntherceptor" "Publisher" "James Teh"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Syntherceptor" "DisplayVersion" "1.0"

	; 32-bit SAPI keys
	SetRegView 32
	WriteRegStr HKLM "SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor" "" "Syntherceptor"
	WriteRegStr HKLM "SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor" "CLSID" "{a2f5f730-cf28-4ac7-8f7f-0e69a6d10c35}"
	WriteRegStr HKLM "SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor" "VoiceName" "en"

	WriteRegStr HKLM "SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor\Attributes" "Age" "Adult"
	WriteRegStr HKLM "SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor\Attributes" "Gender" "Male"
	WriteRegStr HKLM "SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor\Attributes" "Language" "9"
	WriteRegStr HKLM "SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor\Attributes" "Vendor" "James Teh"
	WriteRegStr HKLM "SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor\Attributes" "Version" "1.0"

	WriteRegStr HKLM "SOFTWARE\Classes\CLSID\{a2f5f730-cf28-4ac7-8f7f-0e69a6d10c35}" "" "syntherceptor.ttsEngine"
	WriteRegStr HKLM "SOFTWARE\Classes\CLSID\{a2f5f730-cf28-4ac7-8f7f-0e69a6d10c35}\InprocServer32" "" "$INSTDIR\x86\syntherceptor.dll"
	WriteRegStr HKLM "SOFTWARE\Classes\CLSID\{a2f5f730-cf28-4ac7-8f7f-0e69a6d10c35}\InprocServer32" "ThreadingModel" "Both"

	; 64-bit SAPI keys
	SetRegView 64
	WriteRegStr HKLM "SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor" "" "Syntherceptor"
	WriteRegStr HKLM "SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor" "CLSID" "{a2f5f730-cf28-4ac7-8f7f-0e69a6d10c35}"
	WriteRegStr HKLM "SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor" "VoiceName" "en"

	WriteRegStr HKLM "SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor\Attributes" "Age" "Adult"
	WriteRegStr HKLM "SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor\Attributes" "Gender" "Male"
	WriteRegStr HKLM "SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor\Attributes" "Language" "9"
	WriteRegStr HKLM "SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor\Attributes" "Vendor" "James Teh"
	WriteRegStr HKLM "SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor\Attributes" "Version" "1.0"

	WriteRegStr HKLM "SOFTWARE\Classes\CLSID\{a2f5f730-cf28-4ac7-8f7f-0e69a6d10c35}" "" "syntherceptor.ttsEngine"
	WriteRegStr HKLM "SOFTWARE\Classes\CLSID\{a2f5f730-cf28-4ac7-8f7f-0e69a6d10c35}\InprocServer32" "" "$INSTDIR\x64\syntherceptor.dll"
	WriteRegStr HKLM "SOFTWARE\Classes\CLSID\{a2f5f730-cf28-4ac7-8f7f-0e69a6d10c35}\InprocServer32" "ThreadingModel" "Both"

	MessageBox MB_YESNO|MB_ICONQUESTION "Set Syntherceptor as the default SAPI voice for this user?" IDYES setDefault IDNO skipDefault

setDefault:
	SetRegView 64
	WriteRegStr HKCU "SOFTWARE\Microsoft\Speech\Voices" "DefaultTokenId" "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor"
	SetRegView 32
	WriteRegStr HKCU "SOFTWARE\Microsoft\Speech\Voices" "DefaultTokenId" "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor"

skipDefault:
SectionEnd

Section "Uninstall"
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Syntherceptor"

	SetRegView 32
	DeleteRegKey HKLM "SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor"
	DeleteRegKey HKLM "SOFTWARE\Classes\CLSID\{a2f5f730-cf28-4ac7-8f7f-0e69a6d10c35}"

	SetRegView 64
	DeleteRegKey HKLM "SOFTWARE\Microsoft\Speech\Voices\Tokens\syntherceptor"
	DeleteRegKey HKLM "SOFTWARE\Classes\CLSID\{a2f5f730-cf28-4ac7-8f7f-0e69a6d10c35}"

	Delete "$INSTDIR\x86\syntherceptor.dll"
	Delete "$INSTDIR\x86\nvdaControllerClient.dll"

	Delete "$INSTDIR\x64\syntherceptor.dll"
	Delete "$INSTDIR\x64\nvdaControllerClient.dll"

	Delete "$INSTDIR\Uninstall.exe"

	RMDir "$INSTDIR\x86"
	RMDir "$INSTDIR\x64"
	RMDir "$INSTDIR"
SectionEnd
