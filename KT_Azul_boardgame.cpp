#include <iostream>
#include <time.h>

using namespace std;

struct Game { //Jatek Game strukturaja --- 3.feladat
	int korongokSzama;
	int* korongok;
	int tabla[6];
	int zsak[5];
	int eldobott[5];
};

struct Player { //Player struktura --- 7. feladat
	int fal[5][5];
	int padlo[7];
	int mintasor[5][5];
	int pontszam;
	bool kezdojatekosJelzo;
	bool gyoztes; //gyoztes megallapitasahoz kesobb kiiratasnal
};

int* korongokLetrehoz(short jatekos_szam) { //jatekosoktol fuggoen korongok letrehozasa dinamikusan --- 2.feladat
	int* korongok = new int[((jatekos_szam * 2 + 1) * 5)];
	//0-ra inicializalas
	for (int i = 0; i < ((jatekos_szam * 2 + 1) * 5); i++) {
		korongok[i] = 0;
	}
	return korongok;
}

Game jatek_letrehoz(short jatekos_szam) {
	Game game;

	game.korongokSzama = jatekos_szam * 2 + 1; //korongok szamanak kiszamitasa jatekosszamtol
	game.korongok = korongokLetrehoz(jatekos_szam);
	//tabla 0ra inicializalas
	for (int i = 0; i < 6; i++) {
		game.tabla[i] = 0;
	}
	game.tabla[5]++;
	//csempek zsakba helyezese--osszesen 100 csempe es 5 fele szin
	game.zsak[0] = 20;
	game.zsak[1] = 20;
	game.zsak[2] = 20;
	game.zsak[3] = 20;
	game.zsak[4] = 20;
	//eldobottak inicializalasa 0ra
	for (int i = 0; i < 5; i++) {
		game.eldobott[i] = 0;
	}
	return game;
}

Player jatekos_letrehoz() {
	Player player;
	//fal ertekei -1re allitas
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			player.fal[i][j] = -1;
		}
	}
	//mintasor -1
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			player.mintasor[i][j] = -1;
		}
	}
	//padlo -1
	for (int i = 0; i < 7; i++) {
		player.padlo[i] = -1;
	}
	player.pontszam = 0;//kezdopontszam beallitas
	player.kezdojatekosJelzo = false;//kezdojatekos jelzot elvesszuk
	player.gyoztes = false;
	return player;
}

void uresMintasor(Player &player, int sor) { //mintasor uresse tetele

	for (int i = 0; i < 5; i++) {
		player.mintasor[sor][i] = -1; //minden ertekhez alapbol -1et allitunk be
	}
}

char csempeHuzas(int* zsak) { //csempe huzas a zsakbol --- 1.feladat
	short random;
	do {
		random = rand() % (4 - 0 + 1) + 0; //random szam 0-4 kozott
	} while (zsak[random] > 0); //addig amig a csempe nincsen a zsakban

	zsak[random]--; //dbszam csokkentese csempeknek

	return ('A' + random); //csempe visszadasa most mar szin szerint (az adott generalt ertek hozzaadasaval)
}

bool zsakUresE(Game game) { //zsak ellenorzese hogy van e benne valami
	bool ures = true;
	for (int i = 0; i < 5; i++) {
		ures = (ures & !game.zsak[i]);
	}
	return ures;
}

void zsakFeltolt(Game& game) { //zsak feltoltese az eldobottakbol
	for (int i = 0; i < 5; i++) {
		game.zsak[i] = game.eldobott[i]; //minden csempe hozzadadasa a zsakhoz az eldobottakbol
		game.eldobott[i] = 0; //az adott csempe uresse tetele (0-val feltoltes tehat)
	}
}

char csempeHuzas(Game& game) { //csempe huzasa a zsakbol
	short random;
	do {
		if (zsakUresE(game)) {
			zsakFeltolt(game);
		}
		random = rand() % (4 - 0 + 1) + 0;
	} while (game.zsak[random] <= 0);

	game.zsak[random]--;

	return ('A' + random);
}

void korongFeltolt(Game& game) { //korongok feltoltese
	for (int i = 0; i < game.korongokSzama; i++) {
		for (int j = 0; j < 4; j++) { //4 csempe rarakashoz
			int index = i + game.korongokSzama * ((int)csempeHuzas(game) - 'A'); //csempe index szamolas
			game.korongok[index]++; //dbszam csempe noveles
		}
	}
}

