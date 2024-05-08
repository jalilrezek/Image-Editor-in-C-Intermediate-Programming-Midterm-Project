#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "image_manip.h"
#include "ppm_io.h"

Image grayscale(const Image in) {
    int rows = in.rows;
    int cols = in.cols;
    Pixel* pixels = in.data;
	Image new;
	new.rows = rows;
	new.cols = cols;
    new.data = malloc(sizeof(Pixel) * new.rows * new.cols); 


    int gray = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Pixel curpix = pixels[i * cols + j];
            int r = (int) curpix.r;
            int g = (int) curpix.g;
            int b = (int) curpix.b;
            gray = 0.3 * r + 0.59 * g + 0.11 * b; 
            new.data[i * cols + j].r = (unsigned char) gray; // doing it on curpix doesn't work. 
            new.data[i * cols + j].g = (unsigned char) gray;
            new.data[i * cols + j].b = (unsigned char) gray;
        }
    }
	return new;
}

Image rotate_ccw(const Image in) {
    // original[0][1] = new[numRows - 1][0]
    // original[i][j] = new[numRows - j][i]
    // new: numCols * (numRows - 1) - j * numCols + i
    // numCols * (numRows - 2)
    // new: numCols * (numRows - [origCol + 1]) + origRow
    // new.data[new.cols * (new.rows - (origCol + 1)) + origRow] = im->data[origRow * im->cols + origCol];

    Image new;
    new.rows = in.cols;
    new.cols = in.rows;
    new.data = malloc(sizeof(Pixel) * new.rows * new.cols); 

    //int newCol = 0;
    //int newRow = 0;

    for (int origRow = 0; origRow < in.rows; origRow++) {
        for (int origCol = 0; origCol < in.cols; origCol++) {
            new.data[new.cols * (new.rows - (origCol + 1)) + origRow] = in.data[origRow * in.cols + origCol];
        }
    }
    //free(im.data); --> it can be done in project.c
    //im.data = NULL;
    
    return new;
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

Image blur( const Image in , double sigma ){

	// Conv kernel
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
	
	// Perform convolutions
	// Create a new image with padded zeros
	Image gauss_blur = make_image_zero_padd(in.rows,in.cols,N);
	int new_col_size = in.cols + (N-1);// N/2 on each side
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
	free(gauss_blur.data);
	return convolved;

}


Image saturate(const Image im, double scale) { // with factor = 0, it acts as grayscale. Works with factor = 1 and 2
    int rows = im.rows;
    int cols = im.cols;
    Pixel* pixels = im.data;
    int gray = 0;
    float red_diff = 0;
    float green_diff = 0;
    float blue_diff = 0;
    float red_result = 0;
    float green_result = 0;
    float blue_result = 0;

    Image new;
    new.rows = im.rows;
    new.cols = im.cols;

    new.data = malloc(sizeof(Pixel) * new.rows * new.cols); 

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Pixel curpix = pixels[i * cols + j];
            int r = (int) curpix.r;
            int g = (int) curpix.g;
            int b = (int) curpix.b;
            gray = 0.3 * r + 0.59 * g + 0.11 * b; 

            red_diff = r - gray;
            green_diff = g - gray;
            blue_diff = b - gray;

            red_diff *= scale;
            green_diff *= scale;
            blue_diff *= scale;

            red_result = red_diff + gray;
            green_result = green_diff + gray;
            blue_result = blue_diff + gray;

            if (red_result > 255) { // if it doesn't work, remove the "else" and just have separate ifs.
                red_result = 255;
            } else if (red_result < 0) {
                red_result = 0;
            }
            if (green_result > 255) {
                green_result = 255;
            } else if (green_result < 0) {
                green_result = 0;
            }
            if (blue_result > 255) {
                blue_result = 255;
            } else if (blue_result < 0) {
                blue_result = 0;
            }


            new.data[i * cols + j].r = (unsigned char) red_result; // doing it on curpix doesn't work. 
            new.data[i * cols + j].g = (unsigned char) green_result;
            new.data[i * cols + j].b = (unsigned char) blue_result;
        }
    }
    //free(im->data);
    //im->data = NULL;

    return new;

}