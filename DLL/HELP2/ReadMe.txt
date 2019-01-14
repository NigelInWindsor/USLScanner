The only files you need are 


htmlhelp.h		contains the prototypes for the htmlApi functions required for the CHelp class that goes into USLScanner project
htmlhelp.lib		The library files for the HTML APi functions this need to be added to the project and in projectsettings
USLScanner_Help.chm	This is the compiled help file, contains all the help, needs to reside in the same directory as the USCanner.exe file
Help.ogx		The CHelp class


The Help directory contains all the individual pages that was used to produce the USLScanner_Help.chm file. You don't require this unless you want to change the help.

Htmlhelp.exe is a setup file that will install htmlhelp program allowing you to open and edit the compiled help. You don't require this unless you want to change the help.


To use the CHelp class just call Open, if you call open with a page name it will open that page name some examples below:


CHelp	m_Help; //Located in USLScanner.h a global help object of Class help

App->m_Help.Open(_T("OrientationDlg.html"),NULL); //This will open The page detailing the Orientation dialog

App->m_Help.Open(); //This will open The help at the first page