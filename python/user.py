import os
from valve import parse as vparse

FFFFF=open("log.txt", "wb+")
oprint = print
def print(*x,end="\n"):
    oprint(*x,end=end)
    #FFFFF.write((' '.join(str(y) for y in x) + end).encode('utf-16'))

csgo_path = 'C:/Program Files (x86)/Steam/steamapps/common/Counter-Strike Global Offensive/'
game_items_filepath = os.path.join(csgo_path, 'csgo/scripts/items/items_game.txt')
resource_filepath = os.path.join(csgo_path, 'csgo/resource/csgo_english.txt')

game_items_string = ''
with open(game_items_filepath, 'r') as f:
    game_items_string = f.read()

game_items = vparse(game_items_string)[0][-1]

paint_kits_raw = {}

for (key, value) in game_items:
    if key == "paint_kits":
        for (key2, value2) in value:
            if key2 in paint_kits_raw:
                print("overlapping pain kit at id =", key2)
            paint_kits_raw[key2] = value2

paint_kits = []
for id in paint_kits_raw:
    raw = paint_kits_raw[id]
    data = {"id":int(id), 'visibleName': '-'}
    
    if data['id'] == 9001: # workshop default
        continue
    
    for (key, value) in raw:
        if key == 'name':
            data[key] = value
        elif key == 'description_tag':
            data[key] = (value[1:] if value.startswith("#") else value).lower()
    
    if 'name' in data and 'description_tag' in data:
        paint_kits.append(data)
    else:
        print("================ WARNING: FAULTY PAINTKIT w/ ID =", id)

resource_str = ''
with open(resource_filepath, 'rb') as f:
    resource_str = f.read().decode('utf-16')
    
resource = vparse(resource_str)[0][1][1][1]

resource_dict = {}
for (key, value) in resource:
    resource_dict[key.lower()] = value

for pk in paint_kits:
    dt = pk['description_tag']
    if dt in resource_dict:
        tag = resource_dict[dt]
        pk['visibleName'] = tag
    else:
        print("========== WARNING: FAILED TO FIND description_tag OF", dt, "FOR ID", pk['id'])

item_sets_raw = []
for (key, value) in game_items:
    if key == "item_sets":
        item_sets_raw.append(value)
        
cases_raw = []
for isr in item_sets_raw:
    for (key,value) in isr:
        data = {}
        for (key2, value2) in value:
            if key2 == "name":
                data[key2] = (value2[1:] if value2.startswith("#") else value2).lower()
            elif key2 == "items":
                data[key2] = value2
        if 'name' in data and 'items' in data:
            cases_raw.append(data)
        else:
            print("========== WARNING: INVALID CASE", key)

items = {}
for caser in cases_raw:
    for (item, _) in caser['items']:
        if '[' in item and ']' in item:
            skin = item[item.index("[")+1:item.index("]")]
            weapon = item[item.index("]")+1:]
            if skin in items:
                items[skin].append(weapon)
            else:
                items[skin]=[weapon]

for pk in paint_kits:
    if pk['name'] in items:
        pk['apply_to'] = items[pk['name']]
    print(pk)