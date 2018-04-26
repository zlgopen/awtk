
#include <stdio.h>
#include <string.h>
#if defined(WIN32)
#include <direct.h>
#define getcwd _getcwd
#define DIRPREFIX "\\"
#else
#include <unistd.h>
#define DIRPREFIX "/"
#endif
#include "picasso_image.h"

static char full_path[1024];

int main(int argc, char* argv[])
{
    char * path;
    psx_image * data;
    int ret;
    float q = 0.9f;

    if (psx_image_init() != 0) {
        fprintf(stderr, "Init faild!\n");
        return -1;
    }

    path = argv[1];
    if (!path || argc < 3) {
        fprintf(stderr, "usage: image_info.exe  <in_filename> <out_filename> <type>\n");
        return -1;
    }

    getcwd(full_path, 1024);
	strcat(full_path, DIRPREFIX);
    strcat(full_path, path);

    data = psx_image_load(full_path, &ret);
    if (!data) {
        fprintf(stderr, "Load picture faild! error code %d\n", ret);
		psx_image_shutdown();
        return -1;
    }

    fprintf (stderr, "Load picture: %s \nSize: %d x %d \nBytes per scanline %d \nFormat: %d\nFrames: %d \n", 
             path, data->width, data->height, data->pitch, data->format, (int)data->num_frames);

    if ((ret = psx_image_save_to_file(data, argv[2], argv[3], q)) != S_OK)
        fprintf(stderr,"Save image faild : %d\n", ret);

    psx_image_destroy(data);
    psx_image_shutdown();
    return 0;
}
