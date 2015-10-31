lcamt = 0
inside_func = 0
func_list = []
with open("tmp.c") as f:
    l = f.readline()
    while(len(l)!=0):
        if 'case' in l:
            l = l.lstrip(' ')
            l = l.lstrip('case')
            l = l.lstrip(' ')
            l = l.rstrip('\n')
            l = l.rstrip(' ')
            l = l.rstrip(':')
            lcamt = 0
            inside_func = 1
            print('void* Func_',end='')
            s = 'Func_'+l
            func_list.append(s)
            print(l,end = '')
            print('(MTnode* root)')
        elif inside_func :
            lcamt += l.count('{')
            lcamt -= l.count('}')
            if(lcamt==0):
                inside_func = 0
            if 'break;' in l:
                l = l.lstrip(' ')
                l = l.lstrip('break;')
                l = l.rstrip('\n')
            print(l,end='')
        l = f.readline()
    count = 0
    for s in func_list:
        count+=1
        print(s,',',end = '')
        if(count%4 ==0):
            print('')
