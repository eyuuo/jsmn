#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */
/*
static const char *JSON_STRING =
	"{\"user\": \"yukyoung\", \"admin\": false, \"uid\":21700549,\n  "
	"\"groups\": [\"handong\", \"anjung\", \"427\", \"260\"]}";
*/

char * readjsonfile();

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int main() {
	int i;
	int r;
	char*JSON_STRING = readjsonfile();
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
			printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
					JSON_STRING + t[i].start);
		}
	}
	return EXIT_SUCCESS;
}



char * readjsonfile(){
  char * line;
  char * temp;
  char * del_line_p;
  int N, i, n=0;
  char c;

  FILE *fp1;
  fp1=fopen("data.txt","r");
  while((c=fgetc(fp1))!=EOF)
    if(c=='\n') n++;
  fclose(fp1);
	temp = (char *)malloc(sizeof(char));
  line = (char *)malloc(sizeof(char));
  FILE * fp;
  fp = fopen("data.txt", "rt");
  fgets(line, sizeof(line), fp);
  if ((del_line_p = (char*)strchr(line, '\n')) != NULL)*del_line_p = '\0';
  strcpy(temp,line);
 while(i<n){    i++;
    fgets(line, sizeof(line), fp);
    if ((del_line_p = (char*)strchr(line, '\n')) != NULL)*del_line_p = '\0';
    N=strlen(temp);
    temp = (char *) realloc(temp, strlen(line));
    strncpy(temp+N,line,strlen(line));
  }
	fclose(fp);
  return temp;
}
