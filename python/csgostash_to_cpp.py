data = eval(open("csgostash_output.txt", 'rb').read().decode('utf-8'))

paintkit_names = {}
output = {}

for d in data:
    if not d['paintkit'] in output:
        output[d['paintkit']] = []
        paintkit_names[d['paintkit']] = d['skin']
    output[d['paintkit']].append(d['weapon'])
    if paintkit_names[d['paintkit']] != d['skin']:
        print("="*10 + f" PAINTKIT {d['paintkit']} NAME MISMATCH " + "="*10)
        paintkit_names[d['paintkit']] = d['skin']

sorted_output = []
for id in output:
    sorted_output.append((id, output[id]))
sorted_output.sort(key=lambda x: x[0])

fstring = ' '* 8 + 'PaintKit(%d, XOR("%s"), %s),'
for (id, weapons) in sorted_output:
    print(fstring % (id, paintkit_names[id], ' | '.join(["(1ull << (uint64_t)%s)" % w for w in weapons])))