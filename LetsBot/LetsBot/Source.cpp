#include <Windows.h>
#include <string>
#include <TlHelp32.h>

 DWORD getBaseAddress(DWORD pid, TCHAR* n);

int main(int arg, char* argv[])
{
	HWND hWnd = 0;
	DWORD pid = 0;

	hWnd = FindWindow(0, L"Tibia");
	if (hWnd)
	{
		GetWindowThreadProcessId(hWnd, &pid);
		if (pid)
		{
			HANDLE gHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
			if (gHandle)
			{
				//convert string to TCHAR*
				std::string str = "Tibia.exe";
				TCHAR* processName = new TCHAR[str.size() + 1];
				processName[str.size()] = 0;
				std::copy(str.begin(), str.end(), processName);

				DWORD baseAddress = getBaseAddress(pid, processName);
				CloseHandle(gHandle);
			}
		}
	}
	return 0;
}

DWORD getBaseAddres(DWORD pid, TCHAR* n)
{
	HANDLE module_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (module_snapshot)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(MODULEENTRY32);
			if (Module32First(module_snapshot, &modEntry))
			{
				do {
					if (!strcmp(modEntry.szModule, n))
					{
						CloseHandle(module_snapshot);
						return (DWORD)modEntry.modBaseAddr;
					}
				} while (Module32Next(module_snapshot, &modEntry));
			}
	}
}