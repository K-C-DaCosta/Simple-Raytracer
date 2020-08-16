#include "../include/triangle.h"
#include "../include/macro_util.h"
#include <math.h>

#define _TRM(M,a,b,c,d,e)(M[a]*(M[b]*M[c]-M[d]*M[e]))

void computeV0V1V2(Vec4 *v0,Vec4 *v1,Vec4 *v2,Vec4 points[3],Vec4 *point);

float triangleFastRayIntersectionTest(Vec4 *d, Vec4 *e, Triangle *tri){
    Vec4 point,parm,v0;
    float M,beta,gamma,omega,t,_a,_bc,_d,_e,_f,inf = INFINITY;
    uint32_t logic;
    /*
        parm.x       d->x           v1.x
        *            *              *
        normal.x     normal.y       v0.x
        +            +              +
        parm.y       d->y           v1.y
        *            *              *
        normal.y     fd->noramal.y  v0.y
        +            +              +
        ...          ...            ...

    */
    //v4Su(&parm,tri->points+0,e);
    parm.x = tri->points[0].x - e->x;
    parm.y = tri->points[0].y - e->y;
    parm.z = tri->points[0].z - e->z;
    t = _DOTVV(parm,tri->fd.normal)/_DOTPP(d,(&tri->fd.normal));
    //v4ASA(&point,d,t,e);//point = d*t + e
    point.x = d->x*t + e->x;
    point.y = d->y*t + e->y;
    point.z = d->z*t + e->z;
    //computeV0V1V2(&v0,&v1,&v2,tri->points,&point);
    //v4Su(&v0,&point,tri->points + 0);
    v0.x = point.x-tri->points[0].x;
    v0.y = point.y-tri->points[0].y;
    v0.z = point.z-tri->points[0].z;
    _a  = tri->fd._a;//v4Dot(&v1,&v1);
    _bc = tri->fd._bc;//v4Dot(&v2,&v1);
    _d  = tri->fd._d;//v4Dot(&v2,&v2);
    _e  = _DOTVV(tri->fd.v1,v0);
    _f  = _DOTVV(v0,tri->fd.v2);
    M = tri->fd.invDet;//1.0f/(_a*_d - _bc*_bc);
    beta  =(_e*_d-_bc*_f)*M;
    gamma =(_a*_f-_e*_bc)*M;
    omega = 1.0f - gamma - beta;

    /*
    if( beta > 0.0f && gamma > 0.0f && omega > 0.0f ){
        return t;
    }else{
        return INFINITY;
    }
    */
    //code below makes fuction completely branchless. Not worth it.
    logic = (UNION_CAST(beta,uint)|UNION_CAST(gamma,uint)|UNION_CAST(omega,uint));
    logic >>=31; // logic == 0 cond is true and   logic 1 when condition is false;
    logic *= 0xffffffff; // logic is 0x0 when logic is 0,logic is 0xFF.. when logic is 1 (this is cross platform extension)
    logic = (UNION_CAST(t,uint)&(~logic)) | (UNION_CAST(inf,uint)&logic);
    return UNION_CAST(logic,float);
}

float triangleRayIntersectionTest(Vec4 *d,Vec4 *e,Triangle* tri){
    Vec4 *a,*b,*c;

    float M = 0,beta = 0,gamma = 0,t = 0;//inf = INFINITY,omega = 0;
    //uint32_t logic;

    a  = tri->points + 0;
    b  = tri->points + 1;
    c  = tri->points + 2;

    const int  _a= 0,  _d = 1,  _g = 2 ,  _j = 3,
               _b= 4,  _e = 5,  _h = 6 ,  _k = 7,
               _c= 8,  _f = 9,  _i = 10,  _l = 11;


    float m[12] = {
                   a->x-b->x , a->x-c->x ,  d->x  , a->x-e->x, // 0  1  2  3     a d g j
                   a->y-b->y , a->y-c->y ,  d->y  , a->y-e->y, // 4  5  6  7     b e h k
                   a->z-b->z , a->z-c->z ,  d->z  , a->z-e->z  // 8  9 10  11    c f i l
                  };
    M = _TRM(m,_a,_e,_i,_h,_f) + _TRM(m,_b,_g,_f,_d,_i) + _TRM(m,_c,_d,_h,_e,_g);

    if(fabs(M) < 0.01f){
      return INFINITY;
    }
    
    M= 1.0f/M;

    beta  = (_TRM(m,_j,_e,_i,_h,_f) + _TRM(m,_k,_g,_f,_d,_i) + _TRM(m,_l,_d,_h,_e,_g))*M;
    gamma = (_TRM(m,_i,_a,_k,_j,_b) + _TRM(m,_h,_j,_c,_a,_l) + _TRM(m,_g,_b,_l,_k,_c))*M;
    //omega = 1.0f - beta-gamma;
    t     = (_TRM(m,_f,_a,_k,_j,_b) + _TRM(m,_e,_j,_c,_a,_l) + _TRM(m,_d,_b,_l,_k,_c))*M*-1.0f;;

    if( beta > 0.0f && gamma > 0.0f && beta+gamma < 1.0f ){
        return t;
    }else{
        return INFINITY;
    }

//    logic = (UNION_CAST(beta,uint)|UNION_CAST(gamma,uint)|UNION_CAST(omega,uint));
//    logic>>=31; // logic == 0 cond is true and   logic 1 when condition is false;aasd
//    logic *= 0xffffffff; // logic is 0x0 when logic is 0,logic is 0xFF.. when logic is 1 (this is cross platform extension)
//    logic = (UNION_CAST(t,uint)&(~logic)) | (UNION_CAST(inf,uint)&logic);
//    return UNION_CAST(logic,float);
}

