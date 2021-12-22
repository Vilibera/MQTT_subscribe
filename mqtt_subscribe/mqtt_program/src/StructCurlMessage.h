#define CONFIG "m_conf"

#define EQUAL 2
#define NOT_EQUAL 3
#define LESS 5
#define MORE 6
#define LESS_EQUAL 4
#define MORE_EQUAL 1
#define STRING 1
#define DECIMAL 2

struct sender{
	char *email;
	char *username;
	char *password;
	char *smtpPort;
	char *smtpIP;
};



struct topic {
	char *name;
	char *key;
	char *value;
	char *Comparison;
	char *recEmail;
	char *type;

};

