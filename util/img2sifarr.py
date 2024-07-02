from PIL import Image

def get_pixel_colors(image_path):
    # Open the image
    image = Image.open(image_path)
    # Ensure the image is in RGB mode
    image = image.convert("RGBA")
    # Get the size of the image
    width, height = image.size

    # Create a list to hold the pixel color parts
    pixel_colors = []

    # Loop over each pixel in the image
    for y in range(height):
        for x in range(width):
            # Get the color of the pixel
            r, g, b, a = image.getpixel((x, y))
            # Append the parts to the list
            pixel_colors.append((r, g, b, a))

    return pixel_colors

def colors_to_string(pixel_colors):
    # Create a list to hold byte values as strings
    byte_string_list = []
    
    # Convert each color part to a byte string and append to the list
    for color_parts in pixel_colors:
        byte_string_list.extend([str(byte) for byte in color_parts])
    
    # Join the byte strings with commas
    l = str(len(byte_string_list))
    byte_string = "uint8_t wallpaper[" + l + "] = {" + ','.join(byte_string_list) + "};"
    
    return byte_string

def main():
    image_path = "wallp.png"  # Change this to the path of your image

    pixel_colors = get_pixel_colors(image_path)
    byte_string = colors_to_string(pixel_colors)
    with open("../src/include/temp/background.h", "w") as f:
        f.write(byte_string)

if __name__ == "__main__":
    main()