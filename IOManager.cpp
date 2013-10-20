#include "IOManager.h"

MapData* IOManager::GetMapData()
{
    mapData = new MapData();
    mapData->height = 5;
    mapData->width = 5;
    return mapData;
}

void IOManager::KeyboardInputRun()
{
    int* swapRequest = new int[4];
    while (true)
    {
        std::cout << ("////////////////////////////////////////");
        std::cout << ("// IOManager waiting for new input:");
        std::cout << ("// Imput format:");
        std::cout << ("// 0,0 0,1");
        std::cout << ("////////////////////////////////////////");
        std::string input;
        std::cin >> input;
        std::cout << ("Input read: " + input);
        ParseInput(input, swapRequest);
        if (swapRequest != NULL)
        {
            //gameManager->solveWizard->SwapCells(swapRequest);
        }
    }
}

void IOManager::ParseInput(std::string input, int *swapRequest)
{
    swapRequest[0] = input[0];
    swapRequest[1] = input[1];
    swapRequest[2] = input[2];
    swapRequest[3] = input[3];
    std::cout << "Succesfully parsed: " << swapRequest[0] << "," << swapRequest[1] << " " << swapRequest[2] << "," << swapRequest[3];
}
