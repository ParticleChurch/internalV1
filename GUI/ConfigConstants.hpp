#pragma once

namespace ConfigManager
{
	constexpr auto ConfigAllDisabled = R"(
# a4g4.com config generated on mm/dd/yyyy @ hh:mm:ss
# For help making a config, please visit https://a4g4.com/help/config
# 
# The format for this file is as follows,
# and is explained in greater detail at above link:
# property-name : keybind_type @ key_name: value



#############
## Offense ##
#############

## Aimbot
enable-aimbot                   :: false
enable-silentaim                :: false
aimbot-autoshoot                :: true
aimbot-autowall                 :: true
aimbot-aimstep                  :: 0
aimbot-pistol-smoothing-method  :: None
aimbot-pistol-smoothing-amount  :: 0.0
aimbot-pistol-fov               :: 0.0
aimbot-pistol-hitbox            :: {}
aimbot-pistol-hitbox-priority   :: Head
aimbot-pistol-mindamage-visible :: 0
aimbot-pistol-mindamage-hidden  :: 0
aimbot-pistol-hitchance         :: 0
aimbot-pistol-if-lethal         :: false
aimbot-smg-smoothing-method     :: None
aimbot-smg-smoothing-amount     :: 0.0
aimbot-smg-fov                  :: 0.0
aimbot-smg-hitbox               :: {}
aimbot-smg-hitbox-priority      :: Head
aimbot-smg-mindamage-visible    :: 0
aimbot-smg-mindamage-hidden     :: 0
aimbot-smg-hitchance            :: 0
aimbot-smg-if-lethal            :: false
aimbot-heavy-smoothing-method   :: None
aimbot-heavy-smoothing-amount   :: 0.0
aimbot-heavy-fov                :: 0.0
aimbot-heavy-hitbox             :: {}
aimbot-heavy-hitbox-priority    :: Head
aimbot-heavy-mindamage-visible  :: 0
aimbot-heavy-mindamage-hidden   :: 0
aimbot-heavy-hitchance          :: 0
aimbot-heavy-if-lethal          :: false
aimbot-rifle-smoothing-method   :: None
aimbot-rifle-smoothing-amount   :: 0.0
aimbot-rifle-fov                :: 0.0
aimbot-rifle-hitbox             :: {}
aimbot-rifle-hitbox-priority    :: Head
aimbot-rifle-mindamage-visible  :: 0
aimbot-rifle-mindamage-hidden   :: 0
aimbot-rifle-hitchance          :: 0
aimbot-rifle-if-lethal          :: false
aimbot-scout-smoothing-method   :: None
aimbot-scout-smoothing-amount   :: 0.0
aimbot-scout-fov                :: 0.0
aimbot-scout-hitbox             :: {}
aimbot-scout-hitbox-priority    :: Head
aimbot-scout-mindamage-visible  :: 0
aimbot-scout-mindamage-hidden   :: 0
aimbot-scout-hitchance          :: 0
aimbot-scout-if-lethal          :: false
aimbot-awp-smoothing-method     :: None
aimbot-awp-smoothing-amount     :: 0.0
aimbot-awp-fov                  :: 0.0
aimbot-awp-hitbox               :: {}
aimbot-awp-hitbox-priority      :: Head
aimbot-awp-mindamage-visible    :: 0
aimbot-awp-mindamage-hidden     :: 0
aimbot-awp-hitchance            :: 0
aimbot-awp-if-lethal            :: false
aimbot-auto-smoothing-method    :: None
aimbot-auto-smoothing-amount    :: 0.0
aimbot-auto-fov                 :: 0.0
aimbot-auto-hitbox              :: {}
aimbot-auto-hitbox-priority     :: Head
aimbot-auto-mindamage-visible   :: 0
aimbot-auto-mindamage-hidden    :: 0
aimbot-auto-hitchance           :: 0
aimbot-auto-if-lethal           :: false

## Triggerbot
enable-triggerbot       :: false
triggerbot-through-wall :: false
triggerbot-delay        :: 0
triggerbot-magnet       :: false

## Other
backtracking-time :: 0



#############
## Defense ##
#############

## Fake Lag
antiaim-fakelag-tick     :: 0
antiaim-fakelag-distance :: 0

## Legit
antiaim-legit-enable    :: false
antiaim-legit-max-angle :: 0
antiaim-legit-invert    :: Left

## Rage
antiaim-rage-enable      :: false
antiaim-rage-pitch       :: Up
antiaim-rage-real        :: -179
antiaim-rage-real-jitter :: 0
antiaim-rage-fake        :: 0
antiaim-rage-fake-jitter :: 0.0
antiaim-rage-invert      :: 
antiaim-rage-fake-invert :: false

## Visuals
antiaim-visual-fake         :: false
antiaim-visual-fake-lag     :: false
antiaim-visual-fake-color   :: (0, 150, 255)
antiaim-visual-fake-opacity :: 0.0
antiaim-visual-real         :: false
antiaim-visual-real-color   :: (0, 150, 255)
antiaim-visual-real-opacity :: 0.0



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
misc-other-killsay-input  :: 
misc-other-fullautopistol :: false
misc-other-clantag-input  :: 



##########
## Menu ##
##########

## Menu
show-menu       : toggle @ insert : true
menu-complexity :: Professional
show-help-link  :: false
show-watermark  :: false

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

)";

