// compile using this:
// g++ -o rrf_pfinder rrf_pfinder.cpp -lgmp

// import libraries
#include "really_really_fast.h"

unsigned int n_digits = 10;
unsigned int n_trials = 1000;
const bool verbose = false;

unsigned int n_trials_macro = 10;
unsigned int n_trials_micro = 10;

// initialize file to write primes to
FILE* primefile;

// main loop
int main(void) {

    for ( int l = 0; l < n_trials_macro; l++ ) {

        n_digits += 50;

        for (int i = 0; i < n_trials_micro; i++){

            unsigned int counter = 0;
            unsigned int n_primes = 0;

            // define and initialize random number generator
            gmp_randstate_t rstate;
            gmp_randinit_mt(rstate);

            // define and initialize random seed for rng
            srand((unsigned int)time(NULL));
            unsigned long seed = rand()%100000000000;
            gmp_randseed_ui(rstate, seed);

            if (verbose){cout << "generated seed..." << endl;}

            mpz_t upper, base, random;
            mpz_inits(upper, base, random, NULL);

            // intialize 'base' to 10
            mpz_set_ui(base, 10);

            // define upper bound values given number of digits
            mpz_pow_ui(upper, base, n_digits);
            
            // generate random number in range
            mpz_urandomm(random, rstate, upper);



            //start timer
            auto start = high_resolution_clock::now();
            if (verbose){cout << "started timer..." << endl;}

            while (counter < n_trials){

                if (verbose){
                    cout << "\nchecking candidate" << endl;
                    cout.flush();
                }

                if (isPrime_mpz_fast(random, rstate, verbose)) {
                    if (verbose){
                        cout << "\ncandidate was prime\n" << endl;
                        cout.flush(); 
                    }
                    /*
                    primefile = fopen("p_out.txt", "a");
                    fputs("\n", primefile);
                    mpz_out_str(primefile, 10, random);
                    fputs("\n", primefile);

                    fclose(primefile);
                    */

                    n_primes++;
                } else{
                    if (verbose){
                        cout << "\t-----> test failed" << endl;
                        cout << "\ncandidate was not prime\n" << endl;
                        cout.flush();
                }
                }

                // increment candidate by one
                mpz_add_ui(random, random, 1);
                counter++;
            }

            if (verbose){cout << "finished" << endl;}
            
            // stop timer
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);

            if (verbose){cout << "\ncode took " << duration.count()/1000 << "ms to check " << n_trials << " candiates, finding " << n_primes << "\n\t(rate of " << 1000000*n_trials/duration.count() << " trials per second)" << endl;}
            else {cout << n_digits << ",\t" << duration.count() << endl;}

            primefile = fopen("t_out.txt", "a");
            fprintf(primefile, "%d,\t%ld\n", n_digits, duration.count());
            fclose(primefile);
            

            gmp_randclear(rstate);
            mpz_clears(upper, base, random, NULL);
        }
    }
     
    return 0;
}