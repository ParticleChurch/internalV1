import selenium
from selenium import webdriver
from bs4 import BeautifulSoup as bs
driver = webdriver.Chrome("chromedriver.exe")

def get_paintkit_id(url):
    x = "skin-misc-details"
    print("get_paintkit_id:", url)
    driver.get(url)
    html = driver.page_source
    soup = bs(html, features="lxml")
    ps = soup.select('.skin-misc-details')[0].findChildren("p", recursive=False)
    for p in ps:
        label = p.findChildren("strong")[0].get_text(strip = True)
        if "finish catalog" in label.lower():
            id = p.findChildren("span")[0].get_text(strip = True)
            return int(id)

def get_weapons(url):
    weapons = []
    
    print("get_weapons:", url)
    driver.get(url)
    html = driver.page_source
    soup = bs(html, features="lxml")
    images = soup.select('.img-responsive')
    for img in images:
        if img.parent.name != "a":
            continue
        link = img.parent
        skinurl = link.attrs['href']
        if not skinurl.startswith("https://csgostash.com/skin/"):
            continue
        
        weapon_html = link.parent
        h3 = weapon_html.findChildren("h3", recursive=False)[0]
        skinName = h3.get_text(strip = "true")
        weapons.append({"paintkit": get_paintkit_id(skinurl), "skin": skinName, "link": skinurl})
    return weapons

BRUH = [
    # guns
    ("Glock-18", "Weapon::Glock18"),
    ("P2000", "Weapon::P2000"),
    ("USP-S", "Weapon::USPS"),
    ("Dual+Berettas", "Weapon::DualBerettas"),
    ("P250", "Weapon::P250"),
    ("Tec-9", "Weapon::Tec9"),
    ("Five-SeveN", "Weapon::FiveSeven"),
    ("CZ75-Auto", "Weapon::CZ75"),
    ("Desert+Eagle", "Weapon::DesertEagle"),
    ("R8+Revolver", "Weapon::R8Revolver"),
    ("Nova", "Weapon::Nova"),
    ("XM1014", "Weapon::XM1014"),
    ("Sawed-Off", "Weapon::SawedOff"),
    ("MAG-7", "Weapon::MAG7"),
    ("M249", "Weapon::M249"),
    ("Negev", "Weapon::Negev"),
    ("MP9", "Weapon::MP9"),
    ("MAC-10", "Weapon::MAC10"),
    ("MP7", "Weapon::MP7"),
    ("MP5-SD", "Weapon::MP5"),
    ("UMP-45", "Weapon::UMP45"),
    ("P90", "Weapon::P90"),
    ("PP-Bizon", "Weapon::PPBizon"),
    ("FAMAS", "Weapon::FAMAS"),
    ("Galil+AR", "Weapon::Galil"),
    ("M4A4", "Weapon::M4A4"),
    ("AK-47", "Weapon::AK47"),
    ("M4A1-S", "Weapon::M4A1S"),
    ("SSG+08", "Weapon::SSG08"),
    ("AUG", "Weapon::AUG"),
    ("SG+553", "Weapon::SG553"),
    ("AWP", "Weapon::AWP"),
    ("SCAR-20", "Weapon::SCAR20"),
    ("G3SG1", "Weapon::G3SG1"),
    
    # knives
]

with open("csgostash_output.txt", "w") as f:
    f.write("[\n")
    for (url_weapon_name, weapon_name) in BRUH:
        weapons = get_weapons("https://csgostash.com/weapon/" + url_weapon_name)
        for w in weapons:
            w['weapon'] = weapon_name
            s = " "*4 + str(w) + ", \n"
            s = s.encode('ascii', 'backslashreplace').decode('ascii')
            print(s, end = '')
            f.write(s)
    f.write("]")

driver.close()