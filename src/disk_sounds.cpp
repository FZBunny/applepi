

#include <unistd.h>
#include <QtGlobal>

#include "disk_sounds.h"
#include "debugging_dumps.h"
#include "xpm_images.h"


DiskSounds::DiskSounds (void)
{


}


void DiskSounds::play (quint16 soundNumber)
{
    switch (soundNumber) {
        case DISK144K:
            break ;
        case DISK800K:
            break ;
        default:
            break ;
    }
}
