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
/*
Image blend (const Image in1, const Image in2 ,double alpha) {
	// Find max image dimenstions
	int max_cols = (in1.cols>in2.cols) ? in1.cols : in2.cols;
	int max_rows = (in1.rows>in2.rows) ? in1.rows : in2.rows;
	int min_rows = (in1.rows<in2.rows) ? in1.rows : in2.rows;
	int min_cols = (in1.cols<in2.cols) ? in1.cols : in2.cols;

	Image blend = make_image(max_rows,max_cols);
	Image mod_in1 = make_image(in1.rows,in1.cols);
	Image mod_in2 = make_image(in2.rows,in2.cols);

	// Assign blend to the largest image dimensions
	int k = 0;
	for(int i = 0; i<max_rows; i++){
		for(int j = 0; j<max_cols; j++){
			if(in1.cols > in2.cols){
				blend.data[k] = in1.data[j];
				k++;
			}
			else {
				blend.data[k] = in2.data[j];
				k++;
			}
		}
	}
	
	// Apply alpha on in1
	for(int i = 0; i<min_rows; i++){
		for(int j = 0; j<min_cols; j++){
			mod_in1.data[i].r = in1.data[i].r * alpha;
			mod_in1.data[i].g = in1.data[i].g * alpha;
			mod_in1.data[i].b = in1.data[i].b * alpha;
		}
	}
}
*/

