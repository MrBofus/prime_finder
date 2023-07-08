import math
import lib.prime_core as pc
import time



mpower = 110111

print(int(math.log10( mpower ) + 1))

candidate = 2**(mpower) - 1

print(int(math.log10( candidate ) + 1))


t_to_start = time.monotonic()
print(pc.LucasLehmer(mpower, True))
t_to_end = time.monotonic()

print(t_to_end - t_to_start)
