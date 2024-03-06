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