bool zsakUresE(int *zsak) { //zsak uressegenek ellenorzese
	bool ures = true;
	for (int i = 0; i < 5; i++) {
		ures = (ures & !zsak[i]);
	}

	return ures;
}

void korongTablaMegjelenit(Game game) { //korongok es az asztal kozepen levo csempek megjelenitese --- 4.feladat
	cout << "**Asztal**" << endl;
	for (int i = 0; i < 5; i++) {
		if (game.tabla[i] > 0) { //ha nem ures a csempe
			cout << (char)('A' + i) << "\t" << game.tabla[i] << endl; //csempe megjelenitese mar egy adott szinkent
		}
	}
	if (game.tabla[5] != 0) { //ha a kezdojatekos jelzot meg nem kapta meg senki sem
		cout << "X" << endl; //asztalon megjelenit
	}

	for (int i = 0; i < game.korongokSzama; i++) {
		cout << "** " << i + 1 << ". Korong**" << endl;
		for (int j = 0; j < 5; j++) {
			if (game.korongok[i + game.korongokSzama * j] > 0) { //ha nem ures a korong
				char csempe = 'A' + j; //csempe adott szinre konvertalas
				cout << csempe << "\t" << game.korongok[i + game.korongokSzama * j] << endl; //minden korongon megtalalhato szinek megjelenitese
			}

		}
	}
}

short CsempeHuzasKorongrol(Game &game, char Csempe, int x) { //csempe huzasa a korongrol
	int csempek = 0;
	if (game.korongok[x + game.korongokSzama * (Csempe - 'A')] > 0) { //ha van csempe
		csempek = game.korongok[x + game.korongokSzama * (Csempe - 'A')]; //csempek szamanak rogzitese
		game.korongok[x + game.korongokSzama * (Csempe - 'A')] = 0; //csempe uresse tetele
		return csempek; //csempek szamat visszaadjuk
	}
	return -1; //kilepes -1ha nincs
}

short csempeHuzasAsztalrol(Game &game, char Csempe) { //csempek huzasa az asztalrol
	int csempek = 0;
	if (game.tabla[Csempe - 'A'] > 0) { //ha van csempe
		csempek = game.tabla[Csempe - 'A']; //szam rogzit
		game.tabla[Csempe - 'A'] = 0; //uresse tetel
		return csempek; //visszaadjuk a szamat
	}
	return -1;
}

void korongrolAsztalraFeltolt(Game &game, int x) { //csempek athelyezese a korongrol az asztalra
	for (int i = 0; i < 5; i++) {
		if (game.korongok[x + game.korongokSzama * i] > 0) { //ha van csempe
			game.tabla[i] = game.tabla[i] + game.korongok[x + game.korongokSzama * i]; //csempe athelyezese az asztalra
			game.korongok[x + game.korongokSzama * i] = 0; //csempe uresse tetel
		}
	}
}

bool mintasorEllenorzese(Player player, char szin, int minta_index) { //mintasor ellenorzese
	if (player.mintasor[minta_index][0] != -1 && player.mintasor[minta_index][0] != (szin - 'A')) { //ures-e a hely vagy tartalmazza e ugyanazt a szint
		return false; //ha nem akkor hamis
	}

	for (int i = 0; i < 5; i++) {
		if (player.fal[minta_index][i] == (szin - 'A')) { //ellenorzes hogy a falon mar van e az adott szin
			return false; //ha nincs akkor hamis
		}
	}

	return true;
}

void eldobottbaHelyez(Game& game, char szin, int dbszam) { //csempe berakasa az eldobottak koze
	if (szin > 5) {
		game.eldobott[szin - 'A'] += dbszam; //az adott index novelese a darab szamtol fuggoen
		return;
	}

	if (szin < 5)
		game.eldobott[szin] += dbszam; //az adott index novelese a darab szamtol fuggoen
}

