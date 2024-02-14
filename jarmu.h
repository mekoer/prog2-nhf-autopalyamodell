#ifndef JARMU_H
#define JARMU_H

#include <iostream>

#include "memtrace.h"

// return true p=0.15 valoszinuseggel 
bool csokkent_e();

class Jarmu {
protected:
	size_t seb;
	size_t pos;
	size_t ures_elotte;
private:
	size_t max_seb;
public:
	Jarmu(size_t seb, size_t pos);
	Jarmu(size_t seb, size_t pos, size_t ures, size_t maxseb);

	// setter fuggvenyek
	void setSpeed(size_t newseb) { seb = newseb; }
	void setUres_elotte(size_t newures) { ures_elotte = newures; }
	void setPos(size_t newpos) { pos = newpos; }

	// getter fuggvenyek
	size_t getPos() const { return pos; }
	size_t getUres_elotte() const { return ures_elotte; }
	size_t getMax_speed() const { return max_seb; }
	size_t getSpeed() const { return seb; }

	// virtualis kiiro fv, minden jarmu mas karakterrel van jelolve
	// kamion miatt is szukseges, mert 2 egyseg hosszu
	virtual void kiir(std::ostream& os, size_t& i) const = 0;

	//virtualis fv, ami kamion eseteben az utanfuto_pos, a tobbi jarmunel pedig a pos adattagot adja vissza
	virtual size_t getHatsoPos() const { return pos; }
	
	// a szab�lyok szerint beallitja az adott jarmu objektum seb adattagjat
	void setseb();

	// eloremozgatja a jarmuvet seb cellaval
	virtual void eloremozgat() { pos += seb; }

	// ures adattagot allitja be
	void setures(Jarmu* j_utani) {
		this->setUres_elotte((j_utani->getHatsoPos()) - (this->getPos()) - 1);
	}

	// fajlba irashoz kell, az adattagokat irja egy streamre, vesszovel elvalasztva
	virtual void adattagok_print(std::ostream& os) const = 0;

	virtual ~Jarmu() {}
};

// Auto osztaly, Jarmubol szarmaztatva
// max_seb = 5, kiirasa: |o|
class Auto : public Jarmu {
public:
	Auto(size_t pos)
		: Jarmu(5, pos) {}
	Auto(size_t seb, size_t pos, size_t ures)
		: Jarmu(seb, pos, ures, 5) {}
	
	//kiiro fv
	void kiir(std::ostream& os, size_t& i) const {
		os << std::endl << i << "	|o|";
		i++;
	}

	// fajlba irashoz kell, az adattagokat irja egy streamre, vesszovel elvalasztva
	void adattagok_print(std::ostream& os) const {
		os << "auto," << seb << "," << pos << "," << ures_elotte << "," << std::endl;
	}
};

// Sauto osztaly, Jarmubol szarmaztatva
// max_seb = 7, kiirasa: |s|
class Sauto : public Jarmu {
public:
	Sauto(size_t pos)
		: Jarmu(7, pos) {}
	Sauto(size_t seb, size_t pos, size_t ures)
		: Jarmu(seb, pos, ures, 7) {
	}

	//kiiro fv
	void kiir(std::ostream& os, size_t& i) const {
		os << std::endl << i << "	|s|";
		i++;
	}

	// fajlba irashoz kell, az adattagokat irja egy streamre, vesszovel elvalasztva
	void adattagok_print(std::ostream& os) const {
		os << "sauto," << seb << "," << pos << "," << ures_elotte << "," << std::endl;
	}
};

// Kamion osztaly, Jarmubol szarmaztatva
// ket egyseg hosszu - utanfuto_pos = pos-1
// max_seb = 3, kiirasa: |k|
//						 |k|
class Kamion : public Jarmu {
	size_t utanfuto_pos;
public:
	Kamion(size_t pos, size_t pos2)
		: Jarmu(3, pos)
		, utanfuto_pos(pos2) {}
	Kamion(size_t seb, size_t pos, size_t pos2, size_t ures)
		: Jarmu(seb, pos, ures, 3)
		, utanfuto_pos(pos2) {}

	//kiiro fv
	void kiir(std::ostream& os, size_t& i) const {
		os << "\r" << i-1 << "	|k|" << std::endl << i << "	|k|";
		i++;
	}

	//virtualis fv, ami visszaadja az utanfuto_pos adattagot
	size_t getHatsoPos() const { return utanfuto_pos; }

	//kulon eloremozgat fv
	void eloremozgat() {
		pos += seb;
		utanfuto_pos += seb;
	}

	// fajlba irashoz kell, az adattagokat irja egy streamre, vesszovel elvalasztva
	void adattagok_print(std::ostream& os) const {
		os << "kamion," << seb << "," << pos << "," << utanfuto_pos << "," << ures_elotte << "," << std::endl;
	}
};

#endif