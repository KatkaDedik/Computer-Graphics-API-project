input_file = open("kahoot.txt", "r")

output_file = open("kahoot_notes.txt", "w")

i = 63


while (i > 0):
    lines = list()
    for j in range(6):
        lines.append(input_file.readline())
        #print(lines[j])
        i -= 1

    index = 4

    for k in range(6):
        line = lines[k]
        if line[3] != '-':
            index = 3
            break

    while(index < 60):

        out_line = ""
        for l in range(6):
            line = lines[l]
            out_line = out_line + line[index]
            #out_line += lines[l].

        out_line = out_line + '\n'
        output_file.write(out_line)

        if (index == 30 or index == 31):
            index -= 1

        index += 3

    output_file.write("******\n")
    input_file.readline()
    input_file.readline()
    i -= 2

