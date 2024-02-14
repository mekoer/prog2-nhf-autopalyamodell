#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>

#include "palya.h"
#include "jarmu.h"

#include "memtrace.h"

bool foglalt_pos(size_t* kpos1, size_t* kpos2, size_t kpos_meret, size_t rnum) {
	for (size_t i = 0; i < kpos_meret; i++) {
		if (rnum == kpos1[i] || rnum == kpos2[i])
			return true;
	}
	return false;
}

// operator<<
std::ostream& operator<<(std::ostream& os, const Palya& palya) {
	size_t i = 0;
	while (i < palya.getPalya_hossz()) {
		bool vaneitt = false;
		for (size_t j = 0; j < palya.getJarmuvek_szama(); j++) {
			if (palya[j].getPos() == i) {
				palya[j].kiir(os, i);
				vaneitt = true;
			}
		}
		if (vaneitt == false) {
			os << std::endl << i << "	| |";
			i++;
		}
	}
	return os;
}

Palya::Palya(size_t mennyi_jarmu, size_t milyen_hosszu)
	: jarmuvek_szama(0)
	, max(mennyi_jarmu)
	, palya_hossz(milyen_hosszu) {
	if (mennyi_jarmu > milyen_hosszu)
		throw std::out_of_range("nem lehet tobb jarmu, mint amilyen hosszu a palya.");
	jarmuvek = new Jarmu * [max];
}

// tombben sorrendbe rendezi a Jarmuveket, pos adattag szerint
	// a rendezes utan beallitja az ures_elotte adattagokat is
void Palya::rendez() {
	// selection sort algoritmus
	for (size_t i = 0; i < jarmuvek_szama - 1; ++i) {
		size_t minindex = i;
		for (size_t j = i + 1; j < jarmuvek_szama; ++j)
			if (jarmuvek[j]->getPos() < jarmuvek[minindex]->getPos())
				minindex = j;
		if (minindex != i) {
			Jarmu* temp = jarmuvek[minindex];
			jarmuvek[minindex] = jarmuvek[i];
			jarmuvek[i] = temp;
		}
	}

	// set ures_elotte addattagok
	this->setures();
}

// feltolti jarmuvekkel a palyat
	// param0 - autok szama
	// param1 - sautok szama
	// param2 - kamionok szama
void Palya::feltolt(size_t autok, size_t sautok, size_t kamionok) {
	size_t jarmuvek_ossz = autok + kamionok + sautok;
	if (jarmuvek_ossz > max)
		throw std::out_of_range("ennyi jarmu nem fer a palyara.");
	
	size_t* rnum = new size_t[palya_hossz];
	for (size_t i = 0; i < palya_hossz; i++) {
		rnum[i] = i;
	}

	std::srand(std::time(0));
	std::random_shuffle(rnum, rnum + palya_hossz);

	size_t* elso_pos = new size_t[kamionok];
	size_t* masodik_pos = new size_t[kamionok];
	size_t i = 0;

	// kamionok elhelyezese
	while (jarmuvek_szama != kamionok) {
		// ha a pozicio valid es elfer egy kamion, odateszunk egyet
		// kamion ket pozicio tagjat feljegyezzuk ket tombben
		if (foglalt_pos(elso_pos, masodik_pos, jarmuvek_szama, rnum[i]) == false &&
			foglalt_pos(elso_pos, masodik_pos, jarmuvek_szama, rnum[i] - 1) == false) {
			if (rnum[i] == 0) {
				jarmuvek[jarmuvek_szama] = new Kamion(1, 0);
				elso_pos[jarmuvek_szama] = 1;
				masodik_pos[jarmuvek_szama] = 0;
				jarmuvek_szama++;
				i++;
			}
			else if (rnum[i] > 0) {
				jarmuvek[jarmuvek_szama] = new Kamion(rnum[i], (rnum[i] - 1));
				elso_pos[jarmuvek_szama] = rnum[i];
				masodik_pos[jarmuvek_szama] = rnum[i] - 1;
				jarmuvek_szama++;
				i++;
			}
		}
		else i++; // ha a pozicio mar foglalt, tovabblepunk rnum kovetkezo elemere
	}
	// autok elhelyezese
	while ((jarmuvek_szama - kamionok) != autok) {
		if (foglalt_pos(elso_pos, masodik_pos, kamionok, rnum[i]) == false) {
			jarmuvek[jarmuvek_szama] = new Auto(rnum[i]);
			jarmuvek_szama++;
			i++;
		}
		else i++;
	}
	// sportautok elhelyezese
	while ((jarmuvek_szama - kamionok - autok) != sautok) {
		if (foglalt_pos(elso_pos, masodik_pos, kamionok, rnum[i]) == false) {
			jarmuvek[jarmuvek_szama] = new Sauto(rnum[i]);
			jarmuvek_szama++;
			i++;
		}
		else i++;
	}

	this->rendez();

	delete[] rnum;
	delete[] elso_pos;
	delete[] masodik_pos;
}

