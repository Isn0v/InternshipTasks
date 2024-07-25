height = 10**3 - 1
width = 10**3 - 1

with open("test/data/Pizza/huge_input_2.txt", "w") as f:
    f.write("2 2\n")
    f.write("1 0\n")
    f.write("1 1\n")
    f.write("{} {}\n".format(height, width))

    for i in range(height):
        if i % 2 == 0:
            f.write("1 1 "*(width//2))
            f.write("1 ")
            
        else:
            f.write("1 0 "*(width//2))
            f.write("1 ")
        f.write("\n")

with open("test/data/Pizza/huge_expected_2.txt", "w") as f:
    f.write("1 1 "*(width//2))
    f.write("1 ")
    f.write("\n")
    for i in range(1, height):
        if i % 2 == 0:
            f.write("2 2 "*(width//2))
            f.write("1 ")
        else:
            f.write("2 * "*(width//2))
            f.write("1 ")
        f.write("\n")