void padlonElhelyez(Player &player, Game &game, char szin, int szamlalo) { //csempe elhelyezese a padlon
	for (int i = 0; i < 7 && szamlalo > 0; i++) {
		if (player.padlo[i] == -1) {//ha a padlovonal ures akkor csempe elhelyezese itt
			player.padlo[i] = (szin - 'A');
			szamlalo--;
		}
	}

	if (szamlalo > 0) { //ha meg mindig van csempe
		eldobottbaHelyez(game, szin, szamlalo);
	}
}

void padlonElhelyez(Player& player) { //kezdojatekos jelzo elhelyezese a padlo vonalon
	for (int i = 0; i < 7; i++) {
		if (player.padlo[i] == -1) { //ha a padlovonalon levo hely ures
			player.padlo[i] = 5; //a csempe padlovonalhoz adasa
			return;
		}
	}
}

void csempeKivalaszt(Game &game, Player &player) { //csempe kivalasztasa a korongrol/asztalrol a jatekos altal
	int valasztas = 1;
	char szin = 'A';
	int visszavonas = 1;
vissza:
	do {
		if (valasztas != 1 && valasztas != 2) { //ha valasztas nem 1 v. 2 akkor hibauzenet
			cout << "Hiba: Rossz valasztast adtal meg!" << endl;
		}
		cout << "--Menu--" << endl << "1)Asztal" << "\t2)Korongok" << endl; //menu kiiratas
		cout << "Kerlek valaszd ki honnan szeretnel csempet elvenni:";
		cin >> valasztas;
		if (cin.fail()) //rossz input kezelese
		{
			valasztas = -1;
			cout << "Kerlek szamot adj meg!" << endl;
			cin.clear(); //hiba uzenet torles
			cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); //rossz bemente eldobas
		}
	} while ((valasztas != 1 && valasztas != 2)); //amig a felhasznalo nem adott meg helyes bemenetet

	switch (valasztas)
	{
	case 1: //ha az asztalrol szeretne valasztani a felhasznalo
		do {
			if (visszavonas < 0) { //ha nincs visszavonas az azt jelenti hogy nincs olyan szinu csempe amit a felhasznalo valasztott igy ertesiteni kell
				cout << "Hiba: Nincs " << (char)szin << " szinu csempe!" << endl;
			}
			do {
				if (szin > 'E' || szin < 'A') {
					cout << "Hiba: Rossz szint adtal meg!" << endl;
				}
				cout << "Kerlek valassz szint(A-E) az asztalrol (F a vissza)!" << ":";
				cin >> szin;

				if (szin == 'F') { //F inputra visszamegy a menube
					goto vissza;
				}
			} while (szin > 'E' || szin < 'A');
			visszavonas = csempeHuzasAsztalrol(game, szin); //csempe huzas az asztalrol
			if (visszavonas > 0 && game.tabla[5] == 1) { //ha a csempe az asztalrol sikeresen visszalett rakva es meg mindig ott a kezdojatekos jelzo
				padlonElhelyez(player); //kezdojatekos jelzo elhelyezese
				player.kezdojatekosJelzo = true; //hozzaadas az adott jatekoshoz
				game.tabla[5] = 0; //majd vegul eltavolitas az asztalrol
			}
		} while (visszavonas < 0);
		break;
	case 2: //ha a felhasznalo a korongot valasztotta
		int korong = 1;
		do {
			if (korong < 1 || korong>game.korongokSzama) {
				cout << "Hiba: Rossz korongot adtal meg!" << endl;
			}
			cout << "Kerlek valaszd ki a korong sorszamat(1-" << game.korongokSzama << ") (0 a vissza):";
			cin >> korong;
			if (cin.fail())
			{
				korong = -1;
				cout << "Kerlek szamot adj meg!" << endl;
				cin.clear();
				cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); //rossz input eldobas
			}

			if (korong == 0) {
				goto vissza;
			}
		} while (korong < 1 || korong > game.korongokSzama);

		do {
			if (visszavonas < 0) {
				cout << "Hiba: Nincs " << (char)szin << " szinu csempe!" << endl;
			}
			do {
				if (szin > 'E' || szin < 'A') {
					cout << "Hiba: Rossz szint adtal meg!" << endl;
				}
				cout << "Kerlek valassz egy szint (A-E) a(z) " << korong << ". korongrol (F a vissza):";
				cin >> szin;

				if (szin == 'F') {
					goto vissza;
				}

			} while (szin > 'E' || szin < 'A');
			visszavonas = CsempeHuzasKorongrol(game, szin, korong - 1); //csempe visszarakasa az adott korongrol
		} while (visszavonas < 0);

		korongrolAsztalraFeltolt(game, korong - 1); //a korongrol az osszes egyeb csempe asztalra helyezese
		break;
	}

	bool mintasorOK = true;
	do {
		if (!mintasorOK) {
			cout << "Hiba: A csempe nem helyezheto el a valasztott mintasorra!" << endl;
		}
		do {
			if (valasztas < 0 || valasztas > 5) {
				cout << "Hiba: Rossz mintasort adtal meg!" << endl;
			}
			cout << "Kerlek valaszd ki a mintasort (1-5) ahova a csempet szeretned letenni(0 a padlovonalra helyzes):";
			cin >> valasztas;
			if (cin.fail())
			{
				valasztas = -1;
				cout << "Kerlek szamot adj meg" << endl;
				cin.clear();
				cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
			}
		} while (valasztas < 0 || valasztas > 5);
		if (valasztas == 0) {
			break;
		}
		mintasorOK = mintasorEllenorzese(player, szin, valasztas - 1); //mintasor helyessegenek ellenorzese, ha nem jo akkor ujra az egesz
	} while (!mintasorOK);

	if (valasztas == 0) { //ha a bemenet 0
		padlonElhelyez(player, game, szin, visszavonas); //osszes csempe padlo vonalra helyezese
		return;
	}

	for (int i = 0; i < valasztas && visszavonas > 0; i++) { //csempek mintasorba helyezese
		if (player.mintasor[valasztas - 1][i] == -1) {
			player.mintasor[valasztas - 1][i] = (szin - 'A');
			visszavonas--;
		}
	}

	if (visszavonas > 0) {
		padlonElhelyez(player, game, szin, visszavonas); //padlon helyezzuk el
	}

}

