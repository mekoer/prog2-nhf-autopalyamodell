#include <iostream>

#include "jarmu.h"

#include "memtrace.h"


// return true p=0.15 valoszinuseggel 
bool csokkent_e() {
	std::srand(std::time(0));
	int r = (rand() % 100) + 1;
	return r <= 15;
}

Jarmu::Jarmu(size_t seb, size_t pos)
	: seb(seb)
	, pos(pos)
	, ures_elotte(0)
	, max_seb(seb) {}

Jarmu::Jarmu(size_t seb, size_t pos, size_t ures, size_t maxseb)
	: seb(seb)
	, pos(pos)
	, ures_elotte(ures)
	, max_seb(maxseb) {
}

// a szab�lyok szerint beallitja az adott jarmu objektum seb adattagjat
void Jarmu::setseb() {
	if (seb < max_seb)
		seb += 1;
	if (seb > ures_elotte)
		seb = ures_elotte;
	if (csokkent_e() == true)
		seb -= 1;
}