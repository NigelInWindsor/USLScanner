
#pragma once

//Here are functions list used to display warning and error message from the Kernel
_COM_DECL_ void UTCom_SystemMessageBox(const wchar_t *pMsg);//blocking display with single "OK" button.
_COM_DECL_ void UTCom_SystemMessageBoxList(const wchar_t *pMsg);//display in the list, non blocking display, no need for the user to press button "OK".
_COM_DECL_ UINT UTCom_SystemMessageBox(const wchar_t *pMsg,const wchar_t *pTitle,UINT nType);//pop-up that required specific answer from the user (IDYES, IDNO ...).

namespace ComMessageBox
{
_COM_DECL_ int OempaApiMessageBox(HWND hWnd,LPCTSTR lpszText,LPCTSTR lpszCaption,UINT nType);//pop-up used by the customized API.

//Callback functions list to overwrite the behavior of those functions
typedef void (WINAPI* CallbackSystemMessageBox)(const wchar_t *pMsg);
typedef void (WINAPI* CallbackSystemMessageBoxList)(const wchar_t *pMsg);
typedef UINT (WINAPI* CallbackSystemMessageBoxButtons)(const wchar_t *pMsg,const wchar_t *pTitle,UINT nType);//should return IDOK IDYES IDNO...depending of the button pressed by the user.
typedef int (WINAPI* CallbackMessageBox)(HWND hWnd,LPCTSTR lpszText,LPCTSTR lpszCaption,UINT nType);

//You can overwrite your own functions to avoid the display of kernel popup window.
//Be careful in case you overwrite those functions, you dont have the right to display
//the message box directly in your callback, you need to post a message to a window.
_COM_DECL_ void SetCallbackSystemMessageBox(CallbackSystemMessageBox pCallback);
_COM_DECL_ void SetCallbackSystemMessageBoxList(CallbackSystemMessageBoxList pCallback);
_COM_DECL_ void SetCallbackSystemMessageBoxButtons(CallbackSystemMessageBoxButtons pCallback);
_COM_DECL_ void SetCallbackMessageBox(CallbackMessageBox pCallback);

_COM_DECL_ CallbackSystemMessageBox GetCallbackSystemMessageBox();
_COM_DECL_ CallbackSystemMessageBoxList GetCallbackSystemMessageBoxList();
_COM_DECL_ CallbackSystemMessageBoxButtons GetCallbackSystemMessageBoxButtons();
_COM_DECL_ CallbackMessageBox GetCallbackMessageBox();

}
