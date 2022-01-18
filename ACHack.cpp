// ACHack.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"

int main()
{

    // Get Proc ID of Target
    DWORD procID = GetProcId(L"ac_client.exe");


    // Get Mod Base Address
    uintptr_t moduleBase = GetModuleBaseAddress(procID, L"ac_client.exe");

    // Get handel to Process
    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procID);

    // Resolve base address
    uintptr_t dynamicPtrBaseAddr = moduleBase + 0x10f4f4;

    std::cout << "DynamicPtrBaseAddress =" << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;
    // Resole ammo pointer chain

    std::vector<unsigned int> ammoOffsets = { 0x374, 0x14, 0x0 };

    uintptr_t ammoAddress = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);

    std::cout << "Ammo Address =" << "0x" << std::hex << ammoAddress << std::endl;

    // Read out ammmo 

    int ammoValue = 0;
    ReadProcessMemory(hProcess, (BYTE*)ammoAddress, &ammoValue, sizeof(ammoValue), nullptr);

    std::cout << "Current Ammo = " << std::dec << ammoValue << std::endl;

    // Write to it

    int newAmmo = 2000;
    WriteProcessMemory(hProcess, (BYTE*)ammoAddress, &newAmmo, sizeof(newAmmo), nullptr);

   
    ReadProcessMemory(hProcess, (BYTE*)ammoAddress, &ammoValue, sizeof(ammoValue), nullptr);
  
    // Health Offsets
    std::vector<unsigned int> healthOffsets = { 0xf8 };
    uintptr_t healthAddress = FindDMAAddy(hProcess, dynamicPtrBaseAddr, healthOffsets);

    // Read Health
    int healthErrorCheck = 0;
    ReadProcessMemory(hProcess, (BYTE*)healthAddress, &healthErrorCheck, sizeof(healthErrorCheck), nullptr);
    std::cout << "Current User Health: " << std::dec << healthErrorCheck << std::endl;

    // Write to Health
    int newHealth = 10000;
    WriteProcessMemory(hProcess, (BYTE*)healthAddress, &newHealth, sizeof(newHealth), nullptr);



    std::cout << "New Ammo = " << std::dec << ammoValue << std::endl;
    std::cout << "New Health = " << std::dec << healthErrorCheck << std::endl;
 



    getchar();
}

