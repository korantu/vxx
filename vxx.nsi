; example2.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------

; The name of the installer
Name "Vxx Surface Tweaker"

; The file to write
OutFile "VxxSetup.exe"

; The default installation directory
InstallDir $PROGRAMFILES\Vxx

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
;InstallDirRegKey HKLM "Software\NSIS_Example2" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel user

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "Vxx (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "vxx.exe"
  ;File "vxx.pdf"
  File "vxx.ico"
  File "cygwin1.dll"
  File "cygz.dll"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\Vxx "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Vxx" "" "Vxx Editor"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Vxx" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Vxx" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Vxx" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Example data"

  SetOutPath $INSTDIR
  
  ; Put file there
  File /r "example_data"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Vxx"
  CreateShortCut "$SMPROGRAMS\Vxx\Vxx.lnk" "$INSTDIR\vxx.exe" "example_data" "$INSTDIR\vxx.ico" 0
  CreateShortCut "$SMPROGRAMS\Vxx\Vxx Documentation.lnk" "http://sites.google.com/site/vxxsoftware/" "" "" 0
  CreateShortCut "$SMPROGRAMS\Vxx\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Vxx"
  DeleteRegKey HKLM SOFTWARE\Vxx

  ; Remove files and uninstaller
  Delete $INSTDIR\vxx.exe
  Delete $INSTDIR\vxx.pdf
  Delete $INSTDIR\vxx.ico
  Delete $INSTDIR\cygwin1.dll
  Delete $INSTDIR\cygz.dll
  RMDir /r $INSTDIR\example_data
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\Vxx\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\Vxx"
  RMDir "$INSTDIR"

SectionEnd
