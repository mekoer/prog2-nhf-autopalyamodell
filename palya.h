#ifndef PALYA_H
#define PALYA_H

#include <algorithm>
#include <fstream>
#include <iostream>

#include "jarmu.h"

#include "memtrace.h"


// igaz, ha rnum pozicio meg nem foglalt egy kamion altal
bool foglalt_pos(size_t* kpos1, size_t* kpos2, size_t kpos_meret, size_t rnum);

class Palya;
std::ostream& operator<<(std::ostream& os, const Palya& palya);

class Palya {
	size_t jarmuvek_szama;
	size_t max;
	size_t palya_hossz;
	Jarmu** jarmuvek;
public:
	Palya(size_t mennyi_jarmu, size_t milyen_hosszu);
	
	//getter fuggvenyek
	size_t getPalya_hossz() const { return palya_hossz; }
	size_t getJarmuvek_szama() const { return jarmuvek_szama; }
	size_t getMax() const { return max; }

	//setter fv
	void setJarmuvek_szama(size_t newszama) { jarmuvek_szama = newszama; }

	// indexelo operator overload
	Jarmu& operator[](size_t idx) {
		if (idx > jarmuvek_szama - 1)
			throw std::out_of_range("hibas index");
		return *(jarmuvek[idx]);
	}
	Jarmu& operator[](size_t idx) const {
		if (idx > jarmuvek_szama - 1)
			throw std::out_of_range("hibas index");
		return *(jarmuvek[idx]);
	}

	// tombben sorrendbe rendezi a Jarmuveket, pos adattag szerint
	// a rendezes utan beallitja az ures_elotte adattagokat is
	void rendez();

	// feltolti jarmuvekkel a palyat
	// param0 - autok szama
	// param1 - sautok szama
	// param2 - kamionok szama
	void feltolt(size_t autok, size_t sautok, size_t kamionok);

	// Jarmu::setures(), az osszes tarolt elemen vegrehajtva
	void setures();
	
	// Jarmu::setseb(), az osszes tarolt elemen vegrehajtva
	void setseb();

	// Jarmu::eloremozgat(), az egesz tarolora megvalositva
	void eloremozgat();

	// szimulacios ciklus futtatasa
	// param0 - ennyi ciklus fog lefutni
	void ciklus_lep(size_t hanyszor);

	// szoveges fajlba menti a palya tartalmat
	void ment();

	// szoveges fajlbol betolti a palya tartalmat
	void betolt();

	~Palya();
};

#endif