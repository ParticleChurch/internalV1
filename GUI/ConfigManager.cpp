#include "ConfigManager.hpp"
#define MULTILINE(...) #__VA_ARGS__

std::vector<ConfigManager::ConfigData> ConfigManager::Configs = {
	ConfigManager::ConfigData{
		false,
		"Everything Disabled",
R"(
# PARTICLE.CHURCH config generated on mm/dd/yyyy @ hh:mm:ss
# For help making a config, please visit https://particle.church/help/config
# 
# The format for this file is as follows,
# and is explained in greater detail at above link:
# property-name : value; keybind_type @ key_name



#############
## Offense ##
#############

## Aimbot
enable-aimbot                   :: false
enable-silentaim                :: true
aimbot-autoshoot                :: true
aimbot-autowall                 :: true
aimbot-aimstep                  :: 0
aimbot-pistol-smoothing-method  :: None
aimbot-pistol-smoothing-amount  :: 0.0
aimbot-pistol-fov               :: 0.0
aimbot-pistol-hitbox            :: [None]
aimbot-pistol-hitbox-priority   :: Head
aimbot-pistol-mindamage-visible :: 0
aimbot-pistol-mindamage-hidden  :: 0
aimbot-pistol-hitchance         :: 0
aimbot-pistol-if-lethal         :: true
aimbot-smg-smoothing-method     :: None
aimbot-smg-smoothing-amount     :: 0.0
aimbot-smg-fov                  :: 0.0
aimbot-smg-hitbox               :: [None]
aimbot-smg-hitbox-priority      :: Head
aimbot-smg-mindamage-visible    :: 0
aimbot-smg-mindamage-hidden     :: 0
aimbot-smg-hitchance            :: 0
aimbot-smg-if-lethal            :: true
aimbot-heavy-smoothing-method   :: None
aimbot-heavy-smoothing-amount   :: 0.0
aimbot-heavy-fov                :: 0.0
aimbot-heavy-hitbox             :: [None]
aimbot-heavy-hitbox-priority    :: Head
aimbot-heavy-mindamage-visible  :: 0
aimbot-heavy-mindamage-hidden   :: 0
aimbot-heavy-hitchance          :: 0
aimbot-heavy-if-lethal          :: true
aimbot-rifle-smoothing-method   :: None
aimbot-rifle-smoothing-amount   :: 0.0
aimbot-rifle-fov                :: 0.0
aimbot-rifle-hitbox             :: [None]
aimbot-rifle-hitbox-priority    :: Head
aimbot-rifle-mindamage-visible  :: 0
aimbot-rifle-mindamage-hidden   :: 0
aimbot-rifle-hitchance          :: 0
aimbot-rifle-if-lethal          :: true
aimbot-scout-smoothing-method   :: None
aimbot-scout-smoothing-amount   :: 0.0
aimbot-scout-fov                :: 0.0
aimbot-scout-hitbox             :: [None]
aimbot-scout-hitbox-priority    :: Head
aimbot-scout-mindamage-visible  :: 0
aimbot-scout-mindamage-hidden   :: 0
aimbot-scout-hitchance          :: 0
aimbot-scout-if-lethal          :: true
aimbot-awp-smoothing-method     :: None
aimbot-awp-smoothing-amount     :: 0.0
aimbot-awp-fov                  :: 0.0
aimbot-awp-hitbox               :: [None]
aimbot-awp-hitbox-priority      :: Head
aimbot-awp-mindamage-visible    :: 0
aimbot-awp-mindamage-hidden     :: 0
aimbot-awp-hitchance            :: 0
aimbot-awp-if-lethal            :: true
aimbot-auto-smoothing-method    :: None
aimbot-auto-smoothing-amount    :: 0.0
aimbot-auto-fov                 :: 0.0
aimbot-auto-hitbox              :: [None]
aimbot-auto-hitbox-priority     :: Head
aimbot-auto-mindamage-visible   :: 0
aimbot-auto-mindamage-hidden    :: 0
aimbot-auto-hitchance           :: 0
aimbot-auto-if-lethal           :: true

## Triggerbot
enable-triggerbot       :: true
triggerbot-through-wall :: true
triggerbot-delay        :: 0
triggerbot-magnet       :: true

## Other
backtracking-time :: 0



#############
## Defense ##
#############

## Fake Lag
antiaim-fakelag-tick     :: 3
antiaim-fakelag-distance :: 64

## Legit
antiaim-legit-enable    :: false
antiaim-legit-max-angle :: 0
antiaim-legit-invert    :: [error]

## Rage
antiaim-rage-enable      :: false
antiaim-rage-pitch       :: Up
antiaim-rage-real        :: 0
antiaim-rage-real-jitter :: 0
antiaim-rage-fake        :: 0
antiaim-rage-fake-jitter :: 100.0
antiaim-rage-invert      :: [error]
antiaim-rage-fake-invert :: false

## Visuals
antiaim-visual-fake         :: false
antiaim-visual-fake-lag     :: false
antiaim-visual-fake-color   :: (0, 150, 255)
antiaim-visual-fake-opacity :: 100.0
antiaim-visual-real         :: false
antiaim-visual-real-color   :: (0, 150, 255)
antiaim-visual-real-opacity :: 100.0



#############
## Visuals ##
#############

## Chams
visuals-chams-enemy-visible-enable    :: false
visuals-chams-enemy-visible-color     :: (0, 150, 255)
visuals-chams-enemy-visible-material  :: Normal
visuals-chams-enemy-hidden-enable     :: false
visuals-chams-enemy-hidden-color      :: (0, 150, 255)
visuals-chams-enemy-hidden-material   :: Normal
visuals-chams-friend-visible-enable   :: false
visuals-chams-friend-visible-color    :: (0, 150, 255)
visuals-chams-friend-visible-material :: Normal
visuals-chams-friend-hidden-enable    :: false
visuals-chams-friend-hidden-color     :: (0, 150, 255)
visuals-chams-friend-hidden-material  :: Normal

## ESP
visuals-esp-other-grenadebox               :: false
visuals-esp-other-weapon-box               :: false
visuals-esp-other-weapon-name              :: false
visuals-esp-enemy-enable                   :: false
visuals-esp-enemy-bbox                     :: false
visuals-esp-enemy-bbox-color               :: (255, 255, 255)
visuals-esp-enemy-name                     :: false
visuals-esp-enemy-name-color               :: (255, 255, 255)
visuals-esp-enemy-snapline                 :: false
visuals-esp-enemy-snapline-color           :: (255, 255, 255)
visuals-esp-enemy-skeleton                 :: false
visuals-esp-enemy-skeleton-color           :: (255, 255, 255)
visuals-esp-enemy-health                   :: false
visuals-esp-enemy-health-color             :: (0, 255, 0)
visuals-esp-enemy-health-color-background  :: (255, 0, 0)
visuals-esp-enemy-ammo                     :: false
visuals-esp-enemy-ammo-color               :: (0, 0, 255)
visuals-esp-enemy-ammo-color-background    :: (100, 100, 100)
visuals-esp-friend-enable                  :: false
visuals-esp-friend-bbox                    :: false
visuals-esp-friend-bbox-color              :: (255, 255, 255)
visuals-esp-friend-name                    :: false
visuals-esp-friend-name-color              :: (255, 255, 255)
visuals-esp-friend-snapline                :: false
visuals-esp-friend-snapline-color          :: (255, 255, 255)
visuals-esp-friend-skeleton                :: false
visuals-esp-friend-skeleton-color          :: (255, 255, 255)
visuals-esp-friend-health                  :: false
visuals-esp-friend-health-color            :: (0, 255, 0)
visuals-esp-friend-health-color-background :: (255, 0, 0)
visuals-esp-friend-ammo                    :: false
visuals-esp-friend-ammo-color              :: (0, 0, 255)
visuals-esp-friend-ammo-color-background   :: (100, 100, 100)

## World
visuals-world-enable        :: false
visuals-world-color         :: (0, 150, 255)
visuals-world-opacity       :: 0
visuals-world-prop-enable   :: false
visuals-world-prop-color    :: (0, 150, 255)
visuals-world-prop-opacity  :: 0
visuals-world-skybox-enable :: false
visuals-world-skybox-name   :: cs_baggage_skybox_
visuals-world-skybox-color  :: (0, 150, 255)

## Misc
visuals-misc-thirdperson     :: false
visuals-misc-vote-indicator  :: false
visuals-misc-spec-list       :: false
visuals-misc-teamdamage-list :: false



##########
## Misc ##
##########

## Movement
misc-movement-bhop           :: false
misc-movement-slowwalk       :: false
misc-movement-slowwalk-speed :: 0
misc-movement-airstuck       :: false
misc-movement-fastcrouch     :: false
misc-movement-fakeduck       :: false
misc-movement-autostrafe     :: None

## Other
misc-other-autoaccept     :: false
misc-other-killsay        :: false
misc-other-killsay-input  :: "Get pwnd by particle.church!"
misc-other-fullautopistol :: false
misc-other-clantag-input  :: ""



##########
## Menu ##
##########

## Menu
show-menu       : toggle @ insert : true
menu-complexity :: Professional
show-help-link  :: true
show-watermark  :: true

## Colors
menu-text-color                       :: (255, 255, 255)
menu-eject-color                      :: (200, 75, 75)
menu-background-color1                :: (30, 30, 30)
menu-background-color2                :: (20, 20, 20)
menu-background-color3                :: (50, 50, 50)
menu-background-color4                :: (75, 75, 75)
menu-option-selector-background-color :: (20, 20, 20)
menu-option-selector-active-color     :: (60, 60, 60)
menu-option-selector-text-color       :: (255, 255, 255)
menu-option-color1                    :: (230, 230, 230)
menu-option-color2                    :: (0, 150, 255)

## Opacities

)"
	},
	ConfigManager::ConfigData{
		false,
		"Default Legit",
R"(
# PARTICLE.CHURCH config generated on mm/dd/yyyy @ hh:mm:ss
# For help making a config, please visit https://particle.church/help/config
# 
# The format for this file is as follows,
# and is explained in greater detail at above link:
# property-name : value; keybind_type @ key_name



#############
## Offense ##
#############

## Aimbot
enable-aimbot                   :: false
enable-silentaim                :: true
aimbot-autoshoot                :: true
aimbot-autowall                 :: true
aimbot-aimstep                  :: 0
aimbot-pistol-smoothing-method  :: None
aimbot-pistol-smoothing-amount  :: 0.0
aimbot-pistol-fov               :: 0.0
aimbot-pistol-hitbox            :: [None]
aimbot-pistol-hitbox-priority   :: Head
aimbot-pistol-mindamage-visible :: 0
aimbot-pistol-mindamage-hidden  :: 0
aimbot-pistol-hitchance         :: 0
aimbot-pistol-if-lethal         :: true
aimbot-smg-smoothing-method     :: None
aimbot-smg-smoothing-amount     :: 0.0
aimbot-smg-fov                  :: 0.0
aimbot-smg-hitbox               :: [None]
aimbot-smg-hitbox-priority      :: Head
aimbot-smg-mindamage-visible    :: 0
aimbot-smg-mindamage-hidden     :: 0
aimbot-smg-hitchance            :: 0
aimbot-smg-if-lethal            :: true
aimbot-heavy-smoothing-method   :: None
aimbot-heavy-smoothing-amount   :: 0.0
aimbot-heavy-fov                :: 0.0
aimbot-heavy-hitbox             :: [None]
aimbot-heavy-hitbox-priority    :: Head
aimbot-heavy-mindamage-visible  :: 0
aimbot-heavy-mindamage-hidden   :: 0
aimbot-heavy-hitchance          :: 0
aimbot-heavy-if-lethal          :: true
aimbot-rifle-smoothing-method   :: None
aimbot-rifle-smoothing-amount   :: 0.0
aimbot-rifle-fov                :: 0.0
aimbot-rifle-hitbox             :: [None]
aimbot-rifle-hitbox-priority    :: Head
aimbot-rifle-mindamage-visible  :: 0
aimbot-rifle-mindamage-hidden   :: 0
aimbot-rifle-hitchance          :: 0
aimbot-rifle-if-lethal          :: true
aimbot-scout-smoothing-method   :: None
aimbot-scout-smoothing-amount   :: 0.0
aimbot-scout-fov                :: 0.0
aimbot-scout-hitbox             :: [None]
aimbot-scout-hitbox-priority    :: Head
aimbot-scout-mindamage-visible  :: 0
aimbot-scout-mindamage-hidden   :: 0
aimbot-scout-hitchance          :: 0
aimbot-scout-if-lethal          :: true
aimbot-awp-smoothing-method     :: None
aimbot-awp-smoothing-amount     :: 0.0
aimbot-awp-fov                  :: 0.0
aimbot-awp-hitbox               :: [None]
aimbot-awp-hitbox-priority      :: Head
aimbot-awp-mindamage-visible    :: 0
aimbot-awp-mindamage-hidden     :: 0
aimbot-awp-hitchance            :: 0
aimbot-awp-if-lethal            :: true
aimbot-auto-smoothing-method    :: None
aimbot-auto-smoothing-amount    :: 0.0
aimbot-auto-fov                 :: 0.0
aimbot-auto-hitbox              :: [None]
aimbot-auto-hitbox-priority     :: Head
aimbot-auto-mindamage-visible   :: 0
aimbot-auto-mindamage-hidden    :: 0
aimbot-auto-hitchance           :: 0
aimbot-auto-if-lethal           :: true

## Triggerbot
enable-triggerbot       :: true
triggerbot-through-wall :: true
triggerbot-delay        :: 0
triggerbot-magnet       :: true

## Other
backtracking-time :: 0



#############
## Defense ##
#############

## Fake Lag
antiaim-fakelag-tick     :: 3
antiaim-fakelag-distance :: 64

## Legit
antiaim-legit-enable    :: false
antiaim-legit-max-angle :: 0
antiaim-legit-invert    :: [error]

## Rage
antiaim-rage-enable      :: false
antiaim-rage-pitch       :: Up
antiaim-rage-real        :: 0
antiaim-rage-real-jitter :: 0
antiaim-rage-fake        :: 0
antiaim-rage-fake-jitter :: 100.0
antiaim-rage-invert      :: [error]
antiaim-rage-fake-invert :: false

## Visuals
antiaim-visual-fake         :: false
antiaim-visual-fake-lag     :: false
antiaim-visual-fake-color   :: (0, 150, 255)
antiaim-visual-fake-opacity :: 100.0
antiaim-visual-real         :: false
antiaim-visual-real-color   :: (0, 150, 255)
antiaim-visual-real-opacity :: 100.0



#############
## Visuals ##
#############

## Chams
visuals-chams-enemy-visible-enable    :: false
visuals-chams-enemy-visible-color     :: (0, 150, 255)
visuals-chams-enemy-visible-material  :: Normal
visuals-chams-enemy-hidden-enable     :: false
visuals-chams-enemy-hidden-color      :: (0, 150, 255)
visuals-chams-enemy-hidden-material   :: Normal
visuals-chams-friend-visible-enable   :: false
visuals-chams-friend-visible-color    :: (0, 150, 255)
visuals-chams-friend-visible-material :: Normal
visuals-chams-friend-hidden-enable    :: false
visuals-chams-friend-hidden-color     :: (0, 150, 255)
visuals-chams-friend-hidden-material  :: Normal

## ESP
visuals-esp-other-grenadebox               :: false
visuals-esp-other-weapon-box               :: false
visuals-esp-other-weapon-name              :: false
visuals-esp-enemy-enable                   :: false
visuals-esp-enemy-bbox                     :: false
visuals-esp-enemy-bbox-color               :: (255, 255, 255)
visuals-esp-enemy-name                     :: false
visuals-esp-enemy-name-color               :: (255, 255, 255)
visuals-esp-enemy-snapline                 :: false
visuals-esp-enemy-snapline-color           :: (255, 255, 255)
visuals-esp-enemy-skeleton                 :: false
visuals-esp-enemy-skeleton-color           :: (255, 255, 255)
visuals-esp-enemy-health                   :: false
visuals-esp-enemy-health-color             :: (0, 255, 0)
visuals-esp-enemy-health-color-background  :: (255, 0, 0)
visuals-esp-enemy-ammo                     :: false
visuals-esp-enemy-ammo-color               :: (0, 0, 255)
visuals-esp-enemy-ammo-color-background    :: (100, 100, 100)
visuals-esp-friend-enable                  :: false
visuals-esp-friend-bbox                    :: false
visuals-esp-friend-bbox-color              :: (255, 255, 255)
visuals-esp-friend-name                    :: false
visuals-esp-friend-name-color              :: (255, 255, 255)
visuals-esp-friend-snapline                :: false
visuals-esp-friend-snapline-color          :: (255, 255, 255)
visuals-esp-friend-skeleton                :: false
visuals-esp-friend-skeleton-color          :: (255, 255, 255)
visuals-esp-friend-health                  :: false
visuals-esp-friend-health-color            :: (0, 255, 0)
visuals-esp-friend-health-color-background :: (255, 0, 0)
visuals-esp-friend-ammo                    :: false
visuals-esp-friend-ammo-color              :: (0, 0, 255)
visuals-esp-friend-ammo-color-background   :: (100, 100, 100)

## World
visuals-world-enable        :: false
visuals-world-color         :: (0, 150, 255)
visuals-world-opacity       :: 0
visuals-world-prop-enable   :: false
visuals-world-prop-color    :: (0, 150, 255)
visuals-world-prop-opacity  :: 0
visuals-world-skybox-enable :: false
visuals-world-skybox-name   :: cs_baggage_skybox_
visuals-world-skybox-color  :: (0, 150, 255)

## Misc
visuals-misc-thirdperson     :: false
visuals-misc-vote-indicator  :: false
visuals-misc-spec-list       :: false
visuals-misc-teamdamage-list :: false



##########
## Misc ##
##########

## Movement
misc-movement-bhop           :: false
misc-movement-slowwalk       :: false
misc-movement-slowwalk-speed :: 0
misc-movement-airstuck       :: false
misc-movement-fastcrouch     :: false
misc-movement-fakeduck       :: false
misc-movement-autostrafe     :: None

## Other
misc-other-autoaccept     :: false
misc-other-killsay        :: false
misc-other-killsay-input  :: "Get pwnd by particle.church!"
misc-other-fullautopistol :: false
misc-other-clantag-input  :: ""



##########
## Menu ##
##########

## Menu
show-menu       : toggle @ insert : true
menu-complexity :: Professional
show-help-link  :: true
show-watermark  :: true

## Colors
menu-text-color                       :: (255, 255, 255)
menu-eject-color                      :: (200, 75, 75)
menu-background-color1                :: (30, 30, 30)
menu-background-color2                :: (20, 20, 20)
menu-background-color3                :: (50, 50, 50)
menu-background-color4                :: (75, 75, 75)
menu-option-selector-background-color :: (20, 20, 20)
menu-option-selector-active-color     :: (60, 60, 60)
menu-option-selector-text-color       :: (255, 255, 255)
menu-option-color1                    :: (230, 230, 230)
menu-option-color2                    :: (0, 150, 255)

## Opacities

)"
	},
	ConfigManager::ConfigData{
		false,
		"Default Rage",
R"(
# PARTICLE.CHURCH config generated on mm/dd/yyyy @ hh:mm:ss
# For help making a config, please visit https://particle.church/help/config
# 
# The format for this file is as follows,
# and is explained in greater detail at above link:
# property-name : value; keybind_type @ key_name



#############
## Offense ##
#############

## Aimbot
enable-aimbot                   :: false
enable-silentaim                :: true
aimbot-autoshoot                :: true
aimbot-autowall                 :: true
aimbot-aimstep                  :: 0
aimbot-pistol-smoothing-method  :: None
aimbot-pistol-smoothing-amount  :: 0.0
aimbot-pistol-fov               :: 0.0
aimbot-pistol-hitbox            :: [None]
aimbot-pistol-hitbox-priority   :: Head
aimbot-pistol-mindamage-visible :: 0
aimbot-pistol-mindamage-hidden  :: 0
aimbot-pistol-hitchance         :: 0
aimbot-pistol-if-lethal         :: true
aimbot-smg-smoothing-method     :: None
aimbot-smg-smoothing-amount     :: 0.0
aimbot-smg-fov                  :: 0.0
aimbot-smg-hitbox               :: [None]
aimbot-smg-hitbox-priority      :: Head
aimbot-smg-mindamage-visible    :: 0
aimbot-smg-mindamage-hidden     :: 0
aimbot-smg-hitchance            :: 0
aimbot-smg-if-lethal            :: true
aimbot-heavy-smoothing-method   :: None
aimbot-heavy-smoothing-amount   :: 0.0
aimbot-heavy-fov                :: 0.0
aimbot-heavy-hitbox             :: [None]
aimbot-heavy-hitbox-priority    :: Head
aimbot-heavy-mindamage-visible  :: 0
aimbot-heavy-mindamage-hidden   :: 0
aimbot-heavy-hitchance          :: 0
aimbot-heavy-if-lethal          :: true
aimbot-rifle-smoothing-method   :: None
aimbot-rifle-smoothing-amount   :: 0.0
aimbot-rifle-fov                :: 0.0
aimbot-rifle-hitbox             :: [None]
aimbot-rifle-hitbox-priority    :: Head
aimbot-rifle-mindamage-visible  :: 0
aimbot-rifle-mindamage-hidden   :: 0
aimbot-rifle-hitchance          :: 0
aimbot-rifle-if-lethal          :: true
aimbot-scout-smoothing-method   :: None
aimbot-scout-smoothing-amount   :: 0.0
aimbot-scout-fov                :: 0.0
aimbot-scout-hitbox             :: [None]
aimbot-scout-hitbox-priority    :: Head
aimbot-scout-mindamage-visible  :: 0
aimbot-scout-mindamage-hidden   :: 0
aimbot-scout-hitchance          :: 0
aimbot-scout-if-lethal          :: true
aimbot-awp-smoothing-method     :: None
aimbot-awp-smoothing-amount     :: 0.0
aimbot-awp-fov                  :: 0.0
aimbot-awp-hitbox               :: [None]
aimbot-awp-hitbox-priority      :: Head
aimbot-awp-mindamage-visible    :: 0
aimbot-awp-mindamage-hidden     :: 0
aimbot-awp-hitchance            :: 0
aimbot-awp-if-lethal            :: true
aimbot-auto-smoothing-method    :: None
aimbot-auto-smoothing-amount    :: 0.0
aimbot-auto-fov                 :: 0.0
aimbot-auto-hitbox              :: [None]
aimbot-auto-hitbox-priority     :: Head
aimbot-auto-mindamage-visible   :: 0
aimbot-auto-mindamage-hidden    :: 0
aimbot-auto-hitchance           :: 0
aimbot-auto-if-lethal           :: true

## Triggerbot
enable-triggerbot       :: true
triggerbot-through-wall :: true
triggerbot-delay        :: 0
triggerbot-magnet       :: true

## Other
backtracking-time :: 0



#############
## Defense ##
#############

## Fake Lag
antiaim-fakelag-tick     :: 3
antiaim-fakelag-distance :: 64

## Legit
antiaim-legit-enable    :: false
antiaim-legit-max-angle :: 0
antiaim-legit-invert    :: [error]

## Rage
antiaim-rage-enable      :: false
antiaim-rage-pitch       :: Up
antiaim-rage-real        :: 0
antiaim-rage-real-jitter :: 0
antiaim-rage-fake        :: 0
antiaim-rage-fake-jitter :: 100.0
antiaim-rage-invert      :: [error]
antiaim-rage-fake-invert :: false

## Visuals
antiaim-visual-fake         :: false
antiaim-visual-fake-lag     :: false
antiaim-visual-fake-color   :: (0, 150, 255)
antiaim-visual-fake-opacity :: 100.0
antiaim-visual-real         :: false
antiaim-visual-real-color   :: (0, 150, 255)
antiaim-visual-real-opacity :: 100.0



#############
## Visuals ##
#############

## Chams
visuals-chams-enemy-visible-enable    :: false
visuals-chams-enemy-visible-color     :: (0, 150, 255)
visuals-chams-enemy-visible-material  :: Normal
visuals-chams-enemy-hidden-enable     :: false
visuals-chams-enemy-hidden-color      :: (0, 150, 255)
visuals-chams-enemy-hidden-material   :: Normal
visuals-chams-friend-visible-enable   :: false
visuals-chams-friend-visible-color    :: (0, 150, 255)
visuals-chams-friend-visible-material :: Normal
visuals-chams-friend-hidden-enable    :: false
visuals-chams-friend-hidden-color     :: (0, 150, 255)
visuals-chams-friend-hidden-material  :: Normal

## ESP
visuals-esp-other-grenadebox               :: false
visuals-esp-other-weapon-box               :: false
visuals-esp-other-weapon-name              :: false
visuals-esp-enemy-enable                   :: false
visuals-esp-enemy-bbox                     :: false
visuals-esp-enemy-bbox-color               :: (255, 255, 255)
visuals-esp-enemy-name                     :: false
visuals-esp-enemy-name-color               :: (255, 255, 255)
visuals-esp-enemy-snapline                 :: false
visuals-esp-enemy-snapline-color           :: (255, 255, 255)
visuals-esp-enemy-skeleton                 :: false
visuals-esp-enemy-skeleton-color           :: (255, 255, 255)
visuals-esp-enemy-health                   :: false
visuals-esp-enemy-health-color             :: (0, 255, 0)
visuals-esp-enemy-health-color-background  :: (255, 0, 0)
visuals-esp-enemy-ammo                     :: false
visuals-esp-enemy-ammo-color               :: (0, 0, 255)
visuals-esp-enemy-ammo-color-background    :: (100, 100, 100)
visuals-esp-friend-enable                  :: false
visuals-esp-friend-bbox                    :: false
visuals-esp-friend-bbox-color              :: (255, 255, 255)
visuals-esp-friend-name                    :: false
visuals-esp-friend-name-color              :: (255, 255, 255)
visuals-esp-friend-snapline                :: false
visuals-esp-friend-snapline-color          :: (255, 255, 255)
visuals-esp-friend-skeleton                :: false
visuals-esp-friend-skeleton-color          :: (255, 255, 255)
visuals-esp-friend-health                  :: false
visuals-esp-friend-health-color            :: (0, 255, 0)
visuals-esp-friend-health-color-background :: (255, 0, 0)
visuals-esp-friend-ammo                    :: false
visuals-esp-friend-ammo-color              :: (0, 0, 255)
visuals-esp-friend-ammo-color-background   :: (100, 100, 100)

## World
visuals-world-enable        :: false
visuals-world-color         :: (0, 150, 255)
visuals-world-opacity       :: 0
visuals-world-prop-enable   :: false
visuals-world-prop-color    :: (0, 150, 255)
visuals-world-prop-opacity  :: 0
visuals-world-skybox-enable :: false
visuals-world-skybox-name   :: cs_baggage_skybox_
visuals-world-skybox-color  :: (0, 150, 255)

## Misc
visuals-misc-thirdperson     :: false
visuals-misc-vote-indicator  :: false
visuals-misc-spec-list       :: false
visuals-misc-teamdamage-list :: false



##########
## Misc ##
##########

## Movement
misc-movement-bhop           :: false
misc-movement-slowwalk       :: false
misc-movement-slowwalk-speed :: 0
misc-movement-airstuck       :: false
misc-movement-fastcrouch     :: false
misc-movement-fakeduck       :: false
misc-movement-autostrafe     :: None

## Other
misc-other-autoaccept     :: false
misc-other-killsay        :: false
misc-other-killsay-input  :: "Get pwnd by particle.church!"
misc-other-fullautopistol :: false
misc-other-clantag-input  :: ""



##########
## Menu ##
##########

## Menu
show-menu       : toggle @ insert : true
menu-complexity :: Professional
show-help-link  :: true
show-watermark  :: true

## Colors
menu-text-color                       :: (255, 255, 255)
menu-eject-color                      :: (200, 75, 75)
menu-background-color1                :: (30, 30, 30)
menu-background-color2                :: (20, 20, 20)
menu-background-color3                :: (50, 50, 50)
menu-background-color4                :: (75, 75, 75)
menu-option-selector-background-color :: (20, 20, 20)
menu-option-selector-active-color     :: (60, 60, 60)
menu-option-selector-text-color       :: (255, 255, 255)
menu-option-color1                    :: (230, 230, 230)
menu-option-color2                    :: (0, 150, 255)

## Opacities

)"
	},
};

