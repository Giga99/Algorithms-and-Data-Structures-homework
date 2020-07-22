#include <iostream>
#include <string>
using namespace std;

struct Graf {
	int* edges, * indices, *brGranaCvora;
	int brCvorova = 0, brGrana = 0;
};

Graf* kreirajGraf() {
	cout << "Unesite broj cvorova grafa: ";
	int m;
	cin >> m;
	if (m > 0) {
		Graf* g = new Graf();
		g->brCvorova = m;
		g->indices = new int[g->brCvorova];
		g->brGrana = 0;
		g->edges = new int[g->brGrana];
		g->brGranaCvora = new int[g->brCvorova];
		for (int i = 0; i < g->brCvorova; i++) {
			g->indices[i] = -1;
			g->brGranaCvora[i] = 0;
		}
		return g;
	} else {
		cout << "Broj cvorova prevazilazi granicu!" << endl;
		return nullptr;
	}
}

void pomDodajGranu(Graf* g, int cvor, int cvorDodele) {
	int pom = g->brGrana;
	if (cvor + 1 == g->brCvorova) {
		if (g->indices[cvor] == -1) g->indices[cvor] = g->brGrana;
		g->edges[g->brGrana] = cvorDodele;
	} else {
		for (int i = cvor + 1; i < g->brCvorova; i++) {
			if (g->indices[i] != -1) {
				pom = g->indices[i];
				for (int j = i; j < g->brCvorova; j++) {
					if (g->indices[j] == -1) continue;
					g->indices[j]++;
				}
				break;
			}
		}
		for (int i = g->brGrana; i > pom; i--) g->edges[i] = g->edges[i - 1];
		g->edges[pom] = cvorDodele;
		if (g->indices[cvor] == -1) g->indices[cvor] = pom;
	}
	g->brGrana++;
	g->brGranaCvora[cvor]++;
}

void dodajGranu(Graf* g) {
	cout << "Izmedju koja dva cvora zelite da dodate granu? ";
	int c1, c2;
	cin >> c1 >> c2;
	if (c1 >= g->brCvorova || c1 < 0) cout << "Cvor [" << c1 << "] ne postoji u datom grafu.";
	else if (c2 >= g->brCvorova || c2 < 0) cout << "Cvor [" << c2 << "] ne postoji u datom grafu.";
	else {
		int* pomocni = new int[g->brGrana + 1];
		for (int i = 0; i < g->brGrana; i++) pomocni[i] = g->edges[i];
		g->edges = pomocni;
		pomocni = nullptr;
		pomDodajGranu(g, c1, c2);
		pomDodajGranu(g, c2, c1);
	}
}

void pomUkloniGranu(Graf* g, int cvor, int pomCvor) {
	int pom = -1;
	for (int i = g->indices[cvor]; i < g->indices[cvor] + g->brGranaCvora[cvor]; i++) {
		if (g->edges[i] == pomCvor) {
			pom = i;
			break;
		}
	}
	if (pom != -1) {
		for (int i = pom; i < g->brGrana - 1; i++) g->edges[i] = g->edges[i + 1];
		if (g->brGranaCvora[cvor] == 1) g->indices[cvor] = -1;
		for (int i = cvor + 1; i < g->brCvorova; i++) {
			if (g->indices[i] == -1) continue;
			else g->indices[i]--;
		}

		int* pomocni = new int[g->brGrana - 1];
		for (int i = 0; i < g->brGrana - 1; i++) pomocni[i] = g->edges[i];
		g->edges = pomocni;
		g->brGranaCvora[cvor]--;
		g->brGrana--;
		pomocni = nullptr;
	}
}

void ukloniGranu(Graf* g) {
	cout << "Izmedju koja dva cvora zelite da uklonite granu? ";
	int c1, c2;
	cin >> c1 >> c2;
	if (c1 >= g->brCvorova || c1 < 0) cout << "Cvor [" << c1 << "] ne postoji u datom grafu.";
	else if (c2 >= g->brCvorova || c2 < 0) cout << "Cvor [" << c2 << "] ne postoji u datom grafu.";
	else {
		pomUkloniGranu(g, c1, c2);
		pomUkloniGranu(g, c2, c1);
	}
}

