#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
   
   /* struct to store a point */
   typedef struct {
     int x;
     int y;
  } Point;
  
  /* struct to store an RGB pixel, one byte per channel */
  typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
  } Pixel;
  
  /* struct to store an entire image
   * pixels are linearized in row-major order, with the first block of pixels corresponding to the first row, then the second, e    tc.
   */
  typedef struct {
    Pixel *data;
    int rows;
    int cols;
  } Image;

int write_ppm( FILE *fp , const Image im ) {
  // Complete this function
   fprintf(fp,"P6\n%d %d\n255\n",im.cols,im.rows);
  // Replace the next two lines
  size_t size = (size_t) im.rows * im.cols;
  fwrite(im.data,sizeof(Pixel),size,fp);
  return 0;
 }
void free_image( Image *im ) {
 	 
	free(im->data);
 } 

 /* allocate a new image of the specified size;
  * doesn't initialize pixel values */
 Image make_image( int rows , int cols ) {
   	// Complete this function
 	Image im = { NULL , rows , cols };
 	im.data = malloc(sizeof(Pixel)*rows*cols*sizeof(unsigned char));
	if(!(im.data)){
		fprintf(stderr, "Unable to allocate memory");	
	}
   return im;
 }   
 

 /* helper function for read_ppm, takes a filehandle
   * and reads a number, but detects and skips comment lines
   */
  int read_num( FILE *fp ) {
    assert(fp);
  
    int ch;
    while((ch = fgetc(fp)) == '#') { // # marks a comment line
      while( ((ch = fgetc(fp)) != '\n') && ch != EOF ) {
        /* discard characters til end of line */
      }
    }
    ungetc(ch, fp); // put back the last thing we found
  
    int val;
    if (fscanf(fp, "%d", &val) == 1) { // try to get an int
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
  
  Image read_ppm( FILE *fp ) {
    Image im = { NULL , 0 , 0 };
  
    /* confirm that we received a good file handle */
    if( !fp ){
      fprintf( stderr , "Error:ppm_io - bad file pointer\n" );
      return im;
    }
  
    int rows=-1 , cols=-1;
  
    /* read in tag; fail if not P6 */
    char tag[20];
    tag[19] = '\0';
    // Why 19 characters to read just P6?
	fscanf( fp , "%19s\n" , tag );
	// Compare 20 first 20 characters, but it will terminate before that
    if( strncmp( tag , "P6" , 20 ) ) {
      fprintf( stderr , "Error:ppm_io - not a PPM (bad tag)\n" );
      return im;
    }
  
  
    /* read image dimensions */
  
    //read in columns
    cols = read_num( fp ); // NOTE: cols, then rows (i.e. X size followed by Y size)
    //read in rows
    rows = read_num( fp );
  
    //read in colors; fail if not 255  
	int colors = read_num( fp );
    if( colors!=255 ){
      fprintf( stderr , "Error:ppm_io - PPM file with colors different from 255\n" );
      return im;
    }
  
    //confirm that dimensions are positive
    if( cols<=0 || rows<=0 ){
      fprintf( stderr , "Error:ppm_io - PPM file with non-positive dimensions\n" );
      return im;
    }
  
    /* Allocate the new image */
    im = make_image( rows , cols );
    if( !im.data ){
      fprintf( stderr , "Error:ppm_io - Could not allocate new image\n" );
      return im;
    }
    /* finally, read in Pixels */
  
    /* read in the binary Pixel data */
    if( fread( im.data , sizeof(Pixel) , im.rows * im.cols , fp ) != (size_t)(im.rows * im.cols) ) {
      fprintf(stderr, "Error:ppm_io - failed to read data from file!\n");
      free_image( &im );
      return im;
    }
  
    //return the image struct pointer
    return im;
  }


int main () {

FILE *fp = fopen("kitten.ppm","rb");
Image im = read_ppm(fp);

FILE *out = fopen("kitten_cpy.ppm","wb");
write_ppm(out,im);

printf("First Pixel: R:%d, G:%d, B:%d\n",im.data->r,im.data->g,im.data->b);
printf("First Pixel: R:%d, G:%d, B:%d\n", im.data[0].r,im.data[1].r,im.data[2].r);
free(im.data);
fclose(fp);
fclose(out);
return 0;

}


