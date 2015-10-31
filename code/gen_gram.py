prod_name = ''
count = 0
with open("origin.y") as f:
    for line in f:
        if not((':' in line)or('|' in line)):
            print(line)
            continue
        line = line.rstrip('\n')
        line = line.rstrip(' ')
        if ':' in line:
            (prod_name,prod) = line.split(":")
            prod_name = prod_name.rstrip(' ')
            count = 1
        elif '|' in line:
            count += 1
            (useless,prod) = line.split("|")
        print(prod_name,end='')
        print(count,':',prod)