// Jarmu::setures(), az osszes tarolt elemen vegrehajtva
void Palya::setures() {
	for (size_t i = 0; i < jarmuvek_szama - 1; i++) {
		jarmuvek[i]->setures(jarmuvek[i + 1]);
	}
	// leggyorsabb amivel mehet 7, igy biztosan nem ragad be a vegen
	jarmuvek[jarmuvek_szama - 1]->setUres_elotte(7);
}

// Jarmu::setseb(), az osszes tarolt elemen vegrehajtva
void Palya::setseb() {
	for (size_t i = 0; i < jarmuvek_szama; i++) {
		jarmuvek[i]->setseb();
	}
}

// Jarmu::eloremozgat(), az egesz tarolora megvalositva
void Palya::eloremozgat() {
	for (int i = (jarmuvek_szama - 1); i >= 0; --i) {
		jarmuvek[i]->eloremozgat();
		if (jarmuvek[i]->getHatsoPos() >= palya_hossz) {
			delete jarmuvek[i];
			jarmuvek_szama -= 1;
		}
	}
}

// kovetkezo allapot
void Palya::ciklus_lep(size_t hanyszor) {
	for (size_t i = 0; i < hanyszor; i++) {
		this->setures();
		this->setseb();
		this->eloremozgat();
		std::cout << *this << std::endl;
		if (jarmuvek_szama == 0) {
			break;
		}
	}
}

// szoveges fajlba menti a palya tartalmat
void Palya::ment() {
	std::fstream out;
	out.open("mentes.txt", std::ios::out);
	out << jarmuvek_szama << "," << palya_hossz << "," << std::endl;
	for (size_t i = 0; i < jarmuvek_szama; i++) {
		jarmuvek[i]->adattagok_print(out);
	}
	out.close();
}

// szoveges fajlbol betolti a palya tartalmat
void Palya::betolt() {
	std::fstream in;
	in.open("mentes.txt", std::ios::in);

	size_t mennyijarmu, milyenhosszu;
	(in >> mennyijarmu).ignore(10, ',');
	(in >> milyenhosszu).ignore(10, ',');

	palya_hossz = milyenhosszu;

	// uj palya foglalas
	Jarmu** temporary = new Jarmu * [mennyijarmu];
	for (size_t i = 0; i < jarmuvek_szama; i++) {
		delete jarmuvek[i];
	}
	delete[] jarmuvek;

	jarmuvek = temporary;
	jarmuvek_szama = 0;

	for (size_t i = 0; i < mennyijarmu; i++) {
		std::string t;
		size_t sebesseg, pozicio, pozicio2, hanyures;
		std::getline(in, t, ',');

		if (t == "\nauto") {
			(in >> sebesseg).ignore(10, ',');
			(in >> pozicio).ignore(10, ',');
			(in >> hanyures).ignore(10, ',');
			jarmuvek[i] = new Auto(sebesseg, pozicio, hanyures);
			jarmuvek_szama++;
		}
		else if (t == "\nsauto") {
			(in >> sebesseg).ignore(10, ',');
			(in >> pozicio).ignore(10, ',');
			(in >> hanyures).ignore(10, ',');
			jarmuvek[i] = new Sauto(sebesseg, pozicio, hanyures);
			jarmuvek_szama++;
		}
		else if (t == "\nkamion") {
			(in >> sebesseg).ignore(10, ',');
			(in >> pozicio).ignore(10, ',');
			(in >> pozicio2).ignore(10, ',');
			(in >> hanyures).ignore(10, ',');
			jarmuvek[i] = new Kamion(sebesseg, pozicio, pozicio2, hanyures);
			jarmuvek_szama++;
		}
	}
	in.close();
}

Palya::~Palya() {
	for (size_t i = 0; i < jarmuvek_szama; i++) {
		delete jarmuvek[i];
	}
	delete[] jarmuvek;
}

