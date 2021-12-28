#define CONFIG "m_conf"

#define LESSEQUAL 4
#define MOREEQUAL 1
#define EQUAL 2
#define NOTEQUAL 3
#define LESS 5
#define MORE 6

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

