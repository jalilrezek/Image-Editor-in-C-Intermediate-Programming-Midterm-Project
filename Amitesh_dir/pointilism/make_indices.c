#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main () {

   // Select a random radius
   // Generate a random number between 0 and 4
   int radius = (rand() % 5) + 1; // Generate a random number between 1 and 5
   // Initialize rows and cols
   int rows = 400;
   int cols = 750;
   
   // Now to generate random integers which will be the pixel locations
   int num_pixels = (rows*cols) * 0.03;
   int rand_pixels [num_pixels];
   
   // Assign random pixel location from the image
   for(int i = 0; i<num_pixels; i++){
       rand_pixels[i] = rand()% (rows*cols);
   }
   
   // Create a list of indices you need from the random indices
   FILE *fp = fopen("test.txt","w");

   // ----- TOP -----
   int indices_top[num_pixels][radius];
   for(int i=0; i<num_pixels; i++){
       for(int j = 0; j<radius; j++){
           // if it goes beyond the top img boundary assing an idx of -1 to skip it
           indices_top[i][j] = (rand_pixels[i]-(cols*(j+1)) < 0) ? -1 : rand_pixels[i] - (cols*(j+1));
       }
   }

   // ----- BOTTOM -----
   int indices_bot[num_pixels][radius];
   for(int i=0; i<num_pixels; i++){
       for(int j = 0; j<radius; j++){
           // if it goes beyond the top img boundary assing an idx of -1 to skip it
           indices_bot[i][j] = (rand_pixels[i]+(cols*(j+1)) > rows*cols) ? -1 : rand_pixels[i] + (cols*(j+1));
       }
   }

   // ----- LEFT -----
   int indices_left[num_pixels][radius];
   int overflow = 0;
   for(int i=0; i<num_pixels; i++){
   	overflow = (rand_pixels[i]%cols == 0) ? 1 : 0;
       for(int j = 0; j<radius; j++){
           if(overflow) {
				indices_left[i][j] = -1;
		   }
		   else {
				indices_left[i][j] = rand_pixels[i] - (j+1);
				overflow = ((rand_pixels[i]-j)%cols ==0) ? 1 : 0;
		   }
       } 
   }

   // ----- RIGHT -----
   int indices_right[num_pixels][radius];
   for(int i=0; i<num_pixels; i++){
       for(int j = 0; j<radius; j++){
           if((rand_pixels[i]+(j+1)) % (cols) == 0) {
		   		indices_right[i][j] = -1;
		   }
		   else {
				indices_right[i][j] = rand_pixels[i] + (j+1);
		   }
       } 
   }

// Create an offset matrix for a given radius

int top_right[num_pixels][radius][radius];
//int top_left[num_pixels][radius][radius];
//int bot_right[num_pixels][radius][radius];
//int bot_left[num_pixels][radius][radius];

for(int i = 0; i<num_pixels; i++){
	for(int j = 0; j<radius; j++){
		for(int k = 0; k<radius; k++){
			top_right[i][j][k] = -1;
		}
	}
}

int X = 0;
int Y = radius;
int point;
int loop_var = 0;
for (int i=0;i<num_pixels; i++){
	X = 0;
	Y = radius;
	loop_var = 0;
	while (X<Y){
		if(loop_var == 0){
			point = 3-(2*radius);
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
			if(indices_right[i][X-1] !=-1){
				if(indices_top[i][Y-1] !=-1){
					top_right[i][X-1][j] = indices_right[i][X-1]+(cols*(j+1));
				}
				else {
					top_right[i][X-1][j] = -1;
				}
			}
			else {
				top_right[i][X-1][j] = -1;
			}
			
		}
	}	
}


for(int i = 0; i<num_pixels; i++){
	for(int j = 0; j<radius; j++){
		for(int k = 0; k<radius; k++){
			fprintf(fp,"top_right:%d ",top_right[i][j][k]);
		}
		fprintf(fp,"\n");
	}
	fprintf(fp,"\n");
}
fclose(fp);

}
