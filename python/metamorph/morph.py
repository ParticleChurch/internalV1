filename = r"E:\GitHub\internalV1\Release\CSGOCollabV1.dll"

import radare as r2
p = r2.PEParser(filename, import_parsedata = False)
