#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"


//구조체 타입 선언
typedef enum {
 C_FOOD = 0,//set
 C_SET =1 //food
}mycategory_t;

typedef struct {
  mycategory_t cat; //메뉴분류//cat=0이면 food
  char name[20]; // 메뉴명
  char size[10]; // 사이즈
  int price; //가격
}mymenu_t;


void printmenu(mymenu_t *m[], int mcount);
int makemymenu(const char *json, jsmntok_t *t, int tokcount, mymenu_t * m[]);
char * readjsonfile(const char * filename);
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
  mymenu_t * mymenu[20];
	char typename[5][20]=
 {"JSMN_UNDEFINED","JSMN_OBJECT","JSMN_ARRAY","JSMN_STRING","JSMN_PRIMITIVE"};
	int keyarray[128], keyamount;
  int menucount =0 ;
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


	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}

  //-----------------------------------------------------------//

    menucount = makemymenu(JSON_STRING, t, r, mymenu);
  	printmenu(mymenu, menucount);
    int n,w;
    int total=0;
    printf("We only take orders five times a day.\n");
    printf("Please select the menu by number.\n");
    for(w=0;w<5;w++)
    {
      printf("Ordwe %d :\n",w+1);
      scanf("%d",&n);
      if(n==w+1)
      total=total+ mymenu[w]->price;
    }
    printf("todays total: %d",total);
  //-----------------------------------------------------------//


   return EXIT_SUCCESS;
}



int makemymenu(const char *json, jsmntok_t *t, int tokcount, mymenu_t * m[]){
  int i;
  int Num1=0;
  int Num2=0;
  char str[50];
  /* Loop over all keys of the root object */
  for (i = 1; i < tokcount; i++) {
    
     m[Num1]=(mymenu_t*)malloc(sizeof(mymenu_t));
    Num1++;
    if (jsoneq(json, &t[i], "name") == 0) {

    strncpy(m[Num1]->name,json+ t[i+1].start,t[i].end-t[i].start);
      i++;
    } else if (jsoneq(json, &t[i], "size") == 0) {
      /* We may use strndup() to fetch string value */
        strncpy(m[Num1]->size,json+ t[i+1].start,t[i].end-t[i].start);
      i++;
    } else if (jsoneq(json, &t[i], "price") == 0) {
      /* We may want to do strtol() here to get numeric value */
        
     strncpy(str,json+ t[i+1].start,t[i].end-t[i].start);
     m[Num1]->price= atoi(str);
      i++;
    } else if (jsoneq(json, &t[i], "food") == 0) {
      int j;
      m[Num1]->cat =0;
      if (t[i+1].type != JSMN_ARRAY) {
        continue; /* We expect groups to be an array of strings */
      }
      i += t[i+1].size + 1;
    }else if (jsoneq(json, &t[i], "set_menu") == 0) {
      int j;
      m[Num1]->cat =0;
      if (t[i+1].type != JSMN_ARRAY) {
        continue; /* We expect groups to be an array of strings */
      }
      i += t[i+1].size + 1;
    } else {

  return Num1;
     }
   }
return Num1;
}


void printmenu(mymenu_t *m[], int mcount){
  int i;
  for(i=0;i<mcount;i++)
  {
    if(m[i]->cat==C_FOOD)
      printf("%d food :\n%s\n%s\n%d",i+1,m[i]->name,m[i]->size,m[i]->price);
    if(m[i]->cat==C_SET)
      printf("%d set_menu :\n%s\n%s\n%d",i+1,m[i]->name,m[i]->size,m[i]->price);
  }
}
/*
typedef enum {
 C_FOOD = 0,//set
 C_SET =1 //food
}mycategory_t;

typedef enum {
  mycategory_t cat; //메뉴분류//cat=0이면 food
  char name[20]; // 메뉴명
  char size[10]; // 사이즈
  int price; //가격
}mymenu_t;

*/
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

