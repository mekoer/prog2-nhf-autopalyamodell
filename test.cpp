#define TESZTEK

#include <iostream>
#include <exception>

#include "palya.h"
#include "jarmu.h"
#include "gtest_lite.h"

#include "memtrace.h"

using std::cout;
using std::cin;
using std::endl;

// ha a TESZTEK makro definialva van
#ifdef TESZTEK
void tesztek() {
	TEST(Jarmu, setures)
		Auto a(5);
		Auto b(10);
		Jarmu* j = &b;
		a.setures(j);
		EXPECT_EQ(4, a.getUres_elotte()) << "nem jol allit be" << endl;
		Kamion k(10, 9);
		j = &k;
		a.setures(j);
		EXPECT_EQ(3, a.getUres_elotte()) << "kamion eseten nem jol allit be" << endl;
	END
		TEST(Palya, konstruktor)
		EXPECT_THROW(Palya p(10, 9), std::out_of_range);
		EXPECT_NO_THROW(Palya p(9, 10));
	END
		TEST(Palya, rendez)
		Palya p(3, 3);
		p.feltolt(2, 1, 0);
		EXPECT_GT(p[1].getPos(), p[0].getPos()) << "rendezes utan nem jok a poziciok" << endl;
		EXPECT_GT(p[2].getPos(), p[1].getPos()) << "rendezes utan nem jok a poziciok" << endl;
	END
		TEST(Palya, index)
		Palya p(10, 10);
		p.feltolt(2, 2, 3);
		EXPECT_THROW(p[10], std::out_of_range);
		EXPECT_NO_THROW(p[4]);
	END
		TEST(Jarmu, kiir)
		Palya p(3, 3);
		p.feltolt(3, 0, 0);
		std::stringstream str;
		str << p;
		EXPECT_STREQ("\n0	|o|\n1	|o|\n2	|o|", str.str().c_str());
	END
		TEST(Jarmu, eloremozgat)
		Palya p(1, 10);
		p.feltolt(1, 0, 0);
		p[0].setPos(0);
		p[0].setSpeed(5);
		p[0].setUres_elotte(6);
		p.eloremozgat();
		EXPECT_EQ(5, p[0].getPos());
	END
		TEST(Jarmu, setseb)
		Palya p(2, 10);
		p.feltolt(2, 0, 0);
		p[0].setPos(0);
		p[1].setPos(3);
		p.setures();
		p.setseb();
		// <= viszonyra tesztelo makroval kell a 15% esellyel bekovetkezo (sebesseg - 1) lassitas miatt
		EXPECT_GE(2, p[0].getSpeed());
	END
	TEST(Palya, ment)
		Palya p(1, 2);
		p.feltolt(1, 0, 0);
		p.ment();
		std::stringstream str1;
		str1 << p;
		Palya q(1, 2);
		q.betolt();
		std::stringstream str2;
		str2 << q;
		EXPECT_STREQ(str1.str().c_str(), str2.str().c_str());
	END
}
#endif

int main(void) {
#ifdef TESZTEK
	tesztek();
#endif
	bool main = true;
	while (main) {
		size_t kamionok = 0;
		size_t autok = 0;
		size_t sautok = 0;
		size_t milyenhosszu = 0;

		bool adatok_ker = true;
		while (adatok_ker) {
			cout << "hany kamion legyen a palyan? (max seb: 3, hossz: 2 cella)" << endl;
			cin >> kamionok;

			cout << "hany auto legyen a palyan? (max seb: 5, hossz: 1 cella)" << endl;
			cin >> autok;

			cout << "hany sportauto legyen a palyan? (max seb: 7, hossz: 1 cella)" << endl;
			cin >> sautok;
			size_t palya_suggest = kamionok * 3 + autok + sautok;
			cout << "milyen hosszu legyen az autopalya? ajanlott: " << palya_suggest << endl;
			cin >> milyenhosszu;

			try {
				Palya palya(kamionok + autok + sautok, milyenhosszu);
				palya.feltolt(autok, sautok, kamionok);
			}
			catch (std::out_of_range& err) {
				std::cerr << err.what() << endl;
				continue;
			}
			adatok_ker = false;
		}

		Palya sim(kamionok + autok + sautok, milyenhosszu);
		sim.feltolt(autok, sautok, kamionok);

		cout << "lep == leptetes | uj == ej palya generalasa | ment == mentes | tolt == betoltes | stop == kilepes, a program bezarasa";
		cout << sim << endl;

		bool fut = true;
		while (fut) {
			if (sim.getJarmuvek_szama() == 0) {
				fut = false;
			}
			else {
				std::string temp;
				cin >> temp;
				if (temp == "lep") {
					sim.ciklus_lep(1);
				}
				else if (temp == "uj")
					fut = false;
				else if (temp == "ment") {
					sim.ment();
					cout << "sikeres mentes" << endl;
				}
				else if (temp == "tolt") {
					sim.betolt();
					cout << "a betoltes sikeres" << endl << sim << endl;
				}
				else if (temp == "stop") {
					main = false;
					fut = false;
				}
				else
					cout << "ilyen parancs nincs, probald ujra" << endl;
			}
		}
	}
	
}