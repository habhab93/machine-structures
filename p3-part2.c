#include <emmintrin.h>
#include <string.h>
#include <nmmintrin.h>
#include <stdio.h>
#include <omp.h>
#define KERNX 3 //this is the x-size of the kernel. It will always be odd.
#define KERNY 3 //this is the y-size of the kernel. It will always be odd.




int conv2D(float* in, float* out, int data_size_X, int data_size_Y,
                    float* kernel)
{
	omp_set_num_threads(16);
    // the x coordinate of the kernel's center
    int kern_cent_X = (KERNX - 1)/2;
    // the y coordinate of the kernel's center
    int kern_cent_Y = (KERNY - 1)/2;
    
    int rows=data_size_X+2;
    int cols=data_size_Y+2;
    float tempin[rows*cols];
	int area = rows*cols;

	int l;
	#pragma omp parallel for
	for(l = 0; l < area; l++) {
	tempin[l]=0;
    }
	int k;

	#pragma omp parallel for
    for (k = 0; k < data_size_Y; k++) {
    	memcpy((tempin+1+(k+1)*rows), (in+k*data_size_X), data_size_X*4);
	}

    
 

	__m128 a1;
	__m128 a2;
	__m128 a3;
	__m128 a4;
	float* kernval;
	float* regval;
	//float* outval;
	int y,x,i,j;

   // main convolution loop
#pragma omp parallel for private(x,j,i,kernval,regval,a1,a2,a3,a4) firstprivate(data_size_X, data_size_Y, kern_cent_X, kern_cent_Y, kernel)
// THINK ABOUT KERNEL, OUTVAL
	for(y = 0; y < data_size_Y; y++){ // the y coordinate of theoutput location we're focusing on
		
		for(x = 0; x < data_size_X/4*4; x+=4){ // the x coordinate of the output location we're focusing on
		float* outval = (out+x+y*data_size_X);
		a4 = _mm_setzero_ps();				
			for(j = -kern_cent_Y; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate
				for(i = -kern_cent_X; i <= kern_cent_X; i++){ // kernel unflipped x coordinate
					
				
					kernval = (kernel+(kern_cent_X-i)+(kern_cent_Y-j)*KERNX);
					regval = (tempin+(x+i+1) + (y+j+1)*(data_size_X+2));
					
					

					a1 = _mm_load1_ps(kernval);
					a2 = _mm_loadu_ps(regval);
					a3 = _mm_mul_ps(a1, a2);
					a4 = _mm_add_ps(a4, a3);

				}
			}
		_mm_storeu_ps(outval, a4);
		}		
		for(x = data_size_X/4*4; x < data_size_X; x++){ // the x coordinate of the output location we're focusing on
		float* outval = (out+x+y*data_size_X);
			for(i = -kern_cent_X; i <= kern_cent_X; i++){ // kernel unflipped x coordinate

			
				for(j = -kern_cent_Y; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate	
					*outval += kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * tempin[(x+i+1) + (y+j+1)*(data_size_X+2)];
									}
			}
		}
	}
	return 1;
}


