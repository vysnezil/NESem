#include "Dialog.h"
#include <stdlib.h>
#include "Logger.h"

char* Dialog::getFile()
{
    NFD_Init();

    nfdchar_t* outPath;
    nfdfilteritem_t filterItem[1] = {"NES rom files", "nes"};
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);
    if (result == NFD_OKAY)
    {
        return outPath;
        NFD_FreePath(outPath);
    }
    else if (result == NFD_CANCEL)
    {
        exit(0);
    }
    else
    {
        Logger::log("File picker: Error: ", NFD_GetError());
    }

    NFD_Quit();
    return nullptr;
}
