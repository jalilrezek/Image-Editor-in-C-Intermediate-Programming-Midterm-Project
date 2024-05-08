//project.c

#include <ctype.h>
#include <stdbool.h>

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

bool isRealNumber(const char *str);
bool isInteger(const char *str);

int execute_project(int argc, char* argv[]); 

int main (int argc, char* argv[]) {

  int result = execute_project(argc, argv);
  //printf("Exit code: %d\n", result);
  return result; 
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

bool isRealNumber(const char *str) { // handles things like: 0. .5 does NOT allow the lone period, " . "
    if (str == NULL || *str == '\0') // Check for empty string or NULL pointer
        return false;

    // Skipping leading whitespace
    while (isspace(*str))
        str++;

    // Handling optional sign
    if (*str == '+' || *str == '-')
        str++;

    bool hasDigits = false;
    bool hasDot = false;

    while (*str != '\0') {
        if (isdigit(*str)) {
            hasDigits = true;
        } else if (*str == '.' && !hasDot) {
            hasDot = true;
        } else {
            // If we encounter any other character, it's not a valid real number
            return false;
        }
        str++;
    }

    // At least one digit must be present
    return hasDigits;
}

/*bool isRealNumber(const char *str) {
    bool hasDot = false;
    if (!isdigit(str[0])) { // the lone period will now not count. Nor will a number like ".5" but "0.5" however will make it
      return false;
    }
    while (*str) {
        if (*str == '.') {
            if (hasDot) // If it already has a dot, then more than one dot is not a valid real number
                return false;
            hasDot = true;
        } else if (!isdigit(*str)) {
            return false;
        }
        str++;
    }
    bool result = isInteger(str);
    if (!hasDot) {
      return result;
    } else {
      return hasDot;
    }
}

bool isInteger(const char *str) {
    while (*str) {
        if (!isdigit(*str))
            return false;
        str++;
    }
    return true;
} */


int execute_project(int argc, char* argv[]) {
    if (argc < 4) { 
        print_usage();
        return RC_MISSING_FILENAME;
    }
    char *filename = argv[1];
    char *operation = argv[3];

    FILE* infile = fopen(filename, "rb");

    Image im;

    if (infile == NULL) { 
       // printf("Failed to open file\n"); 
        return RC_OPEN_FAILED; 
    } 

    if (strcmp(operation, "grayscale") != 0 && strcmp(operation, "rotate-ccw") != 0 &&
      strcmp(operation, "saturate") != 0 && strcmp(operation, "blend") != 0 && 
      strcmp(operation, "pointilism") != 0 && strcmp(operation, "blur") != 0) {
        //("Unsupported image processing operation\n");
        return RC_INVALID_OPERATION;
    }


    else if (strcmp(operation, "grayscale") == 0 || strcmp(argv[argc-1], "grayscale") == 0) {
      if (argc != 4) {
        //("incorrect number of arguments for %s\n", operation);
        fclose(infile);
        return RC_INVALID_OP_ARGS;
      }

      im = read_ppm(infile);
      if (im.cols == 0 && im.rows == 0 && im.data == NULL) {
        //printf("Invalid ppm\n");
        free_image(&im);
        fclose(infile);
        return RC_INVALID_PPM; 
      }

      FILE* gray = fopen(argv[2], "wb");
      if (!gray) {
        free_image(&im);
        fclose(infile);
        return RC_WRITE_FAILED;
      }

      Image grayImage;

      grayImage = grayscale(im);
      int numWritten = write_ppm(gray, grayImage);
      if (numWritten != im.rows * im.cols) { // failed to write to output file properly
        free_image(&im);
        free_image(&grayImage);
        fclose(infile);
        return RC_WRITE_FAILED;
      }

      fclose(gray);
      free_image(&grayImage);

    } 


    
    else if (strcmp(operation, "rotate-ccw") == 0 || strcmp(argv[argc-1], "rotate-ccw") == 0) {
      if (argc != 4) {
        //printf("incorrect number of arguments for %s\n", operation);
        fclose(infile);
        return RC_INVALID_OP_ARGS;
      }
      im = read_ppm(infile);
      if (im.cols == 0 && im.rows == 0 && im.data == NULL) {
        //("Invalid ppm\n");
        free_image(&im);
        fclose(infile);
        return RC_INVALID_PPM; 
      }
      FILE* rotated = fopen(argv[2], "wb");
      if (!rotated) {
        free_image(&im);
        fclose(infile);
        return RC_WRITE_FAILED;
      }

      Image rotatedImage;

      rotatedImage = rotate_ccw(im);
      int numWritten = write_ppm(rotated, rotatedImage);
      if (numWritten != im.rows * im.cols) { // failed to write to output file properly
        free_image(&im);
        free_image(&rotatedImage);
        fclose(infile);
        return RC_WRITE_FAILED;
      }
      fclose(rotated);
      free_image(&rotatedImage);
    } 
    

    
    else if (strcmp(operation, "saturate") == 0 || strcmp(argv[argc-2], "saturate") == 0) { // need to fix this.
      if (argc != 5) {
        //("incorrect number of arguments for %s\n", operation);
        fclose(infile);
        return RC_INVALID_OP_ARGS;
      }

      im = read_ppm(infile);
      if (im.cols == 0 && im.rows == 0 && im.data == NULL) {
        //printf("Invalid ppm\n");
        free_image(&im);
        fclose(infile);
        return RC_INVALID_PPM; 
      }

      if(!isRealNumber(argv[4])) {
       // printf("Invalid arguments for %s\n", operation);
        free_image(&im);
        fclose(infile);
        //printf("MADE IT %d\n", RC_OP_ARGS_RANGE_ERR);
        return RC_OP_ARGS_RANGE_ERR;
      }

      double scaleFactor = atof(argv[4]);

      FILE* saturateFile = fopen(argv[2], "wb");
      if (!saturateFile) {
        free_image(&im);
        fclose(infile);
        return RC_WRITE_FAILED;
      }

      Image saturatedImage;

      saturatedImage = saturate(im, scaleFactor);
      int numWritten = write_ppm(saturateFile, saturatedImage);
     if (numWritten != im.rows * im.cols) { // failed to write to output file properly
        free_image(&im);
        free_image(&saturatedImage);
        fclose(infile);
        return RC_WRITE_FAILED;
      }

      fclose(saturateFile);
      free_image(&saturatedImage);
    } 
    
    else if (strcmp(argv[3], "blend") == 0 || strcmp(argv[argc-3], "blend") == 0) { 
      if (argc != 6) {
        //printf("incorrect number of arguments for %s\n", operation);
        fclose(infile);
        return RC_INVALID_OP_ARGS; 
      }

      im = read_ppm(infile);
      if (im.cols == 0 && im.rows == 0 && im.data == NULL) {
        //printf("Invalid ppm\n");
        free_image(&im);
        fclose(infile);
        return RC_INVALID_PPM; 
      }

      if (!isRealNumber(argv[5])) {
        //("Invalid arguments for %s\n", operation);
        free_image(&im);
        fclose(infile);
        //printf("Invalid alpha for blend. Exit code: %d\n", RC_OP_ARGS_RANGE_ERR);
        return RC_OP_ARGS_RANGE_ERR;
      }

      double alpha = atof(argv[5]);

      char* file2 = argv[2];
      FILE* secondFile = fopen(file2, "r");

      if (secondFile == NULL) {
        //printf("Unable to open second file for blend\n"); // should this instead be invalid argument error? Subjective?
        free_image(&im);
        fclose(infile);
        //printf("Unable to open second file for blend. Exit code: %d\n", RC_OPEN_FAILED);
        return RC_OPEN_FAILED;
      }



      Image im2 = read_ppm(secondFile);
      if (im.cols == 0 && im.rows == 0 && im.data == NULL) {
        free_image(&im);
        free_image(&im2);
        fclose(infile);
        return RC_INVALID_PPM; // read_ppm will already have printed out some error message to specify what's going on. But, see
        } // "problem" above. Are some of these errors not because of invalid ppm? If so, would it be wrong to group them under
        // invalid ppm error as is done here?

      FILE* blendFile = fopen(argv[4], "wb");
      if (!blendFile) {
        free_image(&im);
        fclose(infile);
        return RC_WRITE_FAILED;
      }

      Image blendImage;

      blendImage = blend(im, im2, alpha);

      int numWritten = write_ppm(blendFile, blendImage);


      if (im.rows >= im2.rows && im.cols >= im2.cols) {
        if (numWritten != im.rows * im.cols) { // failed to write to output file properly
          free_image(&im);
          free_image(&im2);
          free_image(&blendImage);
          fclose(infile);
          //printf("Unable to open %s for writing in blend\n", argv[4]);
          return RC_WRITE_FAILED;
        }
      } else if (im2.rows >= im.rows && im2.cols >= im.cols) {
        if (numWritten != im2.rows * im2.cols) { // failed to write to output file properly
          free_image(&im);
          free_image(&im2);
          free_image(&blendImage);
          fclose(infile);
          //printf("Unable to open %s for writing in blend\n", argv[4]);
          return RC_WRITE_FAILED;
        }
      } else if (im.rows >= im2.rows && im.cols <= im2.cols) {
        if (numWritten != im.rows * im2.cols) { // failed to write to output file properly
          free_image(&im);
          free_image(&im2);
          free_image(&blendImage);
          fclose(infile);
         // printf("Unable to open %s for writing in blend\n", argv[4]);
          return RC_WRITE_FAILED;
        }
      } else if (im.rows <= im2.rows && im.cols >= im2.cols) {
        if (numWritten != im2.rows * im.cols) { // failed to write to output file properly
          free_image(&im);
          free_image(&im2);
          free_image(&blendImage);
          fclose(infile);
         // printf("Unable to open %s for writing in blend\n", argv[4]);
          return RC_WRITE_FAILED;
        }
      }


      fclose(blendFile);
      fclose(secondFile);
      free_image(&im2);
      free_image(&blendImage);

    }

    else if (strcmp(operation, "pointilism") == 0 || strcmp(argv[argc-1], "pointilism") == 0) {
      if (argc != 4) {
       // printf("incorrect number of arguments for %s\n", operation);
        fclose(infile);
        return RC_INVALID_OP_ARGS;
      }

      im = read_ppm(infile);
      if (im.cols == 0 && im.rows == 0 && im.data == NULL) {
       // printf("Invalid ppm\n");
        free_image(&im);
        fclose(infile);
        return RC_INVALID_PPM; 
      }

      FILE* pointilismFile = fopen(argv[2], "wb");
      if (!pointilismFile) {
        free_image(&im);
        fclose(infile);
        return RC_WRITE_FAILED;
      }

      Image pointilismImage;

      pointilismImage = pointilism(im, 1);
      int numWritten = write_ppm(pointilismFile, pointilismImage);
      if (numWritten != im.rows * im.cols) { // failed to write to output file properly
        free_image(&im);
        free_image(&pointilismImage);
        fclose(infile);
        return RC_WRITE_FAILED;
      }
      fclose(pointilismFile);
      free_image(&pointilismImage);
    }

    else if (strcmp(operation, "blur") == 0 || strcmp(argv[argc-2], "blur") == 0) {
      if (argc != 5) {
       // printf("incorrect number of arguments for %s; error code is %d\n", operation, RC_INVALID_OP_ARGS);
        fclose(infile);
        return RC_INVALID_OP_ARGS;
      }

      im = read_ppm(infile);
      if (im.cols == 0 && im.rows == 0 && im.data == NULL) {
        //printf("Invalid ppm\n");
        free_image(&im);
        fclose(infile);
        return RC_INVALID_PPM; 
      }

      if (!isRealNumber(argv[4])) {
        //printf("Invalid arguments for %s\n", operation);
        //printf("%s\n", argv[4]);
        free_image(&im);
        fclose(infile);
        return RC_OP_ARGS_RANGE_ERR;
      }

      double sigma = atof(argv[4]);

  

      FILE* blurFile = fopen(argv[2], "wb");
      if (!blurFile) {
        free_image(&im);
        fclose(infile);
        return RC_WRITE_FAILED;
      }

      Image blurImage;

      blurImage = blur(im, sigma);
      int numWritten = write_ppm(blurFile, blurImage);
      if (numWritten != im.rows * im.cols) { // failed to write to output file properly
        free_image(&im);
        free_image(&blurImage);
        fclose(infile);
        return RC_WRITE_FAILED;
      }
      fclose(blurFile);
      free_image(&blurImage);
    } 
    
    else {
     //printf("Unspecified error condition");
        free_image(&im);
        fclose(infile);
      return RC_UNSPECIFIED_ERR;
    }

    free_image(&im);
    fclose(infile);
    //fclose(original);

    return RC_SUCCESS;

}


