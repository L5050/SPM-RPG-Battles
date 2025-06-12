#include "ip.h"
#include "ip_badgemenu.h"
#include "ip_badgepouch.h"
#include "ip_messages.h"
#include "ip_pausetex.h"
#include "ip_save.h"

#include <patch.h>

#include <mod_ui_base/window.h>
#include <mod_ui_base/colours.h>

#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/seqdef.h>
#include <wii/os.h>

namespace ip {

/*
    General mod functions
*/

void main()
{
    wii::os::OSReport(IP_VERSION ": main running\n");

    badgeMenuPatch();
    pauseTexPatch();
}

}
