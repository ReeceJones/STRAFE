#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>

DWORD GetPID(const char* processName)
{
	HANDLE hPid = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL); //snapshot of programs
	PROCESSENTRY32 processEntry32; 
	processEntry32.dwSize = sizeof(PROCESSENTRY32);
	while (Process32Next(hPid, &processEntry32))
	{
		if (strstr(processEntry32.szExeFile, processName))
		{
			CloseHandle(hPid);
			return processEntry32.th32ProcessID;
		}
	}
	CloseHandle(hPid);
	return -1;
}

BOOL AllocateAndWrite(HANDLE hProcess, std::string DllPath, void** memory)
{
	SIZE_T szMemWritten;
	*memory = VirtualAllocEx(hProcess, 0, DllPath.length(), MEM_COMMIT, PAGE_EXECUTE_READWRITE); //allocate memory

	if (!(*memory))
		return FALSE;
	else
		return WriteProcessMemory(hProcess, *memory, DllPath.c_str(), DllPath.length(), &szMemWritten); //write to memory
}

BOOL InjectLibrary(std::string DllPath, std::string processName, int iPid, HANDLE hProcess)
{
	if (iPid != -1 && hProcess)
	{
		void* location;
		if (AllocateAndWrite(hProcess, DllPath, &location))
		{
			FARPROC LoadLib = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
			if (location && CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLib, location, NULL, NULL))
			{
				CloseHandle(hProcess);
				return TRUE;
			}
		}
	}
	CloseHandle(hProcess);
	return FALSE;
}

int Failure()
{
	std::cout << "INJECTION FAILED" << std::endl;
	Beep(3000, 200);
	return EXIT_FAILURE;
}

int main()
{
	using namespace std; //just to make it easier
	string process; //our process name
	string dll; //our dll name
	cout << "Process Name (include .exe): ";
	getline(cin, process); //get process name
	int iPid = GetPID(process.c_str());
	if (iPid == -1)
		return Failure(); //process is invalid do not continue
	cout <<  "Process ID: " << iPid << endl << "DLL Name (include .dll): ";
	getline(cin, dll); //get dll name
	HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, false, iPid); //open a handle to our process
	char cDll[MAX_PATH]; //our full dll path
	GetFullPathName(dll.c_str(), MAX_PATH, cDll, NULL);
	cout << "Dll Path: " << cDll << endl;

	if (InjectLibrary(cDll, process, iPid, hProcess)) //inject our dll
	  return EXIT_SUCCESS;

	return Failure();
}

