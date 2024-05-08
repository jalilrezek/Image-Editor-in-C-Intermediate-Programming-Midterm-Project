#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "image_manip.h"
#include "ppm_io.h"


////////////////////////////////////////
// Definitions of the functions       //
// declared in image_manip.h go here! //
////////////////////////////////////////

Image grayscale (const Image in){
  float gray = 0.0;
  Image gray_image = make_image(in.rows, in.cols);

  for(int i = 0; i < (in.rows*in.cols); i++){
	 gray = 0.3 * in.data[i].r + 0.59 * in.data[i].g + 0.11 * in.data[i].b;
	 gray_image.data[i].r = (char)gray;
	 gray_image.data[i].g = (char)gray;
	 gray_image.data[i].b = (char)gray;
  }
  
  return gray_image;
}


Image blur( const Image in , double sigma ){

  // Conv kernel
  // Find matrix size
  FILE *test = fopen("test.txt","w");
  double sigma_sq = sigma * sigma;
  double PI = 3.1415926535;
  int N = sigma*10;
  //double normalize = 0.0;
  // N should always be odd
  if(N % 2 == 0){
      N++;
  }
  int position = -(N/2); 
  int dx = position;
  int dy = position;
  double kernel[N][N];
  for(int i =0; i<N; i++){
      for(int j=0; j<N ; j++){
          kernel [i][j] = (1.0 / (2.0 * PI *(sigma_sq))) * exp(-((dx*dx)+(dy*dy))/(2 * (sigma_sq)));
          normalize += kernel[i][j];
          dx++;
      }
      dy++;
      dx = position;
  }
	


    // Perform convolutions
	// Create a new image with padded zeros
	Image gauss_blur = make_image_zero_padd(in.rows,in.cols,N);
	int new_col_size = in.cols + (N-1);// N/2 on each side
	int new_row_size = in.rows + (N-1);
	// Now copy the image im into the gauss blur
	int k = new_col_size * (N/2); // skip row
	k = k + (N/2); // To skip the zero padd in the columns
	int j = 0;
	int row_cnt = N/2;
	for (int l = 0; l<in.rows; l++){
	for (int i = 0; i<in.cols; i++){
		gauss_blur.data[i+k].r =  in.data[j].r;
		gauss_blur.data[i+k].b =  in.data[j].b;
		gauss_blur.data[i+k].g =  in.data[j].g;
		j++;
		}
		k = new_col_size*row_cnt + (N/2);
		row_cnt++;
	}
   	
	// Convolution
	double normalize_r = 0.0;
	double normalize_g = 0.0;
	double normalize_b = 0.0;
	double pixel_r = 0.0;
	double pixel_g = 0.0;
	double pixel_b = 0.0;
	int loop_itr = 0;
	int x_offset = 0; // To move the sliding window right by 1 after completing every conv
	int y_offset = 0; // to move the sliding window by 1 after completing every row
	int next_pixel = 0; // To go over each pixel in the image
	Image convolved = make_image(in.rows,in.cols);
	for(int b = 0; b<in.rows; b++){ // Go over all rows
	for(int i = 0; i<in.cols; i++){	// Go over all elements in a row
		for(int y = 0; y<N ; y++){ //To loop over the 2D kernel
			loop_itr = (new_col_size) * (y+y_offset);
			for(int x = 0; x<N; x++) {
					pixel_r += gauss_blur.data[x+x_offset+loop_itr].r * kernel[y][x];
					pixel_g += gauss_blur.data[x+x_offset+loop_itr].g * kernel[y][x];
					pixel_b += gauss_blur.data[x+x_offset+loop_itr].b * kernel[y][x];
					loop_itr = (new_col_size)*(y+y_offset);
					normalize_r = (gauss_blur.data[x+x_offset+loop_itr].r == 0)? normalize_r : normalize_r+kernel[x][y];
					normalize_g = (gauss_blur.data[x+x_offset+loop_itr].g == 0)? normalize_g : normalize_g+kernel[x][y];
					normalize_b = (gauss_blur.data[x+x_offset+loop_itr].b == 0)? normalize_b : normalize_b+kernel[x][y];
				}
			}
		// Take average only when a single conv is over
		fprintf(test,"\n");
		convolved.data[next_pixel].r = (unsigned char)(pixel_r / normalize_r);
		convolved.data[next_pixel].g = (unsigned char)(pixel_g / normalize_g);
		convolved.data[next_pixel].b = (unsigned char)(pixel_b / normalize_b);
		normalize_r = 0.0;
		normalize_g = 0.0;
		normalize_b = 0.0;
		loop_itr = new_col_size * y_offset; // reset the row number to again iterate over y
		x_offset++; // after completing each pixel in a row
		next_pixel++; // move to the next pixel in the convolved image
		pixel_r = 0.0;
		pixel_g = 0.0;
		pixel_b = 0.0;
		}
		x_offset = 0;
		y_offset++;
		loop_itr = (new_col_size*y_offset);
	}
	

fclose(test);
free(gauss_blur.data);
return convolved;

}