void triangleGetNormal(Vec4* point, Vec4 *norm,Triangle *tri){
    *norm = tri->fd.normal;
}

void triangleGetAmbientColor(Vec4 *point, Vec4 *color, Triangle *tri){
    Vec4 v0;
    Vec2 tex;
    float M,beta,gamma,omega,_a,_bc,_d,_e,_f;
    uint32_t texel;


    v4Su(&v0,point,tri->points);
    _a  = tri->fd._a;//v4Dot(&v1,&v1);
    _bc = tri->fd._bc;//v4Dot(&v2,&v1);
    _d  = tri->fd._d;//v4Dot(&v2,&v2);
    _e  = _DOTVV(tri->fd.v1,v0);
    _f  = _DOTVV(v0,tri->fd.v2);
    M = tri->fd.invDet;
    beta  =(_e*_d-_bc*_f)*M;
    gamma =(_a*_f-_e*_bc)*M;
    omega = 1.0f - gamma - beta;

    //tex = t0*beta + t1*gamma + t2*omega
    v2S (&tex,tri->tOrds+0 ,omega );
    v2IAS(&tex,tri->tOrds+1,beta);
    v2IAS(&tex,tri->tOrds+2,gamma);
    tex.x = CLAMP(tex.x,0.0f,1.0f);
    tex.y = CLAMP(tex.y,0.0f,1.0f);

    texel = imgSampleTexture(tri->texture,&tex);

    color->w = (texel&0xff000000)>>24;
    color->x = (texel&0x00ff0000)>>16;
    color->y = (texel&0x0000ff00)>> 8;
    color->z = (texel&0x000000ff);
    v4IS(color,1.0f/255.0f * 0.3f);
}

bool triangleComputeFaceData(Triangle *tri){
    Vec4 *v1, *v2;
    Vec4  *a = tri->points + 0,
          *b = tri->points + 1,
          *c = tri->points + 2;
    FaceData *fd = &tri->fd;
    v1 = &tri->fd.v1;
    v2 = &tri->fd.v2;

    v4Su(v1,b,a);
    v4Su(v2,c,a);
    fd->_a  = v4DotIW(v1,v1);
    fd->_bc = v4DotIW(v2,v1);
    fd->_d  = v4DotIW(v2,v2);
    fd->invDet = (fd->_a*fd->_d - fd->_bc*fd->_bc);

    if(fabsf(fd->invDet) < 0.01f){
        return false; //degenerate triangle
    }

    fd->invDet = 1.0f/fd->invDet;

    //compute normal(unit vector orthoganal to triangles surface)
    v4Cross(&fd->normal,v2,v1);
    v4Norm(&fd->normal);

    return true;
}

void computeV0V1V2(Vec4 *v0,Vec4 *v1,Vec4 *v2,Vec4 points[3],Vec4 *point){
    Vec4 *a,*b,*c;
    a  = points + 0;
    b  = points + 1;
    c  = points + 2;
    v4Su(v0,point,a);
    v4Su(v1,b,a);
    v4Su(v2,c,a);
}

void computeNormal(Vec4 *norm,Triangle *tri){
    //assumes triangles vertices are clockwise
    Vec4 a,b;
    v4Su(&a,&tri->points[2],&tri->points[0]);
    v4Su(&b,&tri->points[1],&tri->points[0]);
    v4Cross(norm,&a,&b);
}