constexpr auto ConfigDefaultLegit = R"(
# a4g4.com config generated on mm/dd/yyyy @ hh:mm:ss
# For help making a config, please visit https://a4g4.com/help/config
# 
# The format for this file is as follows,
# and is explained in greater detail at above link:
# property-name : keybind_type @ key_name: value



#############
## Offense ##
#############

## Aimbot
enable-aimbot                   : hold_enable @ mouse1 : false
enable-silentaim                :: false
enable-autoshoot                :: false
enable-autowall                 :: false
aimbot-pistol-smoothing-method  :: Fast-to-Slow
aimbot-pistol-smoothing-amount  :: 88.0
aimbot-pistol-fov               :: 0.9
aimbot-pistol-hitbox            :: {Head}
aimbot-pistol-hitbox-priority   :: Head
aimbot-pistol-mindamage-visible :: 4
aimbot-pistol-mindamage-hidden  :: 1
aimbot-pistol-hitchance         :: 100
aimbot-pistol-if-lethal         :: false
aimbot-smg-smoothing-method     :: Fast-to-Slow
aimbot-smg-smoothing-amount     :: 72.6
aimbot-smg-fov                  :: 16.4
aimbot-smg-hitbox               :: {Neck}
aimbot-smg-hitbox-priority      :: Head
aimbot-smg-mindamage-visible    :: 3
aimbot-smg-mindamage-hidden     :: 3
aimbot-smg-hitchance            :: 100
aimbot-smg-if-lethal            :: false
aimbot-heavy-smoothing-method   :: None
aimbot-heavy-smoothing-amount   :: 0.0
aimbot-heavy-fov                :: 0.0
aimbot-heavy-hitbox             :: {}
aimbot-heavy-hitbox-priority    :: Head
aimbot-heavy-mindamage-visible  :: 0
aimbot-heavy-mindamage-hidden   :: 0
aimbot-heavy-hitchance          :: 0
aimbot-heavy-if-lethal          :: false
aimbot-rifle-smoothing-method   :: Linear
aimbot-rifle-smoothing-amount   :: 95.8
aimbot-rifle-fov                :: 5.6
aimbot-rifle-hitbox             :: {Head}
aimbot-rifle-hitbox-priority    :: Head
aimbot-rifle-mindamage-visible  :: 2
aimbot-rifle-mindamage-hidden   :: 4
aimbot-rifle-hitchance          :: 100
aimbot-rifle-if-lethal          :: false
aimbot-scout-smoothing-method   :: Linear
aimbot-scout-smoothing-amount   :: 76.8
aimbot-scout-fov                :: 1.4
aimbot-scout-hitbox             :: {Head}
aimbot-scout-hitbox-priority    :: Head
aimbot-scout-mindamage-visible  :: 5
aimbot-scout-mindamage-hidden   :: 4
aimbot-scout-hitchance          :: 100
aimbot-scout-if-lethal          :: false
aimbot-awp-smoothing-method     :: Linear
aimbot-awp-smoothing-amount     :: 95.3
aimbot-awp-fov                  :: 5.6
aimbot-awp-hitbox               :: {Upper-Chest}
aimbot-awp-hitbox-priority      :: Head
aimbot-awp-mindamage-visible    :: 2
aimbot-awp-mindamage-hidden     :: 5
aimbot-awp-hitchance            :: 100
aimbot-awp-if-lethal            :: false
aimbot-auto-smoothing-method    :: Linear
aimbot-auto-smoothing-amount    :: 88.5
aimbot-auto-fov                 :: 5.6
aimbot-auto-hitbox              :: {Neck}
aimbot-auto-hitbox-priority     :: Head
aimbot-auto-mindamage-visible   :: 2
aimbot-auto-mindamage-hidden    :: 3
aimbot-auto-hitchance           :: 100
aimbot-auto-if-lethal           :: false

