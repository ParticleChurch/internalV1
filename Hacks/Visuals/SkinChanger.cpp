#include "../../Include.hpp"

SkinChanger* skinchanger = new SkinChanger();

void SkinChanger::ForceSkin(Entity* weap, int paintkit)
{
    if (paintkit == 0)
        return;

    int* idhigh = weap->GetIDHigh();
    if (idhigh == nullptr) // nullptr check this shit, sometimes its annoying
        return;

    *idhigh = 1; // the reason we set this is so the game is forced to use the fallback values

    int* paintkit_ = weap->GetPaintKit();
    if (!paintkit_) return;

    *paintkit_ = paintkit; // we'll set this in our FrameStage hook

    float* wear = weap->GetFallBackWear();
    if (!wear) return;

    *wear = 0.0000000001f; // lower = better
}

std::map <std::string, int> GetSkins()
{
    std::map<std::string, int> newmap;
    newmap["Groundwater"] = 2;
    newmap["Candy Apple"] = 3;
    newmap["Forest DDPAT"] = 5;
    newmap["Arctic Camo"] = 6;
    newmap["Desert Storm"] = 8;
    newmap["Bengal Tiger"] = 9;
    newmap["Copperhead"] = 10;
    newmap["Skulls"] = 11;
    newmap["Crimson Web"] = 12;
    newmap["Blue Streak"] = 13;
    newmap["Red Laminate"] = 14;
    newmap["Gunsmoke"] = 15;
    newmap["Jungle Tiger"] = 16;
    newmap["Urban DDPAT"] = 17;
    newmap["Virus"] = 20;
    newmap["Granite Marbleized"] = 21;
    newmap["Contrast Spray"] = 22;
    newmap["Forest Leaves"] = 25;
    newmap["Lichen Dashed"] = 26;
    newmap["Bone Mask"] = 27;
    newmap["Anodized Navy"] = 28;
    newmap["Snake Camo"] = 30;
    newmap["Silver"] = 32;
    newmap["Hot Rod"] = 33;
    newmap["Metallic DDPAT"] = 34;
    newmap["Ossified"] = 36;
    newmap["Blaze"] = 37;
    newmap["Fade"] = 38;
    newmap["Bulldozer"] = 39;
    newmap["Night"] = 40;
    newmap["Copper"] = 41;
    newmap["Blue Steel"] = 42;
    newmap["Stained"] = 43;
    newmap["Case Hardened"] = 44;
    newmap["Contractor"] = 46;
    newmap["Colony"] = 47;
    newmap["Dragon Tattoo"] = 48;
    newmap["Lightning Strike"] = 51;
    newmap["Slaughter"] = 59;
    newmap["Dark Water"] = 60;
    newmap["Hypnotic"] = 61;
    newmap["Bloomstick"] = 62;
    newmap["Cold Blooded"] = 67;
    newmap["Carbon Fiber"] = 70;
    newmap["Scorpion"] = 71;
    newmap["Safari Mesh"] = 72;
    newmap["Wings"] = 73;
    newmap["Polar Camo"] = 74;
    newmap["Blizzard Marbleized"] = 75;
    newmap["Winter Forest"] = 76;
    newmap["Boreal Forest"] = 77;
    newmap["Forest Night"] = 78;
    newmap["Orange DDPAT"] = 83;
    newmap["Pink DDPAT"] = 84;
    newmap["Mudder"] = 90;
    newmap["Cyanospatter"] = 92;
    newmap["Caramel"] = 93;
    newmap["Grassland"] = 95;
    newmap["Blue Spruce"] = 96;
    newmap["Ultraviolet"] = 98;
    newmap["Sand Dune"] = 99;
    newmap["Storm"] = 100;
    newmap["Tornado"] = 101;
    newmap["Whiteout"] = 102;
    newmap["Grassland Leaves"] = 104;
    newmap["Polar Mesh"] = 107;
    newmap["Condemned"] = 110;
    newmap["Glacier Mesh"] = 111;
    newmap["Sand Mesh"] = 116;
    newmap["Sage Spray"] = 119;
    newmap["Jungle Spray"] = 122;
    newmap["Sand Spray"] = 124;
    newmap["X-Ray"] = 125;
    newmap["Urban Perforated"] = 135;
    newmap["Waves Perforated"] = 136;
    newmap["Orange Peel"] = 141;
    newmap["Urban Masked"] = 143;
    newmap["Jungle Dashed"] = 147;
    newmap["Sand Dashed"] = 148;
    newmap["Urban Dashed"] = 149;
    newmap["Jungle"] = 151;
    newmap["Demolition"] = 153;
    newmap["Afterimage"] = 154;
    newmap["Bullet Rain"] = 155;
    newmap["Death by Kitty"] = 156;
    newmap["Palm"] = 157;
    newmap["Walnut"] = 158;
    newmap["Brass"] = 159;
    newmap["Splash"] = 162;
    newmap["Modern Hunter"] = 164;
    newmap["Splash Jam"] = 165;
    newmap["Blaze Orange"] = 166;
    newmap["Radiation Hazard"] = 167;
    newmap["Nuclear Threat"] = 168;
    newmap["Fallout Warning"] = 169;
    newmap["Predator"] = 170;
    newmap["Irradiated Alert"] = 171;
    newmap["Black Laminate"] = 172;
    newmap["BOOM"] = 174;
    newmap["Scorched"] = 175;
    newmap["Faded Zebra"] = 176;
    newmap["Memento"] = 177;
    newmap["Doomkitty"] = 178;
    newmap["Nuclear Threat"] = 179;
    newmap["Fire Serpent"] = 180;
    newmap["Corticera"] = 181;
    newmap["Emerald Dragon"] = 182;
    newmap["Overgrowth"] = 183;
    newmap["Corticera"] = 184;
    newmap["Golden Koi"] = 185;
    newmap["Wave Spray"] = 186;
    newmap["Zirka"] = 187;
    newmap["Graven"] = 188;
    newmap["Bright Water"] = 189;
    newmap["Black Limba"] = 190;
    newmap["Tempest"] = 191;
    newmap["Shattered"] = 192;
    newmap["Bone Pile"] = 193;
    newmap["Spitfire"] = 194;
    newmap["Demeter"] = 195;
    newmap["Emerald"] = 196;
    newmap["Anodized Navy"] = 197;
    newmap["Hazard"] = 198;
    newmap["Dry Season"] = 199;
    newmap["Mayan Dreams"] = 200;
    newmap["Palm"] = 201;
    newmap["Jungle DDPAT"] = 202;
    newmap["Rust Coat"] = 203;
    newmap["Mosaico"] = 204;
    newmap["Jungle"] = 205;
    newmap["Tornado"] = 206;
    newmap["Facets"] = 207;
    newmap["Sand Dune"] = 208;
    newmap["Groundwater"] = 209;
    newmap["Anodized Gunmetal"] = 210;
    newmap["Ocean Foam"] = 211;
    newmap["Graphite"] = 212;
    newmap["Ocean Foam"] = 213;
    newmap["Graphite"] = 214;
    newmap["X-Ray"] = 215;
    newmap["Blue Titanium"] = 216;
    newmap["Blood Tiger"] = 217;
    newmap["Hexane"] = 218;
    newmap["Hive"] = 219;
    newmap["Hemoglobin"] = 220;
    newmap["Serum"] = 221;
    newmap["Blood in the Water"] = 222;
    newmap["Nightshade"] = 223;
    newmap["Water Sigil"] = 224;
    newmap["Ghost Camo"] = 225;
    newmap["Blue Laminate"] = 226;
    newmap["Electric Hive"] = 227;
    newmap["Blind Spot"] = 228;
    newmap["Azure Zebra"] = 229;
    newmap["Steel Disruption"] = 230;
    newmap["Cobalt Disruption"] = 231;
    newmap["Crimson Web"] = 232;
    newmap["Tropical Storm"] = 233;
    newmap["Ash Wood"] = 234;
    newmap["VariCamo"] = 235;
    newmap["Night Ops"] = 236;
    newmap["Urban Rubble"] = 237;
    newmap["VariCamo Blue"] = 238;
    newmap["CaliCamo"] = 240;
    newmap["Hunting Blind"] = 241;
    newmap["Army Mesh"] = 242;
    newmap["Gator Mesh"] = 243;
    newmap["Teardown"] = 244;
    newmap["Army Recon"] = 245;
    newmap["Amber Fade"] = 246;
    newmap["Damascus Steel"] = 247;
    newmap["Red Quartz"] = 248;
    newmap["Cobalt Quartz"] = 249;
    newmap["Full Stop"] = 250;
    newmap["Pit Viper"] = 251;
    newmap["Silver Quartz"] = 252;
    newmap["Acid Fade"] = 253;
    newmap["Nitro"] = 254;
    newmap["Asiimov"] = 255;
    newmap["The Kraken"] = 256;
    newmap["Guardian"] = 257;
    newmap["Mehndi"] = 258;
    newmap["Redline"] = 259;
    newmap["Pulse"] = 260;
    newmap["Marina"] = 261;
    newmap["Rose Iron"] = 262;
    newmap["Rising Skull"] = 263;
    newmap["Sandstorm"] = 264;
    newmap["Kami"] = 265;
    newmap["Magma"] = 266;
    newmap["Cobalt Halftone"] = 267;
    newmap["Tread Plate"] = 268;
    newmap["The Fuschia Is Now"] = 269;
    newmap["Victoria"] = 270;
    newmap["Undertow"] = 271;
    newmap["Titanium Bit"] = 272;
    newmap["Heirloom"] = 273;
    newmap["Copper Galaxy"] = 274;
    newmap["Red FragCam"] = 275;
    newmap["Panther"] = 276;
    newmap["Stainless"] = 277;
    newmap["Blue Fissure"] = 278;
    newmap["Asiimov"] = 279;
    newmap["Chameleon"] = 280;
    newmap["Corporal"] = 281;
    newmap["Redline"] = 282;
    newmap["Trigon"] = 283;
    newmap["Heat"] = 284;
    newmap["Terrain"] = 285;
    newmap["Antique"] = 286;
    newmap["Pulse"] = 287;
    newmap["Sergeant"] = 288;
    newmap["Sandstorm"] = 289;
    newmap["Guardian"] = 290;
    newmap["Heaven Guard"] = 291;
    newmap["Death Rattle"] = 293;
    newmap["Green Apple"] = 294;
    newmap["Franklin"] = 295;
    newmap["Meteorite"] = 296;
    newmap["Tuxedo"] = 297;
    newmap["Army Sheen"] = 298;
    newmap["Caged Steel"] = 299;
    newmap["Emerald Pinstripe"] = 300;
    newmap["Atomic Alloy"] = 301;
    newmap["Vulcan"] = 302;
    newmap["Isaac"] = 303;
    newmap["Slashed"] = 304;
    newmap["Torque"] = 305;
    newmap["Antique"] = 306;
    newmap["Retribution"] = 307;
    newmap["Kami"] = 308;
    newmap["Howl"] = 309;
    newmap["Curse"] = 310;
    newmap["Desert Warfare"] = 311;
    newmap["Cyrex"] = 312;
    newmap["Orion"] = 313;
    newmap["Heaven Guard"] = 314;
    newmap["Poison Dart"] = 315;
    newmap["Jaguar"] = 316;
    newmap["Bratatat"] = 317;
    newmap["Road Rash"] = 318;
    newmap["Detour"] = 319;
    newmap["Red Python"] = 320;
    newmap["Master Piece"] = 321;
    newmap["Nitro"] = 322;
    newmap["Rust Coat"] = 323;
    newmap["Chalice"] = 325;
    newmap["Knight"] = 326;
    newmap["Chainmail"] = 327;
    newmap["Hand Cannon"] = 328;
    newmap["Dark Age"] = 329;
    newmap["Briar"] = 330;
    newmap["Royal Blue"] = 332;
    newmap["Indigo"] = 333;
    newmap["Twist"] = 334;
    newmap["Module"] = 335;
    newmap["Desert-Strike"] = 336;
    newmap["Tatter"] = 337;
    newmap["Pulse"] = 338;
    newmap["Caiman"] = 339;
    newmap["Jet Set"] = 340;
    newmap["First Class"] = 341;
    newmap["Leather"] = 342;
    newmap["Commuter"] = 343;
    newmap["Dragon Lore"] = 344;
    newmap["First Class"] = 345;
    newmap["Coach Class"] = 346;
    newmap["Pilot"] = 347;
    newmap["Red Leather"] = 348;
    newmap["Osiris"] = 349;
    newmap["Tigris"] = 350;
    newmap["Conspiracy"] = 351;
    newmap["Fowl Play"] = 352;
    newmap["Water Elemental"] = 353;
    newmap["Urban Hazard"] = 354;
    newmap["Desert-Strike"] = 355;
    newmap["Koi"] = 356;
    newmap["Ivory"] = 357;
    newmap["Supernova"] = 358;
    newmap["Asiimov"] = 359;
    newmap["Cyrex"] = 360;
    newmap["Abyss"] = 361;
    newmap["Labyrinth"] = 362;
    newmap["Traveler"] = 363;
    newmap["Business Class"] = 364;
    newmap["Olive Plaid"] = 365;
    newmap["Green Plaid"] = 366;
    newmap["Reactor"] = 367;
    newmap["Setting Sun"] = 368;
    newmap["Nuclear Waste"] = 369;
    newmap["Bone Machine"] = 370;
    newmap["Styx"] = 371;
    newmap["Nuclear Garden"] = 372;
    newmap["Contamination"] = 373;
    newmap["Toxic"] = 374;
    newmap["Radiation Hazard"] = 375;
    newmap["Chemical Green"] = 376;
    newmap["Hot Shot"] = 377;
    newmap["Fallout Warning"] = 378;
    newmap["Cerberus"] = 379;
    newmap["Wasteland Rebel"] = 380;
    newmap["Grinder"] = 381;
    newmap["Murky"] = 382;
    newmap["Basilisk"] = 383;
    newmap["Griffin"] = 384;
    newmap["Firestarter"] = 385;
    newmap["Dart"] = 386;
    newmap["Urban Hazard"] = 387;
    newmap["Cartel"] = 388;
    newmap["Fire Elemental"] = 389;
    newmap["Highwayman"] = 390;
    newmap["Cardiac"] = 391;
    newmap["Delusion"] = 392;
    newmap["Tranquility"] = 393;
    newmap["Cartel"] = 394;
    newmap["Man-o'-war"] = 395;
    newmap["Urban Shock"] = 396;
    newmap["Naga"] = 397;
    newmap["Chatterbox"] = 398;
    newmap["Catacombs"] = 399;
    newmap["龍王 (Dragon King)"] = 400;
    newmap["System Lock"] = 401;
    newmap["Malachite"] = 402;
    newmap["Deadly Poison"] = 403;
    newmap["Muertos"] = 404;
    newmap["Serenity"] = 405;
    newmap["Grotto"] = 406;
    newmap["Quicksilver"] = 407;
    newmap["Tiger Tooth"] = 409;
    newmap["Damascus Steel"] = 410;
    newmap["Damascus Steel"] = 411;
    newmap["Marble Fade"] = 413;
    newmap["Rust Coat"] = 414;
    newmap["Doppler"] = 415;
    newmap["Doppler"] = 416;
    newmap["Doppler"] = 417;
    newmap["Doppler"] = 418;
    newmap["Doppler"] = 419;
    newmap["Doppler"] = 420;
    newmap["Doppler"] = 421;
    newmap["Elite Build"] = 422;
    newmap["Armor Core"] = 423;
    newmap["Worm God"] = 424;
    newmap["Bronze Deco"] = 425;
    newmap["Valence"] = 426;
    newmap["Monkey Business"] = 427;
    newmap["Eco"] = 428;
    newmap["Djinn"] = 429;
    newmap["Hyper Beast"] = 430;
    newmap["Heat"] = 431;
    newmap["Man-o'-war"] = 432;
    newmap["Neon Rider"] = 433;
    newmap["Origami"] = 434;
    newmap["Pole Position"] = 435;
    newmap["Grand Prix"] = 436;
    newmap["Twilight Galaxy"] = 437;
    newmap["Chronos"] = 438;
    newmap["Hades"] = 439;
    newmap["Icarus Fell"] = 440;
    newmap["Minotaur's Labyrinth"] = 441;
    newmap["Asterion"] = 442;
    newmap["Pathfinder"] = 443;
    newmap["Daedalus"] = 444;
    newmap["Hot Rod"] = 445;
    newmap["Medusa"] = 446;
    newmap["Duelist"] = 447;
    newmap["Pandora's Box"] = 448;
    newmap["Poseidon"] = 449;
    newmap["Moon in Libra"] = 450;
    newmap["Sun in Leo"] = 451;
    newmap["Shipping Forecast"] = 452;
    newmap["Emerald"] = 453;
    newmap["Para Green"] = 454;
    newmap["Akihabara Accept"] = 455;
    newmap["Hydroponic"] = 456;
    newmap["Bamboo Print"] = 457;
    newmap["Bamboo Shadow"] = 458;
    newmap["Bamboo Forest"] = 459;
    newmap["Aqua Terrace"] = 460;
    newmap["Counter Terrace"] = 462;
    newmap["Terrace"] = 463;
    newmap["Neon Kimono"] = 464;
    newmap["Orange Kimono"] = 465;
    newmap["Crimson Kimono"] = 466;
    newmap["Mint Kimono"] = 467;
    newmap["Midnight Storm"] = 468;
    newmap["Sunset Storm 壱"] = 469;
    newmap["Sunset Storm 弐"] = 470;
    newmap["Daybreak"] = 471;
    newmap["Impact Drill"] = 472;
    newmap["Seabird"] = 473;
    newmap["Aquamarine Revenge"] = 474;
    newmap["Hyper Beast"] = 475;
    newmap["Yellow Jacket"] = 476;
    newmap["Neural Net"] = 477;
    newmap["Rocket Pop"] = 478;
    newmap["Bunsen Burner"] = 479;
    newmap["Evil Daimyo"] = 480;
    newmap["Nemesis"] = 481;
    newmap["Ruby Poison Dart"] = 482;
    newmap["Loudmouth"] = 483;
    newmap["Ranger"] = 484;
    newmap["Handgun"] = 485;
    newmap["Elite Build"] = 486;
    newmap["Cyrex"] = 487;
    newmap["Riot"] = 488;
    newmap["Torque"] = 489;
    newmap["Frontside Misty"] = 490;
    newmap["Dualing Dragons"] = 491;
    newmap["Survivor Z"] = 492;
    newmap["Flux"] = 493;
    newmap["Stone Cold"] = 494;
    newmap["Wraiths"] = 495;
    newmap["Nebula Crusader"] = 496;
    newmap["Golden Coil"] = 497;
    newmap["Rangeen"] = 498;
    newmap["Cobalt Core"] = 499;
    newmap["Special Delivery"] = 500;
    newmap["Wingshot"] = 501;
    newmap["Green Marine"] = 502;
    newmap["Big Iron"] = 503;
    newmap["Kill Confirmed"] = 504;
    newmap["Scumbria"] = 505;
    newmap["Point Disarray"] = 506;
    newmap["Ricochet"] = 507;
    newmap["Fuel Rod"] = 508;
    newmap["Corinthian"] = 509;
    newmap["Retrobution"] = 510;
    newmap["The Executioner"] = 511;
    newmap["Royal Paladin"] = 512;
    newmap["Power Loader"] = 514;
    newmap["Imperial"] = 515;
    newmap["Shapewood"] = 516;
    newmap["Yorick"] = 517;
    newmap["Outbreak"] = 518;
    newmap["Tiger Moth"] = 519;
    newmap["Avalanche"] = 520;
    newmap["Teclu Burner"] = 521;
    newmap["Fade"] = 522;
    newmap["Amber Fade"] = 523;
    newmap["Fuel Injector"] = 524;
    newmap["Elite Build"] = 525;
    newmap["Photic Zone"] = 526;
    newmap["Kumicho Dragon"] = 527;
    newmap["Cartel"] = 528;
    newmap["Valence"] = 529;
    newmap["Triumvirate"] = 530;
    newmap["Royal Legion"] = 532;
    newmap["The Battlestar"] = 533;
    newmap["Lapis Gator"] = 534;
    newmap["Praetorian"] = 535;
    newmap["Impire"] = 536;
    newmap["Hyper Beast"] = 537;
    newmap["Necropos"] = 538;
    newmap["Jambiya"] = 539;
    newmap["Lead Conduit"] = 540;
    newmap["Fleet Flock"] = 541;
    newmap["Judgement of Anubis"] = 542;
    newmap["Red Astor"] = 543;
    newmap["Ventilators"] = 544;
    newmap["Orange Crash"] = 545;
    newmap["Firefight"] = 546;
    newmap["Spectre"] = 547;
    newmap["Chantico's Fire"] = 548;
    newmap["Bioleak"] = 549;
    newmap["Oceanic"] = 550;
    newmap["Asiimov"] = 551;
    newmap["Fubar"] = 552;
    newmap["Atlas"] = 553;
    newmap["Ghost Crusader"] = 554;
    newmap["Re-Entry"] = 555;
    newmap["Primal Saber"] = 556;
    newmap["Black Tie"] = 557;
    newmap["Lore"] = 558;
    newmap["Lore"] = 559;
    newmap["Lore"] = 560;
    newmap["Lore"] = 561;
    newmap["Lore"] = 562;
    newmap["Black Laminate"] = 563;
    newmap["Black Laminate"] = 564;
    newmap["Black Laminate"] = 565;
    newmap["Black Laminate"] = 566;
    newmap["Black Laminate"] = 567;
    newmap["Gamma Doppler"] = 568;
    newmap["Gamma Doppler"] = 569;
    newmap["Gamma Doppler"] = 570;
    newmap["Gamma Doppler"] = 571;
    newmap["Gamma Doppler"] = 572;
    newmap["Autotronic"] = 573;
    newmap["Autotronic"] = 574;
    newmap["Autotronic"] = 575;
    newmap["Autotronic"] = 576;
    newmap["Autotronic"] = 577;
    newmap["Bright Water"] = 578;
    newmap["Bright Water"] = 579;
    newmap["Freehand"] = 580;
    newmap["Freehand"] = 581;
    newmap["Freehand"] = 582;
    newmap["Aristocrat"] = 583;
    newmap["Phobos"] = 584;
    newmap["Violent Daimyo"] = 585;
    newmap["Wasteland Rebel"] = 586;
    newmap["Mecha Industries"] = 587;
    newmap["Desolate Space"] = 588;
    newmap["Carnivore"] = 589;
    newmap["Exo"] = 590;
    newmap["Imperial Dragon"] = 591;
    newmap["Iron Clad"] = 592;
    newmap["Chopper"] = 593;
    newmap["Harvester"] = 594;
    newmap["Reboot"] = 595;
    newmap["Limelight"] = 596;
    newmap["Bloodsport"] = 597;
    newmap["Aerial"] = 598;
    newmap["Ice Cap"] = 599;
    newmap["Neon Revolution"] = 600;
    newmap["Syd Mead"] = 601;
    newmap["Imprint"] = 602;
    newmap["Directive"] = 603;
    newmap["Roll Cage"] = 604;
    newmap["Scumbria"] = 605;
    newmap["Ventilator"] = 606;
    newmap["Weasel"] = 607;
    newmap["Petroglyph"] = 608;
    newmap["Airlock"] = 609;
    newmap["Dazzle"] = 610;
    newmap["Grim"] = 611;
    newmap["Powercore"] = 612;
    newmap["Triarch"] = 613;
    newmap["Fuel Injector"] = 614;
    newmap["Briefing"] = 615;
    newmap["Slipstream"] = 616;
    newmap["Doppler"] = 617;
    newmap["Doppler"] = 618;
    newmap["Doppler"] = 619;
    newmap["Ultraviolet"] = 620;
    newmap["Ultraviolet"] = 621;
    newmap["Polymer"] = 622;
    newmap["Ironwork"] = 623;
    newmap["Dragonfire"] = 624;
    newmap["Royal Consorts"] = 625;
    newmap["Mecha Industries"] = 626;
    newmap["Cirrus"] = 627;
    newmap["Stinger"] = 628;
    newmap["Black Sand"] = 629;
    newmap["Sand Scale"] = 630;
    newmap["Flashback"] = 631;
    newmap["Buzz Kill"] = 632;
    newmap["Sonar"] = 633;
    newmap["Gila"] = 634;
    newmap["Turf"] = 635;
    newmap["Shallow Grave"] = 636;
    newmap["Cyrex"] = 637;
    newmap["Wasteland Princess"] = 638;
    newmap["Bloodsport"] = 639;
    newmap["Fever Dream"] = 640;
    newmap["Jungle Slipstream"] = 641;
    newmap["Blueprint"] = 642;
    newmap["Xiangliu"] = 643;
    newmap["Decimator"] = 644;
    newmap["Oxide Blaze"] = 645;
    newmap["Capillary"] = 646;
    newmap["Crimson Tsunami"] = 647;
    newmap["Emerald Poison Dart"] = 648;
    newmap["Akoben"] = 649;
    newmap["Ripple"] = 650;
    newmap["Last Dive"] = 651;
    newmap["Scaffold"] = 652;
    newmap["Neo-Noir"] = 653;
    newmap["Seasons"] = 654;
    newmap["Zander"] = 655;
    newmap["Orbit Mk01"] = 656;
    newmap["Blueprint"] = 657;
    newmap["Cobra Strike"] = 658;
    newmap["Macabre"] = 659;
    newmap["Hyper Beast"] = 660;
    newmap["Sugar Rush"] = 661;
    newmap["Oni Taiji"] = 662;
    newmap["Briefing"] = 663;
    newmap["Hellfire"] = 664;
    newmap["Aloha"] = 665;
    newmap["Hard Water"] = 666;
    newmap["Woodsman"] = 667;
    newmap["Red Rock"] = 668;
    newmap["Death Grip"] = 669;
    newmap["Death's Head"] = 670;
    newmap["Cut Out"] = 671;
    newmap["Metal Flowers"] = 672;
    newmap["Morris"] = 673;
    newmap["Triqua"] = 674;
    newmap["The Empress"] = 675;
    newmap["High Roller"] = 676;
    newmap["Hunter"] = 677;
    newmap["See Ya Later"] = 678;
    newmap["Goo"] = 679;
    newmap["Off World"] = 680;
    newmap["Leaded Glass"] = 681;
    newmap["Oceanic"] = 682;
    newmap["Llama Cannon"] = 683;
    newmap["Cracked Opal"] = 684;
    newmap["Jungle Slipstream"] = 685;
    newmap["Phantom"] = 686;
    newmap["Tacticat"] = 687;
    newmap["Exposure"] = 688;
    newmap["Ziggy"] = 689;
    newmap["Stymphalian"] = 690;
    newmap["Mortis"] = 691;
    newmap["Night Riot"] = 692;
    newmap["Flame Test"] = 693;
    newmap["Moonrise"] = 694;
    newmap["Neo-Noir"] = 695;
    newmap["Bloodsport"] = 696;
    newmap["Black Sand"] = 697;
    newmap["Lionfish"] = 698;
    newmap["Wild Six"] = 699;
    newmap["Urban Hazard"] = 700;
    newmap["Grip"] = 701;
    newmap["Aloha"] = 702;
    newmap["SWAG-7"] = 703;
    newmap["Arctic Wolf"] = 704;
    newmap["Cortex"] = 705;
    newmap["Oxide Blaze"] = 706;
    newmap["Neon Rider"] = 707;
    newmap["Amber Slipstream"] = 708;
    newmap["Eco"] = 709;
    newmap["Shred"] = 710;
    newmap["Code Red"] = 711;
    newmap["High Seas"] = 712;
    newmap["Warhawk"] = 713;
    newmap["Nightmare"] = 714;
    newmap["Capillary"] = 715;
    newmap["Toy Soldier"] = 716;
    newmap["Traction"] = 717;
    newmap["PAW"] = 718;
    newmap["Powercore"] = 719;
    newmap["Devourer"] = 720;
    newmap["Survivalist"] = 721;
    newmap["Snek-9"] = 722;
    newmap["Eye of Athena"] = 723;
    newmap["Wild Lotus"] = 724;
    newmap["Day Lily"] = 725;
    newmap["Sunset Lily"] = 726;
    newmap["Midnight Lily"] = 727;
    newmap["Teal Blossom"] = 728;
    newmap["Crimson Blossom"] = 729;
    newmap["Dark Blossom"] = 730;
    newmap["Banana Leaf"] = 731;
    newmap["Synth Leaf"] = 732;
    newmap["Rust Leaf"] = 733;
    newmap["Wild Lily"] = 734;
    newmap["Night Stripe"] = 735;
    newmap["The Prince"] = 736;
    newmap["Cinquedea"] = 737;
    newmap["Orange Murano"] = 738;
    newmap["Violet Murano"] = 739;
    newmap["Navy Murano"] = 740;
    newmap["Dark Filigree"] = 741;
    newmap["Red Filigree"] = 742;
    newmap["Orange Filigree"] = 743;
    newmap["Baroque Red"] = 744;
    newmap["Baroque Purple"] = 745;
    newmap["Baroque Orange"] = 746;
    newmap["Twin Turbo"] = 747;
    newmap["Calf Skin"] = 748;
    newmap["Vino Primo"] = 749;
    newmap["Integrale"] = 750;
    newmap["Hand Brake"] = 751;
    newmap["Fade"] = 752;
    newmap["Dirt Drop"] = 753;
    newmap["Rust Coat"] = 754;
    newmap["Slide"] = 755;
    newmap["Gungnir"] = 756;
    newmap["Emerald Jörmungandr"] = 757;
    newmap["Flame Jörmungandr"] = 758;
    newmap["Astral Jörmungandr"] = 759;
    newmap["Frost Borre"] = 760;
    newmap["Copper Borre"] = 761;
    newmap["Red Stone"] = 762;
    newmap["Mjölnir"] = 763;
    newmap["Facility Sketch"] = 775;
    newmap["Facility Negative"] = 776;
    newmap["Facility Draft"] = 777;
    newmap["Facility Dark"] = 778;
    newmap["Random Access"] = 779;
    newmap["Mainframe"] = 780;
    newmap["Co-Processor"] = 781;
    newmap["Motherboard"] = 782;
    newmap["Bulkhead"] = 783;
    newmap["Coolant"] = 784;
    newmap["Mandrel"] = 785;
    newmap["Exchanger"] = 786;
    newmap["Core Breach"] = 787;
    newmap["Acheron"] = 788;
    newmap["Nuclear Garden"] = 789;
    newmap["Cold Fusion"] = 790;
    newmap["Remote Control"] = 791;
    newmap["Control Panel"] = 792;
    newmap["Converter"] = 793;
    newmap["Sweeper"] = 794;
    newmap["Safety Net"] = 795;
    newmap["Check Engine"] = 796;
    newmap["Brake Light"] = 797;
    newmap["Nitro"] = 798;
    newmap["High Beam"] = 799;
    newmap["Lab Rats"] = 800;
    newmap["Asiimov"] = 801;
    newmap["Momentum"] = 802;
    newmap["Neo-Noir"] = 803;
    newmap["Modest Threat"] = 804;
    newmap["Mecha Industries"] = 805;
    newmap["Scavenger"] = 806;
    newmap["Signal"] = 807;
    newmap["Oxide Blaze"] = 808;
    newmap["Wood Fired"] = 809;
    newmap["Phosphor"] = 810;
    newmap["Magnesium"] = 811;
    newmap["Pipe Down"] = 812;
    newmap["Nevermore"] = 813;
    newmap["Black Sand"] = 814;
    newmap["Danger Close"] = 815;
    newmap["Fubar"] = 816;
    newmap["Flashback"] = 817;
    newmap["Crypsis"] = 835;
    newmap["Uncharted"] = 836;
    newmap["Angry Mob"] = 837;
    newmap["Atheris"] = 838;
    newmap["Bamboozle"] = 839;
    newmap["Whitefish"] = 840;
    newmap["Light Rail"] = 841;
    newmap["Akoben"] = 842;
    newmap["Skull Crusher"] = 843;
    newmap["The Emperor"] = 844;
    newmap["Momentum"] = 845;
    newmap["Gauss"] = 846;
    newmap["Mischief"] = 847;
    newmap["Verdigris"] = 848;
    newmap["Off World"] = 849;
    newmap["Incinegator"] = 850;
    newmap["Moonrise"] = 851;
    newmap["Doppler"] = 852;
    newmap["Doppler"] = 853;
    newmap["Doppler"] = 854;
    newmap["Doppler"] = 855;
    newmap["Marble Fade"] = 856;
    newmap["Damascus Steel"] = 857;
    newmap["Damascus Steel"] = 858;
    newmap["Emerald Quartz"] = 859;
    newmap["Pyre"] = 860;
    newmap["Barricade"] = 861;
    newmap["Moss Quartz"] = 862;
    newmap["Night Borre"] = 863;
    newmap["Candy Apple"] = 864;
    newmap["Stone Mosaico"] = 865;
    newmap["Canal Spray"] = 866;
    newmap["Stained Glass"] = 867;
    newmap["Sea Calico"] = 868;
    newmap["Sundown"] = 869;
    newmap["Jungle Thicket"] = 870;
    newmap["Surfwood"] = 871;
    newmap["Bamboo Garden"] = 872;
    newmap["Seabird"] = 873;
    newmap["Embargo"] = 884;
    newmap["Rat Rod"] = 885;
    newmap["Arctic Wolf"] = 886;
    newmap["Containment Breach"] = 887;
    newmap["Acid Wash"] = 888;
    newmap["Decimator"] = 889;
    newmap["Plume"] = 890;
    newmap["Black Sand"] = 891;
    newmap["Memento"] = 892;
    newmap["Neon Ply"] = 893;
    newmap["Obsidian"] = 894;
    newmap["Balance"] = 895;
    newmap["Torn"] = 896;
    newmap["Colony IV"] = 897;
    newmap["Stalker"] = 898;
    newmap["Bloodshot"] = 899;
    newmap["Warbird"] = 900;
    newmap["Aztec"] = 902;
    newmap["Elite 1.6"] = 903;
    newmap["Decommissioned"] = 904;
    newmap["Flash Out"] = 905;
    newmap["Buddy"] = 906;
    newmap["Inferno"] = 907;
    newmap["Classic Crate"] = 908;
    newmap["Popdog"] = 909;
    newmap["Hydra"] = 910;
    newmap["Nostalgia"] = 911;
    newmap["Death by Puppy"] = 913;
    newmap["Assault"] = 914;
    newmap["Agent"] = 915;
    newmap["Plastique"] = 916;
    newmap["Wildfire"] = 917;
    newmap["Sacrifice"] = 918;
    newmap["Commemoration"] = 919;
    newmap["Boroque Sand"] = 920;
    newmap["Phantom Disruptor"] = 941;
    newmap["Tom Cat"] = 942;
    newmap["Capillary"] = 943;
    newmap["Distressed"] = 944;
    newmap["Blue Ply"] = 945;
    newmap["Player Two"] = 946;
    newmap["Disco Tech"] = 947;
    newmap["Justice"] = 948;
    newmap["Desert Strike"] = 949;
    newmap["Prototype"] = 950;
    newmap["Acid Etched"] = 951;
    newmap["Bone Forged"] = 952;
    newmap["Apocalypto"] = 953;
    newmap["Enforcer"] = 954;
    newmap["Darkwing"] = 955;
    newmap["Fever Dream"] = 956;
    newmap["Bullet Queen"] = 957;
    newmap["Ultralight"] = 958;
    newmap["Legion of Anubis"] = 959;
    newmap["Gnarled"] = 960;
    newmap["Monster Call"] = 961;
    newmap["Printstream"] = 962;
    newmap["Vogue"] = 963;
    newmap["Brother"] = 964;
    newmap["Allure"] = 965;
    newmap["Ol' Rusty"] = 966;
    newmap["Mainframe 001"] = 967;
    newmap["Cassette"] = 968;
    newmap["Freight"] = 969;
    newmap["Entombed"] = 970;
    newmap["Tooth Fairy"] = 971;
    newmap["Connexion"] = 972;
    newmap["Runic"] = 973;
    newmap["Kitbash"] = 974;
    newmap["Exoskeleton"] = 975;
    newmap["Vendetta"] = 976;
    newmap["Cocoa Rampage"] = 977;
    newmap["Dezastre"] = 978;
    newmap["Fairy Tale"] = 979;
    newmap["Digital Mesh"] = 980;
    newmap["Vandal"] = 981;
    newmap["Contaminant"] = 982;
    newmap["Deep Relief"] = 983;
    newmap["Printstream"] = 984;
    newmap["Cyber Security"] = 985;
    newmap["Condition Zero"] = 986;
    newmap["Clear Polymer"] = 987;
    newmap["Neo-Noir"] = 988;
    newmap["Parallax"] = 989;
    newmap["Gold Bismuth"] = 990;
    newmap["Monster Mashup"] = 991;
    newmap["The Bronze"] = 992;
    newmap["Global Offensive"] = 993;
    newmap["Charter"] = 994;
    newmap["Surveillance"] = 995;
    newmap["Threat Detected"] = 996;
    newmap["Dispatch"] = 997;
    newmap["Switch Board"] = 998;
    newmap["Prime Conspiracy"] = 999;
    newmap["Run and Hide"] = 1000;
    newmap["Welcome to the Jungle"] = 1001;
    newmap["Berries And Cherries"] = 1002;
    newmap["Crime Scene"] = 1003;
    newmap["X-Ray"] = 1004;
    newmap["Heist"] = 1005;
    newmap["Night Heist"] = 1006;
    newmap["Vault Heist"] = 1007;
    newmap["Houndstooth"] = 1008;
    newmap["Hot Snakes"] = 1009;
    newmap["Phoenix Chalk"] = 1010;
    newmap["Phoenix Marker"] = 1011;
    newmap["Phoenix Stencil"] = 1012;
    newmap["Phoenix Blacklight"] = 1013;
    newmap["Clay Ambush"] = 1014;
    newmap["Tiger Pit"] = 1015;
    newmap["Franklin"] = 1016;
    newmap["Blue Phosphor"] = 1017;
    newmap["Panthera onca"] = 1018;
    newmap["Panther Camo"] = 1019;
    newmap["Ancient Earth"] = 1020;
    newmap["Ancient Lore"] = 1021;
    newmap["Lush Ruins"] = 1022;
    newmap["Tall Grass"] = 1023;
    newmap["Blast From the Past"] = 1024;
    newmap["Gold Brick"] = 1025;
    newmap["Fade"] = 1026;
    newmap["Target Acquired"] = 1027;
    newmap["Magna Carta"] = 1028;
    newmap["Silk Tiger"] = 1029;
    newmap["Bengal Tiger"] = 1030;
    newmap["Ancient Visions"] = 1031;
    newmap["Dusk Ruins"] = 1032;
    newmap["Carved Jade"] = 1033;
    newmap["Ancient Ritual"] = 1034;
    newmap["Charred"] = 10006;
    newmap["Snakebite"] = 10007;
    newmap["Bronzed"] = 10008;
    newmap["Leather"] = 10009;
    newmap["Spruce DDPAT"] = 10010;
    newmap["Lunar Weave"] = 10013;
    newmap["Convoy"] = 10015;
    newmap["Crimson Weave"] = 10016;
    newmap["Superconductor"] = 10018;
    newmap["Arid"] = 10019;
    newmap["Slaughter"] = 10021;
    newmap["Eclipse"] = 10024;
    newmap["Spearmint"] = 10026;
    newmap["Boom!"] = 10027;
    newmap["Cool Mint"] = 10028;
    newmap["Forest DDPAT"] = 10030;
    newmap["Crimson Kimono"] = 10033;
    newmap["Emerald Web"] = 10034;
    newmap["Foundation"] = 10035;
    newmap["Badlands"] = 10036;
    newmap["Pandora's Box"] = 10037;
    newmap["Hedge Maze"] = 10038;
    newmap["Guerrilla"] = 10039;
    newmap["Diamondback"] = 10040;
    newmap["King Snake"] = 10041;
    newmap["Imperial Plaid"] = 10042;
    newmap["Overtake"] = 10043;
    newmap["Racing Green"] = 10044;
    newmap["Amphibious"] = 10045;
    newmap["Bronze Morph"] = 10046;
    newmap["Omega"] = 10047;
    newmap["Vice"] = 10048;
    newmap["POW!"] = 10049;
    newmap["Turtle"] = 10050;
    newmap["Transport"] = 10051;
    newmap["Polygon"] = 10052;
    newmap["Cobalt Skulls"] = 10053;
    newmap["Overprint"] = 10054;
    newmap["Duct Tape"] = 10055;
    newmap["Arboreal"] = 10056;
    newmap["Emerald"] = 10057;
    newmap["Mangrove"] = 10058;
    newmap["Rattler"] = 10059;
    newmap["Case Hardened"] = 10060;
    newmap["Crimson Web"] = 10061;
    newmap["Buckshot"] = 10062;
    newmap["Fade"] = 10063;
    newmap["Mogul"] = 10064;
    newmap["Marble Fade"] = 10065;
    newmap["Lt. Commander"] = 10066;
    newmap["Tiger Strike"] = 10067;
    newmap["Field Agent"] = 10068;
    newmap["Rezan the Red"] = 10069;
    newmap["Snow Leopard"] = 10070;
    newmap["Queen Jaguar"] = 10071;
    newmap["Black Tie"] = 10072;
    newmap["Slingshot"] = 10073;
    newmap["Big Game"] = 10074;
    newmap["Scarlet Shamagh"] = 10075;
    newmap["Nocts"] = 10076;
    newmap["Finish Line"] = 10077;
    newmap["Smoke Out"] = 10078;
    newmap["Blood Pressure"] = 10079;
    newmap["3rd Commando Company"] = 10080;
    newmap["Desert Shamagh"] = 10081;
    newmap["Giraffe"] = 10082;
    newmap["Constrictor"] = 10083;
    newmap["CAUTION!"] = 10084;
    newmap["Jade"] = 10085;
    newmap["Yellow-banded"] = 10086;
    newmap["Needle Point"] = 10087;
    newmap["Unhinged"] = 10088;

    return newmap;
}

