import os
import metame.r2parser as r2parser
from tkinter import Tk as tk
from tkinter.filedialog import askopenfilename
from tkinter.filedialog import asksaveasfilename
import argparse
import shutil

tk().withdraw()
print("input file: ", end="")
infile = r"E:/GitHub/internalV1/Release/CSGOCollabV1.dll" #askopenfilename(initialdir = os.path.dirname(__file__))
print(infile)
print("output file: ", end="")
outfile = r"E:/GitHub/internalV1/Release/metamorph.dll" #asksaveasfilename(initialdir = os.path.dirname(__file__))
print(outfile)

db = True #input("Debug [Y/N]: ")[0] in "yYtT"
print("Debugging Enabled" if db else "Debugging Disabled")

f = True #input("Force [Y/N]: ")[0] in "yYtT"
print("Force Enabled" if db else "Force Disabled")


r = r2parser.R2Parser(infile, True, debug=db, force_replace=f)
patches = r.iterate_fcn()
r.close()

shutil.copy(infile, outfile)

r = r2parser.R2Parser(outfile, False, debug=db, write=True)
r.patch_binary(patches)
r.close()