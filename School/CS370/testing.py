import first

def one(number):
    return number ** 2

def two(number):
    return number * 2

def three(number):
    return one(number) - two(number)

print(three(5))
print(first.three(5))
print(first.four(5))	

