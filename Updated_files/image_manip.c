#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "image_manip.h"
#include "ppm_io.h"

Image grayscale (const Image in){
	double gray = 0.0;
	Image gray_image = make_image(in.rows, in.cols);

	for(int i = 0; i < (in.rows*in.cols); i++){
		gray = (0.3 * in.data[i].r) + (0.59 * in.data[i].g) + (0.11*in.data[i].b);
	 	gray_image.data[i].r = (unsigned char) gray;
		gray_image.data[i].g = (unsigned char) gray;
	 	gray_image.data[i].b = (unsigned char) gray;
  	}
  
  	return gray_image;
}
Image rotate_ccw( const Image in ){
   Image rotate = make_image(in.cols,in.rows);
   rotate.rows = in.cols;
   rotate.cols = in.rows;  
 
   int k = 0;
   int next_col = in.cols-1;
   for (int i = 0; i < rotate.rows; i++) {
       next_col = in.cols - (i+1);
	   for (int j = 0; j < rotate.cols; j++) {
	   		rotate.data[k] = in.data[next_col];
			k++;
			next_col += in.cols;
	   }
   }
   return rotate;

}

Image blend (const Image in1, const Image in2 ,double alpha) {
	// Find max image dimenstions
	int max_col = (in1.cols>=in2.cols) ? in1.cols : in2.cols;
	int max_row = (in1.rows>=in2.rows) ? in1.rows : in2.rows;
	int min_row = (in1.rows<in2.rows) ? in1.rows : in2.rows;
	int min_col = (in1.cols<in2.cols) ? in1.cols : in2.cols;
	Image blend = make_image(max_row,max_col);

	// Considering all cases
	int IMAGE_ONE_MORE_ROWS = (in1.rows>in2.rows);
	int IMAGE_TWO_MORE_ROWS = (in1.rows<in2.rows);
	int IMAGE_ONE_MORE_COLS = (in1.cols>in2.cols);
	int IMAGE_TWO_MORE_COLS = (in1.cols<in2.cols);
	int IMAGE_EQUAL_ROWS = (in1.rows == in2.rows);
	int IMAGE_EQUAL_COLS = (in1.cols == in2.cols);
	

	int next_col = 0;
	int next_row = 0;
	int k = 0;
	double blended_rpix_in1 = 0;
	double blended_rpix_in2 = 0;
	
	double blended_gpix_in1 = 0;
	double blended_gpix_in2 = 0;
	
	double blended_bpix_in1 = 0;
	double blended_bpix_in2 = 0;

	//--------------------------------------------------//
	// Image 1 has more rows but image 2 has more cols
	if(IMAGE_ONE_MORE_ROWS && IMAGE_TWO_MORE_COLS){
		// Fill extended col portion
		printf("Should not be here also");
		for(int i = 0; i<min_row;i++){
			for(int j = min_col; j<max_col; j++){
				blend.data[j+next_col] = in2.data[j+next_col];
			}
			next_col += max_col;
		}
		next_col = min_row * (min_col-1);
		// Fill the extended row portion
		for(int i = min_row; i<max_row; i++){
			for(int j = 0; j<min_col ; j++){
				blend.data[next_col+j] = in1.data[j+next_col];
			}
			next_col += min_col;
		}
		// Blend the images
		for(int i = 0; i<min_row; i++){
			for(int j = 0; j<min_col; j++){
				blended_rpix_in1 = in1.data[k].r * alpha;
				blended_gpix_in1 = in1.data[k].g * alpha;
				blended_bpix_in1 = in1.data[k].b * alpha;
				blended_rpix_in2 = in2.data[j+next_row].r * (1-alpha);
				blended_gpix_in2 = in2.data[j+next_row].g * (1-alpha);
				blended_bpix_in2 = in2.data[j+next_row].b * (1-alpha);
				blend.data[j+next_row].r =  blended_rpix_in1 + blended_rpix_in2;
				blend.data[j+next_row].g =  blended_gpix_in1 + blended_gpix_in2;
				blend.data[j+next_row].b =  blended_bpix_in1 + blended_bpix_in2;
				k++;
			}
			next_row += max_col;
		}
	}
	//--------------------------------------------------//
	
	// Image 1 has more cols but image tow has more rows
	// Bottom corner of the image will be black
	if(IMAGE_ONE_MORE_COLS && IMAGE_TWO_MORE_ROWS){
		// Fill extended col portion
		for(int i = 0; i<min_row;i++){
			for(int j = min_col; j<max_col; j++){
				blend.data[j+next_col] = in1.data[j+next_col];
			}
			next_col += max_col;
		}
		next_col = min_row * (min_col-1);
		// Fill the extended row portion
		for(int i = min_row; i<max_row; i++){
			for(int j = 0; j<min_col ; j++){
				blend.data[next_col+j] = in2.data[j+next_col];
			}
			next_col += min_col;
		}
		// Blend the images	
		for(int i = 0; i<min_row; i++){
			for(int j = 0; j<min_col; j++){
				blended_rpix_in1 = in1.data[k].r * alpha;
				blended_gpix_in1 = in1.data[k].g * alpha;
				blended_bpix_in1 = in1.data[k].b * alpha;
				blended_rpix_in2 = in2.data[j+next_row].r * (1-alpha);
				blended_gpix_in2 = in2.data[j+next_row].g * (1-alpha);
				blended_bpix_in2 = in2.data[j+next_row].b * (1-alpha);
				blend.data[j+next_row].r = blended_rpix_in1 + blended_rpix_in2;
				blend.data[j+next_row].g =  blended_gpix_in1 + blended_gpix_in2;
				blend.data[j+next_row].b = blended_bpix_in1 + blended_bpix_in2;
				k++;
			}
			next_row += max_col;
		}
	}
	//--------------------------------------------------//
	// Image 1 is larger
	if( (IMAGE_ONE_MORE_COLS && IMAGE_ONE_MORE_ROWS) || (IMAGE_ONE_MORE_ROWS && IMAGE_EQUAL_COLS) || (IMAGE_ONE_MORE_COLS && IMAGE_EQUAL_ROWS) ){
		for(int i = 0; i<max_row; i++){
			for(int j = 0; j<max_col; j++){
				blend.data[k] = in1.data[k];
				k++;
			}
		}	
		k = 0;
		for(int i = 0; i<min_row; i++){
			for(int j = 0; j<min_col; j++){
				blended_rpix_in2 = in2.data[k].r * alpha;
				blended_gpix_in2 = in2.data[k].g * alpha;
				blended_bpix_in2 = in2.data[k].b * alpha;
				blended_rpix_in1 = in1.data[j+next_row].r * (1-alpha);
				blended_gpix_in1 = in1.data[j+next_row].g * (1-alpha);
				blended_bpix_in1 = in1.data[j+next_row].b * (1-alpha);
				blend.data[j+next_row].r = blended_rpix_in1 + blended_rpix_in2;
				blend.data[j+next_row].g = blended_gpix_in1 + blended_gpix_in2;
				blend.data[j+next_row].b = blended_bpix_in1 + blended_bpix_in2;
				k++;
			}
			next_row += max_col;
		}
	}
	//--------------------------------------------------//
	// Image 2 is larger
	if((IMAGE_TWO_MORE_ROWS && IMAGE_TWO_MORE_COLS) || (IMAGE_TWO_MORE_ROWS && IMAGE_EQUAL_COLS) || (IMAGE_TWO_MORE_COLS && IMAGE_EQUAL_ROWS)){
		for(int i = 0; i<max_row; i++){
			for(int j = 0; j<max_col; j++){
				blend.data[k] = in2.data[k];
				k++;
			}
		}	
		k = 0;
		for(int i = 0; i<min_row; i++){
			for(int j = 0; j<min_col; j++){
				blended_rpix_in1 = in1.data[k].r * alpha;
				blended_gpix_in1 = in1.data[k].g * alpha;
				blended_bpix_in1 = in1.data[k].b * alpha;
				blended_rpix_in2 = in2.data[j+next_row].r * (1-alpha);
				blended_gpix_in2 = in2.data[j+next_row].g * (1-alpha);
				blended_bpix_in2 = in2.data[j+next_row].b * (1-alpha);
				blend.data[j+next_row].r = blended_rpix_in1 + blended_rpix_in2;
				blend.data[j+next_row].g = blended_gpix_in1 + blended_gpix_in2;
				blend.data[j+next_row].b = blended_bpix_in1 + blended_bpix_in2;
				k++;
			}
			next_row += max_col;
		}
	}
	
	//--------------------------------------------------//
	// If image sizes are equal
	if(IMAGE_EQUAL_ROWS && IMAGE_EQUAL_COLS){
		for(int i = 0; i<max_row; i++){
			for(int j = 0; j<max_col; j++){
				blended_rpix_in1 = in1.data[k].r * alpha;
				blended_gpix_in1 = in1.data[k].g * alpha;
				blended_bpix_in1 = in1.data[k].b * alpha;
				blended_rpix_in2 = in2.data[k].r * (1-alpha);
				blended_gpix_in2 = in2.data[k].g * (1-alpha);
				blended_bpix_in2 = in2.data[k].b * (1-alpha);
				blend.data[k].r = blended_rpix_in1 + blended_rpix_in2;
				blend.data[k].g = blended_gpix_in1 + blended_gpix_in2;
				blend.data[k].b = blended_bpix_in1 + blended_bpix_in2;
				k++;
			}
		}
	}		
	//--------------------------------------------------//
	return blend;

}

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

   // ----- Creating + shaped filter -----//
   int overflow = 0;
   for(int i=0; i<num_pixels; i++){
       for(int j = 0; j<random_radius[i]; j++){
           // if it goes beyond the top img boundary assing an idx of 0 to skip it
           indices_top[i][j] = (rand_pixels[i]-(cols*(j+1)) < 0) ? 0 : rand_pixels[i] - (cols*(j+1));
           indices_bot[i][j] = (rand_pixels[i]+(cols*(j+1)) > rows*cols) ? 0 : rand_pixels[i] + (cols*(j+1));
           indices_left[i][j] = (overflow) ? 0 : rand_pixels[i] - (j+1);
		   indices_right[i][j] = ((rand_pixels[i]+(j+1)) % (cols) == 0) ? 0 : rand_pixels[i] + (j+1);
		   overflow = ((rand_pixels[i]-j)%cols ==0) ? 1 : 0;
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
/*
Image blur( const Image in , double sigma ){

	// Making the conv kernel
	// Find matrix size
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
  double normalize = 0.0
  double pix_r = 0.0;
  double pix_g = 0,0;
  double pix_b = 0.0;
  // Do conv
  for(int i = 0; i<in.rows*in.cols; i++){
  	
  }

}

*/


Image saturate( const Image in , double scale ){
	
	Image saturate = make_image(in.rows,in.cols);
	Image gray_image = grayscale(in);
	
	double r_pix = 0;
	double g_pix = 0;
	double b_pix = 0;

	for(int i = 0 ; i<in.rows*in.cols; i++){
		r_pix = ((in.data[i].r - gray_image.data[i].r)*scale) + gray_image.data[i].r;
		g_pix = ((in.data[i].g - gray_image.data[i].g)*scale) + gray_image.data[i].g;
		b_pix = ((in.data[i].b - gray_image.data[i].b)*scale) + gray_image.data[i].b;	
		r_pix = (r_pix>255) ? 255: r_pix;
		g_pix = (g_pix>255) ? 255: g_pix;
		b_pix = (b_pix>255) ? 255: b_pix;
		
		saturate.data[i].r = (r_pix<0) ? 0: r_pix;
		saturate.data[i].g = (g_pix<0) ? 0: g_pix;
		saturate.data[i].b = (b_pix<0) ? 0: b_pix;
	}

	free(gray_image.data);
	return saturate;
}