void jatekosKiirat(Player player) { //jatekos kiiratasa ---8.feladat

	cout << "1 2 3 4 5 ";
	cout << "Pontok:" << player.pontszam << endl; //pontok kiirasa
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cout << (char)((player.fal[i][j] > -1) ? player.fal[i][j] + 'A' : '-'); //fal kiiratas
			cout << ' ';


		}
		cout << i + 1 << " "; //szamlalo kiiratas
		for (int j = 0; j < i + 1; j++) {
			cout << (char)((player.mintasor[i][j] > -1) ? (player.mintasor[i][j] + 'A') : '-'); //mintasorok
			cout << ' ';
		}
		cout << endl;
	}

	for (int i = 0; i < 7; i++) {
		if (player.padlo[i] == 5) {
			cout << "X"; //kezdojatekos jelzo
		}
		else {
			cout << (char)((player.padlo[i] > -1) ? (player.padlo[i] + 'A') : '-'); //padlovonal
		}
		cout << "  ";
	}

	cout << "\n-1 -1 -2 -2 -2 -3 -3" << endl; //padlovonal szamozasa
}

bool asztalUresE(Game game) { //asztal ellenorzese hogy ures e
	bool ures = true;
	for (int i = 0; i < 5; i++) {
		ures = (ures & !game.tabla[i]); //megvizsgalas hogy ures e
	}

	return ures;
}

bool korongUresE(Game game) { //korong ellenorzese hogy ures e
	bool ures = true;
	for (int i = 0; i < (game.korongokSzama * 5); i++) {
		ures = (ures & !game.korongok[i]); //korong uressegenek vizsgalata
	}

	return ures;
}

bool forduloVegeE(Game game) { //ellenorzes hogy vege-e egy adott fordulonak
	return (asztalUresE(game) & korongUresE(game)); //ha az asztal es korong is urees akkor igazat adunk vissza
}

int megteltSorok(Player player) { //teli sorok szamolasa --- 11.feladat
	int sorok = 0;
	bool ures_hely = false;
	for (int i = 0; i < 5; i++) {
		ures_hely = false;
		for (int j = 0; j < 5; j++) {
			if (player.fal[i][j] == -1) { //ha ures csempet talaltunk
				ures_hely = true; //ures helyet igazra allitjuk es kilepunk a ciklusbol
				break;
			}
		}

		if (!ures_hely) { //ha nincs ures hely
			sorok++;
		}
	}

	return sorok;
}

