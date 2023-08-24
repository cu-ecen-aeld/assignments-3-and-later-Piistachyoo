#include <stdio.h>
#include <syslog.h>

#define LOG_PRIO(_LEVEL)	(LOG_USER | _LEVEL)

int main(int argc, char **argv){
	char *string;
	// Check if argc is less than 2
	if(3 != argc){
		openlog("writer", LOG_CONS, LOG_USER);
		syslog(LOG_PRIO(LOG_ERR), "ERROR!\nUsage: writer <file> <string>\n");
		closelog();
		printf("ERROR!\nUsage: writer <file> <string>\n");	
		return 1;
	}
	else{
		FILE *myfile = fopen(argv[1], "w");
		if(NULL == myfile){
			openlog("writer", LOG_CONS, LOG_USER);
			syslog(LOG_PRIO(LOG_ERR), "ERROR!\n Path is incorrect\n");
			closelog();
			printf("ERROR!\nPath is incorrect\n");
		}
		else{
			string = argv[2];
			while(*string){
				fwrite(string, sizeof(char), 1, myfile);
				string++;
			}

			openlog("writer", LOG_CONS, LOG_USER);
			syslog(LOG_PRIO(LOG_DEBUG), "Writing %s to %s", argv[0], argv[1]);	
			closelog();
		}
	}
	return 0;
}
