import itertools
import random

stuff = [1, 2, 3,4,5,6]
min_limit = 2
max_limit = 4
list_permut= []
choose_quantity = 5

for L in range(min_limit, max_limit):
    for subset in itertools.combinations(stuff, L):
        list_permut.append(subset)


for i in range(choose_quantity):
    print(random.choice(list_permut))