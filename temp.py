import math
import lib.prime_core as pc
import time
import random


def bounded_mersenne_search(lower, upper):
    trials = 100
    tcount = 0
    while tcount < trials:
        print('\n\n\nfinding new candidate...')
        while True:
            mpower = random.randint(lower, upper)

            _ = pc.is_Prime(0, mpower, 8)

            if _:
                break

        print('\npower is ' + str(int(math.log10( mpower ) + 1)) + ' digits (' + str(mpower) + ')')

        candidate = 2**(mpower) - 1

        print('candidate is ' + str(int(math.log10( candidate ) + 1)) + ' digits')

        prob = 2*math.log2(mpower) / mpower
        print('probability of being prime: ' + str(round(100*prob, 5)) + '%\n\n')

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


'''
t_to_start = time.monotonic()
p = pc.LucasLehmer(1, 11213, True)
t_to_end = time.monotonic()

print('\ntime using Lucas Lehmer: ' + str(round(t_to_end - t_to_start, 2)) + 's\n\n')

t_to_start = time.monotonic()
p = pc.is_Prime(0, 2**11213 - 1, 8)
t_to_end = time.monotonic()
print(p)
print('time using Miller-Rabin: ' + str(round(t_to_end - t_to_start, 2)) + 's\n\n')
'''

'''
s = 0.0011
P = 1/s
print(P*43 / (3600))
'''

print(pc.is_Prime(0, 2**21701 - 1, 8))