std::string ConfigManager::GetOpenFile()
{
	char filename[MAX_PATH];
	ZeroMemory(&filename, MAX_PATH);

	OPENFILENAME ofn{};
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Text Files (.cfg, .txt, .church)\0*.cfg;*.txt;*.church\0All Files\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Open Config";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_NODEREFERENCELINKS | OFN_EXPLORER;

	if (!GetOpenFileName(&ofn)) return "";
	return std::string(filename, filename + MAX_PATH);
}

std::string ConfigManager::GetSaveFile()
{
	char filename[MAX_PATH];
	ZeroMemory(&filename, MAX_PATH);
	strcpy(filename, "particle.church");

	OPENFILENAME ofn{};
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Text Files (.cfg, .txt, .church)\0*.cfg;*.txt;*.church\0All Files\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Save Config";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_NODEREFERENCELINKS | OFN_EXPLORER | OFN_OVERWRITEPROMPT;

	if (!GetSaveFileName(&ofn)) return "";
	return std::string(filename, filename + MAX_PATH);
	return GetOpenFile();
}

DWORD WINAPI ConfigManager::ImportThreaded(void* _)
{
	std::string filename = GetOpenFile();
	if (filename == "") return 1;

	std::ifstream file(filename.c_str());
	if (!file.is_open()) return 2;
	file.seekg(0, std::ios::end);
	std::streampos bytes = file.tellg();
	if (bytes > 100000) return 3; // too large
	file.seekg(0);
	file.clear();

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string str = buffer.str();

	std::string baseName = PathFindFileName(filename.c_str());

	// attempt to remove extension
	{
		size_t last = baseName.find_last_of('.');
		if (last > 0 && last != std::string::npos)
		{
			baseName = baseName.substr(0, last);
		}
	}

	ConfigData cdata{
		true,
		baseName,
		str
	};
	Configs.push_back(cdata);

	std::cout << "Imported " << str.size() << " bytes from : " << filename << std::endl;
	return 0;
}

