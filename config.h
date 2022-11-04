#ifndef CONFIG_H
#define CONFIG_H

#define PACKET_SIZE 1000
#define MAX_VERTICES 65535*3
#define ALIGH_VERT(alignment, type) memalign(alignment, sizeof(type) * MAX_VERTICES);
#define ALIGN_VERT_128(type) ALIGH_VERT(128, type);

#define FB_WIDTH 640
#define FB_HEIGHT 512

#define MAX_MODELS 256

#endif