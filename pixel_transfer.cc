#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>

#define RGB2Y_FLOAT(r, g, b) \
    ((unsigned char)(0.299 * r + 0.587 * g + 0.114 * b))
#define RGB2U_FLOAT(r, g, b) \
    ((unsigned char)(-0.147 * r - 0.289 * g + 0.436 * b))
#define RGB2V_FLOAT(r, g, b) \
    ((unsigned char)(0.615 * r - 0.515 * g + 0.100 * b))
#define YUV420P2R_FLOAT(y, u, v) \
    ((unsigned char)(y + 1.4075 * (u - 128)))
#define YUV420P2G_FLOAT(y, u, v) \
    ((unsigned char)(y - 0.3455 * (u - 128) - 0.7169 * (v - 128)))
#define YUV420P2B_FLOAT(y, u, v) \
    ((unsigned char)(y + 1.779 * (u - 128)))

#define RGB2Y(r, g, b) \
    ((unsigned char)((66 * r + 129 * g + 25 * b + 128) >> 8) + 16)

#define RGB2U(r, g, b) \
    ((unsigned char)((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128)

#define RGB2V(r, g, b) \
    ((unsigned char)((112 * r - 94 * g - 18 * b + 128) >> 8) + 128)

unsigned char clip_value(unsigned char value, unsigned char maxVal, unsigned char minVal)
{
    if (value < minVal)
    {
        return minVal;
    }
    else if (value > maxVal)
    {
        return maxVal;
    }
    return value;
}

int rgb_2_yuv420p(unsigned char *src_buffer, unsigned char *dst_buffer, int w, int h)
{
    unsigned char *y = dst_buffer;
    unsigned char *u = dst_buffer + w * h;
    unsigned char *v = u + w * h * 1 / 4;

    unsigned char r, g, b;
    unsigned char *ptrRGB;
    //2:执行转换
    for (int i = 0; i < h; i++)
    {
        ptrRGB = src_buffer + w * i * 3;
        for (int j = 0; j < w; j++)
        {
            r = *(ptrRGB++);
            g = *(ptrRGB++);
            b = *(ptrRGB++);
            *y++ = clip_value(RGB2Y(r, g, b), 255, 0);
            if ((i % 2 == 0) && (j % 2 == 0))
            {
                *u++ = clip_value(RGB2U(r, g, b), 255, 0);
            }
            else if (j % 2 == 0)
            {
                *v++ = clip_value(RGB2V(r, g, b), 255, 0);
            }
        }
    }
    return 0;
}

void yuv420p_2_rgb(unsigned char *src_buffer, unsigned char *dst_buffer, int w, int h)
{
    unsigned char *ptrY = (unsigned char *)malloc(w * h);
    unsigned char *ptrU = (unsigned char *)malloc(w * h * 1 / 4);
    unsigned char *ptrV = (unsigned char *)malloc(w * h * 1 / 4);
    unsigned char *r = dst_buffer;
    unsigned char *g = (r + 1);
    unsigned char *b = (g + 1);
    unsigned char y, u, v;

    memcpy(ptrY, src_buffer, w * h);
    memcpy(ptrU, src_buffer, w * h * 1 / 4);
    memcpy(ptrV, src_buffer+w*h*5/4, w * h * 1 / 4);

    for (int i = 0; i < w;i++){
        for (int j = 0; j < h;j++){
            if(i!=0 && i%2==0){
                ptrU++;
                ptrV++;
            }
            if(j!=0 && j%2==0){
                ptrU -= w;
                ptrV -= w;
            }
            y = *(ptrY++);
            u = *(ptrU);
            v = *(ptrV);
            *r = YUV420P2R_FLOAT(y, u, v);
            *g = YUV420P2G_FLOAT(y, u, v);
            *b = YUV420P2B_FLOAT(y, u, v);
            r+=3;
            g+=3;
            b+=3;
        }
    }
    free(ptrY);
    free(ptrU);
    free(ptrV);
}

int yuv420p_to_rgb(const char *src_file, const char *dst_file, int w, int h, int num)
{
    FILE *src_fp = fopen(src_file, "rb");
    if (src_file == NULL)
    {
        printf("Open src file failed\n");
        return -1;
    }
    FILE *dst_fp = fopen(dst_file, "wb");
    if (dst_file == NULL)
    {
        printf("Open dst file failed\n");
        return -1;
    }

    int ret = -1;
    unsigned char *src_buffer = (unsigned char *)malloc(w * h * 3 / 2);
    unsigned char *dst_buffer = (unsigned char *)malloc(w * h * 3);
    memset(dst_buffer, 0, sizeof(dst_buffer));

    for (int i = 0; i < num; i++)
    {
        ret = fread(src_buffer, 1, w * h * 3 / 2, src_fp);
        rgb_2_yuv420p(src_buffer, dst_buffer, w, h);
        //3:写入目的文件
        fwrite(dst_buffer, 1, w * h * 3, dst_fp);
    }

    free(src_buffer);
    src_buffer = NULL;
    free(dst_buffer);
    dst_buffer = NULL;

    fclose(src_fp);
    fclose(dst_fp);
    return 0;
}

int rgb_to_yuv420p(const char *src_file, const char *dst_file, int w, int h, int num)
{
    //1:open file
    FILE *src_fp = fopen(src_file, "rb");
    if (src_fp == NULL)
    {
        printf("Open src file failed\n");
        return -1;
    }
    FILE *dst_fp = fopen(dst_file, "wb");
    if (dst_fp == NULL)
    {
        printf("Open dst file failed\n");
        return -1;
    }

    int ret = -1;
    //2:read data to buffer
    unsigned char *src_buffer = (unsigned char *)malloc(w * h * 3);
    unsigned char *dst_buffer = (unsigned char *)malloc(w * h * 3 / 2);
    memset(dst_buffer, 0, sizeof(dst_buffer));

    for (int i = 0; i < num; i++)
    {
        ret = fread(src_buffer, 1, w * h * 3, src_fp);
        rgb_2_yuv420p(src_buffer, dst_buffer, w, h);
        //3:写入目的文件
        fwrite(dst_buffer, 1, w * h * 3 / 2, dst_fp);
    }

    free(src_buffer);
    src_buffer = NULL;
    free(dst_buffer);
    dst_buffer = NULL;

    fclose(src_fp);
    fclose(dst_fp);
    return 0;
}

int main(int argc, char *argv[])
{
    /*
	if (argc < 5) {
		printf("Usage: a.exe src_file dst_file\n");
		return -1;
	}*/
    int w1 = 800;
    int h1 = 450;
    const char *src1_file = "./test_picture/rgb_test.rgb";
    const char *dst1_file = "./test_picture/my_rgb2yuv_output.yuv";

    const char *src_file = "./test_picture/yuv_test.yuv";
    const char *dst_file = "./test_picture/my_yuv2rgb_output.rgb";

    int w = 800;
    int h = 450;
    rgb_to_yuv420p(src1_file, dst1_file, w1, h1, 1);
    yuv420p_to_rgb(src_file, dst_file, w, h, 1);
    return 0;
}