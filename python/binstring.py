from tkinter import Tk as tk
from tkinter.filedialog import askopenfilename
import time
import os

w = 0
try:
    w = int(input("chars per line? (0 for wrap): "))
except:
    pass

tk().withdraw()
filename = askopenfilename(initialdir = os.path.dirname(__file__))

with open(filename, "rb") as f:
    nBytes = 0
    if w <= 0:
        print('const char* text = "', end = '')
        while bytes:=f.read(128):
            x = ''
            for b in bytes:
                h = hex(b)[2:]
                while len(h) < 2:
                    h = '0' + h
                x += "\\x" + h
            nBytes += len(bytes)
            print(x, end = '')
        print('";')
    else:
        print('const char* text =', end = '')
        while bytes := f.read(w):
            x = ' \\\n    "'
            for b in bytes:
                h = hex(b)[2:]
                while len(h) < 2:
                    h = '0' + h
                x += "\\x" + h
            nBytes += len(bytes)
            print(x, end = '"')
    print(f";\nsize_t nBytes = {nBytes}; // the number of bytes in the file, not including the NUL added to the end of string literals")