import r2pipe
import json

class PEParser:
    def __init__(self, filename, import_parsedata = False):
        if not import_parsedata:
            self.dllfile = filename
            self.r2in = r2pipe.open(filename, [])
            self.file_info = json.loads(self.r2in.cmd("ij"))
            
            # check file type
            if not "bin" in self.file_info:
                raise TypeError("This is not a PE file.")
            self.archtype = "%s bit %s" % (self.file_info['bin']['bits'], self.file_info['bin']['arch'])
            if self.archtype != "32 bit x86":
                raise TypeError("Unsupported architecture. %s instead of 32 bit x86" % self.archtype)
            
            # parse functions
            self.r2in.cmd("aaa")
            self.entries = json.loads(self.r2in.cmd("aflj"))
            self.fcns = []
            for entry in self.entries:
                if entry['type'] != "fcn":
                    continue
                f = json.loads(self.r2in.cmd("pdfj @%s" % entry["name"]))
                self.fcns.append(f)
                print(entry)
                return
        else:
            with open(filename, "r") as f:
                d = json.loads(f.read())
                self.file_info = d['file_info']
                self.dllfile = self.file_info['core']['file']
                self.entries = d['entries']
                self.fcns = d['fcns']
    
    def export_parsedata(self, filename):
        with open(filename, "w+") as f:
            f.write(json.dumps({"file_info": self.file_info, "entries": self.entries, "fcns": self.fcns}, indent=4))
    
    def __str__(self):
        s = json.dumps(self.file_info, indent = 4)
        s += "\n" + "=" * 20 + "\n"
        s += json.dumps(self.entries, indent = 4)
        return s