#include <stdio.h>
#include <curl/curl.h>
#include <syslog.h>
#include <string.h>
#include "StructCurlMessage.h"
#include "StructAddress.h"
#include <stdlib.h>
 
static const char *payload_text = NULL;
struct upload_status {
  size_t bytes_read;
};
 
static size_t payload_source(char *ptr, size_t size, size_t nmemb, void *userp)
{
  struct upload_status *upload_ctx = (struct upload_status *)userp;
  const char *data;
  size_t room = size * nmemb;
 
  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
    return 0;
  }
 
  data = &payload_text[upload_ctx->bytes_read];
 
  if(data) {
    size_t len = strlen(data);
    if(room < len)
      len = room;
    memcpy(ptr, data, len);
    upload_ctx->bytes_read += len;
 
    return len;
  }
 
  return 0;
}
 
void getUrl(char *ip, char *port , char **ptr){
	char url[1024];
	char *checkaddress = NULL;

	if(port != NULL || ip != NULL){
		sprintf(url, "smtp://%s:%s",ip, port);
		// checkaddress = strstr(url, "smtp:/");
		// if(!checkaddress){
		// 	sprintf(url, "smtp://%s:%s", ip, port);
		// }
	}
	else{
		syslog(LOG_ERR,"Wrong url address");
		checkaddress = strstr(ip, "smtp://");
		if(!checkaddress){
			sprintf(url, "smtp://%s:%s", ip, port);
		}
	}
	*ptr = url;
	memset(url, 0, sizeof(url));
}

void curl_message(char *recipient,struct sender *sen, char *message)
 {	
	char errbuf[CURL_ERROR_SIZE];
 	payload_text = NULL;
	CURL *curl;
 	errbuf[0] = 0;
 
	CURLcode res = CURLE_OK;
	struct curl_slist *recipients = NULL;
	struct upload_status upload_ctx = { 0 };
	char *url = NULL;
	curl = curl_easy_init();
	if(curl){
		CURL *handle = curl_easy_init();
		char error[CURL_ERROR_SIZE]; 
		payload_text = message;
		getUrl(sen->smtpIP,sen->smtpPort,&url);
		
		curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_MAIL_FROM, sen->email );
 	


		recipients = curl_slist_append(recipients,recipient);
		curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

	
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
		curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

		

		curl_easy_setopt(curl, CURLOPT_USERNAME, sen->username);
		curl_easy_setopt(curl, CURLOPT_PASSWORD, sen->password);

		CURLcode ret = curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, error);

		
		res = curl_easy_perform(curl);
		if(res != CURLE_OK){
			syslog(LOG_ERR,"curl_easy_perform() failed: %s", curl_easy_strerror(res));
			size_t len = strlen(errbuf);
   			syslog(LOG_ERR, "\nlibcurl: (%d) ", res);
    		if(len)
      			syslog(LOG_ERR, "%s%s", errbuf,((errbuf[len - 1] != '\n') ? "\n" : ""));
		}
			
		else
			syslog(LOG_INFO, "Message sent to email: %s\r", recipient);
		curl_slist_free_all(recipients);
		curl_easy_cleanup(curl);
	}
}