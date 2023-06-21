#include <spm/evtmgr.h>
#include <common.h>

namespace spm::iValues {

extern "C" {

extern const spm::evtmgr::EvtScriptCode theParentOfBeginRPG;
extern spm::evtmgr::EvtScriptCode * finalRpgChild;
extern spm::evtmgr::EvtScriptCode * underchompAttack1;
extern spm::evtmgr::EvtScriptCode * underchompAttack2;
extern spm::evtmgr::EvtScriptCode * underchompAttack3;
extern spm::evtmgr::EvtScriptCode * checkWinOrContinue;
extern spm::evtmgr::EvtScriptCode * attacking;
extern spm::evtmgr::EvtScriptCode * usePixls;
extern spm::evtmgr::EvtScriptCode * useItems;
extern spm::evtmgr::EvtScriptCode * runningAway;

extern spm::evtmgr::EvtScriptCode * techChild1;
extern spm::evtmgr::EvtScriptCode * techChild2;
void techtext1();
void techtext2();

}
}
