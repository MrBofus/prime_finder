import random
import math
import time
import numpy as np

def print_green(text):
    print('\033[0;32m' + text + '\033[0;0m')


def trial_composite(j, a, d, n, s):
    # print_green('checking null case')
    if pow(a, d, n) == 1:
        return False
    
    # print_green('iterating tests')
    for i in range(s):
        print_green('\tnode: ' + str(j+1) + '; checking ' + str(i) + '/' + str(s))
        v_ = pow(a, 2**i * d, n)
        if v_ == n-1:
            return False
    
    return True



def is_Prime(j, n, n_trials):
    # print_green('checking last digit...')
    # Miller-Rabin primality test
    
    l = n%10
    if (l == 2 or l == 4 or l == 5 
        or l == 6 or l == 8 or l == 0):
        return False
    
    # print_green('last digit verified')

    # print_green('starting bitshift')
    s = 0
    d = n - 1
    while d%2 == 0:
        d >>= 1
        s += 1
    
    # print_green('bitshifted')

     #print_green('starting trials')
    for i in range(n_trials):
        # print_green('\ttrial ' + str(i+1))
        a = random.randrange(2, n)
        if trial_composite(j, a, d, n, s):
            return False
    # print_green('trials finished')
    
    return True



# print('getting prime list...')
# with open('resources/primes3.txt') as f:
#     lines = f.readlines()

# primelist = []
# for i in range(0, len(lines), 2): 
#     primelist.append(int(lines[i].split(' ')[0]))

print_green('computing mersenne number...')
mersenne = 10**1000 + 1

print('number of digits: ' + str(int(math.log10(mersenne)+1)))

print_green('beginning primality test:')

t_to_start = time.monotonic()
print('\t(result: ' + str(is_Prime(1, mersenne, 8)) + ')')
t_to_finish = time.monotonic()

print('time: ' + str(t_to_finish-t_to_start) + 's')

_ = input()