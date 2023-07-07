

def print_green(text):
    # print('\033[0;32m' + text + '\033[0;0m')
    print(text)
    # pass

def divide_into_range(lower_bound, upper_bound, steps):
    step_size = int((upper_bound - lower_bound)/steps)
    
    old = lower_bound
    ranges = []
    for i in range(steps):
        ranges.append([old, old+step_size])
        old += step_size
    
    return ranges