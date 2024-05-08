#include <stdio.h>
#include <math.h>

int main () {

// Find matrix size
double sigma = 0.5;
double sigma_sq = sigma * sigma;
double PI = 3.1415926535;
int N = sigma*10;
// N should always be odd
if(N % 2 == 0){
	N++;
}
int position = N/2;

int dx = position;
int dy = position;
double kernel[N][N];
printf("%d\n",position);
for(int i =0; i<N; i++){
	for(int j=0; j<N ; j++){
		kernel [i][j] = (1.0 / (2.0 * PI *(sigma_sq))) * exp(-((dx*dx)+(dy*dy))/(2 * (sigma_sq)));
		printf("%lf\n",kernel[i][j]);
		(j<N/2) ? dx-- : dx++;
	}
	(i<N/2) ? dy-- : dy++;
	dx = position;
}

printf("%lf\n",*(*(kernel+(N/2))+N/2));

return 0;

}
