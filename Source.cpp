#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

DWORD _Proceso(const char* proceso) {
	DWORD procID = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap == INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry)) {
			do {
				if (!_stricmp(procEntry.szExeFile, proceso)) {
					procID = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}


	CloseHandle(hSnap);
	return procID;

}
using namespace std;
int main() {
	DWORD procID = 0;
	char DLL_PATH[300], proc[300];

	cout << "Nombre del proceso ---> "; cin >> proc;
	cout << "Ubicacion del DLL ---> "; cin >> DLL_PATH;

	while (!procID) {
		procID = GetProcessId(proc);
		Sleep(60);
	}

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 0);
	if (hProc && hProc == INVALID_HANDLE_VALUE) {
		void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);


		WriteProcessMemory(hProc, loc, DLL_PATH, strlen(DLL_PATH) + 1, 0);

		HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);
		if (hThread) {
			CloseHandle(hThread);
		}
	}

	if (hProc)
		CloseHandle(hProc);

	system("pause");
	return 0;
}