prod_name = ''
indent = ''
with open("syntax.y") as f:
    for line in f:
        if '%empty' in line:
            print line
            continue
        line = line.rstrip('\n')
        line = line.rstrip(' ')
        if ':' in line:
            (prod_name,prod) = line.split(":")
            prod_name = prod_name.rstrip(' ')
            indent = ' '*(len(prod_name))
        elif '|' in line:
            (useless,prod) = line.split("|")
        else:
            continue
        prod_amount = prod.count(' ')
        line2=line
        print line2,'{'
        print indent,"MTnode** list=malloc(sizeof(void*)*%d);"%prod_amount
        for i in range(0,prod_amount):
            print indent,"list[%d]=$%d;"%(i,i+1)
        print indent,"$$ = create_node(list,%d,\"%s\",&@1,NONTERM);"%(prod_amount,prod_name)
        print indent,'}'
