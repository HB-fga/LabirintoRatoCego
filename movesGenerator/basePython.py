import random

while True:
    options = input()
    if options == "LOSE" or options == "WIN":
        break
    choice = random.randint(0, len(options) - 1)
    print(options[choice])

ratName = "ratoAleatorioPython"

print(ratName)
