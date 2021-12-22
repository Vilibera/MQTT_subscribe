void uci_element_Check(struct uci_option *option, char* type, char** value);
void uci_element_parseSubscriber(struct uci_package *package, struct mosquitto **mosq);
int uci_element_parse_Checks(struct uci_package *package, struct sender *sen);
int uci_element_parseTopic(struct uci_package *package,  const struct mosquitto_message *msg, struct topic *top);