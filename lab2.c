#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define KRAJ_RADA 1

uint64_t MS[5] = {0ULL,0ULL,0ULL,0ULL,0ULL};
int najnoviji = 0;
int najstariji = 0;
int kraj = 0;

int ulaz[8] = {0};
int broj[8] = {0};

void udji_u_KO(int i){
  int pom, j, max = 0;

  ulaz[i] = 1;

  for (pom = 0; pom < 8; ++pom)
    if (broj[pom] > max)
      max = broj[pom];
  broj[i] = max + 1;

  ulaz[i] = 0;

  for (j = 0; j < 8; j++){
    while (ulaz[j] == 1)
      ;
    while (broj[j] != 0 &&
          (broj[j] < broj[i] || (broj[j] == broj[i] && j < i)))
      ;
  }
}

void izadji_iz_KO(int i){
  broj[i] = 0;
}


void *radna_dretva(void *rbr)
{
  int *id = rbr;
  do {
    uint64_t x = (rand() * time(NULL) * 648002905263869ULL)
                  % 18116652324202926351ULL;
    udji_u_KO(*id);
    MS[najstariji] = x;
    najnoviji = najstariji;
    if (najstariji == 4)
      najstariji = 0;
    else
      najstariji++;
    izadji_iz_KO(*id);
  } while (kraj != KRAJ_RADA);

}


void *dretva_provjera(void *rbr)
{
  int *id = rbr;
  do {
    udji_u_KO(*id);
    uint64_t y = MS[najnoviji];
    printf("ID = %d, y = %llx, uzeo broj\n", *id, y);
    izadji_iz_KO(*id);
    sleep(y % 5ULL);
    printf("ID = %d, y = %llx, potrosio broj\n", *id, y);
  } while (kraj != KRAJ_RADA);

}


int main() {

  int i, j, BR[8];
  pthread_t t[8];

  for (i = 0; i < 4; ++i) {
    BR[i] = i;
    if (pthread_create(&t[i], NULL, radna_dretva, &BR[i])) {
      printf("Ne mogu stvoriti dretvu\n");
      exit(1);
    }
  }

  sleep(2);

  for (j = 4; j < 8; ++j) {
    BR[j] = j;
    if (pthread_create(&t[j], NULL, dretva_provjera, &BR[j])) {
      printf("Ne mogu stvoriti dretvu\n");
      exit(1);
    }
  }

  sleep(30);

  kraj = KRAJ_RADA;

  return 0;
}
