#include <iostream>
#include "prime_functions_fast.h"

FILE* primefile = fopen("factors2.txt", "a");

int factorize(mpz_t prime, unsigned int p, int min_digits) {
    int count = 0;

    mpz_t n, comparator, iterator, square_root;
    mpz_inits(n, comparator, iterator, square_root, NULL);
    mpz_set(n, prime);
    mpz_set(comparator, n);
 
    // count the number of times 2 divides
    mpz_mod_ui(comparator, n, 2);
    while ( mpz_cmp_ui(comparator, 0) == 0 ){
        mpz_rshift(n, n, 1); // equivalent to n=n/2;
        count++;

        mpz_set(comparator, n);
        mpz_mod_ui(comparator, n, 2);
    }
 
    // if 2 divides it
    if (count){
        fputs("2 ", primefile);
        fprintf(primefile, "%u", count);
        fputs("\n", primefile);
    }

    if (mpz_cmp_ui(n, 0) < 0){
        mpz_clears(n, comparator, iterator, square_root, NULL);
        return 0;
    }
 
    mpz_set(comparator, n);
    mpz_sqrt(square_root, n);
    mpz_set_ui(iterator, 3);

    gmp_printf("\nsqrt: %Zd\n", square_root);

    // check for all the possible numbers that can
    // divide it
    while (mpz_cmp(iterator, square_root) <= 0) {
        count = 0;

        mpz_mod(comparator, n, iterator);
        while (mpz_cmp_ui(comparator, 0) == 0) {
            count++;
            mpz_div(n, n, iterator);
            mpz_set(comparator, n);
            mpz_mod(comparator, n, iterator);
        }

        if (count) {
            mpz_out_str(primefile, 10, iterator);
            fputs(" ", primefile);
            fprintf(primefile, "%u", count); 
            fputs("\n", primefile);
        }

        mpz_add_ui(iterator, iterator, 2);
        // gmp_printf("\n\ti: %Zd\n", iterator);
        
    }

    // if n at the end is a prime number.
    if ( mpz_cmp_ui(n, 2) > 0 ){
        size_t length = mpz_sizeinbase(n, 10);
        
        if (length < min_digits) {
            mpz_out_str(primefile, 10, n);
            fputs(" 1\n", primefile);

            cout << "\n found one -- " << p << endl;
            // gmp_printf ("%Zd 1\n", n);
        }
        else{
            fputs("factor was too long\n", primefile);
        }
    }
    else {cout << "\n found one -- " << p << endl;}
    
    mpz_clears(n, comparator, iterator, square_root, NULL);
    return 0;
}

int main(void) {
    mpz_t prime;
    mpz_init(prime);
    mpz_set_str(prime, "68962976413139755920247180083386124099306029227254639390034087133122698476038899405648051152010180385196222379115745747601177582190809384624580283396712079482723031230055682081008125280227016444245060322135574374879544587319737945411626829554466241498089958349138928737254449673609860236277213556956579972419893707916373267686652267474524331385721098922258420675183869143728687016041470689134606311745448768489060785960578801646863671063462836866693016399556920034910197002290426575770910585067291840009009727559898079268928120548732946532965388791481050539263790419948018979628374063846821133465347639799290064811971390101294185764488663243237586884809133191279158443140789367577796305335794849524211918189124948128596459092876693730082639431767054405568318967469610513480675176500817351935391268536848016393426351701472955085956648847548050738970980470235009341845571004150199201976104130364730806353513804099203737182578574858435392017886610668671586039596549188187457377959082330131770452675952857", 10);

    for (unsigned int i = 1; i < 1000; i++) {
        cout << "\rtrying f = " << i;
        cout.flush();

        fputs("\n`````````````````````````````````````\n", primefile);
        fprintf(primefile, "%d", i);
        fputs("\n`````````````````````````````````````\n", primefile);
        
        mpz_sub_ui(prime, prime, 1);
        int j = factorize(prime, i, 5);

        fputs("\n`````````````````````````````````````\n", primefile);
    }

    cout << "\n";

    fclose(primefile);
    mpz_clear(prime);
    return 0;
}