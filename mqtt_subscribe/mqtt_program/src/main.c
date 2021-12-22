#include <stdio.h>
#include <mosquitto.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <uci.h>
#include <argp.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include "mqtt_functions.h"
#include "curl_message.h"
#include "StructCurlMessage.h"
#include "uci_check.h"
#include "uci_alloc.h"
#include "StructAddress.h"
#include "ComparisonMessage.h"

struct uci_context *ctx = NULL;
struct uci_package *package = NULL;
struct sender sender = {NULL, NULL, NULL, NULL,NULL};

#define OPT_ABORT  1     /* –abort */
static struct argp_option options[] = {
  {"ipaddress",    'h',"IpAddress", 0,  "Ip Address" },
  {"port",         'p',"Port",      0,  "Port" },
  {"abort",         OPT_ABORT,      0, 0, "Abort before showing any output"},
  { 0 }
};


static error_t parse_opt (int key, char *arg, struct argp_state *state);

const char *argp_program_version ="Mosquitto subscribe";
const char *argp_program_bug_address ="<viliusbernikas@teltonika.lt>";

static char doc[] ="Mosquitto subscribe";

static char args_doc[] = "ARG1 [STRING...]";

static struct argp argp = { options, parse_opt, args_doc, doc }; 

static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
    struct Connection *con = state->input;
    switch (key){
        case 'h': 
        {   
            con->address = arg;
            break;
          
        }
        case 'p':
        {
             con->port = arg;
             break;
            
        }
		case ARGP_KEY_END:
		if ((!con->address) || (!con->port)){
			argp_usage (state);
		}
		break;
		default:
			return ARGP_ERR_UNKNOWN;
 }
 return 0;
} 


void sigHandler(int signo) 
{
	// struct mosquitto *mosq;
	// mqtt_disconnect(mosq);
	syslog(LOG_INFO,"Program is closing...");
	// syslog(LOG_ERR, "Error info: %s", curl_easy_strerror(signo));
	closelogs(signo);
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
	int rc;
	struct topic topic = { NULL, NULL, NULL, NULL, NULL, NULL };
	struct json_object *parsed_json = NULL;
	struct json_object *object_json = NULL;
	char *messageValue = NULL;
	char message[1023];
	rc = uci_element_parseTopic(package, msg, &topic);
	if(rc != 0){
		syslog(LOG_ERR, "Failed to parse information about topic");
		syslog(LOG_INFO, "Message received: topic: %s message: %s", msg->topic, (char*)msg->payload);
		return;
	}

	syslog(LOG_INFO, "Topic parsed succesfully");

	parsed_json = json_tokener_parse((char*)msg->payload);
	if (parsed_json == NULL){
		syslog(LOG_ERR, "Failed to parse JSON message");
		syslog(LOG_INFO, "Message received: topic: %s message: %s", msg->topic, (char*)msg->payload);
		return;
	}

	syslog(LOG_INFO, "JSON parsed succesfully");

	json_object_object_get_ex(parsed_json, topic.key, &object_json);	
	if (object_json ==NULL){
		syslog(LOG_ERR,"Message value doesn't have specified key, object field name");
		syslog(LOG_INFO, "Message received: topic: %s, key: %s, message: %s", msg->topic,topic.key, (char*)msg->payload);
		return;
	}

	syslog(LOG_INFO, "JSON get succesfully");

	messageValue = json_object_get_string(object_json);
	if(messageValue == NULL){
			syslog(LOG_ERR,"Message is empty");
			return;
	}
	uci_Check_Type(&topic, &sender, messageValue);
}

void mqtt_init(struct mosquitto *mosq, struct Connection con, struct uci_package *package)
{
    
	int rc;

	rc = uci_element_parse_Checks(package, &sender);
	if(rc != 0){
		syslog(LOG_ERR, "Sender parsed failed");
		CleanAll(1);
	}
	syslog(LOG_INFO, "Sender parsed succesfully");
    mosquitto_lib_init();
	mqtt_new(&mosq);
    mosquitto_message_callback_set(mosq, on_message);

	mqtt_connect(&mosq, con.address,con.port);

	uci_element_parseSubscriber(package, &mosq);
	
	mosquitto_loop_forever(mosq, -1, 1);
}

//Main method
int main(int argc,char *argv[])
{
    struct mosquitto *mosq;
	struct Connection con;

    con.address=NULL;
    con.port=NULL;
	openlog(NULL, LOG_CONS, LOG_USER);
    argp_parse(&argp,argc,argv,0,0,&con);

	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);

	uci_alloc(ctx, CONFIG, &package);
	mqtt_init(mosq, con, package);

	uci_free_context(ctx);
	mosquitto_lib_cleanup();

    CleanAll(0);
    return 0;
}