## Other
backtracking-time :: 46



#############
## Defense ##
#############

## Fake Lag
antiaim-fakelag-tick     :: 1
antiaim-fakelag-distance :: 64

## Legit
antiaim-legit-enable    :: false
antiaim-legit-max-angle :: 0
antiaim-legit-invert    : toggle @ mouse5 : Left

## Rage
antiaim-rage-enable      :: false
antiaim-rage-pitch       :: Up
antiaim-rage-real        :: -178
antiaim-rage-real-jitter :: 0
antiaim-rage-fake        :: 0
antiaim-rage-fake-jitter :: 0.0
antiaim-rage-invert      : toggle @ mouse5 : 
antiaim-rage-fake-invert :: false



#############
## Visuals ##
#############

## Chams
visuals-chams-enemy-visible-enable    :: true
visuals-chams-enemy-visible-color     :: (9, 177, 0)
visuals-chams-enemy-visible-material  :: Normal
visuals-chams-enemy-hidden-enable     :: false
visuals-chams-enemy-hidden-color      :: (0, 150, 255)
visuals-chams-enemy-hidden-material   :: Normal
visuals-chams-friend-visible-enable   :: true
visuals-chams-friend-visible-color    :: (136, 136, 136)
visuals-chams-friend-visible-material :: Normal
visuals-chams-friend-hidden-enable    :: false
visuals-chams-friend-hidden-color     :: (0, 150, 255)
visuals-chams-friend-hidden-material  :: Normal

## ESP
visuals-esp-enemy-enable                   :: false
visuals-esp-enemy-bbox                     :: false
visuals-esp-enemy-bbox-color               :: (255, 255, 255)
visuals-esp-enemy-name                     :: true
visuals-esp-enemy-name-color               :: (255, 255, 255)
visuals-esp-enemy-snapline                 :: false
visuals-esp-enemy-snapline-color           :: (255, 255, 255)
visuals-esp-enemy-skeleton                 :: true
visuals-esp-enemy-skeleton-color           :: (253, 0, 0)
visuals-esp-enemy-health                   :: true
visuals-esp-enemy-health-color             :: (0, 255, 0)
visuals-esp-enemy-health-color-background  :: (255, 0, 0)
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

## World
visuals-world-enable        :: true
visuals-world-color         :: (120, 120, 120)
visuals-world-opacity       :: 100
visuals-world-skybox-enable :: true
visuals-world-skybox-name   :: sky_lunacy
visuals-world-skybox-color  :: (45, 45, 45)

## Misc
visuals-misc-thirdperson : toggle @ v : false



##########
## Misc ##
##########

## Movement
misc-movement-bhop           :: true
misc-movement-slowwalk       :: false
misc-movement-slowwalk-speed :: 0
misc-movement-airstuck       :: false
misc-movement-fastcrouch     :: true
misc-movement-fakeduck       :: false
misc-movement-autostrafe     :: None
misc-movement-leg            :: false
misc-movement-leg-time       :: 0

## Other
misc-other-autoaccept    :: true
misc-other-killsay       :: false
misc-other-killsay-input :: 
misc-other-clantag-input :: a4g4.com



