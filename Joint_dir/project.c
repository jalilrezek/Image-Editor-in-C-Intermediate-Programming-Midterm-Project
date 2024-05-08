/* Questions/tasks: Check if file created for writing is null every time I do so? I.e. for every operation?
if fail to write to it or open it

How do I verify that user input is an integer? atoi() converts ascii to integer -- resolved using isInteger() function

For blend(), if they supply an invalid second file and also mess up another argument, for instance by giving a non-real number
argument, which error do I report?

What is "correct usage"? Consider not providing mandatory arguments vs giving incorrect number of args for specified operation.
I.e. missing an argument. Is the incorrect usage error code supposed to be tested first? Ex for 
"./project trees.ppm kitten.ppm blend" raise not providing mandatory arguments, or incorrect number of args exception?

Similarly, what if I do "./project trees.ppm fjidjfi blend 2.0" --> in this case, the third argument is invalid, and also
results in being unable to open the file. Should this be reported as file I/O error or an invalid argument error?

Can blur factor be a float/double or must be int? Ex can it be 3.5?

Consider:  ./project kitten.ppm who kittens_saturate_0.ppm saturate 0
The user tried calling saturate, which we know cuz it's the 2nd to last argument. But, it should be the fourth argumment.
So the argument is being read as "kittens_saturate_0.ppm
Is it then OK to report this as unsupported operation? Or should I infer that the user is trying to use saturate, and report it
as an invalid use of saturate?


*/

/* Notes: Within each "if argv[1] == operation" statement, that means that a valid operation was supplied, and we already checked
input file I/O and valid ppm by the time we get to the loops, so the only errors we could encounter are incorrect # args, and 
invalid arguments for specified operation.

*/


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

int execute_project(int argc, char* argv[]); // how else should I simplify main()?

