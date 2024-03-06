import numpy as np
import reikna.cluda as cluda
import numpy as np
import cv2
import os


THIS_FILE_PATH = os.path.dirname(os.path.realpath(__file__))

api = cluda.ocl_api()
thr = api.Thread.create()
prg = thr.compile(open(os.path.join(THIS_FILE_PATH, "kernel.c")).read())


def pixeldelta_ocl(image1, image2):
    """
    Calculate the difference between two images using OpenCL. The images must be
    the same size. This is a wrapper around the OpenCL kernel.

    Parameters
    ----------
    image1 : np.ndarray
        The first image
    image2 : np.ndarray
        The second image, must be the same size as the first

    Returns
    -------
    diff_h : np.ndarray
        The difference between the two images
    """
    img1_width = image1.shape[0]
    img1_height = image1.shape[1]
    img2_width = image2.shape[0]
    img2_height = image2.shape[1]

    if img1_width != img2_width or img1_height != img2_height:
        raise ValueError("Image sizes do not match")
    else:
        width = img1_width
        height = img1_height

    image1_d = thr.to_device(image1.astype(np.int32))
    image2_d = thr.to_device(image2.astype(np.int32))
    diff_d = thr.empty_like(image1_d)

    knl = prg.python_pixel_delta
    knl(
        diff_d,
        image1_d,
        image2_d,
        np.int32(width),
        np.int32(height),
        global_size=(width, height, 1),
    )
    diff_h = diff_d.get()
    return 1, diff_h


if __name__ == "__main__":
    image1 = cv2.imread("tester1.png", cv2.IMREAD_UNCHANGED)
    image2 = cv2.imread("tester2.png", cv2.IMREAD_UNCHANGED)

    width = image1.shape[0]
    height = image1.shape[1]
    if image1.shape[2] == 3:
        image1 = cv2.cvtColor(image1, cv2.COLOR_RGB2RGBA)
        image2 = cv2.cvtColor(image2, cv2.COLOR_RGB2RGBA)
    image1 = np.ascontiguousarray(image1).astype(np.int32)
    image2 = np.ascontiguousarray(image2).astype(np.int32)

    _, diff_h = pixeldelta_ocl(
        image1.astype(np.uint8),
        image2.astype(np.uint8),
    )
    cv2.imwrite("diff.png", diff_h)

# vim: fdm=marker sw=4 ts=4 sts=4 et
