#include "stdafx.h"

using namespace System;
using namespace System::Reflection;
using namespace System::Runtime::CompilerServices;
using namespace System::Runtime::InteropServices;
using namespace System::Security::Permissions;

//
// General Information about an assembly is controlled through the following
// set of attributes. Change these attribute values to modify the information
// associated with an assembly.
//
[assembly:AssemblyTitleAttribute("csDriverOEMPA")];
[assembly:AssemblyDescriptionAttribute("")];
[assembly:AssemblyConfigurationAttribute("")];
[assembly:AssemblyCompanyAttribute("")];
[assembly:AssemblyProductAttribute("csDriverOEMPA")];
[assembly:AssemblyCopyrightAttribute("Copyright (c)  2013")];
[assembly:AssemblyTrademarkAttribute("")];
[assembly:AssemblyCultureAttribute("")];

//
// Version information for an assembly consists of the following four values:
//
//      Major Version
//      Minor Version
//      Build Number
//      Revision
//
// You can specify all the value or you can default the Revision and Build Numbers
// by using the '*' as shown below:

[assembly:AssemblyVersionAttribute("1.1.5.4")];

[assembly:ComVisible(false)];

[assembly:CLSCompliantAttribute(true)];

[assembly:SecurityPermission(SecurityAction::RequestMinimum, UnmanagedCode = true)];

//https://msdn.microsoft.com/en-us/library/xwb8f617(v=vs.110).aspx
//http://www.codeproject.com/Articles/8874/Strong-Names-Explained
//https://msdn.microsoft.com/fr-fr/library/51ket42z(v=vs.110).aspx
//https://msdn.microsoft.com/en-us/library/1w45z383(v=vs.110).aspx
//http://www.codeproject.com/Articles/10721/Editing-an-assembly-s-manifest-and-more
//http://www.codeproject.com/Articles/35678/How-to-sign-C-CLI-assemblies-with-a-strong-name
//http://stackoverflow.com/questions/2310246/when-should-we-not-create-assemblys-strong-name-what-are-the-disadvantages-of
//http://stackoverflow.com/questions/667017/how-to-check-if-a-file-has-a-digital-signature
//https://msdn.microsoft.com/fr-fr/library/ms235305.aspx
//http://stackoverflow.com/questions/21257664/add-company-name-product-name-etc-to-mixed-mode-assembly-dll
//http://stackoverflow.com/questions/3929540/managed-c-strong-name-signing
//http://stackoverflow.com/questions/2656412/is-there-an-easy-way-to-sign-a-c-cli-assembly-in-vs-2010
//[assembly:AssemblyKeyFileAttribute("sgKey.snk")];
//sn -Ra "$(TargetPath)" $(ProjectName).snk
//sn -e "$(TargetPath)" $(ProjectName).bin			<= TO CHECK