f1 = r"E:/GitHub/internalV1/Release/CSGOCollabV1.dll"
f2 = r"E:/GitHub/internalV1/Release/metamorph.dll"

one = open(f1, "rb")
two = open(f2, "rb")

L = [] # same is negative, diff is positive
diff = 0
same = 0
while (b1:=one.read(128)) and (b2:=two.read(128)):
    for (bb1, bb2) in zip(b1, b2):
        if bb1 == bb2:
            same += 1
            if diff >= 1:
                L.append(diff)
                diff = 0
        else:
            diff += 1
            if same >= 1:
                L.append(-same)
                same = 0

s = sorted(L)
print("unchanged sequences:", [-x for x in s[:10]])
print("changed sequences:", [*reversed(s[-10:])])
print("Total bytes changed: %d" % sum([q for q in s if q > 0]))
print("Total bytes unchanged: %d" % sum([-q for q in s if q < 0]))
print(s.count(3))
one.close()
two.close()