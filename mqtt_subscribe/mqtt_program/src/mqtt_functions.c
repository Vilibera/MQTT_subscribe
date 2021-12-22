#include <mosquitto.h>
#include <syslog.h>
#include "logscleanup.h"
#include <stdlib.h>
#include <ctype.h>
#include <json-c/json.h>
#include "StructAddress.h"
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
static int running = 1;


void mqtt_new(struct mosquitto **mosq)
{
	*mosq = mosquitto_new(NULL, true, NULL);
	if(*mosq == NULL){
		syslog(LOG_ERR, "Error: Out of memory.\n");
		CleanAll(1);
	}
	syslog(LOG_INFO, "Created new mosquitto successfully");
}

void mqtt_connect(struct mosquitto **mosq, char *address, char *port)
{
	int rc = MOSQ_ERR_SUCCESS;
	long portNum;
	char *portPtr;

	if(address == NULL && port == NULL){
		syslog(LOG_ERR, "Must specify port and address...");
		mosquitto_lib_cleanup();
		CleanAll(1);
	}

	portNum = strtol(port, &portPtr, 10);
	if((portPtr == port)){
		syslog(LOG_ERR, "Port must be an integer...");
		mosquitto_lib_cleanup();
		CleanAll(1);
	}
	rc = mosquitto_connect(*mosq, address, portNum, 60);
	if(rc != MOSQ_ERR_SUCCESS){
		mosquitto_destroy(*mosq);
		syslog(LOG_ERR, "Error connecting: %s\n", mosquitto_strerror(rc));
		mosquitto_lib_cleanup();
		CleanAll(1);
	}
	
	syslog(LOG_INFO, "Connected to a broker successfully.");
}

void mqtt_subscribe(struct mosquitto **mosq, char *topicName, int qos)
{	
	int ch;
	int rc = MOSQ_ERR_SUCCESS;
	rc = mosquitto_subscribe(*mosq, NULL,topicName,qos);
	if(rc != MOSQ_ERR_SUCCESS) {
		syslog(LOG_ERR, "Error subscribing: %s\n", mosquitto_strerror(rc));
		mosquitto_disconnect(*mosq);
		CleanAll(rc);
	}
	syslog(LOG_INFO, "subscribed to a topic: %s, QoS: %d", topicName,qos);
	
		
	
	
}
void mqtt_disconnect(struct mosquitto **mosq)
{
	int rc;
	rc = mosquitto_disconnect(*mosq);
	if(rc != MOSQ_ERR_SUCCESS){
		syslog(LOG_ERR, "Error disconnecting: %s\n", mosquitto_strerror(rc));
	}


}