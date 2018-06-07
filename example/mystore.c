#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

void printmenu(mymenu_t *m[], int mcount);
int makemymenu(const char *json, jsmntok_t *t, int tokcount, mymenu_t * m[]);
void printkeys(const char *json, jsmntok_t *t, int tokcount);
char * readjsonfile(const char * filename);
int findkeys(const char *json, jsmntok_t *t, int tokcount, int *keys);
void printvalues(const char *json, jsmntok_t *t, int tokcount,int *keys);
void printall(const char *json, jsmntok_t * t, int tokcount);
static char * JSON_STRING;
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
	strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
	return 0;
	}
	return -1;
}

int main(int argc,char * argv[]) {
	int i;
	int r;
	char typename[5][20]=
 {"JSMN_UNDEFINED","JSMN_OBJECT","JSMN_ARRAY","JSMN_STRING","JSMN_PRIMITIVE"};
	int keyarray[128], keyamount;

	if(argc==1)
	JSON_STRING = readjsonfile("mymenu.json");
	else 
	JSON_STRING = readjsonfile(argv[1]);
	printf("%s \n",JSON_STRING);	
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
#ifdef DEBUG_MODE
	printf("\n<JSON_STRING>\n");
	printf("%s",JSON_STRING);
        printf("\n============\n");
#endif
	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	
#ifdef DEBUG_MODE
	for(i=0;i<r;i++){
		printf("[%2d] (%d) %d~%d, size:%d\n", i, t[i].type, t[i].start,t[i].end, t[i].size);
	}	
#endif 
//-----------------------------------------------------------//
//	printall(JSON_STRING, t,r);
	menucount = makemymenu(JSON_STRING, t, r, mymenu);
	printmenu(mymenu, menucount);
//-----------------------------------------------------------//

//	printf("\n------------------------------\n");
//	printkeys(JSON_STRING, t,r);
//	printf("\n------------------------------\n");
//	keyamount = findkeys(JSON_STRING, t, r, keyarray);
//	printvalues(JSON_STRING, t, r, keyarray);
//	printf("\n------------------------------\n");
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}

	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "user") == 0) {
			/* We may use strndup() to fetch string value */
			printf("- User: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "admin") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			printf("- Admin: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "uid") == 0) {
			/* We may want to do strtol() here to get numeric value */
			printf("- UID: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "groups") == 0) {
			int j;
			printf("- Groups:\n");
			if (t[i+1].type != JSMN_ARRAY) {
				continue; /* We expect groups to be an array of strings */
			}
			for (j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
				printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
			}
			i += t[i+1].size + 1;
		} else {
		if(t[i].size==1)
		if(strcmp(typename[t[i].type],"JSMN_ARRAY")!=0)
        	if(strcmp(typename[t[i].type],"JSMN_OBJECT")!=0)
		{	
      	printf("- %.*s : \n",t[i].end-t[i].start,JSON_STRING+ t[i].start);
       		}
		if(strcmp(typename[t[i+1].type],"JSMN_ARRAY")!=0)
		printf("{");
		//if(strcmp(typename[t[i+1].type],"JSMN_OBJECT")!=0)
               // printf("[");
		if(t[i+1].size==0)
                {
        printf("-> %.*s ", t[i+1].end-t[i+1].start,JSON_STRING + t[i+1].start);
		//if(strcmp(typename[t[i+1].type],"JSMN_OBJECT")!=0)
               // printf("]");
		if(strcmp(typename[t[i+1].type],"JSMN_ARRAY")!=0)
                printf("}");

		 printf("\n");
                       i++;
                 }
			
	    }
	}
	return EXIT_SUCCESS;
}




int makemymenu(const char *json, jsmntok_t *t, int tokcount, mymenu_t * m[]){



}


void printmenu(mymenu_t *m[], int mcount){

}



int findkeys(const char *json, jsmntok_t *t, int tokcount, int *keys){
	 char typename[5][20]=
 {"JSMN_UNDEFINED","JSMN_OBJECT","JSMN_ARRAY","JSMN_STRING","JSMN_PRIMITIVE"};
	int n = 0;
	int i;
	for(i=1;i<tokcount;i++) 
	{
	if(t[i].size==1)
        if(strcmp(typename[t[i].type],"JSMN_ARRAY")!=0)
        if(strcmp(typename[t[i].type],"JSMN_OBJECT")!=0)
       		{
		 keys[n]=i;
	//	printf("---------------%d\n",keys[n]);
		 n++;	
			
		}
        }
	
	return n;
}
void printvalues(const char *json, jsmntok_t *t, int tokcount,int *keys){
	 int n = 0;
	int i,j,k;k=0;
	while(1){
	if(keys[k]==NULL)break;
	k++;
	}
	int len =k ;
//printf(">>>>>>>>>>>>>>>>>>>>>>>%d",len);
	printf("printvalues : \n");
        for(j=0;j<len;j++)
	{ 
	 for(i=1;i<tokcount;i++)
        {
           if(i==keys[j])
  printf(" %.*s (%d) \n",t[i].end-t[i].start,json + t[i].start,i);
       	        
        }
	}

}


void printkeys(const char *json, jsmntok_t *t, int tokcount){
  int i;
  int n =0;
  char typename[5][20]=
  {"JSMN_UNDEFINED","JSMN_OBJECT","JSMN_ARRAY","JSMN_STRING","JSMN_PRIMITIVE"};
  printf("***** All keys *****\n");
  for(i=1;i<tokcount;i++) {
    if(t[i].size==1)
	if(strcmp(typename[t[i].type],"JSMN_ARRAY")!=0)
	if(strcmp(typename[t[i].type],"JSMN_OBJECT")!=0)
	{
	
	n++;
	printf("[%2d] %.*s (%d) \n",n,t[i].end-t[i].start,json + t[i].start,i);
	}	
  
  }
  printf("\n");
}
 

void printall(const char *json, jsmntok_t * t, int tokcount){
  int i;
  char typename[5][20]=
 {"JSMN_UNDEFINED","JSMN_OBJECT","JSMN_ARRAY","JSMN_STRING","JSMN_PRIMITIVE"};
  printf("***** All Tokens *****\n");
  for(i=1;i<tokcount;i++) {
    #ifdef JSMN_PARENT_LINKS
    printf("[%2d] %.*s (size=%d, %d~%d, %s) P=%d\n",i,t[i].end-t[i].start,json + t[i].start, t[i].size, t[i].start, t[i].end, typename[t[i].type],t[i].parent);
    #else
    printf("[%2d] %.*s (size=%d, %d~%d, %s) \n",i,t[i].end-t[i].start,json + t[i].start, t[i].size, t[i].start, t[i].end, typename[t[i].type]);
    #endif
  }
  printf("\n");
};


char * readjsonfile(const char * filename){
  char * line;
  char temp[200];

  line = (char *)malloc(sizeof(char));
  FILE * fp;
  fp = fopen(filename, "rt");
   while(fgets(temp, sizeof(temp), fp) != NULL){
        temp[strlen(temp)-1]== '\n';//temp의 엔터값 삭제 하고 이어붙인다.
  	line = (char*)realloc(line,strlen(line) + strlen(temp));
	strcat(line,temp);
  }	
  fclose(fp);
  return line;
}
