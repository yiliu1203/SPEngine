#include "DXTrace.h"
#include <cstdio>

HRESULT WINAPI DXTrace(_In_z_ const CHAR* strFile, _In_ DWORD dwLine, _In_ HRESULT hr, _In_opt_ const CHAR* strMsg,
                       _In_ bool bPopMsgBox)
{
    CHAR strBufferFile[MAX_PATH];
    CHAR strBufferLine[128];
    CHAR strBufferError[300];
    CHAR strBufferMsg[1024];
    CHAR strBufferHR[40];
    CHAR strBuffer[3000];

    sprintf_s(strBufferLine, 128, "%u", dwLine);
    if (strFile) {
        sprintf_s(strBuffer, 3000, "%s(%s): ", strFile, strBufferLine);
        OutputDebugString(strBuffer);
    }

    size_t nMsgLen = (strMsg) ? strnlen_s(strMsg, 1024) : 0;
    if (nMsgLen > 0) {
        OutputDebugString(strMsg);
        OutputDebugString(" ");
    }
    // Windows SDK 8.0起DirectX的错误信息已经集成进错误码中，可以通过FormatMessage获取错误信息字符串
    // 不需要分配字符串内存
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  nullptr,
                  hr,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  strBufferError,
                  256,
                  nullptr);

    CHAR* errorStr = strrchr(strBufferError, '\r');
    if (errorStr) {
        errorStr[0] = '\0';   // 擦除FormatMessage带来的换行符(把\r\n的\r置换为\0即可)
    }

    sprintf_s(strBufferHR, 40, " (0x%0.8x)", hr);
    strcat_s(strBufferError, strBufferHR);
    sprintf_s(strBuffer, 3000, "Error Code:%s", strBufferError);
    OutputDebugString(strBuffer);

    OutputDebugString("\n");

    if (bPopMsgBox) {
        strcpy_s(strBufferFile, MAX_PATH, "");
        if (strFile) strcpy_s(strBufferFile, MAX_PATH, strFile);

        strcpy_s(strBufferMsg, 1024, "");
        if (nMsgLen > 0) sprintf_s(strBufferMsg, 1024, "current call:%s\n", strMsg);

        sprintf_s(strBuffer,
                  3000,
                  "filename:%s\n lineno:%s\nerror code:%s\n%s you are debug current app?",
                  strBufferFile,
                  strBufferLine,
                  strBufferError,
                  strBufferMsg);

        int nResult = MessageBox(GetForegroundWindow(), strBuffer, "Error", MB_YESNO | MB_ICONERROR);
        if (nResult == IDYES) DebugBreak();
    }

    return hr;
}