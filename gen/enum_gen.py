prod_name = ''
count = 301
with open("syntax.y") as f:
    for line in f:
        line = line.rstrip('\n')
        line = line.rstrip(' ')
        if ':' in line:
            (prod_name,prod) = line.split(":")
            prod_name = prod_name.rstrip(' ')
            print "    %s=%d,"%(prod_name,count)
            count+=1