##########
## Menu ##
##########

## Menu
show-menu       : toggle @ insert : true
menu-complexity :: Professional
show-help-link  :: false
show-watermark  :: false

## Colors
menu-text-color                       :: (255, 254, 254)
menu-eject-color                      :: (200, 75, 75)
menu-background-color1                :: (93, 93, 93)
menu-background-color2                :: (0, 0, 0)
menu-background-color3                :: (0, 73, 126)
menu-background-color4                :: (93, 93, 93)
menu-option-selector-background-color :: (255, 254, 254)
menu-option-selector-active-color     :: (132, 132, 132)
menu-option-selector-text-color       :: (0, 0, 0)
menu-option-color1                    :: (255, 254, 254)
menu-option-color2                    :: (0, 110, 182)

## Opacities

)";

constexpr auto ConfigDefualtRage = R"(
# a4g4.com config generated on mm/dd/yyyy @ hh:mm:ss
# For help making a config, please visit https://a4g4.com/help/config
# 
# The format for this file is as follows,
# and is explained in greater detail at above link:
# property-name : keybind_type @ key_name: value



#############
## Offense ##
#############

## Aimbot
enable-aimbot                   :: true
enable-silentaim                :: true
enable-autoshoot                :: true
enable-autowall                 :: true
aimbot-pistol-smoothing-method  :: None
aimbot-pistol-smoothing-amount  :: 0.0
aimbot-pistol-fov               :: 180.0
aimbot-pistol-hitbox            :: {Head}
aimbot-pistol-hitbox-priority   :: Head
aimbot-pistol-mindamage-visible :: 2
aimbot-pistol-mindamage-hidden  :: 2
aimbot-pistol-hitchance         :: 100
aimbot-pistol-if-lethal         :: false
aimbot-smg-smoothing-method     :: None
aimbot-smg-smoothing-amount     :: 0.0
aimbot-smg-fov                  :: 0.0
aimbot-smg-hitbox               :: {}
aimbot-smg-hitbox-priority      :: Head
aimbot-smg-mindamage-visible    :: 0
aimbot-smg-mindamage-hidden     :: 0
aimbot-smg-hitchance            :: 0
aimbot-smg-if-lethal            :: false
aimbot-heavy-smoothing-method   :: None
aimbot-heavy-smoothing-amount   :: 0.0
aimbot-heavy-fov                :: 180.0
aimbot-heavy-hitbox             :: {Head, Stomach, Pelvis}
aimbot-heavy-hitbox-priority    :: Head
aimbot-heavy-mindamage-visible  :: 0
aimbot-heavy-mindamage-hidden   :: 0
aimbot-heavy-hitchance          :: 0
aimbot-heavy-if-lethal          :: false
aimbot-rifle-smoothing-method   :: None
aimbot-rifle-smoothing-amount   :: 0.0
aimbot-rifle-fov                :: 180.0
aimbot-rifle-hitbox             :: {Head}
aimbot-rifle-hitbox-priority    :: Head
aimbot-rifle-mindamage-visible  :: 4
aimbot-rifle-mindamage-hidden   :: 3
aimbot-rifle-hitchance          :: 100
aimbot-rifle-if-lethal          :: false
aimbot-scout-smoothing-method   :: None
aimbot-scout-smoothing-amount   :: 0.0
aimbot-scout-fov                :: 180.0
aimbot-scout-hitbox             :: {Pelvis}
aimbot-scout-hitbox-priority    :: Head
aimbot-scout-mindamage-visible  :: 13
aimbot-scout-mindamage-hidden   :: 6
aimbot-scout-hitchance          :: 100
aimbot-scout-if-lethal          :: false
aimbot-awp-smoothing-method     :: None
aimbot-awp-smoothing-amount     :: 0.0
aimbot-awp-fov                  :: 180.0
aimbot-awp-hitbox               :: {Upper-Chest, Stomach, Pelvis}
aimbot-awp-hitbox-priority      :: Head
aimbot-awp-mindamage-visible    :: 3
aimbot-awp-mindamage-hidden     :: 5
aimbot-awp-hitchance            :: 69
aimbot-awp-if-lethal            :: false
aimbot-auto-smoothing-method    :: None
aimbot-auto-smoothing-amount    :: 0.0
aimbot-auto-fov                 :: 180.0
aimbot-auto-hitbox              :: {Head, Stomach, Pelvis}
aimbot-auto-hitbox-priority     :: Head
aimbot-auto-mindamage-visible   :: 1
aimbot-auto-mindamage-hidden    :: 2
aimbot-auto-hitchance           :: 71
aimbot-auto-if-lethal           :: false

