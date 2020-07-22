#include <iostream>
#include <string>
using namespace std;

struct Cvor {
	int x, y;
	bool posecen = false, gore = false, dole = false , levo = false , desno = false, ulaz = false, izlaz = false;
};

struct Graf {
	int* edges, * indices, * brGranaCvora;
	int brCvorova = 0, brGrana = 0;
};

struct CvorRes {
	int x, y;
	CvorRes* sled;
	CvorRes(int xx, int yy, CvorRes* s = nullptr) : x(xx), y(yy), sled(s) {}
};

Cvor** kreiranjeLavirinta(int* row, int* column) {
	cout << "Unesite broj redova i kolona lavirinta(Broj redova ne sme biti veci od 80, a broj kolona ne veci od 50): ";
	int r, c;
	cin >> r >> c;

	if (r > 0 && r <= 80 && c > 0 && c <= 50) {
		cout << "Unesite poziciju ulaza(Pozicija mora biti na ivici lavirinta): ";
		int xU, yU;
		cin >> xU >> yU;

		while (1) {
			if ((xU == 0 || xU == r - 1) || (yU == 0 || yU == c - 1)) break;
			else {
				cout << "Unesite ponovo poziciju ulaza(Pozicija mora biti na ivici lavirinta): ";
				cin >> xU >> yU;
			}
		}

		cout << "Unesite poziciju izlaza(Pozicija mora biti na ivici lavirinta): ";
		int xI, yI;
		cin >> xI >> yI;

		while (1) {
			if ((xI == 0 || xI == r - 1) || (yI == 0 || yI == c - 1)) break;
			else {
				cout << "Unesite ponovo poziciju izlaz(Pozicija mora biti na ivici lavirinta): ";
				cin >> xI >> yI;
			}
		}

		*row = r;
		*column = c;
		Cvor** l = new Cvor * [r];
		for (int i = 0; i < r; i++) {
			l[i] = new Cvor[c];
			for (int j = 0; j < c; j++) {
				if (xU == i && yU == j) l[i][j].ulaz = true;
				if (xI == i && yI == j) l[i][j].izlaz = true;
				l[i][j].x = i;
				l[i][j].y = j;
			}
		}
		
		while (true) {
			cout << "Unesite koordinate prolaza izmedju cvorova((-1, -1) za prekid, cvorovi moraju biti jedan do drugog): " << endl;
			cout << "Prvi cvor: ";
			int x1, y1;
			cin >> x1 >> y1;

			cout << "Drugi cvor: ";
			int x2, y2;
			cin >> x2 >> y2;

			if(x1 == -1 && y1 == -1 || x2 == -1 && y2 == -1) break;
			else if ((x2 - x1) == 1 && (y2 - y1) == 0) {
				l[x2][y2].gore = true;
				l[x1][y1].dole = true;
			} else if ((x2 - x1) == -1 && (y2 - y1) == 0) {
				l[x2][y2].dole = true;
				l[x1][y1].gore = true;
			} else if ((x2 - x1) == 0 && (y2 - y1) == 1) {
				l[x2][y2].levo = true;
				l[x1][y1].desno = true;
			} else if ((x2 - x1) == 0 && (y2 - y1) == -1) {
				l[x2][y2].desno = true;
				l[x1][y1].levo = true;
			}

		}

		return l;
	} else {
		cout << "Velicine nisu dobre!" << endl;
		return nullptr;
	}
}

void pomDodajGranu(Graf* g, int cvor, int cvorDodele) {
	g->brGrana++;
	g->brGranaCvora[cvor]++;
	int* pomocni = new int[g->brGrana];
	for (int i = 0; i < g->brGrana; i++) pomocni[i] = g->edges[i];
	g->edges = pomocni;
	pomocni = nullptr;

	int pom = g->brGrana - 1;
	if (cvor + 1 == g->brCvorova) {
		if (g->indices[cvor] == -1) g->indices[cvor] = g->brGrana - 1;
		g->edges[g->brGrana - 1] = cvorDodele;
	} else {
		for (int i = cvor + 1; i < g->brCvorova - 1; i++) {
			if (g->indices[i] != -1) {
				pom = g->indices[i];
				for (int j = i; j < g->brCvorova - 1; j++) {
					if (g->indices[j] == -1) continue;
					g->indices[j]++;
				}
				break;
			}
		}
		for (int i = g->brGrana - 1; i > pom; i--) g->edges[i] = g->edges[i - 1];
		g->edges[pom] = cvorDodele;
		if (g->indices[cvor] == -1) g->indices[cvor] = pom;
	}
}

