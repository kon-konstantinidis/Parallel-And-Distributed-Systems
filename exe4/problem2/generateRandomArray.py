import random

size = 5000000

f = open("randomArray"+str(size)+".txt", "w+")
for i in range(size):
    rand = random.randint(1,size*3)
    f.write(str(rand) + '\n')
f.close()

