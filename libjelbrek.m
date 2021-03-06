#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <mach/mach.h>
#include <mach-o/dyld.h>
#include "libjelbrek.h"
#include "jailbreak_daemonUser.h"
#include "commands.h"

static bool dosuid=false;

kern_return_t bootstrap_look_up(mach_port_t port, const char *service, mach_port_t *server_port);

struct __attribute__((__packed__)) jb_connection {
    mach_port_t jbd_port;
};

/* Open a connection to jailbreakd */
jb_connection_t jb_connect() {
    mach_port_t jbd_port;
    if (bootstrap_look_up(bootstrap_port, "slice.jailbreakd", &jbd_port) != 0) {
        return NULL;
    }
    
    struct jb_connection *conn = malloc(sizeof(struct jb_connection));
    conn->jbd_port = jbd_port;
    return (jb_connection_t)conn;
}

/* Close and free a jailbreakd connection */
void jb_disconnect(jb_connection_t connection) {
    struct jb_connection *conn = (struct jb_connection *)connection;
    mach_port_deallocate(mach_task_self(), conn->jbd_port);
    free(conn);
}

/* Entitle a process with the given flags (blocking, requires connection) */
int jb_entitle_now(jb_connection_t connection, pid_t pid, uint32_t flags) {
    struct jb_connection *conn = (struct jb_connection *)connection;
    kern_return_t ret = jbd_call(conn->jbd_port, JAILBREAKD_COMMAND_ENTITLE, pid);
    
    if (ret != KERN_SUCCESS) {
        return 1;
    }
    
    return 0;
}

int jb_unsandbox_now(jb_connection_t connection, pid_t pid) {
    struct jb_connection *conn = (struct jb_connection *)connection;
    kern_return_t ret = jbd_call(conn->jbd_port, JAILBREAKD_COMMAND_UNSANDBOX, pid);
    
    if (ret != KERN_SUCCESS) {
        return 1;
    }
    
    return 0;
}


/* Fix setuid on a process (blocking, requires connection) */
int jb_fix_setuid_now(jb_connection_t connection, pid_t pid) {
    if (dosuid) {
        struct jb_connection *conn = (struct jb_connection *)connection;
        kern_return_t ret = jbd_call(conn->jbd_port, JAILBREAKD_COMMAND_FIXUP_SETUID, pid);
        
        if (ret != KERN_SUCCESS) {
            return 1;
        }
    }
    
    return 0;
}

#if __BLOCKS__
/* Entitle a process with the given flags (asynchronous, requires connection) */
void jb_entitle(jb_connection_t connection, pid_t pid, uint32_t flags, jb_callback_t callback) {
    int ret = jb_entitle_now(connection, pid, flags);
    callback(ret);
}

/* Fix setuid on a process (asynchronous, requries connection) */
void jb_fix_setuid(jb_connection_t connection, pid_t pid, jb_callback_t callback) {
    if (dosuid) {
        int ret = jb_fix_setuid_now(connection, pid);
        callback(ret);
    } else {
        callback(0);
    }
}
#endif

/* Entitle a process with the given flags (blocking, no connection required) */
int jb_oneshot_entitle_now(pid_t pid, uint32_t flags) {
    jb_connection_t conn = jb_connect();
    if (conn == NULL) {
        return 1;
    }
    
    int ret = jb_entitle_now(conn, pid, flags);
    jb_disconnect(conn);
    
    return ret;
}

int jb_oneshot_unsandbox_now(pid_t pid) {
    jb_connection_t conn = jb_connect();
    if (conn == NULL) {
        return 1;
    }
    
    int ret = jb_unsandbox_now(conn, pid);
    jb_disconnect(conn);
    
    return ret;
}

/* Fix setuid on a process (blocking, no connection required) */
int jb_oneshot_fix_setuid_now(pid_t pid) {
    if (dosuid) {
        jb_connection_t conn = jb_connect();
        if (conn == NULL) {
            return 1;
        }
        
        int ret = jb_fix_setuid_now(conn, pid);
        jb_disconnect(conn);
        return ret;
    }
    return 0;
}

#if __BLOCKS__
/* Entitle a process with the given flags (asynchronous, no connection required) */
void jb_oneshot_entitle(pid_t pid, uint32_t flags, jb_callback_t callback) {
    int ret = jb_oneshot_entitle_now(pid, flags);
    callback(ret);
}

/* Entitle a process with the given flags (asynchronous, no connection required) */
void jb_oneshot_unsandbox(pid_t pid, jb_callback_t callback) {
    int ret = jb_oneshot_unsandbox_now(pid);
    callback(ret);
}

/* Fix setuid on a process (asynchronous, no connection required) */
void jb_oneshot_fix_setuid(pid_t pid, jb_callback_t callback) {
    if (dosuid) {
        int ret = jb_oneshot_fix_setuid_now(pid);
        callback(ret);
    }
    callback(0);
}
#endif


__attribute__((constructor))
void ctor() {
    if (NSVersionOfRunTimeLibrary("System") >= (1252<<16) ) {
        dosuid=true;
    }
}
