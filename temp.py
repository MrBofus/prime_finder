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
s = 1/12000
P = 1/s
print(P*12 / (3600))
'''

# p = int(10**10000 - 1)
# print(pc.is_Prime(0, 101, 8))
'''
with open('resources/mersenne_primes.txt', 'a') as f:
    f.write(str(p) + ' is prime\n\n')
'''

print('starting...')
prime_candidate = 0

print( int(math.log10(prime_candidate) + 1) )

# p = int(10**10000 - 1)
print(pc.is_Prime(0, 
                  prime_candidate, 
                  8))
'''

with open('cpp/quadruplet_search_2.txt') as f:
    lines = f.readlines()

# split strings into list of primes
primelist = []
for i in range(0, len(lines), 2): 
    primelist.append(int(lines[i]))

# iterate through each prime stored in the list of primes
pcounter = 0
for i in range(1, len(primelist)):
    # if there is a twin prime, append it to the twin primes text file
    if primelist[i] - primelist[i-1] == 2:
        
        print('twin prime found')
        with open('resources/q_twin_primes.txt', 'a') as f:
            f.write('```````````````````\ntwin primes:\n\n')
            f.write(str(primelist[i-1]) + '\n\n')
            f.write(str(primelist[i]) + '\n\n')
            f.write('```````````````````\n\n\n\n')
    
    
    
    if pcounter > 0:
        # if there is a triplet prime, append it to the twin primes text file
        if ((primelist[i] - primelist[i-1] == 4 and 
                primelist[i] - primelist[i-2] == 6) or 
                (primelist[i] - primelist[i-1] == 2 and 
                    primelist[i] - primelist[i-2] == 6)):
            
            print('triplet prime found')
            with open('resources/q_twin_primes.txt', 'a') as f:
                f.write('```````````````````\ntriplet primes:\n\n')
                f.write(str(primelist[i-2]) + '\n\n')
                f.write(str(primelist[i-1]) + '\n\n')
                f.write(str(primelist[i]) + '\n\n')
                f.write('```````````````````\n\n\n\n')
    
    pcounter += 1
'''