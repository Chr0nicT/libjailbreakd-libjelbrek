//
//  callJBD.h
//  libjelbrek
//
//  Created by Tanay Findley on 5/21/19.
//  Copyright © 2019 Slice Team. All rights reserved.
//

#ifndef callJBD_h
#define callJBD_h

#define JAILBREAKD_COMMAND_ENTITLE 1
#define JAILBREAKD_COMMAND_ENTITLE_AND_SIGCONT 2
#define JAILBREAKD_COMMAND_ENTITLE_AND_SIGCONT_AFTER_DELAY 4
#define JAILBREAKD_COMMAND_ENTITLE_AND_SIGCONT_FROM_XPCPROXY 5
#define JAILBREAKD_COMMAND_FIXUP_SETUID 6
#define JAILBREAKD_COMMAND_UNSANDBOX 7
#define JAILBREAKD_COMMAND_FIXUP_DYLIB 8
#define JAILBREAKD_COMMAND_FIXUP_EXECUTABLE 9

#include <stdio.h>
#include <mach/kern_return.h>
#include <sys/types.h>

kern_return_t calljailbreakd(int command, pid_t PID, char *file);

#endif /* callJBD_h */
