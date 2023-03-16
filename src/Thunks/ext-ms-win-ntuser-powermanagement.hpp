#if (YY_Thunks_Support_Version < NTDDI_WIN8)
#include <winuser.h>
#endif

namespace YY
{
	namespace Thunks
	{
#ifndef CONST_DEVICE_NOTIFY_WINDOW_HANDLE
#define CONST_DEVICE_NOTIFY_WINDOW_HANDLE (HPOWERNOTIFY)0x1001
#endif

#if (YY_Thunks_Support_Version < NTDDI_WIN8)

		// Windows 8 [��������Ӧ��]��Windows Server 2012 [��������Ӧ��]
		__DEFINE_THUNK(
		user32,
		8,
		HPOWERNOTIFY,
		WINAPI,
		RegisterSuspendResumeNotification,
			IN HANDLE _hRecipient,
			IN DWORD _fFlags
			)
		{
			if (const auto _pfnRegisterSuspendResumeNotification = try_get_RegisterSuspendResumeNotification())
			{
				return _pfnRegisterSuspendResumeNotification(_hRecipient, _fFlags);
			}

			if (DEVICE_NOTIFY_CALLBACK == _fFlags)
			{
				HPOWERNOTIFY _hRegistrationHandle = nullptr;

				auto _lStatus = PowerRegisterSuspendResumeNotificationDownlevel(DEVICE_NOTIFY_CALLBACK, _hRecipient, &_hRegistrationHandle);
				if (_lStatus == ERROR_SUCCESS)
				{
					return _hRegistrationHandle;
				}
				else
				{
					SetLastError(_lStatus);
					return nullptr;
				}
			}
			else if (DEVICE_NOTIFY_WINDOW_HANDLE == _fFlags)
			{
				// ���������Ҫ�򴰿ڷ��͵�Դ֪ͨ�������ϰ汾ϵͳ��֧�� �ִ���˯��
				// ������ǿ�����ȷ������������ġ��������Ǻ��Բ�����ֱ�ӷ���һ����ǳ������ɡ�
				UNREFERENCED_PARAMETER(_hRecipient);

				return CONST_DEVICE_NOTIFY_WINDOW_HANDLE;
			}
			else
			{
				// ��֧�����������
				SetLastError(ERROR_INVALID_PARAMETER);
				return nullptr;
			}
		}
#endif

#if (YY_Thunks_Support_Version < NTDDI_WIN8)

		// Windows 8 [��������Ӧ��]��Windows Server 2012 [��������Ӧ��]
		__DEFINE_THUNK(
		user32,
		4,
		BOOL,
		WINAPI,
		UnregisterSuspendResumeNotification,
			IN HPOWERNOTIFY _hHandle
			)
		{
			if (const auto _pfnUnregisterSuspendResumeNotification = try_get_UnregisterSuspendResumeNotification())
			{
				return _pfnUnregisterSuspendResumeNotification(_hHandle);
			}

			if (_hHandle == nullptr)
			{
				SetLastError(ERROR_INVALID_PARAMETER);
				return FALSE;
			}
			else if (_hHandle == CONST_DEVICE_NOTIFY_WINDOW_HANDLE)
			{
				// ���ԣ����� DEVICE_NOTIFY_WINDOW_HANDLE �ĳ���
				return TRUE;
			}
			else
			{
				// ���� DEVICE_NOTIFY_CALLBACK��ע��
				auto _lStatus = PowerUnregisterSuspendResumeNotificationDownlevel(_hHandle);
				if (_lStatus == ERROR_SUCCESS)
				{
					return TRUE;
				}
				else
				{
					SetLastError(_lStatus);
					return FALSE;
				}
			}
		}
#endif
	}
}
