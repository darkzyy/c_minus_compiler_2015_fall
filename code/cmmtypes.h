#ifndef __CMMTYPES_H__
#define __CMMTYPES_H__

typedef struct Type_ Type;
typedef struct FieldList_ FieldList;
typedef struct ArgList_ ArgList;

//#define __TYPES_USE__UNION_H__
#ifndef __TYPES_USE__UNION_H__

struct Type_{
    int size;
    enum{
        basic,
        array,
        structure,
        semantic_error
    }kind;
    int basic;
    struct{
        Type* elem;
        int size;
    }array;
    FieldList* fl;
};
#else
struct Type_{
    int size;
    enum{
        basic,
        array,
        structure,
        semantic_error
    }kind;
    union{
        int basic;
        struct{
            Type* elem;
            int size;
        }array;
        FieldList* fl;
    };
};
#endif

struct FieldList_{
    char* name; //field name
    Type* type; //field type
    FieldList* next; //next field
    FieldList* tail; //
    int offset;
};

struct ArgList_{
    Type* type;
    ArgList* next;
};

void init_basic_type();
int type_cmp(Type* tx,Type* ty);

#endif
