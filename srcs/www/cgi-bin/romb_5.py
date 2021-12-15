sum = 0
num = 10
while sum  < num:
    for sum1 in range(0, num - sum):
        print(" ", end="")
    for sum2 in range(0, sum * 2):
        print("i", end="")
    print()
    sum+=1

while sum  > 0:
    for sum1 in range(0, num - sum):
        print(" ", end="")
    for sum2 in range(0, sum * 2):
        print("i", end="")
    print()
    sum-=1


ints = 10
i = 0
while i < ints:
    for sum1 in range(1, ints):
        print(end=" ")
    for sum2 in range(1, i + 1):
        print(sum2, end="")
    print()
    i+=1
while i > 0:
    for sum2 in range(1, ints - i):
        print(end="")
    for sum1 in range(1, i):
        print(sum1, end="")
    print()
    i-=1
