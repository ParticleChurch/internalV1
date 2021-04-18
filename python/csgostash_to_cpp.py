data = eval(open("csgostash_output.txt", 'rb').read().decode('utf-8'))

paintkit_names = {}
output = {}
doppler = []
gammaDoppler = []

dopplerIds = {
    852: "Doppler Phase 1",
    853: "Doppler Phase 2",
    854: "Doppler Phase 3",
    855: "Doppler Phase 4",
    415: "Ruby Doppler",
    416: "Sapphire Doppler",
    417: "Black Pearl Doppler",
}
gammaDopplerIds = {
    569: "Gamma Doppler Phase 1",
    570: "Gamma Doppler Phase 2",
    571: "Gamma Doppler Phase 3",
    572: "Gamma Doppler Phase 4",
    568: "Emerald Gamma Doppler",
}

for d in data:
    if d['paintkit'] is None and "doppler" in d['skin'].lower():
        if "gamma" in d['skin'].lower():
            gammaDoppler.append(d['weapon'])
        else:
            doppler.append(d['weapon'])
        continue
    
    if not d['paintkit'] in output:
        output[d['paintkit']] = []
        paintkit_names[d['paintkit']] = d['skin']
    output[d['paintkit']].append(d['weapon'])
    if paintkit_names[d['paintkit']] != d['skin']:
        print("="*10 + f" PAINTKIT {d['paintkit']} NAME MISMATCH " + "="*10)
        paintkit_names[d['paintkit']] = d['skin']

for id in dopplerIds:
    paintkit_names[id] = dopplerIds[id]
for id in gammaDopplerIds:
    paintkit_names[id] = gammaDopplerIds[id]

for d in doppler:
    for id in dopplerIds:
        if not id in output:
            output[id] = []
        output[id].append(d)
for d in gammaDoppler:
    for id in gammaDopplerIds:
        if not id in output:
            output[id] = []
        output[id].append(d)

sorted_output = []
for id in output:
    weapons = []
    knives = []
    gloves = []
    for v in output[id]:
        if v.lower().startswith("weapon"):
            weapons.append(v)
        elif v.lower().startswith("knife"):
            knives.append(v)
        elif v.lower().startswith("glove"):
            gloves.append(v)
    sorted_output.append((id, weapons, knives, gloves))
sorted_output.sort(key=lambda x: x[0])

'''
with open("csgostash_cpp_defs.txt", "w+") as f:
    pass
for (id, weapons, knives, gloves) in sorted_output:
    s = ' ' * 4
    if len(weapons) > 0:
        s += 'constexpr Weapon __pkw%d[]{ %sWeapon::INVALID }; ' % (id, ", ".join(weapons) + ', ')
    else:
        s += 'constexpr auto __pkw%d = __pkw_empty; ' % id
        
    if len(knives) > 0:
        s += 'constexpr Knife __pkk%d[]{ %sKnife::INVALID }; ' % (id, ", ".join(knives) + ', ')
    else:
        s += 'constexpr auto __pkk%d = __pkk_empty; ' % id
        
    if len(gloves) > 0:
        s += 'constexpr Glove __pkg%d[]{ %sGlove::INVALID };' % (id, ", ".join(gloves) + ', ')
    else:
        s += 'constexpr auto __pkg%d = __pkg_empty;' % id
        
    with open("csgostash_cpp_defs.txt", "a") as f:
        f.write(s + "\n")
'''

fstring = ' '* 8 + 'PaintKit(%d, "%s", __pkw%d, __pkk%d, __pkg%d),'
with open("csgostash_cpp_listelements.txt", "w+") as f:
    pass
for (id, weapons, knives, gloves) in sorted_output:
    with open("csgostash_cpp_listelements.txt", "a") as f:
        f.write(fstring % (id, paintkit_names[id], id, id, id) + "\n")