Graf* kreiranjeLavirinta2(int r, int c) {
	Graf* lav2 = new Graf();
	lav2->brCvorova = r*c;
	lav2->indices = new int[lav2->brCvorova];
	lav2->edges = new int[lav2->brGrana];
	lav2->brGranaCvora = new int[lav2->brCvorova];
	for (int i = 0; i < lav2->brCvorova; i++) {
		lav2->indices[i] = -1;
		lav2->brGranaCvora[i] = 0;
	}

	while (true) {
		cout << "Unesite koordinate prolaza izmedju cvorova((-1, -1) za prekid, cvorovi moraju biti jedan do drugog): " << endl;
		cout << "Prvi cvor: ";
		int x1, y1;
		cin >> x1 >> y1;

		cout << "Drugi cvor: ";
		int x2, y2;
		cin >> x2 >> y2;

		if (x1 == -1 && y1 == -1 || x2 == -1 && y2 == -1) break;
		else if (((x2 - x1) == 1 && (y2 - y1) == 0) || ((x2 - x1) == -1 && (y2 - y1) == 0)
			|| ((x2 - x1) == 0 && (y2 - y1) == 1) || ((x2 - x1) == 0 && (y2 - y1) == -1)) {

			pomDodajGranu(lav2, c * x1 + y1, c * x2 + y2);
			pomDodajGranu(lav2, c * x2 + y2, c * x1 + y1);
		}

	}

	return lav2;
}

void resiLavirint(Cvor** l, Graf* l2, int r, int c) {
	int xU, yU, xI, yI;

	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			if (l[i][j].ulaz) {
				xU = i;
				yU = j;
			} else if (l[i][j].izlaz) {
				xI = i;
				yI = j;
			}
		}
	}

	CvorRes** resenje = new CvorRes * [100];
	Cvor* queue = new Cvor[r * c];
	int numResenja = 0, numQueue = 0, indexQueue = 0;

	l[xU][yU].posecen = true;
	queue[numQueue++] = l[xU][yU];
	resenje[numResenja++] = new CvorRes(xU, yU);

	while (numQueue != indexQueue) {
		Cvor pom = queue[indexQueue++];
		int raskr = 0;

		if (pom.desno && !l[pom.x][pom.y + 1].posecen) {
			l[pom.x][pom.y + 1].posecen = true;
			queue[numQueue++] = l[pom.x][pom.y + 1];
			raskr++;
		}		
		if (pom.levo && !l[pom.x][pom.y - 1].posecen) {
			l[pom.x][pom.y - 1].posecen = true;
			queue[numQueue++] = l[pom.x][pom.y - 1];
			raskr++;
		}
		if (pom.dole && !l[pom.x + 1][pom.y].posecen) {
			l[pom.x + 1][pom.y].posecen = true;
			queue[numQueue++] = l[pom.x + 1][pom.y];
			raskr++;
		}
		if (pom.gore && !l[pom.x - 1][pom.y].posecen) {
			l[pom.x - 1][pom.y].posecen = true;
			queue[numQueue++] = l[pom.x - 1][pom.y];
			raskr++;
		}

		if (raskr > 1) {
			CvorRes* pom2 = resenje[numResenja - 1];
			while (pom2->sled) pom2 = pom2->sled;
			pom2->sled = new CvorRes(pom.x, pom.y);
			CvorRes* tek = resenje[numResenja++] = new CvorRes(xU, yU);
			pom2 = resenje[numResenja - 2]->sled;
			while (pom2) {
				tek->sled = new CvorRes(pom2->x, pom2->y);
				pom2 = pom2->sled;
				tek = tek->sled;
			}
			tek->sled = new CvorRes(queue[numQueue - 1].x, queue[numQueue - 1].y);
			pom2 = new CvorRes(queue[numQueue - 2].x, queue[numQueue - 2].y);
		} else {
			for (int i = 0; i < numResenja; i++) {
				CvorRes* tek = resenje[i];
				while (tek->sled) tek = tek->sled;
				if ((tek->x == pom.x && tek->y - pom.y == 1 && pom.desno) || (tek->x == pom.x && tek->y - pom.y == -1 && pom.levo)
					|| (tek->x - pom.x == 1 && tek->y == pom.y && pom.dole) || (tek->x - pom.x == -1 && tek->y == pom.y && pom.gore)) {

					tek->sled = new CvorRes(pom.x, pom.y);
				}
			}
		}
	}

	int res = -1;
	for (int i = 0; i < numResenja; i++) {
		CvorRes* tek = resenje[i];
		while (tek) {
			if (tek->x == xI && tek->y == yI) {
				res = i;
			}
			tek = tek->sled;
		}
	}

	if (res == -1) cout << "Lavirint nema resenja!" << endl;
	else {
		CvorRes* tek = resenje[res];
		while (tek) {
			cout << '(' << tek->x << ", " << tek->y;
			if (tek->x == xI && tek->y == yI) cout << ')';
			else cout << ")->";
			tek = tek->sled;
		}
		cout << endl;
	}

	for (int i = 0; i < numResenja; i++) {
		CvorRes *tek = resenje[i];
		while (tek) {
			CvorRes* stari = tek;
			tek = tek->sled;
			delete stari;
		}
	}

	delete[] resenje;
}

