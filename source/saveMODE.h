typedef struct SAVE_MODE_SECT{	
	char gamecode[4];		
	u8 savemode;	
} SAVE_MODE_;
const SAVE_MODE_  __attribute__((aligned(4))) saveMODE_table[] = { 
{"AFZJ",0x11},//0001 - F-Zero(JP).zip
{"AMAJ",0x21},//0002 - Super Mario Advance(JP).zip
{"AREJ",0x11},//0003 - Battle Network Rockman EXE(JP).zip
{"AKRJ",0x11},//0004 - Kurukuru Kururin(JP).zip
{"AD2J",0x11},//0005 - Mr. Driller 2(JP).zip
{"ACRJ",0x32},//0006 - Chu Chu Rocket!(JP).zip
{"AAMJ",0x11},//0007 - Akumajou Dracula - Circle of the Moon(JP).zip
{"AG7J",0x11},//0008 - Advance GTA(JP).zip
{"AKWJ",0x11},//0009 - Konami Wai Wai Racing Advance(JP).zip
{"ADFJ",0x11},//0010 - Bakunetsu Dodge Ball Fighters(JP).zip
{"APBJ",0x11},//0011 - Pinobee no Daibouken(JP).zip
{"AP3J",0x11},//0012 - Power Pro Kun Pocket 3(JP).zip
{"ANPJ",0x11},//0013 - Napoleon(JP).zip
{"AGTJ",0x32},//0014 - Zen-Nippon GT Senshuken(JP).zip
{"AFPJ",0x32},//0015 - Fire Pro Wrestling A(JP).zip
{"AMJJ",0x11},//0016 - Tweety no Hearty Party(JP).zip
{"AGRJ",0x32},//0017 - JGTO Kounin Golf Master - Japan Golf Tour Game(JP).zip
{"AWPJ",0x32},//0018 - Winning Post(JP).zip
{"ASHJ",0x11},//0019 - Silent Hill - Play Novel(JP).zip
{"AYDJ",0x32},//0020 - Yu-Gi-Oh! Dungeon Dice Monsters(JP).zip
{"ABCJ",0x11},//0021 - Boku ha Koukuu Kanseikan(JP).zip
{"AJPJ",0x11},//0022 - J.League Pocket(JP).zip
{"AMMJ",0x11},//0023 - Momotarou Matsuri(JP).zip
{"AMNJ",0x11},//0024 - Monster Guardians(JP).zip
{"AMZE",0x22},//0025 - Super Mario Advance(UE).zip
{"AFFJ",0x22},//0026 - Final Fight One(JP).zip
{"ABSJ",0x11},//0027 - Bomberman Story(JP).zip
{"AI3E",0x00},//0028 - Iridion 3D(UE).zip
{"APBE",0x11},//0029 - Pinobee - Wings of Adventure(UE).zip
{"AR2E",0x00},//0030 - Ready 2 Rumble Boxing - Round 2(US).zip
{"AKWE",0x11},//0031 - Konami Krazy Racers(US).zip
{"AAMP",0x11},//0032 - Castlevania - Circle of the Moon(EU).zip
{"ATHE",0x21},//0033 - Tony Hawk's Pro Skater 2(UE).zip
{"ACAE",0x00},//0034 - GT Advance - Championship Racing(UE).zip
{"ANME",0x00},//0035 - Namco Museum(US).zip
{"ACRE",0x32},//0036 - Chu Chu Rocket!(US).zip
{"AEJE",0x00},//0037 - Earthworm Jim(UE).zip
{"APFE",0x00},//0038 - Pitfall - The Mayan Adventure(UE).zip
{"ASAE",0x00},//0039 - Army Men Advance(UE).zip
{"ADFE",0x11},//0040 - Super Dodge Ball Advance(US).zip
{"AFZE",0x11},//0041 - F-Zero - Maximum Velocity(UE).zip
{"AFPE",0x32},//0042 - Fire Pro Wrestling(UE).zip
{"AKWP",0x11},//0043 - Konami Krazy Racers(EU).zip
{"AR2P",0x00},//0044 - Ready 2 Rumble Boxing - Round 2(EU).zip
{"AAME",0x11},//0045 - Castlevania - Circle of the Moon(US).zip
{"ARYE",0x21},//0046 - Rayman Advance(US).zip
{"ATOJ",0x32},//0047 - Tactics Ogre Gaiden - The Knight of Lodis(JP).zip
{"AKRP",0x11},//0048 - Kurukuru Kururin(EU).zip
{"ATCP",0x22},//0049 - Top Gear GT Championship(EU).zip
{"AMAE",0x22},//0050 - Super Mario Advance(UE).zip
{"ARYP",0x21},//0051 - Rayman Advance(EU).zip
{"ATMP",0x11},//0052 - Tweety and the Magic Gems(EU).zip
{"ATHD",0x21},//0053 - Tony Hawk's Pro Skater 2(DE).zip
{"AYRJ",0x11},//0054 - Narikiri Jockey Game - Yuushun Rhapsody(JP).zip
{"AQAJ",0x11},//0055 - Choro Q Advance(JP).zip
{"ATRJ",0x11},//0056 - Toyrobo Force(JP).zip
{"AHPE",0x22},//0057 - Hot Potato!(US).zip
{"ABSE",0x11},//0058 - Bomberman Tournament(UE).zip
{"ATHF",0x21},//0059 - Tony Hawk's Pro Skater 2(FR).zip
{"ABFJ",0x11},//0060 - Breath of Fire - Ryuu no Senshi(JP).zip
{"AY5J",0x11},//0061 - Yu-Gi-Oh! Duel Monsters 5 Expert 1(JP).zip
{"AXRJ",0x22},//0062 - Super Street Fighter II X - Revival(JP).zip
{"AMPJ",0x11},//0063 - Mahjong Keiji(JP).zip
{"APCE",0x00},//0064 - Pac-Man Collection(US).zip
{"ATMF",0x11},//0065 - Titi et les Bijoux Magiques(FR).zip
{"AMOJ",0x11},//0066 - EX Monopoly(JP).zip
{"AB7J",0x11},//0067 - Minna no Shiiku Series 1 - Boku no Kabuto Mushi(JP).zip
{"AGBJ",0x32},//0068 - GetBackers Dakkanya - Jigoku no Scaramouche(JP).zip
{"AJSJ",0x11},//0069 - Space Hexcite - Maetel Legend EX(JP).zip
{"AKLJ",0x22},//0070 - Kaze no Klonoa - Yumemiru Teikoku(JP).zip
{"AMKJ",0x32},//0071 - Mario Kart Advance(JP).zip
{"ADRJ",0x11},//0072 - Doraemon - Midori no Wakusei Dokidoki Daikyuushutsu!(JP).zip
{"ADNE",0x22},//0073 - Jurassic Park III - The DNA Factor(US).zip
{"AK4J",0x11},//0074 - Kinniku Banzuke - Kongou-kun no Daibouken!(JP).zip
{"AGMJ",0x32},//0075 - JGTO Kounin Golf Master Mobile - Japan Golf Tour Game(JP).zip
{"AZTJ",0x32},//0076 - Zero-Tours(JP).zip
{"ASKJ",0x11},//0077 - Sutakomi - Star Communicator(JP).zip
{"AMBJ",0x32},//0078 - Mobile Pro Yakyuu - Kantoku no Saihai(JP).zip
{"AGSJ",0x32},//0079 - Ougon no Taiyou - Hirakareshi Fuuin(JP).zip
{"ADQJ",0x11},//0080 - Dokapon Q - Monster Hunter!(JP).zip
{"AW7J",0x11},//0081 - Minna no Shiiku Series 2 - Boku no Kuwagata(JP).zip
{"AJ3J",0x11},//0082 - Jurassic Park III - Kyouryuu ni Ainiikou!(JP).zip
{"AFFP",0x22},//0083 - Final Fight One(EU).zip
{"AABJ",0x11},//0084 - Super Black Bass Advance(JP).zip
{"AYMJ",0x11},//0085 - Tanbi Musou - Meine Liebe(JP).zip
{"AMKE",0x32},//0086 - Mario Kart - Super Circuit(US).zip
{"AKMJ",0x11},//0087 - Kiwame Mahjong Deluxe - Mirai Senshi 21(JP).zip
{"AFOE",0x00},//0088 - Fortress(UE).zip
{"ADNP",0x22},//0089 - Jurassic Park III - The DNA Factor(EU).zip
{"AWAJ",0x11},//0090 - Wario Land Advance(JP).zip
{"ATMD",0x11},//0091 - Tweety and the Magic Gems(DE).zip
{"AJQJ",0x22},//0092 - Jurassic Park III - Advanced Action(JP).zip
{"AGRE",0x32},//0093 - ESPN Final Round Golf 2002(US).zip
{"ATAE",0x22},//0094 - Tang Tang(US).zip
{"AWRE",0x32},//0095 - Advance Wars(US).zip
{"AFTE",0x00},//0096 - F-14 Tomcat(UE).zip
{"ALHJ",0x22},//0097 - Love Hina Advance - Shukufuku no Kane ha Naru Kana(JP).zip
{"AMIP",0x00},//0098 - Men in Black - The Series(EU).zip
{"AGRP",0x32},//0099 - ESPN Final Round Golf(EU).zip
{"AVIJ",0x32},//0100 - Medarot Navi - Kuwagata(JP).zip
{"ANAJ",0x32},//0101 - Medarot Navi - Kabuto(JP).zip
{"ATLE",0x00},//0102 - Atlantis - The Lost Empire(UE).zip
{"AJ3P",0x11},//0103 - Jurassic Park III - Park Builder(EU).zip
{"ATWE",0x00},//0104 - Tetris Worlds(US).zip
{"AMKP",0x32},//0105 - Mario Kart - Super Circuit(EU).zip
{"ARPJ",0x32},//0106 - Robot Ponkottsu 2 - Ring Version(JP).zip
{"ACVJ",0x32},//0107 - Robot Ponkottsu 2 - Cross Version(JP).zip
{"ABZE",0x00},//0108 - NFL Blitz 20-02(US).zip
{"AGKJ",0x00},//0109 - Gensou Suikoden - Card Stories(JP).zip
{"AXSE",0x22},//0110 - ESPN X-Games Skateboarding(US).zip
{"ASRJ",0x32},//0111 - Super Robot Taisen A(JP).zip
{"ARKE",0x00},//0112 - Rocket Power - Dream Scheme(UE).zip
{"AKLE",0x22},//0113 - Klonoa - Empire of Dreams(US).zip
{"AJ3E",0x11},//0114 - Jurassic Park III - Park Builder(US).zip
{"ASEE",0x00},//0115 - Spider-Man - Mysterio's Menace(UE).zip
{"ACPE",0x00},//0116 - Caesars Palace Advance(UE).zip
{"AF9J",0x11},//0117 - Field of Nine - Digital Edition 2001(JP).zip
{"ABKE",0x22},//0118 - BackTrack(UE).zip
{"AMSJ",0x11},//0119 - Morita Shougi Advance(JP).zip
{"ARGE",0x00},//0120 - Rugrats - Castle Capers(UE).zip
{"APRE",0x00},//0121 - Power Rangers - Time Force(US).zip
{"AZEJ",0x32},//0122 - Z.O.E. 2173 - Testament(JP).zip
{"ALDP",0x22},//0123 - Lady Sia(EU).zip
{"ASSE",0x22},//0124 - High Heat Major League Baseball 2002(UE).zip
{"AXME",0x22},//0125 - X-Men - Reign of Apocalypse(UE).zip
{"ADLE",0x22},//0126 - Dexter's Laboratory - Deesaster Strikes!(US).zip
{"A2XE",0x00},//0127 - MX 2002 featuring Ricky Carmichael(UE).zip
{"AFFE",0x22},//0128 - Final Fight One(US).zip
{"ANBJ",0x32},//0129 - Nobunaga no Yabou(JP).zip
{"ALBE",0x11},//0130 - LEGO Bionicle(US).zip
{"AL2E",0x11},//0131 - LEGO Island 2 - The Brickster's Revenge(US).zip
{"AKIJ",0x00},//0132 - Kiki Kaikai Advance(JP).zip
{"ASQE",0x00},//0133 - Snood(US).zip
{"AKBE",0x11},//0134 - Sports Illustrated for Kids - Baseball(US).zip
{"AHSJ",0x11},//0135 - Hatena Satena(JP).zip
{"AWTE",0x00},//0136 - Wild Thornberrys, The - Chimp Chase(UE).zip
{"ASDE",0x00},//0137 - Scooby-Doo and the Cyber Chase(US).zip
{"AKFE",0x11},//0138 - Sports Illustrated for Kids - Football(US).zip
{"ABOE",0x00},//0139 - Boxing Fever(UE).zip
{"ASBJ",0x11},//0140 - Gyakuten Saiban(JP).zip
{"AL2P",0x11},//0141 - LEGO Island 2 - The Brickster's Revenge(EU).zip
{"ALDE",0x22},//0142 - Lady Sia(US).zip
{"APYJ",0x22},//0143 - Minna de Puyo Puyo(JP).zip
{"ATUP",0x22},//0144 - Total Soccer(EU).zip
{"ADBP",0x22},//0145 - Denki Blocks!(EU).zip
{"APHE",0x00},//0146 - Prehistorik Man(UE).zip
{"AKTJ",0x22},//0147 - Hello Kitty Collection - Miracle Fashion Maker(JP).zip
{"AELP",0x00},//0148 - European Super League(EU).zip
{"APXJ",0x22},//0149 - Phalanx - The Enforce Fighter A-144(JP).zip
{"AKPJ",0x11},//0150 - Nakayoshi Mahjong - Kaburiichi(JP).zip
{"AHBJ",0x11},//0151 - Hamster Monogatari 2 GBA(JP).zip
{"AXSJ",0x22},//0152 - ESPN X-Games Skateboarding(JP).zip
{"ALBP",0x11},//0153 - LEGO Bionicle(EU).zip
{"AMXE",0x00},//0154 - Monsters, Inc.(UE).zip
{"ADME",0x22},//0155 - Doom(UE).zip
{"AYAJ",0x11},//0156 - Dokodemo Taikyoku - Yakuman Advance(JP).zip
{"ATAP",0x22},//0157 - Tang Tang(EU).zip
{"ALLP",0x00},//0158 - Lucky Luke - Wanted!(EU).zip
{"ASYP",0x22},//0159 - Spyro - Season of Ice(EU).zip
{"ASYE",0x22},//0160 - Spyro - Season of Ice(US).zip
{"ABTE",0x00},//0161 - Batman Vengeance(US).zip
{"AXRP",0x22},//0162 - Super Street Fighter II Turbo - Revival(EU).zip
{"AKKE",0x00},//0163 - Kao the Kangaroo(US).zip
{"AWFE",0x00},//0164 - WWF - Road to WrestleMania(UE).zip
{"AREE",0x11},//0165 - Megaman Battle Network(US).zip
{"AQAE",0x11},//0166 - Gadget Racers(US).zip
{"AGAP",0x22},//0167 - Gradius Advance(EU).zip
{"AMLE",0x22},//0168 - M&M's Blast!(US).zip
{"AWAE",0x11},//0169 - Wario Land 4(UE).zip
{"AHKJ",0x11},//0170 - Hikaru no Go(JP).zip
{"AGSE",0x32},//0171 - Golden Sun(UE).zip
{"ADKP",0x00},//0172 - Donald Duck Advance(EU).zip
{"AHRE",0x22},//0173 - Harry Potter and the Sorcerer's Stone(UE).zip
{"ABTP",0x00},//0174 - Batman Vengeance(EU).zip
{"AJCE",0x22},//0175 - Jackie Chan Adventures - Legend of the Darkhand(UE).zip
{"AEVE",0x00},//0176 - Alienators - Evolution Continues(UE).zip
{"ASPE",0x00},//0177 - SpongeBob SquarePants - SuperSponge(US).zip
{"AJNE",0x00},//0178 - Jimmy Neutron - Boy Genius(US).zip
{"AIGP",0x00},//0179 - Inspector Gadget - Advance Mission(EU).zip
{"ATME",0x11},//0180 - Tweety and the Magic Gems(US).zip
{"ASWE",0x00},//0181 - Star Wars - Jedi Power Battles(US).zip
{"AMQE",0x00},//0182 - Midnight Club - Street Racing(US).zip
{"AHOE",0x22},//0183 - Mat Hoffman's Pro BMX(UE).zip
{"ARFE",0x00},//0184 - Razor Freestyle Scooter(US).zip
{"ABMP",0x00},//0185 - Super Bust-A-Move(EU).zip
{"ADQE",0x11},//0186 - Dokapon(US).zip
{"AISP",0x22},//0187 - International Superstar Soccer(EU).zip
{"ATLX",0x00},//0188 - Atlantis - The Lost Empire(EU).zip
{"A4VJ",0x11},//0189 - Yuujou no Victory Goal 4v4 Arashi - Get the Goal!!(JP).zip
{"ANDE",0x00},//0190 - Nancy Drew - Message in a Haunted Mansion(US).zip
{"APTE",0x22},//0191 - Powerpuff Girls, The - Mojo Jojo A-Go-Go(US).zip
{"A2ME",0x22},//0192 - Madden NFL 2002(US).zip
{"AFRE",0x22},//0193 - Frogger's Adventures - Temple of the Frog(US).zip
{"AGPE",0x00},//0194 - No Rules - Get Phat(UE).zip
{"ADEJ",0x11},//0195 - Adventure of Tokyo Disney Sea(JP).zip
{"ABQP",0x22},//0196 - David Beckham Soccer(EU).zip
{"ATWX",0x00},//0197 - Tetris Worlds(EU).zip
{"ARXE",0x00},//0198 - Rampage - Puzzle Attack(UE).zip
{"AM3E",0x00},//0199 - Midway's Greatest Arcade Hits(UE).zip
{"AJQE",0x22},//0200 - Jurassic Park III - Island Attack(US).zip
{"ADFP",0x11},//0201 - Super Dodge Ball Advance(EU).zip
{"ALRE",0x22},//0202 - LEGO Racers 2(US).zip
{"AMGE",0x11},//0203 - ESPN Great Outdoor Games - Bass 2002(US).zip
{"AESE",0x00},//0204 - Ecks vs. Sever(US).zip
{"AHWE",0x22},//0205 - Hot Wheels - Burnin' Rubber(US).zip
{"AKGP",0x22},//0206 - Mech Platoon(EU).zip
{"APXP",0x22},//0207 - Phalanx - The Enforce Fighter A-144(EU).zip
{"AETE",0x00},//0208 - E.T. - The Extra-Terrestrial(US).zip
{"AX2E",0x22},//0209 - Dave Mirra Freestyle BMX 2(US).zip
{"ASCE",0x00},//0210 - Shaun Palmer's Pro Snowboarder(UE).zip
{"APRD",0x00},//0211 - Power Rangers - Time Force(DE).zip
{"ASXJ",0x32},//0212 - Sangokushi(JP).zip
{"AYNP",0x00},//0213 - Planet of the Apes(EU).zip
{"ARWP",0x22},//0214 - Robot Wars - Advanced Destruction(EU).zip
{"ACFE",0x00},//0215 - Cruis'n Velocity(UE).zip
{"ATJE",0x00},//0216 - Tom and Jerry - The Magic Ring(US).zip
{"AMFE",0x11},//0217 - Monster Rancher Advance(US).zip
{"AIKP",0x00},//0218 - International Karate Advanced(EU).zip
{"ADSJ",0x32},//0219 - Daisenryaku(JP).zip
{"AGAE",0x22},//0220 - Gradius Galaxies(US).zip
{"A22J",0x11},//0221 - EZ-Talk - Shokyuu Hen 1(JP).zip
{"A23J",0x11},//0222 - EZ-Talk - Shokyuu Hen 2(JP).zip
{"AMEE",0x00},//0223 - Army Men - Operation Green(UE).zip
{"ADVE",0x22},//0224 - Driven(US).zip
{"AHOX",0x22},//0225 - Mat Hoffman's Pro BMX(EU).zip
{"ABME",0x00},//0226 - Super Bust-A-Move(US).zip
{"ACGP",0x22},//0228 - Columns Crown(EU).zip
{"AJCF",0x22},//0229 - Aventures de Jackie Chan, Les - La Legende de la Main Noire(FR).zip
{"AMVJ",0x32},//0230 - Magical Vacation(JP).zip
{"AR7J",0x11},//0231 - Advance Rally(JP).zip
{"AYSJ",0x22},//0232 - Gakkou wo Tsukurou!! Advance(JP).zip
{"AMFJ",0x11},//0233 - Monster Farm Advance(JP).zip
{"ATNP",0x00},//0234 - Thunderbirds - International Rescue(EU).zip
{"ASCD",0x00},//0238 - Shaun Palmer's Pro Snowboarder(DE).zip
{"AASJ",0x22},//0239 - World Advance Soccer - Shouri heno Michi(JP).zip
{"ANMJ",0x00},//0240 - Namco Museum(JP).zip
{"APME",0x00},//0241 - Planet Monsters(US).zip
{"AA2J",0x22},//0242 - Super Mario Advance 2(JP).zip
{"AE2J",0x11},//0243 - Battle Network Rockman EXE 2(JP).zip
{"ABFE",0x11},//0245 - Breath of Fire(US).zip
{"ACGJ",0x22},//0246 - Columns Crown(JP).zip
{"AM5E",0x00},//0247 - Mortal Kombat Advance(US).zip
{"AJWJ",0x22},//0248 - Jikkyou World Soccer Pocket(JP).zip
{"AREP",0x11},//0249 - Megaman Battle Network(EU).zip
{"AABE",0x22},//0250 - American Bass Challenge(US).zip
{"ASOJ",0x32},//0252 - Sonic Advance(JP).zip
{"AK5J",0x11},//0253 - Kinniku Banzuke - Kimero! Kiseki no Kanzen Seiha(JP).zip
{"AHEJ",0x11},//0254 - Bakuten Shoot Beyblade - Gekitou! Saikyou Blader(JP).zip
{"AFSE",0x00},//0255 - Flintstones, The - Big Trouble in Bedrock(US).zip
{"ATKJ",0x22},//0256 - Tekken Advance(JP).zip
{"AB2J",0x11},//0257 - Breath of Fire II - Shimei no Ko(JP).zip
{"AT2J",0x32},//0258 - Dragon Quest Characters - Torneco no Daibouken 2 Advance - Fushigi no Dungeon(JP).zip
{"ATZJ",0x11},//0259 - Zoids Saga(JP).zip
{"ARHJ",0x11},//0260 - Recca no Honoo - The Game(JP).zip
{"AWXJ",0x22},//0261 - ESPN Winter X-Games Snowboarding 2002(JP).zip
{"AY6J",0x11},//0262 - Yu-Gi-Oh! Duel Monsters 6 Expert 2(JP).zip
{"AWRP",0x32},//0263 - Advance Wars(EU).zip
{"AKKP",0x00},//0264 - Kao the Kangaroo(EU).zip
{"AKOJ",0x22},//0265 - King of Fighters EX, The - NeoBlood(JP).zip
{"AALJ",0x22},//0266 - Kidou Tenshi Angelic Layer - Misaki to Yume no Tenshi-tachi(JP).zip
{"ASNJ",0x22},//0267 - Sansara Naga 1x2(JP).zip
{"A24J",0x11},//0268 - EZ-Talk - Shokyuu Hen 3(JP).zip
{"A25J",0x11},//0269 - EZ-Talk - Shokyuu Hen 4(JP).zip
{"AKGE",0x22},//0272 - Mech Platoon(US).zip
{"ADAE",0x00},//0273 - Dark Arena(UE).zip
{"ACTX",0x22},//0274 - Creatures(EU).zip
{"APCJ",0x00},//0275 - Pac-Man Collection(JP).zip
{"AFSP",0x00},//0276 - Flintstones, The - Big Trouble in Bedrock(EU).zip
{"AEAJ",0x11},//0277 - Snap Kid's(JP).zip
{"AOBP",0x00},//0278 - Asterix & Obelix - Bash Them All!(EU).zip
{"AWXE",0x22},//0279 - ESPN Winter X-Games Snowboarding 2002(US).zip
{"AGXJ",0x32},//0280 - Guilty Gear X - Advance Edition(JP).zip
{"AGAJ",0x22},//0281 - Gradius Generation(JP).zip
{"AGLJ",0x22},//0282 - Tomato Adventure(JP).zip
{"AWIJ",0x22},//0283 - Hyper Sports 2002 Winter(JP).zip
{"ATKE",0x22},//0284 - Tekken Advance(US).zip
{"AWIE",0x22},//0285 - ESPN International Winter Sports 2002(US).zip
{"AS5E",0x22},//0286 - Salt Lake 2002(US).zip
{"APCP",0x00},//0287 - Pac-Man Collection(EU).zip
{"AWZJ",0x11},//0288 - Wizardry Summoner(JP).zip
{"ASVJ",0x22},//0289 - Shanghai Advance(JP).zip
{"ASOE",0x32},//0290 - Sonic Advance(US).zip
{"AIBJ",0x11},//0291 - Guranbo(JP).zip
{"AJME",0x22},//0292 - Jonny Moseley Mad Trix(UE).zip
{"APYE",0x22},//0293 - Puyo Pop(US).zip
{"AOKJ",0x11},//0294 - Okumanchouja Game - Nottori Daisakusen!(JP).zip
{"APNJ",0x22},//0295 - Pinky Monkey Town(JP).zip
{"AHRJ",0x22},//0296 - Harry Potter to Kenja no Ishi(JP).zip
{"AA2E",0x22},//0297 - Super Mario Advance 2 - Super Mario World(US).zip
{"AJQP",0x22},//0298 - Jurassic Park III - Dino Attack(EU).zip
{"AWRE",0x32},//0299 - Advance Wars(US).zip
{"ACGE",0x22},//0300 - Columns Crown(US).zip
{"ACIJ",0x22},//0301 - WTA Tour Tennis Pocket(JP).zip
{"AX2P",0x22},//0302 - Dave Mirra Freestyle BMX 2(EU).zip
{"APPE",0x00},//0303 - Peter Pan - Return to Neverland(US).zip
{"AGSF",0x32},//0304 - Golden Sun(FR).zip
{"AMGP",0x22},//0305 - ESPN Great Outdoor Games - Bass Tournament(EU).zip
{"A26J",0x11},//0306 - EZ-Talk - Shokyuu Hen 5(JP).zip
{"A27J",0x11},//0307 - EZ-Talk - Shokyuu Hen 6(JP).zip
{"ABNE",0x00},//0308 - NBA Jam 2002(UE).zip
{"AKYJ",0x11},//0309 - Captain Tsubasa - Eikou no Kiseki(JP).zip
{"AP9P",0x11},//0310 - Pocket Music(EU).zip
{"AVKP",0x22},//0311 - Virtual Kasparov(EU).zip
{"AHWP",0x22},//0312 - Hot Wheels - Burnin' Rubber(EU).zip
{"ATJP",0x00},//0313 - Tom and Jerry - The Magic Ring(EU).zip
{"APMP",0x00},//0314 - Planet Monsters(EU).zip
{"AJNX",0x00},//0315 - Jimmy Neutron - Boy Genius(EU).zip
{"ACRP",0x32},//0316 - Chu Chu Rocket!(EU).zip
{"AM4P",0x00},//0317 - Moto GP(EU).zip
{"AMHJ",0x22},//0318 - Bomberman Max 2 - Bomberman Version(JP).zip
{"ACQP",0x22},//0319 - Crash Bandicoot XS(EU).zip
{"AGNJ",0x22},//0320 - Goemon - New Age Shutsudou!(JP).zip
{"AJ2J",0x11},//0321 - J.League Pocket 2(JP).zip
{"ALAP",0x00},//0322 - Land Before Time, The(EU).zip
{"AM9P",0x22},//0323 - Mike Tyson Boxing(EU).zip
{"AIGE",0x00},//0324 - Inspector Gadget - Advance Mission(US).zip
{"APXE",0x22},//0325 - Phalanx - The Enforce Fighter A-144(US).zip
{"AZEE",0x32},//0326 - Zone of the Enders - The Fist of Mars(US).zip
{"AFGP",0x00},//0327 - American Tail, An - Fievel's Gold Rush(EU).zip
{"AWEJ",0x11},//0328 - Black Black - Bura Bura(JP).zip
{"AXRE",0x22},//0329 - Super Street Fighter II Turbo - Revival(US).zip
{"AUYJ",0x22},//0330 - Yuureiyashiki no Nijuuyojikan(JP).zip
{"ADOJ",0x11},//0331 - Domo-kun no Fushigi Terebi(JP).zip
{"AMCJ",0x32},//0332 - Mail de Cute(JP).zip
{"AMYJ",0x22},//0333 - Bomberman Max 2 - Max Version(JP).zip
{"AYNE",0x00},//0334 - Planet of the Apes(US).zip
{"AT3E",0x22},//0335 - Tony Hawk's Pro Skater 3(UE).zip
{"AHHE",0x22},//0336 - High Heat Major League Baseball 2003(US).zip
{"ANLE",0x22},//0337 - NHL 2002(US).zip
{"ACSP",0x00},//0338 - Casper(EU).zip
{"ASOP",0x32},//0339 - Sonic Advance(EU).zip
{"AMIE",0x00},//0340 - Men in Black - The Series(US).zip
{"ASWX",0x00},//0341 - Star Wars - Jedi Power Battles(EU).zip
{"ATCE",0x22},//0342 - Top Gear GT Championship(US).zip
{"AADJ",0x00},//0343 - Donald Duck Advance(JP).zip
{"AGDJ",0x11},//0344 - Chinmoku no Iseki - Estpolis Gaiden(JP).zip
{"ACQE",0x22},//0345 - Crash Bandicoot - The Huge Adventure(US).zip
{"AM6E",0x22},//0346 - Mike Tyson Boxing(US).zip
{"AM4E",0x00},//0347 - Moto GP(US).zip
{"AR6E",0x22},//0348 - Tom Clancy's Rainbow Six - Rogue Spear(US).zip
{"AIDE",0x22},//0349 - Space Invaders(UE).zip
{"ASRJ",0x32},//0350 - Super Robot Taisen A(CN).zip
{"ADVP",0x22},//0351 - Driven(EU).zip
{"AKSE",0x22},//0352 - Mary-Kate and Ashley - Girls Night Out(US).zip
{"AGCJ",0x32},//0353 - Guru Logic Champ(JP).zip
{"ADKE",0x00},//0354 - Donald Duck Advance(US).zip
{"AAGJ",0x22},//0355 - Angelique(JP).zip
{"AP4J",0x11},//0356 - Power Pro Kun Pocket 4(JP).zip
{"ABPE",0x11},//0357 - Baseball Advance(US).zip
{"AR6P",0x22},//0358 - Tom Clancy's Rainbow Six - Rogue Spear(EU).zip
{"AKCE",0x00},//0359 - Konami Collector's Series - Arcade Advanced(US).zip
{"AMRE",0x22},//0360 - Motocross Maniacs Advance(US).zip
{"AAMJ",0x11},//0361 - Akumajou Dracula - Circle of the Moon(CN).zip
{"ABJP",0x22},//0362 - Broken Sword - The Shadow of the Templars(EU).zip
{"ARVJ",0x11},//0363 - Groove Adventure Rave - Hikari to Yami no Daikessen(JP).zip
{"AIAE",0x00},//0364 - Ice Age(US).zip
{"AS4E",0x22},//0365 - Shrek - Swamp Kart Speedway(UE).zip
{"ACYP",0x22},//0366 - Chessmaster(EU).zip
{"AGSD",0x32},//0367 - Golden Sun(DE).zip
{"AKVJ",0x22},//0368 - K-1 Pocket Grand Prix(JP).zip
{"ABMJ",0x00},//0369 - Super Puzzle Bobble Advance(JP).zip
{"ASFJ",0x22},//0370 - Slot! Pro Advance - Takarabune & Ooedo Sakurafubuki 2(JP).zip
{"ASMJ",0x11},//0371 - Saibara Rieko no Dendou Mahjong(JP).zip
{"AKGJ",0x22},//0372 - Kikaika Guntai - Mech Platoon(JP).zip
{"ADDJ",0x22},//0373 - Diadroids World - Evil Teikoku no Yabou(JP).zip
{"AJUJ",0x32},//0374 - Jissen Pachi-Slot Hisshouhou! - Juuou Advance(JP).zip
{"AHMJ",0x22},//0375 - Dai-Mahjong(JP).zip
{"ABJE",0x22},//0376 - Broken Sword - The Shadow of the Templars(US).zip
{"ABVP",0x00},//0377 - Maya the Bee - The Great Adventure(EU).zip
{"AFEJ",0x11},//0378 - Fire Emblem - Fuuin no Tsurugi(JP).zip
{"AHUJ",0x32},//0379 - Shining Soul(JP).zip
{"AAVE",0x00},//0380 - Atari Anniversary Advance(US).zip
{"ABYE",0x00},//0381 - Britney's Dance Beat(US).zip
{"AQBP",0x11},//0382 - Scrabble(EU).zip
{"AGSS",0x32},//0383 - Golden Sun(ES).zip
{"AJAE",0x11},//0384 - Monster Jam - Maximum Destruction(US).zip
{"AFLP",0x22},//0385 - FILA Decathlon(EU).zip
{"AJOJ",0x32},//0386 - Magical Houshin(JP).zip
{"AEGP",0x00},//0387 - Extreme Ghostbusters - Code Ecto-1(EU).zip
{"ASZE",0x00},//0388 - Scorpion King, The - Sword of Osiris(US).zip
{"AIEJ",0x11},//0389 - Isseki Hattyou - Kore 1ppon de 8syurui!(JP).zip
{"APPP",0x00},//0390 - Peter Pan - Return to Neverland(EU).zip
{"AABP",0x22},//0391 - Super Black Bass Advance(EU).zip
{"ABFX",0x11},//0392 - Breath of Fire(EU).zip
{"AWSP",0x00},//0393 - Tiny Toon Adventures - Wacky Stackers(EU).zip
{"AKLP",0x22},//0394 - Klonoa - Empire of Dreams(EU).zip
{"A3TE",0x00},//0395 - Three Stooges, The(US).zip
{"AXIP",0x00},//0396 - X-Bladez - Inline Skater(EU).zip
{"AVKE",0x22},//0397 - Virtual Kasparov(US).zip
{"A54J",0x11},//0398 - Koukou Juken Advance Series Eigo Koubun Hen - 26 Units Shuuroku(JP).zip
{"AWOE",0x22},//0399 - Wolfenstein 3-D(US).zip
{"PEAJ",0x32},//0400 - Card e-Reader(JP).zip
{"ALNJ",0x11},//0401 - Lunar Legend(JP).zip
{"AA2P",0x22},//0402 - Super Mario Advance 2 - Super Mario World(EU).zip
{"AB2E",0x11},//0403 - Breath of Fire II(US).zip
{"AKXE",0x22},//0404 - Spider-Man(UE).zip
{"AS8E",0x00},//0405 - Star X(US).zip
{"ASZP",0x00},//0406 - Scorpion King, The - Sword of Osiris(EU).zip
{"ATWJ",0x00},//0407 - Tetris Worlds(JP).zip
{"ASEJ",0x22},//0408 - Spider-Man - Mysterio no Kyoui(JP).zip
{"ABRE",0x11},//0409 - Blender Bros.(US).zip
{"ARZJ",0x11},//0410 - Rockman Zero(JP).zip
{"A2GJ",0x22},//0411 - Advance GT2(JP).zip
{"AEWJ",0x22},//0412 - Ui-Ire - World Soccer Winning Eleven(JP).zip
{"ATIJ",0x32},//0413 - Tennis no Ouji-sama - Genius Boys Academy(JP).zip
{"AINJ",0x11},//0414 - Initial D - Another Stage(JP).zip
{"AQAP",0x11},//0415 - Penny Racers(EU).zip
{"ABGJ",0x22},//0416 - Sweet Cookie Pie(JP).zip
{"AD3E",0x22},//0417 - Dinotopia - The Timestone Pirates(UE).zip
{"KHPJ",0x22},//0418 - Koro Koro Puzzle - Happy Panechu!(JP).zip
{"AGOJ",0x11},//0419 - Kurohige no Golf Shiyouyo(JP).zip
{"ATPJ",0x32},//0420 - Keitai Denjuu Telefang 2 - Power(JP).zip
{"ATSJ",0x32},//0421 - Keitai Denjuu Telefang 2 - Speed(JP).zip
{"AKDJ",0x11},//0422 - Kaeru B Back(JP).zip
{"ADIE",0x00},//0423 - Desert Strike Advance(US).zip
{"AA3E",0x11},//0424 - All-Star Baseball 2003(US).zip
{"ANHE",0x22},//0425 - NASCAR Heat 2002(US).zip
{"AFMJ",0x11},//0426 - Formation Soccer 2002(JP).zip
{"ADPJ",0x22},//0427 - Doraemon - Dokodemo Walker(JP).zip
{"AANJ",0x11},//0428 - Animal Mania - Dokidoki Aishou Check(JP).zip
{"AG2J",0x11},//0429 - Kami no Kijutsu - Illusion of the Evil Eyes(JP).zip
{"AN5J",0x22},//0430 - Kawa no Nushi Tsuri 5 - Fushigi no Mori Kara(JP).zip
{"ACBJ",0x22},//0431 - Gekitou! Car Battler Go!!(JP).zip
{"ATOE",0x32},//0432 - Tactics Ogre - The Knight of Lodis(US).zip
{"AT5E",0x22},//0433 - Tiger Woods PGA Tour Golf(UE).zip
{"ALGE",0x22},//0434 - Dragon Ball Z - The Legacy of Goku(US).zip
{"AHIJ",0x22},//0435 - Hitsuji no Kimochi(JP).zip
{"AZGJ",0x11},//0436 - Jinsei Game Advance(JP).zip
{"AHNE",0x22},//0437 - Spy Hunter(US).zip
{"AD6P",0x22},//0438 - Davis Cup(EU).zip
{"ATFP",0x22},//0439 - Total Soccer Manager(EU).zip
{"ADWE",0x00},//0440 - Downforce(UE).zip
{"AFUJ",0x22},//0441 - Youkaidou(JP).zip
{"ACKE",0x22},//0442 - Backyard Baseball(US).zip
{"AGSI",0x32},//0443 - Golden Sun(IT).zip
{"ATCX",0x22},//0444 - GT Championship(EU).zip
{"AEPP",0x22},//0445 - Sheep(EU).zip
{"ATXP",0x22},//0446 - Next Generation Tennis(EU).zip
{"ADZE",0x11},//0447 - Dragon Ball Z - Collectible Card Game(US).zip
{"AH3J",0x11},//0448 - Tottoko Hamutarou 3 - Love Love Daibouken Dechu(JP).zip
{"AS2E",0x00},//0449 - Star Wars Episode II - Attack of the Clones(US).zip
{"AC6E",0x00},//0450 - Spirit - Stallion of the Cimarron - Search for Homeland(US).zip
{"AJ4E",0x00},//0451 - Earthworm Jim 2(US).zip
{"AMHE",0x22},//0452 - Bomberman Max 2 - Blue Advance(US).zip
{"AMYE",0x22},//0453 - Bomberman Max 2 - Red Advance(US).zip
{"AFQE",0x00},//0454 - Frogger Advance - The Great Quest(US).zip
{"AMWE",0x22},//0455 - Muppet Pinball Mayhem(US).zip
{"AAEE",0x00},//0456 - Hey Arnold! - The Movie(US).zip
{"AKXF",0x22},//0457 - Spider-Man(FR).zip
{"APGE",0x00},//0458 - Punch King - Arcade Boxing(US).zip
{"AGGP",0x00},//0459 - Gremlins - Stripe vs. Gizmo(EU).zip
{"ACHJ",0x11},//0460 - Castlevania - Byakuya no Concerto(JP).zip
{"ANCE",0x22},//0461 - ZooCube(US).zip
{"ALTE",0x00},//0462 - Lilo & Stitch(US).zip
{"AP8E",0x22},//0463 - Scooby-Doo(US).zip
{"AZEP",0x32},//0464 - Zone of the Enders - The Fist of Mars(EU).zip
{"ASDX",0x00},//0465 - Scooby-Doo and the Cyber Chase(EU).zip
{"ARDE",0x00},//0466 - Ripping Friends, The(UE).zip
{"AT3F",0x22},//0467 - Tony Hawk's Pro Skater 3(FR).zip
{"AE2E",0x11},//0468 - Megaman Battle Network 2(US).zip
{"AKCJ",0x00},//0469 - Konami Arcade Game Collection(JP).zip
{"AMRJ",0x22},//0470 - Motocross Maniacs Advance(JP).zip
{"AIAP",0x00},//0471 - Ice Age(EU).zip
{"ANQE",0x22},//0472 - Nicktoons Racing(US).zip
{"AVRP",0x22},//0473 - V-Rally 3(EU).zip
{"ABYX",0x00},//0474 - Britney's Dance Beat(EU).zip
{"ARPE",0x32},//0475 - Robopon 2 - Ring Version(US).zip
{"ACVE",0x32},//0476 - Robopon 2 - Cross Version(US).zip
{"AATJ",0x22},//0477 - Family Tennis Advance(JP).zip
{"AD7P",0x00},//0478 - Droopy's Tennis Open(EU).zip
{"AC7P",0x00},//0479 - CT Special Forces(EU).zip
{"APDE",0x22},//0480 - Pinball of the Dead, The(US).zip
{"AGWE",0x22},//0481 - GT Advance 2 - Rally Racing(US).zip
{"AS8P",0x00},//0482 - Star X(EU).zip
{"ALSE",0x22},//0483 - LEGO Soccer Mania(UE).zip
{"AMRP",0x22},//0484 - Maniac Racers Advance(EU).zip
{"AS2X",0x00},//0485 - Star Wars Episode II - Attack of the Clones(EU).zip
{"ADQP",0x11},//0486 - Dokapon(EU).zip
{"ATEP",0x22},//0487 - Pro Tennis WTA Tour(EU).zip
{"ALTP",0x00},//0488 - Lilo & Stitch(EU).zip
{"AGFJ",0x32},//0489 - Ougon no Taiyou - Ushinawareshi Toki(JP).zip
{"AAOE",0x22},//0490 - Aero the Acro-Bat - Rascal Rival Revenge(US).zip
{"AK9E",0x32},//0491 - Medabots AX - Rokusho Version(US).zip
{"AK8E",0x32},//0492 - Medabots AX - Metabee Version(US).zip
{"AY7J",0x11},//0493 - Yu-Gi-Oh! Duel Monsters 7 - Kettou Toshi Densetsu(JP).zip
{"AZOJ",0x22},//0494 - Pinball of the Dead, The(JP).zip
{"AB8J",0x11},//0495 - Bakuten Shoot Beyblade 2002 - Ikuze! Bakutou! Chou Jiryoku Battle!!(JP).zip
{"APLP",0x00},//0496 - Pinball Challenge Deluxe(EU).zip
{"ACYF",0x22},//0497 - Chessmaster(FR).zip
{"ASLE",0x00},//0498 - Stuart Little 2(UE).zip
{"ANFJ",0x11},//0499 - Monster Gate(JP).zip
{"AKQP",0x00},//0500 - Kong - The Animated Series(EU).zip
{"ATGE",0x00},//0501 - Top Gun - Firestorm Advance(UE).zip
{"A6DJ",0x22},//0502 - Disney Sports - Soccer(JP).zip
{"ACJJ",0x22},//0503 - Chou Makaimura R(JP).zip
{"AFWJ",0x32},//0504 - Final Fire Pro Wrestling - Yume no Dantai Unei!(JP).zip
{"A2BJ",0x22},//0505 - Bubble Bobble - Old & New(JP).zip
{"AKEJ",0x11},//0506 - Hikaru no Go 2(JP).zip
{"ATTP",0x00},//0507 - Tiny Toon Adventures - Buster's Bad Dream(EU).zip
{"AU3P",0x22},//0508 - Moorhen 3 - The Chicken Chase!(EU).zip
{"ARJJ",0x22},//0509 - Custom Robo GX(JP).zip
{"AB2P",0x11},//0510 - Breath of Fire II(EU).zip
{"AKAJ",0x11},//0511 - Shaman King Card Game - Chou Senjiryakketsu 2(JP).zip
{"AGVJ",0x22},//0512 - Ghost Trap(JP).zip
{"ANZP",0x00},//0513 - Antz - Extreme Racing(EU).zip
{"AJ9J",0x32},//0514 - Super Robot Taisen R(JP).zip
{"AETP",0x00},//0515 - E.T. - The Extra-Terrestrial(EU).zip
{"AKCP",0x00},//0516 - Konami Collector's Series - Arcade Classics(EU).zip
{"AX3E",0x22},//0517 - xXx(UE).zip
{"AWTF",0x00},//0518 - Famille Delajungle, La - A la Poursuite de Darwin(FR).zip
{"AHPX",0x22},//0519 - Hot Potato!(EU).zip
{"AXSP",0x22},//0520 - ESPN X-Games Skateboarding(EU).zip
{"AP6J",0x22},//0521 - Pinobee & Phoebee(JP).zip
{"AFAJ",0x11},//0522 - Fushigi no Kuni no Angelique(JP).zip
{"AHWJ",0x22},//0523 - Hot Wheels Advance(JP).zip
{"AKUJ",0x11},//0524 - Kurohige no Kurutto Jintori(JP).zip
{"AN6J",0x22},//0525 - Kaze no Klonoa G2 - Dream Champ Tournament(JP).zip
{"AIDJ",0x22},//0526 - Space Invaders EX(JP).zip
{"AK9P",0x32},//0527 - Medabots AX - Rokusho Version(EU).zip
{"A4MP",0x00},//0528 - Manic Miner(EU).zip
{"AKZJ",0x32},//0529 - Kamaitachi no Yoru Advance(JP).zip
{"ASXJ",0x32},//0530 - Sangokushi(CN).zip
{"AG9J",0x11},//0531 - Greatest Nine(JP).zip
{"AVMJ",0x22},//0532 - V-Master Cross(JP).zip
{"AKOE",0x22},//0533 - King of Fighters EX, The - NeoBlood(US).zip
{"ACXE",0x22},//0534 - Cubix - Robots for Everyone - Clash 'N Bash(US).zip
{"A3MJ",0x22},//0535 - Mickey to Minnie no Magical Quest(JP).zip
{"AFCJ",0x22},//0536 - Rockman & Forte(JP).zip
{"AAEE",0x00},//0537 - Hey Arnold! - The Movie(EU).zip
{"ANCP",0x22},//0538 - ZooCube(EU).zip
{"AYIE",0x00},//0539 - Urban Yeti!(UE).zip
{"ANJE",0x22},//0540 - Madden NFL 2003(US).zip
{"A3ME",0x22},//0541 - Magical Quest Starring Mickey & Minnie(US).zip
{"A2KE",0x22},//0542 - Spy Kids Challenger(US).zip
{"ANKE",0x00},//0543 - NFL Blitz 20-03(US).zip
{"AQCJ",0x11},//0544 - Combat Choro Q - Advance Daisakusen(JP).zip
{"ACUJ",0x22},//0545 - Crash Bandicoot Advance(JP).zip
{"AN7J",0x22},//0546 - Famista Advance(JP).zip
{"AD9E",0x22},//0547 - Duke Nukem Advance(US).zip
{"AH2E",0x22},//0548 - Mat Hoffman's Pro BMX 2(UE).zip
{"AVPP",0x00},//0549 - V.I.P.(EU).zip
{"AM8E",0x22},//0550 - Monster Force(US).zip
{"AHCJ",0x11},//0551 - Minimoni - Mika no Happy Morning Chatty(JP).zip
{"APWE",0x00},//0552 - Power Rangers - Wild Force(UE).zip
{"ATYJ",0x22},//0553 - Gambler Densetsu Tetsuya - Yomigaeru Densetsu(JP).zip
{"AQRE",0x00},//0554 - ATV Quad Power Racing(UE).zip
{"AD5J",0x11},//0555 - Mr. Driller A - Fushigi na Pacteria(JP).zip
{"ALMJ",0x11},//0556 - Pro Yakyuu Team wo Tsukurou! Advance(JP).zip
{"AT4P",0x00},//0557 - Turok Evolution(EU).zip
{"AMXJ",0x00},//0558 - Monsters, Inc.(JP).zip
{"AMUJ",0x11},//0559 - Mutsu - Water Looper Mutsu(JP).zip
{"AVRJ",0x22},//0560 - V-Rally 3(JP).zip
{"AGIJ",0x32},//0561 - Medarot G - Kuwagata Version(JP).zip
{"ARKF",0x00},//0562 - Rocket Power - Le Cauchemar d'Otto(FR).zip
{"AMXY",0x00},//0563 - Monsters, Inc.(EU).zip
{"AXBJ",0x22},//0564 - Black Matrix Zero(JP).zip
{"A3BE",0x00},//0565 - Sabrina - The Teenage Witch - Potion Commotion(US).zip
{"ACLJ",0x11},//0566 - Sakura Momoko no UkiUki Carnival(JP).zip
{"AM7J",0x11},//0567 - Hamepane - Tokyo Mew Mew(JP).zip
{"ARGF",0x00},//0568 - Razmoket, Les - Voler n'est pas Jouer(FR).zip
{"AILE",0x00},//0569 - Aggressive Inline(US).zip
{"APIP",0x00},//0570 - Pinky and the Brain - The Masterplan(EU).zip
{"AGQP",0x22},//0571 - Go! Go! Beckham! - Adventure on Soccer Island(EU).zip
{"ANOJ",0x11},//0572 - Nobunaga Ibun(JP).zip
{"AAIJ",0x32},//0573 - Gachasute! Dino Device - Red(JP).zip
{"ADNJ",0x22},//0574 - Jurassic Park III - Ushinawareta Idenshi(JP).zip
{"ATHJ",0x22},//0575 - SK8 - Tony Hawk's Pro Skater 2(JP).zip
{"ACHP",0x11},//0576 - Castlevania - Harmony of Dissonance(EU).zip
{"AC2J",0x11},//0577 - J.League Pro Soccer Club wo Tsukurou! Advance(JP).zip
{"ANQP",0x22},//0578 - Nicktoons Racing(EU).zip
{"ASTJ",0x11},//0579 - Densetsu no Stafy(JP).zip
{"A3DJ",0x11},//0580 - Disney Sports - American Football(JP).zip
{"A4DJ",0x22},//0581 - Disney Sports - Skateboarding(JP).zip
{"AILP",0x00},//0582 - Aggressive Inline(EU).zip
{"AT9E",0x22},//0583 - BMX Trick Racer(US).zip
{"ARQE",0x00},//0584 - Matchbox Cross Town Heroes(US).zip
{"AIPE",0x22},//0585 - Silent Scope(US).zip
{"AT5X",0x22},//0586 - Tiger Woods PGA Tour Golf(EU).zip
{"AS6P",0x22},//0587 - Speedball 2 - Brutal Deluxe(EU).zip
{"AZCE",0x11},//0588 - Megaman Zero(UE).zip
{"ACZP",0x22},//0589 - Comix Zone(EU).zip
{"AN2J",0x32},//0590 - Natural 2 - Duo(JP).zip
{"A3BP",0x00},//0591 - Sabrina - The Teenage Witch - Potion Commotion(EU).zip
{"AQ3E",0x00},//0592 - SpongeBob SquarePants - Revenge of the Flying Dutchman(UE).zip
{"A3AE",0x22},//0593 - Super Mario Advance 3 - Yoshi's Island(US).zip
{"AA6P",0x22},//0594 - Sum of All Fears, The(EU).zip
{"A3AJ",0x22},//0595 - Super Mario Advance 3(JP).zip
{"AAKE",0x22},//0596 - Airforce Delta Storm(US).zip
{"ABDE",0x22},//0597 - Boulder Dash EX(US).zip
{"AYFE",0x22},//0598 - Backyard Football(US).zip
{"AVBE",0x00},//0599 - Barbie Groovy Games(US).zip
{"AGWP",0x22},//0600 - GT Advance 2 - Rally Racing(EU).zip
{"ADYJ",0x22},//0601 - Hanafuda Trump Mahjong - Depachika Wayouchuu(JP).zip
{"AEME",0x00},//0602 - Egg Mania(US).zip
{"AG5E",0x22},//0603 - Super Ghouls'n Ghosts(UE).zip
{"AS3E",0x22},//0604 - Kelly Slater's Pro Surfer(UE).zip
{"ASGE",0x00},//0605 - Smuggler's Run(US).zip
{"AD9P",0x22},//0606 - Duke Nukem Advance(EU).zip
{"ACMP",0x22},//0607 - Colin McRae Rally 2.0(EU).zip
{"AZUJ",0x22},//0608 - Street Fighter Zero 3 Upper(JP).zip
{"AECJ",0x11},//0609 - Samurai Evolution - Oukoku Geist(JP).zip
{"A3PE",0x00},//0610 - Sega Smash Pack(US).zip
{"ADUP",0x22},//0611 - Driver 2 Advance(EU).zip
{"ACHE",0x11},//0612 - Castlevania - Harmony of Dissonance(US).zip
{"ALOE",0x22},//0613 - Lord of the Rings, The - The Fellowship of the Ring(US).zip
{"AT8P",0x22},//0614 - Tennis Masters Series 2003(EU).zip
{"A2SE",0x22},//0615 - Spyro 2 - Season of Flame(US).zip
{"AJXE",0x00},//0616 - Adventures of Jimmy Neutron vs. Jimmy Negatron, The(UE).zip
{"A3MP",0x22},//0617 - Magical Quest Starring Mickey & Minnie(EU).zip
{"AF7J",0x22},//0618 - Tokimeki Yume Series 1 - Ohanaya-san ni Narou!(JP).zip
{"PSAE",0x31},//0619 - e-Reader(US).zip
{"ARIJ",0x11},//0620 - Groove Adventure Rave - Hikari to Yami no Daikessen 2(JP).zip
{"AHJP",0x22},//0621 - Hugo - The Evil Mirror Advance(EU).zip
{"AWYP",0x00},//0622 - Worms World Party(EU).zip
{"AOCJ",0x22},//0623 - Chobits - Atashi Dake no Hito(JP).zip
{"A3AP",0x22},//0624 - Super Mario Advance 3 - Yoshi's Island(EU).zip
{"ATDJ",0x11},//0625 - Daisuki Teddy(JP).zip
{"AP8F",0x22},//0626 - Scooby-Doo(FR).zip
{"AFQP",0x00},//0627 - Frogger Advance - The Great Quest(EU).zip
{"ABIJ",0x32},//0628 - Gachasute! Dino Device - Blue(JP).zip
{"ARNJ",0x22},//0629 - Harukanaru Toki no Naka de(JP).zip
{"ABWE",0x00},//0630 - Butt-Ugly Martians - B.K.M. Battles(US).zip
{"AH7J",0x22},//0631 - Nakayoshi Pet Advance Series 1 - Kawaii Hamster(JP).zip
{"AI7J",0x22},//0632 - Nakayoshi Pet Advance Series 2 - Kawaii Koinu(JP).zip
{"AN3J",0x22},//0633 - Nakayoshi Pet Advance Series 3 - Kawaii Koneko(JP).zip
{"AGHJ",0x32},//0634 - Medarot G - Kabuto Version(JP).zip
{"A3HJ",0x11},//0635 - Hime Kishi Monogatari - Princess Blue(JP).zip
{"AGXP",0x32},//0636 - Guilty Gear X - Advance Edition(EU).zip
{"AK6E",0x00},//0637 - Soccer Kid(UE).zip
{"AEEE",0x00},//0638 - Ballistic - Ecks vs. Sever(US).zip
{"ALGP",0x22},//0639 - Dragon Ball Z - The Legacy of Goku(EU).zip
{"AESP",0x00},//0640 - Ecks vs. Sever(EU).zip
{"AFYE",0x32},//0641 - Fire Pro Wrestling 2(US).zip
{"AIVP",0x00},//0642 - Invader(EU).zip
{"A9MP",0x11},//0643 - Motoracer Advance(EU).zip
{"AY3E",0x22},//0644 - Army Men - Turf Wars(US).zip
{"AAPJ",0x22},//0645 - Metalgun Slinger(JP).zip
{"AAKJ",0x22},//0646 - Airforce Delta II(JP).zip
{"AK3E",0x22},//0647 - Turbo Turtle Adventure(US).zip
{"AKOP",0x22},//0648 - King of Fighters EX, The - NeoBlood(EU).zip
{"AHQJ",0x11},//0649 - Harobots - Robo Hero Battling!!(JP).zip
{"AM8P",0x22},//0650 - Monster Force(EU).zip
{"AVBP",0x00},//0651 - Barbie Groovy Games(EU).zip
{"A7SP",0x00},//0652 - Smurfs, The - The Revenge of the Smurfs(EU).zip
{"ADHE",0x22},//0653 - Defender of the Crown(US).zip
{"AB9E",0x22},//0654 - Dual Blades(US).zip
{"AIAJ",0x00},//0655 - Ice Age(JP).zip
{"A2JJ",0x22},//0656 - J.League Winning Eleven Advance 2002(JP).zip
{"APZP",0x22},//0657 - Pinball Advance(EU).zip
{"AVTE",0x22},//0658 - Virtua Tennis(US).zip
{"AQWP",0x11},//0659 - Game & Watch Gallery Advance(EU).zip
{"AOPJ",0x22},//0660 - Oshare Princess(JP).zip
{"AH4E",0x22},//0661 - Shrek - Hassle at the Castle(US).zip
{"AGZJ",0x22},//0662 - Galaxy Angel - Moridakusan Tenshi no Full-Course - Okawari Jiyuu(JP).zip
{"AHXJ",0x22},//0663 - High Heat Major League Baseball 2003(JP).zip
{"ANCJ",0x22},//0664 - ZooCube(JP).zip
{"AHAJ",0x22},//0665 - Hamster Paradise Advanchu(JP).zip
{"ADBJ",0x22},//0666 - Denki Blocks!(JP).zip
{"APUJ",0x22},//0667 - PukuPuku Tennen Kairanban(JP).zip
{"ANYJ",0x11},//0668 - Gachinko Pro Yakyuu(JP).zip
{"AB6P",0x22},//0669 - Black Belt Challenge(EU).zip
{"AY5E",0x11},//0670 - Yu-Gi-Oh! The Eternal Duelist Soul(US).zip
{"ARBE",0x22},//0671 - Robotech - The Macross Saga(UE).zip
{"AX3F",0x22},//0672 - xXx(FR).zip
{"A55P",0x00},//0673 - Who Wants to Be a Millionaire(EU).zip
{"A3GJ",0x11},//0674 - Gyakuten Saiban 2(JP).zip
{"AICJ",0x32},//0675 - Oshaberi Inko Club(JP).zip
{"ACCP",0x22},//0676 - Crazy Chase(EU).zip
{"AC6F",0x00},//0677 - Spirit - L'Etalon des Plaines - A la Recherche de la Terre Natale(FR).zip
{"AHVJ",0x22},//0678 - Nakayoshi Youchien - Sukoyaka Enji Ikusei Game(JP).zip
{"ARSP",0x22},//0679 - Robot Wars - Extreme Destruction(EU).zip
{"ALXP",0x22},//0680 - Ace Lightning(EU).zip
{"AXYE",0x22},//0681 - SSX Tricky(UE).zip
{"A7KJ",0x11},//0682 - Hoshi no Kirby - Yume no Izumi Deluxe(JP).zip
{"AJGE",0x22},//0683 - Tarzan - Return to the Jungle(UE).zip
{"AW8E",0x22},//0684 - WWE - Road to WrestleMania X8(UE).zip
{"A5AE",0x22},//0685 - Bionicle - Matoran Adventures(UE).zip
{"A9DE",0x22},//0686 - Doom II(US).zip
{"A2SP",0x22},//0687 - Spyro 2 - Season of Flame(EU).zip
{"AJZJ",0x11},//0688 - Bomberman Jetters - Densetsu no Bomberman(JP).zip
{"AN9J",0x22},//0689 - Tales of the World - Narikiri Dungeon 2(JP).zip
{"AMTE",0x11},//0690 - Metroid Fusion(US).zip
{"AXXP",0x22},//0691 - Santa Claus Jr. Advance(EU).zip
{"AAHE",0x00},//0692 - Secret Agent Barbie - Royal Jewels Mission(US).zip
{"ASLF",0x00},//0693 - Stuart Little 2(FR).zip
{"AM2P",0x11},//0694 - Megaman Battle Network 2(EU).zip
{"AN4E",0x22},//0695 - NHL Hitz 20-03(US).zip
{"AGEP",0x00},//0696 - Gekido Advance - Kintaro's Revenge(EU).zip
{"ACBE",0x22},//0697 - Car Battler Joe(US).zip
{"AAKP",0x22},//0698 - Deadly Skies(EU).zip
{"AH5J",0x11},//0699 - Beast Shooter - Mezase Beast King!(JP).zip
{"A7IJ",0x11},//0700 - Silk to Cotton(JP).zip
{"A8YJ",0x32},//0701 - Best Play Pro Yakyuu(JP).zip
{"AODJ",0x22},//0702 - Minami no Umi no Odyssey(JP).zip
{"A2QJ",0x11},//0703 - Monster Farm Advance 2(JP).zip
{"A3OJ",0x11},//0704 - Di Gi Charat - DigiCommunication(JP).zip
{"AGUE",0x00},//0705 - Masters of the Universe - He-Man Power of Grayskull(US).zip
{"AD4E",0x11},//0706 - Dungeons & Dragons - Eye of the Beholder(US).zip
{"AT6E",0x22},//0707 - Tony Hawk's Pro Skater 4(UE).zip
{"AR9E",0x00},//0708 - Reign of Fire(US).zip
{"APKE",0x00},//0709 - Pocky & Rocky with Becky(US).zip
{"AP5E",0x22},//0710 - Powerpuff Girls, The - Him and Seek(US).zip
{"ADXE",0x00},//0711 - Dexter's Laboratory - Chess Challenge(US).zip
{"AZQE",0x00},//0712 - Treasure Planet(US).zip
{"ACCE",0x22},//0713 - Crazy Chase(US).zip
{"AG4P",0x00},//0714 - Godzilla - Domination!(EU).zip
{"ALOP",0x22},//0715 - Lord of the Rings, The - The Fellowship of the Ring(EU).zip
{"AD6E",0x22},//0716 - Davis Cup(US).zip
{"ALPE",0x22},//0717 - Lord of the Rings, The - The Two Towers(UE).zip
{"AJLE",0x22},//0718 - Justice League - Injustice for All(US).zip
{"A55D",0x00},//0719 - Wer wird Millionar(DE).zip
{"AFVE",0x00},//0720 - Fairly Odd Parents!, The - Enter the Cleft(US).zip
{"A7HE",0x22},//0721 - Harry Potter and the Chamber of Secrets(UE).zip
{"AAWE",0x00},//0722 - Contra Advance - The Alien Wars EX(US).zip
{"AFBE",0x22},//0723 - Frogger's Adventures 2 - The Lost Wand(US).zip
{"A63J",0x22},//0724 - Kawaii Pet Shop Monogatari 3(JP).zip
{"ASGP",0x00},//0725 - Smuggler's Run(EU).zip
{"AP5P",0x22},//0726 - Powerpuff Girls, The - Him and Seek(EU).zip
{"ACEP",0x00},//0727 - Agassi Tennis Generation(EU).zip
{"AZDP",0x00},//0728 - Zidane Football Generation(EU).zip
{"A2FE",0x22},//0729 - Defender(US).zip
{"A2WE",0x00},//0730 - Star Wars - The New Droid Army(US).zip
{"A6DE",0x22},//0731 - Disney Sports - Soccer(US).zip
{"AXHJ",0x11},//0732 - Dan Doh!! Xi(JP).zip
{"AL9P",0x00},//0733 - Tomb Raider - The Prophecy(EU).zip
{"AUMP",0x00},//0734 - Mummy, The(EU).zip
{"AAWJ",0x00},//0735 - Contra Hard Spirits(JP).zip
{"AFJE",0x22},//0736 - FIFA(UE).zip
{"AC5J",0x11},//0737 - Shin Megami Tensei Devil Children - Yami no Sho(JP).zip
{"AO7J",0x11},//0738 - From TV Animation One Piece - Nanatsu Shima no Daihihou(JP).zip
{"AP7P",0x00},//0739 - Pink Panther - Pinkadelic Pursuit(EU).zip
{"A3RE",0x00},//0740 - Revenge of Shinobi, The(US).zip
{"AR8E",0x22},//0741 - Rocky(US).zip
{"A6CJ",0x11},//0742 - Croket! - Yume no Banker Survival!(JP).zip
{"AH8E",0x00},//0743 - Hot Wheels - Velocity X(US).zip
{"AAYE",0x00},//0744 - Mary-Kate and Ashley Sweet 16 - Licensed to Drive(US).zip
{"AR5E",0x00},//0745 - Rugrats - I Gotta Go Party(UE).zip
{"AJHJ",0x11},//0746 - Hamster Club 3(JP).zip
{"AOGJ",0x32},//0747 - Super Robot Taisen - Original Generation(JP).zip
{"A2QE",0x11},//0748 - Monster Rancher Advance 2(US).zip
{"AXTE",0x22},//0749 - LEGO Island Xtreme Stunts(UE).zip
{"AG8E",0x22},//0750 - Galidor - Defenders of the Outer Dimension(US).zip
{"AR4E",0x00},//0751 - Rocket Power - Beach Bandits(UE).zip
{"AWLE",0x00},//0752 - Wild Thornberrys Movie, The(UE).zip
{"AXDE",0x22},//0753 - Mortal Kombat - Deadly Alliance(US).zip
{"AH4P",0x22},//0754 - Shrek - Hassle at the Castle(EU).zip
{"AMTP",0x11},//0755 - Metroid Fusion(EU).zip
{"AL4J",0x11},//0756 - Shin Megami Tensei Devil Children - Hikari no Sho(JP).zip
{"AXPJ",0x31},//0757 - Pocket Monsters - Sapphire(JP).zip
{"AD4P",0x11},//0758 - Dungeons & Dragons - Eye of the Beholder(EU).zip
{"ANPF",0x11},//0759 - Aigle de Guerre, L'(FR).zip
{"AXVJ",0x31},//0760 - Pocket Monsters - Ruby(JP).zip
{"AAXJ",0x22},//0761 - Fantastic Maerchen - Cake-yasan Monogatari(JP).zip
{"A57J",0x11},//0762 - Scan Hunter - Sennen Kaigyo wo Oe!(JP).zip
{"ALUE",0x22},//0763 - Super Monkey Ball Jr.(US).zip
{"AEMJ",0x00},//0764 - Egg Mania(JP).zip
{"AM4J",0x22},//0765 - Moto GP(JP).zip
{"A8CJ",0x00},//0766 - Card Party(JP).zip
{"AIPJ",0x22},//0767 - Silent Scope(JP).zip
{"AJ4P",0x00},//0768 - Earthworm Jim 2(EU).zip
{"AJGF",0x22},//0769 - Tarzan - L'Appel de la Jungle(FR).zip
{"AYCE",0x22},//0770 - Phantasy Star Collection(US).zip
{"AYGE",0x22},//0771 - Gauntlet - Dark Legacy(US).zip
{"AEYE",0x00},//0772 - Kim Possible - Revenge of Monkey Fist(US).zip
{"ARME",0x22},//0773 - Minority Report - Everybody Runs(UE).zip
{"AARE",0x22},//0774 - Altered Beast - Guardian of the Realms(US).zip
{"A7KE",0x11},//0775 - Kirby - Nightmare in Dream Land(US).zip
{"AZLE",0x22},//0776 - Legend of Zelda, The - A Link to the Past & Four Swords(US).zip
{"AZQP",0x00},//0777 - Treasure Planet(EU).zip
{"AZUP",0x22},//0778 - Street Fighter Alpha 3 Upper(EU).zip
{"AT7F",0x00},//0779 - Titeuf - Ze Gagmachine(FR).zip
{"A2LE",0x22},//0780 - Legends of Wrestling II(UE).zip
{"AQ2J",0x11},//0781 - Choro Q Advance 2(JP).zip
{"AJKJ",0x22},//0782 - Jikkyou World Soccer Pocket 2(JP).zip
{"AUGE",0x00},//0783 - Medal of Honor - Underground(US).zip
{"A2WP",0x00},//0784 - Star Wars - The New Droid Army(EU).zip
{"AZPE",0x22},//0785 - Zapper(US).zip
{"AEDP",0x22},//0786 - Carrera Power Slide(EU).zip
{"A6BJ",0x11},//0787 - Battle Network Rockman EXE 3(JP).zip
{"AWBP",0x00},//0788 - Worms Blast(EU).zip
{"AR5F",0x00},//0789 - Razmoket, Les - A moi la Fiesta(FR).zip
{"A55F",0x00},//0790 - Qui Veut Gagner des millions(FR).zip
{"AH6E",0x22},//0791 - Hardcore Pinball(UE).zip
{"AEEP",0x00},//0792 - Ecks vs. Sever II - Ballistic(EU).zip
{"AT4E",0x00},//0793 - Turok Evolution(US).zip
{"AB3E",0x22},//0794 - Dave Mirra Freestyle BMX 3(UE).zip
{"A8PJ",0x32},//0795 - Derby Stallion Advance(JP).zip
{"AK2J",0x22},//0796 - Kinnikuman IIsei - Seigi Choujin heno Michi(JP).zip
{"A7HJ",0x22},//0797 - Harry Potter to Himitsu no Heya(JP).zip
{"AL3J",0x11},//0798 - Shaman King Card Game - Chou Senjiryakketsu 3(JP).zip
{"AWWP",0x22},//0799 - Woody Woodpecker in Crazy Castle 5(EU).zip
{"ATVP",0x00},//0800 - Tir et But - Edition Champions du Monde(FR).zip
{"AA4J",0x11},//0801 - Monster Maker 4 - Flash Card(JP).zip
{"AA5J",0x11},//0802 - Monster Maker 4 - Killer Dice(JP).zip
{"A2DJ",0x22},//0803 - Darius R(JP).zip
{"A2IJ",0x22},//0804 - Magi Nation(JP).zip
{"AK7J",0x22},//0805 - Klonoa Heroes - Densetsu no Star Medal(JP).zip
{"ABEE",0x22},//0806 - BattleBots - Beyond the BattleBox(US).zip
{"A2HJ",0x22},//0807 - Hajime no Ippo - The Fighting!(JP).zip
{"ALNE",0x22},//0808 - Lunar Legend(US).zip
{"A2TE",0x00},//0809 - Pinball Tycoon(US).zip
{"A4BE",0x00},//0810 - Monster! Bass Fishing(US).zip
{"A4CE",0x22},//0811 - I Spy Challenger!(US).zip
{"A2AE",0x22},//0812 - Disney Sports - Basketball(US).zip
{"A4DE",0x22},//0813 - Disney Sports - Skateboarding(US).zip
{"A3DE",0x11},//0814 - Disney Sports - Football(US).zip
{"A2NJ",0x33},//0815 - Sonic Advance 2(JP).zip
{"AYLJ",0x11},//0816 - Sega Rally Championship(JP).zip
{"AERE",0x00},//0817 - Dora the Explorer - The Search for the Pirate Pig's Treasure(US).zip
{"AYKE",0x33},//0818 - Karnaaj Rally(UE).zip
{"AIOE",0x22},//0819 - Invincible Iron Man, The(UE).zip
{"AAOJ",0x22},//0820 - Acrobat Kid(JP).zip
{"AUCJ",0x22},//0821 - Uchuu Daisakusen Choco Vader - Uchuu Kara no Shinryakusha(JP).zip
{"A59J",0x22},//0822 - Toukon Heat(JP).zip
{"ANWJ",0x22},//0823 - Elevator Action - Old & New(JP).zip
{"A5BJ",0x11},//0824 - Chocobo Land - A Game of Dice(JP).zip
{"A8HE",0x00},//0825 - Cabela's Big Game Hunter(US).zip
{"AVZE",0x22},//0826 - Super Bubble Pop(US).zip
{"AJJE",0x22},//0827 - Jazz Jackrabbit(US).zip
{"ALJE",0x22},//0828 - Sea Trader - Rise of Taipan(US).zip
{"A3RP",0x00},//0829 - Revenge of Shinobi, The(EU).zip
{"A87J",0x22},//0830 - Ohanaya-san Monogatari GBA - Iyashikei Ohanaya-san Ikusei Game(JP).zip
{"AB9J",0x22},//0831 - Dual Blades(JP).zip
{"AEXJ",0x22},//0832 - King of Fighters EX2, The - Howling Blood(JP).zip
{"A55I",0x00},//0833 - Chi Vuol Essere Milionario(IT).zip
{"AOSJ",0x22},//0834 - Samurai Deeper Kyo(JP).zip
{"A8VJ",0x11},//0835 - Boboboubo Boubobo - Ougi 87.5 Bakuretsu Hanage Shinken(JP).zip
{"AVAJ",0x11},//0836 - Tennis no Ouji-sama - Aim at the Victory!(JP).zip
{"AJGD",0x22},//0837 - Tarzan - Ruckkehr in den Dschungel(DE).zip
{"A3EJ",0x11},//0838 - Bakuten Shoot Beyblade 2002 - Gekisen! Team Battle!! Kouryuu no Shou - Daichi Hen(JP).zip
{"A3WJ",0x11},//0839 - Bakuten Shoot Beyblade 2002 - Gekisen! Team Battle!! Seiryuu no Shou - Takao Hen(JP).zip
{"A56J",0x22},//0840 - Dokidoki Cooking Series 1 - Komugi-chan no Happy Cake(JP).zip
{"AG4E",0x00},//0841 - Godzilla - Domination!(US).zip
{"AHZJ",0x22},//0842 - Higanbana(JP).zip
{"AIRP",0x00},//0843 - Inspector Gadget Racing(EU).zip
{"AF8E",0x22},//0844 - F1 2002(UE).zip
{"A8BP",0x22},//0845 - Medabots - Metabee Version(EU).zip
{"A2OJ",0x22},//0846 - K-1 Pocket Grand Prix 2(JP).zip
{"A9QJ",0x11},//0847 - Kururin Paradise(JP).zip
{"AC8E",0x22},//0848 - Crash Bandicoot 2 - N-Tranced(US).zip
{"AMXD",0x00},//0849 - Monster AG, Die(DE).zip
{"A8MJ",0x11},//0850 - Kotoba no Puzzle - Mojipittan Advance(JP).zip
{"AJFP",0x00},//0851 - Jungle Book 2, The(EU).zip
{"ALQJ",0x11},//0852 - Little Buster Q(JP).zip
{"AXZP",0x00},//0853 - Micro Machines(EU).zip
{"A8SE",0x22},//0854 - Digimon - Battle Spirit(US).zip
{"AXQF",0x00},//0855 - Taxi 3(FR).zip
{"AY2P",0x22},//0856 - International Superstar Soccer Advance(EU).zip
{"A5PJ",0x11},//0857 - Power Pro Kun Pocket 5(JP).zip
{"ANSJ",0x22},//0858 - Marie, Elie & Anis no Atelier - Soyokaze Kara no Dengon(JP).zip
{"ABDJ",0x22},//0859 - Boulder Dash EX(JP).zip
{"AIYJ",0x11},//0860 - Inuyasha - Naraku no Wana! Mayoi no Mori no Shoutaijou(JP).zip
{"AEKJ",0x22},//0861 - Elemix!(JP).zip
{"AVLE",0x00},//0862 - Daredevil(UE).zip
{"A2GE",0x22},//0863 - GT Advance 3 - Pro Concept Racing(US).zip
{"AOHJ",0x22},//0864 - Minimoni - Onegai Ohoshi-sama!(JP).zip
{"A3XJ",0x11},//0865 - Battle Network Rockman EXE 3 Black(JP).zip
{"AFXJ",0x33},//0866 - Final Fantasy Tactics Advance(JP).zip
{"AYDE",0x32},//0867 - Yu-Gi-Oh! Dungeon Dice Monsters(US).zip
{"AHUP",0x33},//0868 - Shining Soul(EU).zip
{"A3KP",0x00},//0869 - International Karate Plus(EU).zip
{"A9AP",0x00},//0870 - Demon Driver - Time to Burn Rubber!(EU).zip
{"AZNP",0x00},//0871 - Archer Maclean's - Super Dropzone(EU).zip
{"A9GP",0x00},//0872 - Stadium Games(EU).zip
{"AFHP",0x00},//0873 - Strike Force Hydra(EU).zip
{"AWCP",0x00},//0874 - World Tennis Stars(EU).zip
{"AYZP",0x22},//0875 - Rayman 3(EU).zip
{"ALPJ",0x22},//0876 - Lord of the Rings, The - Futatsu no Tou(JP).zip
{"AMTJ",0x11},//0877 - Metroid Fusion(JP).zip
{"ACOJ",0x22},//0878 - Manga-ka Debut Monogatari(JP).zip
{"AF4J",0x22},//0879 - Fushigi no Kuni no Alice(JP).zip
{"A4SJ",0x22},//0880 - Spyro Advance(JP).zip
{"AONP",0x22},//0881 - Bubble Bobble - Old & New(EU).zip
{"AG6J",0x11},//0882 - Mugenborg(JP).zip
{"AZLP",0x22},//0883 - Legend of Zelda, The - A Link to the Past & Four Swords(EU).zip
{"A8RJ",0x11},//0884 - Tennis no Ouji-sama 2003 - Passion Red(JP).zip
{"A9LJ",0x11},//0885 - Tennis no Ouji-sama 2003 - Cool Blue(JP).zip
{"ATUJ",0x22},//0886 - Total Soccer Advance(JP).zip
{"A39J",0x22},//0887 - Lode Runner(JP).zip
{"AUQP",0x00},//0888 - Butt-Ugly Martians - B.K.M. Battles(EU).zip
{"AA7E",0x11},//0889 - All-Star Baseball 2004(US).zip
{"AYCP",0x22},//0890 - Phantasy Star Collection(EU).zip
{"AWQE",0x22},//0891 - Wings(US).zip
{"AWKJ",0x22},//0892 - Wagamama Fairy Mirumo de Pon! - Ougon Maracas no Densetsu(JP).zip
{"AZPP",0x22},//0893 - Zapper(EU).zip
{"AVTP",0x22},//0894 - Virtua Tennis(EU).zip
{"A9PJ",0x11},//0895 - Tales of the World - Summoner's Lineage(JP).zip
{"ATQP",0x22},//0896 - TOCA World Touring Cars(EU).zip
{"A52J",0x11},//0897 - Koukou Juken Advance Series Eitango Hen - 2000 Words Shuuroku(JP).zip
{"AXPE",0x31},//0898 - Pokemon - Sapphire Version(US).zip
{"AZUE",0x22},//0899 - Street Fighter Alpha 3 Upper(US).zip
{"AZLJ",0x22},//0900 - Zelda no Densetsu - Kamigami no Triforce & 4tsu no Tsurugi(JP).zip
{"AIFE",0x00},//0901 - Tom and Jerry in Infurnal Escape(US).zip
{"A6ME",0x22},//0902 - Megaman & Bass(US).zip
{"AYLE",0x11},//0903 - Sega Rally Championship(US).zip
{"A2FP",0x22},//0904 - Defender(EU).zip
{"AC8P",0x22},//0905 - Crash Bandicoot 2 - N-Tranced(EU).zip
{"A2NE",0x33},//0906 - Sonic Advance 2(US).zip
{"AXVE",0x31},//0907 - Pokemon - Ruby Version(US).zip
{"AY8J",0x11},//0908 - Yu-Gi-Oh! Duel Monsters 8 - Hametsu no Daijashin(JP).zip
{"AZWJ",0x11},//0909 - Made in Wario(JP).zip
{"A7OE",0x22},//0910 - 007 - NightFire(UE).zip
{"A2RE",0x22},//0911 - Bratz(US).zip
{"AZME",0x00},//0912 - Muppets, The - On With the Show!(UE).zip
{"A9NE",0x00},//0913 - Piglet's Big Game(US).zip
{"ALVP",0x22},//0914 - Lost Vikings, The(EU).zip
{"A6TP",0x22},//0915 - Dr. Muto(EU).zip
{"A2NP",0x33},//0916 - Sonic Advance 2(EU).zip
{"AJTE",0x22},//0917 - Samurai Jack - The Amulet of Time(UE).zip
{"A5ME",0x00},//0918 - MLB SlugFest 20-04(US).zip
{"AUSJ",0x33},//0919 - From TV Animation One Piece - Mezase! King of Berry(JP).zip
{"AVLD",0x00},//0920 - Daredevil(DE).zip
{"ALEP",0x22},//0921 - Bruce Lee - Return of the Legend(EU).zip
{"AAQP",0x00},//0922 - Animal Snap - Rescue Them 2 by 2(EU).zip
{"ABUE",0x22},//0923 - Ultimate Brain Games(UE).zip
{"A4PJ",0x22},//0924 - Sister Princess - RePure(JP).zip
{"AWNP",0x00},//0925 - Castleween(EU).zip
{"ASUE",0x00},//0926 - Superman - Countdown to Apokolips(US).zip
{"AZ8P",0x22},//0927 - Super Puzzle Fighter II Turbo(EU).zip
{"A64J",0x22},//0928 - Shimura Ken no Baka Tonosama - Bakushou Tenka Touitsu Game(JP).zip
{"AI9J",0x11},//0929 - Inukko Club(JP).zip
{"A9HJ",0x22},//0930 - Dragon Quest Monsters - Caravan Heart(JP).zip
{"AJXD",0x00},//0931 - Jimmy Neutron vs. Jimmy Negatron(DE).zip
{"AGGP",0x00},//0932 - Gremlins - Stripe vs. Gizmo(EU).zip
{"AAUJ",0x11},//0933 - Shin Megami Tensei(JP).zip
{"A55S",0x00},//0934 - Quiere ser Millonario(ES).zip
{"ADUE",0x22},//0935 - Driver 2 Advance(US).zip
{"AE3E",0x22},//0936 - Ed, Edd n Eddy - Jawbreakers!(US).zip
{"ALUP",0x22},//0937 - Super Monkey Ball Jr.(EU).zip
{"AUZE",0x00},//0938 - Santa Claus Saves the Earth(EU).zip
{"A9BP",0x22},//0939 - Medabots - Rokusho Version(EU).zip
{"AGFE",0x32},//0940 - Golden Sun - The Lost Age(UE).zip
{"ANXP",0x22},//0941 - Ninja Cop(EU).zip
{"AQPE",0x00},//0942 - Disney Princess(UE).zip
{"AMHP",0x22},//0943 - Bomberman Max 2 - Blue Advance(EU).zip
{"AMYP",0x22},//0944 - Bomberman Max 2 - Red Advance(EU).zip
{"A73J",0x11},//0945 - Whistle! - Dai 37 Kai Tokyo-to Chuugakkou Sougou Taiiku Soccer Taikai(JP).zip
{"AAZJ",0x22},//0946 - Ao-Zoura to Nakamatachi - Yume no Bouken(JP).zip
{"AH3E",0x11},//0947 - Hamtaro - Ham-Ham Heartbreak(US).zip
{"A3CE",0x22},//0948 - Crazy Taxi - Catch a Ride(US).zip
{"AVLX",0x00},//0949 - Daredevil(EU).zip
{"ARFP",0x00},//0950 - Freestyle Scooter(EU).zip
{"AM5P",0x00},//0951 - Mortal Kombat Advance(EU).zip
{"A9SJ",0x22},//0952 - Dancing Sword - Senkou(JP).zip
{"ALVE",0x22},//0953 - Lost Vikings, The(US).zip
{"AYWP",0x11},//0954 - Yu-Gi-Oh! Worldwide Edition - Stairway to the Destined Duel(EU).zip
{"AWVE",0x22},//0955 - X2 - Wolverine's Revenge(UE).zip
{"A4NJ",0x11},//0956 - Bokujou Monogatari - Mineral Town no Nakamatachi(JP).zip
{"AMGJ",0x22},//0957 - Exciting Bass(JP).zip
{"APEE",0x00},//0958 - Pink Panther - Pinkadelic Pursuit(US).zip
{"A7GJ",0x11},//0959 - Sengoku Kakumei Gaiden(JP).zip
{"A5KJ",0x22},//0960 - Medarot Ni Core - Kabuto Version(JP).zip
{"ACME",0x22},//0961 - Colin McRae Rally 2.0(US).zip
{"ATBJ",0x22},//0962 - Slot! Pro 2 Advance - GoGo Juggler & New Tairyou(JP).zip
{"A4LJ",0x22},//0963 - Sylvanian Families 4 - Meguru Kisetsu no Tapestry(JP).zip
{"ANTJ",0x11},//0964 - Nippon Pro Mahjong Renmei Kounin Tetsuman Advance - Menkyo Kaiden Series(JP).zip
{"AW3J",0x22},//0965 - Watashi no Makesalon(JP).zip
{"A2AJ",0x22},//0966 - Disney Sports - Basketball(JP).zip
{"A53J",0x11},//0967 - Koukou Juken Advance Series Eijukugo Hen - 650 Phrases Shuuroku(JP).zip
{"AZ2J",0x11},//0968 - Zoids Saga II(JP).zip
{"A2VJ",0x22},//0969 - Kisekko Gurumi - Chesty to Nuigurumi-tachi no Mahou no Bouken(JP).zip
{"A5QJ",0x22},//0970 - Medarot Ni Core - Kuwagata Version(JP).zip
{"ARAJ",0x11},//0971 - Shin Nippon Pro Wrestling - Toukon Retsuden Advance(JP).zip
{"AZ9J",0x22},//0972 - Simple 2960 Tomodachi Series Vol. 2 - The Block Kuzushi(JP).zip
{"AZBJ",0x22},//0973 - Bass Tsuri Shiyouze! - Tournament ha Senryaku da!(JP).zip
{"AUTJ",0x00},//0974 - Tomb Raider - The Prophecy(JP).zip
{"AJEJ",0x11},//0975 - Fancy Pocket(JP).zip
{"AO2J",0x22},//0976 - Oshare Princess 2(JP).zip
{"ASUP",0x00},//0977 - Superman - Countdown to Apokolips(EU).zip
{"AYLP",0x11},//0978 - Sega Rally Championship(EU).zip
{"AE7J",0x11},//0979 - Fire Emblem - Rekka no Ken(JP).zip
{"AB4J",0x22},//0980 - Summon Night - Craft Sword Monogatari(JP).zip
{"A8TJ",0x33},//0981 - RPG Tsukuru Advance(JP).zip
{"AA6E",0x22},//0982 - Sum of All Fears, The(US).zip
{"A8NJ",0x11},//0983 - Hunter X Hunter - Minna Tomodachi Daisakusen!!(JP).zip
{"AWVF",0x22},//0984 - X-Men 2 - La Vengeance de Wolverine(FR).zip
{"AYZE",0x22},//0985 - Rayman 3(US).zip
{"AZAJ",0x22},//0986 - Azumanga Daiou Advance(JP).zip
{"AOME",0x22},//0987 - Disney Sports - Motocross(US).zip
{"A2PJ",0x11},//0988 - Bouken Yuuki Pluster World - Pluston GP(JP).zip
{"AF3J",0x22},//0989 - Zero One(JP).zip
{"A62J",0x11},//0990 - Rockman Zero 2(JP).zip
{"AZKJ",0x22},//0991 - Simple 2960 Tomodachi Series Vol. 1 - The Table Game Collection(JP).zip
{"AOEX",0x22},//0992 - Drome Racers(EU).zip
{"AO4E",0x22},//0993 - Tom Clancy's Splinter Cell(US).zip
{"A7AJ",0x11},//0994 - Naruto - Ninjutsu Zenkai! Saikyou Ninja Daikesshuu(JP).zip
{"AZ8E",0x22},//0995 - Super Puzzle Fighter II Turbo(US).zip
{"A2CJ",0x11},//0996 - Castlevania - Akatsuki no Minuet(JP).zip
{"A2CP",0x11},//0997 - Castlevania - Aria of Sorrow(EU).zip
{"AGDE",0x11},//0998 - Lufia - The Ruins of Lore(US).zip
{"A2CE",0x11},//0999 - Castlevania - Aria of Sorrow(US).zip
{"AUGX",0x00},//1000 - Medal of Honor - Underground(EU).zip
{"AIFP",0x00},//1001 - Tom and Jerry in Infurnal Escape(EU).zip
{"AYWJ",0x11},//1002 - Yu-Gi-Oh! Duel Monsters - International Worldwide Edition(JP).zip
{"A8OJ",0x22},//1003 - Dokidoki Cooking Series 2 - Gourmet Kitchen - Suteki na Obentou(JP).zip
{"AZIE",0x00},//1004 - Finding Nemo(UE).zip
{"AZWE",0x11},//1005 - WarioWare, Inc. - Mega Microgame$!(US).zip
{"ARQP",0x00},//1006 - Matchbox Cross Town Heroes(EU).zip
{"AC6D",0x00},//1007 - Spirit - Der wilde Mustang - Auf der Suche nach Homeland(DE).zip
{"AZID",0x00},//1008 - Findet Nemo(DE).zip
{"AI2E",0x00},//1009 - Iridion II(US).zip
{"AI2P",0x00},//1010 - Iridion II(EU).zip
{"AVYD",0x22},//1011 - Buffy - Im Bann der Damonen - Koenig Darkhuls Zorn(DE).zip
{"AZWP",0x11},//1012 - WarioWare, Inc. - Minigame Mania(EU).zip
{"A5NP",0x22},//1013 - Donkey Kong Country(EU).zip
{"AYPP",0x00},//1014 - Sega Arcade Gallery(EU).zip
{"ABYY",0x00},//1015 - Britney's Dance Beat(EU).zip
{"A55H",0x00},//1016 - Weekend Miljonairs(NL).zip
{"A2RP",0x22},//1017 - Bratz(EU).zip
{"ACEE",0x00},//1018 - Agassi Tennis Generation(US).zip
{"AYWE",0x11},//1019 - Yu-Gi-Oh! Worldwide Edition - Stairway to the Destined Duel(US).zip
{"A4DP",0x22},//1020 - Disney Sports - Skateboarding(EU).zip
{"A6DP",0x22},//1021 - Disney Sports - Football(EU).zip
{"AMQP",0x00},//1022 - Midnight Club - Street Racing(EU).zip
{"A84J",0x11},//1023 - Tottoko Hamutarou 4 - Nijiiro Daikoushin Dechu(JP).zip
{"A5DP",0x22},//1024 - Disney Sports - Snowboarding(EU).zip
{"ACYE",0x22},//1025 - Chessmaster(US).zip
{"AC6P",0x00},//1026 - Spirit - Stallion of the Cimarron - Search for Homeland(EU).zip
{"A83J",0x22},//1027 - Hamster Monogatari 3 GBA(JP).zip
{"AWWJ",0x22},//1028 - Woody Woodpecker - Crazy Castle 5(JP).zip
{"AEHJ",0x22},//1029 - Puzzle & Tantei Collection(JP).zip
{"A8GJ",0x33},//1030 - GetBackers Dakkanya - Metropolis Dakkan Sakusen!(JP).zip
{"AT3D",0x22},//1031 - Tony Hawk's Pro Skater 3(DE).zip
{"AVEP",0x22},//1032 - Pro Beach Soccer(EU).zip
{"AHLE",0x22},//1033 - Incredible Hulk, The(US).zip
{"AW9E",0x22},//1034 - Wing Commander - Prophecy(US).zip
{"APJJ",0x11},//1035 - Bouken Yuuki Pluster World - Densetsu no Plust Gate(JP).zip
{"A5WE",0x00},//1036 - Rugrats - Go Wild(UE).zip
{"AK8P",0x32},//1037 - Medabots AX - Metabee Version(EU).zip
{"A2ZJ",0x32},//1038 - Zen-Nippon Shounen Soccer Taikai 2 - Mezase Nippon-ichi!(JP).zip
{"AP8D",0x22},//1039 - Scooby-Doo(DE).zip
{"A2GP",0x22},//1040 - GT Advance 3 - Pro Concept Racing(EU).zip
{"A3VE",0x33},//1041 - Sonic Pinball Party(US).zip
{"AKXD",0x22},//1042 - Spider-Man(DE).zip
{"AO4P",0x22},//1043 - Tom Clancy's Splinter Cell(EU).zip
{"AFOP",0x00},//1044 - Fortress(EU).zip
{"AWOP",0x22},//1045 - Wolfenstein 3-D(EU).zip
{"A6MP",0x22},//1046 - Megaman & Bass(EU).zip
{"ANNJ",0x22},//1047 - Gekitou Densetsu Noah - Dream Management(JP).zip
{"AK2E",0x22},//1048 - Ultimate Muscle - The Kinnikuman Legacy - The Path of the Superhero(US).zip
{"AHLP",0x22},//1049 - Incredible Hulk, The(EU).zip
{"AWDE",0x22},//1050 - Wakeboarding Unleashed featuring Shaun Murray(EU).zip
{"AFNJ",0x22},//1051 - Angel Collection - Mezase! Gakuen no Fashion Leader(JP).zip
{"AWNJ",0x22},//1052 - Mahou no Pumpkin - Ann to Greg no Daibouken(JP).zip
{"A3CP",0x22},//1053 - Crazy Taxi - Catch a Ride(EU).zip
{"AFBP",0x22},//1054 - Frogger's Adventures 2 - The Lost Wand(EU).zip
{"A5NE",0x22},//1055 - Donkey Kong Country(US).zip
{"ALFP",0x22},//1056 - Dragon Ball Z - The Legacy of Goku II(EU).zip
{"A3PP",0x00},//1057 - Sega Smash Pack(EU).zip
{"AOMJ",0x22},//1058 - Disney Sports - Motocross(JP).zip
{"A5DJ",0x22},//1059 - Disney Sports - Snowboarding(JP).zip
{"AW2E",0x32},//1060 - Advance Wars 2 - Black Hole Rising(US).zip
{"ALCE",0x00},//1061 - Little League Baseball 2002(US).zip
{"ANXE",0x22},//1062 - Ninja Five-0(US).zip
{"A6GJ",0x11},//1063 - Monster Gate - Ooinaru Dungeon - Fuuin no Orb(JP).zip
{"AEMP",0x00},//1064 - Eggo Mania(EU).zip
{"AUXP",0x22},//1065 - Stuntman(EU).zip
{"ABEP",0x22},//1066 - BattleBots - Beyond the BattleBox(EU).zip
{"A2AP",0x22},//1067 - Disney Sports - Basketball(EU).zip
{"AOMP",0x22},//1068 - Disney Sports - Motocross(EU).zip
{"A2UJ",0x11},//1069 - Mother 1+2(JP).zip
{"A9TJ",0x22},//1070 - Metal Max 2 Kai(JP).zip
{"A5UE",0x22},//1071 - Space Channel 5 - Ulala's Cosmic Attack(US).zip
{"AOEE",0x22},//1072 - Drome Racers(US).zip
{"AEXE",0x22},//1073 - King of Fighters EX2, The - Howling Blood(EU).zip
{"AJLP",0x22},//1074 - Justice League - Injustice for All(EU).zip
{"AH3P",0x11},//1075 - Hamtaro - Ham-Ham Heartbreak(EU).zip
{"A4RE",0x22},//1076 - Rock n' Roll Racing(US).zip
{"A8QE",0x00},//1077 - Pirates of the Caribbean - The Curse of the Black Pearl(US).zip
{"A3XE",0x11},//1078 - Megaman Battle Network 3 Blue(US).zip
{"APGP",0x00},//1079 - Punch King - Arcade Boxing(EU).zip
{"A6BE",0x11},//1080 - Megaman Battle Network 3 White(US).zip
{"AUXE",0x22},//1081 - Stuntman(US).zip
{"AJRE",0x22},//1082 - Jet Grind Radio(US).zip
{"A4AE",0x00},//1083 - Simpsons, The - Road Rage(UE).zip
{"AVYE",0x22},//1084 - Buffy - The Vampire Slayer - Wrath of the Darkhul King(UE).zip
{"ALFE",0x22},//1085 - Dragon Ball Z - The Legacy of Goku II(US).zip
{"AQPD",0x00},//1086 - Disneys Prinzessinnen(DE).zip
{"AFBJ",0x22},//1087 - Frogger - Mahou no Kuni no Daibouken(JP).zip
{"A8EJ",0x22},//1088 - Hachiemon(JP).zip
{"BHCJ",0x22},//1089 - Hamster Monogatari Collection(JP).zip
{"BKKJ",0x22},//1090 - Minna no Shiiku Series - Boku no Kabuto-Kuwagata(JP).zip
{"AF8X",0x22},//1091 - F1 2002(EU).zip
{"AXPD",0x31},//1092 - Pokemon - Saphir-Edition(DE).zip
{"AX4J",0x31},//1093 - Super Mario Advance 4(JP).zip
{"AV3E",0x22},//1094 - Spy Kids 3-D - Game Over(US).zip
{"AT7P",0x00},//1095 - Tootuff - The Gagmachine(EU).zip
{"A29J",0x22},//1096 - Mickey to Minnie no Magical Quest 2(JP).zip
{"BK2J",0x11},//1097 - Croket! 2 - Yami no Bank to Banqueen(JP).zip
{"BKIJ",0x22},//1098 - Nakayoshi Pet Advance Series 4 - Kawaii Koinu Mini - Wankoto Asobou!! Kogata Inu(JP).zip
{"BGBJ",0x22},//1099 - Get! - Boku no Mushi Tsukamaete(JP).zip
{"A82J",0x22},//1100 - Hamster Paradise - Pure Heart(JP).zip
{"AW9P",0x22},//1101 - Wing Commander - Prophecy(EU).zip
{"U3IJ",0x22},//1102 - Bokura no Taiyou - Taiyou Action RPG(JP).zip
{"A86J",0x33},//1103 - Sonic Pinball Party(JP).zip
{"AXVI",0x31},//1104 - Pokemon - Versione Rubino(IT).zip
{"AXPI",0x31},//1105 - Pokemon - Versione Zaffiro(IT).zip
{"AXVD",0x31},//1106 - Pokemon - Rubin-Edition(DE).zip
{"A6OJ",0x22},//1107 - Onimusha Tactics(JP).zip
{"A4KJ",0x11},//1108 - Hamster Club 4(JP).zip
{"AXPS",0x31},//1109 - Pokemon - Edicion Zafiro(ES).zip
{"AXVS",0x31},//1110 - Pokemon - Edicion Rubi(ES).zip
{"AXPF",0x31},//1111 - Pokemon - Version Saphir(FR).zip
{"A5GJ",0x11},//1112 - Dragon Drive - World D Break(JP).zip
{"AU2J",0x33},//1113 - Shining Soul II(JP).zip
{"AXVF",0x31},//1114 - Pokemon - Version Rubis(FR).zip
{"AJ6J",0x22},//1115 - Aladdin(JP).zip
{"A9CP",0x00},//1116 - CT Special Forces - Back to Hell(EU).zip
{"AN8J",0x22},//1117 - Tales of Phantasia(JP).zip
{"BPPJ",0x11},//1118 - Pokemon Pinball - Ruby & Sapphire(JP).zip
{"AC4J",0x22},//1119 - Meitantei Conan - Nerawareta Tantei(JP).zip
{"A6SJ",0x33},//1120 - Super Robot Taisen D(JP).zip
{"A89J",0x11},//1121 - Rockman EXE Battle Chip GP(JP).zip
{"AYEJ",0x22},//1122 - Top Gear Rally(JP).zip
{"PSAJ",0x31},//1123 - Card e-Reader+(JP).zip
{"A8ZJ",0x22},//1124 - Shin Megami Tensei Devil Children - Puzzle de Call!(JP).zip
{"AWTD",0x00},//1125 - Expedition der Stachelbeeren - Zoff im Zoo(DE).zip
{"AOIE",0x22},//1126 - Shrek - Reekin' Havoc(US).zip
{"AZ3J",0x11},//1127 - Cyberdrive Zoids - Kijuu no Senshi Hyuu(JP).zip
{"AA9J",0x11},//1128 - Duel Masters(JP).zip
{"BGMJ",0x22},//1129 - Gensou Maden Saiyuuki - Hangyaku no Toushin-taishi(JP).zip
{"BWDJ",0x22},//1130 - Wan Nyan Doubutsu Byouin(JP).zip
{"BMDE",0x22},//1131 - Madden NFL 2004(US).zip
{"BMTE",0x22},//1132 - Monster Truck Madness(UE).zip
{"A8SP",0x22},//1133 - Digimon - Battle Spirit(EU).zip
{"A4RP",0x22},//1134 - Rock n' Roll Racing(EU).zip
{"BPPE",0x11},//1135 - Pokemon Pinball - Ruby & Sapphire(US).zip
{"AVSJ",0x33},//1136 - Shinyaku Seiken Densetsu(JP).zip
{"BOBJ",0x11},//1137 - Boboboubo Boubobo - Maji de!! Shinken Battle(JP).zip
{"AW4E",0x22},//1138 - Mortal Kombat - Tournament Edition(US).zip
{"AQMP",0x22},//1139 - Magical Quest 2 Starring Mickey & Minnie(EU).zip
{"AQHE",0x00},//1140 - Rescue Heroes - Billy Blazes!(US).zip
{"AFXE",0x33},//1141 - Final Fantasy Tactics Advance(US).zip
{"BESE",0x22},//1142 - Extreme Skate Adventure(UE).zip
{"AVFJ",0x11},//1143 - Densetsu no Stafy 2(JP).zip
{"BGFJ",0x11},//1144 - GetBackers Dakkanya - Jagan Fuuin!(JP).zip
{"BFSE",0x00},//1145 - Freekstyle(US).zip
{"AZZE",0x00},//1146 - Rocket Power - Zero Gravity Zone(US).zip
{"BPWE",0x00},//1147 - Power Rangers - Ninja Storm(US).zip
{"AYHP",0x22},//1148 - Starsky & Hutch(EU).zip
{"AZSE",0x22},//1149 - Gem Smashers(US).zip
{"ADBE",0x22},//1150 - Denki Blocks!(US).zip
{"AUEJ",0x11},//1151 - Naruto - Konoha Senki(JP).zip
{"BIOE",0x00},//1152 - Bionicle(US).zip
{"BMME",0x22},//1153 - Scooby-Doo! - Mystery Mayhem(US).zip
{"BKZE",0x22},//1154 - Banjo-Kazooie - Grunty's Revenge(US).zip
{"AW2P",0x32},//1155 - Advance Wars 2 - Black Hole Rising(EU).zip
{"AGFD",0x32},//1156 - Golden Sun 2 - Die Vergessene Epoche(DE).zip
{"BO3J",0x22},//1157 - Oshare Princess 3(JP).zip
{"AOTP",0x00},//1158 - Polly Pocket! - Super Splash Island(EU).zip
{"BESX",0x22},//1159 - Extreme Skate Adventure(EU).zip
{"AZRP",0x00},//1160 - Mr. Nutz(EU).zip
{"U3IE",0x22},//1161 - Boktai - The Sun is in Your Hand(US).zip
{"AGFS",0x32},//1162 - Golden Sun - La Edad Perdida(ES).zip
{"AGFF",0x32},//1163 - Golden Sun - L'Age Perdu(FR).zip
{"AAJJ",0x11},//1164 - Shin Kisekae Monogatari(JP).zip
{"BOJJ",0x22},//1165 - Ojarumaru - Gekkouchou Sanpo de Ojaru(JP).zip
{"A8DJ",0x33},//1166 - Doubutsu Shima no Chobi Gurumi(JP).zip
{"BD8E",0x22},//1167 - Disney's Party(UE).zip
{"AQXE",0x22},//1168 - Blackthorne(US).zip
{"BTQJ",0x11},//1169 - Tantei Gakuen Q - Meitantei ha Kimi da!(JP).zip
{"BDYJ",0x11},//1170 - Shin Megami Tensei Devil Children - Koori no Sho(JP).zip
{"BDHJ",0x11},//1171 - Shin Megami Tensei Devil Children - Honoo no Sho(JP).zip
{"AI8E",0x00},//1172 - Barbie Horse Adventures - Blue Ribbon Race(US).zip
{"A9RE",0x00},//1173 - Road Rash - Jailbreak(US).zip
{"A4AX",0x00},//1174 - Simpsons, The - Road Rage(EU).zip
{"AGFI",0x32},//1175 - Golden Sun - L'Era Perduta(IT).zip
{"BDIJ",0x22},//1176 - Koinu to Issho - Aijou Monogatari(JP).zip
{"A9NX",0x00},//1178 - Piglet's Big Game(EU).zip
{"A7KP",0x11},//1179 - Kirby - Nightmare in Dream Land(EU).zip
{"BJNE",0x00},//1180 - Adventures of Jimmy Neutron, The - Boy Genius - Jet Fusion(UE).zip
{"A5WF",0x00},//1181 - Razmoket, Les - Rencontrent les Delajungle(FR).zip
{"BDSE",0x22},//1182 - Digimon - Battle Spirit 2(US).zip
{"BBDE",0x00},//1183 - BattleBots - Design & Destroy(US).zip
{"AJ8J",0x22},//1184 - Jurassic Park Institute Tour - Dinosaur Rescue(JP).zip
{"BODE",0x00},//1185 - Oddworld - Munch's Oddysee(UE).zip
{"ABDP",0x22},//1186 - Boulder Dash EX(EU).zip
{"A6BP",0x11},//1187 - Megaman Battle Network 3 White(EU).zip
{"BZDJ",0x11},//1188 - Zettaizetsumei Dangerous Jiisan - Shijou Saikyou no Dogeza(JP).zip
{"A5TJ",0x11},//1189 - Shin Megami Tensei II(JP).zip
{"AX4P",0x31},//1190 - Super Mario Advance 4 - Super Mario Bros. 3(EU).zip
{"AH9P",0x22},//1191 - Hobbit, The(EU).zip
{"BBFJ",0x22},//1192 - Battle X Battle - Kyodai Gyo Densetsu(JP).zip
{"A8QP",0x00},//1193 - Pirates of the Caribbean - The Curse of the Black Pearl(EU).zip
{"AFXP",0x33},//1194 - Final Fantasy Tactics Advance(EU).zip
{"BLKE",0x22},//1195 - Lion King 1.5, The(US).zip
{"AQXP",0x22},//1196 - Blackthorne(EU).zip
{"BQDE",0x00},//1197 - Quad Desert Fury(US).zip
{"ANMP",0x00},//1198 - Namco Museum(EU).zip
{"BYHE",0x22},//1199 - Backyard Hockey(US).zip
{"AQDE",0x00},//1200 - Crouching Tiger, Hidden Dragon(US).zip
{"AC5E",0x11},//1201 - DemiKids - Dark Version(US).zip
{"AL4E",0x11},//1202 - DemiKids - Light Version(US).zip
{"AI8P",0x00},//1203 - Barbie Horse Adventures(EU).zip
{"A3XP",0x11},//1204 - Megaman Battle Network 3 Blue(EU).zip
{"BMAJ",0x11},//1205 - Mermaid Melody - Pichi Pichi Pitch(JP).zip
{"BOMJ",0x22},//1206 - Bomberman Jetters - Game Collection(JP).zip
{"A62E",0x11},//1207 - Megaman Zero 2(US).zip
{"BJUE",0x22},//1208 - Tak and the Power of Juju(US).zip
{"BMPJ",0x11},//1209 - Wagamama Fairy Mirumo de Pon! - Taisen Mahoudama(JP).zip
{"BKGJ",0x22},//1210 - Kawaii Pet Game Gallery(JP).zip
{"BMRJ",0x22},//1211 - Matantei Loki Ragnarok - Gensou no Labyrinth(JP).zip
{"AX4E",0x31},//1212 - Super Mario Advance 4 - Super Mario Bros. 3(US).zip
{"BNTE",0x22},//1213 - Teenage Mutant Ninja Turtles(US).zip
{"AORJ",0x11},//1214 - Oriental Blue - Ao no Tengai(JP).zip
{"BPWP",0x00},//1215 - Power Rangers - Ninja Storm(EU).zip
{"BKZX",0x22},//1216 - Banjo-Kazooie - Grunty's Revenge(EU).zip
{"AVDJ",0x11},//1217 - Legend of Dynamic Goushouden - Houkai no Rondo(JP).zip
{"BDPE",0x22},//1218 - Super Duper Sumos(US).zip
{"BIOP",0x00},//1219 - Bionicle(EU).zip
{"AQPF",0x00},//1220 - Disney Princesse(FR).zip
{"BLME",0x22},//1221 - Lizzie McGuire - On the Go!(US).zip
{"BTGX",0x22},//1222 - Top Gear Rally(EU).zip
{"BATE",0x00},//1223 - Batman - Rise of Sin Tzu(US).zip
{"BLKP",0x22},//1224 - Lion King, The(EU).zip
{"AOWE",0x22},//1225 - Spyro - Attack of the Rhynocs(US).zip
{"BTOE",0x22},//1226 - Tony Hawk's Underground(UE).zip
{"BAAE",0x00},//1227 - Operation Armored Liberty(US).zip
{"A62P",0x11},//1228 - Megaman Zero 2(EU).zip
{"BFJE",0x22},//1229 - Frogger's Journey - The Forgotten Relic(US).zip
{"BTGE",0x22},//1230 - Top Gear Rally(US).zip
{"BHPE",0x22},//1231 - Harry Potter - Quidditch World Cup(UE).zip
{"BPYE",0x22},//1232 - Prince of Persia - The Sands of Time(US).zip
{"BSQE",0x00},//1233 - SpongeBob SquarePants - Battle for Bikini Bottom(US).zip
{"AZHP",0x00},//1234 - Hugo - Bukkazoom!(EU).zip
{"AE7E",0x11},//1235 - Fire Emblem(US).zip
{"BLRE",0x22},//1236 - Lord of the Rings, The - The Return of the King(UE).zip
{"AXPE",0x31},//1237 - Pokemon - Sapphire Version(EU).zip
{"BFIE",0x22},//1238 - FIFA 2004(UE).zip
{"A3VP",0x33},//1239 - Sonic Pinball Party(EU).zip
{"AY7E",0x11},//1240 - Yu-Gi-Oh! The Sacred Cards(US).zip
{"BBRE",0x22},//1241 - Brother Bear(US).zip
{"AXVE",0x31},//1242 - Pokemon - Ruby Version(EU).zip
{"AF6E",0x00},//1243 - Fairly Odd Parents!, The - Breakin' da Rules(US).zip
{"AQTE",0x22},//1244 - Dr. Seuss' The Cat in the Hat(US).zip
{"BPPP",0x11},//1245 - Pokemon Pinball - Ruby & Sapphire(EU).zip
{"A88P",0x22},//1246 - Mario & Luigi - Superstar Saga(EU).zip
{"BSHJ",0x22},//1247 - Minna no Soft Series - Shanghai(JP).zip
{"AYDP",0x32},//1248 - Yu-Gi-Oh! Dungeon Dice Monsters(EU).zip
{"BTWE",0x22},//1249 - Tiger Woods PGA Tour 2004(UE).zip
{"AVYF",0x22},//1250 - Buffy Contre les Vampires - La Colere de Darkhul(FR).zip
{"BSXE",0x22},//1251 - SSX 3(UE).zip
{"BPYP",0x22},//1252 - Prince of Persia - The Sands of Time(EU).zip
{"BEYP",0x22},//1253 - Beyblade V-Force - Ultimate Blader Jam(EU).zip
{"A9KJ",0x22},//1254 - Slime Morimori Dragon Quest - Shougeki no Shippo Dan(JP).zip
{"BMJJ",0x22},//1255 - Minna no Soft Series - Minna no Mahjong(JP).zip
{"A85J",0x22},//1256 - Sanrio Puroland - All Characters(JP).zip
{"BMZJ",0x22},//1257 - Minna no Soft Series - Zooo(JP).zip
{"A6OE",0x22},//1258 - Onimusha Tactics(US).zip
{"BJLE",0x22},//1259 - Justice League Chronicles(US).zip
{"ANRE",0x00},//1260 - Cartoon Network - Speedway(US).zip
{"AQME",0x22},//1261 - Magical Quest 2 Starring Mickey & Minnie(US).zip
{"BCNE",0x22},//1262 - Crash Nitro Kart(US).zip
{"AVEE",0x22},//1263 - Ultimate Beach Soccer(US).zip
{"AH9E",0x22},//1264 - Hobbit, The(US).zip
{"AWNE",0x00},//1265 - Spirits & Spells(US).zip
{"BDDE",0x00},//1266 - Double Dragon Advance(US).zip
{"AVSE",0x33},//1267 - Sword of Mana(US).zip
{"A4NE",0x11},//1268 - Harvest Moon - Friends of Mineral Town(US).zip
{"AZIX",0x00},//1269 - Finding Nemo(EU).zip
{"AOWP",0x22},//1270 - Spyro Adventure(EU).zip
{"BLTE",0x22},//1271 - Looney Tunes - Back in Action(UE).zip
{"BM3J",0x22},//1272 - Mickey to Donald no Magical Quest 3(JP).zip
{"AV3P",0x22},//1273 - Spy Kids 3-D - Game Over(EU).zip
{"AEYP",0x00},//1274 - Kim Possible(EU).zip
{"BJBE",0x22},//1275 - 007 - Everything or Nothing(UE).zip
{"BMHE",0x22},//1276 - Medal of Honor - Infiltrator(UE).zip
{"A88E",0x22},//1277 - Mario & Luigi - Superstar Saga(US).zip
{"AO3P",0x00},//1278 - Terminator 3 - Rise of the Machines(EU).zip
{"BEYE",0x22},//1279 - Beyblade V-Force - Ultimate Blader Jam(US).zip
{"AO3E",0x00},//1280 - Terminator 3 - Rise of the Machines(US).zip
{"BCME",0x22},//1281 - CIMA - The Enemy(US).zip
{"BSWE",0x00},//1282 - Star Wars - Flight of the Falcon(US).zip
{"A88J",0x22},//1283 - Mario & Luigi RPG(JP).zip
{"BSWP",0x00},//1284 - Star Wars - Flight of the Falcon(EU).zip
{"BHPJ",0x22},//1285 - Harry Potter - Quidditch World Cup(JP).zip
{"A5CP",0x22},//1286 - Sim City 2000(EU).zip
{"BMLE",0x22},//1287 - Mucha Lucha! - Mascaritas of the Lost Code(US).zip
{"BUSE",0x22},//1288 - Green Eggs and Ham by Dr. Seuss(US).zip
{"BIDE",0x00},//1289 - American Idol(US).zip
{"BNTP",0x22},//1290 - Teenage Mutant Ninja Turtles(EU).zip
{"BHWE",0x00},//1291 - Hot Wheels - World Race(US).zip
{"BFZJ",0x11},//1292 - F-Zero - Falcon Densetsu(JP).zip
{"BCNP",0x22},//1293 - Crash Nitro Kart(EU).zip
{"BGAJ",0x22},//1294 - SD Gundam G-Generation Advance(JP).zip
{"BHWP",0x00},//1295 - Hot Wheels - World Race(EU).zip
{"BODD",0x00},//1296 - Oddworld - Munch's Oddysee(DE).zip
{"BDBE",0x22},//1297 - Dragon Ball Z - Taiketsu(US).zip
{"AQDP",0x00},//1298 - Crouching Tiger, Hidden Dragon(EU).zip
{"AWYE",0x00},//1299 - Worms World Party(US).zip
{"BERE",0x00},//1300 - Dora the Explorer - Super Spies(US).zip
{"BPMP",0x33},//1301 - Premier Manager 2003-04(EU).zip
{"AIHP",0x22},//1302 - Mission Impossible - Operation Surma(EU).zip
{"A4XJ",0x22},//1303 - Gachasute! Dino Device 2 - Dragon(JP).zip
{"A4WJ",0x22},//1304 - Gachasute! Dino Device 2 - Phoenix(JP).zip
{"A6OP",0x22},//1305 - Onimusha Tactics(EU).zip
{"BLMP",0x22},//1306 - Lizzie McGuire(EU).zip
{"BP6J",0x11},//1307 - Power Pro Kun Pocket 6(JP).zip
{"BYYE",0x22},//1308 - Yu Yu Hakusho - Spirit Detective(US).zip
{"BSBJ",0x33},//1309 - Sonic Battle(JP).zip
{"BPZJ",0x22},//1310 - Pazunin - Uminin no Puzzle de Nimu(JP).zip
{"BIDD",0x00},//1311 - Deutschland sucht den Superstar(DE).zip
{"ASIE",0x22},//1312 - Sims, The - Bustin' Out(UE).zip
{"BAVE",0x22},//1313 - Activision Anthology(US).zip
{"BUWE",0x00},//1314 - Ultimate Winter Games(US).zip
{"AQWE",0x11},//1315 - Game & Watch Gallery 4(US).zip
{"BISJ",0x22},//1316 - Koinu-Chan no Hajimete no Osanpo - Koinu no Kokoro Ikusei Game(JP).zip
{"BPSJ",0x22},//1317 - Cinnamoroll Kokoniiruyo(JP).zip
{"BSDP",0x00},//1318 - Sitting Ducks(EU).zip
{"B4BJ",0x11},//1319 - Rockman EXE 4 - Tournament Blue Moon(JP).zip
{"B4WJ",0x11},//1320 - Rockman EXE 4 - Tournament Red Sun(JP).zip
{"BPTE",0x22},//1321 - Peter Pan - The Motion Picture Event(US).zip
{"BK3J",0x22},//1322 - Cardcaptor Sakura - Sakura Card de Mini Game(JP).zip
{"A4GJ",0x22},//1323 - Konjiki no Gashbell!! - Unare! Yuujou no Zakeru(JP).zip
{"BFGJ",0x11},//1324 - Bokujou Monogatari - Mineral Town no Nakamatachi for Girl(JP).zip
{"BSSE",0x00},//1325 - Spy Muppets - License to Croak(US).zip
{"B3KJ",0x11},//1326 - Croket! 3 - Guranyuu Oukoku no Nazo(JP).zip
{"BGKJ",0x11},//1327 - Gegege no Kitarou - Kikiippatsu! Youkai Rettou(JP).zip
{"AEXE",0x22},//1328 - King of Fighters EX2, The - Howling Blood(US).zip
{"BMYJ",0x11},//1329 - Wagamama Fairy Mirumo de Pon! - 8 Nin no Toki no Yousei(JP).zip
{"A5NJ",0x22},//1330 - Super Donkey Kong(JP).zip
{"BFNJ",0x00},//1331 - Finding Nemo(JP).zip
{"BTAJ",0x22},//1332 - Astro Boy - Tetsuwan Atom - Atom Heart no Himitsu(JP).zip
{"A5DE",0x22},//1333 - Disney Sports - Snowboarding(US).zip
{"AC8J",0x22},//1334 - Crash Bandicoot Advance 2 - Gurugurusaimin Dai Panic!(JP).zip
{"BMEE",0x22},//1335 - Max Payne(US).zip
{"BTTJ",0x22},//1336 - Tetris Advance(JP).zip
{"BDRJ",0x22},//1337 - Ochaken no Heya(JP).zip
{"BNSE",0x22},//1338 - Need for Speed - Underground(UE).zip
{"BS5J",0x22},//1339 - Silvanian Family - Yousei no Stick to Fushigi no Ki - Marron-inu no Onna-no-ko(JP).zip
{"BS4J",0x22},//1340 - Simple 2960 Tomodachi Series Vol. 4 - The Trump - Minna de Asoberu 12 Shurui no Trump Game(JP).zip
{"BWMJ",0x22},//1341 - WanWan Meitantei(JP).zip
{"BAMJ",0x11},//1342 - Ashita no Joe - Makka ni Moeagare!(JP).zip
{"A5CE",0x22},//1343 - Sim City 2000(US).zip
{"ALRP",0x22},//1344 - LEGO Racers 2(EU).zip
{"AARP",0x22},//1345 - Altered Beast - Guardian of the Realms(EU).zip
{"ATKP",0x22},//1346 - Tekken Advance(EU).zip
{"BMOJ",0x11},//1347 - Minna no Ouji-sama(JP).zip
{"AGEE",0x00},//1348 - Gekido Advance - Kintaro's Revenge(US).zip
{"AWIP",0x22},//1349 - ESPN International Winter Sports 2002(EU).zip
{"AG4J",0x00},//1350 - Godzilla - Kaijuu Dairantou Advance(JP).zip
{"BSBE",0x33},//1351 - Sonic Battle(US).zip
{"BHYJ",0x22},//1352 - Hyokkori Hyoutanjima - Don Gabacho Daikatsuyaku no Maki(JP).zip
{"AHUE",0x33},//1353 - Shining Soul(US).zip
{"BFSP",0x00},//1354 - Freekstyle(EU).zip
{"A9RP",0x00},//1355 - Road Rash - Jailbreak(EU).zip
{"BGDE",0x22},//1356 - Baldur's Gate - Dark Alliance(US).zip
{"A3JJ",0x11},//1357 - Gyakuten Saiban 3(JP).zip
{"AWSE",0x00},//1358 - Tiny Toon Adventures - Wacky Stackers(US).zip
{"B4PJ",0x22},//1359 - Sims, The(JP).zip
{"BPRJ",0x31},//1360 - Pocket Monsters - FireRed(JP).zip
{"AVZP",0x22},//1361 - Super Bubble Pop(EU).zip
{"BPGJ",0x31},//1362 - Pocket Monsters - LeafGreen(JP).zip
{"BMMP",0x22},//1363 - Scooby-Doo! - Mystery Mayhem(EU).zip
{"AY7P",0x11},//1364 - Yu-Gi-Oh! The Sacred Cards(EU).zip
{"BBRX",0x22},//1365 - Brother Bear(EU).zip
{"BY3J",0x11},//1366 - Yu-Gi-Oh! Duel Monsters Expert 3(JP).zip
{"BSQP",0x00},//1367 - SpongeBob SquarePants - Battle for Bikini Bottom(EU).zip
{"BGDP",0x22},//1368 - Baldur's Gate - Dark Alliance(EU).zip
{"AD2P",0x11},//1369 - Mr. Driller 2(EU).zip
{"BMXE",0x11},//1370 - Metroid - Zero Mission(US).zip
{"APYP",0x22},//1371 - Puyo Pop(EU).zip
{"AQPI",0x00},//1372 - Disney Principesse(IT).zip
{"AVCE",0x22},//1373 - Corvette(US).zip
{"BOZE",0x00},//1374 - Ozzy & Drix(US).zip
{"BM8J",0x11},//1375 - Mermaid Melody - Pichipichi Pitch Pichipichi Party(JP).zip
{"AJRP",0x22},//1376 - Jet Set Radio(EU).zip
{"AZFP",0x22},//1377 - Need for Speed - Porsche Unleashed(EU).zip
{"BYWE",0x11},//1378 - Yu-Gi-Oh! World Championship Tournament 2004(US).zip
{"AC7E",0x00},//1379 - CT Special Forces(US).zip
{"ALEE",0x22},//1380 - Bruce Lee - Return of the Legend(US).zip
{"FICJ",0x11},//1381 - Famicom Mini Vol. 03 - Ice Climber(JP).zip
{"FXVJ",0x11},//1382 - Famicom Mini Vol. 07 - Xevious(JP).zip
{"FSMJ",0x11},//1383 - Famicom Mini Vol. 01 - Super Mario Bros.(JP).zip
{"BPHP",0x22},//1384 - Pitfall - The Lost Expedition(EU).zip
{"FDKJ",0x11},//1385 - Famicom Mini Vol. 02 - Donkey Kong(JP).zip
{"FZLJ",0x11},//1386 - Famicom Mini Vol. 05 - The Hyrule Fantasy - Zelda no Densetsu 1(JP).zip
{"FMPJ",0x11},//1387 - Famicom Mini Vol. 08 - Mappy(JP).zip
{"ACYD",0x22},//1388 - Chessmaster(DE).zip
{"A4ND",0x11},//1389 - Harvest Moon - Friends of Mineral Town(DE).zip
{"FSOJ",0x11},//1390 - Famicom Mini Vol. 10 - Star Soldier(JP).zip
{"FEBJ",0x00},//1391 - Famicom Mini Vol. 04 - Excitebike(JP).zip
{"FBMJ",0x11},//1392 - Famicom Mini Vol. 09 - Bomberman(JP).zip
{"FPMJ",0x11},//1393 - Famicom Mini Vol. 06 - Pac-Man(JP).zip
{"BPTP",0x00},//1394 - Peter Pan - The Motion Picture Event(EU).zip
{"BPHF",0x22},//1395 - Pitfall - L'Expedition Perdue(FR).zip
{"AZIY",0x00},//1396 - Finding Nemo(EU).zip
{"BGSJ",0x22},//1397 - Gakuen Senki Muryou(JP).zip
{"BKZI",0x22},//1398 - Banjo-Kazooie - La Vendetta di Grunty(IT).zip
{"AMXX",0x00},//1399 - Monsters, Inc.(EU).zip
{"BDTJ",0x22},//1400 - Downtown - Nekketsu Monogatari EX(JP).zip
{"AAVP",0x00},//1401 - Atari Anniversary Advance(EU).zip
{"ASQP",0x00},//1402 - Snood(EU).zip
{"A89E",0x11},//1403 - Megaman - Battle Chip Challenge(US).zip
{"BU6J",0x22},//1404 - Taiketsu! Ultra Hero(JP).zip
{"AGXE",0x32},//1406 - Guilty Gear X - Advance Edition(US).zip
{"AU2P",0x33},//1407 - Shining Soul II(EU).zip
{"BSLP",0x22},//1408 - Tom Clancy's Splinter Cell - Pandora Tomorrow(EU).zip
{"B08J",0x22},//1409 - One Piece - Going Baseball(JP).zip
{"BADP",0x22},//1410 - Aladdin(EU).zip
{"BIDP",0x00},//1411 - Pop Idol(EU).zip
{"AWUP",0x22},//1412 - Sabre Wulf(EU).zip
{"BJUP",0x22},//1413 - Tak and the Power of Juju(EU).zip
{"AQTP",0x22},//1414 - Dr. Seuss' The Cat in the Hat(EU).zip
{"AVSX",0x33},//1415 - Sword of Mana(EU).zip
{"BYSJ",0x11},//1416 - Yu-Gi-Oh! Sugoroku no Sugoroku(JP).zip
{"BMUE",0x00},//1417 - Scooby-Doo 2 - Monsters Unleashed(UE).zip
{"BR3E",0x00},//1418 - R-Type III - The Third Lightning(US).zip
{"A89P",0x11},//1419 - Megaman - Battle Chip Challenge(EU).zip
{"BDBP",0x22},//1420 - Dragon Ball Z - Taiketsu(EU).zip
{"BMEP",0x22},//1421 - Max Payne Advance(EU).zip
{"AVSY",0x33},//1422 - Sword of Mana(EU).zip
{"BMQP",0x22},//1423 - Magical Quest 3 Starring Mickey & Donald(EU).zip
{"BSRE",0x22},//1424 - Wade Hixton's Counter Punch(UE).zip
{"BTIJ",0x11},//1425 - Tantei Gakuen Q - Kyukyoku Trick ni Idome(JP).zip
{"B3MJ",0x11},//1426 - Mermaid Melody - Pichi Pichi Picchi Pichi Pichitto Live Start(JP).zip
{"AZJJ",0x22},//1427 - Dragon Ball Z - Bukuu Tougeki(JP).zip
{"BDDJ",0x00},//1428 - Double Dragon Advance(JP).zip
{"BS3J",0x22},//1429 - Simple 2960 Tomodachi Series Vol. 3 - The Itsudemo Puzzle(JP).zip
{"BHRJ",0x22},//1430 - Hagane no Renkinjutsushi - Meisou no Rondo(JP).zip
{"BFKP",0x22},//1431 - Franklin the Turtle(EU).zip
{"A4NP",0x11},//1432 - Harvest Moon - Friends of Mineral Town(EU).zip
{"APTP",0x22},//1433 - Powerpuff Girls, The - Mojo Jojo A-Go-Go(EU).zip
{"ADLP",0x22},//1434 - Dexter's Laboratory - Deesaster Strikes!(EU).zip
{"BYWP",0x11},//1435 - Yu-Gi-Oh! World Championship Tournament 2004(EU).zip
{"AJFE",0x00},//1436 - Jungle Book, The(US).zip
{"A3ZE",0x22},//1437 - Street Jam Basketball(UE).zip
{"BD2J",0x11},//1438 - Duel Masters 2 - Invisible Advance(JP).zip
{"BRPJ",0x22},//1439 - Liliput Oukoku(JP).zip
{"BB9J",0x11},//1440 - Boboboubo Boubobo - 9 Kyoku Senshi Gag Yuugou(JP).zip
{"BDJJ",0x22},//1441 - Digimon Racing(JP).zip
{"AAQE",0x00},//1442 - Animal Snap - Rescue Them 2 by 2(US).zip
{"ABQE",0x22},//1443 - David Beckham Soccer(US).zip
{"ANUE",0x00},//1444 - Antz - Extreme Racing(US).zip
{"BC3P",0x00},//1445 - CT Special Forces 3 - Bioterror(EU).zip
{"A9AE",0x00},//1446 - Demon Driver - Time to Burn Rubber!(US).zip
{"AIHE",0x22},//1447 - Mission Impossible - Operation Surma(US).zip
{"BHME",0x00},//1448 - Home on the Range(US).zip
{"A3QP",0x00},//1449 - Sound of Thunder, A(EU).zip
{"AR3E",0x22},//1450 - Ice Nine(UE).zip
{"BMUX",0x00},//1451 - Scooby-Doo 2 - Monsters Unleashed(EU).zip
{"BMXP",0x11},//1452 - Metroid - Zero Mission(EU).zip
{"BPNJ",0x22},//1453 - Pikapika Nurse Monogatari - Nurse Ikusei Game(JP).zip
{"B8KJ",0x11},//1454 - Hoshi no Kirby - Kagami no Daimeikyuu(JP).zip
{"BOXP",0x22},//1455 - FightBox(EU).zip
{"BKWE",0x22},//1456 - Book Worm(US).zip
{"BZ3J",0x11},//1457 - Rockman Zero 3(JP).zip
{"BMGJ",0x11},//1458 - Mario Golf - GBA Tour(JP).zip
{"A6RE",0x11},//1459 - Shifting Gears Road Trip(US).zip
{"AU2E",0x33},//1460 - Shining Soul II(US).zip
{"BKSJ",0x22},//1461 - Cardcaptor Sakura - Card Friends(JP).zip
{"BZGJ",0x11},//1462 - Zettaizetsumei Dangerous Jiisan - Naki no Ikkai Zettai Fukujuu Violence Kouchou(JP).zip
{"BUVJ",0x11},//1463 - Uchuu no Stellvia(JP).zip
{"BP3J",0x22},//1464 - Pia Carrot he Youkoso!! 3.3(JP).zip
{"AENE",0x00},//1465 - Serious Sam Advance(US).zip
{"BTPE",0x00},//1466 - Ten Pin Alley 2(US).zip
{"BDGP",0x22},//1467 - Digimon Racing(EU).zip
{"BCLE",0x00},//1468 - Super Collapse! II(US).zip
{"BN2J",0x11},//1469 - Naruto - Saikyou Ninja Daikesshuu 2(JP).zip
{"AF5P",0x11},//1470 - Shining Force - Resurrection of the Dark Dragon(EU).zip
{"BLRJ",0x22},//1471 - Lord of the Rings, The - Ou no Kikan(JP).zip
{"ANVJ",0x11},//1472 - Shiren Monsters Netsal(JP).zip
{"B4SJ",0x11},//1473 - Tennis no Ouji-sama 2004 - Stylish Silver(JP).zip
{"B4GJ",0x11},//1474 - Tennis no Ouji-sama 2004 - Glorious Gold(JP).zip
{"BPBJ",0x11},//1475 - Pyuu to Fuku! Jaguar - Byoo to Deru! Megane-kun(JP).zip
{"AO7K",0x11},//1476 - One Piece - Ilgop Seomui Daebomul(KS).zip
{"BSEE",0x22},//1477 - Shrek 2(UE).zip
{"BANE",0x00},//1478 - Van Helsing(US).zip
{"BKCJ",0x22},//1479 - Crayon Shin-chan - Arashi wo Yobu Cinema Land no Daibouken!(JP).zip
{"AENP",0x00},//1480 - Serious Sam Advance(EU).zip
{"BKTJ",0x22},//1481 - Koutetsu Teikoku from HOT-B(JP).zip
{"BGNJ",0x22},//1482 - Kidou Senshi Gundam Seed - Tomo to Kimi to koko de.(JP).zip
{"U3IP",0x22},//1483 - Boktai - The Sun is in Your Hand(EU).zip
{"BANP",0x00},//1484 - Van Helsing(EU).zip
{"MSSE",0x00},//1485 - Game Boy Advance Video - SpongeBob SquarePants - Volume 1(US).zip
{"MS2E",0x00},//1486 - Game Boy Advance Video - SpongeBob SquarePants - Volume 2(US).zip
{"MF2E",0x00},//1487 - Game Boy Advance Video - Fairly Odd Parents!, The - Volume 2(US).zip
{"BJBJ",0x22},//1488 - 007 - Everything or Nothing(JP).zip
{"BM5E",0x33},//1489 - Mario vs. Donkey Kong(US).zip
{"B3SE",0x33},//1490 - Sonic Advance 3(US).zip
{"AQ2P",0x11},//1491 - Gadget Racers(EU).zip
{"FTWJ",0x11},//1492 - Famicom Mini Vol. 19 - Twin Bee(JP).zip
{"FBFJ",0x11},//1493 - Famicom Mini Vol. 13 - Balloon Fight(JP).zip
{"FMKJ",0x11},//1494 - Famicom Mini Vol. 18 - Makaimura(JP).zip
{"FDDJ",0x11},//1495 - Famicom Mini Vol. 16 - Dig Dug(JP).zip
{"FMBJ",0x11},//1496 - Famicom Mini Vol. 11 - Mario Bros.(JP).zip
{"FTBJ",0x11},//1497 - Famicom Mini Vol. 17 - Takahashi Meijin no Bouken Shima(JP).zip
{"FGGJ",0x11},//1498 - Famicom Mini Vol. 20 - Ganbare Goemon! Karakuri Douchuu(JP).zip
{"FWCJ",0x11},//1499 - Famicom Mini Vol. 14 - Wrecking Crew(JP).zip
{"FDMJ",0x11},//1500 - Famicom Mini Vol. 15 - Dr. Mario(JP).zip
{"FCLJ",0x11},//1501 - Famicom Mini Vol. 12 - Clu Clu Land(JP).zip
{"BDTE",0x22},//1502 - River City Ransom EX(US).zip
{"BHTE",0x22},//1503 - Harry Potter and the Prisoner of Azkaban(UE).zip
{"BTFJ",0x11},//1504 - Tokyo Majin Gakuen - Fuju Houroku(JP).zip
{"MDRE",0x00},//1505 - Game Boy Advance Video - Dora the Explorer - Volume 1(US).zip
{"BMBE",0x22},//1506 - Mighty Beanz - Pocket Puzzles(US).zip
{"MSHE",0x00},//1507 - Game Boy Advance Video - Sonic X - Volume 1(US).zip
{"BFZP",0x11},//1508 - F-Zero - GP Legend(EU).zip
{"BD4E",0x22},//1509 - Crash Bandicoot Purple - Ripto's Rampage(US).zip
{"BSTE",0x22},//1510 - Spyro Orange - The Cortex Conspiracy(US).zip
{"AA9E",0x22},//1511 - Duel Masters - Sempai Legends(US).zip
{"FZLE",0x11},//1512 - Classic NES Series - The Legend of Zelda(UE).zip
{"BSBP",0x33},//1513 - Sonic Battle(EU).zip
{"FSME",0x11},//1514 - Classic NES Series - Super Mario Bros.(UE).zip
{"MSBE",0x00},//1515 - Game Boy Advance Video - Strawberry Shortcake - Volume 1(US).zip
{"FBME",0x11},//1516 - Classic NES Series - Bomberman(UE).zip
{"FEBE",0x11},//1517 - Classic NES Series - Excitebike(UE).zip
{"FXVE",0x11},//1518 - Classic NES Series - Xevious(UE).zip
{"FP7E",0x11},//1519 - Classic NES Series - Pac-Man(UE).zip
{"FICE",0x11},//1520 - Classic NES Series - Ice Climber(UE).zip
{"FDKE",0x11},//1521 - Classic NES Series - Donkey Kong(UE).zip
{"MFOE",0x00},//1522 - Game Boy Advance Video - Fairly Odd Parents!, The - Volume 1(US).zip
{"BUCE",0x22},//1523 - Ultimate Card Games(UE).zip
{"MGUE",0x00},//1524 - Game Boy Advance Video - All Grown Up! - Volume 1(US).zip
{"BNJJ",0x22},//1525 - Jajamaru Jr. Denshouki - Jalecolle mo Arisourou(JP).zip
{"BM5J",0x33},//1526 - Mario vs. Donkey Kong(JP).zip
{"BMXJ",0x11},//1527 - Metroid - Zero Mission(JP).zip
{"BMHJ",0x22},//1528 - Medal of Honor - Infiltrator(JP).zip
{"AWUE",0x22},//1529 - Sabre Wulf(US).zip
{"AF5E",0x11},//1530 - Shining Force - Resurrection of the Dark Dragon(US).zip
{"BM4J",0x22},//1531 - Mickey no Pocket Resort(JP).zip
{"AWAC",0x11},//1532 - Waliou Xunbao Ji(CN).zip
{"AWWE",0x22},//1533 - Woody Woodpecker in Crazy Castle 5(US).zip
{"BXME",0x00},//1534 - XS Moto(US).zip
{"BTHE",0x00},//1535 - Thunder Alley(US).zip
{"B3SJ",0x33},//1536 - Sonic Advance 3(JP).zip
{"BLXP",0x22},//1537 - Asterix & Obelix XXL(EU).zip
{"BSEX",0x22},//1538 - Shrek 2(EU).zip
{"AAHP",0x00},//1539 - Secret Agent Barbie - Royal Jewels Mission(EU).zip
{"B2DP",0x22},//1540 - Donkey Kong Country 2(EU).zip
{"B3SP",0x33},//1541 - Sonic Advance 3(EU).zip
{"BSTP",0x22},//1542 - Spyro Fusion(EU).zip
{"BD4P",0x22},//1543 - Crash Bandicoot Fusion(EU).zip
{"BMGE",0x11},//1544 - Mario Golf - Advance Tour(US).zip
{"BD3J",0x33},//1545 - Dragon Quest Characters - Torneco no Daibouken 3 Advance - Fushigi no Dungeon(JP).zip
{"BHTJ",0x22},//1546 - Harry Potter to Azkaban no Shuujin(JP).zip
{"BSAJ",0x22},//1547 - Super Chinese Advance 1 & 2(JP).zip
{"AZJE",0x22},//1548 - Dragon Ball Z - Supersonic Warriors(US).zip
{"MPBE",0x00},//1549 - Game Boy Advance Video - Pokemon - Volume 2(US).zip
{"MPAE",0x00},//1550 - Game Boy Advance Video - Pokemon - Volume 1(US).zip
{"A8LJ",0x11},//1551 - BB Booru(JP).zip
{"A5SJ",0x22},//1552 - Oshare Wanko(JP).zip
{"BSPE",0x22},//1553 - Spider-Man 2(UE).zip
{"B4WE",0x11},//1554 - Megaman Battle Network 4 - Red Sun(US).zip
{"B4BE",0x11},//1555 - Megaman Battle Network 4 - Blue Moon(US).zip
{"AY8E",0x11},//1556 - Yu-Gi-Oh! Reshef of Destruction(US).zip
{"MCTE",0x00},//1557 - Game Boy Advance Video - Cartoon Network Collection - Volume 1(US).zip
{"MKDE",0x00},//1558 - Game Boy Advance Video - Codename Kids Next Door - Volume 1(US).zip
{"B2DJ",0x22},//1559 - Super Donkey Kong 2(JP).zip
{"B8KP",0x11},//1560 - Kirby & The Amazing Mirror(EU).zip
{"MNCE",0x00},//1561 - Game Boy Advance Video - Nicktoon's Collection - Volume 1(US).zip
{"B8DE",0x00},//1562 - Around the World in 80 Days(US).zip
{"BKME",0x22},//1563 - Kim Possible 2 - Drakken's Demise(US).zip
{"MJME",0x00},//1564 - Game Boy Advance Video - The Adventures of Jimmy Neutron - Boy Genius - Volume 1(US).zip
{"BSPX",0x22},//1565 - Spider-Man 2(EU).zip
{"BMGU",0x11},//1566 - Mario Golf - Advance Tour(AU).zip
{"BSDE",0x00},//1567 - Sitting Ducks(US).zip
{"A9CE",0x00},//1568 - CT Special Forces 2 - Back in the Trenches(US).zip
{"BTAE",0x22},//1569 - Astro Boy - Omega Factor(US).zip
{"B85A",0x11},//1570 - Hamtaro - Ham-Ham Games(JU).zip
{"BKOJ",0x22},//1571 - Kaiketsu Zorori to Mahou no Yuuenchi - Ohimesama wo Sukue!(JP).zip
{"BZ2J",0x11},//1572 - Zettaizetsumei Dangerous Jiisan - Tsuu Ikari no Oshioki Blues(JP).zip
{"BDKJ",0x22},//1573 - Digi Communication 2 - Datou! Black Gemagema Dan(JP).zip
{"AE7X",0x11},//1574 - Fire Emblem(EU).zip
{"AE7Y",0x11},//1575 - Fire Emblem(EU).zip
{"BWFJ",0x11},//1576 - Wagamama Fairy Mirumo de Pon! - Yume no Kakera(JP).zip
{"B85P",0x11},//1577 - Hamtaro - Ham-Ham Games(EU).zip
{"MTME",0x00},//1578 - Game Boy Advance Video - Teenage Mutant Ninja Turtles - Volume 1(US).zip
{"A9DP",0x22},//1579 - Doom II(EU).zip
{"BKBJ",0x11},//1580 - Konjiki no Gashbell!! - Makai no Bookmark(JP).zip
{"A3NJ",0x11},//1581 - Monster Summoner(JP).zip
{"MYGE",0x00},//1582 - Game Boy Advance Video - Yu-Gi-Oh! Yugi vs. Joey - Volume 1(US).zip
{"BNRJ",0x11},//1583 - Naruto RPG - Uketsugareshi Hi no Ishi(JP).zip
{"BCWE",0x22},//1584 - Catwoman(UE).zip
{"U32J",0x22},//1585 - Zoku Bokura no Taiyou - Taiyou Shounen Django(JP).zip
{"BD5J",0x11},//1586 - Duel Masters 2 - Kirifuda Shoubu Version(JP).zip
{"BH2J",0x22},//1587 - Hagane no Renkinjutsushi - Omoide no Soumeikyoku(JP).zip
{"BK4J",0x11},//1588 - Croket! 4 - Bank no Mori no Mamorigami(JP).zip
{"BGIJ",0x33},//1589 - Get Ride! AMDriver - Senkou no Hero Tanjou!(JP).zip
{"AMAC",0x22},//1590 - Chaoji Maliou 2(CN).zip
{"ALFJ",0x22},//1591 - Dragon Ball Z - The Legacy of Goku II International(JP).zip
{"BPFJ",0x22},//1592 - Puyo Puyo Fever(JP).zip
{"BGHJ",0x22},//1593 - Gakkou no Kaidan - Hyakuyobako no Fuuin(JP).zip
{"BZOJ",0x22},//1594 - Zero One SP(JP).zip
{"B8PJ",0x11},//1595 - Power Pro Kun Pocket 1+2(JP).zip
{"BLJJ",0x33},//1596 - Rejienzu - Yomigaeru Shiren no Shima(JP).zip
{"BFFJ",0x11},//1597 - Final Fantasy I & II Advance(JP).zip
{"BD9E",0x00},//1598 - Dragon Tales - Dragon Adventures(US).zip
{"BJUX",0x22},//1599 - Tak and the Power of Juju(EU).zip
{"BDAJ",0x22},//1600 - Donchan Puzzle Hanabi de Dohn Advance(JP).zip
{"BHAJ",0x33},//1601 - Hanabi Hyakkei Advance(JP).zip
{"BG4J",0x22},//1602 - SD Gundam Force(JP).zip
{"AF5J",0x11},//1603 - Shining Force - Kuroki Ryuu no Fukkatsu(JP).zip
{"BR4J",0x33},//1604 - Rockman EXE 4.5 - Real Operation(JP).zip
{"BDCJ",0x22},//1605 - Doubutsujima no Chobi Gurumi 2 - Tamachan Monogatari(JP).zip
{"BDXJ",0x22},//1606 - B-Legends! Battle B-Daman - Moero! B-Damashii!!(JP).zip
{"BNBJ",0x22},//1607 - Himawari Doubutsu Byouin - Pet no Oishasan Ikusei Game(JP).zip
{"BUTJ",0x33},//1608 - Ultra Keibitai - Monster Attack(JP).zip
{"B3DJ",0x11},//1609 - Densetsu no Stafy 3(JP).zip
{"AZFE",0x22},//1610 - Need for Speed - Porsche Unleashed(US).zip
{"AC9E",0x00},//1611 - Cartoon Network - Block Party(US).zip
{"BGNE",0x00},//1612 - Mobile Suit Gundam Seed - Battle Assault(US).zip
{"BMFE",0x22},//1613 - Madden NFL 2005(US).zip
{"BTBE",0x00},//1614 - Thunderbirds(UE).zip
{"FM2J",0x11},//1615 - Famicom Mini Vol. 21 - Super Mario Bros. 2(JP).zip
{"FNMJ",0x11},//1616 - Famicom Mini Vol. 22 - Nazo no Murasame Jou(JP).zip
{"FMRJ",0x11},//1617 - Famicom Mini Vol. 23 - Metroid(JP).zip
{"FPTJ",0x11},//1618 - Famicom Mini Vol. 24 - Hikari Shinwa - Palthena no Kagami(JP).zip
{"FLBJ",0x11},//1619 - Famicom Mini Vol. 25 - The Legend of Zelda 2 - Link no Bouken(JP).zip
{"FFMJ",0x11},//1620 - Famicom Mini Vol. 26 - Famicom Mukashi Banashi - Shin Onigashima - Zen Kou Hen(JP).zip
{"FTKJ",0x11},//1621 - Famicom Mini Vol. 27 - Famicom Tantei Club - Kieta Koukeisha - Zen Kou Hen(JP).zip
{"FTUJ",0x11},//1622 - Famicom Mini Vol. 28 - Famicom Tantei Club Part II - Ushiro ni Tatsu Shoujo - Zen Kou Hen(JP).zip
{"FADJ",0x11},//1623 - Famicom Mini Vol. 29 - Akumajou Dracule(JP).zip
{"FSDJ",0x11},//1624 - Famicom Mini Vol. 30 - SD Gundam World - Gachapon Senshi Scramble Wars(JP).zip
{"BSNE",0x00},//1625 - SpongeBob SquarePants - The Movie(US).zip
{"BSLE",0x22},//1626 - Tom Clancy's Splinter Cell - Pandora Tomorrow(US).zip
{"BPHE",0x22},//1627 - Pitfall - The Lost Expedition(US).zip
{"AZJP",0x22},//1628 - Dragon Ball Z - Supersonic Warriors(EU).zip
{"BSKJ",0x22},//1629 - Summon Night - Craft Sword Monogatari 2(JP).zip
{"BMVJ",0x22},//1630 - Super Mario Ball(JP).zip
{"AX4P",0x31},//1631 - Super Mario Advance 4 - Super Mario Bros. 3(EU).zip
{"BCNJ",0x22},//1632 - Crash Bandicoot - Bakusou! Nitro Cart(JP).zip
{"BDGE",0x22},//1633 - Digimon Racing(US).zip
{"BRFE",0x22},//1634 - Rapala Pro Fishing(UE).zip
{"BCBE",0x22},//1635 - Crushed Baseball(US).zip
{"BPRE",0x31},//1636 - Pokemon - Fire Red Version(US).zip
{"BPGE",0x31},//1637 - Pokemon - Leaf Green Version(US).zip
{"BPOE",0x00},//1638 - Power Rangers - Dino Thunder(UE).zip
{"BZ3P",0x11},//1639 - Megaman Zero 3(EU).zip
{"ATWY",0x00},//1640 - Tetris Worlds(EU).zip
{"BZIE",0x00},//1641 - Finding Nemo - The Continuing Adventures(UE).zip
{"AY8P",0x11},//1642 - Yu-Gi-Oh! Reshef of Destruction(EU).zip
{"BOSJ",0x11},//1643 - Boboboubo Boubobo - Bakutou Hajike Taisen(JP).zip
{"AZJK",0x22},//1644 - Dragon Ball Z - Moogongtoogeuk(KS).zip
{"BAUE",0x00},//1645 - Barbie - The Princess and the Pauper(US).zip
{"BG3E",0x22},//1646 - Dragon Ball Z - Buu's Fury(US).zip
{"AZ2E",0x11},//1647 - Zoids Legacy(US).zip
{"BHMP",0x00},//1648 - Home on the Range(EU).zip
{"BPEJ",0x31},//1649 - Pocket Monsters - Emerald(JP).zip
{"MDBE",0x00},//1651 - Game Boy Advance Video - Dragon Ball GT - Volume 1(US).zip
{"BJYE",0x22},//1652 - Adventures of Jimmy Neutron, The - Boy Genius - Attack of the Twonkies(UE).zip
{"BGCE",0x22},//1653 - Advance Guardian Heroes(US).zip
{"BMGD",0x11},//1654 - Mario Golf - Advance Tour(DE).zip
{"BMGI",0x11},//1655 - Mario Golf - Advance Tour(IT).zip
{"BMGF",0x11},//1656 - Mario Golf - Advance Tour(FR).zip
{"BSPI",0x22},//1657 - Spider-Man 2(IT).zip
{"BCKP",0x22},//1658 - Star Wars Trilogy - Apprentice of the Force(EU).zip
{"BCKE",0x22},//1659 - Star Wars Trilogy - Apprentice of the Force(US).zip
{"A4BP",0x00},//1660 - Monster! Bass Fishing(EU).zip
{"BFZE",0x11},//1661 - F-Zero - GP Legend(US).zip
{"BAGJ",0x22},//1662 - Advance Guardian Heroes(JP).zip
{"BECJ",0x22},//1663 - Angel Collection 2 - Pichimo ni Narou(JP).zip
{"BDNJ",0x11},//1664 - Dan Doh!! - Tobase Shouri no Smile Shot!!(JP).zip
{"BSGJ",0x22},//1665 - Minna no Soft Series - Minna no Shogi(JP).zip
{"AHPP",0x22},//1666 - Hot Potato!(EU).zip
{"AYBE",0x22},//1667 - Backyard Basketball(US).zip
{"BACP",0x00},//1668 - Action Man - Robot Attack(EU).zip
{"B8DP",0x00},//1669 - Around the World in 80 Days(EU).zip
{"AONE",0x22},//1670 - Bubble Bobble - Old & New(US).zip
{"BF2E",0x00},//1671 - Fairly Odd Parents!, The - Shadow Showdown(US).zip
{"BSUE",0x22},//1672 - Shark Tale(UE).zip
{"BAUP",0x00},//1673 - Barbie - The Princess and the Pauper(EU).zip
{"BPCE",0x22},//1674 - Ms. Pac-Man - Maze Madness(US).zip
{"BPRS",0x31},//1675 - Pokemon - Edicion Rojo Fuego(ES).zip
{"BPGS",0x31},//1676 - Pokemon - Edicion Verde Hoja(ES).zip
{"BPGD",0x31},//1677 - Pokemon - Blattgrune Edition(DE).zip
{"BPRD",0x31},//1678 - Pokemon - Feuerrote Edition(DE).zip
{"BPRF",0x31},//1679 - Pokemon - Version Rouge Feu(FR).zip
{"BPGF",0x31},//1680 - Pokemon - Version Vert Feuille(FR).zip
{"BKHE",0x22},//1681 - Kill Switch(US).zip
{"BCCE",0x22},//1682 - Nicktoons - Freeze Frame Frenzy(US).zip
{"MPCE",0x00},//1683 - Game Boy Advance Video - Pokemon - Volume 3(US).zip
{"MPDE",0x00},//1684 - Game Boy Advance Video - Pokemon - Volume 4(US).zip
{"BPRI",0x31},//1685 - Pokemon - Versione Rosso Fuoco(IT).zip
{"BPGI",0x31},//1686 - Pokemon - Versione Verde Foglia(IT).zip
{"MN2E",0x00},//1687 - Game Boy Advance Video - Nicktoon's Collection - Volume 2(US).zip
{"TVAP",0x00},//1688 - TV Tuner PAL(CN).zip
{"BMGS",0x11},//1690 - Mario Golf - Advance Tour(ES).zip
{"BPGE",0x31},//1691 - Pokemon - Leaf Green Version(EU).zip
{"BE8J",0x11},//1692 - Fire Emblem - Seima no Kouseki(JP).zip
{"B2TE",0x22},//1693 - Tony Hawk's Underground 2(UE).zip
{"BMVE",0x22},//1694 - Mario Pinball Land(US).zip
{"BPRE",0x31},//1695 - Pokemon - Fire Red Version(UE).zip
{"BKNE",0x00},//1696 - LEGO Knights' Kingdom(US).zip
{"AYHE",0x22},//1697 - Starsky & Hutch(US).zip
{"BTCF",0x00},//1698 - Titeuf - Mega Compet'(FR).zip
{"BPLE",0x00},//1699 - Archer Maclean's - 3D Pool(US).zip
{"BF5E",0x22},//1700 - FIFA Football 2005(UE).zip
{"AWDE",0x22},//1701 - Wakeboarding Unleashed featuring Shaun Murray(US).zip
{"BZ3E",0x11},//1702 - Megaman Zero 3(US).zip
{"RZWJ",0x11},//1703 - Mawaru - Made in Wario(JP).zip
{"BT9E",0x22},//1704 - Tak 2 - The Staff of Dreams(US).zip
{"BLSE",0x22},//1705 - Lilo & Stitch 2 - Hamsterviel Havoc(US).zip
{"BSUX",0x22},//1706 - Shark Tale(EU).zip
{"BL2E",0x22},//1707 - Lizzie McGuire 2 - Lizzie Diaries(US).zip
{"BRVE",0x22},//1708 - That's So Raven(US).zip
{"BTNE",0x22},//1709 - Tron 2.0 - Killer App(US).zip
{"BTYE",0x22},//1710 - Ty the Tasmanian Tiger 2 - Bush Rescue(UE).zip
{"MC2E",0x00},//1711 - Game Boy Advance Video - Cartoon Network Collection - Volume 2(US).zip
{"B55P",0x00},//1713 - Who Wants to Be a Millionaire - 2nd Edition(EU).zip
{"BGEE",0x22},//1714 - SD Gundam Force(US).zip
{"BWWE",0x22},//1715 - WWE - Survivor Series(UE).zip
{"B8KE",0x11},//1716 - Kirby & The Amazing Mirror(US).zip
{"BT2E",0x22},//1717 - Teenage Mutant Ninja Turtles 2 - Battle Nexus(US).zip
{"BFTJ",0x31},//1718 - F-Zero - Climax(JP).zip
{"U32E",0x22},//1719 - Boktai 2 - Solar Boy Django(US).zip
{"BALE",0x22},//1720 - All Grown Up! - Express Yourself(UE).zip
{"BKNP",0x00},//1721 - LEGO Knights' Kingdom(EU).zip
{"BHZP",0x00},//1722 - 2 Games in 1 - Hot Wheels - Velocity X + Hot Wheels - World Race(EU).zip
{"BGGE",0x00},//1723 - Golden Nugget Casino(US).zip
{"BSYJ",0x11},//1724 - Sentouin Yamada Hajime(JP).zip
{"BMCE",0x00},//1725 - Monster Trucks(US).zip
{"BF8E",0x00},//1726 - Super Hornet - FA 18F(US).zip
{"A84P",0x11},//1727 - Hamtaro - Rainbow Rescue(EU).zip
{"BGTE",0x22},//1728 - Grand Theft Auto Advance(US).zip
{"BYDE",0x11},//1729 - Yu-Gi-Oh! Destiny Board Traveler(US).zip
{"BFDJ",0x22},//1730 - Fruit Mura no Doubutsutachi(JP).zip
{"BOPJ",0x22},//1731 - Twin Series 2 - Oshare Princess 4 + Sweet Life(JP).zip
{"BAZJ",0x22},//1732 - Aka-chan Doubutsuen(JP).zip
{"BIKJ",0x22},//1733 - Ochaken Kururin - Honwaka Puzzle de Hotto Shiyo(JP).zip
{"BSIE",0x22},//1734 - Shrek 2 - Beg for Mercy(UE).zip
{"AA9P",0x22},//1735 - Duel Masters - Sempai Legends(EU).zip
{"BE2J",0x22},//1736 - Majokko Cream-chan no Gokko Series 2 - Kisekae Angel(JP).zip
{"BWKJ",0x22},//1737 - Wanko de Kururin! Wancle(JP).zip
{"BWAJ",0x22},//1738 - Majokko Cream-chan no Gokko Series 1 - Wannyan Idol Gakuen(JP).zip
{"BPQJ",0x22},//1739 - PukuPuku Tennen Kairanban - Koi no Cupid Daisakusen(JP).zip
{"BRNJ",0x11},//1740 - Licca-chan no Oshare Nikki(JP).zip
{"BI2J",0x22},//1741 - Koinu to Issho 2(JP).zip
{"BGTP",0x22},//1742 - Grand Theft Auto Advance(EU).zip
{"BZMJ",0x22},//1743 - Zelda no Densetsu - Fushigi no Boushi(JP).zip
{"BXAE",0x00},//1744 - Texas Hold'em Poker(US).zip
{"FLBE",0x11},//1745 - Classic NES Series - Zelda II - The Adventure of Link(UE).zip
{"FADE",0x11},//1746 - Classic NES Series - Castlevania(UE).zip
{"FDME",0x11},//1747 - Classic NES Series - Dr. Mario(UE).zip
{"FMRE",0x11},//1748 - Classic NES Series - Metroid(UE).zip
{"BICE",0x00},//1749 - Incredibles, The(UE).zip
{"BELE",0x00},//1750 - Elf - The Movie(US).zip
{"B3AE",0x22},//1751 - Lord of the Rings, The - The Third Age(UE).zip
{"BSIX",0x22},//1752 - Shrek 2 - Beg for Mercy(EU).zip
{"BD6E",0x22},//1753 - Duel Masters - Kaijudo Showdown(US).zip
{"BPXE",0x22},//1754 - Polar Express, The(UE).zip
{"BSME",0x22},//1755 - Metal Slug Advance(US).zip
{"BSVE",0x00},//1756 - Smashing Drive(US).zip
{"BOCE",0x33},//1757 - Urbz, The - Sims in the City(UE).zip
{"BZMP",0x22},//1758 - Legend of Zelda, The - The Minish Cap(EU).zip
{"BF2D",0x00},//1759 - Cosmo & Wanda - Wenn Elfen Helfen! - Das Schatten-Duell(DE).zip
{"BPVP",0x22},//1760 - Pferd & Pony - Mein Pferdehof(EU).zip
{"BZIY",0x00},//1761 - Finding Nemo - The Continuing Adventures(EU).zip
{"BSOE",0x11},//1762 - Shaman King - Master of Spirits(US).zip
{"BNDE",0x00},//1763 - Codename Kids Next Door - Operation S.O.D.A.(US).zip
{"B8CJ",0x11},//1764 - Kingdom Hearts - Chain of Memories(JP).zip
{"AXVS",0x31},//1765 - Pokemon - Edicion Rubi(ES).zip
{"AXPS",0x31},//1766 - Pokemon - Edicion Zafiro(ES).zip
{"BDMJ",0x22},//1767 - Super Real Mahjong Dousoukai(JP).zip
{"BDLJ",0x11},//1768 - Shin Megami Tensei Devil Children - Messiah Riser(JP).zip
{"BLYE",0x22},//1769 - Lemony Snicket's - A Series of Unfortunate Events(UE).zip
{"BCCX",0x22},//1770 - SpongeBob SquarePants and Friends - Freeze Frame Frenzy(EU).zip
{"BICX",0x00},//1771 - Incredibles, The(EU).zip
{"BHEE",0x00},//1772 - Hot Wheels - Stunt Track Challenge(UE).zip
{"BG5E",0x22},//1773 - Cabela's Big Game Hunter - 2005 Adventures(UE).zip
{"BP9E",0x00},//1774 - World Championship Poker(US).zip
{"BNFE",0x22},//1775 - Need for Speed - Underground 2(UE).zip
{"B2DE",0x22},//1776 - Donkey Kong Country 2(US).zip
{"BM5P",0x33},//1777 - Mario vs. Donkey Kong(EU).zip
{"BKHP",0x22},//1778 - Kill Switch(EU).zip
{"ADHP",0x22},//1779 - Defender of the Crown(EU).zip
{"BT2P",0x22},//1780 - Teenage Mutant Ninja Turtles 2 - Battle Nexus(EU).zip
{"BB2E",0x22},//1781 - Beyblade G-Revolution(US).zip
{"BDVJ",0x22},//1782 - Dragon Ball - Advanced Adventure(JP).zip
{"BRGE",0x22},//1783 - Yu Yu Hakusho - Tournament Tactics(UE).zip
{"BSMJ",0x22},//1784 - Metal Slug Advance(JP).zip
{"ACTY",0x22},//1785 - Creatures(EU).zip
{"BFUE",0x22},//1786 - Fear Factor - Unleashed(US).zip
{"BARP",0x22},//1787 - 2 Games in 1 - GT Advance 3 + Moto GP(EU).zip
{"BFJJ",0x22},//1788 - Frogger - Kodaibunmei no Nazo(JP).zip
{"BPAE",0x22},//1789 - Pac-Man World(US).zip
{"BDZD",0x00},//1790 - 2 Games in 1 - Monster AG, Die + Findet Nemo(DE).zip
{"A9GE",0x00},//1791 - Stadium Games(US).zip
{"AL9E",0x00},//1792 - Tomb Raider - The Prophecy(US).zip
{"BICD",0x00},//1793 - Unglaublichen, Die(DE).zip
{"BASJ",0x11},//1794 - Gakuen Alice - Dokidoki Fushigi Taiken(JP).zip
{"BZIJ",0x22},//1795 - Finding Nemo - Arata na Bouken(JP).zip
{"BTNP",0x22},//1796 - Tron 2.0 - Killer App(EU).zip
{"BLSP",0x22},//1797 - Lilo & Stitch 2(EU).zip
{"BB2P",0x22},//1798 - Beyblade G-Revolution(EU).zip
{"BLYX",0x22},//1799 - Lemony Snicket's - A Series of Unfortunate Events(EU).zip
{"BMVP",0x22},//1800 - Super Mario Ball(EU).zip
{"B42J",0x22},//1801 - Kidou Senshi Gundam Seed Destiny(JP).zip
{"BGWJ",0x31},//1802 - Game Boy Wars Advance 1+2(JP).zip
{"BREJ",0x11},//1803 - Yakusoku no Chi Riviera(JP).zip
{"BP4P",0x33},//1804 - Premier Manager 2004-2005(EU).zip
{"BFFE",0x11},//1805 - Final Fantasy I & II - Dawn of Souls(US).zip
{"BICI",0x00},//1806 - Incredibili, Gli(IT).zip
{"BPKP",0x22},//1807 - Payback(EU).zip
{"BPCP",0x22},//1808 - Ms. Pac-Man - Maze Madness(EU).zip
{"BJCJ",0x22},//1809 - Moero!! Jaleco Collection(JP).zip
{"BFFP",0x11},//1810 - Final Fantasy I & II - Dawn of Souls(EU).zip
{"BP7J",0x11},//1811 - Power Pro Kun Pocket 7(JP).zip
{"BOCJ",0x33},//1812 - Urbz, The - Sims in the City(JP).zip
{"BSFJ",0x22},//1813 - Sylvania Family - Fashion Designer Ninaritai!(JP).zip
{"B2SJ",0x22},//1814 - Cinnamon - Yume no Daibouken(JP).zip
{"BLPD",0x22},//1815 - 2 Games in 1 - Disneys Konig der Lowen + Disneys Prinzessinnen(DE).zip
{"BSNX",0x00},//1816 - SpongeBob SquarePants - The Movie(EU).zip
{"BDEE",0x00},//1817 - Dead to Rights(US).zip
{"BDOE",0x00},//1818 - Dora the Explorer - Super Star Adventures!(US).zip
{"BUME",0x00},//1819 - Monopoly(US).zip
{"BPIE",0x22},//1820 - It's Mr. Pants(UE).zip
{"KYGJ",0x22},//1821 - Yoshi no Banyu Inryoku(JP).zip
{"BDEP",0x00},//1822 - Dead to Rights(EU).zip
{"BPAP",0x22},//1823 - Pac-Man World(EU).zip
{"BDFP",0x00},//1824 - 2 Games in 1 - SpongeBob SquarePants - SuperSponge + Revenge of the Flying Dutchman(EU).zip
{"BSZX",0x00},//1825 - 2 Games in 1 - SpongeBob SquarePants - SuperSponge + SpongeBob SquarePants - Battle for Bikini Bottom(EU).zip
{"BR3P",0x00},//1826 - R-Type III - The Third Lightning(EU).zip
{"BLPF",0x22},//1827 - 2 Games in 1 - Roi Lion, Le + Disney Princesse(FR).zip
{"B8CE",0x11},//1828 - Kingdom Hearts - Chain of Memories(US).zip
{"BFPJ",0x22},//1829 - Futari ha Pretty Cure - Arienaai! Yume no Sono ha Daimeikyuu(JP).zip
{"BK5J",0x11},//1830 - Korokke Great - Toki no Boukensha(JP).zip
{"B3PJ",0x22},//1831 - PukuPuku Tennen Kairanban - Youkoso Illusion Land he(JP).zip
{"BRBJ",0x11},//1832 - Rockman EXE 5 - Team of Blues(JP).zip
{"BITJ",0x22},//1833 - Onmyou Taisenki Zeroshiki(JP).zip
{"BS8J",0x22},//1834 - Spyro Advance - Wakuwaku Tomodachi Daisakusen!(JP).zip
{"BKJJ",0x11},//1835 - Keroro Gunsou - Taiketsu! Keroro Cart de Arimasu!!(JP).zip
{"BHDJ",0x22},//1836 - Hello! - Idol Debut(JP).zip
{"BKDJ",0x22},//1837 - Crash Bandicoot Advance - Wakuwaku Tomodachi Daisakusen!(JP).zip
{"BBSJ",0x22},//1838 - Boukyaku no Senritsu - The Melody of Oblivion(JP).zip
{"BRZD",0x00},//1839 - 2 Games in 1 - Power Rangers - Ninja Storm + Power Rangers - Time Force(DE).zip
{"BSMP",0x22},//1840 - Metal Slug Advance(EU).zip
{"BUMP",0x00},//1841 - Monopoly(EU).zip
{"BDUJ",0x11},//1842 - Duel Masters 3(JP).zip
{"BLIJ",0x22},//1843 - Little Patissier - Cake no Oshiro(JP).zip
{"BKVJ",0x22},//1844 - Shingata Medarot - Kabuto Version(JP).zip
{"BKUJ",0x22},//1845 - Shingata Medarot - Kuwagata Version(JP).zip
{"BWPJ",0x11},//1846 - Wagamama Fairy Mirumo de Pon! - Nazo no Kagi to Shinjitsu no Tobir(JP).zip
{"BJ3J",0x11},//1847 - Zettaizetsumei Dangerous Jiisan 3 - Hateshinaki Mamonogatari(JP).zip
{"BGJJ",0x33},//1848 - Genseishin Justirisers - Souchaku! Chikyuu no Senshitachi(JP).zip
{"BGYJ",0x22},//1849 - Konjiki no Gashbell!! - Unare! Yuujou no Zakeru 2(JP).zip
{"B3AJ",0x22},//1850 - Lord of the Rings, The - Uchitsu Kuni Daisanki(JP).zip
{"AVRE",0x22},//1851 - V-Rally 3(US).zip
{"BGOP",0x00},//1852 - Garfield - The Search for Pooky(EU).zip
{"BRAP",0x22},//1853 - Racing Gears Advance(EU).zip
{"AEGE",0x00},//1854 - Extreme Ghostbusters - Code Ecto-1(US).zip
{"B3TJ",0x22},//1855 - Tales of the World - Narikiri Dungeon 3(JP).zip
{"AJDP",0x00},//1856 - James Pond - Codename Robocod(EU).zip
{"BYIJ",0x11},//1857 - Yu-Gi-Oh! Duel Monsters - International Worldwide Edition 2(JP).zip
{"B2KJ",0x22},//1858 - Kiss x Kiss - Seirei Gakuen(JP).zip
{"A9BE",0x22},//1859 - Medabots - Rokusho Version(US).zip
{"ATEE",0x22},//1860 - WTA Tour Tennis(US).zip
{"AFGE",0x00},//1861 - American Tail, An - Fievel's Gold Rush(US).zip
{"ALAE",0x00},//1862 - Land Before Time, The(US).zip
{"B35E",0x00},//1863 - Strawberry Shortcake - Summertime Adventure(US).zip
{"BAJE",0x22},//1864 - Banjo Pilot(US).zip
{"BZME",0x22},//1865 - Legend of Zelda, The - The Minish Cap(US).zip
{"B8MJ",0x22},//1866 - Mario Party Advance(JP).zip
{"BLBX",0x22},//1867 - 2 Games in 1 - Brother Bear + Lion King, The(EU).zip
{"BRXJ",0x11},//1868 - Vattroller X(JP).zip
{"AH9J",0x22},//1869 - Hobbit no Bouken - Lord of the Rings - Hajimari no Monogatari(JP).zip
{"BOVJ",0x22},//1870 - Bouken-Ou Beet - Busters Road(JP).zip
{"BLYD",0x22},//1871 - Lemony Snicket - Ratselhafte Ereignisse(DE).zip
{"BADE",0x22},//1872 - Aladdin(US).zip
{"BT3J",0x22},//1873 - Tantei Jinguuji Saburou - Shiroi Kage no Syoujyo(JP).zip
{"B3QJ",0x33},//1874 - Sangokushi - Koumeiden(JP).zip
{"B3EJ",0x33},//1875 - Sangokushi - Eiketsuden(JP).zip
{"BG8J",0x22},//1876 - Ganbare Dodge Fighters(JP).zip
{"AYPE",0x00},//1877 - Sega Arcade Gallery(US).zip
{"A5UP",0x22},//1878 - Space Channel 5 - Ulala's Cosmic Attack(EU).zip
{"B2RJ",0x33},//1879 - Super Robot Taisen - Original Generation 2(JP).zip
{"BBKP",0x11},//1880 - Donkey Kong - King of Swing(EU).zip
{"AGGE",0x00},//1881 - Gremlins - Stripe vs. Gizmo(US).zip
{"AR9P",0x00},//1882 - Reign of Fire(EU).zip
{"AROP",0x22},//1883 - Rocky(EU).zip
{"B4WP",0x11},//1884 - Megaman Battle Network 4 - Red Sun(EU).zip
{"BHLE",0x11},//1885 - Shaman King - Legacy of the Spirits - Soaring Hawk(US).zip
{"BWSE",0x11},//1886 - Shaman King - Legacy of the Spirits - Sprinting Wolf(US).zip
{"BWHE",0x00},//1887 - Winnie the Pooh's - Rumbly Tumbly Adventure(US).zip
{"A3KE",0x00},//1888 - International Karate Plus(US).zip
{"AFHE",0x00},//1889 - Strike Force Hydra(US).zip
{"AZNE",0x00},//1890 - Archer Maclean's - Super Dropzone(US).zip
{"AWCE",0x00},//1891 - World Tennis Stars(US).zip
{"A8BE",0x22},//1892 - Medabots - Metabee Version(US).zip
{"AOTE",0x00},//1893 - Polly Pocket! - Super Splash Island(US).zip
{"B4BP",0x11},//1894 - Megaman Battle Network 4 - Blue Moon(EU).zip
{"AXIE",0x00},//1895 - X-Bladez - Inline Skater(US).zip
{"ARUE",0x22},//1896 - Robot Wars - Advanced Destruction(US).zip
{"BGCP",0x22},//1897 - Advance Guardian Heroes(EU).zip
{"BY7E",0x11},//1898 - Yu-Gi-Oh! 7 Trials to Glory - World Championship Tournament 2005(US).zip
{"BAJP",0x22},//1899 - Banjo Pilot(EU).zip
{"BTAP",0x22},//1900 - Astro Boy - Omega Factor(EU).zip
{"BPOX",0x00},//1901 - Power Rangers - Dino Thunder(EU).zip
{"BLVJ",0x33},//1902 - Legendz - Sign of Nekuromu(JP).zip
{"BCTE",0x00},//1903 - The Cat in the Hat by Dr. Seuss(US).zip
{"BKZS",0x22},//1904 - Banjo-Kazooie - La Venganza de Grunty(ES).zip
{"BZFJ",0x11},//1905 - Zoids Saga - Fuzors(JP).zip
{"BICS",0x00},//1906 - Increibles, Los(ES).zip
{"BZIX",0x00},//1907 - Finding Nemo - The Continuing Adventures(EU).zip
{"A3QE",0x00},//1908 - Sound of Thunder, A(US).zip
{"BAEE",0x00},//1909 - Ace Combat Advance(UE).zip
{"AN6E",0x22},//1910 - Klonoa 2 - Dream Champ Tournament(US).zip
{"BG6E",0x00},//1911 - Super Army War(US).zip
{"BRTE",0x22},//1912 - Robots(US).zip
{"ZMAJ",0x11},//1913 - Playan AV Player(JP).zip
{"BRKJ",0x11},//1914 - Rockman EXE 5 - Team of Colonel(JP).zip
{"AKQE",0x00},//1915 - Kong - The Animated Series(US).zip
{"BDSP",0x22},//1916 - Digimon - Battle Spirit 2(EU).zip
{"AXVI",0x31},//1917 - Pokemon - Versione Rubino(IT).zip
{"AXPI",0x31},//1918 - Pokemon - Versione Zaffiro(IT).zip
{"AIPP",0x22},//1919 - Silent Scope(EU).zip
{"BRAE",0x22},//1920 - Racing Gears Advance(US).zip
{"BSUI",0x22},//1921 - Shark Story(IT).zip
{"AAOP",0x22},//1922 - Aero the Acro-Bat - Rascal Rival Revenge(EU).zip
{"BYYP",0x22},//1923 - Yu Yu Hakusho - Spirit Detective(EU).zip
{"AWGP",0x22},//1924 - Salt Lake 2002(EU).zip
{"BWHP",0x00},//1925 - Winnie the Pooh's - Rumbly Tumbly Adventure(EU).zip
{"BRTP",0x22},//1926 - Robots(EU).zip
{"BBHE",0x00},//1927 - Blades of Thunder(US).zip
{"A2YE",0x33},//1928 - Top Gun - Combat Zones(US).zip
{"BRME",0x11},//1929 - Rave Master - Special Attack Force!(US).zip
{"BMKJ",0x33},//1930 - Mezase Koushien(JP).zip
{"AAWP",0x00},//1931 - Contra Advance - The Alien Wars EX(EU).zip
{"BG6P",0x00},//1932 - Glory Days - The Essence of War(EU).zip
{"BKAJ",0x31},//1933 - Sennen Kazoku(JP).zip
{"BRYP",0x22},//1934 - Rayman - Hoodlums' Revenge(EU).zip
{"BDFE",0x00},//1935 - 2 Games in 1 - SpongeBob SquarePants - SuperSponge + Revenge of the Flying Dutchman(US).zip
{"BCVE",0x22},//1936 - 2 Games in 1 - Scooby-Doo! - Mystery Mayhem + Scooby-Doo and the Cyber Chase(US).zip
{"BK8J",0x11},//1937 - Kappa no Kai-Kata - Katan Daibouken(JP).zip
{"BMGP",0x11},//1938 - Mario Golf - Advance Tour(EU).zip
{"AQDS",0x00},//1939 - Disney Princesas(ES).zip
{"BLDS",0x22},//1940 - 2 Games in 1 - Disney Princesas + Lizzie McGuire(ES).zip
{"BD6P",0x22},//1941 - Duel Masters - Kaijudo Showdown(EU).zip
{"BDZS",0x00},//1942 - 2 Games in 1 - Monstruos, S.A. + Buscando a Nemo(ES).zip
{"BCYE",0x22},//1943 - Backyard Baseball 2006(US).zip
{"BRYE",0x22},//1944 - Rayman - Hoodlum's Revenge(US).zip
{"BILP",0x22},//1945 - Bionicle - Maze of Shadows(EU).zip
{"BT9X",0x22},//1946 - Tak 2 - The Staff of Dreams(EU).zip
{"BEEP",0x22},//1947 - Maya the Bee - Sweet Gold(EU).zip
{"BPFP",0x22},//1948 - Puyo Pop Fever(EU).zip
{"ARGS",0x00},//1949 - Rugrats - Travesuras en el Castillo(ES).zip
{"B36J",0x11},//1950 - Shin Sangoku Musou Advance(JP).zip
{"BNYJ",0x22},//1951 - Nyan Nyan Nyanko no Nyan Collection(JP).zip
{"B8ME",0x22},//1952 - Mario Party Advance(US).zip
{"BLWE",0x22},//1953 - LEGO Star Wars - The Video Game(UE).zip
{"B9TJ",0x22},//1954 - Shark Tale(JP).zip
{"BLDP",0x22},//1955 - 2 Games in 1 - Disney Princess + Lizzie McGuire(EU).zip
{"BDZI",0x00},//1956 - 2 Games in 1 - Monsters & Co. + Alla Ricerca di Nemo(IT).zip
{"B3LP",0x00},//1957 - Killer 3D Pool(EU).zip
{"BR2E",0x11},//1958 - Mr. Driller 2(US).zip
{"BYOP",0x11},//1959 - Yu-Gi-Oh! Day of the Duelist - World Championship Tournament 2005(EU).zip
{"BO8K",0x22},//1960 - One Piece - Going Baseball - Haejeok Yaku(KS).zip
{"BDVK",0x22},//1961 - Dragon Ball - Advanced Adventure(KS).zip
{"AHJE",0x22},//1962 - Hugo - The Evil Mirror Advance(US).zip
{"BTZE",0x22},//1963 - Tokyo Xtreme Racer Advance(US).zip
{"MS3E",0x00},//1964 - Game Boy Advance Video - SpongeBob SquarePants - Volume 3(US).zip
{"MCNE",0x00},//1965 - Game Boy Advance Video - Cartoon Network Collection - Platinum Edition(US).zip
{"MDCE",0x00},//1966 - Game Boy Advance Video - Disney Channel Collection - Volume 1(US).zip
{"KYGP",0x22},//1967 - Yoshi's Universal Gravitation(EU).zip
{"MSRE",0x00},//1968 - Game Boy Advance Video - Super Robot Monkey Team Hyper Force Go! - Volume 1(US).zip
{"BSVP",0x00},//1969 - Smashing Drive(EU).zip
{"APOE",0x00},//1970 - Popeye - Rush for Spinach(UE).zip
{"B4ZJ",0x22},//1971 - Rockman Zero 4(JP).zip
{"BBRP",0x22},//1972 - Brother Bear(EU).zip
{"BG2J",0x22},//1973 - Kessakusen! Ganbare Goemon 1+2(JP).zip
{"BQAJ",0x22},//1974 - Meitantei Conan - Atasuki no Monument(JP).zip
{"BP8E",0x00},//1975 - Pac-Man Pinball Advance(US).zip
{"BCUJ",0x22},//1976 - Ochaken no Yumebouken(JP).zip
{"BIPJ",0x22},//1977 - One Piece - Dragon Dream(JP).zip
{"BTRJ",0x33},//1978 - Welcome to the Tower SP(JP).zip
{"AXPF",0x31},//1979 - Pokemon - Version Saphir(FR).zip
{"AXVF",0x31},//1980 - Pokemon - Version Rubis(FR).zip
{"AWXP",0x22},//1981 - ESPN Winter X-Games Snowboarding 2(EU).zip
{"BTLJ",0x22},//1982 - Minna no Soft Series - Happy Trump 20(JP).zip
{"BICJ",0x00},//1983 - Mr. Incredible(JP).zip
{"AXVD",0x31},//1984 - Pokemon - Rubin-Edition(DE).zip
{"AXPD",0x31},//1985 - Pokemon - Saphir-Edition(DE).zip
{"BPEE",0x31},//1986 - Pokemon - Emerald Version(UE).zip
{"B3LE",0x00},//1987 - Killer 3D Pool(US).zip
{"MFPE",0x00},//1988 - Game Boy Advance Video - The Proud Family - Volume 1(US).zip
{"B8CP",0x11},//1989 - Kingdom Hearts - Chain of Memories(EU).zip
{"BR9J",0x22},//1990 - Relaxuma na Mainichi(JP).zip
{"BQBJ",0x22},//1991 - Konchu Monster - Battle Master(JP).zip
{"BQSJ",0x22},//1992 - Konchu Monster - Battle Stadium(JP).zip
{"BE3P",0x22},//1993 - Star Wars Episode III - Revenge of the Sith(EU).zip
{"BLAE",0x00},//1994 - Scrabble Blast!(US).zip
{"MCME",0x00},//1995 - Game Boy Advance Video - Cartoon Network Collection - Limited Edition(US).zip
{"A55U",0x00},//1996 - Who Wants to Be a Millionaire(AU).zip
{"BE8E",0x11},//1997 - Fire Emblem - The Sacred Stones(US).zip
{"BE3E",0x22},//1998 - Star Wars Episode III - Revenge of the Sith(US).zip
{"BDZP",0x00},//1999 - 2 Games in 1 - Monsters, Inc. + Finding Nemo(EU).zip
{"RZWE",0x11},//2000 - WarioWare - Twisted!(US).zip
{"BKPJ",0x22},//2001 - Kawaii Pet Game Gallery 2(JP).zip
{"B82J",0x22},//2002 - Kawaii Koinu Wonderful(JP).zip
{"BWXJ",0x22},//2003 - Wanko Mix - Chiwanko World(JP).zip
{"BGZE",0x22},//2004 - Madagascar(US).zip
{"A9TJ",0x22},//2005 - Metal Max 2 Kai(JP).zip
{"AXVE",0x31},//2006 - Pokemon - Ruby Version(EU).zip
{"BL5P",0x00},//2007 - 2 Games in 1 - LEGO Bionicle + LEGO Knights' Kingdom(EU).zip
{"BBKJ",0x11},//2008 - Bura Bura Donkey(JP).zip
{"BWBD",0x22},//2009 - 2 Games in 1 - Disneys Prinzessinnen + Baren Bruder(DE).zip
{"BCVP",0x22},//2010 - 2 Games in 1 - Scooby-Doo! - Mystery Mayhem + Scooby-Doo and the Cyber Chase(EU).zip
{"AXPE",0x31},//2011 - Pokemon - Sapphire Version(UE).zip
{"BWBS",0x22},//2012 - 2 Games in 1 - Disney Princesas + Hermano Oso(ES).zip
{"U32P",0x22},//2013 - Boktai 2 - Solar Boy Django(EU).zip
{"B8MP",0x22},//2014 - Mario Party Advance(EU).zip
{"BRKP",0x11},//2015 - Megaman Battle Network 5 - Team Colonel(EU).zip
{"BRBP",0x11},//2016 - Megaman Battle Network 5 - Team Protoman(EU).zip
{"AMWP",0x22},//2017 - Muppet Pinball Mayhem(EU).zip
{"AOIP",0x22},//2018 - Shrek - Reekin' Havoc(EU).zip
{"BLYI",0x22},//2019 - Lemony Snicket - Una Serie di Sfortunati Eventi(IT).zip
{"AWQP",0x22},//2020 - Wings(EU).zip
{"BBGE",0x22},//2021 - Batman Begins(UE).zip
{"KYGE",0x22},//2022 - Yoshi Topsy-Turvy(US).zip
{"BDVP",0x22},//2023 - Dragon Ball - Advanced Adventure(EU).zip
{"BNGJ",0x11},//2024 - Mahou Sensei Negima! - Private Lesson Damedesuu Toshokanjima(JP).zip
{"AFRP",0x22},//2025 - Frogger's Adventures - Temple of the Frog(EU).zip
{"B4RJ",0x22},//2026 - Shikakui Atama wo Marukusuru Advance - Kokugo Sansu Rika Shakai(JP).zip
{"BPDJ",0x11},//2027 - Bouken Yuuki Pluster World - Densetsu no Plust Gate EX(JP).zip
{"B4KJ",0x22},//2028 - Shikakui Atama wo Marukusuru Advance - Kanji Keisan(JP).zip
{"APRF",0x00},//2029 - Power Rangers - La Force du Temps(FR).zip
{"B2AP",0x22},//2030 - 2 Games in 1 - Asterix & Obelix - Bash Them All! + Asterix & Obelix XXL(EU).zip
{"B8FE",0x22},//2031 - Herbie - Fully Loaded(US).zip
{"BRKE",0x11},//2032 - Megaman Battle Network 5 - Team Colonel(US).zip
{"BK6J",0x11},//2033 - Konchu Ouja - Mushiking(JP).zip
{"BKRJ",0x22},//2034 - No no no Puzzle Chailien(JP).zip
{"BRBE",0x11},//2035 - Megaman Battle Network 5 - Team Protoman(US).zip
{"BMQE",0x22},//2036 - Magical Quest 3 Starring Mickey & Donald(US).zip
{"BGZX",0x22},//2037 - Madagascar(EU).zip
{"BF4E",0x22},//2038 - Fantastic 4(US).zip
{"BREE",0x11},//2039 - Riviera - The Promised Land(US).zip
{"BFCJ",0x22},//2040 - Fantastic Children(JP).zip
{"BCSP",0x22},//2041 - 2 Games in 1 - V-Rally 3 + Stuntman(EU).zip
{"BLPS",0x22},//2042 - 2 Games in 1 - Disney Princesas + Rey Leon, El(ES).zip
{"AZWC",0x11},//2043 - Waliou Zhizao(CN).zip
{"BMXC",0x11},//2044 - Miteluote Lingdian Renwu(CN).zip
{"BM2J",0x22},//2045 - Momotarou Densetsu G - Gold Deck wo Tsukure!(JP).zip
{"BWCE",0x00},//2046 - 2 Games in 1 - Golden Nugget Casino + Texas Hold'em Poker(US).zip
{"BEJJ",0x22},//2047 - Erementar Gerad(JP).zip
{"BM9J",0x11},//2048 - Marheaven - Knockin on Heaven's Door(JP).zip
{"BO5J",0x22},//2049 - Oshare Princess 5(JP).zip
{"BF4X",0x22},//2050 - Fantastic 4(EU).zip
{"B8FP",0x22},//2051 - Herbie - Fully Loaded(EU).zip
{"BLWJ",0x22},//2052 - LEGO Star Wars - The Video Game(JP).zip
{"B5AP",0x22},//2053 - 2 Games in 1 - Spyro - Season of Ice + Crash Bandicoot 2 - N-Tranced(EU).zip
{"B52P",0x22},//2054 - 2 Games in 1 - Spyro 2 - Season of Flame + Crash Nitro Kart(EU).zip
{"B36E",0x11},//2055 - Dynasty Warriors Advance(US).zip
{"BBKE",0x11},//2056 - Donkey Kong - King of Swing(US).zip
{"BRSP",0x00},//2057 - 2 Games in 1 - Rugrats - Go Wild + SpongeBob SquarePants - SuperSponge(EU).zip
{"BABJ",0x22},//2058 - Aleck Bordon Adventure - Tower & Shaft Advance(JP).zip
{"BCFE",0x22},//2059 - Charlie and the Chocolate Factory(US).zip
{"BLEJ",0x33},//2060 - Bleach Advance(JP).zip
{"BKMJ",0x22},//2061 - Kim Possible(JP).zip
{"BLSJ",0x22},//2062 - Lilo & Stitch(JP).zip
{"BH6J",0x22},//2063 - Haro no Puyo Puyo(JP).zip
{"BCFP",0x22},//2064 - Charlie and the Chocolate Factory(EU).zip
{"APDP",0x22},//2065 - Pinball of the Dead, The(EU).zip
{"BFGE",0x11},//2066 - Harvest Moon - More Friends of Mineral Town(US).zip
{"BFMJ",0x22},//2067 - Futari ha Pretty Cure Max Heart - Maji Maji! Fight de IN Janai(JP).zip
{"BKEJ",0x22},//2068 - Konjiki no Gashbell!! - The Card Battle for GBA(JP).zip
{"U33J",0x22},//2069 - Shin Bokura no Taiyou - Gyakushuu no Sabata(JP).zip
{"BQPE",0x22},//2070 - Kim Possible III - Team Possible(US).zip
{"BHFJ",0x22},//2071 - Twin Series 4 - Hamu Hamu Monster EX + F Puzzle Hamusuta(JP).zip
{"BMWJ",0x22},//2072 - Twin Series 5 - Wanwan Meitantei EX + Manou no Kuni no Cake House(JP).zip
{"BWNJ",0x22},//2073 - Twin Series 6 - Wannyan Idol Gakuen + Koinu to Issho(JP).zip
{"BZSE",0x22},//2074 - That's So Raven 2 - Supernatural Style(US).zip
{"B2ME",0x11},//2075 - Shaman King - Master of Spirits 2(US).zip
{"BMZP",0x22},//2076 - Zooo(EU).zip
{"BRZF",0x00},//2077 - 2 Games in 1 - Power Rangers - Ninja Storm + Power Rangers - Time Force(FR).zip
{"B6ME",0x22},//2078 - Madden NFL 06(US).zip
{"BT4E",0x22},//2079 - Dragon Ball GT - Transformation(US).zip
{"BKTP",0x22},//2080 - Steel Empire(EU).zip
{"BGVE",0x22},//2081 - Gumby vs. the Astrobots(US).zip
{"BBJP",0x00},//2082 - 2 Games in 1 - SpongeBob SquarePants - Battle for Bikini Bottom + Jimmy Neutron - Boy Genius(EU).zip
{"BS7P",0x22},//2083 - 2 Games in 1 - Shrek 2 + Shark Tale(EU).zip
{"BGZP",0x22},//2084 - Madagascar(EU).zip
{"ANIP",0x00},//2085 - Animaniacs - Lights, Camera, Action!(EU).zip
{"BP5P",0x33},//2086 - Premier Manager 2005-2006(EU).zip
{"BQMJ",0x22},//2087 - Twin Series 3 - Insect Monster + Suchai Labyrinth(JP).zip
{"B2PJ",0x22},//2088 - Twin Series 7 - Twin Puzzle - Kisekae Wanko Ex + Puzzle Rainbow Magic 2(JP).zip
{"AHNP",0x22},//2089 - Spy Hunter(EU).zip
{"B3GE",0x22},//2090 - Sigma Star Saga(UE).zip
{"BL3E",0x22},//2091 - Lizzie McGuire 3 - Homecoming Havoc(US).zip
{"B2OJ",0x22},//2092 - Pro Mahjong Tsuwamono Advance(JP).zip
{"B2CP",0x00},//2093 - Pac-Man World 2(EU).zip
{"AXDP",0x22},//2094 - Mortal Kombat - Deadly Alliance(EU).zip
{"B66E",0x00},//2095 - Risk - Battleship - Clue(US).zip
{"B6EE",0x00},//2096 - Board Game Classics(US).zip
{"AE3P",0x22},//2097 - Ed, Edd n Eddy - Jawbreakers!(EU).zip
{"ADXP",0x00},//2098 - Dexter's Laboratory - Chess Challenge(EU).zip
{"BX3P",0x23},//2099 - 2 Games in 1 - Spider-Man + Spider-Man 2(EU).zip
{"BX4E",0x22},//2100 - 2 Games in 1 - Tony Hawk's Underground + Kelly Slater's Pro Surfer(UE).zip
{"BP8P",0x00},//2101 - Pac-Man Pinball Advance(EU).zip
{"A2TP",0x00},//2102 - Pinball Tycoon(EU).zip
{"B9AJ",0x00},//2103 - Kunio Kun Nekketsu Collection 1(JP).zip
{"BX2E",0x22},//2104 - 2 Games in 1 - Spider-Man - Mysterio's Menace + X2 - Wolverine's Revenge(UE).zip
{"BGZS",0x22},//2105 - Madagascar(ES).zip
{"AJAP",0x11},//2106 - Monster Jam - Maximum Destruction(EU).zip
{"BGPJ",0x11},//2107 - Get Ride! AMDriver - Shuggeki! Battle Party(JP).zip
{"BTDJ",0x22},//2108 - Poke Inu - Poket Dogs(JP).zip
{"B4WJ",0x11},//2109 - Rockman EXE 4 - Tournament Red Sun(JP).zip
{"B65E",0x00},//2110 - Connect Four - Perfection - Trouble(US).zip
{"B68E",0x00},//2111 - Marble Madness - Klax(US).zip
{"B6ZE",0x00},//2112 - Centipede - Breakout - Warlords(US).zip
{"B5NE",0x00},//2113 - Namco Museum 50th Anniversary(US).zip
{"B6BE",0x00},//2114 - Paperboy - Rampage(US).zip
{"B64E",0x00},//2115 - Pong - Asteroids - Yars' Revenge(US).zip
{"B67E",0x00},//2116 - Sorry! - Aggravation - Scrabble Junior(US).zip
{"ACSE",0x00},//2117 - Casper(US).zip
{"BRLE",0x22},//2118 - Rebelstar - Tactical Command(US).zip
{"BNCJ",0x22},//2119 - Tim Burton's Nightmare Before Christmas, The - The Pumpkin King(JP).zip
{"BMIJ",0x11},//2120 - Wagamama Fairy Mirumo de Pon! - Dokidoki Memorial Panic(JP).zip
{"BX5P",0x22},//2121 - Rayman 10th Anniversary - Rayman Advance + Rayman 3(EU).zip
{"B6EP",0x00},//2122 - Board Game Classics(EU).zip
{"AJDE",0x00},//2123 - James Pond - Codename Robocod(US).zip
{"BF4P",0x22},//2124 - Fantastic 4(EU).zip
{"B6JJ",0x33},//2125 - Super Robot Taisen(JP).zip
{"BTMJ",0x11},//2126 - Mario Tennis Advance(JP).zip
{"BZPJ",0x22},//2127 - Dr. Mario & Panel de Pon(JP).zip
{"BAKP",0x00},//2128 - Koala Brothers - Outback Adventures(EU).zip
{"B4ZP",0x22},//2129 - Megaman Zero 4(EU).zip
{"BYDP",0x11},//2130 - Yu-Gi-Oh! Destiny Board Traveler(EU).zip
{"BSOP",0x11},//2131 - Shaman King - Master of Spirits(EU).zip
{"BE5E",0x00},//2132 - Barbie and the Magic of Pegasus(US).zip
{"BPUS",0x22},//2133 - 2 Games in 1 - Scooby-Doo + Scooby-Doo 2 - Desatado(ES).zip
{"BRZP",0x00},//2134 - 2 Games in 1 - Power Rangers - Ninja Storm + Power Rangers - Time Force(EU).zip
{"AP8P",0x22},//2135 - Scooby-Doo(EU).zip
{"ASBJ",0x11},//2136 - Gyakuten Saiban(JP).zip
{"BW2E",0x00},//2137 - 2 Games in 1 - Cartoon Network - Block Party + Cartoon Network - Speedway(US).zip
{"BWQE",0x00},//2138 - 2 Games in 1 - Quad Desert Fury + Monster Trucks(US).zip
{"BRZE",0x00},//2139 - 2 Games in 1 - Power Rangers - Ninja Storm + Power Rangers - Time Force(US).zip
{"BILE",0x22},//2140 - Bionicle - Maze of Shadows(US).zip
{"B69E",0x00},//2141 - Gauntlet - Rampart(US).zip
{"B62E",0x00},//2142 - Millipede - Super Breakout - Lunar Lander(US).zip
{"BRDE",0x00},//2143 - Power Rangers - S.P.D.(UE).zip
{"BX5E",0x22},//2144 - Rayman 10th Anniversary - Rayman Advance + Rayman 3(US).zip
{"B25E",0x00},//2145 - Scooby-Doo! - Unmasked(US).zip
{"B6AE",0x00},//2146 - Spy Hunter - Super Sprint(US).zip
{"BJWE",0x22},//2147 - Tak - Great Juju Challenge(UE).zip
{"BBEE",0x00},//2148 - 2 Games in 1 - Barbie Groovy Games + Secret Agent Barbie - Royal Jewels Mission(US).zip
{"BTGP",0x22},//2149 - TG Rally(EU).zip
{"BUEE",0x22},//2150 - Danny Phantom - The Ultimate Enemy(US).zip
{"BWEE",0x22},//2151 - Whac-A-Mole(US).zip
{"BCDE",0x22},//2152 - Cinderella - Magical Dreams(US).zip
{"BHZE",0x00},//2153 - 2 Games in 1 - Hot Wheels - Velocity X + Hot Wheels - World Race(US).zip
{"V49J",0x11},//2154 - Screw Breaker - Goushin Dorirurero(JP).zip
{"BBOE",0x00},//2155 - Berenstain Bears, The - Spooky Old Tree(US).zip
{"BULE",0x22},//2156 - Ultimate Spider-Man(US).zip
{"BONE",0x22},//2157 - One Piece(US).zip
{"BF6E",0x22},//2158 - FIFA 06(UE).zip
{"BBXD",0x00},//2159 - Bibi Blocksberg - Der Magische Hexenkreis(DE).zip
{"BFOE",0x00},//2160 - Fairly Odd Parents!, The - Clash with the Anti-World(US).zip
{"AX4E",0x31},//2161 - Super Mario Advance 4 - Super Mario Bros. 3(US).zip
{"BBEP",0x00},//2162 - 2 Games in 1 - Barbie Groovy Games + Secret Agent Barbie - Royal Jewels Mission(EU).zip
{"B2CE",0x00},//2163 - Pac-Man World 2(US).zip
{"BGXJ",0x22},//2164 - Gunstar Super Heroes(JP).zip
{"B4ZE",0x22},//2166 - Megaman Zero 4(US).zip
{"BRRE",0x22},//2167 - Bratz - Rock Angelz(UE).zip
{"BNCE",0x22},//2168 - Tim Burton's Nightmare Before Christmas, The - The Pumpkin King(UE).zip
{"BS6E",0x22},//2169 - Backyard Skateboarding(US).zip
{"BEVE",0x22},//2170 - Ever Girl(US).zip
{"B6FP",0x00},//2171 - Chicken Shoot(EU).zip
{"B3RP",0x22},//2172 - Driver 3(EU).zip
{"BULX",0x22},//2173 - Ultimate Spider-Man(EU).zip
{"BIIJ",0x33},//2174 - Tsuukin Hitofude(JP).zip
{"BYGJ",0x11},//2175 - Yu-Gi-Oh! Duel Monsters GX - Mezase Duel King!(JP).zip
{"BBLE",0x22},//2176 - Teen Titans(US).zip
{"BYPP",0x22},//2177 - Horse & Pony Let's Ride 2(EU).zip
{"BPPJ",0x11},//2178 - Pokemon Pinball - Ruby & Sapphire(JP).zip
{"BGZI",0x22},//2179 - Madagascar(IT).zip
{"BF4I",0x22},//2180 - I Fantastici 4(IT).zip
{"BCXE",0x00},//2181 - Kid's Cards(US).zip
{"MCSE",0x00},//2182 - Game Boy Advance Video - Cartoon Network Collection - Special Edition(US).zip
{"BDUP",0x22},//2183 - Duel Masters - Shadow of the Code(EU).zip
{"B6ZP",0x00},//2184 - Centipede - Breakout - Warlords(EU).zip
{"BPED",0x31},//2185 - Pokemon - Smaragd-Edition(DE).zip
{"B53P",0x23},//2186 - 2 Games in 1 - Spyro Fusion + Crash Bandicoot Fusion(EU).zip
{"BPEF",0x31},//2187 - Pokemon - Version Emeraude(FR).zip
{"BYFE",0x22},//2188 - Backyard Football 2006(US).zip
{"BM7E",0x22},//2189 - Madagascar - Operation Penguin(US).zip
{"BPES",0x31},//2190 - Pokemon - Edicion Esmeralda(ES).zip
{"B26E",0x22},//2191 - World Poker Tour(US).zip
{"BDUE",0x22},//2192 - Duel Masters - Shadow of the Code(US).zip
{"BQQE",0x22},//2193 - SpongeBob SquarePants - Lights, Camera, Pants!(US).zip
{"BH9E",0x22},//2194 - Tony Hawk's American Sk8land(US).zip
{"B6BP",0x00},//2195 - Paperboy - Rampage(EU).zip
{"B8AE",0x22},//2196 - 2 Games in 1 - Crash Bandicoot 2 - N-Tranced + Crash Nitro Kart(US).zip
{"B2HP",0x22},//2197 - 2 Games in 1 - Hugo - Bukkazoom! + Hugo - The Evil Mirror Advance(EU).zip
{"BM7X",0x22},//2198 - Madagascar - Operation Penguin(EU).zip
{"BHGE",0x22},//2199 - Gunstar Super Heroes(US).zip
{"BCMJ",0x22},//2200 - Frontier Stories(JP).zip
{"B9BJ",0x00},//2201 - Kunio Kun Nekketsu Collection 2(JP).zip
{"BE5P",0x00},//2202 - Barbie and the Magic of Pegasus(EU).zip
{"BCHE",0x22},//2203 - Chicken Little(UE).zip
{"BUZE",0x22},//2204 - Ultimate Arcade Games(US).zip
{"B46E",0x33},//2205 - Sims 2, The(UE).zip
{"BEAP",0x22},//2206 - Care Bears - The Care Quests(EU).zip
{"BPEI",0x31},//2207 - Pokemon - Versione Smeraldo(IT).zip
{"BWTP",0x22},//2208 - W.i.t.c.h.(EU).zip
{"AIDF",0x22},//2209 - Space Invaders(FR).zip
{"B3RE",0x22},//2210 - Driver 3(US).zip
{"BNUE",0x00},//2211 - Nicktoons Unite!(US).zip
{"B4DE",0x00},//2212 - Sky Dancers - They Magically Fly!(US).zip
{"BZPP",0x22},//2213 - Dr. Mario & Puzzle League(EU).zip
{"BDQP",0x22},//2214 - Donkey Kong Country 3(EU).zip
{"BE8P",0x11},//2215 - Fire Emblem - The Sacred Stones(EU).zip
{"B69P",0x00},//2217 - Gauntlet - Rampart(EU).zip
{"BW5P",0x33},//2218 - 2 Games in 1 - Sonic Pinball Party + Sonic Advance(EU).zip
{"B63E",0x22},//2219 - Big Mutha Truckers(US).zip
{"BDQE",0x22},//2220 - Donkey Kong Country 3(US).zip
{"BTVE",0x22},//2221 - Ty the Tasmanian Tiger - Night of the Quinkan(US).zip
{"BHGP",0x22},//2222 - Gunstar Future Heroes(EU).zip
{"BH8E",0x23},//2223 - Harry Potter and the Goblet of Fire(UE).zip
{"B4UE",0x22},//2224 - Shrek Super Slam(US).zip
{"BW9P",0x33},//2225 - 2 Games in 1 - Columns Crown + Chu Chu Rocket!(EU).zip
{"B68P",0x00},//2226 - Marble Madness - Klax(EU).zip
{"BW6P",0x33},//2227 - 2 Games in 1 - Sonic Battle + Sonic Pinball Party(EU).zip
{"BW4P",0x33},//2228 - 2 Games in 1 - Sonic Advance + Sonic Battle(EU).zip
{"BIQX",0x00},//2229 - Incredibles, The - Rise of the Underminer(EU).zip
{"BLQP",0x22},//2230 - 2 Games in 1 - Peter Pan - Return to Neverland + Lilo & Stitch 2(EU).zip
{"BIBE",0x00},//2231 - Bible Game, The(US).zip
{"BKQP",0x22},//2233 - King Kong - The Official Game of the Movie(EU).zip
{"BF3E",0x00},//2234 - Ford Racing 3(US).zip
{"BH9X",0x22},//2235 - Tony Hawk's American Sk8land(EU).zip
{"B4UP",0x22},//2236 - Shrek Super Slam(EU).zip
{"BQQX",0x22},//2237 - SpongeBob SquarePants - Lights, Camera, Pants!(EU).zip
{"BFOP",0x00},//2238 - Fairly Odd Parents!, The - Clash with the Anti-World(EU).zip
{"B24J",0x33},//2239 - Pokemon Fushigi no Dungeon - Aka no Kyuujotai(JP).zip
{"BTMP",0x11},//2240 - Mario Power Tennis(EU).zip
{"BETE",0x00},//2241 - Atomic Betty(UE).zip
{"B86E",0x00},//2242 - Hello Kitty - Happy Party Pals(US).zip
{"BH4E",0x22},//2243 - Fantastic 4 - Flame On(US).zip
{"B6AP",0x00},//2244 - Spy Hunter - Super Sprint(EU).zip
{"B2WE",0x22},//2245 - Chronicles of Narnia, The - The Lion, the Witch and the Wardrobe(UE).zip
{"B62P",0x00},//2246 - Millipede - Super Breakout - Lunar Lander(EU).zip
{"B64P",0x00},//2247 - Pong - Asteroids - Yars' Revenge(EU).zip
{"BRRD",0x22},//2248 - Bratz - Rock Angelz(DE).zip
{"BTUP",0x22},//2249 - Totally Spies!(EU).zip
{"BNWE",0x22},//2250 - Need for Speed - Most Wanted(UE).zip
{"BIQE",0x00},//2251 - Incredibles, The - Rise of the Underminer(UE).zip
{"BUAE",0x22},//2252 - Ultimate Puzzle Games(US).zip
{"BR6J",0x11},//2253 - Rockman EXE 6 - Dennoujuu Faltzer(JP).zip
{"BR5J",0x11},//2254 - Rockman EXE 6 - Dennoujuu Grega(JP).zip
{"BWIP",0x22},//2255 - Winx Club(EU).zip
{"BQTP",0x22},//2256 - My Pet Hotel(EU).zip
{"BBAE",0x23},//2257 - Shamu's Deep Sea Adventures(US).zip
{"BT6E",0x22},//2258 - Trollz - Hair Affair!(US).zip
{"BOFD",0x00},//2259 - Ottifanten Pinball(DE).zip
{"B36P",0x11},//2260 - Dynasty Warriors Advance(EU).zip
{"BZPE",0x22},//2261 - Dr. Mario & Puzzle League(US).zip
{"BFLP",0x22},//2262 - Franklin's Great Adventures(EU).zip
{"BM7Y",0x22},//2263 - Madagascar - Operation Penguin(EU).zip
{"BCRP",0x22},//2264 - Crazy Frog Racer(EU).zip
{"BFKE",0x22},//2265 - Franklin the Turtle(US).zip
{"BEAE",0x22},//2266 - Care Bears - The Care Quests(US).zip
{"BD7E",0x22},//2267 - Proud Family, The(US).zip
{"BLOP",0x22},//2268 - Land Before Time, The - Into the Mysterious Beyond(EU).zip
{"B4LJ",0x22},//2269 - Sugar Sugar Rune - Heart Gaippai! Moegi Gakuen(JP).zip
{"BDQJ",0x22},//2270 - Super Donkey Kong Country 3(JP).zip
{"BUDJ",0x22},//2271 - Konjiki no Gashbell!! Yuujou no Zakeru Dream Tag Tournament(JP).zip
{"BTME",0x11},//2272 - Mario Tennis Power Tour(US).zip
{"B3CJ",0x23},//2273 - Summon Night - Craft Sword Monogatari Hajimari no Ishi(JP).zip
{"BZ4J",0x11},//2274 - Final Fantasy IV Advance(JP).zip
{"BZ4E",0x11},//2275 - Final Fantasy IV Advance(US).zip
{"B35P",0x22},//2276 - Strawberry Shortcake - Ice Cream Island Riding Camp(EU).zip
{"BHJP",0x22},//2277 - Heidi - The Game(EU).zip
{"B82E",0x22},//2278 - Dogz(US).zip
{"BEDE",0x22},//2279 - Ed, Edd n Eddy - The Mis-EDventures(US).zip
{"BEBE",0x00},//2280 - Elf Bowling 1 & 2(US).zip
{"BKQE",0x22},//2281 - Kong - The 8th Wonder of the World(US).zip
{"B2VP",0x00},//2282 - Snood 2 - Snoods on Vacation(EU).zip
{"BTUE",0x22},//2283 - Totally Spies!(US).zip
{"A4GE",0x22},//2284 - ZatchBell! - Electric Arena(US).zip
{"B73J",0x00},//2285 - Hudson Best Collection Vol. 3 - Action Collection(JP).zip
{"BFVJ",0x22},//2286 - Twin Series 1 - Mezase Debut! Fashion Designer Monogatari + Kawaii Pet Game Gallery 2(JP).zip
{"BHHE",0x22},//2287 - Hi Hi Puffy AmiYumi - Kaznapped!(US).zip
{"BLFE",0x22},//2288 - 2 Games in 1 - Dragon Ball Z I & II(US).zip
{"BK7E",0x00},//2289 - Kong - King of Atlantis(US).zip
{"BM7P",0x22},//2290 - Madagascar - Operation Penguin(EU).zip
{"BGOE",0x00},//2291 - Garfield - The Search for Pooky(US).zip
{"A7ME",0x00},//2292 - Amazing Virtual Sea Monkeys, The(US).zip
{"ATTE",0x00},//2293 - Tiny Toon Adventures - Scary Dreams(US).zip
{"AWLF",0x00},//2294 - Famille Delajungle, La - Le Film(FR).zip
{"BW5E",0x33},//2295 - 2 Games in 1 - Sonic Pinball Party + Sonic Advance(US).zip
{"BO2J",0x22},//2296 - Ochainu no Bouken Jima - Honwaka Yume no Island(JP).zip
{"BHHJ",0x22},//2297 - Hi! Hi! Puffy AmiYumi(JP).zip
{"ZMBJ",0x11},//2298 - Playan Micro(JP).zip
{"BXKE",0x11},//2299 - 2 Games in 1 - Castlevania - Harmony of Dissonance + Castlevania - Aria of Sorrow(US).zip
{"BTDE",0x22},//2300 - Pocket Dogs(US).zip
{"BWIE",0x22},//2301 - Winx Club(US).zip
{"BYGE",0x11},//2302 - Yu-Gi-Oh! GX Duel Academy(US).zip
{"BLNP",0x22},//2303 - 2 Games in 1 - Looney Tunes - Acme Antics + Looney Tunes - Dizzy Driving(EU).zip
{"BGQE",0x22},//2304 - Greg Hastings' Tournament Paintball Max'D(US).zip
{"B86X",0x00},//2305 - Hello Kitty - Happy Party Pals(EU).zip
{"B3JE",0x22},//2306 - Curious George(US).zip
{"BWUD",0x00},//2307 - Die Wilden Fussball Kerle - Entscheidung im Teufelstopf(DE).zip
{"V49E",0x11},//2308 - Drill Dozer(US).zip
{"BWYP",0x22},//2309 - Winter Sports(EU).zip
{"BAHP",0x22},//2310 - Alien Hominid(EU).zip
{"BW2P",0x00},//2311 - 2 Games in 1 - Cartoon Network - Block Party + Cartoon Network - Speedway(EU).zip
{"B63P",0x22},//2312 - Big Mutha Truckers(EU).zip
{"BF3P",0x00},//2313 - Ford Racing 3(EU).zip
{"xxxx",0x00},//2314 - GBADev 2004Mbit Competition(US).zip
{"BRDX",0x00},//2315 - Power Rangers - S.P.D.(EU).zip
{"B3ZE",0x22},//2316 - Global Star Sudoku Fever(US).zip
{"B2MP",0x11},//2317 - Shaman King - Master of Spirits 2(EU).zip
{"BNUP",0x00},//2318 - SpongeBob SquarePants and Friends Unite!(EU).zip
{"BXKP",0x11},//2319 - 2 Games in 1 - Castlevania - Harmony of Dissonance + Castlevania - Aria of Sorrow(EU).zip
{"BJWX",0x22},//2320 - Tak - Great Juju Challenge(EU).zip
{"BRQP",0x00},//2321 - Rec Room Challenge(EU).zip
{"BY6J",0x11},//2322 - Yu-Gi-Oh! Duel Monsters Expert 2006(JP).zip
{"BKMP",0x22},//2323 - Kim Possible 2 - Drakken's Demise(EU).zip
{"BURE",0x22},//2324 - Paws & Claws - Pet Resort(US).zip
{"BFBP",0x11},//2325 - 2 Games in 1 - Disney Sports - Football + Disney Sports - Skateboarding(EU).zip
{"BIND",0x00},//2326 - 2 Games in 1 - Findet Nemo + Unglaublichen, Die(DE).zip
{"BFWY",0x00},//2327 - 2 Games in 1 - Findet Nemo + Findet Nemo - Das Abenteuer Geht Weiter(DE).zip
{"BK7P",0x00},//2328 - Kong - King of Atlantis(EU).zip
{"A3AC",0x22},//2329 - Yaoxi Dao(CN).zip
{"AMTC",0x11},//2330 - Miteluode Ronghe(CN).zip
{"AN8E",0x22},//2331 - Tales of Phantasia(US).zip
{"BWCP",0x00},//2332 - 2 Games in 1 - Golden Nugget Casino + Texas Hold'em Poker(EU).zip
{"BWQP",0x00},//2333 - 2 Games in 1 - Quad Desert Fury + Monster Trucks(EU).zip
{"A7AE",0x22},//2334 - Naruto - Ninja Council(US).zip
{"BZWJ",0x22},//2335 - Akagi(JP).zip
{"B76J",0x00},//2336 - Hudson Best Collection Vol. 6 - Bouken Jima Collection(JP).zip
{"BUHJ",0x22},//2337 - Ueki no Housoku(JP).zip
{"B25X",0x00},//2338 - Scooby-Doo! - Unmasked(EU).zip
{"B9CJ",0x00},//2339 - Kunio Kun Nekketsu Collection 3(JP).zip
{"BIAE",0x22},//2340 - Ice Age 2 - The Meltdown(US).zip
{"BY2P",0x11},//2341 - 2 Games in 1 - Yu-Gi-Oh! The Sacred Cards + Yu-Gi-Oh! Reshef of Destruction(EU).zip
{"BT8P",0x22},//2342 - 2 Games in 1 - Teenage Mutant Ninja Turtles + Teenage Mutant Ninja Turtles 2 - Battle Nexus(EU).zip
{"BY6E",0x11},//2343 - Yu-Gi-Oh! Ultimate Masters - World Championship Tournament 2006(US).zip
{"BN4J",0x11},//2344 - Kawa no Nushitsuri 3 & 4(JP).zip
{"B3NP",0x00},//2345 - 3 in 1 Sports Pack - Paintball Splat! + Dodgeball - Dodge This! + Big Alley Bowling(EU).zip
{"BY6P",0x11},//2346 - Yu-Gi-Oh! Ultimate Masters Edition - World Championship Tournament 2006(EU).zip
{"BBQJ",0x11},//2347 - Pawa Poke Dash(JP).zip
{"BYUJ",0x23},//2348 - Yggdra Union(JP).zip
{"BTRE",0x33},//2349 - Welcome to the Tower SP(US).zip
{"AN3E",0x22},//2350 - Catz(UE).zip
{"AN8P",0x22},//2351 - Tales of Phantasia(EU).zip
{"BCZE",0x22},//2352 - Street Racing Syndicate(US).zip
{"BIAP",0x22},//2353 - Ice Age 2 - The Meltdown(EU).zip
{"B27E",0x22},//2354 - Top Spin 2(US).zip
{"BL9E",0x22},//2355 - Let's Ride! Dreamer - Inspired by a True Story(US).zip
{"B3ZP",0x22},//2356 - Global Star Sudoku Fever(EU).zip
{"B34E",0x22},//2357 - Let's Ride! Sunshine Stables(US).zip
{"AA2C",0x22},//2358 - Chaoji Maliou Shijie(CN).zip
{"BWOP",0x22},//2359 - World Poker Tour(EU).zip
{"AYWJ",0x11},//2360 - Yu-Gi-Oh! Duel Monsters - International Worldwide Edition(JP).zip
{"BKCS",0x22},//2361 - Shin-chan - Aventuras en Cineland(ES).zip
{"B5NP",0x00},//2362 - Namco Museum 50th Anniversary(EU).zip
{"BIIP",0x33},//2363 - Polarium Advance(EU).zip
{"BEDP",0x22},//2364 - Ed, Edd n Eddy - The Mis-EDventures(EU).zip
{"BELP",0x00},//2365 - Elf - The Movie(EU).zip
{"BWLE",0x22},//2366 - Wild, The(UE).zip
{"BC2J",0x23},//2367 - Crayon Shin-chan - Densetsu wo Yobu Omake no Miyako Shockgaan!(JP).zip
{"BDZF",0x00},//2368 - 2 Games in 1 - Monstres & Cie + Le Monde de Nemo(FR).zip
{"BH4P",0x22},//2369 - Fantastic 4 - Flame On(EU).zip
{"BE4J",0x11},//2370 - Eyeshield 21 - Devilbats Devildays(JP).zip
{"BUOJ",0x22},//2371 - Minna no Soft Series - Numpla Advance(JP).zip
{"ABFP",0x11},//2372 - Breath of Fire(EU).zip
{"B27P",0x22},//2373 - Top Spin 2(EU).zip
{"AN3J",0x22},//2374 - Nakayoshi Pet Advance Series 3 - Kawaii Koneko(JP).zip
{"ASOJ",0x32},//2375 - Sonic Advance(JP).zip
{"B25Y",0x00},//2376 - Scooby-Doo! - Unmasked(EU).zip
{"A3UJ",0x33},//2377 - Mother 3(JP).zip
{"BCGE",0x22},//2378 - Cabbage Patch Kids - The Patch Puppy Rescue(US).zip
{"BLOE",0x22},//2379 - Land Before Time, The - Into the Mysterious Beyond(US).zip
{"BFLE",0x22},//2380 - Franklin's Great Adventures(US).zip
{"BUEX",0x22},//2381 - Danny Phantom - The Ultimate Enemy(EU).zip
{"BT5F",0x00},//2382 - 2 Games in 1 - Titeuf - Ze Gagmachine + Titeuf - Mega Compet'(FR).zip
{"B6WE",0x22},//2383 - 2006 FIFA World Cup(UE).zip
{"BQKJ",0x22},//2384 - Konchuu no Mori no Daibouken(JP).zip
{"B7IJ",0x00},//2385 - Hudson Best Collection Vol. 1 - Bomberman Collection(JP).zip
{"BINP",0x00},//2386 - 2 Games in 1 - Finding Nemo + Incredibles, The(EU).zip
{"BJDP",0x22},//2387 - Dragon's Rock(EU).zip
{"B2QP",0x22},//2388 - 2 Games in 1 - Prince of Persia - The Sands of Time + Tomb Raider - The Prophecy(EU).zip
{"BAKE",0x00},//2389 - Koala Brothers - Outback Adventures(US).zip
{"BJKP",0x22},//2390 - Juka and the Monophonic Menace(EU).zip
{"B6PP",0x22},//2391 - 2 Games in 1 - Pac-Man World + Ms. Pac-Man - Maze Madness(EU).zip
{"BUOE",0x22},//2392 - Dr. Sudoku(US).zip
{"MCPF",0x00},//2393 - Game Boy Advance Video - Cartoon Network Collection - Edition Premium(FR).zip
{"B8SE",0x22},//2394 - 2 Games in 1 - Spyro - Season of Ice + Spyro 2 - Season of Flame(US).zip
{"U32J",0x22},//2395 - Zoku Bokura no Taiyou - Taiyou Shounen Django(JP).zip
{"BH5E",0x22},//2396 - Over the Hedge(US).zip
{"BTJE",0x22},//2397 - Tringo(US).zip
{"BGZJ",0x22},//2398 - Madagascar(JP).zip
{"B74J",0x00},//2399 - Hudson Best Collection Vol. 4 - Nazotoki Collection(JP).zip
{"BW6J",0x33},//2400 - 2 Games in 1 - Sonic Battle + Sonic Pinball Party(JP).zip
{"BW4J",0x33},//2401 - 2 Games in 1 - Sonic Battle + Sonic Advance(JP).zip
{"BC4E",0x00},//2402 - Candy Land - Chutes and Ladders - Memory(US).zip
{"B3UE",0x00},//2403 - Life - Yahtzee - Payday(US).zip
{"BNMJ",0x11},//2404 - Mahou Sensei Negima! - Private Lesson 2 Ojamashimasuu Parasite de Chu(JP).zip
{"B3XE",0x22},//2405 - X-Men III - The Official Game(US).zip
{"BIXJ",0x31},//2406 - Calciobit(JP).zip
{"BAXJ",0x11},//2407 - Animal Yokochou - Dokidoki Shinkyu Shiken(JP).zip
{"B72J",0x00},//2408 - Hudson Best Collection Vol. 2 - Lode Runner Collection(JP).zip
{"BAYJ",0x11},//2409 - Animal Yokochou - Dokidoki Kyushutsu Daisakusen! no Maki(JP).zip
{"B3IJ",0x22},//2410 - Miracle! Banzou - 7 Tsuno Hosh no Kaizokuno(JP).zip
{"B6GE",0x00},//2411 - Chicken Shoot 2(US).zip
{"BC7E",0x22},//2412 - Backyard Sports - Baseball 2007(US).zip
{"BG9E",0x22},//2413 - Garfield and His Nine Lives(US).zip
{"BBMJ",0x22},//2414 - B-Legends! Battle B-Daman - Fire Spirits!(JP).zip
{"B43J",0x22},//2415 - Cinnamon - Fuwafuwa Daisakusen(JP).zip
{"B3XP",0x22},//2416 - X-Men III - The Official Game(EU).zip
{"BZ4P",0x11},//2417 - Final Fantasy IV Advance(EU).zip
{"B6FE",0x00},//2418 - Chicken Shoot(US).zip
{"BHOP",0x00},//2419 - Hardcore Pool(EU).zip
{"BRWP",0x00},//2420 - Racing Fever(EU).zip
{"B53E",0x23},//2421 - 2 Games in 1 - Spyro Orange - The Cortex Conspiracy + Crash Bandicoot Purple - Ripto's Rampage(US).zip
{"BULP",0x22},//2422 - Ultimate Spider-Man(EU).zip
{"BCAE",0x22},//2423 - Cars(UE).zip
{"BBVE",0x22},//2424 - Babar to the Rescue(US).zip
{"BH9P",0x22},//2425 - Tony Hawk's American Sk8land(EU).zip
{"BDVE",0x22},//2426 - Dragon Ball - Advanced Adventure(US).zip
{"BR5E",0x11},//2427 - Megaman Battle Network 6 - Cybeast Gregar(US).zip
{"BR5P",0x11},//2428 - Megaman Battle Network 6 - Cybeast Gregar(EU).zip
{"BR6P",0x11},//2429 - Megaman Battle Network 6 - Cybeast Falzar(EU).zip
{"BCDP",0x22},//2430 - Cinderella - Magical Dreams(EU).zip
{"BH5D",0x22},//2431 - Ab Durch Die Hecke(DE).zip
{"BAQP",0x22},//2432 - Premier Action(EU).zip
{"BG9P",0x22},//2433 - Garfield and His Nine Lives(EU).zip
{"B44P",0x22},//2434 - 3 Games in 1 - Rugrats - I Gotta Go Party + SpongeBob SquarePants - SuperSponge + Tak and the Power of Juju(EU).zip
{"B8QE",0x22},//2435 - Pirates of the Caribbean - Dead Man's Chest(UE).zip
{"BR6E",0x11},//2436 - Megaman Battle Network 6 - Cybeast Falzar(US).zip
{"BY2E",0x11},//2437 - 2 Games in 1 - Yu-Gi-Oh! The Sacred Cards + Yu-Gi-Oh! Reshef of Destruction(US).zip
{"BPJE",0x22},//2438 - Pocket Professor(US).zip
{"BCZP",0x22},//2439 - Street Racing Syndicate(EU).zip
{"BQVP",0x22},//2440 - Meine Tierarztpraxis(EU).zip
{"BM7S",0x22},//2441 - Madagascar - Operacion Pinguino(ES).zip
{"BVDJ",0x11},//2442 - bit Generations - Boundish(JP).zip
{"BVBJ",0x11},//2443 - bit Generations - Dial Hex(JP).zip
{"BVCJ",0x11},//2444 - bit Generations - Dotstream(JP).zip
{"BQ7E",0x22},//2445 - Monster House(US).zip
{"BH5S",0x22},//2446 - Vecinos Invasores(ES).zip
{"BCHJ",0x22},//2447 - Chicken Little(JP).zip
{"B75J",0x00},//2448 - Hudson Best Collection Vol. 5 - Shooting Collection(JP).zip
{"BCAJ",0x22},//2449 - Cars(JP).zip
{"BYGP",0x11},//2450 - Yu-Gi-Oh! GX Duel Academy(EU).zip
{"BVGJ",0x11},//2451 - bit Generations - Soundvoyager(JP).zip
{"BVEJ",0x11},//2452 - bit Generations - Orbital(JP).zip
{"BVHJ",0x11},//2453 - bit Generations - Digidrive(JP).zip
{"BVAJ",0x11},//2454 - bit Generations - Coloris(JP).zip
{"BUYE",0x22},//2455 - Ant Bully, The(US).zip
{"BW3J",0x33},//2456 - 2 Games in 1 - Sonic Advance + Chu Chu Rocket!(JP).zip
{"BHSJ",0x22},//2457 - Hamster Monogatari 3EX + 4 Special(JP).zip
{"AB4E",0x22},//2458 - Summon Night - Swordcraft Story(US).zip
{"BFWP",0x00},//2459 - 2 Games in 1 - Finding Nemo + Finding Nemo - The Continuing Adventures(EU).zip
{"BIQJ",0x00},//2460 - Mr. Incredible - Kyouteki Under Minor Toujyou(JP).zip
{"BBYE",0x22},//2461 - Barnyard(US).zip
{"BRIJ",0x11},//2462 - Rhythm Tengoku(JP).zip
{"BDXE",0x22},//2463 - Battle B-Daman(US).zip
{"BLAX",0x00},//2464 - Scrabble Scramble(EU).zip
{"BBVP",0x22},//2465 - Babar to the Rescue(EU).zip
{"BZTE",0x22},//2466 - VeggieTales - LarryBoy and the Bad Apple(US).zip
{"AOGE",0x33},//2467 - Super Robot Taisen - Original Generation(US).zip
{"BRTJ",0x22},//2468 - Robots(JP).zip
{"BQ7X",0x22},//2469 - Monster House(EU).zip
{"BG7E",0x00},//2470 - Games Explosion!(US).zip
{"B7ME",0x22},//2471 - Madden NFL 07(US).zip
{"BUFE",0x22},//2472 - 2 Games in 1 - Dragon Ball Z - Buu's Fury + Dragon Ball GT - Transformation(US).zip
{"BCAD",0x22},//2473 - Cars(DE).zip
{"BUOP",0x22},//2474 - Dr. Sudoku(EU).zip
{"BUYP",0x22},//2475 - Ant Bully, The(EU).zip
{"BYVE",0x11},//2476 - 2 Games in 1 - Yu-Gi-Oh! Destiny Board Traveler + Yu-Gi-Oh! Dungeon Dice Monsters(EU).zip
{"BCAI",0x22},//2477 - Cars(IT).zip
{"AVSP",0x33},//2478 - Sword of Mana(EU).zip
{"BL7E",0x22},//2479 - LEGO Star Wars II - The Original Trilogy(US).zip
{"BLPI",0x22},//2480 - 2 Games in 1 - Disney Principesse + Re Leone, Il(IT).zip
{"BEIE",0x00},//2481 - Little Einsteins(US).zip
{"BCQE",0x22},//2482 - Cheetah Girls, The(US).zip
{"BL7P",0x22},//2483 - LEGO Star Wars II - The Original Trilogy(EU).zip
{"BOAE",0x22},//2484 - Open Season(US).zip
{"B24E",0x31},//2485 - Pokemon Mystery Dungeon - Red Rescue Team(US).zip
{"BBYX",0x22},//2486 - Barnyard(EU).zip
{"BL6E",0x00},//2487 - My Little Pony - Crystal Princess - The Runaway Rainbow(US).zip
{"BQYE",0x22},//2488 - Danny Phantom - Urban Jungle(US).zip
{"BC6E",0x00},//2489 - Capcom Classics - Mini Mix(US).zip
{"BFXE",0x22},//2490 - Phil of the Future(US).zip
{"BFEE",0x22},//2491 - Dogz - Fashion(US).zip
{"B7FE",0x22},//2492 - FIFA 07(UE).zip
{"AN3X",0x22},//2493 - Catz(EU).zip
{"BAWE",0x22},//2494 - Alex Rider - Stormbreaker(US).zip
{"BB7E",0x22},//2495 - Backyard Sports - Basketball 2007(US).zip
{"BF7E",0x22},//2496 - Backyard Sports - Football 2007(US).zip
{"B4TE",0x22},//2497 - Strawberry Shortcake - Sweet Dreams(US).zip
{"BU4E",0x00},//2498 - Unfabulous!(US).zip
{"BAPE",0x22},//2499 - American Dragon - Jake Long - Rise of the Huntsclan!(UE).zip
{"BUXD",0x22},//2500 - Bibi und Tina - Ferien auf dem Martinshof(DE).zip
{"BN9E",0x22},//2501 - Little Mermaid, The - Magic in Two Kingdoms(UE).zip
{"BZ5J",0x11},//2502 - Final Fantasy V Advance(JP).zip
{"BWBF",0x22},//2503 - 2 Games in 1 - Disney Princesse + Frere des Ours(FR).zip
{"BRRF",0x22},//2504 - Bratz - Rock Angelz(FR).zip
{"BHUP",0x22},//2506 - Horse And Pony - My Stud Farm(EU).zip
{"BBME",0x22},//2507 - Battle B-Daman - Fire Spirits!(US).zip
{"AP8S",0x22},//2508 - Scooby-Doo(ES).zip
{"BQNE",0x22},//2509 - Disney Princess - Royal Adventure(US).zip
{"BN2E",0x11},//2510 - Naruto - Ninja Council 2(US).zip
{"BH7E",0x22},//2511 - Over the Hedge - Hammy Goes Nuts!(US).zip
{"B2FE",0x00},//2512 - Family Feud(US).zip
{"BQZE",0x22},//2513 - Avatar - The Last Airbender(US).zip
{"BUQE",0x00},//2514 - 2 Games in 1 - Uno + Skip-Bo(US).zip
{"BR7E",0x00},//2515 - Rock'em Sock'em Robots(US).zip
{"B33E",0x22},//2516 - Santa Clause 3, The - The Escape Clause(US).zip
{"BYME",0x00},//2517 - Monster Trucks Mayhem(US).zip
{"BHNE",0x00},//2518 - Original Harlem Globetrotters, The(US).zip
{"BXCE",0x00},//2519 - Ker Plunk! - Toss Across - Tip It(US).zip
{"B3BE",0x00},//2520 - ATV Thunder Ridge Riders(US).zip
{"BB3E",0x00},//2521 - Barbie in the 12 Dancing Princesses(US).zip
{"BB4E",0x00},//2522 - 2 Games in 1 - Matchbox Emergency Response + Matchbox Air, Land & Sea Rescue(US).zip
{"BNPE",0x00},//2523 - Princess Natasha(US).zip
{"BXFE",0x22},//2524 - Bratz - Forever Diamondz(US).zip
{"BXPE",0x00},//2525 - Dora the Explorer - Dora's World Adventure!(US).zip
{"BT8E",0x22},//2526 - 2 Games in 1 - Teenage Mutant Ninja Turtles + Teenage Mutant Ninja Turtles 2 - Battle Nexus(US).zip
{"BJHE",0x22},//2527 - Justice League Heroes - The Flash(US).zip
{"BNKE",0x00},//2528 - Noddy - A Day in Toyland(US).zip
{"BBIE",0x22},//2529 - Barbie Diaries, The - High School Mystery(US).zip
{"B3YE",0x22},//2530 - Legend of Spyro, The - A New Beginning(US).zip
{"BSKE",0x22},//2531 - Summon Night - Swordcraft Story 2(US).zip
{"BS7E",0x22},//2532 - 2 Games in 1 - Shrek 2 + Shark Tale(US).zip
{"BQ4E",0x22},//2533 - SpongeBob SquarePants - Creature from the Krusty Krab(US).zip
{"BH5I",0x22},//2534 - Gang del Bosco, La(IT).zip
{"AUME",0x00},//2535 - Mummy, The(US).zip
{"B3OE",0x00},//2536 - Mouse Trap - Simon - Operation(US).zip
{"BQJE",0x00},//2537 - 2 Games in 1 - Hot Wheels - Stunt Track Challenge + Hot Wheels - World Race(US).zip
{"BLHE",0x00},//2538 - Flushed Away(US).zip
{"BHBP",0x22},//2539 - Hunde & Katzen Best Friends(EU).zip
{"BH7P",0x22},//2540 - Over the Hedge - Hammy Goes Nuts!(EU).zip
{"BJAP",0x00},//2541 - GT Racers(EU).zip
{"B2VE",0x00},//2542 - Snood 2 - Snoods on Vacation(US).zip
{"BU5E",0x00},//2543 - Uno 52(US).zip
{"BHVP",0x22},//2544 - Scurge - Hive(EU).zip
{"B3YP",0x22},//2545 - Legend of Spyro, The - A New Beginning(EU).zip
{"BXFD",0x22},//2546 - Bratz - Forever Diamondz(DE).zip
{"BZUE",0x22},//2547 - Teen Titans 2 - The Brotherhood's Revenge(US).zip
{"BFYE",0x22},//2548 - Foster's Home for Imaginary Friends(US).zip
{"BNVE",0x22},//2549 - Nicktoons - Battle for Volcano Island(US).zip
{"BZCE",0x22},//2550 - Suite Life of Zack & Cody, The - Tipton Caper(US).zip
{"BLCE",0x22},//2551 - Camp Lazlo - Leaky Lake Games(US).zip
{"BX3E",0x23},//2552 - 2 Games in 1 - Spider-Man + Spider-Man 2(US).zip
{"B3NE",0x00},//2553 - 3 in 1 Sports Pack - Paintball Splat! + Dodgeball - Dodge This! + Big Alley Bowling(US).zip
{"B4ME",0x22},//2554 - Marvel - Ultimate Alliance(US).zip
{"BOAX",0x22},//2555 - Open Season(EU).zip
{"BN7E",0x22},//2556 - Need for Speed Carbon - Own the City(UE).zip
{"BRQE",0x00},//2557 - Rec Room Challenge(US).zip
{"BRLP",0x22},//2558 - Rebelstar - Tactical Command(EU).zip
{"BJKE",0x22},//2559 - Juka and the Monophonic Menace(US).zip
{"BHVE",0x22},//2560 - Scurge - Hive(US).zip
{"BL8P",0x22},//2561 - Tomb Raider - Legend(EU).zip
{"BQ4P",0x22},//2562 - SpongeBob SquarePants - Creature from the Krusty Krab(EU).zip
{"BXSE",0x22},//2563 - Tony Hawk's Downhill Jam(US).zip
{"BZ5E",0x11},//2564 - Final Fantasy V Advance(US).zip
{"BIEE",0x22},//2565 - Grim Adventures of Billy & Mandy, The(US).zip
{"BJTE",0x00},//2566 - Tom and Jerry Tales(US).zip
{"B24P",0x31},//2567 - Pokemon Mystery Dungeon - Red Rescue Team(EU).zip
{"BWJP",0x00},//2568 - Who Wants to Be a Millionaire Junior(EU).zip
{"B4OE",0x33},//2569 - Sims 2, The - Pets(US).zip
{"BU2E",0x22},//2570 - 2 Games in 1 - SpongeBob SquarePants - Battle for Bikini Bottom + Nicktoons - Freeze Frame Frenzy(US).zip
{"BPUE",0x22},//2571 - 2 Games in 1 - Scooby-Doo + Scooby-Doo 2 - Monsters Unleashed(US).zip
{"B6PE",0x22},//2572 - 2 Games in 1 - Pac-Man World + Ms. Pac-Man - Maze Madness(US).zip
{"BXHE",0x22},//2573 - 2 Games in 1 - Shrek 2 + Madagascar - Operation Penguin(US).zip
{"BLNE",0x22},//2574 - 2 Games in 1 - Looney Tunes - Acme Antics + Looney Tunes - Dizzy Driving(US).zip
{"B23E",0x22},//2575 - 2 Games in 1 - Cabela's Big Game Hunter - 2005 Adventures + Rapala Pro Fishing(US).zip
{"BIHE",0x22},//2576 - Bionicle Heroes(US).zip
{"B82Y",0x22},//2577 - Dogz(EU).zip
{"BXSP",0x22},//2578 - Tony Hawk's Downhill Jam(EU).zip
{"BIHP",0x22},//2579 - Bionicle Heroes(EU).zip
{"B2RE",0x33},//2580 - Super Robot Taisen - Original Generation 2(US).zip
{"BIJE",0x22},//2581 - Sonic the Hedgehog - Genesis(US).zip
{"BWVE",0x22},//2582 - Winx Club - Quest for the Codex(US).zip
{"AH8P",0x00},//2583 - Hot Wheels - Velocity X(EU).zip
{"B4OX",0x33},//2584 - Sims 2, The - Pets(EU).zip
{"BC9E",0x22},//2585 - Spider-Man - Battle for New York(US).zip
{"BENP",0x22},//2586 - Eragon(EU).zip
{"BENE",0x22},//2587 - Eragon(US).zip
{"BHHP",0x22},//2588 - Hi Hi Puffy AmiYumi - Kaznapped!(EU).zip
{"BJTP",0x00},//2589 - Tom and Jerry Tales(EU).zip
{"BH3P",0x23},//2590 - Happy Feet(EU).zip
{"BJHP",0x22},//2591 - Justice League Heroes - The Flash(EU).zip
{"BLHP",0x00},//2592 - Flushed Away(EU).zip
{"BKFX",0x22},//2593 - Biene Maja, Die - Klatschmohnwiese in Gefahr(DE).zip
{"BQNP",0x22},//2594 - Disney Princess - Royal Adventure(EU).zip
{"BH3E",0x23},//2595 - Happy Feet(US).zip
{"BIIE",0x33},//2596 - Polarium Advance(US).zip
{"BT7E",0x00},//2597 - Tonka - On the Job(US).zip
{"BYUE",0x23},//2598 - Yggdra Union - We'll Never Fight Alone(US).zip
{"BZ6J",0x11},//2599 - Final Fantasy VI Advance(JP).zip
{"BL8E",0x22},//2600 - Tomb Raider - Legend(US).zip
{"BL4E",0x33},//2601 - Lizzie McGuire 2 - Lizzie Diaries Special Edition(US).zip
{"A9ME",0x11},//2602 - Motoracer Advance(US).zip
{"BDZE",0x00},//2603 - 2 Games in 1 - Monsters, Inc. + Finding Nemo(US).zip
{"B2LE",0x22},//2604 - Totally Spies! 2 - Undercover(US).zip
{"BHXE",0x00},//2605 - Hot Wheels - All Out(US).zip
{"BQLE",0x00},//2606 - March of the Penguins(US).zip
{"BP9P",0x00},//2607 - World Championship Poker(EU).zip
{"BB4P",0x00},//2608 - 2 Games in 1 - Matchbox Emergency Response + Matchbox Air, Land & Sea Rescue(EU).zip
{"BC9P",0x22},//2609 - Spider-Man - Battle for New York(EU).zip
{"B3JP",0x22},//2610 - Curious George(EU).zip
{"BCJE",0x22},//2611 - Charlotte's Web(US).zip
{"BYAE",0x00},//2612 - F24 Stealth Fighter(US).zip
{"B4IE",0x22},//2613 - Shrek - Smash n' Crash Racing(US).zip
{"BFRP",0x23},//2614 - My Animal Centre in Africa(EU).zip
{"MN3E",0x00},//2615 - Game Boy Advance Video - Nicktoons - Volume 3(US).zip
{"BTJP",0x22},//2616 - Tringo(EU).zip
{"BQ3E",0x22},//2617 - Rayman - Raving Rabbids(US).zip
{"BQXE",0x22},//2618 - Superman Returns - Fortress of Solitude(UE).zip
{"BZ4J",0x11},//2619 - Final Fantasy IV Advance(JP).zip
{"BWVP",0x22},//2620 - Winx Club - Quest for the Codex(EU).zip
{"BQ3P",0x22},//2621 - Rayman - Raving Rabbids(EU).zip
{"BFQE",0x22},//2622 - Mazes of Fate(US).zip
{"BWZP",0x22},//2623 - 2 Games in 1 - Winnie the Pooh's - Rumbly Tumbly Adventure + Rayman 3(EU).zip
{"B2BP",0x22},//2624 - 2 Games in 1 - SpongeBob SquarePants - The Movie + SpongeBob SquarePants and Friends - Freeze Frame Frenzy(EU).zip
{"B4DP",0x00},//2625 - Sky Dancers - They Magically Fly!(EU).zip
{"B6GP",0x00},//2626 - Chicken Shoot 2(EU).zip
{"B86P",0x00},//2627 - Hello Kitty - Happy Party Pals(EU).zip
{"BCGP",0x22},//2628 - Cabbage Patch Kids - The Patch Puppy Rescue(EU).zip
{"BTZP",0x22},//2629 - Tokyo Xtreme Racer Advance(EU).zip
{"MCPE",0x00},//2630 - Game Boy Advance Video - Cartoon Network Collection - Premium Edition(US).zip
{"BHQP",0x00},//2631 - Agent Hugo - Roborumble(EU).zip
{"BX6E",0x00},//2632 - 2 Games in 1 - SpongeBob SquarePants - Battle for Bikini Bottom + Fairly Odd Parents!, The - Breakin' da Rules(US).zip
{"ZMDE",0x00},//2633 - Nintendo MP3 Player(US).zip
{"BQZP",0x22},//2634 - Avatar - The Legend of Aang(EU).zip
{"BBCE",0x00},//2635 - Back to Stone(US).zip
{"BT6Y",0x22},//2636 - Trollz - Hair Affair!(EU).zip
{"B2NP",0x22},//2637 - Arthur and the Minimoys(EU).zip
{"AKOE",0x22},//2638 - King of Fighters EX, The - NeoBlood(US).zip
{"BBZP",0x00},//2639 - Bratz - Babyz(EU).zip
{"BFEP",0x22},//2640 - Dogz - Fashion(EU).zip
{"AUGP",0x00},//2641 - Medal of Honor - Underground(EU).zip
{"BOAP",0x22},//2642 - Open Season(EU).zip
{"AE3E",0x22},//2643 - Ed, Edd n Eddy - Jawbreakers!(US).zip
{"BXGE",0x22},//2644 - 2 Games in 1 - Shrek 2 + Madagascar(US).zip
{"BF2P",0x00},//2645 - Fairly Odd Parents!, The - Shadow Showdown(EU).zip
{"BBZE",0x00},//2646 - Bratz - Babyz(US).zip
{"B4MP",0x22},//2647 - Marvel - Ultimate Alliance(EU).zip
{"BCAY",0x22},//2648 - Cars(EU).zip
{"BRDY",0x00},//2649 - Power Rangers - S.P.D.(EU).zip
{"BFWZ",0x00},//2650 - 2 Games in 1 - Finding Nemo + Finding Nemo - The Continuing Adventures(EU).zip
{"BUEP",0x22},//2651 - Danny Phantom - The Ultimate Enemy(EU).zip
{"BQ7P",0x22},//2652 - Monster House(EU).zip
{"BINZ",0x00},//2653 - 2 Games in 1 - Buscando a Nemo + Increibles, Los(ES).zip
{"BR7P",0x00},//2654 - Rock'em Sock'em Robots(EU).zip
{"BRRS",0x22},//2655 - Bratz - Rock Angelz(ES).zip
{"BT6X",0x22},//2656 - Trollz - Hair Affair!(EU).zip
{"BHNP",0x00},//2657 - Original Harlem Globetrotters, The(EU).zip
{"BC2S",0x23},//2658 - Shin-chan - Contra los Munecos de Shock Gahn(ES).zip
{"B2NE",0x22},//2659 - Arthur and the Invisibles(US).zip
{"B2EE",0x00},//2660 - 2 Games in 1 - Dora the Explorer - The Search for the Pirate Pig's Treasure + Dora the Explorer - Super Star Adventures!(US).zip
{"B2LP",0x22},//2661 - Totally Spies! 2 - Undercover(EU).zip
{"BAWX",0x22},//2662 - Alex Rider - Stormbreaker(EU).zip
{"BB3P",0x00},//2663 - Barbie in the 12 Dancing Princesses(EU).zip
{"BBAP",0x23},//2664 - Shamu's Deep Sea Adventures(EU).zip
{"BBIP",0x22},//2665 - Barbie Diaries, The - High School Mystery(EU).zip
{"BNKP",0x00},//2666 - Noddy - A Day in Toyland(EU).zip
{"BXFP",0x22},//2667 - Bratz - Forever Diamondz(EU).zip
{"B82P",0x22},//2668 - Dogz(EU).zip
{"BQWE",0x00},//2669 - Strawberry Shortcake - Summertime Adventure - Special Edition(US).zip
{"BINY",0x00},//2670 - 2 Games in 1 - Finding Nemo + Incredibles, The(EU).zip
{"BW3P",0x33},//2671 - 2 Games in 1 - Sonic Advance + Chu Chu Rocket!(EU).zip
{"BPVX",0x22},//2672 - Pippa Funnell - Stable Adventure(EU).zip
{"BPRJ",0x31},//2673 - Pocket Monsters - FireRed(JP).zip
{"MCSF",0x00},//2674 - Game Boy Advance Video - Cartoon Network Collection - Edition Speciale(FR).zip
{"MTMF",0x00},//2675 - Game Boy Advance Video - Teenage Mutant Ninja Turtles - Le Demenagement(FR).zip
{"MYGF",0x00},//2676 - Game Boy Advance Video - Yu-Gi-Oh! Yugi vs. Joey(FR).zip
{"BH5F",0x22},//2677 - Nos Voisins - Les Hommes(FR).zip
{"BINX",0x00},//2678 - 2 Games in 1 - Finding Nemo + Incredibles, The(EU).zip
{"BEFP",0x22},//2679 - Best Friends - My Horse(EU).zip
{"BYPX",0x22},//2680 - Pippa Funnell 2(EU).zip
{"BUQP",0x00},//2681 - 2 Games in 1 - Uno + Skip-Bo(EU).zip
{"BSZP",0x00},//2682 - 2 Games in 1 - SpongeBob SquarePants - SuperSponge + SpongeBob SquarePants - Battle for Bikini Bottom(EU).zip
{"BLAP",0x00},//2683 - Scrabble Scramble(EU).zip
{"BJYF",0x22},//2684 - Jimmy Neutron - Un Garcon Genial - L'Attaque des Twonkies(FR).zip
{"AX4J",0x31},//2685 - Super Mario Advance 4(JP).zip
{"BNPP",0x00},//2686 - Princess Natasha(EU).zip
{"BYMP",0x00},//2687 - Monster Trucks Mayhem(EU).zip
{"B3BP",0x00},//2688 - ATV Thunder Ridge Riders(EU).zip
{"BZ6E",0x11},//2689 - Final Fantasy VI Advance(US).zip
{"B3FP",0x00},//2690 - Polly Pocket! - Super Splash Island(EU).zip
{"BALX",0x22},//2691 - All Grown Up! - Express Yourself(EU).zip
{"B4IP",0x22},//2692 - Shrek - Smash n' Crash Racing(EU).zip
{"BH5P",0x22},//2693 - Over the Hedge(EU).zip
{"BR8E",0x22},//2694 - Ghost Rider(UE).zip
{"BXGP",0x22},//2695 - 2 Games in 1 - Shrek 2 + Madagascar(EU).zip
{"BFWX",0x00},//2696 - 2 Games in 1 - Finding Nemo + Finding Nemo - The Continuing Adventures(EU).zip
{"BCJP",0x22},//2697 - Charlotte's Web(EU).zip
{"BPUF",0x22},//2698 - 2 Games in 1 - Scooby-Doo + Scooby-Doo 2 - Les Monstres Se Dechainent(FR).zip
{"BCAZ",0x22},//2699 - Cars(EU).zip
{"B9SP",0x22},//2700 - Trick Star(EU).zip
{"BZYE",0x00},//2701 - Zoey 101(US).zip
{"B3FE",0x00},//2702 - Polly Pocket! - Super Splash Island(US).zip
{"BWBI",0x22},//2703 - 2 Games in 1 - Disney Principesse + Koda, Fratello Orso(IT).zip
{"BYLP",0x22},//2704 - Kid Paddle(EU).zip
{"BNEE",0x00},//2705 - 2 Games in 1 - Finding Nemo - The Continuing Adventures + Incredibles, The(US).zip
{"BXWD",0x22},//2706 - Wilden Fussball-Kerle, Die - Gefahr im Wilde Kerle Land(DE).zip
{"BT6P",0x22},//2707 - Trollz - Hair Affair!(EU).zip
{"BUIE",0x00},//2708 - Uno Free Fall(US).zip
{"BH5H",0x22},//2709 - Over the Hedge - Beesten bij de Buren(NL).zip
{"BHXP",0x00},//2710 - Hot Wheels - All Out(EU).zip
{"AJYE",0x00},//2711 - Drake & Josh(US).zip
{"BQYD",0x22},//2712 - Danny Phantom - Dschungelstadt(DE).zip
{"BEXE",0x22},//2713 - TMNT(US).zip
{"BI7E",0x11},//2714 - Nickelodeon Vol. 1 GBA 4-Pack(US).zip
{"BEXP",0x22},//2715 - TMNT(EU).zip
{"BPUP",0x22},//2716 - 2 Games in 1 - Scooby-Doo + Scooby-Doo 2 - Monsters Unleashed(EU).zip
{"BXHP",0x22},//2717 - 2 Games in 1 - Shrek 2 + Madagascar - Operation Penguin(EU).zip
{"BDZH",0x00},//2718 - 2 Games in 1 - Monsters en Co. + Finding Nemo(NL).zip
{"BU5P",0x00},//2719 - Uno 52(EU).zip
{"BRHE",0x22},//2720 - Meet the Robinsons(US).zip
{"BRSF",0x00},//2721 - 2 Games in 1 - Les Razmoket Rencontrent les Delajungle + SpongeBob SquarePants - SuperSponge(FR).zip
{"BNBE",0x22},//2722 - Petz Vet(US).zip
{"BIME",0x22},//2723 - Dogz 2(US).zip
{"BRHP",0x22},//2724 - Meet the Robinsons(EU).zip
{"ABYP",0x00},//2725 - Britney's Dance Beat(EU).zip
{"BDOP",0x00},//2726 - Dora the Explorer - Super Star Adventures!(EU).zip
{"BZ5P",0x11},//2727 - Final Fantasy V Advance(EU).zip
{"BQLP",0x00},//2728 - March of the Penguins(EU).zip
{"MCNF",0x00},//2729 - Game Boy Advance Video - Cartoon Network Collection - Edition Platinum(FR).zip
{"BI3D",0x22},//2730 - Spider-Man 3(DE).zip
{"BROP",0x00},//2731 - Postman Pat and the Greendale Rocket(EU).zip
{"BI3E",0x22},//2732 - Spider-Man 3(US).zip
{"BI3P",0x22},//2733 - Spider-Man 3(EU).zip
{"BI3S",0x22},//2734 - Spider-Man 3(ES).zip
{"BEME",0x00},//2735 - M&M's Break' Em(US).zip
{"BCAX",0x22},//2736 - Cars(EU).zip
{"BQTX",0x22},//2737 - Mijn Dierenpension(NL).zip
{"BQVX",0x22},//2738 - Mijn Dierenpraktijk(NL).zip
{"BPVY",0x22},//2739 - Paard & Pony - Mijn Manege(NL).zip
{"BYPY",0x22},//2740 - Paard & Pony - Paard in Galop!(NL).zip
{"BI3I",0x22},//2741 - Spider-Man 3(IT).zip
{"B3HE",0x22},//2742 - Shrek the Third(US).zip
{"B2YE",0x00},//2743 - 2K Sports - Major League Baseball 2K7(US).zip
{"BNVP",0x22},//2744 - SpongeBob SquarePants and Friends - Battle for Volcano Island(EU).zip
{"BLPP",0x22},//2745 - 2 Games in 1 - Lion King, The + Disney Princess(EU).zip
{"BC5P",0x00},//2746 - Cocoto Kart Racer(EU).zip
{"BIMP",0x22},//2747 - Dogz 2(EU).zip
{"BUIP",0x00},//2748 - Uno Free Fall(EU).zip
{"BXUE",0x22},//2749 - Surf's Up(US).zip
{"BW7P",0x33},//2750 - 2 Games in 1 - Sonic Battle + Chu Chu Rocket!(EU).zip
{"B3HP",0x22},//2751 - Shrek the Third(EU).zip
{"BW8P",0x33},//2752 - 2 Games in 1 - Sonic Pinball Party + Columns Crown(EU).zip
{"BZ6P",0x11},//2753 - Final Fantasy VI Advance(EU).zip
{"BIMX",0x22},//2754 - Dogz 2(EU).zip
{"BI3F",0x22},//2755 - Spider-Man 3(FR).zip
{"BNLE",0x00},//2756 - Ratatouille(US).zip
{"BHUE",0x22},//2757 - Horsez(US).zip
{"B82X",0x22},//2758 - Dogz(FR).zip
{"BQTF",0x22},//2759 - Lea - Passion Veterinaire(FR).zip
{"BBCP",0x00},//2760 - Back to Stone(EU).zip
{"BC8P",0x00},//2761 - Cocoto - Platform Jumper(EU).zip
{"BJXE",0x22},//2762 - Harry Potter and the Order of the Phoenix(UE).zip
{"B94D",0x22},//2763 - 2 Games in 1 - Pferd & Pony - Mein Pferdehof + Pferd & Pony - Lass Uns Reiten 2(DE).zip
{"BNLY",0x00},//2764 - Ratatouille(EU).zip
{"AFZC",0x11},//2765 - Jisu F-Zero Weilai Saiche(CN).zip
{"BNLX",0x00},//2766 - Ratatouille(EU).zip
{"BXUX",0x22},//2767 - Surf's Up(EU).zip
{"BQCE",0x22},//2768 - Crash of the Titans(US).zip
{"BU7E",0x23},//2769 - Legend of Spyro, The - The Eternal Night(US).zip
{"BB5E",0x00},//2770 - Arctic Tale(US).zip
{"BBUE",0x00},//2771 - Bratz - The Movie(US).zip
{"BZNE",0x00},//2772 - Deal or No Deal(US).zip
{"BYXE",0x22},//2773 - Puppy Luv - Spa and Resort(US).zip
{"BBWE",0x00},//2774 - Avatar - The Last Airbender - The Burning Earth(US).zip
{"BI4E",0x11},//2775 - Four Pack Racing - GT Advance + GT Advance 2 + GT Advance 3 + Moto GP(US).zip
{"BUJE",0x00},//2776 - Nicktoons - Attack of the Toybots(US).zip
{"BZXE",0x00},//2777 - SpongeBob's Atlantis Squarepantis(US).zip
{"BBNE",0x22},//2778 - Barbie as the Island Princess(US).zip
{"BBUD",0x00},//2779 - Bratz - The Movie(DE).zip
{"BWBP",0x22},//2780 - 2 Games in 1 - Disney Princess + Brother Bear(EU).zip
{"BQCP",0x22},//2781 - Crash of the Titans(EU).zip
{"BBUX",0x00},//2782 - Bratz - The Movie(EU).zip
{"BNLP",0x00},//2783 - Ratatouille(EU).zip
{"B54E",0x22},//2784 - 2 Games in 1 - Crash & Spyro Super Pack - Spyro - Season of Ice + Crash Bandicoot - The Huge Adventure(US).zip
{"BXUP",0x22},//2785 - Surf's Up(EU).zip
{"BCPE",0x00},//2786 - Cars Mater-National Championship(US).zip
{"AJHE",0x11},//2787 - Petz - Hamsterz Life 2(US).zip
{"BZRE",0x22},//2788 - Enchanted - Once Upon Andalasia(US).zip
{"BUJX",0x00},//2789 - Nicktoons - Attack of the Toybots(EU).zip
{"BJ2E",0x22},//2790 - High School Musical - Livin' the Dream(US).zip
{"BIYE",0x22},//2791 - Math Patrol - The Kleptoid Threat(US).zip
{"BBWP",0x00},//2792 - Avatar - The Legend of Aang - The Burning Earth(EU).zip
{"BCPP",0x00},//2793 - Cars Mater-National Championship(EU).zip
{"BIVP",0x00},//2794 - Ignition Collection Volume 1 - Animal Snap + Super Dropzone + World Tennis Stars(EU).zip
{"BKFE",0x22},//2795 - Bee Game, The(US).zip
{"BU7P",0x23},//2796 - Legend of Spyro, The - The Eternal Night(EU).zip
{"BJPP",0x23},//2797 - Harry Potter Collection(EU).zip
{"BIRK",0x22},//2798 - Iron Kid(KS).zip
{"BEFE",0x22},//2799 - Let's Ride! - Friends Forever - USA(US).zip
{"BB8E",0x22},//2800 - Word Safari - The Friendship Totems(US).zip
{"BHBE",0x22},//2801 - Best Friends - Dogs & Cats(US).zip
{"BLJK",0x33},//2802 - Legendz - Buhwarhaneun Siryeonyi Seom(KS).zip
{"AOSE",0x22},//2803 - Samurai Deeper Kyo(US).zip
{"BGZH",0x22},//2804 - Madagascar(NL).zip
{"BLCP",0x22},//2805 - Camp Lazlo - Leaky Lake Games(EU).zip
{"BFYP",0x22},//2806 - Foster's Home for Imaginary Friends(EU).zip
{"AOGP",0x33},//2807 - Super Robot Taisen - Original Generation(EU).zip
{"BYUP",0x23},//2808 - Yggdra Union - We'll Never Fight Alone(EU).zip
{"BBUP",0x00},//2809 - Bratz - The Movie(EU).zip
{"BQ9Q",0x22},//2810 - Pixeline i Pixieland(EU).zip
{"AE2J",0x11},//2811 - Battle Network RockMan EXE 2(JP).zip
{"AHKJ",0x11},//2812 - Hikaru no Go(JP).zip
{"BASJ",0x11},//2813 - Gakuen Alice - DokiDoki Fushigi Taiken(JP).zip
{"ACOJ",0x22},//2814 - Manga-ka Debut Monogatari(JP).zip
{"A6BJ",0x11},//2815 - Battle Network RockMan EXE 3(JP).zip
{"BFDJ",0x22},//2816 - Fruit Mura no Doubutsu Tachi(JP).zip
{"AX4J",0x31},//2817 - Super Mario Advance 4(JP).zip
{"B3DJ",0x11},//2818 - Densetsu no Stafy 3(JP).zip
{"BRIJ",0x11},//2819 - Rhythm Tengoku(JP).zip

{"AWRC",0x32},//Advance_Wars_(Prototype,_iQue).zip
{"AVFC",0x11},//Densetsu_no_Stafy2_(Prototype,_iQue).zip
{"ASTC",0x11},//Densetsu_no_Stafy_(Prototype,_iQue).zip
{"BBKC",0x11},//DK_King_of_Swing_(Prototype,_iQue).zip
{"FICC",0x22},//Famicom_Mini_Collection_(Prototype,_iQue).zip
{"AFEC",0x11},//Fire_Emblem_(Prototype,_iQue).zip
{"A9QC",0x11},//Kuruin_Paradise_(Prototype,_iQue).zip
{"AKRC",0x11},//Kuru_Kuru_Kuruin_(Prototype,_iQue).zip
{"AMKC",0x32},//Mario_Kart_Super_Circuit_(Prototype,_iQue).zip
{"A88C",0x22},//Mario_Luigi_Super_Star_Saga_(Prototype,_iQue).zip
{"BIIC",0x33},//Polarium_Advance_(Prototype,_iQue).zip
{"AGLC",0x22},//Tomato_Adventure_(Prototype,_iQue).zip

{"FFFF",0x00}
};