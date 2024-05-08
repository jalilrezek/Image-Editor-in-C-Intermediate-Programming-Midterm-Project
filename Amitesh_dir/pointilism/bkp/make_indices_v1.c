#include <stdio.h>
#include <stdlib.h>


int main () {

   // Select a random radius
   // Generate a random number between 0 and 4
 
   int radius = (rand() % 5) + 1; // Generate a random number between 1 and 5
   int rows = 400;
   int cols = 750;
   // Now to generate random integers which will be the pixel locations
   int num_pixels = (rows*cols) * 0.03;
   int rand_pixels [num_pixels];
   // Assign random pixel location from the image
   for(int i = 0; i<num_pixels; i++){
       rand_pixels[i] = rand()% (rows*cols);
   }
   //Image pointilism = make_image (rows,cols);
   // Copy image over to the newly allocated array
   //pointilism.data = in.data;
 	FILE *fp = fopen("test.txt","w");  
   // Create a filters of indices you need from the random indices
	int filters_top_right [num_pixels] [radius] [radius];
	int top = 1;
	int right = 1;
	for(int pix_idx = 0; pix_idx<num_pixels; pix_idx++){
		for(; top<=radius; top++){
			for(; right<=radius; right++){
				if(rand_pixels[pix_idx]+right % cols == 0){
					filters_top_right[pix_idx][top-1][right-1] = -1;
					fprintf(fp,"%d ",filters_top_right[pix_idx][top-1][right-1]);
				}
				else {
					filters_top_right[pix_idx][top-1][right-1] = rand_pixels[pix_idx]+right; 
					fprintf(fp,"%d ",filters_top_right[pix_idx][top-1][right-1]);
				}
			}
			
			if(rand_pixels[pix_idx]-(cols*top) < 0 ){
				filters_top_right[pix_idx][top-1][right-1] = -1;
				fprintf(fp,"%d ",filters_top_right[pix_idx][top-1][right-1]);
			}
			else {
				filters_top_right[pix_idx][top-1][right-1] = rand_pixels[pix_idx] - (cols*top);
				fprintf(fp,"%d ",filters_top_right[pix_idx][top-1][right-1]);
			}
		}

 	}	
	
	fclose(fp);
 
 }