void ispis(Cvor** l, Graf* l2, int r, int c) {
	bool unetU = true, unetI = true;
	for (int i = 0; i < c; i++) {
		if (l[0][i].ulaz && i != 0) {
			cout << " o";
			unetU = false;
		} else if (l[0][i].izlaz && i != 0) { 
			cout << " x";
			unetI = false;
		} else cout << " _";
	}
	cout << endl;
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			if (l[i][j].levo) {}
			else if (l[i][j].ulaz && j == 0 && unetU) {
				cout << 'o'; 
				unetU = false;
			} else if (l[i][j].izlaz && j == 0 && unetI) {
				cout << 'x';
				unetI = false;
			} else if (j == 0) cout << '|';

			if (l[i][j].dole) cout << ' ';
			else if (l[i][j].ulaz && unetU && i == r - 1) {
				cout << 'o';
				unetU = false;
			} else if (l[i][j].izlaz && unetI && i == r - 1) {
				cout << 'x';
				unetI = false;
			} else if (!l[i][j].dole) cout << '_';

			if (l[i][j].desno) cout << ' ';
			else if (l[i][j].ulaz && unetU && j == c - 1) {
				cout << 'o';
				unetU = false;
			} else if (l[i][j].izlaz && unetI && j == c - 1) {
				cout << 'x';
				unetI = false;
			} else if (!l[i][j].desno) cout << '|';
		}
		cout << endl;
	}

	cout << endl;

	cout << "Broj cvorova je: " << l2->brCvorova << ", broj grana je: " << l2->brGrana << endl;
	cout << "edges: [";
	for (int i = 0; i < l2->brGrana; i++)cout << l2->edges[i] << " ";
	cout << "]" << endl;
	cout << "indices: [";
	for (int i = 0; i < l2->brCvorova; i++)cout << l2->indices[i] << " ";
	cout << "]" << endl;
}

void izbrisiLavirint(Cvor** l, Graf* l2, int r, int c) {
	for (int i = 0; i < r; i++) delete[] l[i];
	delete[] l;
	delete[] l2->brGranaCvora;
	delete[] l2->indices;
	delete l2;
}

int main(){
	Cvor** lavirint = nullptr;
	Graf* lavirint2 = nullptr;
	int row, column;

	while (true) {
		cout << "Izaberite jednu od opcija: " << endl;
		cout << "0) Kraj programa" << endl;
		cout << "1) Kreiranje lavirinta zadatih dimenzija" << endl;
		cout << "2) Resavanje lavirinta" << endl;
		cout << "3) Ispis lavirinta" << endl;
		cout << "4) Brisanje lavirinta" << endl;
		cout << "Izbor: ";
		int izbor;
		cin >> izbor;
		cout << endl;

		switch (izbor) {
		case 0:
			exit(0);
		case 1:
			lavirint = kreiranjeLavirinta(&row, &column);
			if (lavirint) lavirint2 = kreiranjeLavirinta2(row, column);
			break;
		case 2:
			if (lavirint && lavirint2) resiLavirint(lavirint, lavirint2, row, column);
			else cout << "Lavirint ne postoji!" << endl;
			break;
		case 3:
			if (lavirint && lavirint2) ispis(lavirint, lavirint2, row, column);
			else cout << "Lavirint ne postoji!" << endl;
			break;
		case 4:
			if (lavirint && lavirint2) {
				izbrisiLavirint(lavirint, lavirint2, row, column);
				lavirint = nullptr;
				lavirint2 = nullptr;
			} else cout << "Lavirint ne postoji!" << endl;
			break;
		default:
			cout << "Nedozvoljen izbor, unesite ponovo!" << endl;
			break;
		}

		cout << endl;
	}
}