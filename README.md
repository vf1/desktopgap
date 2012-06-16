DesktopGap for Windows
======================
chromiumembedded is used as host instead of WebBrowser control (IE).

How to build .msi installer of your application on VC2010
---------------------------------------------------------

### Create setup project in VC

### Donwload and Add MergeModule.msm to the project
https://github.com/downloads/vf1/desktopgap/MergeModule.msm
Right-click on the project in Solution explorer, select Add Merge Module

### Zip your HTML application
Archive name is DesktopGap.zip

### Add DesktopGap.zip to the setup project

Create shortcuts
----------------

You need to call DesktopGap.exe to start application. But VC setup project 
can not create shortcut to .msm's files. You could use ShellExecute.exe tiny
tool to call DesktopGap.exe. Add ShellExecute.exe to setup project,
create link to it and set argument 'DesktopGap.exe' string.
