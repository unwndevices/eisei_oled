import serial
import os
import datetime
import customtkinter
from customtkinter import CTkImage, CTkFont
from PIL import Image, ImageTk, ImageOps, ImageEnhance

if not os.path.exists('./screenshots/'):
    os.makedirs('./screenshots/')

# Create a serial connection
ser = serial.Serial('COM12', 115200)

# Create a tkinter GUI
root = customtkinter.CTk()
root.title("Eisei screengrabber")
root.iconbitmap("./icon.ico")
image_label = customtkinter.CTkLabel(root)
image_label.pack()

font = CTkFont(family="DIN 2014", size=17)

# Width and height of the display
WIDTH = 128
HEIGHT = 128
# Scaling factor
SCALE = 5

# Create an empty black image
empty_image = Image.new('L', (WIDTH * SCALE, HEIGHT * SCALE), color=0)
empty_tk_image = CTkImage(empty_image, size= (WIDTH * SCALE, HEIGHT * SCALE))
image_label.configure(image=empty_tk_image, text = "")
image_label.image = empty_tk_image

# Reverse the color map
color_map = [(255, 246, 0), (238, 230, 3), (221, 213, 3), (204, 197, 3), (187, 181, 3), (170, 165, 3),
             (153, 148, 3), (136, 132, 3), (119, 116, 3), (102, 100, 3), (85, 83, 3), (81, 81, 3),(68, 67, 3),
             (51, 51, 3), (41, 39, 3), (0, 2, 3)][::-1]


def send_screenshot_command():
    command = "screenshot"
    ser.write(command.encode())

def save_image():
    # Get the current image from the image_label
    current_image = image_label.pil_image
    # Get the current date and time
    now = datetime.datetime.now()
    # Format the date and time to be used in the filename
    timestamp = now.strftime("%d-%m-%y-%H-%M-%S")
    # Create the filename
    filename = f"./screenshots/{timestamp}.png"
    # Save the image to the selected location
    current_image.save(filename, "PNG")


def read_from_arduino():
    while ser.inWaiting() > 0:
        # Read the line sent from the Arduino
        line = ser.readline().decode('utf-8').strip()
        print(f"Received line: {line}")  # print for debugging

        if line == ">img:":
            print("Start of image data")  # print for debugging
            # Start reading image data
            data = []
            while True:
                line = ser.readline().decode('utf-8').strip()
                if line == "<":
                    # End of image data
                    print("End of image data")  # print for debugging
                    break
                
                # Convert the line to an integer and split it into two 4-bit pixel values
                val = int(line)
                pixel1 = val >> 4  # Get the first 4 bits
                pixel2 = val & 0x0F  # Get the last 4 bits
                
                # Scale the pixel values from 4-bit to 8-bit grayscale and add them to the data list
                data.extend([pixel1 * 17, pixel2 * 17])

            # Create an image from the data
            image = Image.new('L', (HEIGHT, WIDTH))  # create image with swapped width and height
            pixels = image.load()

            # Fill the image with the data
            for i in range(0, len(data), 2):  # iterate over pairs of pixels
                x = i // HEIGHT  # column index
                y = i % HEIGHT  # row index
                pixels[y, x] = data[i]  # set pixel to first pixel value
                if y+1 < HEIGHT:  # check if there is a next row
                    pixels[y+1, x] = data[i+1]  # set pixel to second pixel value

            # Apply the transformations
            image = image.transpose(Image.FLIP_TOP_BOTTOM)  # flip vertically
            image = image.transpose(Image.FLIP_LEFT_RIGHT)  # flip horizontally

            # Resize the image
            image = image.resize((WIDTH * SCALE, HEIGHT * SCALE), Image.NEAREST)

            # Convert the image to grayscale and apply the color map
            image = image.convert('L')
            data = [color_map[pixel // 17] if pixel != 0 else (0, 0, 0) for pixel in image.getdata()]

            # Convert the image to RGB
            image = Image.new('RGB', image.size)
            image.putdata(data)


            # Convert the image to a format that tkinter can use
            tk_image = CTkImage(image, size= (WIDTH * SCALE, HEIGHT * SCALE))
             # Save a reference to the original PIL Image object
            image_label.pil_image = image
            
            # Update the GUI with the new image
            image_label.configure(image=tk_image)
            image_label.image = tk_image
        else:
            # If no image data is received, show an empty black image
            image_label.configure(image=empty_tk_image)
            image_label.image = empty_tk_image

    # Schedule the next read
    root.after(100, read_from_arduino)

# Create a frame for the buttons
button_frame = customtkinter.CTkFrame(root)
button_frame.pack(pady=10)  # Add some vertical padding

# Create the screenshot button
screenshot_button = customtkinter.CTkButton(button_frame, text="screenshot", command=send_screenshot_command, fg_color="#E6DE03", text_color="#131039", font=font)
screenshot_button.pack(side="left", padx=20 , pady=10)  # pack it to the left and add some horizontal padding

# Create the save button
save_button = customtkinter.CTkButton(button_frame, text="save", command=save_image, fg_color="#E6DE03", text_color="#131039", font=font)
save_button.pack(side="left", padx=20, pady=10)  # pack it to the left of the screenshot button


root.after(100, read_from_arduino)
root.mainloop()
