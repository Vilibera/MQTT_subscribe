#include <uci.h>
#include <syslog.h>
#include "StructCurlMessage.h"
#include "logscleanup.h"


void uci_load_packages(struct uci_context *ctx, const char *config_name, struct uci_package **package)
{
	if(uci_load(ctx, config_name, package) !=UCI_OK){
		syslog(LOG_ERR, "Failed to load uci");
		uci_free_context(ctx);
		CleanAll(1);
	}
}

void uci_alloc(struct uci_context *ctx, char* configName, struct uci_package **package)
{
	ctx = uci_alloc_context();
	if(ctx == NULL){
		syslog(LOG_ERR, "Failed to allocate memory to uci context");
		CleanAll(1);
	}
	uci_load_packages(ctx, CONFIG, package);
}

