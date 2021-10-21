#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <vector>
#include <string>

#define PI 3.1415

#define ZIG(i,y,x) level[i]=dct[x*8+y]
#define ZIGZAG8_FRAME \
    ZIG(0,0,0)  ZIG(1,0,1)  ZIG(2,1,0)  ZIG(3,2,0)  \
    ZIG(4,1,1)  ZIG(5,0,2)  ZIG(6,0,3)  ZIG(7,1,2)  \
    ZIG(8,2,1)  ZIG(9,3,0)  ZIG(10,4,0) ZIG(11,3,1) \
    ZIG(12,2,2) ZIG(13,1,3) ZIG(14,0,4) ZIG(15,0,5) \
    ZIG(16,1,4) ZIG(17,2,3) ZIG(18,3,2) ZIG(19,4,1) \
    ZIG(20,5,0) ZIG(21,6,0) ZIG(22,5,1) ZIG(23,4,2) \
    ZIG(24,3,3) ZIG(25,2,4) ZIG(26,1,5) ZIG(27,0,6) \
    ZIG(28,0,7) ZIG(29,1,6) ZIG(30,2,5) ZIG(31,3,4) \
    ZIG(32,4,3) ZIG(33,5,2) ZIG(34,6,1) ZIG(35,7,0) \
    ZIG(36,7,1) ZIG(37,6,2) ZIG(38,5,3) ZIG(39,4,4) \
    ZIG(40,3,5) ZIG(41,2,6) ZIG(42,1,7) ZIG(43,2,7) \
    ZIG(44,3,6) ZIG(45,4,5) ZIG(46,5,4) ZIG(47,6,3) \
    ZIG(48,7,2) ZIG(49,7,3) ZIG(50,6,4) ZIG(51,5,5) \
    ZIG(52,4,6) ZIG(53,3,7) ZIG(54,4,7) ZIG(55,5,6) \
    ZIG(56,6,5) ZIG(57,7,4) ZIG(58,7,5) ZIG(59,6,6) \
    ZIG(60,5,7) ZIG(61,6,7) ZIG(62,7,6) ZIG(63,7,7) \




//move left
int move_left(std::vector<std::vector<int>> &matrix)
{
    int n = matrix.size(), m = matrix[0].size();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            matrix[i][j] = matrix[i][j] + 128;
        }
    }
    return 0;
}

//move right
int move_right(std::vector<std::vector<int>> &matrix)
{
    int n = matrix.size(), m = matrix[0].size();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            matrix[i][j] = matrix[i][j] - 128;
        }
    }
    return 0;
}

float DCT(std::vector<std::vector<int>> &matrix, int u, int v, int N)
{
    /*
    if (u < 0 || u >= N || v < N || v >= N)
    {
        printf("u or v is out of range!\n");
        return -1;
    }
    */
    float dct_ans;
    float alpha_u, alpha_v;
    float p1 = 0, p2 = 0, sum = 0;
    if (u == 0)
    {
        alpha_u = sqrt(1.0 / N);
    }
    else
    {
        alpha_u = sqrt(2.0 / N);
    }
    if (v == 0)
    {
        alpha_v = sqrt(1.0 / N);
    }
    else
    {
        alpha_v = sqrt(2.0 / N);
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            p1 = cos((PI * (2 * i + 1) * u) / (2 * N));
            p2 = cos((PI * (2 * j + 1) * v) / (2 * N));
            sum += matrix[i][j] * p1 * p2;
        }
    }
    dct_ans = sum * alpha_u * alpha_v;
    return dct_ans;
}

int rounding(float num)
{
    if (num >= 0)
    {
        return (int)(num + 0.5);
    }
    else
    {
        return (int)(num - 0.5);
    }
}
float IDCT(std::vector<std::vector<int>> &matrix, int u, int v, int N)
{
    /*
    if (u < 0 || u >= N || v < N || v >= N)
    {
        printf("u or v is out of range!\n");
        return -1;
    }
    */
    float idct_ans;
    float alpha_i, alpha_j;
    float p1 = 0, p2 = 0, sum = 0;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (i == 0)
            {
                alpha_i = sqrt(1.0 / N);
            }
            else
            {
                alpha_i = sqrt(2.0 / N);
            }
            if (j == 0)
            {
                alpha_j = sqrt(1.0 / N);
            }
            else
            {
                alpha_j = sqrt(2.0 / N);
            }
            p1 = cos(PI * (2 * u + 1) * i / (2 * N));
            p2 = cos(PI * (2 * v + 1) * j / (2 * N));
            sum += alpha_i * alpha_j * matrix[i][j] * p1 * p2;
        }
    }
    idct_ans = sum;
    return idct_ans;
}

