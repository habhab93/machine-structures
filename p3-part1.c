#include <emmintrin.h>
#include <string.h>
#include <nmmintrin.h>
#include <stdio.h>
#define KERNX 3 //this is the x-size of the kernel. It will always be odd.
#define KERNY 3 //this is the y-size of the kernel. It will always be odd.




int conv2D(float* in, float* out, int data_size_X, int data_size_Y,
                    float* kernel)
{
    // the x coordinate of the kernel's center
    int kern_cent_X = (KERNX - 1)/2;
    // the y coordinate of the kernel's center
    int kern_cent_Y = (KERNY - 1)/2;
    
    int rows=data_size_X+2;
    int cols=data_size_Y+2;
    float tempin[rows*cols];
    memset(tempin, 0, rows*cols*4);

    for (int i = 0; i < data_size_Y; i++) {
    	memcpy((tempin+1+(i+1)*rows), (in+i*data_size_X), data_size_X*4);
    }
    
 
	
/* for(int r = 1; r < rows-1; r++) {
	for (int c = 1; c < cols-1; c++) {
		int i = r+c*rows;
		if (r==0) {
			tempin[i]=0;
		} else if (r==(rows-1)) {
			tempin[i]=0;
		} else if (c==0) {
			tempin[i]=0;
		} else if (c==(cols-1)) {
			tempin[i]=0;
		} else {
			tempin[i]=in[(r-1)+(c-1)*data_size_X];
		}
		tempin[i]=in[(r-1)+(c-1)*data_size_X];
    }
} */



	__m128 a1 = _mm_setzero_ps();
	__m128 a2 = _mm_setzero_ps();
	__m128 a3 = _mm_setzero_ps();
	
	


   // main convolution loop
	
	for(int y = 0; y < data_size_Y; y++){ // the y coordinate of theoutput location we're focusing on
		for(int x = 0; x < data_size_X/4*4; x+=4){ // the x coordinate of the output location we're focusing on
		float* outval = (out+x+y*data_size_X);
		__m128 a4 = _mm_setzero_ps();			
			for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate
				for(int i = -kern_cent_X; i <= kern_cent_X; i++){ // kernel unflipped x coordinate
					
				
					float* kernval = (kernel+(kern_cent_X-i)+(kern_cent_Y-j)*KERNX);
					float* regval = (tempin+(x+i+1) + (y+j+1)*(data_size_X+2));
					
					

					a1 = _mm_load1_ps(kernval);
					a2 = _mm_loadu_ps(regval);
					a3 = _mm_mul_ps(a1, a2);
					a4 = _mm_add_ps(a4, a3);

				}
			}
		_mm_storeu_ps(outval, a4);
		}
		for(int x = data_size_X/4*4; x < data_size_X; x++){ // the x coordinate of the output location we're focusing on
		float* outval = (out+x+y*data_size_X);
			for(int i = -kern_cent_X; i <= kern_cent_X; i++){ // kernel unflipped x coordinate

			
				for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate
					
				
					*outval += kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * tempin[(x+i+1) + (y+j+1)*(data_size_X+2)];
					

									}
			}
		}
	}
	return 1;
}

