#ifndef _MACRO_UTIL_H_
#define _MACRO_UTIL_H_



#define GI(rows,cols,width)(((cols)*(width)+(rows)))

#define GIint(I,J,w)((((int)(J))*w +((int)(I))))

#define GSI(rows,cols,width)((rows)*(width)+(cols))

#define MIN(a,b)((((a)<(b))?a:b))

#define MAX(a,b)((((a)>(b))?a:b))

#define CLAMP(a,min,max)(MAX(MIN(a,max),min))

#define REINTCAST(V,T)((*((T*)(&V))))

//type-punning, but written to remove optimization warnings
#define UNION_CAST(x, destType) \
    (((union {__typeof__(x) a; destType b;})x).b)

#define  _GC(_B,_X,_Y,_W,_MSK,_SHFT)(((_MSK&_B[GI(_X,_Y,_W)])>>_SHFT))



#endif
