//
//  libjelbrek.m
//  libjelbrek
//
//  Created by Tanay Findley on 5/21/19.
//  Copyright © 2019 Slice Team. All rights reserved.
//

#import "libjelbrek.h"
#include "callJBD.h"
#include <sys/types.h>

/* Entitle a process with the given flags (blocking, requires connection) */
int jb_entitle_now(pid_t pid, uint32_t flags) {
    kern_return_t ret = calljailbreakd(JAILBREAKD_COMMAND_ENTITLE, pid, NULL);
    
    if (ret != KERN_SUCCESS) {
        return 1;
    }
    
    return 0;
}


/* Fix setuid on a process (blocking, requires connection) */
int jb_fix_setuid_now(pid_t pid) {
    kern_return_t ret = calljailbreakd(JAILBREAKD_COMMAND_FIXUP_SETUID, pid, NULL);
        
    if (ret != KERN_SUCCESS) {
        return 1;
    }
    
    return 0;
}


/* Entitle a process with the given flags (blocking, no connection required) */
int jb_oneshot_entitle_now(pid_t pid, uint32_t flags) {
    jb_entitle_now(pid, flags);
    return 0;
}

/* Fix setuid on a process (blocking, no connection required) */
int jb_oneshot_fix_setuid_now(pid_t pid) {
    jb_fix_setuid_now(pid);
    return 0;
}

int jb_unsandbox_now(pid_t pid) {
    kern_return_t ret = calljailbreakd(JAILBREAKD_COMMAND_UNSANDBOX, pid, NULL);
    
    if (ret != KERN_SUCCESS)
    {
        return -1;
    }
    
    return 0;
}

int jb_fixup_exec_now(char *exec) {
    kern_return_t ret = calljailbreakd(JAILBREAKD_COMMAND_FIXUP_EXECUTABLE, 0, exec);
    
    if (ret != KERN_SUCCESS)
    {
        return -1;
    }
    
    return 0;
}

int jb_fixup_dylib_now(char *dylib) {
    kern_return_t ret = calljailbreakd(JAILBREAKD_COMMAND_FIXUP_DYLIB, 0, dylib);
    
    if (ret != KERN_SUCCESS)
    {
        return -1;
    }
    
    return 0;
}
