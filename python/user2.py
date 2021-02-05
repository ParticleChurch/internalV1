import valve
import os

csgo_path = 'C:/Program Files (x86)/Steam/steamapps/common/Counter-Strike Global Offensive/'
game_items_filepath = os.path.join(csgo_path, 'csgo/scripts/items/items_game.txt')
resource_filepath = os.path.join(csgo_path, 'csgo/resource/csgo_english.txt')

path = resource_filepath

file_str = ''
with open(path, 'rb') as f:
    file_str = f.read().decode('utf-16')

file_contents = valve.parse(file_str)
valve.export(file_contents, "__EXPORT/resource")
