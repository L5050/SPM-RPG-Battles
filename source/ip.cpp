#include "ip.h"
#include "ip_badgemenu.h"
#include "ip_badgepouch.h"
#include "ip_pausetex.h"

#include <patch.h>

#include <mod_ui_base/window.h>

#include <spm/filemgr.h>
#include <spm/pausewin.h>
#include <wii/os.h>

namespace ip {

/*
    General mod functions
*/

void ipInit()
{
    badgeMenuPatch();
    pauseTexPatch();
    spm::pausewin::pausewinUpdate();
}

void main()
{
    wii::os::OSReport(IP_VERSION ": main running\n");
    badgeIconTpl = nullptr;
    badgeMenuTpl = nullptr;
    writeBranchLink(spm::pausewin::pausewinMain, 0x1C, ipInit);
}

}
