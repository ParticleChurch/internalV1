x = None
with open(r"E:\GitHub\internalV1\Release\Config.cod", "r") as f:
    x = f.readlines()
    
def is_machine(l):
    if len(l) < len("  00000 00"): return False
    if not l.startswith("  "): return False
    try:
        n = int(l[2:7], 16)
    except BaseException as e:
        return False
    return True

log = [
    
]

w = 0
for l in x:
    w += 1
    if not is_machine(l): continue;
    l = l.strip()
    n = int(l[:5], 16)
    
    if n == 0:
        log.append([w, 0, l, "unknown"])
    elif len(log) > 0:
        log[-1][1] = n
        log[-1][-1] = l

#log.sort(key = lambda x: x[1], reverse = True)

print("Ctrl+F in the .cod file for \"Function, COMDAT\"")
print("and find the first instruction after that. That's")
print("where the function 'starts'.")
print(" Line number | function size")
print("-------------+--------------")
for [line_number, size, begin, end] in log:
    print(" %9d   |   %9d" % (line_number, size))
    
input("DONE!")