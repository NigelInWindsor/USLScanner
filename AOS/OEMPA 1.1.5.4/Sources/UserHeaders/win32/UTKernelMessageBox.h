
#pragma once

//Here are functions list used to display warning and error message from the Kernel
UTKERNEL_DRIVER_API void UTKernel_SystemMessageBox(const wchar_t *pMsg);//blocking display with single "OK" button.
UTKERNEL_DRIVER_API void UTKernel_SystemMessageBoxList(const wchar_t *pMsg);//display in the list, non blocking display, no need for the user to press button "OK".
UTKERNEL_DRIVER_API UINT UTKernel_SystemMessageBox(const wchar_t *pMsg,const wchar_t *pTitle,UINT nType);//pop-up that required specific answer from the user (IDYES, IDNO ...).
UTKERNEL_DRIVER_API int OempaApiMessageBox(HWND hWnd,LPCTSTR lpszText,LPCTSTR lpszCaption,UINT nType);//pop-up used by the customized API.

//Callback functions list to overwrite the behavior of those functions
typedef void (WINAPI* CallbackSystemMessageBox)(const wchar_t *pMsg);
typedef void (WINAPI* CallbackSystemMessageBoxList)(const wchar_t *pMsg);
typedef UINT (WINAPI* CallbackSystemMessageBoxButtons)(const wchar_t *pMsg,const wchar_t *pTitle,UINT nType);//should return IDOK IDYES IDNO...depending of the button pressed by the user.
typedef int (WINAPI* CallbackMessageBox)(HWND hWnd,LPCTSTR lpszText,LPCTSTR lpszCaption,UINT nType);

//You can overwrite your own functions to avoid the display of kernel popup window.
//Be careful in case you overwrite those functions, you dont have the right to display
//the message box directly in your callback, you need to post a message to a window.
UTKERNEL_DRIVER_API void SetCallbackSystemMessageBox(CallbackSystemMessageBox pCallback);
UTKERNEL_DRIVER_API void SetCallbackSystemMessageBoxList(CallbackSystemMessageBoxList pCallback);
UTKERNEL_DRIVER_API void SetCallbackSystemMessageBoxButtons(CallbackSystemMessageBoxButtons pCallback);
UTKERNEL_DRIVER_API void SetCallbackMessageBox(CallbackMessageBox pCallback);

UTKERNEL_DRIVER_API CallbackSystemMessageBox GetCallbackSystemMessageBox();
UTKERNEL_DRIVER_API CallbackSystemMessageBoxList GetCallbackSystemMessageBoxList();
UTKERNEL_DRIVER_API CallbackSystemMessageBoxButtons GetCallbackSystemMessageBoxButtons();
UTKERNEL_DRIVER_API CallbackMessageBox GetCallbackMessageBox();