## Other
backtracking-time :: 200



#############
## Defense ##
#############

## Fake Lag
antiaim-fakelag-tick     :: 3
antiaim-fakelag-distance :: 64

## Legit
antiaim-legit-enable    :: false
antiaim-legit-max-angle :: 100
antiaim-legit-invert    : toggle @ mouse5 : Left

## Rage
antiaim-rage-enable      :: true
antiaim-rage-pitch       :: Down
antiaim-rage-real        :: 1
antiaim-rage-real-jitter :: 0
antiaim-rage-fake        :: 100
antiaim-rage-fake-jitter :: 8.1
antiaim-rage-invert      : toggle @ mouse5 : 
antiaim-rage-fake-invert :: false



#############
## Visuals ##
#############

## Chams
visuals-chams-enemy-visible-enable    :: true
visuals-chams-enemy-visible-color     :: (160, 0, 0)
visuals-chams-enemy-visible-material  :: Pearlescent
visuals-chams-enemy-hidden-enable     :: true
visuals-chams-enemy-hidden-color      :: (0, 101, 255)
visuals-chams-enemy-hidden-material   :: Animated
visuals-chams-friend-visible-enable   :: false
visuals-chams-friend-visible-color    :: (0, 150, 255)
visuals-chams-friend-visible-material :: Normal
visuals-chams-friend-hidden-enable    :: false
visuals-chams-friend-hidden-color     :: (0, 150, 255)
visuals-chams-friend-hidden-material  :: Normal

## ESP
visuals-esp-enemy-enable                   :: true
visuals-esp-enemy-bbox                     :: false
visuals-esp-enemy-bbox-color               :: (255, 255, 255)
visuals-esp-enemy-name                     :: true
visuals-esp-enemy-name-color               :: (255, 255, 255)
visuals-esp-enemy-snapline                 :: false
visuals-esp-enemy-snapline-color           :: (255, 255, 255)
visuals-esp-enemy-skeleton                 :: true
visuals-esp-enemy-skeleton-color           :: (255, 0, 0)
visuals-esp-enemy-health                   :: true
visuals-esp-enemy-health-color             :: (0, 255, 0)
visuals-esp-enemy-health-color-background  :: (255, 0, 0)
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

## World
visuals-world-enable        :: true
visuals-world-color         :: (94, 94, 94)
visuals-world-opacity       :: 100
visuals-world-skybox-enable :: true
visuals-world-skybox-name   :: sky_lunacy
visuals-world-skybox-color  :: (138, 138, 138)

## Misc
visuals-misc-thirdperson : toggle @ v : true



##########
## Misc ##
##########

## Movement
misc-movement-bhop           :: true
misc-movement-slowwalk       : hold_enable @ alt : false
misc-movement-slowwalk-speed :: 100
misc-movement-airstuck       :: false
misc-movement-fastcrouch     :: false
misc-movement-fakeduck       :: false
misc-movement-autostrafe     :: Legit
misc-movement-leg            :: false
misc-movement-leg-time       :: 171

## Other
misc-other-autoaccept    :: false
misc-other-killsay       :: true
misc-other-killsay-input :: lol, im not that good >_<
misc-other-clantag-input :: a4g4.com



##########
## Menu ##
##########

## Menu
show-menu       : toggle @ insert : true
menu-complexity :: Professional
show-help-link  :: false
show-watermark  :: false

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
menu-option-color1                    :: (0, 0, 0)
menu-option-color2                    :: (180, 0, 0)

## Opacities
)";
}