int megteltOszlopok(Player player) { //teli oszlopok szamolasa
	int oszlopok = 0;
	bool ures_hely = false;
	for (int i = 0; i < 5; i++) {
		ures_hely = false;
		for (int j = 0; j < 5; j++) {
			if (player.fal[j][i] == -1) {
				ures_hely = true;
				break;
			}
		}

		if (!ures_hely) {
			oszlopok++;
		}
	}

	return oszlopok;
}

bool jatekVegeE(Player player[], int jszam) { //jatek ellenorzese hogy vege tert e
	bool vege = false;
	for (int i = 0; i < jszam; i++) {
		vege = (vege | (megteltSorok(player[i]) > 0 ? true : false)); //sorok ellenorzese hogy tele vagy sem, ha tele akkor igaz visszaadasa
	}

	return vege;
}

bool mintasorTeliE(int sor[], int szamlalo) { //mintasor ellenorzese hogy teli e
	for (int i = 0; i < szamlalo; i++) { //ciklus a mintasor hosszatol fuggoen
		if (sor[i] == -1) { //ha az elem ures
			return false;
		}
	}

	return true;
}

bool falEllenorzes(Player player, char szin, int oszlop) { //fal helyessegenek ellenorzese
	for (int i = 0; i < 5; i++) {
		if (player.fal[i][oszlop] == (szin - 'A')) { //ha a fal oszlopa tartalmazza ugyanazt a szint
			return false;
		}
	}

	return true;
}

int pontszam(Player& player, int sor, int oszlop) { //pontszamok szammitasa
	int szamlalo = 0;

	//ha az oszlop 0 akkor +1 hozzaadas a szamlaloba
	if (sor == 0) {
		szamlalo = 1;
	}

	for (int i = sor; i > 0; i--) { //oszlop felfele ciklus
		if (player.fal[i][oszlop] != -1) {
			szamlalo++;
			continue;
		}

		break;
	}

	for (int i = sor + 1; i < 5; i++) { //oszloptol lefele cik
		if (player.fal[i][oszlop] != -1) {
			szamlalo++;
			continue;
		}

		break;
	}

	for (int i = oszlop - 1; i > 0; i--) { //sortol balra ciklus
		if (player.fal[sor][i] != -1) {
			szamlalo++;
			continue;
		}

		break;
	}

	for (int i = oszlop + 1; i < 5; i++) { //sortol jobbra
		if (player.fal[sor][i] != -1) {
			szamlalo++;
			continue;
		}

		break;
	}
	return szamlalo;
}

void padloPont(Player &player, Game &game) { //padlovonal pontozasanak szamitasa
	int padlovonalPont[] = { -1,-1,-2,-2,-2,-3,-3 }; //padlovonal levonas pontjainak letrehozasa

	for (int i = 0; i < 7; i++) {
		if (player.padlo[i] >= 0) { //ha talalhato elem a padlovonalon
			player.pontszam += padlovonalPont[i]; //hozzaadjuk a pontszamhoz (tehat levonjuk)

			if (player.pontszam < 0) { //ha a pontszam 0 ala csokken
				for (int i = 0; i < 7; i++) {
					if (player.padlo[i] != -1 && player.padlo[i] != 5)
						eldobottbaHelyez(game, player.padlo[i], 1);
					player.padlo[i] = -1;
				}
				player.pontszam = 0; //pontszam 0 ra allitas
				return;
			}
			continue;
		}
		break;
	}

	for (int i = 0; i < 7; i++) { //padlovonal urites
		if (player.padlo[i] != -1 && player.padlo[i] != 5)
			eldobottbaHelyez(game, player.padlo[i], 1);
		player.padlo[i] = -1;
	}
}

