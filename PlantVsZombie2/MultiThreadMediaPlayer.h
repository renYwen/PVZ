#pragma once

#include <Windows.h>
#pragma comment(lib,"winmm.lib")


#ifdef _DEBUG
#include <stdio.h>
#endif // _DEBUG

class __MultiThreadMediaPlayer
{
private:
	struct MCI_PARAM//�������ò���
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
			//�ȴ�����

			WaitForSingleObject(pThis->hEventBeg, INFINITE);//WaitForSingleObjectEx(pThis->hEvent, INFINITE, TRUE);//�ɾ���״̬�ȴ�

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
		WaitForSingleObject(hThread, INFINITE);//�ȴ��̹߳ر�
		CloseHandle(hThread);
		CloseHandle(hEventBeg);
		CloseHandle(hEventEnd);
		DeleteCriticalSection(&rtlCriticalSection);
	}

	MCIERROR SendStringA(LPCSTR lpstrCommand, LPSTR lpstrReturnString, UINT uReturnLength, HWND hwndCallback)
	{
		EnterCriticalSection(&rtlCriticalSection);

		//���ò���
		mciParam =
		{
			lpstrCommand,
			lpstrReturnString,
			uReturnLength,
			hwndCallback,
		};

		//��ʼ����
		SetEvent(hEventBeg);

		//�ȴ��������
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

		//���������
		MCIERROR ret = mciParam.mciErr;
		LeaveCriticalSection(&rtlCriticalSection);

		return ret;
	}
};

extern __MultiThreadMediaPlayer MCI;