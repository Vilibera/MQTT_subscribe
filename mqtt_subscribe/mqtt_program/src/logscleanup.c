#include <syslog.h>

void closelogs(int rc) 
{
    closelog();
    exit(rc);
}

void CleanAll(int rc)
{
    syslog(LOG_INFO,"mqtt_program is cleaning up");
    closelogs(rc);
}
