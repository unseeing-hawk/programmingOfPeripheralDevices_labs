import cv2


def on_change(self):  # Does nothing but function must be passed to cv2.createTrackbar
    pass


cap = cv2.VideoCapture(0)  # Define capture device 0 = first capture device etc
cv2.namedWindow('Canny', cv2.WINDOW_AUTOSIZE)  # Create a named window to add trackbars and video frames to

# Create trackbars
cv2.createTrackbar("D", "Canny", 6, 100, on_change)  # Bilateral filter
cv2.createTrackbar("Sigma Colour", "Canny", 30, 150, on_change)
cv2.createTrackbar("Sigma Space", "Canny", 1, 150, on_change)
cv2.createTrackbar("Lower", "Canny", 20, 255, on_change)  # Limits for canny edge detection
cv2.createTrackbar("Upper", "Canny", 25, 255, on_change)
cv2.createTrackbar("Gaussian Blur", "Canny", 1, 15, on_change)  # Gaussian blur
cv2.createTrackbar("Median Blur", "Canny", 0, 99, on_change)  # Median blur

cv2.startWindowThread()  # Required with namedWindow()

width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
writer = cv2.VideoWriter('result.avi', cv2.VideoWriter_fourcc(*'DIVX'), 20, (width, height))

# Define list of odd numbers, required for median blur.
med = list(range(1, 200, 2))

while (True):
    d = cv2.getTrackbarPos("D", "Canny")
    sc = cv2.getTrackbarPos("Sigma Colour", "Canny")
    ss = cv2.getTrackbarPos("Sigma Space", "Canny")

    l = cv2.getTrackbarPos("Lower", "Canny")  # Limits for canny edge detection
    u = cv2.getTrackbarPos("Upper", "Canny")

    m = cv2.getTrackbarPos("Median Blur", "Canny")  # Median blur
    m = med[m]  # Select m from list of odd numbers, faster than conditionals?
    b = cv2.getTrackbarPos("Gaussian Blur", "Canny")  # Gaussian blur

    if b == 0:  # Ensure that b is not 0, this would cause error in cv2.blur()
        b = 1

    _, frame = cap.read()  # Capture frame, ret not used so assigned to _
    fps = cap.get(cv2.CAP_PROP_FPS)
    print(fps)

    # Process frame to (hopefully) improve edge detection
    frame = cv2.cvtColor(frame, cv2.COLOR_RGB2HLS)
    median = cv2.medianBlur(frame, m)
    gaussian = cv2.blur(median, (b, b))
    processed = cv2.bilateralFilter(gaussian, d, sc, ss)
    canny = cv2.Canny(processed, l, u, L2gradient=True)

    writer.write(frame)

    # Open windows showing each step of the image processing.
    cv2.imshow('Bilateral', processed)
    cv2.imshow('Canny', canny)

    if cv2.waitKey(1) & 0xFF == ord('q'):  # Quits on q key press
        break

cap.release()
writer.release()
cv2.destroyAllWindows()