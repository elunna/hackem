import re
import math


with open("dat.txt", "r") as a_file:
    for line in a_file:
        stripped_line = line.rstrip()
        numbers = re.findall('[0-9]+', stripped_line)
        if numbers and len(str(numbers[0])) > 1:
            stripped_line = re.sub(str(int(numbers[0])), str(int(numbers[0]) + 1), stripped_line)
        print(stripped_line)