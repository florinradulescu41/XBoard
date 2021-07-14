1. Instrucțiuni de compilare:

Programul compileaza sursele la comanda make. Aceeasi comanda are ca efect si
pornirea executabilului creat, care incepe rularea Xboard. Din acest punct este
realizata decshiderea interfetei Xboard, incarcarea engine-ului si inceperea
posibilitatii de a transmite comenzi acestuia prin interfata grafica (mutari
facute pe tabla de sah).

2. Detalii despre structura proiectului:

Proiectul foloseste ca baza bitboard-urile, care sunt niste codificari ale
tablei de sah in functie de ocuparea acesteia cu piese, locuri posibile in
care acestea se pot muta sau pot ataca, pe baza de biti (1 sau 0).
Principiile acestei structuri pe baza de bitboard vor fi explicate peste
un paragraf, cand vorbim despre fisierul bitboard.cpp.

Engine-ul este compus din mai multe fisiere cu cod scris in C++. Primul dintre
ele, main.cpp, contine doar functia main, care porneste jocul.
Acest fisier foloseste game.cpp, unde sunt declarate mai multe posibilitati
de comanda pe parcursul executiei si rularii. Clasa Game contine un do-while
in cadrul caruia sunt analizate constant posibilele comezi pe care engine-ul
le poate primi si procesa (xboard, protover, new, black etc.)

Fisierul bitboard.cpp contine implementarea structurii de bitboard. Definirea
unor variabile de RANK si FILE constituie globalizarea atributelor de linii si
randuri ale tablei de sah. Bitboardurile fiind o codificare a tuturor spatiilor
de pe tabla de sah, au forma unui unsigned long long (64 de biti, 1 bit pentru
fiecare casuta a tablei). Clasa Bitboard are o enumerare ce codifica in mod
clasic (coloanele de la a la h, randurile de la 1 la 8) casutele tablei.
Functia de update are rolul de a reincarca bitboard-urile dupa executarea unei
mutari. Ea este folosita de catre setMove, care primeste o mutare pe care o 
piesa identificata in prealabil o poate face. Piesele si grupurile de piese
de acelasi fel sau culoare au bitboard-urile declarate separat. Mutarile sunt
intrepretate ca verificari si schimbari ale bitilor (shiftari) in cadrul
diferitelor bitboard-uri. Acestea sunt explicitate in fisierul pieces.cpp.

3. Detalii despre abordarea algoritmică (algoritmi folositi, complexitati)

Momentan nu e cazul. Enigine-ul asteapta comenzi de la xboard/utilizator, le
interpreteaza si raspunde. In cazul in care trebuie sa faca o mutare, foloseste
bitboard-urile pentru a vedea ce mutari poate face. In cazul pionilor, spre
exemplu, engine-ul verifica piesele (pionii) ce pot fi mutati, pozitiile
actuale ale acestora si pozitiile pe care le pot ocupa. Din cele din urma,
alege o pozitie si face o mutare valida. In cazul in care nu mai poate face
nicio mutare valida, acesta da resign si jocul se termina.

4. Surse de inspirație

Ideea de implemenatre pe baza de bitboard-uri a fost preluata de pe platforma
chessprogramming si de pe Youtube. Informatii suplimentare despre sah si lucrul
cu bitboard au fost luate si de pe WinBoard Forum, Wikipedia, chessprogramming,
etc.
