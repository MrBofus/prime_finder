import math
import lib.prime_core as pc
import time
import random


trials = 100
tcount = 0
while tcount < trials:
    print('\n\n\nfinding new candidate...')
    while True:
        mpower = random.randint(5*10**5, 6*10**5)

        _ = pc.is_Prime(0, mpower, 8)

        if _:
            break

    print('\npower is ' + str(int(math.log10( mpower ) + 1)) + ' digits (' + str(mpower) + ')')

    candidate = 2**(mpower) - 1

    print('candidate is ' + str(int(math.log10( candidate ) + 1)) + ' digits')

    print('probability of being prime: ' + str(round(100*math.log10(mpower)/mpower, 5)) + '%\n\n')

    t_to_start = time.monotonic()
    p = pc.LucasLehmer(1, mpower, True)

    t_to_end = time.monotonic()

    print('\n\n\n')
    print('time: ' + str(round(t_to_end - t_to_start, 2)) + 's\n\n')

    if p == -1:
        print('candidate is not prime')
    else:
        print('candidate is prime')
        with open('resources/mersenne_primes.txt', 'a') as f:
            f.write(str(p) + ' is prime\n\n')

    tcount += 1