DWORD WINAPI ConfigManager::ExportThreaded(void* _)
{
	// convert all properties to string
	std::string s = Config::ExportToString();
	std::string filename = GetSaveFile();
	if (filename == "") return 1;

	std::fstream file;
	file.open(filename.c_str(), std::fstream::out | std::fstream::trunc);
	if (!file.is_open()) return 2;

	file.write(s.c_str(), s.size());

	file.close();

	std::cout << "Exported " << s.size() << " bytes to : " << filename << std::endl;
	return 0;
}

void ConfigManager::Import()
{
	HANDLE t = CreateThread(NULL, 0, ImportThreaded, NULL, NULL, NULL);
	if (t) CloseHandle(t);
}

void ConfigManager::Export()
{
	HANDLE t = CreateThread(NULL, 0, ExportThreaded, NULL, NULL, NULL);
	if (t) CloseHandle(t);
}

bool ConfigManager::Remove(size_t i)
{
	if (i >= Configs.size()) return false;
	Configs.erase(Configs.begin() + i);
	return true;
}

void ConfigManager::Load(ConfigManager::ConfigData cdata)
{
	// clean first
	if (Configs.size() > 0)
		Config::LoadFromString(Configs[0].Value);
	Config::ClearAllKeybinds();

	size_t nProperties = Config::LoadFromString(cdata.Value);
	std::cout << "Loaded " << nProperties << " properties" << std::endl;
	// todo notify the user
}