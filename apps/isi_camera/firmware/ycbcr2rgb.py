import sys
import png
import struct

if len(sys.argv) != 4:
    print("Usage: python ycbcr2rgb {image name}.yuv {width} {height}")
    sys.exit(-1)

name = sys.argv[1]
width = int(sys.argv[2])
height = int(sys.argv[3])

if name[-3:] != 'yuv':
    print("\"" + name + "\" is not a yuv file.")

imgData = []
imgDataLength = 0
img = []

def clamp(val, min, max):
    if val < min:
        val = min

    if val > max:
        val = max

    return int(val)

def ycbcr2rgb(y, cb, cr):
    Y = float(y)
    Cb = float(cb)
    Cr = float(cr)

    r = int((Y + 1.40200 * (Cr - 0x80)))
    g = int((Y - 0.34414 * (Cb - 0x80) - 0.71414 * (Cr - 0x80)))
    b = int((Y + 1.77200 * (Cb - 0x80)))

    r = clamp(r, 0, 255)
    g = clamp(g, 0, 255)
    b = clamp(b, 0, 255)

    return [r, g, b]

def readRow(file):
    row = []

    halfWidth = int(width / 2)

    for i in range(0, halfWidth):
        cr = struct.unpack('B', infile.read(1))[0]
        y1 = struct.unpack('B', infile.read(1))[0]
        cb = struct.unpack('B', infile.read(1))[0]
        y0 = struct.unpack('B', infile.read(1))[0]

        rgba = ycbcr2rgb(y0, cb, cr)

        row.append(rgba[0])
        row.append(rgba[1])
        row.append(rgba[2])

        rgba = ycbcr2rgb(y1, cb, cr)

        row.append(rgba[0])
        row.append(rgba[1])
        row.append(rgba[2])

    return row

with open(name, 'rb') as infile:
    for i in range(0, height):
        row = readRow(infile)

        imgData.append(row)

pngWriter = png.Writer(width=width, height=height, bitdepth=8, greyscale=False)
outName = name.replace(".yuv", ".png")
outFile = open(outName, "wb")
pngWriter.write(outFile, imgData)
outFile.close()
