from PIL import Image


def get_pixel_colors(image_path):
    image = Image.open(image_path)
    image = image.convert("RGBA")
    width, height = image.size

    pixel_colors = []

    for y in range(height):
        for x in range(width):
            r, g, b, a = image.getpixel((x, y))
            pixel_colors.append((r, g, b, a))

    return pixel_colors


def write_colors_to_file(pixel_colors, output_file):
    with open(output_file, "wb") as file:
        for color_parts in pixel_colors:
            file.write(bytes(color_parts))


def main():
    image_path = "wallp.png"
    output_file = "../content/wallp.sif"

    pixel_colors = get_pixel_colors(image_path)
    write_colors_to_file(pixel_colors, output_file)
    print(f"Pixel colors have been written to {output_file}")


if __name__ == "__main__":
    main()
