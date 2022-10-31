import os
import sys
import numpy as np
import random

perdida_prob = float(sys.argv[1])
propag_promedio = float(sys.argv[2])
exp_param = 1/propag_promedio

try:
    while True:
        if random.random() < perdida_prob:
            print("-1")
        else:
            print(np.random.exponential(exp_param))
except BrokenPipeError:
    devnull = os.open(os.devnull, os.O_WRONLY)
    os.dup2(devnull, sys.stdout.fileno())
    sys.exit(1)
