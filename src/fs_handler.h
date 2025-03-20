#ifndef FS_HANDLER_H
#define FS_HANDLER_H

#include <SPIFFS.h> // Include SPIFFS library

namespace FSHandler
{

    bool setupFS();
    void listFSContents();

} // namespace FSHandler

#endif // FS_HANDLER_H