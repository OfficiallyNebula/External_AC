#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"


int main()
{

    //Get ProcessID of target
    DWORD procId = GetProcId(L"ac_client.exe");

    //Get Module Base Address
    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

    // Get Handle to Process
    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);


    //Resolve base address of the pointer chain
    uintptr_t dynamicPtrBaseAddr = moduleBase + 0x10f4f4;

    std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

    //Resolve our ammo pointer chain
    std::vector<unsigned int> ammoOffsets = { 0x374, 0x14, 0x0 };
    uintptr_t ammoAddr = findDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);

    std::cout << "AmmoAddr = " << "0x" << std::hex << ammoAddr << std::endl;

    //Read ammo value
    int ammoValue = 0;

    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "Current ammo = " << std::dec << ammoValue << std::endl;


    // Write ammo value
    int newAmmo = 1337;
    WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

    //Read Ammo value again
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "New ammo = " << std::dec << ammoValue << std::endl;


    getchar();
    return 0;


}
