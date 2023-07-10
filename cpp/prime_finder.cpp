// g++ -o prime_finder prime_finder.cpp -lgmp

#include <iostream>
#include <math.h>
#include <cstdlib>
#include <cmath>
#include "prime_functions_fast.h"

int mode = 2;

FILE* primefile;

int main(void) {

	if (mode == 1) {
		unsigned int lower = 58*pow(10, 6);
		unsigned int upper = 59*pow(10, 6);

		unsigned int power = lower;

		unsigned int ntrials = upper-lower;
		unsigned int counter = 0;

		while (counter < ntrials){
			printf("\n\n\n`````````````````````````````````````````````````````\n");
			printf("finding new candidate...\n");
			while (power < upper){
				power += 1;
				if (isPrime(power)){
					break;
				}
			}

			float ndigits = log10(power);
			printf("\npower is %.0f digits (%d)\n", ndigits, power);

			bool val = LucasLehmer(power);

			if (val){
				printf("\ncandidate is prime\n");
				mpz_t result, base;
				mpz_inits(result, base, NULL);

				mpz_set_str(base, "2", 10);
				mpz_pow_ui(result, base, power);
				mpz_sub_ui(result, result, 1);

				primefile = fopen("prime.txt", "a");
				fputs("\n", primefile);
				fprintf(primefile, "%u", power);
				fputs(" -- ", primefile);
				mpz_out_str(primefile, 10, result);
				fputs("\n", primefile);
				fclose(primefile);

				mpz_clears(result, base, NULL);
			}
			else{
				printf("\ncandidate is not prime\n");
			}

			if (power > upper){
				break;
			}

			counter++;
		}
	}

	else if (mode == 2){
		mpz_t lower, upper, base;
		mpz_inits(lower, upper, base, NULL);

		mpz_set_ui(base, 10);
		// mpz_pow_ui(lower, base, 5*pow(10, 2));
		// mpz_pow_ui(upper, base, 6*pow(10, 3));

		mpz_pow_ui(lower, base, 1200);
		mpz_pow_ui(upper, base, 1200);
		mpz_add_ui(upper, upper, 2);

		while (true){
			printf("\n\n\n`````````````````````````````````````````````````````\n");
			printf("checking new candidate...\n");

			size_t length = mpz_sizeinbase(lower, 10);

			printf("candidate is %lu digits\n", length);

			time_t t_to_start = time(NULL);
			if (isPrime_mpz_fast(lower)){
				printf("\ncandidate was prime\n");
				// primefile = fopen("prime.txt", "a");
				// fputs("\n", primefile);
				// mpz_out_str(primefile, 10, lower);
				// fputs("\n", primefile);
				// fclose(primefile);
			}
			time_t t_to_end = time(NULL);
			printf("(took %lds to validate)\n", t_to_end - t_to_start);

			printf("\ncandidate was not prime\n");

			mpz_add_ui(lower, lower, 1);
			int check = mpz_cmp(lower, upper);
			
			if (check > 0){ break; }
		}
	}

	return 0;
}
