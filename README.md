DesktopGap for Windows
======================
chromiumembedded is used as host instead of WebBrowser control (IE).

How to build .msi installer of your application on VC2010
---------------------------------------------------------

### Donwload MergeModule.msm
https://github.com/downloads/vf1/desktopgap/MergeModule.msm

### Create setup project in VC

### Add MergeModule.msm to the project
Right-click on the project in Solution explorer, select Add Merge Module

### Zip your HTML application
Archive name is DesktopGap.zip

### Add DesktopGap.zip to the setup project

Create shortcuts
----------------

The VC setup project can not create shortcut to .msm files. You need to call
DesktopGap.exe to start application. You could add .bat file or create simple
shim application.