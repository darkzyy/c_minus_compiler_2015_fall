with open("lexical.l") as f:
    for line in f:
        if "create" in line:
            (u0,u1,u2,u3,u4) = line.split(',')
            (u5,u6) = u4.split(')')
            l2=line.replace('str','\"'+u5+'\"')
            print l2,
        else:
            print line,

