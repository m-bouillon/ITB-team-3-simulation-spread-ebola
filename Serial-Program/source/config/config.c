#include "config.h"

config readConfigFile(char* filepath){
    char* configFile = loadFile(filepath);
    //Parse json
    const nx_json* json =nx_json_parse_utf8(configFile);
    config conf = {
        nx_json_get(json,"sizeX")->int_value,
        nx_json_get(json,"sizeY")->int_value,
        nx_json_get(json,"infectedPercent")->int_value,
        nx_json_get(json,"incubatedPercent")->int_value,
        nx_json_get(json,"lapsNumber")->int_value
    };
    //Free memory
    nx_json_free(json);
    return conf;
}

char* loadFile(char* filepath) {

    if( access( filepath, F_OK ) == -1 ) {
      ERROR("The file does not exist : ", filepath);
      exit(0);
    }
    struct stat st;
    if (stat(filepath, &st)==-1) {
      exit(0);
    }
    int fd=open(filepath, O_RDONLY);
    if (fd==-1) {
      ERROR("can't open file", filepath);
      exit(0);
    }
    char* text=malloc(st.st_size+1);
    if (st.st_size!=read(fd, text, st.st_size)) {
      ERROR("can't read file", filepath);
      close(fd);
      exit(0);
    }
    close(fd);
    text[st.st_size]='\0';
    return text;
}

void displayHelp(){
    printf("\nThis program is executed by taking as parameter a config file, named as you want it\n");
    printf("The file structure must be the following :\n\n");
    printf("Example : \n");
    printf("{\n\t\"sizeX\" : 10,\n\t\"sizeY\" : 10,\n\t\"infectedPercent\" : 2,\n\t\"incubatedPercent\" : 2,\n\t\"lapsNumber\" : 1000\n}\n\n");
    printf("The program must be executed in this way 'display path/to_config'..\n\n");
}

int checkArgvParameter(char* param){
    int sizeParam = 0;
    int sizeHelp = 0;
    int sizeH = 0;    
    int i = 0;
    char* help = "--help";
    char* h = "-h";   
    i = 0;
    while(param[i++]!='\0') sizeParam++;    
    i = 0;
    while(help[i++]!='\0') sizeHelp++;    
    i = 0;
    while(h[i++]!='\0') sizeH++;

    if(sizeHelp == sizeParam)
      return compareStrings(param, help, sizeParam);
    else if(sizeH == sizeParam)
      return compareStrings(param, h, sizeParam);
    else
      return 0;
}

int compareStrings(char* str1, char* str2, int size){
  	int i=0;
	  for(i=0; i<size; i++){
	  	if(str1[i]!=str2[i]){
	  		return -1;
	  	}
	  }
	return 1;
}

int checkConfFile(config conf){
  if(conf.sizeX <= 0)
    return -1;
  else if(conf.sizeY <= 0)
    return -1;
  else if(conf.infectedPercent <= 0)
    return -1;
  else if(conf.incubatedPercent <= 0)
    return -1;
  else
    return 0;
}
