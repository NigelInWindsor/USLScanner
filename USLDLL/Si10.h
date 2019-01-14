/********************************************************************************
   File name: DLLCode.h

   This file contains all the DLL interfacing object declarations, in this example:
   a class object, two global function object, and a global integer variable. 

   Notice: we use the same header file for compiling the .dll and the .exe (application).
   This header file defines a macro which export the target DLL objects if we are building
   a DLL, otherwise it import the DLL objects into an application which uses the DLL. If
   we define DLLDIR_EX (a preprocessor identifier), then the preprocessor define macro
   DLLDIR (a mnemonic for DLL, import/export, Direction) becomes an export instruction,
   otherwise its an import instruction by default. 
***********************************************************************************/

#ifdef Si10DIR_EX
   #define Si10DIR  __declspec(dllexport)     // export DLL information
#else
   #define Si10DIR  __declspec(dllimport)     // import DLL information
#endif

// The extern "C" declaration allows mixed languages compactability, it prevents
// the C++ compiler from using decorated (modified) names for the functions 
extern "C" { 
       void Si10DIR Si10fun1(char*);
       int  Si10DIR Si10fun2(int);
};

extern int  Si10DIR Si10Arg;

class Si10DIR Si10class
{
   public:
      Si10class();          // Class Constructor
      ~Si10class();         // Class destructor
      int Add(int, int);   // Class function Add
      int Sub(int, int);   // Class function Subtract
      int Arg;             // Warning: you should not import class variables
                           // since the DLL object can be dynamically unloaded.
};

