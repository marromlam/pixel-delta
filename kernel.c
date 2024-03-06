/* Shader code in C for OpenCL
 *
 *
 *
 * */


/*
 * convert double to int
 * @param x - double number
 * @return - int
 * */
int double2int(const double x) {
    // TODO: this only works for openCL, need to change for other platforms
    return convert_int(x);
}

/*
 * blend two pixels
 * @param c - color
 * @param a - alpha
 * @return - blended color
 */
int blend(const double c, const double a) {
    return double2int((255. + ((c) - 255.) * (a)));
}


/*
 * rgb to yuv conversion
 * @param r - red
 * @param g - green
 * @param b - blue
 * @return - yuv
 * */
double rgb2y(const double r, const double g, const double b) {
    return r * 0.29889531 + g * 0.58662247 + b * 0.11448223;
}

double rgb2i(const double r, const double g, const double b) {
    return r * 0.59597799 - g * 0.27417610 - b * 0.32180189;
}

double rgb2q(const double r, const double g, const double b) {
    return r * 0.21147017 - g * 0.52261711 + b * 0.31114694;
}






KERNEL void python_pixel_delta(
  __global int* output,
  __global const int* img1,
  __global const int* img2,
  const int width,
  const int height
)
{
    int idx = get_global_id(0);
    int idy = get_global_id(1);
    // int idz = get_global_id(2);
    int res[4] = {0, 0, 0, 0};

    // printf("idx: %d, idy: %d, \n", idx, idy);
    // if (idx ==0 && idy  == 0) {
        // printf("w: %d, h: %d\n", width, height );
       int diff = 0;
        diff = pixel_delta(res, img1, img2, idx, idy, width, height);
        if (diff > 0) {
            // printf("--->diff: %d\n", diff);
            // then output[idx + idy * width + 4 * width * height] = res;
            // int pos = (y * width + x) * 4;
            // printf("-> %4d %4d %4d %4d", (idx + idy * width) * 4+0,
            // (idx + idy * width) * 4+1,
            // (idx + idy * width) * 4+2,
            // (idx + idy * width) * 4+3);
            output[(idx + idy * width) * 4+0] = res[0];
            output[(idx + idy * width) * 4+1] = res[1];
            output[(idx + idy * width) * 4+2] = res[2];
            output[(idx + idy * width) * 4+3] = res[3];

        }
    // }
}



