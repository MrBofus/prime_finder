import math
import lib.prime_core as pc
import time

candidate = 2**(1109) + 1

print(math.log10( candidate ))

t_to_start = time.monotonic()
print(pc.is_Prime(0, candidate, 8))
t_to_end = time.monotonic()

print(t_to_end - t_to_start)