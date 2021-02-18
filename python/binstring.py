from tkinter import Tk as tk
from tkinter.filedialog import askopenfilename
import time
import os

tk().withdraw()
filename = askopenfilename(initialdir = os.path.dirname(__file__))

print('const char* text = "', end = '')
i = 0
with open(filename, "rb") as f:
    while bytes:=f.read(128):
        x = ''.join([repr(chr(x))[1:-1] if x != ord('"') else '\\"' for x in bytes])
        i += len(bytes)
        print(x, end = '')
print('";')
print(f"size_t nBytes = {i}; // the number of bytes in the file, not including the NUL added to the end of string literals")