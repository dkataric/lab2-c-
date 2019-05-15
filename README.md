# Operacijski sustavi - lab2

Vježba 3: Višezadaćni rad
Upute za laboratorijske vježbe

Višezadaćni rad moguće je ostvariti s pomoću više procesa ili s pomoću više dretvi.
1. Ostvarenje višezadaćnog rada s pomoću više procesa
Zadatak treba ostvariti s dretvama te je ovaj dio informativan. Prikaži svejedno
2. Višedretvenost

Povijest višedretvenog programiranja počinje 60-tih, dok se njihova implementacija na UNIX sustavima pojavljuje sredinom 80-tih godina, a na ostalim sustavima nešto kasnije. Ideja višedretvenog programiranja jest u tome da se program sastoji od više jedinica koje se samostalno mogu izvoditi. Programer ne mora brinuti o redoslijedu njihova izvođenja, već to obavlja sam operacijski sustav. Štoviše, ukoliko je to višeprocesorski sustav, onda se neke jedinice-dretve mogu izvoditi istovremeno. Komunikacija među dretvama je jednostavna i brža u odnosu na komunikaciju među procesima, jer se obavlja preko zajedničkog adresnog prostora, te se može obaviti bez uplitanja operacijskog sustava.

Slika: Arhitektura višedretvenog sustava

Operacijski sustav za koji su predviđene ove laboratorijske vježbe jest UNIX sustav koji podržava POSIX dretve. Gornja slika prikazuje primjere procesa s jednom, dvije, tri, dvije i četiri dretve. Uobičajeno je da operacijski sustav raspoređuje dretve na raspoložive procesore te se u gornjoj slici svaka dretva vidi i u operacijskom sustavu, tj. svakoj dretvi pripada virtualni procesor, na slici označen s LWP (Light Weight Process).
Neki sustavi dozvoljavaju i djelomično upravljanje dretvama u procesima, pa tako broj dretvi u procesu može biti i veći nego što operacijski sustav vidi (pogledati poziv thr_create u Solarisu i pojmove "bound" i "unbound" dretve; "fiber"-i na Win* i sl.).
Funkcije za upravljanje dretvama
Stvaranje dretvi

Sve dretve, osim prve, inicijalne, koja nastaje stvaranjem procesa, nastaju pozivom pthread_create:

int pthread_create ( pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg );

thread je kazaljka na mjesto u memoriji gdje se sprema id novostvorene dretve (ovaj parametar ne smije biti NULL).
attr je adresa strukture koja sadrži podatke o atributima s kojima se želi stvoriti dretvu. Ako se za attr postavi NULL onda se uzimaju pretpostavljene vrijednosti (dovoljno dobre za lab. vježbe).
start_routine predstavlja pokazivač na početnu funkciju koju će novostvorena dretva imati kao početnu (npr. kao što glavna dretva ima funkciju main).
arg je adresa parametra koji se prenosi dretvi (može biti NULL ako se ništa ne prenosi). Budući da se može prenijeti samo jedan parametar u slučaju potrebe prijenosa više parametara oni se pohranjuju u strukturu te se šalje pokazivač na tu struktru.
Završetak rada dretve

Normalan završetak dretve jest njen izlazak iz prve, inicijalne funkcije, ili pozivom funkcije pthread_exit:

int pthread_exit(void *status);

status je kazaljka na stanje sa kojim dretva završava.

Dretva čeka na završetak druge dretve pozivom funkcije pthread_join:

int pthread_join( pthread_t cekana_dr, void **stanje);

cekana_dr je identifikacijski broj dretve na čiji se kraj čeka. stanje je kazaljka na kazaljku izlaznog statusa dočekane dretve. Funkcija pthread_join zaustavlja izvođenje pozivajuće dretve sve dok određena dretva ne završi sa radom. Nakon ispravnog završetka funkcija vraća nulu.

Normalni završetak višedretvenog programa zbiva se kada sve dretve završe s radom, odnosno, kada prva, početna dretva izađe iz prve funkcije (main). Prijevremeni završetak zbiva se pozivom funkcije exit od strane bilo koje dretve, ili pak nekim vanjskim signalom (SIGKILL, SIGSEGV, SIGINT, SIGTERM, ...).
Primjer višedretvenog programa

#include <stdio.h>
#include <pthread.h>

int ZajednickaVarijabla;

void *Pisac (void *x)
{
	int *n = x;
	ZajednickaVarijabla = *n;
}

void *Citac (void *x)
{
	int i;

	do {
		i = ZajednickaVarijabla;
		printf ("Procitano %d\n", i);
		sleep (1);
	}
	while (i == 0);

	printf ("Procitano je: %d\n", i);
}

int main (void)
{
	int i;
	pthread_t thr_id[2];

	ZajednickaVarijabla = 0;
	i = 123;

	/* pokretanje dretvi */
	if (pthread_create (&thr_id[0], NULL, Citac, NULL) != 0) {
		printf ("Greska pri stvaranju dretve!\n");
		exit (1);
	}
	sleep (5);
	if (pthread_create (&thr_id[1], NULL, Pisac, &i) != 0) {
		printf ("Greska pri stvaranju dretve!\n");
		exit (1);
	}

	pthread_join (thr_id[0], NULL);
	pthread_join (thr_id[1], NULL);

	return 0;
}

Identifikacijski broj dretve moguće je dobiti pozivom funkcije pthread_self:

pthread_t pthread_self (void);
Napomena
Prilikom prevođenja potrebno je postaviti zastavicu -pthread da se uključe potrebne biblioteke za višedretveni program. (npr. gcc prvi.c -o prvi -pthread).

Stranice (manual) POSIX dretvi u kojima su detaljno opisane funkcije za rad s dretvama pthread: pthread, pthread_create, pthread_exit, pthread_detach, pthread_join, pthread_mutex_init, pthread_mutex_lock, pthread_mutex_unlock, pthread_mutex_destroy, pthread_cond_init, pthread_cond_wait, pthread_cond_signal, sem_init, sem_wait, sem_post, sem_destroy...
Win32

Stvaranje procesa pod Win32 obavlja se funkcijom CreateProcess(). Primjer.

Zajednička memorija ostvaruje se pomoću funkcija CreateFileMapping i MapViewOfFile. Primjer

Stvaranje dretvi pod Win32 obavlja se funkcijom CreateThread(). Primjer.
