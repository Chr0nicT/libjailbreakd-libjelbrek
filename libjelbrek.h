//
//  libjelbrek.h
//  libjelbrek
//
//  Created by Tanay Findley on 5/21/19.
//  Copyright © 2019 Slice Team. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <sys/types.h>

extern int jb_entitle_now(pid_t pid, uint32_t flags);
extern int jb_fix_setuid_now(pid_t pid);
extern int jb_oneshot_entitle_now(pid_t pid, uint32_t flags);
extern int jb_oneshot_fix_setuid_now(pid_t pid);
extern int jb_unsandbox_now(pid_t pid);
extern int jb_fixup_exec_now(char *exec);
extern int jb_fixup_dylib_now(char *dylib);
