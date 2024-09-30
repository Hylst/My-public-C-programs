Image filtering Program


How It Works:
Select, load and display an picture / bitmap File.
Choose an apply graphical effect according to given parameters.
Display changes and save it modified.

Gaussian Blur: Approximates a smooth blur using a 3x3 Gaussian kernel.
Edge Detection: Highlights edges using a high-pass filter.
Motion Blur: Simulates motion with a directional blur.
Emboss: Adds a 3D shadow effect with a bias.
Mean Filter: Averages the pixels in a 3x3 grid to blur the image. This program now supports the following filters using SDL2:
Gaussian Blur: Smoothens the image by applying a weighted blur.
Edge Detection: Highlights the edges of objects in the image.
Motion Blur: Creates a directional blur effect, simulating movement.
Emboss: Adds a 3D effect to the image.
Mean Filter: Blurs the image by averaging the pixel values.
Median Filter: Removes noise by selecting the median pixel value in a neighborhood.

This updated version of the program adds Sharpen filtering and fully implements the filtering logic for each filter type. The program uses SDL2 to load, manipulate, and display the images, and applies the following filters:

Gaussian Blur
Edge Detection
Motion Blur
Emboss
Mean Filter
Median Filter
Sharpen
Each filter can be applied based on the user's menu selection, and the original and filtered images are displayed side by side. Here's the added functionality:

Sharpen Filter: Defined using a 3x3 kernel, it enhances edges to make the image clearer.
Filtering Logic: All filters are implemented and can be selected from the menu.
UI Enhancements: The original and filtered images are displayed on the screen, and text explanations accompany the filters.
