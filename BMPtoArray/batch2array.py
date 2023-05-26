'''

    This script takes in a bitmap and outputs a text file that is a
    byte array used in Arduino header files.

    You'll need python 2.7

    usage: python bmp2array.py macaque.bmp
    output: output.txt
        Copy and paste this content into a image.h file that
        is called in an Arduino sketch.
        For an example see: https://github.com/sparkfun/SparkFun_SSD1320_OLED_Arduino_Library/tree/master/examples/Example2_Graphics

    This is helpful if you need to display a static bitmap on a
    grayscale OLED display (like the SparkFun flexible OLED).

    This script converts the image to 4-bit grayscale. This means a
    normal 4-byte color is changed to one byte (the blue channel)
    then the last four bits are cut off and combined with the next
    4-byte color to create one 8-bit byte (containing two
    grayscale pixels).

'''

#Various inspiration from
#http://www.dragonwins.com/domains/getteched/bmp/bmpfileformat.htm
#https://github.com/yy502/ePaperDisplay
import os
import sys
import struct

#Open our input file which is defined by the first commandline argument
#then dump it into a list of bytes
folder = sys.argv[1] # get the folder path from command line argument
folder_name = os.path.basename(folder)
outputString = ""
skipped = 0
for frame, filename in enumerate(os.listdir(folder)):
    file_path = os.path.join(folder, filename)
    if os.path.isfile(file_path) and filename.endswith(".bmp"):

        print(file_path) # print the file path

        infile = open(file_path,"rb") #b is for binary
        contents = bytearray(infile.read())
        infile.close()

        #Get the size of this image
        data = [contents[2], contents[3], contents[4], contents[5]]
        fileSize = struct.unpack_from('<i', contents, 2)

        print("Size of file:", fileSize[0])

        #Get the header offset amount
        data = [contents[10], contents[11], contents[12], contents[13]]
        #offset = struct.unpack("I", bytearray(data))
        offset = struct.unpack_from('<i', contents, 10)

        print("Offset:", offset[0])

        #Get the number of colors used
        data = [contents[46], contents[47], contents[48], contents[49]]
        colorsUsed = struct.unpack("I", bytearray(data))

        print("Number of colors used:", colorsUsed[0])

        #Get image width and height
        imgWidth = struct.unpack_from('<i', contents, 18)
        imgHeight = struct.unpack_from('<i', contents, 22)

        print("Image width:", imgWidth[0])
        print("Image height:", imgHeight[0])


        #Create color definition array and init the array of color values
        colorIndex = bytearray(colorsUsed[0])
        for i in range(colorsUsed[0]):
            colorIndex.append(0)

        #Assign the colors to the array
        startOfDefinitions = 54
        for i in range(colorsUsed[0]):    
            colorIndex[i] = contents[startOfDefinitions + (i * 4)]

        #Print the color definitions
        #for i in range(colorsUsed[0]):    
            #print hexlify(colorIndex[i])

        #Start coverting spots to values
        #Start at the offset and go to the end of the file
        #for i in range(fileSize[0] - 1, offset[0], -1):
        image_data = [0]* fileSize[0]
        image_data_unpadded = [0]* imgWidth[0] * imgHeight[0]
        image_data_flipped = [0]* imgWidth[0] * imgHeight[0]

        for i in range(offset[0],fileSize[0], 1):
            image_data[i - offset[0]] = contents[i] 


        padded_width = int((fileSize[0] - offset [0]) / imgHeight[0])
        reminder = 0
        if (imgWidth[0] % 4 != 0):
            reminder = 4 - (imgWidth[0] % 4)
        print("Unpadded width: ", padded_width)    
        print("Padding: ", reminder)    
        for y in range(imgHeight[0]):
            for x in range(padded_width - reminder):
                image_data_unpadded[(y*imgWidth[0]) + x] = image_data[(y*padded_width) + x]



        for y in range(imgHeight[0]):
            for x in range(imgWidth[0]):
                flipped_y = imgHeight[0]-1-y
                image_data_flipped[flipped_y*imgWidth[0] + x] = image_data_unpadded[y*imgWidth[0] + x]



        #Make a string to hold the output of our script
        arraySize = (fileSize[0]- offset[0]) - (imgHeight[0] * reminder)

        print(arraySize)


        #outputString = "/* This was generated using the SparkFun BMPtoArray python script" + '\r'
        #outputString += " See https://github.com/sparkfun/BMPtoArray for more info */" + '\r\r'
        if frame - skipped == 0:
            outputString += "#include <Arduino.h>" + '\r'
            outputString += "static const uint8_t " + folder_name + " [][" + str(arraySize) + "] PROGMEM = {" + '\r'
        outputString += '\r' + "{"


        for i in range(imgWidth[0] * imgHeight[0]):

            colorCode = image_data_flipped[i]
            actualColor = colorIndex[colorCode] #Look up this code in the table

            #Take two bytes, squeeze them to 4 bits
            #Then combine them into one byte
            compressedByte = actualColor >> 4

            #Add this value to the string
            outputString += hex(compressedByte) + ", "

            #Break the array into a new line every 8 entries
            if i % imgWidth[0] == 0:
                outputString += '\r'
            
        #Once we've reached the end of our input string, pull the last two
        #characters off (the last comma and space) since we don't need
        #them. Top it off with a closing bracket and a semicolon.
        outputString = outputString[:-2]
        outputString += "},"

        #Write the output string to our output file
    else:
        skipped += 1;
outputString += "};"

outfile = open("output.h","w")
outfile.write(outputString)
outfile.close()

print("output.h complete")
