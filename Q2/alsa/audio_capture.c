#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <alsa/asoundlib.h>

#define CHANNELS    2
#define FRAMES      768  

int main(int argc, char * argv[]) {

    FILE * rec_file = fopen(argv[1], "w");

    snd_pcm_t * handle;
    snd_pcm_hw_params_t * hw_params; 

    /*Open Sound Card*/
    int ret = 

    /*Allocate configuration Space*/
    
    /*Link configuration space to handle*/
    

    /*Set access mode interleaved/non interleaved*/

    /*Set format*/
    
    /*set channels*/
    
    /*set Rate*/
    

    /*Set hw params*/    

    /*Allocate buffer in application, this buffer contains the audio*/
    int size = CHANNELS * FRAMES * sizeof(uint32_t);
    uint32_t * buffer = (uint32_t * )malloc(size);

    for (;;) {

        /*Read from card*/
        
        /*Save buffer of data to file*/
        int n_bytes = fwrite(buffer, 1, size, rec_file);
        /*flush content*/
        fflush(rec_file);
    }

    /*Close audio card*/
    
    /*Close file*/
    fclose(rec_file);

    return ret;
}