Image pointilism( const Image in , unsigned int seed ) {
   srand(seed);
   //int radius = (rand() % 5) + 1; // Generate a random number between 1 and 5
   int rows = in.rows;
   int cols = in.cols;
   // Now to generate random integers which will be the pixel locations
   int num_pixels = (rows*cols) * 0.03;
   int rand_pixels [num_pixels];

   // Generate random origin pixel locations from the image
   int random_row_coord;
   int random_col_coord;
   int random_radius[num_pixels];
   for(int i = 0; i<num_pixels; i++){
	   random_row_coord = rand() % rows;
	   random_col_coord = rand() % cols;
	   rand_pixels[i] = (random_row_coord*cols) + random_col_coord;
	   random_radius[i] = (rand() % 5) + 1;

   }


   Image pointilism = make_image (rows,cols);
   
   //-------------------------------------------------------------------------------------------------//
   int indices_top[num_pixels][5];
   int indices_bot[num_pixels][5];
   int indices_left[num_pixels][5];
   int indices_right[num_pixels][5];

	for(int i = 0; i<num_pixels; i++){
		for(int j = 0; j<5 ; j++){
			indices_top[i][j] = 0;
			indices_bot[i][j] = 0;
			indices_left[i][j] = 0;
			indices_right[i][j] = 0;
		}
	}


   // ----- TOP -----
   for(int i=0; i<num_pixels; i++){
       for(int j = 0; j<random_radius[i]; j++){
           // if it goes beyond the top img boundary assing an idx of 0 to skip it
           indices_top[i][j] = (rand_pixels[i]-(cols*(j+1)) < 0) ? 0 : rand_pixels[i] - (cols*(j+1));
       }
   }
   // ----- BOTTOM -----
   for(int i=0; i<num_pixels; i++){
       for(int j = 0; j<random_radius[i]; j++){
           // if it goes beyond the top img boundary assing an idx of -0 to skip it
           indices_bot[i][j] = (rand_pixels[i]+(cols*(j+1)) > rows*cols) ? 0 : rand_pixels[i] + (cols*(j+1));
       }
   }
   // ----- LEFT -----
   int overflow = 0;
   for(int i=0; i<num_pixels; i++){
   	overflow = (rand_pixels[i]%cols == 0) ? 1 : 0;
       for(int j = 0; j<random_radius[i]; j++){
           if(overflow) {
				indices_left[i][j] = 0;
		   }
		   else {
				indices_left[i][j] = rand_pixels[i] - (j+1);
				overflow = ((rand_pixels[i]-j)%cols ==0) ? 1 : 0;
		   }
       } 
   }
   // ----- RIGHT -----
   for(int i=0; i<num_pixels; i++){
       for(int j = 0; j<random_radius[i]; j++){
           if((rand_pixels[i]+(j+1)) % (cols) == 0) {
		   		indices_right[i][j] = 0;
		   }
		   else {
				indices_right[i][j] = rand_pixels[i] + (j+1);
		   }
       } 
   }	
   //-------------------------------------------------------------------------------------------------//
	// Declaring pixels to form the cirlular filter
	int top_right[num_pixels][5][5];
	int top_left[num_pixels][5][5];
	int bot_right[num_pixels][5][5];
	int bot_left[num_pixels][5][5];

	for(int i = 0; i<num_pixels; i++){
		for(int j = 0; j<5; j++){
			for(int k = 0; k<5; k++){
				top_right[i][j][k] = 0;
				top_left[i][j][k] = 0;
				bot_right[i][j][k] = 0;
				bot_left[i][j][k] = 0;
			}
		}
	}	
   	//-------------------------------------------------------------------------------------------------//
	//--Bottom Right--//
	int X = 0;
	int Y = 0;
	int point;
	int loop_var = 0;
	for (int i=0;i<num_pixels; i++){
		X = 0;
		Y = random_radius[i];
		loop_var = 0;
		while (X<Y){
			if(loop_var == 0){
				point = 3-(2*random_radius[i]);
				loop_var++;
				}
			if(point <0) {
				point = point + 4*X + 6;
				X = X+1;
			}
			else {
				point = point + 4*(X-Y)+ 10;
				X = X+1;
				Y = Y-1;
			}
			for(int j =0; j<Y; j++){
				if(indices_right[i][X-1] !=0){
					if(indices_bot[i][Y-1] !=0){
						bot_right[i][X-1][j] = indices_right[i][X-1]+(cols*(j+1));
					}
					else {
						bot_right[i][X-1][j] = 0;
					}
				}
				else {
					bot_right[i][X-1][j] = 0;
				}
			}
		}	
	}
   	//-------------------------------------------------------------------------------------------------//
	//--Bottom Left--//
	loop_var = 0;
	for (int i=0;i<num_pixels; i++){
		X = 0;
		Y = random_radius[i];
		loop_var = 0;
		while (X<Y){
			if(loop_var == 0){
				point = 3-(2*random_radius[i]);
				loop_var++;
				}
			if(point <0) {
				point = point + 4*X + 6;
				X = X+1;
			}
			else {
				point = point + 4*(X-Y)+ 10;
				X = X+1;
				Y = Y-1;
			}
			for(int j =0; j<Y; j++){
				if(indices_left[i][X-1] !=0){
					if(indices_bot[i][Y-1] !=0){
						bot_left[i][X-1][j] = indices_left[i][X-1]+(cols*(j+1));
					}
					else {
						bot_left[i][X-1][j] = 0;
					}
				}
				else {
					bot_left[i][X-1][j] = 0;
				}
			
			}
		}	
	}
	//-------------------------------------------------------------------------------------------------//
	//--Top Right--//
	loop_var = 0;
	for (int i=0;i<num_pixels; i++){
		X = 0;
		Y = random_radius[i];
		loop_var = 0;
		while (X<Y){
			if(loop_var == 0){
				point = 3-(2*random_radius[i]);
				loop_var++;
				}
			if(point <0) {
				point = point + 4*X + 6;
				X = X+1;
			}
			else {
				point = point + 4*(X-Y)+ 10;
				X = X+1;
				Y = Y-1;
			}
			for(int j =0; j<Y; j++){
				if(indices_right[i][X-1] !=0){
					if(indices_top[i][Y-1] !=0){
						top_right[i][X-1][j] = indices_right[i][X-1]-(cols*(j+1));
					}
					else {
						top_right[i][X-1][j] = 0;
					}
				}
				else {
					top_right[i][X-1][j] = 0;
				}
			
			}
		}	
	}
	//-------------------------------------------------------------------------------------------------//
	//--Top left--//
	loop_var = 0;
	for (int i=0;i<num_pixels; i++){
		X = 0;
		Y = random_radius[i];
		loop_var = 0;
		while (X<Y){
			if(loop_var == 0){
				point = 3-(2*random_radius[i]);
				loop_var++;
				}
			if(point <0) {
				point = point + 4*X + 6;
				X = X+1;
			}
			else {
				point = point + 4*(X-Y)+ 10;
				X = X+1;
				Y = Y-1;
			}
			for(int j =0; j<Y; j++){
				if(indices_left[i][X-1] !=0){
					if(indices_top[i][Y-1] !=0){
						top_left[i][X-1][j] = indices_left[i][X-1]-(cols*(j+1));
					}
					else {
						top_left[i][X-1][j] = 0;
					}
				}
				else {
					top_left[i][X-1][j] = 0;
				}
			
			}
		}	
	}
   //-------------------------------------------------------------------------------------------------//
   //----------- Applying the circular filters ---------------//
	for(int i = 0; i<num_pixels; i++){
		Pixel pix = in.data[rand_pixels[i]];	
		pointilism.data[rand_pixels[i]] = pix;
		for(int j = 0; j<5; j++){
			pointilism.data[indices_top[i][j]] = (indices_top[i][j] == 0) ? pointilism.data[indices_top[i][j]] : pix;
			pointilism.data[indices_bot[i][j]] = (indices_bot[i][j] == 0) ? pointilism.data[indices_bot[i][j]] : pix;
			pointilism.data[indices_left[i][j]] = (indices_left[i][j] == 0) ? pointilism.data[indices_left[i][j]] : pix;
			pointilism.data[indices_right[i][j]] = (indices_right[i][j] == 0) ? pointilism.data[indices_right[i][j]] : pix;
			for(int k = 0; k<5; k++){
				pointilism.data[top_right[i][j][k]] =  (top_right[i][j][k] == 0)? pointilism.data[top_right[i][j][k]] : pix;
				pointilism.data[top_left[i][j][k]]  =  (top_left[i][j][k]  == 0)? pointilism.data[top_left[i][j][k]]  : pix;
				pointilism.data[bot_right[i][j][k]] =  (bot_right[i][j][k] == 0)? pointilism.data[bot_right[i][j][k]] : pix;
				pointilism.data[bot_left[i][j][k]] =   (bot_left[i][j][k]  == 0)? pointilism.data[bot_left[i][j][k]] :  pix;
			}
		}
	}
   //-------------------------------------------------------------------------------------------------//
	return pointilism;
}

Image blur( const Image in , double sigma ){

	// Conv kernel
	// Find matrix size
	FILE *test = fopen("test.txt","w");
	double sigma_sq = sigma * sigma;
	int N = sigma*10;
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
	// Need to use double values for the pixel before demoting it to char
	double pixel_r = 0.0;
    double normalize = 0.0;
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
						// Skip 0 pixels in the zero padded image
						normalize = (gauss_blur.data[x+x_offset+loop_itr].r==0) ? normalize : normalize+kernel[y][x];
					}
				}
		// Take average only when a single conv is over
		convolved.data[next_pixel].r = (unsigned char)(pixel_r / normalize);
		convolved.data[next_pixel].g = (unsigned char)(pixel_g / normalize);
		convolved.data[next_pixel].b = (unsigned char)(pixel_b / normalize);
		normalize = 0.0;
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

