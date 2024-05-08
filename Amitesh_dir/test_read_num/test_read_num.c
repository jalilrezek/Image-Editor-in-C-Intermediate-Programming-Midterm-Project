#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


int read_num( FILE *fp ) {
    assert(fp);
  
    int ch;
    while((ch = fgetc(fp)) == '#') { // # marks a comment line
      printf("First While:%d\n",ch);
	  while( ((ch = fgetc(fp)) != '\n') && ch != EOF ) {
        printf("Nested While:%d\n",ch);
		/* discard characters til end of line */
      }
    }
    ungetc(ch, fp); // put back the last thing we found
  
    int val;
    if (fscanf(fp, "%d", &val) == 1) { // try to get an int
      printf("If:%d\n",val);
	  while(isspace(ch = fgetc(fp))) {
        // drop trailing whitespace
      }
      ungetc(ch, fp);
      return val; // we got a value, so return it
    } else {
      fprintf(stderr, "Error:ppm_io - failed to read number from file\n");
      return -1;
    }
  }

int main () {

	FILE *fp = fopen("sample.txt","r");
	int ch;
	while((ch= fgetc(fp)) != EOF){
		ungetc(ch,fp);
		read_num (fp);
	}
	//read_num(fp);
	fclose(fp);
	return 0;

}
