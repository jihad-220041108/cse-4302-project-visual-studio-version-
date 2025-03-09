from PIL import Image
import os

# Get all PNG files in the current directory
image_files = [f for f in os.listdir('.') if f.endswith('.png') and f[0].isdigit()]

# Sort the files in ascending order (e.g., '1.png', '2.png', ..., '7.png')
image_files.sort(key=lambda f: int(f.split('.')[0]))

# Open all images
images = [Image.open(f) for f in image_files]

# Get the dimensions of the individual images
width, height = images[0].size

# Determine the total width and height for the new spritesheet
# Assuming we want to place all the images in one row
total_width = width * len(images)
total_height = height

# Create a new blank image for the spritesheet
spritesheet = Image.new('RGBA', (total_width, total_height))

# Paste each image into the spritesheet at the correct position
x_offset = 0
for image in images:
    spritesheet.paste(image, (x_offset, 0))
    x_offset += width

# Save the spritesheet to the current folder
spritesheet.save('spritesheet.png')

print("Spritesheet created successfully!")
