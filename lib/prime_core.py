from threading import Thread
import random
from helper_functions import *
import queue


def trial_composite(j, a, d, n, s):
    # print_green('checking null case')
    if pow(a, d, n) == 1:
        return False
    
    # print_green('iterating tests')
    for i in range(s):
        # print_green('     (node: ' + str(j+1) + ' -- checking ' + str(i+1) + '/' + str(s) + ')')

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

    # print_green('starting trials')
    for i in range(n_trials):
        # print_green('\ttrial ' + str(i+1))
        a = random.randrange(2, n)
        if trial_composite(j, a, d, n, s):
            return False
    # print_green('trials finished')
    
    return True

def findPrimes(lower_bound, upper_bound, number_of_threads):

    q = queue.Queue()

    ranges = divide_into_range(lower_bound, upper_bound, number_of_threads)
    
    threads = [None]*number_of_threads
    for t in range(number_of_threads):
        threads[t] = CustomThread(t, ranges[t], q)
        threads[t].start()
        # print('\t   starting thread #' + str(t))
    
    # print('\n\n   activating workers...\n\n')
    # q.put('start')

    for t in range(number_of_threads):
        threads[t].join()
    
    primes = []
    for T in threads:
        for i in range(len(T.primelist)):
            primes.append(T.primelist[i])
    
    return primes




class CustomThread(Thread):
    def __init__(self, i, r, queue):
        
        Thread.__init__(self)
        self.n = r[0]
        self.m = r[1]
        self.i = i

        self.queue = queue
        
        self.primelist = []
        
    def run(self):
        
        # counter = 0
        for i in range(self.n, self.m):

            # sys.stdout.write('\r')
            # percentage = int( 100*counter / (self.m - self.n))
            # sys.stdout.write("    thread #1: %d%%" % (percentage))
            # sys.stdout.flush()

            # counter += 1

            if is_Prime(self.i, i, 8):
                self.primelist.append(i)
        
        # sys.stdout.flush()
        # sys.stdout.write('\r')

        if len(self.primelist) == 1: plural = ''
        else: plural = 's'
        
        if self.i < 9: extraspace = ' ' 
        else: extraspace = ''
        
        
        print('   thread #' + str(self.i+1) + ' finished; ' + extraspace + 'found ' 
            + str(len(self.primelist)) + ' prime' + plural)