void falonElhelyez(Game& game, Player& player) { //csempe valasztasa es falra helyezes ---9.feladat
	int oszlop = 1;
	bool ervenyes = true;
	for (int i = 0; i < 5; i++) {
		if (mintasorTeliE(player.mintasor[i], i + 1)) { //sor ellenorzese hogy teli e

			do {
				if (!ervenyes) {
					cout << "Hiba: Nem helyezheto le a csempe az adott oszlopba!" << endl;
				}
				do {
					cout << "A(z) " << i + 1 << ". mintasorbol helyezz at egy csempet a tabla egy oszlopaba(0-5) (0 a padlora helyez): ";
					cin >> oszlop;
					if (oszlop > 5 || oszlop < 0) {
						cout << "Hiba: Rossz oszlopot adtal meg!" << endl;
					}
				} while (oszlop > 5 || oszlop < 0);


				if (oszlop == 0) { //ha a valasztas 0
					padlonElhelyez(player, game, (char)(player.mintasor[i][0] + 'A'), i + 1); //csempek elhelyezese a padlovonalon 
					uresMintasor(player, i); //mintasor nullazas
					ervenyes = true;
					continue;
				}
				ervenyes = falEllenorzes(player, (char)(player.mintasor[i][0] + 'A'), oszlop - 1); //fal ellenorzese hogy ervenyes e

				if (player.fal[i][oszlop - 1] != -1) {
					ervenyes = false;
				}

				if (ervenyes) { //ha a fal ervenyes
					player.fal[i][oszlop - 1] = player.mintasor[i][0]; //akkor a mintasorbol atrakjuk a csempet a fal egy oszlopaba
					player.pontszam += pontszam(player, i, oszlop - 1); //pontszamot hozzaadjuk
					eldobottbaHelyez(game, (char)(player.mintasor[i][0] + 'A'), i); //az osszes tobbi csempet a mintasorbol az eldobottakhoz rakjuk, tehat eldobjuk...
					uresMintasor(player, i); //mintasor uresse tetele
				}
			} while (!ervenyes); //addig menjunk amig a fal ervenyes nem lesz
			jatekosKiirat(player); //jatekos allasanak kiiratasa
		}
	}
}

int kezdojatekosJelzo(Player players[], Game &game, int jszam) { //kezdojatekos jelzo csempe megkeresese hogy kinel van
	for (int i = 0; i < jszam; i++) {
		if (players[i].kezdojatekosJelzo) { //ha az adott jatekosnal van a kezdojatekost jelzo
			players[i].kezdojatekosJelzo = false; //kezdojatekos jelzo elvetele tole
			game.tabla[5] = 1; //hozzaadas az asztalhoz
			return i; //jatekosszamanak visszaadasa aki kovetkezik
		}
	}
	return 0;
}

void bonuszpontSzamolas(Player& player) { //Bonuszpontok szamolasa --- 12. feladat
	int oszlopok = megteltOszlopok(player); //teli oszlopok szamanak kiszamolasa
	int sorok = megteltSorok(player); //megtelt sorok kiszamolasa

	int szamlalo[] = { 0,0,0,0,0 }; //szamlalo 0ra beallitasa

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (player.fal[i][j] != -1) {
				szamlalo[player.fal[i][j]]++; //mindegyik csempe szamanak kiszamolasa
			}
		}
	}

	for (int i = 0; i < sorok; i++) {
		cout << "+2 bonusz pont teli sorert." << endl;;
	}
	for (int i = 0; i < oszlopok; i++) {
		cout << "+7 bonusz pont teli oszlopert." << endl;;
	}
	player.pontszam += (sorok * 2); //bonuszpont hozzaadasa a sorokert
	player.pontszam += (oszlopok * 7); //bonuszpont hozzaadasa az oszlopokert

	for (int i = 0; i < 5; i++) {
		if (szamlalo[i] >= 5) { //ha a csempe szamlalo nagyobb mint 5
			cout << "+10 bonusz pont az osszegyult 5 csempes szin miatt." << endl;
			player.pontszam += 10; //10 pont hozzaadasa a pontszamhoz
		}
	}

}

void gyoztes(Player player[], int jszam) { //nyertes meghatarozasa
	int nyert = 0; //nyertes inicializalas
	player[0].gyoztes = true;

	for (int i = 1; i < jszam; i++) {
		if (player[i].pontszam > player[nyert].pontszam) { //ha nagyobb a pontszam akkor gyoztet kicserelese
			player[nyert].gyoztes = false;
			player[i].gyoztes = true;
			nyert = i;
		}
	}

	for (int i = 0; i < jszam; i++) {
		if (player[i].pontszam == player[nyert].pontszam && i != nyert) {
			if (megteltSorok(player[nyert]) > megteltSorok(player[i])) {
				player[nyert].gyoztes = false;
				player[i].gyoztes = true;
				nyert = i;
			}
			else if (megteltSorok(player[nyert]) == megteltSorok(player[i])) {
				player[i].gyoztes = true;
				player[nyert].gyoztes = true;
				return;
			}
		}
	}
}