int main (int argc, char* argv[]) {

  int result = execute_project(argc, argv);
  // Complete the implementation of main
  printf("Exit code: %d\n", result);
  return result; // Replace Me!
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

bool isRealNumber(const char *str) {
    bool hasDot = false;
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
}

bool fourArgCommands(char* operationName, char* arg3, char* lastArg) {
  return (strcmp(arg3, operationName) == 0) || (strcmp(lastArg, operationName) == 0);
}

bool fiveArgCommands(char* operationName, char* arg3, char* secondToLastArg) {
  return (strcmp(arg3, operationName) == 0) || (strcmp(secondToLastArg, operationName) == 0);
}

bool sixArgCommands(char* operationName, char* arg3, char* thirdToLastArg) {
  return (strcmp(arg3, operationName) == 0) || (strcmp(thirdToLastArg, operationName) == 0);
}

int execute_project(int argc, char* argv[]) {
    if (argc < 4) { // should argc > 6 be treated as "missing necessary arguments" or should it fall under code 8, unspecified?
        print_usage();
        return RC_MISSING_FILENAME;
    }
    char *filename = argv[1];
    char *operation = argv[3];

    // ./project fakefile.ppm trees_pointilism.ppm pointilism hi -- CA said seldom will have both wrong command inputs and wrong file
    // he said he doesn't think they'll do a lot of testing for 2 things invalid
    // if I want to test one as a condition of the other, I could do that, but he thinks it's OK to test file I/O first

    FILE* infile = fopen(filename, "rb");

    Image im;

    if (infile == NULL) { // read_ppm can take care of this, but if I leave it all up to read_ppm, then how would I 
        printf("Failed to open file\n"); // differentiate between null infile and a valid file but which is not a valid PPM?
        return RC_OPEN_FAILED; // so, I guess do this here, and then call read_ppm. If read_ppm then fails, it must be that
    } // it was an invalid PPM file
    // problem: read_ppm takes care of a lot of potential errors. not just invalid ppm, but failure to allocate memory for the
    // Image struct, failure to read in binary pixel data (maybe that's an invalid ppm error). If I group them all under
    //  error code 3, invalid ppm, is that OK?
    // TA said we can't change read_ppm, so if an error is coming from read_ppm, report it as file I/O error


    if (!fourArgCommands("grayscale", operation, argv[argc - 1]) && 
        !fourArgCommands("rotate-ccw", operation, argv[argc - 1]) &&
        !fourArgCommands("pointilism", "operation", argv[argc-1]) &&
        !fiveArgCommands("saturate", argv[3], argv[argc - 2]) &&
        !fiveArgCommands("blur", argv[3], argv[argc - 2]) &&
        !sixArgCommands("blend", operation, argv[argc - 2])) {

          printf("Unsupported image processing operation");
          return RC_INVALID_OPERATION;

    }


    /*if (strcmp(operation, "grayscale") != 0 && strcmp(operation, "rotate") != 0 &&
      strcmp(operation, "saturate") != 0 && strcmp(operation, "blend") != 0 && 
      strcmp(operation, "pointilism") != 0 && strcmp(operation, "blur") != 0) {
        printf("Unsupported image processing operation");
        return RC_INVALID_OPERATION;
    }*/


    else if (strcmp(operation, "grayscale") == 0 || strcmp(argv[argc-1], "grayscale") == 0) {
      if (argc != 4) {
        printf("incorrect number of arguments for %s\n", operation);
        return RC_INVALID_OP_ARGS;
      }

      im = read_ppm(infile);
      if (im.cols == 0 && im.rows == 0 && im.data == NULL) {
        printf("Invalid ppm\n");
        return RC_INVALID_PPM; 
      }

      FILE* gray = fopen(argv[2], "wb");
      im = grayscale(im);
      write_ppm(gray, im);
      fclose(gray);

    } 
    
    else if (strcmp(operation, "rotate-ccw") == 0 || strcmp(argv[argc-1], "rotate-ccw") == 0) {
      if (argc != 4) {
        printf("incorrect number of arguments for %s\n", operation);
        return RC_INVALID_OP_ARGS;
      }
      im = read_ppm(infile);
      if (im.cols == 0 && im.rows == 0 && im.data == NULL) {
        printf("Invalid ppm\n");
        return RC_INVALID_PPM; 
      }
      FILE* rotated = fopen(argv[2], "wb");
      im = rotate_ccw(im);
      write_ppm(rotated, im);
      fclose(rotated);
    } 
    
    else if (strcmp(operation, "saturate") == 0 || strcmp(argv[argc-2], "saturate") == 0) { // need to fix this.
      if (argc != 5) {
        printf("incorrect number of arguments for %s\n", operation);
        return RC_INVALID_OP_ARGS;
      }

      im = read_ppm(infile);
      if (im.cols == 0 && im.rows == 0 && im.data == NULL) {
        printf("Invalid ppm\n");
        return RC_INVALID_PPM; 
      }

      if(!isRealNumber(argv[4])) {
        printf("Invalid arguments for %s\n", operation);
        return RC_OP_ARGS_RANGE_ERR;
      }

      double scaleFactor = atof(argv[4]);

      FILE* saturateFile = fopen(argv[2], "wb");
      im = saturate(im, scaleFactor);
      write_ppm(saturateFile, im);
      fclose(saturateFile);
    } 
    
    else if (strcmp(operation, "blend") == 0 || strcmp(argv[argc-3], "blend") == 0) { // for blend, the operation name is argv[3] not argv[2]
      if (argc != 6) {
        printf("incorrect number of arguments for %s\n", operation);
        return RC_INVALID_OP_ARGS; 
      }

      im = read_ppm(infile);
      if (im.cols == 0 && im.rows == 0 && im.data == NULL) {
        printf("Invalid ppm\n");
        return RC_INVALID_PPM; 
      }

      if (!isRealNumber(argv[5])) {
        printf("Invalid arguments for %s\n", operation);
        return RC_OP_ARGS_RANGE_ERR;
      }

      double alpha = atof(argv[5]);

      char* file2 = argv[2];
      FILE* secondFile = fopen(file2, "r");

      if (secondFile == NULL) {
        printf("Unable to open second file for blend\n"); // should this instead be invalid argument error? Subjective?
        return RC_OPEN_FAILED;
      }

      Image result;

      Image im2 = read_ppm(secondFile);
      if (im.cols == 0 && im.rows == 0 && im.data == NULL) {
        return RC_INVALID_PPM; // read_ppm will already have printed out some error message to specify what's going on. But, see
        } // "problem" above. Are some of these errors not because of invalid ppm? If so, would it be wrong to group them under
        // invalid ppm error as is done here?

      FILE* blendFile = fopen(argv[4], "wb");
      result = blend(im, im2, alpha);
      write_ppm(blendFile, im);
      fclose(blendFile);
      free_image(&im2);
      free_image(&result);

    }

    else if (strcmp(operation, "pointilism") == 0 || strcmp(argv[argc-1], "pointilism") == 0) {
      if (argc != 4) {
        printf("incorrect number of arguments for %s\n", operation);
        return RC_INVALID_OP_ARGS;
      }

      im = read_ppm(infile);
      if (im.cols == 0 && im.rows == 0 && im.data == NULL) {
        printf("Invalid ppm\n");
        return RC_INVALID_PPM; 
      }

      FILE* pointilismFile = fopen(argv[2], "wb");
      im = pointilism(im, 1);
      write_ppm(pointilismFile, im);
      fclose(pointilismFile);
    }

    else if (strcmp(operation, "blur") == 0 || strcmp(argv[argc-2], "blur") == 0) {
      if (argc != 5) {
        printf("incorrect number of arguments for %s\n", operation);
        return RC_INVALID_OP_ARGS;
      }

      im = read_ppm(infile);
      if (im.cols == 0 && im.rows == 0 && im.data == NULL) {
        printf("Invalid ppm\n");
        return RC_INVALID_PPM; 
      }

      if (!isRealNumber(argv[4])) {
        printf("Invalid arguments for %s\n", operation);
        printf("%s\n", argv[4]);
        free_image(&im);
        return RC_OP_ARGS_RANGE_ERR;
      }

      double sigma = atof(argv[4]);

      Image result;

      FILE* blurFile = fopen(argv[2], "wb");
      result = blur(im, sigma);
      write_ppm(blurFile, im);
      fclose(blurFile);
      free_image(&result);
    } 
    
    else {
      printf("Unspecified error condition");
      return RC_UNSPECIFIED_ERR;
    }

    free_image(&im);
    fclose(infile);
    //fclose(original);

    return RC_SUCCESS;

}