int quantify(std::vector<std::vector<int>> &matrix, std::vector<std::vector<int>> qp_matrix)
{
    int m = matrix.size(), n = matrix[0].size();
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = matrix[i][j] / qp_matrix[i][j];
        }
    }
    return 0;
}

int Iquantify(std::vector<std::vector<int>> &matrix, std::vector<std::vector<int>> qp_matrix)
{
    int m = matrix.size(), n = matrix[0].size();
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = matrix[i][j] * qp_matrix[i][j];
        }
    }
    return 0;
}

int encode_dct(std::vector<std::vector<int>> &dct_matrix, std::vector<std::vector<int>> &matrix, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            dct_matrix[i][j] = ceil(DCT(matrix, i, j, N));
        }
    }
    return 0;
}

int decode_idct(std::vector<std::vector<int>> &idct_matrix, std::vector<std::vector<int>> &matrix, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            idct_matrix[i][j] = ceil(IDCT(matrix, i, j, N));
        }
    }
    return 0;
}

int display(std::vector<std::vector<int>> &matrix)
{
    int m = matrix.size(), n = matrix[0].size();
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
    return 0;
}

int read_file(const char *filename, unsigned char *data, int w, int h)
{
    int ret = 0;
    FILE *fp = NULL;
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("Open input file error\n");
        return -1;
    }
    ret = fread(data, 1, w * h * 3 / 2, fp);
    if (ret <= 0)
    {
        printf("Read input file error\n");
        fclose(fp);
        return -1;
    }
    fclose(fp);
    return 0;
}

int write_file(const char *filename, unsigned char *data, int w, int h)
{
    int ret = 0;
    FILE *fp = NULL;
    fp = fopen(filename, "wb");
    if (fp == NULL)
    {
        printf("Open output file error\n");
        return -1;
    }
    ret = fwrite(data, 1, w * h * 3 / 2, fp);
    if (ret <= 0)
    {
        printf("Write output file error\n");
        fclose(fp);
        return -1;
    }
    fclose(fp);
    return 0;
}

int encode_frame(std::vector<std::vector<int>> &matrix, std::vector<std::vector<int>> &qp_matrix, int w, int h, int N)
{
    int m = h / N, n = w / N;
    std::vector<std::vector<int>> cur_matrix(N, std::vector<int>(N, 0));
    std::vector<std::vector<int>> origin_matrix(cur_matrix);
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < N; k++)
            {
                for (int l = 0; l < N; l++)
                {
                    cur_matrix[k][l] = matrix[i * N + k][j * N + l];
                }
            }
            
            //move right
            move_right(origin_matrix);
            //dct
            encode_dct(cur_matrix, origin_matrix, N);
            //quant
            quantify(cur_matrix, qp_matrix);

            for (int k = 0; k < N; k++)
            {
                for (int l = 0; l < N; l++)
                {
                    matrix[i * N + k][j * N + l] = cur_matrix[k][l];
                }
            }
            
        }
    }

    return 0;
}

int decode_frame(std::vector<std::vector<int>> &matrix, std::vector<std::vector<int>> &qp_matrix, int w, int h, int N)
{
    int m = h / N, n = w / N;
    std::vector<std::vector<int>> cur_matrix(N, std::vector<int>(N, 0));
    std::vector<std::vector<int>> origin_matrix(cur_matrix);

    for (int i = 0; i < m; i++)  
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < N; k++)
            {
                for (int l = 0; l < N; l++)
                {
                    cur_matrix[k][l] = matrix[i * N + k][j * N + l];
                }
            }
            
            //inverse quant
            Iquantify(origin_matrix, qp_matrix);
            //inverse dct
            decode_idct(cur_matrix, origin_matrix, N);
            //move left
            move_left(cur_matrix);
            for (int k = 0; k < N; k++)
            {
                for (int l = 0; l < N; l++)
                {
                    matrix[i * N + k][j * N + l] = cur_matrix[k][l];
                }
            }
            
        }
    }

    return 0;
}

