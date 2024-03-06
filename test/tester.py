from pixeldelta import pixeldelta
import numpy as np
import cv2


image1 = cv2.imread("/Users/marcos/Downloads/4a.png", cv2.IMREAD_UNCHANGED)
image2 = cv2.imread("/Users/marcos/Downloads/4b.png", cv2.IMREAD_UNCHANGED)

# get width and height of image1
width = image1.shape[0]
height = image1.shape[1]

if image1.shape[2] == 3:
    # we need to add alpha channel to image1
    image1 = cv2.cvtColor(image1, cv2.COLOR_RGB2RGBA)
    image2 = cv2.cvtColor(image2, cv2.COLOR_RGB2RGBA)

output = np.zeros_like(image1)
result = pixeldelta(image1, image2, output, width, height)


# save the output image to disk
cv2.imwrite("/Users/marcos/Downloads/output.png", image1)