void jatekFelszabadit(Game& game, Player player[]) { //memoria felszabaditas
	delete[] game.korongok;
	delete[] player;
}


int main(int argc, char* argv[])
{
	srand(time(NULL));
	int jatekos_szam = -1; //minimum jatekosszam-inicializalas
	if (argc > 1) { //ha van parancssori parameter
		jatekos_szam = atoi(argv[1]); //akkor a jatekosok szamanak inicializalasa parancssorbol
		if (jatekos_szam > 4 || jatekos_szam < 2) {
			cout << "Jatekosok szamanak 2 es 4 kozott kell lennie!" << endl;
			exit(1);
		}
	}
	else {
		do {
			cout << "Kerlek add meg a jatekosok szamat: ";
			cin >> jatekos_szam; //ha parancssori paramteren nem volt akkora felhasznalotol kerdezzuk meg
			if (jatekos_szam > 4 || jatekos_szam < 2) {
				cout << "Jatekosok szamanak 2 es 4 kozott kell lennie!" << endl;
			}
		} while (jatekos_szam > 4 || jatekos_szam < 2);
	}

	int kovetkezoj = 0; //elso jatekos kovetkezik a forduloban
	Game game;
	Player* player = new Player[jatekos_szam];

	for (int i = 0; i < jatekos_szam; i++) {
		player[i] = jatekos_letrehoz(); //az osszes jatekos letrehozasa
	}
	game = jatek_letrehoz(jatekos_szam); //jatekot letrehoz


	while (!jatekVegeE(player, jatekos_szam)) { //amig a jatek veget nem er
		korongFeltolt(game); //az osszes korong megtoltese
		kovetkezoj = kezdojatekosJelzo(player, game, jatekos_szam); //ki kovetkezik az adott forduloban

		while (!forduloVegeE(game)) {  //amig az adott fordulonak nincs vege
			cout << "**A(z) " << kovetkezoj + 1 << ". jatekos kovetkezik**" << endl; //az adott jatekos szamanak kiiratasa aki kovetkezik
			jatekosKiirat(player[kovetkezoj]); //jatekos kiiratasa
			korongTablaMegjelenit(game); //korong es asztal megjelenitese
			csempeKivalaszt(game, player[kovetkezoj]); //csempe kivalasztas
			kovetkezoj++; //fordulo szamanak novelese
			if (kovetkezoj >= jatekos_szam) { //ha vege akkor
				kovetkezoj = 0; //akkor a fordulo szamanak lenullazasa
			}
		}


		cout << "\n\n-------------------------------------------------------\n";
		cout << "--------------------[Fordulo vege]--------------------\n";
		cout << "-------------------------------------------------------\n\n";

		for (int i = 0; i < jatekos_szam; i++) {
			cout << "**A(z) " << i + 1 << ". jatekos kovetkezik**" << endl; //kiiratas hogy ki kovetkezik
			jatekosKiirat(player[i]); //jatekos allas kiiratasa
			falonElhelyez(game, player[i]); //csempek elhelyezese a falra a mintasorbol
			padloPont(player[i], game); //padlovonal pont csokkentes
		}
	}

	for (int i = 0; i < jatekos_szam; i++) {
		cout << "**A(z) " << i + 1 << ". jatekos bonusz pontjai**" << endl; //kihez tartozo bonuszpontokat iratunk ki
		bonuszpontSzamolas(player[i]); //minden jatekosra ezt kiszamitani
	}

	gyoztes(player, jatekos_szam); //gyoztes meghatarozasa
	cout << "*** Eredmeny tabla ***" << endl;
	for (int i = 0; i < jatekos_szam; i++) {

		cout << i + 1 << ". jatekos " << i + 1 << ": " << player[i].pontszam << (player[i].gyoztes ? "(Nyertes)" : "") << endl; //eredmenyek kiiratasa
	}

	jatekFelszabadit(game, player);
	return 0;
}