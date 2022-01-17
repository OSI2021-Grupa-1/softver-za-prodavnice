# Sistem za prodavnice



![prodavnica](https://user-images.githubusercontent.com/51965240/149840781-b65e9e03-da3d-43e9-b7a6-966d25895152.png)


Sistem za upravljanje prodavnicom je jednostavna aplikacija namijenjena da olakša rad u manjim trgovinama. Jednostavna je za korištenje, a pruža niz osnovnih funkcija koje će omogućiti lakšu prodaju, ali i lakše poslovanje uopšte. Sistem za upravljanje prodavnicom daje mogućnost kreiranja računa, izvještaja, prodajnih računa, pregled lagera, razlikuje šefovei radnike itd. Baza podataka na kojoj se ovaj sistem zasniva su tekstualni fajlovi. U nastavku ovog dokumenta biće objašnjene sve funkcionalnosti koje pruža ova aplikacija kako bi se olakšalo zaposlenima pri korištenju ovog sistema.

**VAŽNO: Prije upotrebe aplikacije, neophodno je podesiti zaglavlje računa tako što će se upisati naziv, adresa i broj telefona prodavnice u fajlu** data\info.txt **i to u sljedećem formatu:**

*naziv prodavnce*

*adresa prodavnice*

*broj telefona*

**Pregled osnovnih funkcija sistema**

**Prijava na sistem**

Prijava na sistem je uobičajna. Po pokretanju aplikacije, potrebno je unijeti ispravno korisničko ime i lozinku. Po samoj instalaciji sistema, jedini način da se pokrene sistem je prijava na administratorski nalog koji je tipa *šef.* Korisničko ime, kao i lozinka glaseadmin. Pošto se korisnik na taj način prijavi, potrebno je da kreira lični nalog nakon čega se automatski briše inicijalni nalog admina.

**Kreiranje naloga**

Nove naloge može da kreira isključivo šef. Kada se šef prijavi na svoj nalog, potrebno je da izabere opciju *Pregled radnika.* Potom bira opciju *Novi nalog,* te unosi potrebne podatke: korisničko ime - mora biti jedinstveno, lozinku - mora imati najmanje 8 karaktera, potvrđuje lozinku, te bira tip naloga: šef/radnik. Pritiskom na dugme *Sačuvaj*, korisnik je uspješno kreirao nalog za novog radnika/šefa.

**Promjena lozinke**

Lozinke naloga isključivo može mijenjati šef. To se vrši na sljedeći način. Najprije, šef bira opciju *Pregleda radnika*. U ponuđenoj čeklisti, označava željeni nalog te bira opciju *Promjena lozinke.* Potrebno je da unese novu lozinku dva puta, zbog potvrde. Potom bira opciju *Prihvati.* Time je lozinka uspješno izmijenjena.

**Brisanje naloga**

Povremeno, potrebno je i obrisati pojedine naloge. Tu operaciju takođe može da vrši samo šef. Da bi se nalog uspješno obirsao, šef sve što treba da uradi jeste da odabere opciju *Pregled radnika*, označi odgovarajući nalog u ponuđenoj tabeli, te klikne na dugme *Izbriši naloge.* Postoji mogućnost i brisanja više naloga istovremeno.

**Dodavanje novog artikla**

Dodavanje novih artikala moguće je vršiti samo preko naloga šefa. Da bi se novi artikal uvrstio u bazu podataka prodavnice, neophodno je pratiti sljedeće korake. Prvo, bira se opcija *Pregled artikala,* a potom se bira *Novi artikal.* Nakon toga se unosi naziv artikla, šifra, cijena kao i količina koja se skladišti na lager. Posljednji korak je potvrda klikom na dugme *Kreiraj artikal.*

**Ažuriranje količine proizvoda na lageru**

Izmjene količine proizvoda na lageru se vrše vrlo jednostavno. Sve što šef treba da uradi jeste izabere opciju *Pregled artikala,* gdje će označiti željeni proizvod, u odgovarajuće polje unijeti novu količinu i sačuvati izmjene klikom na *Izmijeni količinu.*

**Brisanje artikala**

Brisanje artikala iz baze podataka se vrši u svega dva koraka. U meniju *Pregled artikala,* šef čekira artikal/e koje želi da obriše, te potvrđuje klikom na *Izbriši artikle.*

**Filtriranje artikala**

Korisniku je omogućeno i filtriranje artikala. U zavisnosti od toga koje artikle želi da mu budu prikazani, sve što treba da uradi jeste da u kartici *Pregled artikala* označi neki od ponuđenih filtera: *količina veća, količina manja, cijena veća, cijena manja,* te unese željenu filter vrijednost i potvrdi klikom na *primjeni.*

**Generisanje izvještaja**

Aplikacija omogućava generisanje četiri tipa izvještaja:

1. Dnevni izvještaj
1. Sedmični izvještaj
1. Mjesečni izvještaj
1. Godišnji izvještaj
1. Proizvoljni izvještaj

Svi izvještaji kreiraju se na sličan način. Odabirom opcije *Izvještaj*, šefu se omogućuje da čekira proizvode za koje želi da kreira izvještaj (to može biti jedan, više ili čak svi artikli iz baze podataka) te bira koju vrstu izvještaja želi. U zavisnosti od izvještaja koji mu je potreban, šef će trebati da unese datum, ili samo godinu za koju želi izvještaj. Po unosu traženih podataka, potvrđuje klikom na *Generiši izvještaj.* Izvještaji se čuvaju u folderu report.

**Sigurnosna kopija**

Kreiranje sigurnosne kopije se vrši vrlo jednostavno. Potrebno je samo kliknuti na opciju *Sigurnosna kopija*, pri čemu se automatski kreira folderbackup u kome se kopira baza podataka, tj. podaci o korisnicima i artiklima na stanju.

**Prodaja**

Prodaju, odnosno izdavanje računa može da obavlja radnik. Koraci pri kreiranju računa su sljedeći:

1. Klik na opciju *Prodaja artikala*
1. Označiti artikal koji je potrebno dodati na račun
1. Unijeti količinu koja je prodata
1. Klik na opciju *Dodaj artikal na račun*
1. Korake 3 i 4 ponoviti za svaki artikal koji je prodat
1. Pošto su svi artikli otkucani, račun se kreiraklikom na *Generiši račun*

Račun se čuva kao .txt fajl u posebnom folderu pod nazivomreceipts. Kreiranjem računa, automatski se ažurira dostupna količina proizvoda na stanju.

**Odjavljivanje naloga**

Pri odjavljivanju s naloga, sve što je potrebno jeste odabrati opciju *Odjava* na početnom ekranu.
