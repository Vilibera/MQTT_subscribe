#include <uci.h>
#include <mosquitto.h>
#include <ctype.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <syslog.h>
#include "mqtt_functions.h"
#include "StructCurlMessage.h"

void uci_element_Check(struct uci_option *option, char* type, char** value)
{
	if(strcmp(option->e.name, type) == 0)
		*value=option->v.string;
}

bool checkParameter(char* topicParameter, char *message)
{
	bool checkTopic = true;
	if(topicParameter == NULL){
		syslog(LOG_ERR, "%s", message);
		checkTopic = false;
	}
	return checkTopic;
}

int uci_element_parse_Checks(struct uci_package *package, struct sender *sen)
{
	struct uci_element *i, *j;
	uci_foreach_element(&package->sections, i){
		struct uci_section *section = uci_to_section(i);
		char *section_type = section->type;
		if(strcmp(section_type,"m_conf") == 0){
			uci_foreach_element(&section->options, j){
				struct uci_option *option = uci_to_option(j);
				uci_element_Check(option,"senderEmail", &sen->email);
				uci_element_Check(option,"userName", &sen->username);
				uci_element_Check(option,"password", &sen->password);
				uci_element_Check(option,"smtpIP", &sen->smtpIP);
				uci_element_Check(option,"smtpPort", &sen->smtpPort);

			}
			if((checkParameter(sen->email,"Sender email not specified..."))    && 
			(checkParameter(sen->username,"Sender username not specified...")) && 
			(checkParameter(sen->password,"Sender password not specified...")) && 
			(checkParameter(sen->smtpIP,"Sender smtpIP not specified...")))
				return 0;
		}
	}
    return -1;
}

void uci_element_parseSubscriber(struct uci_package *package, struct mosquitto **mosq)
{
	struct uci_element *i, *j;
	uci_foreach_element(&package->sections, i){
		struct uci_section *section = uci_to_section(i);
		char *section_type = section->type;
		char *topic = NULL;
		char *qos = NULL;

		if(strcmp(section_type,"topic") == 0){
			uci_foreach_element(&section->options, j){
				struct uci_option *option = uci_to_option(j);
				uci_element_Check(option,"topic", &topic);
				uci_element_Check(option,"qos", &qos);
			}
			if((topic != NULL) && (qos != NULL))
				mqtt_subscribe(mosq, topic, atoi(qos));
		}
	}
}

int uci_element_parseTopic(struct uci_package *package, const struct mosquitto_message *msg, struct topic *top)
{
	struct uci_element *i, *j;
	uci_foreach_element(&package->sections, i){
		struct uci_section *section = uci_to_section(i);
		char *section_type = section->type;
		if(strcmp(section_type,"topic") == 0){
			uci_foreach_element(&section->options, j){
				struct uci_option *option = uci_to_option(j);
				uci_element_Check(option,"topic", &top->name);
				uci_element_Check(option,"key", &top->key);
				uci_element_Check(option,"comparison", &top->Comparison);
				uci_element_Check(option,"type", &top->type);
				uci_element_Check(option,"value", &top->value);
                uci_element_Check(option,"recEmail", &top->recEmail);
			}
			if(strcmp(msg->topic, top->name) == 0)
			if((checkParameter(top->key,"Topic key not specified..."))&& 
				(checkParameter(top->Comparison,"Topic comparison not specified..."))&&
				(checkParameter(top->type,"Topic type not specified..."))	&&
				(checkParameter(top->value,"Topic value not specified..."))&&
				(checkParameter(top->recEmail,"Topic recEmail not specified...")) )
					return 0;
		}
	}
    return -1;
}