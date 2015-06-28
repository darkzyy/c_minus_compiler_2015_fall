#ifndef __CMMTYPES_H__
#define __CMMTYPES_H__

typedef struct Type_ Type;
typedef struct FieldLidt_ FieldList;

struct Type_{
    enum{
        basic,
        array,
        structure
    }kind;
    union{
        int basic;
        struct{
            Type* elem;
            int size;
        }array;
        FieldList* structure;
    };
};

struct FieldLidt_{
    char* name; //field name
    Type* type; //field type
    FieldList* next; //next field
};

#endif
