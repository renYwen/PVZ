#pragma once

#include <Windows.h>
#pragma comment(lib,"winmm.lib")


#ifdef _DEBUG
#include <stdio.h>
#endif // _DEBUG

class __MultiThreadMediaPlayer
{
private:
	struct MCI_PARAM//函数调用参数
	{
		LPCSTR lpstrCommand;
		LPSTR lpstrReturnString;
		UINT uReturnLength;
		HWND hwndCallback;

		MCIERROR mciErr;
	};

	DWORD bLoop;
	MCI_PARAM mciParam;
	HANDLE hEventBeg;
	HANDLE hEventEnd;
	RTL_CRITICAL_SECTION rtlCriticalSection;
	HANDLE hThread;

	static DWORD WINAPI MediaPlayerThread(LPVOID lpThreadParameter)
	{
		__MultiThreadMediaPlayer* pThis = (__MultiThreadMediaPlayer*)lpThreadParameter;

		while (pThis->bLoop)
		{
			//等待唤醒

			WaitForSingleObject(pThis->hEventBeg, INFINITE);//WaitForSingleObjectEx(pThis->hEvent, INFINITE, TRUE);//可警告状态等待

#ifdef _DEBUG
			printf("[MCI] Call:\"%s\" %p %d %p\n", pThis->mciParam.lpstrCommand, pThis->mciParam.lpstrReturnString, pThis->mciParam.uReturnLength, pThis->mciParam.hwndCallback);
#endif // _DEBUG

			pThis->mciParam.mciErr = ::mciSendStringA(pThis->mciParam.lpstrCommand, pThis->mciParam.lpstrReturnString, pThis->mciParam.uReturnLength, pThis->mciParam.hwndCallback);

#ifdef _DEBUG
			printf("[MCI] Return\n");
#endif // _DEBUG

			SetEvent(pThis->hEventEnd);
		}

		return 0;
	}

public:
	__MultiThreadMediaPlayer(void) :mciParam{}
	{
		bLoop = TRUE;
		hEventBeg = CreateEventA(NULL, FALSE, FALSE, NULL);
		hEventEnd = CreateEventA(NULL, FALSE, FALSE, NULL);
		InitializeCriticalSection(&rtlCriticalSection);
		hThread = CreateThread(0, 0, MediaPlayerThread, this, NULL, NULL);
	}

	~__MultiThreadMediaPlayer(void)
	{
		bLoop = FALSE;
		WaitForSingleObject(hThread, INFINITE);//等待线程关闭
		CloseHandle(hThread);
		CloseHandle(hEventBeg);
		CloseHandle(hEventEnd);
		DeleteCriticalSection(&rtlCriticalSection);
	}

	MCIERROR SendStringA(LPCSTR lpstrCommand, LPSTR lpstrReturnString, UINT uReturnLength, HWND hwndCallback)
	{
		EnterCriticalSection(&rtlCriticalSection);

		//设置参数
		mciParam =
		{
			lpstrCommand,
			lpstrReturnString,
			uReturnLength,
			hwndCallback,
		};

		//开始处理
		SetEvent(hEventBeg);

		//等待处理完毕
		WaitForSingleObject(hEventEnd, INFINITE);

#ifdef _DEBUG
		if (mciParam.mciErr != 0)
		{
			char szErrorBuf[MAXERRORLENGTH];
			if (mciGetErrorStringA(mciParam.mciErr, (LPSTR)szErrorBuf, MAXERRORLENGTH))
			{
				printf("[MCI] Error:%s\n", szErrorBuf);
			}
			else
			{
				printf("[MCI] Error:Unknown Error\n");
			}
		}
#endif // _DEBUG

		//保存错误码
		MCIERROR ret = mciParam.mciErr;
		LeaveCriticalSection(&rtlCriticalSection);

		return ret;
	}
};

extern __MultiThreadMediaPlayer MCI;