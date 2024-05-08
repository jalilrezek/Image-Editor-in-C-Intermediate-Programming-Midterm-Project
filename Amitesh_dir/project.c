//project.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppm_io.h"
#include "image_manip.h"

// Return (exit) codes
#define RC_SUCCESS            0
#define RC_MISSING_FILENAME   1
#define RC_OPEN_FAILED        2
#define RC_INVALID_PPM        3
#define RC_INVALID_OPERATION  4
#define RC_INVALID_OP_ARGS    5
#define RC_OP_ARGS_RANGE_ERR  6
#define RC_WRITE_FAILED       7
#define RC_UNSPECIFIED_ERR    8

void print_usage();

int main (int argc, char* argv[]) {
  // Complete the implementation of main
 
 FILE *infile = fopen(argv[1],"rb");
 //FILE *infile_2 = fopen(argv[2],"rb");
 FILE *outfile = fopen(argv[2],"wb");

 Image in1 = read_ppm(infile);
 // Image in2 = read_ppm(infile_2);
 Image gray = grayscale(in1);
 //Image blurr = blur(in1,5.0);  
 //Image point = pointilism(in1,1);
 //Image bld =  blend (in1, in2 ,0.5); 
 //Image rotate = rotate_ccw(in1); 
 //Image sat = saturate(in1,2); 
 
 write_ppm(outfile,gray); 
 //free_image(&sat);
 free_image(&gray);
 free_image(&in1);
 //free_image(&in2);
 //free_image(&blurr);
 fclose(infile);
 fclose(outfile);

 return RC_SUCCESS; // Replace Me!
}

void print_usage() {
  printf("USAGE: ./project <input-image> <output-image> <command-name> <command-args>\n");
  printf("SUPPORTED COMMANDS:\n");
  printf("   grayscale\n" );
  printf("   blend <target image> <alpha value>\n" );
  printf("   rotate-ccw\n" );
  printf("   pointilism\n" );
  printf("   blur <sigma>\n" );
  printf("   saturate <scale>\n" );
}