void SkinChanger::Apply(Entity* Ent)
{
    Entity* weapon = G::LocalPlayer->GetActiveWeapon();
    if (!weapon) return;

    WeaponId weaponID = weapon->GetWeaponId();
    int weaponIDX = GetWeaponIndex(weaponID);

    // If the skin isn't to be changed...
    std::map<int, int>::iterator it;
    it = CurSkins.find(weaponIDX);
    if (it == CurSkins.end())
        return;

    // Change the SKIN!
    ForceSkin(weapon, CurSkins[weaponIDX]);
}

SkinChanger::SkinChanger()
{
    // Set all CurSkins to defualt by defualt
    for (int i = 0; i < 34; i++)
        CurSkins.emplace(i, 0);

    Skins = GetSkins();

}

void SkinChanger::SetVal(int idx, std::string SkinName)
{
    // If invalid index
    if (idx >= 34)
        return;

    // set new Value
    CurSkins[idx] = Skins[SkinName];
}

void SkinChanger::Menu()
{
    if (Config::GetBool("show-console"))
    {
        ImGui::Begin("SkinChanger");

        const char* items1[] = { "None", "Glock", "Hkp2000", "Usp_s", "Elite", "P250", "Tec9", "Fiveseven", "Cz75a", "Deagle", "Revolver", "Nova", "Xm1014", "Sawedoff", "Mag7", "M249", "Negev", "Mac10", "Mp9", "Mp7", "Mp5sd", "Ump45", "P90", "Bizon", "GalilAr", "Famas", "Ak47", "M4A1", "M4a1_s", "Ssg08", "Sg553", "Aug", "Awp", "G3SG1", "Scar20" };
        static const char* current_item1 = items1[0];
        static int chosen = -1;
        {
            if (ImGui::BeginCombo("Gun Combo##GUNS", current_item1)) // The second parameter is the label previewed before opening the combo.
            {
                for (int n = 0; n < IM_ARRAYSIZE(items1); n++)
                {
                    bool is_selected = (current_item1 == items1[n]); // You can store your selection however you want, outside or inside your objects
                    if (ImGui::Selectable(items1[n], is_selected))
                    {
                        chosen = n;
                        current_item1 = items1[n];
                    }
                        
                    if (is_selected) {
                       
                        ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                    }
                       
                }
                ImGui::EndCombo();
            }
        }

        const char* items2[] = { "None", "Groundwater", "Candy Apple", "Forest DDPAT", "Arctic Camo", "Desert Storm", "Bengal Tiger", "Copperhead", "Skulls", "Crimson Web", "Blue Streak", "Red Laminate", "Gunsmoke", "Jungle Tiger", "Urban DDPAT", "Virus", "Granite Marbleized", "Contrast Spray", "Forest Leaves", "Lichen Dashed", "Bone Mask", "Anodized Navy", "Snake Camo", "Silver", "Hot Rod", "Metallic DDPAT", "Ossified", "Blaze", "Fade", "Bulldozer", "Night", "Copper", "Blue Steel", "Stained", "Case Hardened", "Contractor", "Colony", "Dragon Tattoo", "Lightning Strike", "Slaughter", "Dark Water", "Hypnotic", "Bloomstick", "Cold Blooded", "Carbon Fiber", "Scorpion", "Safari Mesh", "Wings", "Polar Camo", "Blizzard Marbleized", "Winter Forest", "Boreal Forest", "Forest Night", "Orange DDPAT", "Pink DDPAT", "Mudder", "Cyanospatter", "Caramel", "Grassland", "Blue Spruce", "Ultraviolet", "Sand Dune", "Storm", "Tornado", "Whiteout", "Grassland Leaves", "Polar Mesh", "Condemned", "Glacier Mesh", "Sand Mesh", "Sage Spray", "Jungle Spray", "Sand Spray", "X-Ray", "Urban Perforated", "Waves Perforated", "Orange Peel", "Urban Masked", "Jungle Dashed", "Sand Dashed", "Urban Dashed", "Jungle", "Demolition", "Afterimage", "Bullet Rain", "Death by Kitty", "Palm", "Walnut", "Brass", "Splash", "Modern Hunter", "Splash Jam", "Blaze Orange", "Radiation Hazard", "Nuclear Threat", "Fallout Warning", "Predator", "Irradiated Alert", "Black Laminate", "BOOM", "Scorched", "Faded Zebra", "Memento", "Doomkitty", "Nuclear Threat", "Fire Serpent", "Corticera", "Emerald Dragon", "Overgrowth", "Corticera", "Golden Koi", "Wave Spray", "Zirka", "Graven", "Bright Water", "Black Limba", "Tempest", "Shattered", "Bone Pile", "Spitfire", "Demeter", "Emerald", "Anodized Navy", "Hazard", "Dry Season", "Mayan Dreams", "Palm", "Jungle DDPAT", "Rust Coat", "Mosaico", "Jungle", "Tornado", "Facets", "Sand Dune", "Groundwater", "Anodized Gunmetal", "Ocean Foam", "Graphite", "Ocean Foam", "Graphite", "X-Ray", "Blue Titanium", "Blood Tiger", "Hexane", "Hive", "Hemoglobin", "Serum", "Blood in the Water", "Nightshade", "Water Sigil", "Ghost Camo", "Blue Laminate", "Electric Hive", "Blind Spot", "Azure Zebra", "Steel Disruption", "Cobalt Disruption", "Crimson Web", "Tropical Storm", "Ash Wood", "VariCamo", "Night Ops", "Urban Rubble", "VariCamo Blue", "CaliCamo", "Hunting Blind", "Army Mesh", "Gator Mesh", "Teardown", "Army Recon", "Amber Fade", "Damascus Steel", "Red Quartz", "Cobalt Quartz", "Full Stop", "Pit Viper", "Silver Quartz", "Acid Fade", "Nitro", "Asiimov", "The Kraken", "Guardian", "Mehndi", "Redline", "Pulse", "Marina", "Rose Iron", "Rising Skull", "Sandstorm", "Kami", "Magma", "Cobalt Halftone", "Tread Plate", "The Fuschia Is Now", "Victoria", "Undertow", "Titanium Bit", "Heirloom", "Copper Galaxy", "Red FragCam", "Panther", "Stainless", "Blue Fissure", "Asiimov", "Chameleon", "Corporal", "Redline", "Trigon", "Heat", "Terrain", "Antique", "Pulse", "Sergeant", "Sandstorm", "Guardian", "Heaven Guard", "Death Rattle", "Green Apple", "Franklin", "Meteorite", "Tuxedo", "Army Sheen", "Caged Steel", "Emerald Pinstripe", "Atomic Alloy", "Vulcan", "Isaac", "Slashed", "Torque", "Antique", "Retribution", "Kami", "Howl", "Curse", "Desert Warfare", "Cyrex", "Orion", "Heaven Guard", "Poison Dart", "Jaguar", "Bratatat", "Road Rash", "Detour", "Red Python", "Master Piece", "Nitro", "Rust Coat", "Chalice", "Knight", "Chainmail", "Hand Cannon", "Dark Age", "Briar", "Royal Blue", "Indigo", "Twist", "Module", "Desert-Strike", "Tatter", "Pulse", "Caiman", "Jet Set", "First Class", "Leather", "Commuter", "Dragon Lore", "First Class", "Coach Class", "Pilot", "Red Leather", "Osiris", "Tigris", "Conspiracy", "Fowl Play", "Water Elemental", "Urban Hazard", "Desert-Strike", "Koi", "Ivory", "Supernova", "Asiimov", "Cyrex", "Abyss", "Labyrinth", "Traveler", "Business Class", "Olive Plaid", "Green Plaid", "Reactor", "Setting Sun", "Nuclear Waste", "Bone Machine", "Styx", "Nuclear Garden", "Contamination", "Toxic", "Radiation Hazard", "Chemical Green", "Hot Shot", "Fallout Warning", "Cerberus", "Wasteland Rebel", "Grinder", "Murky", "Basilisk", "Griffin", "Firestarter", "Dart", "Urban Hazard", "Cartel", "Fire Elemental", "Highwayman", "Cardiac", "Delusion", "Tranquility", "Cartel", "Man-o'-war", "Urban Shock", "Naga", "Chatterbox", "Catacombs", "龍王 (Dragon King)", "System Lock", "Malachite", "Deadly Poison", "Muertos", "Serenity", "Grotto", "Quicksilver", "Tiger Tooth", "Damascus Steel", "Damascus Steel", "Marble Fade", "Rust Coat", "Doppler", "Doppler", "Doppler", "Doppler", "Doppler", "Doppler", "Doppler", "Elite Build", "Armor Core", "Worm God", "Bronze Deco", "Valence", "Monkey Business", "Eco", "Djinn", "Hyper Beast", "Heat", "Man-o'-war", "Neon Rider", "Origami", "Pole Position", "Grand Prix", "Twilight Galaxy", "Chronos", "Hades", "Icarus Fell", "Minotaur's Labyrinth", "Asterion", "Pathfinder", "Daedalus", "Hot Rod", "Medusa", "Duelist", "Pandora's Box", "Poseidon", "Moon in Libra", "Sun in Leo", "Shipping Forecast", "Emerald", "Para Green", "Akihabara Accept", "Hydroponic", "Bamboo Print", "Bamboo Shadow", "Bamboo Forest", "Aqua Terrace", "Counter Terrace", "Terrace", "Neon Kimono", "Orange Kimono", "Crimson Kimono", "Mint Kimono", "Midnight Storm", "Sunset Storm 壱", "Sunset Storm 弐", "Daybreak", "Impact Drill", "Seabird", "Aquamarine Revenge", "Hyper Beast", "Yellow Jacket", "Neural Net", "Rocket Pop", "Bunsen Burner", "Evil Daimyo", "Nemesis", "Ruby Poison Dart", "Loudmouth", "Ranger", "Handgun", "Elite Build", "Cyrex", "Riot", "Torque", "Frontside Misty", "Dualing Dragons", "Survivor Z", "Flux", "Stone Cold", "Wraiths", "Nebula Crusader", "Golden Coil", "Rangeen", "Cobalt Core", "Special Delivery", "Wingshot", "Green Marine", "Big Iron", "Kill Confirmed", "Scumbria", "Point Disarray", "Ricochet", "Fuel Rod", "Corinthian", "Retrobution", "The Executioner", "Royal Paladin", "Power Loader", "Imperial", "Shapewood", "Yorick", "Outbreak", "Tiger Moth", "Avalanche", "Teclu Burner", "Fade", "Amber Fade", "Fuel Injector", "Elite Build", "Photic Zone", "Kumicho Dragon", "Cartel", "Valence", "Triumvirate", "Royal Legion", "The Battlestar", "Lapis Gator", "Praetorian", "Impire", "Hyper Beast", "Necropos", "Jambiya", "Lead Conduit", "Fleet Flock", "Judgement of Anubis", "Red Astor", "Ventilators", "Orange Crash", "Firefight", "Spectre", "Chantico's Fire", "Bioleak", "Oceanic", "Asiimov", "Fubar", "Atlas", "Ghost Crusader", "Re-Entry", "Primal Saber", "Black Tie", "Lore", "Lore", "Lore", "Lore", "Lore", "Black Laminate", "Black Laminate", "Black Laminate", "Black Laminate", "Black Laminate", "Gamma Doppler", "Gamma Doppler", "Gamma Doppler", "Gamma Doppler", "Gamma Doppler", "Autotronic", "Autotronic", "Autotronic", "Autotronic", "Autotronic", "Bright Water", "Bright Water", "Freehand", "Freehand", "Freehand", "Aristocrat", "Phobos", "Violent Daimyo", "Wasteland Rebel", "Mecha Industries", "Desolate Space", "Carnivore", "Exo", "Imperial Dragon", "Iron Clad", "Chopper", "Harvester", "Reboot", "Limelight", "Bloodsport", "Aerial", "Ice Cap", "Neon Revolution", "Syd Mead", "Imprint", "Directive", "Roll Cage", "Scumbria", "Ventilator", "Weasel", "Petroglyph", "Airlock", "Dazzle", "Grim", "Powercore", "Triarch", "Fuel Injector", "Briefing", "Slipstream", "Doppler", "Doppler", "Doppler", "Ultraviolet", "Ultraviolet", "Polymer", "Ironwork", "Dragonfire", "Royal Consorts", "Mecha Industries", "Cirrus", "Stinger", "Black Sand", "Sand Scale", "Flashback", "Buzz Kill", "Sonar", "Gila", "Turf", "Shallow Grave", "Cyrex", "Wasteland Princess", "Bloodsport", "Fever Dream", "Jungle Slipstream", "Blueprint", "Xiangliu", "Decimator", "Oxide Blaze", "Capillary", "Crimson Tsunami", "Emerald Poison Dart", "Akoben", "Ripple", "Last Dive", "Scaffold", "Neo-Noir", "Seasons", "Zander", "Orbit Mk01", "Blueprint", "Cobra Strike", "Macabre", "Hyper Beast", "Sugar Rush", "Oni Taiji", "Briefing", "Hellfire", "Aloha", "Hard Water", "Woodsman", "Red Rock", "Death Grip", "Death's Head", "Cut Out", "Metal Flowers", "Morris", "Triqua", "The Empress", "High Roller", "Hunter", "See Ya Later", "Goo", "Off World", "Leaded Glass", "Oceanic", "Llama Cannon", "Cracked Opal", "Jungle Slipstream", "Phantom", "Tacticat", "Exposure", "Ziggy", "Stymphalian", "Mortis", "Night Riot", "Flame Test", "Moonrise", "Neo-Noir", "Bloodsport", "Black Sand", "Lionfish", "Wild Six", "Urban Hazard", "Grip", "Aloha", "SWAG-7", "Arctic Wolf", "Cortex", "Oxide Blaze", "Neon Rider", "Amber Slipstream", "Eco", "Shred", "Code Red", "High Seas", "Warhawk", "Nightmare", "Capillary", "Toy Soldier", "Traction", "PAW", "Powercore", "Devourer", "Survivalist", "Snek-9", "Eye of Athena", "Wild Lotus", "Day Lily", "Sunset Lily", "Midnight Lily", "Teal Blossom", "Crimson Blossom", "Dark Blossom", "Banana Leaf", "Synth Leaf", "Rust Leaf", "Wild Lily", "Night Stripe", "The Prince", "Cinquedea", "Orange Murano", "Violet Murano", "Navy Murano", "Dark Filigree", "Red Filigree", "Orange Filigree", "Baroque Red", "Baroque Purple", "Baroque Orange", "Twin Turbo", "Calf Skin", "Vino Primo", "Integrale", "Hand Brake", "Fade", "Dirt Drop", "Rust Coat", "Slide", "Gungnir", "Emerald Jörmungandr", "Flame Jörmungandr", "Astral Jörmungandr", "Frost Borre", "Copper Borre", "Red Stone", "Mjölnir", "Facility Sketch", "Facility Negative", "Facility Draft", "Facility Dark", "Random Access", "Mainframe", "Co-Processor", "Motherboard", "Bulkhead", "Coolant", "Mandrel", "Exchanger", "Core Breach", "Acheron", "Nuclear Garden", "Cold Fusion", "Remote Control", "Control Panel", "Converter", "Sweeper", "Safety Net", "Check Engine", "Brake Light", "Nitro", "High Beam", "Lab Rats", "Asiimov", "Momentum", "Neo-Noir", "Modest Threat", "Mecha Industries", "Scavenger", "Signal", "Oxide Blaze", "Wood Fired", "Phosphor", "Magnesium", "Pipe Down", "Nevermore", "Black Sand", "Danger Close", "Fubar", "Flashback", "Crypsis", "Uncharted", "Angry Mob", "Atheris", "Bamboozle", "Whitefish", "Light Rail", "Akoben", "Skull Crusher", "The Emperor", "Momentum", "Gauss", "Mischief", "Verdigris", "Off World", "Incinegator", "Moonrise", "Doppler", "Doppler", "Doppler", "Doppler", "Marble Fade", "Damascus Steel", "Damascus Steel", "Emerald Quartz", "Pyre", "Barricade", "Moss Quartz", "Night Borre", "Candy Apple", "Stone Mosaico", "Canal Spray", "Stained Glass", "Sea Calico", "Sundown", "Jungle Thicket", "Surfwood", "Bamboo Garden", "Seabird", "Embargo", "Rat Rod", "Arctic Wolf", "Containment Breach", "Acid Wash", "Decimator", "Plume", "Black Sand", "Memento", "Neon Ply", "Obsidian", "Balance", "Torn", "Colony IV", "Stalker", "Bloodshot", "Warbird", "Aztec", "Elite 1.6", "Decommissioned", "Flash Out", "Buddy", "Inferno", "Classic Crate", "Popdog", "Hydra", "Nostalgia", "Death by Puppy", "Assault", "Agent", "Plastique", "Wildfire", "Sacrifice", "Commemoration", "Boroque Sand", "Phantom Disruptor", "Tom Cat", "Capillary", "Distressed", "Blue Ply", "Player Two", "Disco Tech", "Justice", "Desert Strike", "Prototype", "Acid Etched", "Bone Forged", "Apocalypto", "Enforcer", "Darkwing", "Fever Dream", "Bullet Queen", "Ultralight", "Legion of Anubis", "Gnarled", "Monster Call", "Printstream", "Vogue", "Brother", "Allure", "Ol' Rusty", "Mainframe 001", "Cassette", "Freight", "Entombed", "Tooth Fairy", "Connexion", "Runic", "Kitbash", "Exoskeleton", "Vendetta", "Cocoa Rampage", "Dezastre", "Fairy Tale", "Digital Mesh", "Vandal", "Contaminant", "Deep Relief", "Printstream", "Cyber Security", "Condition Zero", "Clear Polymer", "Neo-Noir", "Parallax", "Gold Bismuth", "Monster Mashup", "The Bronze", "Global Offensive", "Charter", "Surveillance", "Threat Detected", "Dispatch", "Switch Board", "Prime Conspiracy", "Run and Hide", "Welcome to the Jungle", "Berries And Cherries", "Crime Scene", "X-Ray", "Heist", "Night Heist", "Vault Heist", "Houndstooth", "Hot Snakes", "Phoenix Chalk", "Phoenix Marker", "Phoenix Stencil", "Phoenix Blacklight", "Clay Ambush", "Tiger Pit", "Franklin", "Blue Phosphor", "Panthera onca", "Panther Camo", "Ancient Earth", "Ancient Lore", "Lush Ruins", "Tall Grass", "Blast From the Past", "Gold Brick", "Fade", "Target Acquired", "Magna Carta", "Silk Tiger", "Bengal Tiger", "Ancient Visions", "Dusk Ruins", "Carved Jade", "Ancient Ritual", "Charred", "Snakebite", "Bronzed", "Leather", "Spruce DDPAT", "Lunar Weave", "Convoy", "Crimson Weave", "Superconductor", "Arid", "Slaughter", "Eclipse", "Spearmint", "Boom!", "Cool Mint", "Forest DDPAT", "Crimson Kimono", "Emerald Web", "Foundation", "Badlands", "Pandora's Box", "Hedge Maze", "Guerrilla", "Diamondback", "King Snake", "Imperial Plaid", "Overtake", "Racing Green", "Amphibious", "Bronze Morph", "Omega", "Vice", "POW!", "Turtle", "Transport", "Polygon", "Cobalt Skulls", "Overprint", "Duct Tape", "Arboreal", "Emerald", "Mangrove", "Rattler", "Case Hardened", "Crimson Web", "Buckshot", "Fade", "Mogul", "Marble Fade", "Lt. Commander", "Tiger Strike", "Field Agent", "Rezan the Red", "Snow Leopard", "Queen Jaguar", "Black Tie", "Slingshot", "Big Game", "Scarlet Shamagh", "Nocts", "Finish Line", "Smoke Out", "Blood Pressure", "3rd Commando Company", "Desert Shamagh", "Giraffe", "Constrictor", "CAUTION!", "Jade", "Yellow-banded", "Needle Point", "Unhinged" };
        static const char* current_item2 = items2[0];

        static ImGuiTextFilter filter;
        ImGui::Text("Search:");
        filter.Draw("##searchbar", 340.f);
        ImGui::BeginChild("listbox child", ImVec2(340, 200));
        for (int i = 0; i < IM_ARRAYSIZE(items2); i++) {
            auto paintkit = items2[i];
            if (filter.PassFilter(paintkit)) {
                std::string label = paintkit + (std::string)"###" + std::to_string(i); //do this or you will have problems selecting elements with the same name
                if (ImGui::Selectable(label.c_str()))
                    current_item2 = items2[i];
            }

        }
        ImGui::EndChild();
        
       
        static std::string AppliedGun = "None";
        static std::string AppliedSkin = "Nothing";
        if (ImGui::Button("Apply Skin!"))
        {
            AppliedGun = current_item1;
            AppliedSkin = current_item2;
            SetVal(chosen, current_item2);
        }
        ImGui::Text(std::string("Last applied " + std::string(AppliedSkin) + " to " + AppliedGun).c_str());
       
       
        ImGui::End();
    }
}

void SkinChanger::run(int stage)
{
	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
       
       
        for (int i = 0; i < I::entitylist->GetHighestEntityIndex(); i++) // iterate through everything
        {
            Entity* weapon = (Entity*)I::entitylist->GetClientEntity(i); // define your weapon
            if (!weapon) continue;

            int owner = weapon->GetOwner();
            if (!I::entitylist->GetClientEntityFromHandle((HANDLE)owner)) continue;

            if (I::entitylist->GetClientEntityFromHandle((HANDLE)owner) != G::LocalPlayer) continue;
           
            Apply(weapon);

           /* ForceSkin(weapon, (int)Config::GetFloat("visuals-misc-skin"));*/
           

            //if (!cl::entlist->GetClientEntityFromHandle(*(int*)((DWORD)weapon, offsets.m_hOwnerEntity))) // null check handle, dont check against NULL/0 since a null handle will be 0xFFFFFFFF
            //    continue;
            //if (cl::entlist->GetClientEntityFromHandle(*(int*)((DWORD)weapon, offsets.m_hOwnerEntity)) != localent) // weapon handle isnt yours
            //    continue;
        }
	}
}

