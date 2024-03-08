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




/*
 * draw a pixel
 * @param ret - return value
 * @param r - red
 * @param g - green
 * @param b - blue
 * */
void draw_pixel(int ret[4], const int r, const int g, const int b) {
    ret[0] = r;
    ret[1] = g;
    ret[2] = b;
    ret[3] = 255;
}



/*
 * draw a gray pixel
 * @param img - image
 * @param i - index
 * @param alpha - alpha
 * @param ret - return value
 * */
void draw_gray_pixel(const int* img, const int i, double alpha, int ret[4]) {
    int r = img[i + 0];
    int g = img[i + 1];
    int b = img[i + 2];
    double val = rgb2y(r, g, b);
    val = blend(val, alpha * img[i + 3] / 255.0);
    int val_int = double2int(val);
    draw_pixel(ret, val_int, val_int, val_int);
}



/* Color delta
 * @param img1 - image 1
 * @param img2 - image 2
 * @param k - index
 * @param m - index
 * @param y_only - only y
 * @return - color delta
 *
 * Note: This function is used to calculate the color delta between two pixels
 *
 * Reference:
 * Measuring perceived color difference using YIQ NTSC transmission color space
 * in mobile applications -- Yuriy Kotsarenko, Fernando Ramos
 * */
double color_delta(const int* img1, const int* img2, size_t k, size_t m, int y_only) {
    int r1 = img1[k + 0], g1 = img1[k + 1], b1 = img1[k + 2], a1 = img1[k + 3];
    int r2 = img2[m + 0], g2 = img2[m + 1], b2 = img2[m + 2], a2 = img2[m + 3];

    if (a1 == a2 && r1 == r2 && g1 == g2 && b1 == b2) {
        return 0.0;
    }

    if (a1 < 255) {
        const double alpha1 = a1 / 255.0;
        r1 = blend(r1, alpha1);
        g1 = blend(g1, alpha1);
        b1 = blend(b1, alpha1);
    }

    if (a2 < 255) {
        const double alpha2 = a2 / 255.0;
        r2 = blend(r2, alpha2);
        g2 = blend(g2, alpha2);
        b2 = blend(b2, alpha2);
    }

    const double y1 = rgb2y(r1, g1, b1);
    const double y2 = rgb2y(r2, g2, b2);
    const double deltay = y1 - y2;

    if (y_only) {
        return deltay;
    }

    const double deltai = rgb2i(r1, g1, b1) - rgb2i(r2, g2, b2);
    const double deltaq = rgb2q(r1, g1, b1) - rgb2q(r2, g2, b2);

    const double deltae = 0.5053 * deltay * deltay + 0.299 * deltai * deltai + 0.1957 * deltaq * deltaq;

    return (y1 > y2) ? -deltae : deltae;
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