void dodajCvor(Graf* g) {
	int* pomocni = new int[g->brCvorova + 1];
	for (int i = 0; i < g->brCvorova; i++) pomocni[i] = g->indices[i];
	pomocni[g->brCvorova] = -1;
	g->indices = pomocni;
	pomocni = nullptr;

	int* pomocni2 = new int[g->brCvorova + 1];
	for (int i = 0; i < g->brCvorova; i++) pomocni2[i] = g->brGranaCvora[i];
	pomocni2[g->brCvorova] = 0;
	g->brGranaCvora = pomocni2;
	pomocni2 = nullptr;

	g->indices[g->brCvorova] = -1;
	g->brCvorova++;
}

void ukloniCvor(Graf* g) {
	cout << "Unesite index cvora(0-" << g->brCvorova - 1 << "): ";
	int ind;
	cin >> ind;
	
	for (int i = 0; i < g->brCvorova; i++) {
		if (i == ind) continue;
		else {
			pomUkloniGranu(g, i, ind);
			pomUkloniGranu(g, ind, i);
			g->brGranaCvora[i]--;
		}
	}

	int* pomocni = new int[g->brCvorova - 1];
	int* pomocni2 = new int[g->brCvorova - 1];
	int k = 0;
	for (int i = 0; i < g->brCvorova; i++) {
		if (i == ind) continue;
		pomocni[k++] = g->indices[i];
		pomocni2[k] = g->brGranaCvora[i];
	}
	g->indices = pomocni;
	g->brGranaCvora = pomocni2;
	g->brCvorova--;
	pomocni = nullptr;
	pomocni2 = nullptr;
}

void ispis(Graf* g) {
	cout << "Broj cvorova je: " << g->brCvorova << ", broj grana je: " << g->brGrana << endl;
	cout << "edges: [";
	for (int i = 0; i < g->brGrana; i++)cout << g->edges[i] << " ";
	cout << "]" << endl;
	cout << "indices: [";
	for (int i = 0; i < g->brCvorova; i++)cout << g->indices[i] << " ";
	cout << "]" << endl;
}

void izbrisiGraf(Graf* g) {
	delete[] g->edges;
	delete[] g->indices;
	delete g;
}

int main() {
	Graf* graf = nullptr;

	while (true) {
		cout << "Izaberite jednu od opcija: " << endl;
		cout << "0) Kraj programa" << endl;
		cout << "1) Kreiranje grafa zadatih dimenzija" << endl;
		cout << "2) Dodavanje cvora u graf" << endl;
		cout << "3) Uklanjanje cvora iz grafa" << endl;
		cout << "4) Dodavanje grane izmedju dva cvora" << endl;
		cout << "5) Uklanjanje grane izmedju dva cvora" << endl;
		cout << "6) Ispis reprezentacije grafa" << endl;
		cout << "7) Brisanje grafa" << endl;
		cout << "Izbor: ";
		int izbor;
		cin >> izbor;
		cout << endl;

		switch (izbor) {
		case 0:
			exit(0);
		case 1:
			if (graf) {
				izbrisiGraf(graf);
				graf = nullptr;
			}
			graf = kreirajGraf();
			break;
		case 2:
			if(graf) dodajCvor(graf);
			else cout << "Graf ne postoji!" << endl;
			break;
		case 3:
			if(graf) ukloniCvor(graf);
			else cout << "Graf ne postoji!" << endl;
			break;
		case 4:
			if(graf) dodajGranu(graf);
			else cout << "Graf ne postoji!" << endl;
			break;
		case 5:
			if(graf) ukloniGranu(graf);
			else cout << "Graf ne postoji!" << endl;
			break;
		case 6:
			if (graf) ispis(graf);
			else cout << "Graf ne postoji!" << endl;
			break;
		case 7:
			if (graf) {
				izbrisiGraf(graf);
				graf = nullptr;
			} else cout << "Graf ne postoji!" << endl;
			break;
		default:
			cout << "Nedozvoljen izbor, unesite ponovo!" << endl;
			break;
		}

		cout << endl;
	}
}