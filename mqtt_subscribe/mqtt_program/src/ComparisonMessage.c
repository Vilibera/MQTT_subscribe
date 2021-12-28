#include <uci.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <ctype.h>
#include <string.h>
#include <json-c/json.h>
#include <syslog.h>
#include "mqtt_functions.h"
#include "StructCurlMessage.h"
#include "curl_message.h"



char comp[20];
char message[1023];
char buffermessage[1023];
char buffertopicmessage[1023];
void curl_string_message_format(struct topic *topic,struct sender *sender,char *messageValue,char *comp)
{	
	snprintf(buffermessage,100,"%s",messageValue);
	snprintf(buffertopicmessage,100,"%s",topic->value);
	syslog(LOG_INFO, "Message received: topic: %s message value: %s topic value: %s",topic->name, buffermessage, buffertopicmessage);
	sprintf(message, "Subject: Received message from: %s\r\nTopic value is == %s\r\nMessage value is == %s\r\n key: %s\r\n Message value %s Topic value",
		topic->name, buffertopicmessage, buffermessage, topic->key, comp);
	curl_message(topic->recEmail, sender, message);
}
void curl_decimal_message_format(struct topic *topic,struct sender *sender,int messageValue, int topicValue,char *comp)
{
	syslog(LOG_INFO, "Message received: topic: %s message value: %d topic value: %d",
	topic->name, messageValue, topicValue); 
	sprintf(message,"Subject: Received message from: %s\r\nTopic value is == %d\r\nMessage value is == %d\r\n key: %s\r\n Message value %s Topic value\r\n",
		topic->name, topicValue, messageValue, topic->key,comp);
	curl_message(topic->recEmail, sender, message);
}

void Uci_string_Comparison_Check(char *messageValue, struct topic *topic, struct sender *sender )
{
	int value;
	value=strcmp(messageValue,topic->value);  
	
		switch(atoi(topic->Comparison)){
			case EQUAL: // ==
				if(value== 0){
					sprintf(comp,"EQUAL");
					curl_string_message_format(topic,sender,messageValue,comp);
				}
				break;

			case NOTEQUAL: // !=
				if(value != 0){
					sprintf(comp,"NOT_EQUAL");
					curl_string_message_format(topic,sender,messageValue,comp);
				}
				break;

			default:
					syslog(LOG_ERR, "Wrong comparison for string type available only(EQUAL - NOT_EQUAL)");
				break;	
		}
}

void Uci_decimal_Comparison_Check(int messageValue, int topicValue, struct topic *topic, struct sender *sender )
{	
		switch(atoi(topic->Comparison)){
			case MOREEQUAL: // >=
				if(messageValue >= topicValue){
					sprintf(comp,"MORE_EQUAL");
					curl_decimal_message_format(topic,sender,messageValue,topicValue,comp);
				}
			break;
			case EQUAL: // ==
				if(messageValue == topicValue){
					sprintf(comp,"EQUAL");
					curl_decimal_message_format(topic,sender,messageValue,topicValue,comp);
				}
				break;

			case NOTEQUAL: // !=
				if(messageValue != topicValue){
					sprintf(comp,"NOT_EQUAL");
					curl_decimal_message_format(topic,sender,messageValue,topicValue,comp);
				}
				break;

			case LESSEQUAL: // <=
				if(messageValue <= topicValue){
					sprintf(comp,"LESS_EQUAL");
					curl_decimal_message_format(topic,sender,messageValue,topicValue,comp);
				}
				break;
			case LESS: // <
				if(messageValue < topicValue){
					sprintf(comp,"LESS");
					curl_decimal_message_format(topic,sender,messageValue,topicValue,comp);
				}
				break;
			case MORE: // >
				if(messageValue > topicValue){
					sprintf(comp,"MORE");
					curl_decimal_message_format(topic,sender,messageValue,topicValue,comp);
				}
				break;
			default:
					syslog(LOG_ERR, "Wrong comparison)");
				break;
			
		}
	}

void uci_Check_Type(struct topic *topic, struct sender *sender, char *messageValue)
{
	
	char *topicPtr;
	char *messagePtr;
	long topicNum = strtol(topic->value, &topicPtr, 10);
	long messageNum = strtol(messageValue, &messagePtr, 10);
	switch(atoi(topic->type)){
   
		case STRING:
			if(topicPtr != topic->value){
				syslog(LOG_ERR,"Value from input is a number, Your chosen type is String");
				syslog(LOG_INFO, "Message received: topic: %s key: %s, value: %s",
				topic->name, topic->key, messageValue);
				break;
			}

			if(messagePtr != messageValue){
				syslog(LOG_ERR,"Value from message is a number, Your chosen type is String");
				syslog(LOG_INFO, "Message received: topic: %s key: %s, value: %s",
				topic->name, topic->key, messageValue);
				break;
			}
                	Uci_string_Comparison_Check(messageValue,topic,sender);
		break;
		
		case DECIMAL:
				if(topicPtr == topic->value){
					syslog(LOG_ERR,"Value from input is a string, Your chosen type is decimal");
					syslog(LOG_INFO, "Message received: topic: %s key: %s, value: %s",
					topic->name, topic->key, messageValue);
					break;
				}
				
				if(messagePtr == messageValue){
					syslog(LOG_ERR,"Value from message is a string, Your chosen type is decimal");
					syslog(LOG_INFO, "Message received: topic: %s key: %s, value: %s",
					topic->name, topic->key, messageValue);
					break;
				}
                    Uci_decimal_Comparison_Check(messageNum,topicNum,topic,sender);
		break;

		default:
			syslog(LOG_ERR, "Wrong type available only(STRING OR DECIMAL)");
		break;
	}
}

