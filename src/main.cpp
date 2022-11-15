/***
 *       _____ _                 _         _____  _      _         _____       _           _             
 *      / ____(_)               | |       |  __ \| |    | |       |_   _|     (_)         | |            
 *     | (___  _ _ __ ___  _ __ | | ___   | |  | | |    | |         | |  _ __  _  ___  ___| |_ ___  _ __ 
 *      \___ \| | '_ ` _ \| '_ \| |/ _ \  | |  | | |    | |         | | | '_ \| |/ _ \/ __| __/ _ \| '__|
 *      ____) | | | | | | | |_) | |  __/  | |__| | |____| |____    _| |_| | | | |  __/ (__| || (_) | |   
 *     |_____/|_|_| |_| |_| .__/|_|\___|  |_____/|______|______|  |_____|_| |_| |\___|\___|\__\___/|_|   
 *                        | |                                                _/ |                        
 *                        |_|                                               |__/                         
 */

DWORD get_proc_id(const char* name) // Credit: ZeroMemoryEx 
{
    DWORD procId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnap != INVALID_HANDLE_VALUE) 
	{
        PROCESSENTRY32 pE;
        pE.dwSize = sizeof(pE);

        if (Process32First(hSnap, &pE)) 
		{
            if (!pE.th32ProcessID)
                Process32Next(hSnap, &pE);
            do
            {
                if (!_stricmp(pE.szExeFile, name))
                {
                    procId = pE.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &pE));
        }
    }

    CloseHandle(hSnap);
    return procId;
}

std::filesystem::path grab_current_dir() {
    return std::filesystem::current_path();
}

bool inject_dll(DWORD procID, const char* dllPath)
{
    SIZE_T dllSize = strlen(dllPath) + 1; // Store the dll size.

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procID); // Open process and return handle.
    if (hProc == INVALID_HANDLE_VALUE) // Check if handle exists.
        return false; // If not return false as we can't continue.

    void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (loc)
    {
        bool status = WriteProcessMemory(hProc, loc, dllPath, dllSize, 0);
        if (!status) {
            VirtualFree(loc, dllSize, MEM_RELEASE);
            CloseHandle(hProc);
            return false;
        }

        HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);
        if (!hThread) {
            VirtualFree(loc, dllSize, MEM_RELEASE);
            CloseHandle(hProc);
            return false;
        }

        std::cout << "Injected dll." << std::endl;

        // Clean up.
        VirtualFree(loc, dllSize, MEM_RELEASE);
        CloseHandle(hProc);
        CloseHandle(hThread);

        return true;
    }
    else {
        CloseHandle(hProc);
        return false;
    }

    return true;
}

/*		Args

	Arg 0 = Exe path.
	Arg 1 = Process Name.
	Arg 2 = Dll Name.
	Arg 3 = Keep console open after inject.
*/
int main(int count, char* args[])
{
	// Print args
	// std::cout << "Arg count: " << count << std::endl;

	// for (int i = 0; i < count; i++)
	// 	std::cout << "Arg " << i << ": " << args[i] << std::endl;
	
	SetConsoleTitleA("Simple DLL Injector");

	std::string processName = args[1];
	std::string dllName = args[2];
    std::string keepOpenStr = args[3];
    bool keepOpen = false;
        
    if(keepOpenStr == "true" || keepOpenStr == "1" || keepOpenStr == "on")
        keepOpen = true;
    else if(keepOpenStr == "false" || keepOpenStr == "0" || keepOpenStr == "off")
        keepOpen = false;

	std::filesystem::path curPath = grab_current_dir() / dllName; // Set the dll path to use.

	std::cout << "Welcome to Simple DLL Injector" << std::endl;
    std::cout << "Credits: Fuscina" << std::endl;
	//std::cout << "Process Name: " << processName << std::endl;
	//std::cout << "DLL Name: " << dllName << std::endl;

	if (!std::filesystem::exists(curPath)) // Check if the dll file exists, If not close the injector.
	{
		std::cout << "DLL does not exist. Closing injector." << std::endl;
		Sleep(2000);
		return EXIT_FAILURE;
	}

    std::cout << "Looking for process: " << processName << std::endl;

    DWORD procId = 0;
    do {
        procId = get_proc_id(processName.c_str()); // Try and grab the proccess id.
        Sleep(1000); // Wait 1s then retry.
    } while (procId == 0);

	std::cout << "Found process." << std::endl;

    if (!inject_dll(procId, curPath.string().c_str()))
        std::cout << "Failed to inject dll!" << std::endl;

    Sleep(1000);

    if(keepOpen)
	    system("pause");

	return EXIT_SUCCESS;
}