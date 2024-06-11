from PIL import Image

def get_pixel_colors(image_path):
    # Open the image
    image = Image.open(image_path)
    # Ensure the image is in RGB mode
    image = image.convert("RGB")
    # Get the size of the image
    width, height = 800, 600

    # Create a list to hold the pixel color parts
    pixel_colors = []

    # Loop over each pixel in the image
    for y in range(height):
        for x in range(width):
            # Get the color of the pixel
            r, g, b = image.getpixel((x, y))
            # Append the parts to the list
            pixel_colors.append((r, g, b))

    return pixel_colors

def write_colors_to_file(pixel_colors, output_file):
    with open(output_file, "wb") as file:  # Open file in binary mode
        for color_parts in pixel_colors:
            # Write each RGB component as a byte
            file.write(bytes(color_parts))

def main():
    image_path = "Untitled.png"  # Change this to the path of your image
    output_file = "../content/backgro.sif"

    pixel_colors = get_pixel_colors(image_path)
    write_colors_to_file(pixel_colors, output_file)
    print(f"Pixel colors have been written to {output_file}")

if __name__ == "__main__":
    main()