int encode(const char *in_filename, const char *out_filename, int w, int h)
{
    int ret = 0;
    unsigned char *data = (unsigned char *)malloc(w * h * 3 / 2);
    unsigned char *tranfer_data = (unsigned char *)malloc(w * h * 3 / 2);

    ret = read_file(in_filename, data, w, h);
    if (ret < 0)
    {
        return -1;
    }

    std::vector<std::vector<int>> data_y(h, std::vector<int>(w, 0));
    std::vector<std::vector<int>> data_u(h * 1 / 2, std::vector<int>(w * 1 / 2, 0));
    std::vector<std::vector<int>> data_v(h * 1 / 2, std::vector<int>(w * 1 / 2, 0));
    /*std::vector<std::vector<int>> qp_matrix = {{16, 11, 10, 16, 24, 40, 51, 61},
                                               {12, 12, 14, 19, 26, 58, 60, 55},
                                               {14, 13, 16, 24, 40, 57, 69, 56},
                                               {14, 17, 22, 29, 51, 87, 80, 62},
                                               {18, 22, 37, 56, 68, 109, 103, 77},
                                               {24, 35, 55, 64, 81, 104, 113, 92},
                                               {49, 64, 78, 87, 103, 121, 120, 101},
                                               {72, 92, 95, 98, 112, 100, 103, 99}};
    */
    std::vector<std::vector<int>> qp_matrix(8, std::vector<int> (8, 23));

    //split data
    int m = 0, n = 0;
    for (int i = 0; i < w * h; i++)
    {
        if (i != 0 && i % w == 0)
        {
            m++;
            n = 0;
        }
        data_y[m][n++] = (int)*(data++);
    }
    m = 0;
    n = 0;
    for (int i = 0; i < w * h / 4; i++)
    {
        if (i != 0 && i % w == 0)
        {
            m++;
            n = 0;
        }
        data_u[m][n++] = (int)*(data++);
    }
    m = 0;
    n = 0;
    for (int i = 0; i < w * h / 4; i++)
    {
        if (i != 0 && i % w == 0)
        {
            m++;
            n = 0;
        }
        data_v[m][n++] = (int)*(data++);
    }

    int N = 8;
    //encode
    encode_frame(data_y, qp_matrix, w, h, N);
    encode_frame(data_u, qp_matrix, w / 2, h / 2, N);
    encode_frame(data_v, qp_matrix, w / 2, h / 2, N);

    //decode
    decode_frame(data_y, qp_matrix, w, h, N);
    decode_frame(data_u, qp_matrix, w / 2, h / 2, N);
    decode_frame(data_v, qp_matrix, w / 2, h / 2, N);

  
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            *(tranfer_data++) = (unsigned char)data_y[i][j];
        }
    }

    for (int i = 0; i < h / 2; i++)
    {
        for (int j = 0; j < w / 2; j++)
        {
            *(tranfer_data++) = (unsigned char)data_u[i][j];
        }
    }

    for (int i = 0; i < h / 2; i++)
    {
        for (int j = 0; j < w / 2; j++)
        {
            *(tranfer_data++) = (unsigned char)data_v[i][j];
        }
    }

    ret = write_file(out_filename, tranfer_data, w, h);
    if (ret < 0)
    {
        printf("write file error\n");
        return -1;
    }

    free(data);
    free(tranfer_data);
    return 0;
}

int main(int argc, char *argv[])
{
    /*
    std::vector<std::vector<int>> qp_matrix = {{16, 11, 10, 16, 24, 40, 51, 61},
                                               {12, 12, 14, 19, 26, 58, 60, 55},
                                               {14, 13, 16, 24, 40, 57, 69, 56},
                                               {14, 17, 22, 29, 51, 87, 80, 62},
                                               {18, 22, 37, 56, 68, 109, 103, 77},
                                               {24, 35, 55, 64, 81, 104, 113, 92},
                                               {49, 64, 78, 87, 103, 121, 120, 101},
                                               {72, 92, 95, 98, 112, 100, 103, 99}};

    std::vector<std::vector<int>> matrix = {{52, 55, 61, 66, 70, 61, 64, 73},
                                            {63, 59, 55, 90, 109, 85, 69, 72},
                                            {62, 59, 68, 113, 144, 104, 66, 73},
                                            {63, 58, 71, 122, 154, 106, 70, 69},
                                            {67, 61, 68, 104, 126, 88, 68, 70},
                                            {79, 65, 60, 70, 77, 68, 58, 75},
                                            {85, 71, 64, 59, 55, 61, 65, 83},
                                            {87, 79, 69, 68, 65, 76, 78, 94}};

    std::vector<std::vector<int>> dct_matrix(8, std::vector<int>(8, 0));
    printf("-----------origin matrix-------------\n");
    display(matrix);

    printf("\n----------move right---------------\n");
    move_right(matrix);
    display(matrix);

    printf("\n----------encode dct---------------\n");
    encode_dct(dct_matrix, matrix, 8);
    display(dct_matrix);

    printf("\n----------quantify-----------------\n");
    quantify(dct_matrix, qp_matrix);
    display(dct_matrix);

    printf("\n----------Iquantify----------------\n");
    Iquantify(dct_matrix, qp_matrix);
    display(dct_matrix);

    printf("\n----------decode_idct---------------------\n");
    decode_idct(matrix, dct_matrix, 8);
    display(matrix);

    printf("\n----------move left----------------\n");
    move_left(matrix);
    display(matrix);

    return 0;
*/
    std::string input = "./test_picture/yuv_800x440_test.yuv";
    std::string output = "output_yuv.yuv";
    encode(input.c_str(), output.c_str(), 800, 440);
    return 0;
}