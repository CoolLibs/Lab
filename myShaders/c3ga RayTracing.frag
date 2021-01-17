#version 430

varying vec2 vTexCoords;
uniform float uAspectRatio;
uniform vec3 uCamX;
uniform vec3 uCamY;
uniform vec3 uCamZ;
uniform vec3 uCamPos;
uniform float uFocalLength;

uniform float uTime;

// Adapted from https://github.com/enkimute/ganja.js/blob/master/codegen/cpp/cga.cpp

struct Mvec {
    float c0;
    float c1;
    float c2;
    float c3;
    float c4;
    float c5;
    float c6;
    float c7;
    float c8;
    float c9;
    float c10;
    float c11;
    float c12;
    float c13;
    float c14;
    float c15;
    float c16;
    float c17;
    float c18;
    float c19;
    float c20;
    float c21;
    float c22;
    float c23;
    float c24;
    float c25;
    float c26;
    float c27;
    float c28;
    float c29;
    float c30;
    float c31;
};

//***********************
// CGA.Reverse
// Reverses the order of the basis blades.
//***********************
Mvec reverse(Mvec a) {
    Mvec res;
    res.c0  =  a.c0;
    res.c1  =  a.c1;
    res.c2  =  a.c2;
    res.c3  =  a.c3;
    res.c4  =  a.c4;
    res.c5  =  a.c5;
    res.c6  = -a.c6;
    res.c7  = -a.c7;
    res.c8  = -a.c8;
    res.c9  = -a.c9;
    res.c10 = -a.c10;
    res.c11 = -a.c11;
    res.c12 = -a.c12;
    res.c13 = -a.c13;
    res.c14 = -a.c14;
    res.c15 = -a.c15;
    res.c16 = -a.c16;
    res.c17 = -a.c17;
    res.c18 = -a.c18;
    res.c19 = -a.c19;
    res.c20 = -a.c20;
    res.c21 = -a.c21;
    res.c22 = -a.c22;
    res.c23 = -a.c23;
    res.c24 = -a.c24;
    res.c25 = -a.c25;
    res.c26 =  a.c26;
    res.c27 =  a.c27;
    res.c28 =  a.c28;
    res.c29 =  a.c29;
    res.c30 =  a.c30;
    res.c31 =  a.c31;
    return res;
};


//***********************
// CGA.Dual
// Poincare duality operator.
//***********************
Mvec dual(Mvec a) {
  Mvec res;
  res.c0=-a.c31;
  res.c1=-a.c30;
  res.c2=a.c29;
  res.c3=-a.c28;
  res.c4=a.c27;
  res.c5=a.c26;
  res.c6=a.c25;
  res.c7=-a.c24;
  res.c8=a.c23;
  res.c9=a.c22;
  res.c10=a.c21;
  res.c11=-a.c20;
  res.c12=-a.c19;
  res.c13=a.c18;
  res.c14=a.c17;
  res.c15=-a.c16;
  res.c16=a.c15;
  res.c17=-a.c14;
  res.c18=-a.c13;
  res.c19=a.c12;
  res.c20=a.c11;
  res.c21=-a.c10;
  res.c22=-a.c9;
  res.c23=-a.c8;
  res.c24=a.c7;
  res.c25=-a.c6;
  res.c26=-a.c5;
  res.c27=-a.c4;
  res.c28=a.c3;
  res.c29=-a.c2;
  res.c30=a.c1;
  res.c31=a.c0;
  return res;
};

//***********************
// CGA.Conjugate : res = a.Conjugate()
// Clifford Conjugation
//***********************
Mvec conjugate(Mvec a) {
  Mvec res;
  res.c0=a.c0;
  res.c1=-a.c1;
  res.c2=-a.c2;
  res.c3=-a.c3;
  res.c4=-a.c4;
  res.c5=-a.c5;
  res.c6=-a.c6;
  res.c7=-a.c7;
  res.c8=-a.c8;
  res.c9=-a.c9;
  res.c10=-a.c10;
  res.c11=-a.c11;
  res.c12=-a.c12;
  res.c13=-a.c13;
  res.c14=-a.c14;
  res.c15=-a.c15;
  res.c16=a.c16;
  res.c17=a.c17;
  res.c18=a.c18;
  res.c19=a.c19;
  res.c20=a.c20;
  res.c21=a.c21;
  res.c22=a.c22;
  res.c23=a.c23;
  res.c24=a.c24;
  res.c25=a.c25;
  res.c26=a.c26;
  res.c27=a.c27;
  res.c28=a.c28;
  res.c29=a.c29;
  res.c30=a.c30;
  res.c31=-a.c31;
  return res;
};

//***********************
// CGA.Involute : res = a.Involute()
// Main involution
//***********************
Mvec involute(Mvec a) {
  Mvec res;
  res.c0=a.c0;
  res.c1=-a.c1;
  res.c2=-a.c2;
  res.c3=-a.c3;
  res.c4=-a.c4;
  res.c5=-a.c5;
  res.c6=a.c6;
  res.c7=a.c7;
  res.c8=a.c8;
  res.c9=a.c9;
  res.c10=a.c10;
  res.c11=a.c11;
  res.c12=a.c12;
  res.c13=a.c13;
  res.c14=a.c14;
  res.c15=a.c15;
  res.c16=-a.c16;
  res.c17=-a.c17;
  res.c18=-a.c18;
  res.c19=-a.c19;
  res.c20=-a.c20;
  res.c21=-a.c21;
  res.c22=-a.c22;
  res.c23=-a.c23;
  res.c24=-a.c24;
  res.c25=-a.c25;
  res.c26=a.c26;
  res.c27=a.c27;
  res.c28=a.c28;
  res.c29=a.c29;
  res.c30=a.c30;
  res.c31=-a.c31;
  return res;
};

//***********************
// CGA.Mul : res = a * b 
// The geometric product.
//***********************
Mvec mul(Mvec a, Mvec b) {
  Mvec res;
  res.c0=b.c0*a.c0+b.c1*a.c1+b.c2*a.c2+b.c3*a.c3+b.c4*a.c4-b.c5*a.c5-b.c6*a.c6-b.c7*a.c7-b.c8*a.c8+b.c9*a.c9-b.c10*a.c10-b.c11*a.c11+b.c12*a.c12-b.c13*a.c13+b.c14*a.c14+b.c15*a.c15-b.c16*a.c16-b.c17*a.c17+b.c18*a.c18-b.c19*a.c19+b.c20*a.c20+b.c21*a.c21-b.c22*a.c22+b.c23*a.c23+b.c24*a.c24+b.c25*a.c25+b.c26*a.c26-b.c27*a.c27-b.c28*a.c28-b.c29*a.c29-b.c30*a.c30-b.c31*a.c31;
  res.c1=b.c1*a.c0+b.c0*a.c1-b.c6*a.c2-b.c7*a.c3-b.c8*a.c4+b.c9*a.c5+b.c2*a.c6+b.c3*a.c7+b.c4*a.c8-b.c5*a.c9-b.c16*a.c10-b.c17*a.c11+b.c18*a.c12-b.c19*a.c13+b.c20*a.c14+b.c21*a.c15-b.c10*a.c16-b.c11*a.c17+b.c12*a.c18-b.c13*a.c19+b.c14*a.c20+b.c15*a.c21+b.c26*a.c22-b.c27*a.c23-b.c28*a.c24-b.c29*a.c25-b.c22*a.c26+b.c23*a.c27+b.c24*a.c28+b.c25*a.c29-b.c31*a.c30-b.c30*a.c31;
  res.c2=b.c2*a.c0+b.c6*a.c1+b.c0*a.c2-b.c10*a.c3-b.c11*a.c4+b.c12*a.c5-b.c1*a.c6+b.c16*a.c7+b.c17*a.c8-b.c18*a.c9+b.c3*a.c10+b.c4*a.c11-b.c5*a.c12-b.c22*a.c13+b.c23*a.c14+b.c24*a.c15+b.c7*a.c16+b.c8*a.c17-b.c9*a.c18-b.c26*a.c19+b.c27*a.c20+b.c28*a.c21-b.c13*a.c22+b.c14*a.c23+b.c15*a.c24-b.c30*a.c25+b.c19*a.c26-b.c20*a.c27-b.c21*a.c28+b.c31*a.c29+b.c25*a.c30+b.c29*a.c31;
  res.c3=b.c3*a.c0+b.c7*a.c1+b.c10*a.c2+b.c0*a.c3-b.c13*a.c4+b.c14*a.c5-b.c16*a.c6-b.c1*a.c7+b.c19*a.c8-b.c20*a.c9-b.c2*a.c10+b.c22*a.c11-b.c23*a.c12+b.c4*a.c13-b.c5*a.c14+b.c25*a.c15-b.c6*a.c16+b.c26*a.c17-b.c27*a.c18+b.c8*a.c19-b.c9*a.c20+b.c29*a.c21+b.c11*a.c22-b.c12*a.c23+b.c30*a.c24+b.c15*a.c25-b.c17*a.c26+b.c18*a.c27-b.c31*a.c28-b.c21*a.c29-b.c24*a.c30-b.c28*a.c31;
  res.c4=b.c4*a.c0+b.c8*a.c1+b.c11*a.c2+b.c13*a.c3+b.c0*a.c4+b.c15*a.c5-b.c17*a.c6-b.c19*a.c7-b.c1*a.c8-b.c21*a.c9-b.c22*a.c10-b.c2*a.c11-b.c24*a.c12-b.c3*a.c13-b.c25*a.c14-b.c5*a.c15-b.c26*a.c16-b.c6*a.c17-b.c28*a.c18-b.c7*a.c19-b.c29*a.c20-b.c9*a.c21-b.c10*a.c22-b.c30*a.c23-b.c12*a.c24-b.c14*a.c25+b.c16*a.c26+b.c31*a.c27+b.c18*a.c28+b.c20*a.c29+b.c23*a.c30+b.c27*a.c31;
  res.c5=b.c5*a.c0+b.c9*a.c1+b.c12*a.c2+b.c14*a.c3+b.c15*a.c4+b.c0*a.c5-b.c18*a.c6-b.c20*a.c7-b.c21*a.c8-b.c1*a.c9-b.c23*a.c10-b.c24*a.c11-b.c2*a.c12-b.c25*a.c13-b.c3*a.c14-b.c4*a.c15-b.c27*a.c16-b.c28*a.c17-b.c6*a.c18-b.c29*a.c19-b.c7*a.c20-b.c8*a.c21-b.c30*a.c22-b.c10*a.c23-b.c11*a.c24-b.c13*a.c25+b.c31*a.c26+b.c16*a.c27+b.c17*a.c28+b.c19*a.c29+b.c22*a.c30+b.c26*a.c31;
  res.c6=b.c6*a.c0+b.c2*a.c1-b.c1*a.c2+b.c16*a.c3+b.c17*a.c4-b.c18*a.c5+b.c0*a.c6-b.c10*a.c7-b.c11*a.c8+b.c12*a.c9+b.c7*a.c10+b.c8*a.c11-b.c9*a.c12-b.c26*a.c13+b.c27*a.c14+b.c28*a.c15+b.c3*a.c16+b.c4*a.c17-b.c5*a.c18-b.c22*a.c19+b.c23*a.c20+b.c24*a.c21+b.c19*a.c22-b.c20*a.c23-b.c21*a.c24+b.c31*a.c25-b.c13*a.c26+b.c14*a.c27+b.c15*a.c28-b.c30*a.c29+b.c29*a.c30+b.c25*a.c31;
  res.c7=b.c7*a.c0+b.c3*a.c1-b.c16*a.c2-b.c1*a.c3+b.c19*a.c4-b.c20*a.c5+b.c10*a.c6+b.c0*a.c7-b.c13*a.c8+b.c14*a.c9-b.c6*a.c10+b.c26*a.c11-b.c27*a.c12+b.c8*a.c13-b.c9*a.c14+b.c29*a.c15-b.c2*a.c16+b.c22*a.c17-b.c23*a.c18+b.c4*a.c19-b.c5*a.c20+b.c25*a.c21-b.c17*a.c22+b.c18*a.c23-b.c31*a.c24-b.c21*a.c25+b.c11*a.c26-b.c12*a.c27+b.c30*a.c28+b.c15*a.c29-b.c28*a.c30-b.c24*a.c31;
  res.c8=b.c8*a.c0+b.c4*a.c1-b.c17*a.c2-b.c19*a.c3-b.c1*a.c4-b.c21*a.c5+b.c11*a.c6+b.c13*a.c7+b.c0*a.c8+b.c15*a.c9-b.c26*a.c10-b.c6*a.c11-b.c28*a.c12-b.c7*a.c13-b.c29*a.c14-b.c9*a.c15-b.c22*a.c16-b.c2*a.c17-b.c24*a.c18-b.c3*a.c19-b.c25*a.c20-b.c5*a.c21+b.c16*a.c22+b.c31*a.c23+b.c18*a.c24+b.c20*a.c25-b.c10*a.c26-b.c30*a.c27-b.c12*a.c28-b.c14*a.c29+b.c27*a.c30+b.c23*a.c31;
  res.c9=b.c9*a.c0+b.c5*a.c1-b.c18*a.c2-b.c20*a.c3-b.c21*a.c4-b.c1*a.c5+b.c12*a.c6+b.c14*a.c7+b.c15*a.c8+b.c0*a.c9-b.c27*a.c10-b.c28*a.c11-b.c6*a.c12-b.c29*a.c13-b.c7*a.c14-b.c8*a.c15-b.c23*a.c16-b.c24*a.c17-b.c2*a.c18-b.c25*a.c19-b.c3*a.c20-b.c4*a.c21+b.c31*a.c22+b.c16*a.c23+b.c17*a.c24+b.c19*a.c25-b.c30*a.c26-b.c10*a.c27-b.c11*a.c28-b.c13*a.c29+b.c26*a.c30+b.c22*a.c31;
  res.c10=b.c10*a.c0+b.c16*a.c1+b.c3*a.c2-b.c2*a.c3+b.c22*a.c4-b.c23*a.c5-b.c7*a.c6+b.c6*a.c7-b.c26*a.c8+b.c27*a.c9+b.c0*a.c10-b.c13*a.c11+b.c14*a.c12+b.c11*a.c13-b.c12*a.c14+b.c30*a.c15+b.c1*a.c16-b.c19*a.c17+b.c20*a.c18+b.c17*a.c19-b.c18*a.c20+b.c31*a.c21+b.c4*a.c22-b.c5*a.c23+b.c25*a.c24-b.c24*a.c25-b.c8*a.c26+b.c9*a.c27-b.c29*a.c28+b.c28*a.c29+b.c15*a.c30+b.c21*a.c31;
  res.c11=b.c11*a.c0+b.c17*a.c1+b.c4*a.c2-b.c22*a.c3-b.c2*a.c4-b.c24*a.c5-b.c8*a.c6+b.c26*a.c7+b.c6*a.c8+b.c28*a.c9+b.c13*a.c10+b.c0*a.c11+b.c15*a.c12-b.c10*a.c13-b.c30*a.c14-b.c12*a.c15+b.c19*a.c16+b.c1*a.c17+b.c21*a.c18-b.c16*a.c19-b.c31*a.c20-b.c18*a.c21-b.c3*a.c22-b.c25*a.c23-b.c5*a.c24+b.c23*a.c25+b.c7*a.c26+b.c29*a.c27+b.c9*a.c28-b.c27*a.c29-b.c14*a.c30-b.c20*a.c31;
  res.c12=b.c12*a.c0+b.c18*a.c1+b.c5*a.c2-b.c23*a.c3-b.c24*a.c4-b.c2*a.c5-b.c9*a.c6+b.c27*a.c7+b.c28*a.c8+b.c6*a.c9+b.c14*a.c10+b.c15*a.c11+b.c0*a.c12-b.c30*a.c13-b.c10*a.c14-b.c11*a.c15+b.c20*a.c16+b.c21*a.c17+b.c1*a.c18-b.c31*a.c19-b.c16*a.c20-b.c17*a.c21-b.c25*a.c22-b.c3*a.c23-b.c4*a.c24+b.c22*a.c25+b.c29*a.c26+b.c7*a.c27+b.c8*a.c28-b.c26*a.c29-b.c13*a.c30-b.c19*a.c31;
  res.c13=b.c13*a.c0+b.c19*a.c1+b.c22*a.c2+b.c4*a.c3-b.c3*a.c4-b.c25*a.c5-b.c26*a.c6-b.c8*a.c7+b.c7*a.c8+b.c29*a.c9-b.c11*a.c10+b.c10*a.c11+b.c30*a.c12+b.c0*a.c13+b.c15*a.c14-b.c14*a.c15-b.c17*a.c16+b.c16*a.c17+b.c31*a.c18+b.c1*a.c19+b.c21*a.c20-b.c20*a.c21+b.c2*a.c22+b.c24*a.c23-b.c23*a.c24-b.c5*a.c25-b.c6*a.c26-b.c28*a.c27+b.c27*a.c28+b.c9*a.c29+b.c12*a.c30+b.c18*a.c31;
  res.c14=b.c14*a.c0+b.c20*a.c1+b.c23*a.c2+b.c5*a.c3-b.c25*a.c4-b.c3*a.c5-b.c27*a.c6-b.c9*a.c7+b.c29*a.c8+b.c7*a.c9-b.c12*a.c10+b.c30*a.c11+b.c10*a.c12+b.c15*a.c13+b.c0*a.c14-b.c13*a.c15-b.c18*a.c16+b.c31*a.c17+b.c16*a.c18+b.c21*a.c19+b.c1*a.c20-b.c19*a.c21+b.c24*a.c22+b.c2*a.c23-b.c22*a.c24-b.c4*a.c25-b.c28*a.c26-b.c6*a.c27+b.c26*a.c28+b.c8*a.c29+b.c11*a.c30+b.c17*a.c31;
  res.c15=b.c15*a.c0+b.c21*a.c1+b.c24*a.c2+b.c25*a.c3+b.c5*a.c4-b.c4*a.c5-b.c28*a.c6-b.c29*a.c7-b.c9*a.c8+b.c8*a.c9-b.c30*a.c10-b.c12*a.c11+b.c11*a.c12-b.c14*a.c13+b.c13*a.c14+b.c0*a.c15-b.c31*a.c16-b.c18*a.c17+b.c17*a.c18-b.c20*a.c19+b.c19*a.c20+b.c1*a.c21-b.c23*a.c22+b.c22*a.c23+b.c2*a.c24+b.c3*a.c25+b.c27*a.c26-b.c26*a.c27-b.c6*a.c28-b.c7*a.c29-b.c10*a.c30-b.c16*a.c31;
  res.c16=b.c16*a.c0+b.c10*a.c1-b.c7*a.c2+b.c6*a.c3-b.c26*a.c4+b.c27*a.c5+b.c3*a.c6-b.c2*a.c7+b.c22*a.c8-b.c23*a.c9+b.c1*a.c10-b.c19*a.c11+b.c20*a.c12+b.c17*a.c13-b.c18*a.c14+b.c31*a.c15+b.c0*a.c16-b.c13*a.c17+b.c14*a.c18+b.c11*a.c19-b.c12*a.c20+b.c30*a.c21-b.c8*a.c22+b.c9*a.c23-b.c29*a.c24+b.c28*a.c25+b.c4*a.c26-b.c5*a.c27+b.c25*a.c28-b.c24*a.c29+b.c21*a.c30+b.c15*a.c31;
  res.c17=b.c17*a.c0+b.c11*a.c1-b.c8*a.c2+b.c26*a.c3+b.c6*a.c4+b.c28*a.c5+b.c4*a.c6-b.c22*a.c7-b.c2*a.c8-b.c24*a.c9+b.c19*a.c10+b.c1*a.c11+b.c21*a.c12-b.c16*a.c13-b.c31*a.c14-b.c18*a.c15+b.c13*a.c16+b.c0*a.c17+b.c15*a.c18-b.c10*a.c19-b.c30*a.c20-b.c12*a.c21+b.c7*a.c22+b.c29*a.c23+b.c9*a.c24-b.c27*a.c25-b.c3*a.c26-b.c25*a.c27-b.c5*a.c28+b.c23*a.c29-b.c20*a.c30-b.c14*a.c31;
  res.c18=b.c18*a.c0+b.c12*a.c1-b.c9*a.c2+b.c27*a.c3+b.c28*a.c4+b.c6*a.c5+b.c5*a.c6-b.c23*a.c7-b.c24*a.c8-b.c2*a.c9+b.c20*a.c10+b.c21*a.c11+b.c1*a.c12-b.c31*a.c13-b.c16*a.c14-b.c17*a.c15+b.c14*a.c16+b.c15*a.c17+b.c0*a.c18-b.c30*a.c19-b.c10*a.c20-b.c11*a.c21+b.c29*a.c22+b.c7*a.c23+b.c8*a.c24-b.c26*a.c25-b.c25*a.c26-b.c3*a.c27-b.c4*a.c28+b.c22*a.c29-b.c19*a.c30-b.c13*a.c31;
  res.c19=b.c19*a.c0+b.c13*a.c1-b.c26*a.c2-b.c8*a.c3+b.c7*a.c4+b.c29*a.c5+b.c22*a.c6+b.c4*a.c7-b.c3*a.c8-b.c25*a.c9-b.c17*a.c10+b.c16*a.c11+b.c31*a.c12+b.c1*a.c13+b.c21*a.c14-b.c20*a.c15-b.c11*a.c16+b.c10*a.c17+b.c30*a.c18+b.c0*a.c19+b.c15*a.c20-b.c14*a.c21-b.c6*a.c22-b.c28*a.c23+b.c27*a.c24+b.c9*a.c25+b.c2*a.c26+b.c24*a.c27-b.c23*a.c28-b.c5*a.c29+b.c18*a.c30+b.c12*a.c31;
  res.c20=b.c20*a.c0+b.c14*a.c1-b.c27*a.c2-b.c9*a.c3+b.c29*a.c4+b.c7*a.c5+b.c23*a.c6+b.c5*a.c7-b.c25*a.c8-b.c3*a.c9-b.c18*a.c10+b.c31*a.c11+b.c16*a.c12+b.c21*a.c13+b.c1*a.c14-b.c19*a.c15-b.c12*a.c16+b.c30*a.c17+b.c10*a.c18+b.c15*a.c19+b.c0*a.c20-b.c13*a.c21-b.c28*a.c22-b.c6*a.c23+b.c26*a.c24+b.c8*a.c25+b.c24*a.c26+b.c2*a.c27-b.c22*a.c28-b.c4*a.c29+b.c17*a.c30+b.c11*a.c31;
  res.c21=b.c21*a.c0+b.c15*a.c1-b.c28*a.c2-b.c29*a.c3-b.c9*a.c4+b.c8*a.c5+b.c24*a.c6+b.c25*a.c7+b.c5*a.c8-b.c4*a.c9-b.c31*a.c10-b.c18*a.c11+b.c17*a.c12-b.c20*a.c13+b.c19*a.c14+b.c1*a.c15-b.c30*a.c16-b.c12*a.c17+b.c11*a.c18-b.c14*a.c19+b.c13*a.c20+b.c0*a.c21+b.c27*a.c22-b.c26*a.c23-b.c6*a.c24-b.c7*a.c25-b.c23*a.c26+b.c22*a.c27+b.c2*a.c28+b.c3*a.c29-b.c16*a.c30-b.c10*a.c31;
  res.c22=b.c22*a.c0+b.c26*a.c1+b.c13*a.c2-b.c11*a.c3+b.c10*a.c4+b.c30*a.c5-b.c19*a.c6+b.c17*a.c7-b.c16*a.c8-b.c31*a.c9+b.c4*a.c10-b.c3*a.c11-b.c25*a.c12+b.c2*a.c13+b.c24*a.c14-b.c23*a.c15+b.c8*a.c16-b.c7*a.c17-b.c29*a.c18+b.c6*a.c19+b.c28*a.c20-b.c27*a.c21+b.c0*a.c22+b.c15*a.c23-b.c14*a.c24+b.c12*a.c25-b.c1*a.c26-b.c21*a.c27+b.c20*a.c28-b.c18*a.c29-b.c5*a.c30-b.c9*a.c31;
  res.c23=b.c23*a.c0+b.c27*a.c1+b.c14*a.c2-b.c12*a.c3+b.c30*a.c4+b.c10*a.c5-b.c20*a.c6+b.c18*a.c7-b.c31*a.c8-b.c16*a.c9+b.c5*a.c10-b.c25*a.c11-b.c3*a.c12+b.c24*a.c13+b.c2*a.c14-b.c22*a.c15+b.c9*a.c16-b.c29*a.c17-b.c7*a.c18+b.c28*a.c19+b.c6*a.c20-b.c26*a.c21+b.c15*a.c22+b.c0*a.c23-b.c13*a.c24+b.c11*a.c25-b.c21*a.c26-b.c1*a.c27+b.c19*a.c28-b.c17*a.c29-b.c4*a.c30-b.c8*a.c31;
  res.c24=b.c24*a.c0+b.c28*a.c1+b.c15*a.c2-b.c30*a.c3-b.c12*a.c4+b.c11*a.c5-b.c21*a.c6+b.c31*a.c7+b.c18*a.c8-b.c17*a.c9+b.c25*a.c10+b.c5*a.c11-b.c4*a.c12-b.c23*a.c13+b.c22*a.c14+b.c2*a.c15+b.c29*a.c16+b.c9*a.c17-b.c8*a.c18-b.c27*a.c19+b.c26*a.c20+b.c6*a.c21-b.c14*a.c22+b.c13*a.c23+b.c0*a.c24-b.c10*a.c25+b.c20*a.c26-b.c19*a.c27-b.c1*a.c28+b.c16*a.c29+b.c3*a.c30+b.c7*a.c31;
  res.c25=b.c25*a.c0+b.c29*a.c1+b.c30*a.c2+b.c15*a.c3-b.c14*a.c4+b.c13*a.c5-b.c31*a.c6-b.c21*a.c7+b.c20*a.c8-b.c19*a.c9-b.c24*a.c10+b.c23*a.c11-b.c22*a.c12+b.c5*a.c13-b.c4*a.c14+b.c3*a.c15-b.c28*a.c16+b.c27*a.c17-b.c26*a.c18+b.c9*a.c19-b.c8*a.c20+b.c7*a.c21+b.c12*a.c22-b.c11*a.c23+b.c10*a.c24+b.c0*a.c25-b.c18*a.c26+b.c17*a.c27-b.c16*a.c28-b.c1*a.c29-b.c2*a.c30-b.c6*a.c31;
  res.c26=b.c26*a.c0+b.c22*a.c1-b.c19*a.c2+b.c17*a.c3-b.c16*a.c4-b.c31*a.c5+b.c13*a.c6-b.c11*a.c7+b.c10*a.c8+b.c30*a.c9+b.c8*a.c10-b.c7*a.c11-b.c29*a.c12+b.c6*a.c13+b.c28*a.c14-b.c27*a.c15+b.c4*a.c16-b.c3*a.c17-b.c25*a.c18+b.c2*a.c19+b.c24*a.c20-b.c23*a.c21-b.c1*a.c22-b.c21*a.c23+b.c20*a.c24-b.c18*a.c25+b.c0*a.c26+b.c15*a.c27-b.c14*a.c28+b.c12*a.c29-b.c9*a.c30-b.c5*a.c31;
  res.c27=b.c27*a.c0+b.c23*a.c1-b.c20*a.c2+b.c18*a.c3-b.c31*a.c4-b.c16*a.c5+b.c14*a.c6-b.c12*a.c7+b.c30*a.c8+b.c10*a.c9+b.c9*a.c10-b.c29*a.c11-b.c7*a.c12+b.c28*a.c13+b.c6*a.c14-b.c26*a.c15+b.c5*a.c16-b.c25*a.c17-b.c3*a.c18+b.c24*a.c19+b.c2*a.c20-b.c22*a.c21-b.c21*a.c22-b.c1*a.c23+b.c19*a.c24-b.c17*a.c25+b.c15*a.c26+b.c0*a.c27-b.c13*a.c28+b.c11*a.c29-b.c8*a.c30-b.c4*a.c31;
  res.c28=b.c28*a.c0+b.c24*a.c1-b.c21*a.c2+b.c31*a.c3+b.c18*a.c4-b.c17*a.c5+b.c15*a.c6-b.c30*a.c7-b.c12*a.c8+b.c11*a.c9+b.c29*a.c10+b.c9*a.c11-b.c8*a.c12-b.c27*a.c13+b.c26*a.c14+b.c6*a.c15+b.c25*a.c16+b.c5*a.c17-b.c4*a.c18-b.c23*a.c19+b.c22*a.c20+b.c2*a.c21+b.c20*a.c22-b.c19*a.c23-b.c1*a.c24+b.c16*a.c25-b.c14*a.c26+b.c13*a.c27+b.c0*a.c28-b.c10*a.c29+b.c7*a.c30+b.c3*a.c31;
  res.c29=b.c29*a.c0+b.c25*a.c1-b.c31*a.c2-b.c21*a.c3+b.c20*a.c4-b.c19*a.c5+b.c30*a.c6+b.c15*a.c7-b.c14*a.c8+b.c13*a.c9-b.c28*a.c10+b.c27*a.c11-b.c26*a.c12+b.c9*a.c13-b.c8*a.c14+b.c7*a.c15-b.c24*a.c16+b.c23*a.c17-b.c22*a.c18+b.c5*a.c19-b.c4*a.c20+b.c3*a.c21-b.c18*a.c22+b.c17*a.c23-b.c16*a.c24-b.c1*a.c25+b.c12*a.c26-b.c11*a.c27+b.c10*a.c28+b.c0*a.c29-b.c6*a.c30-b.c2*a.c31;
  res.c30=b.c30*a.c0+b.c31*a.c1+b.c25*a.c2-b.c24*a.c3+b.c23*a.c4-b.c22*a.c5-b.c29*a.c6+b.c28*a.c7-b.c27*a.c8+b.c26*a.c9+b.c15*a.c10-b.c14*a.c11+b.c13*a.c12+b.c12*a.c13-b.c11*a.c14+b.c10*a.c15+b.c21*a.c16-b.c20*a.c17+b.c19*a.c18+b.c18*a.c19-b.c17*a.c20+b.c16*a.c21+b.c5*a.c22-b.c4*a.c23+b.c3*a.c24-b.c2*a.c25-b.c9*a.c26+b.c8*a.c27-b.c7*a.c28+b.c6*a.c29+b.c0*a.c30+b.c1*a.c31;
  res.c31=b.c31*a.c0+b.c30*a.c1-b.c29*a.c2+b.c28*a.c3-b.c27*a.c4+b.c26*a.c5+b.c25*a.c6-b.c24*a.c7+b.c23*a.c8-b.c22*a.c9+b.c21*a.c10-b.c20*a.c11+b.c19*a.c12+b.c18*a.c13-b.c17*a.c14+b.c16*a.c15+b.c15*a.c16-b.c14*a.c17+b.c13*a.c18+b.c12*a.c19-b.c11*a.c20+b.c10*a.c21-b.c9*a.c22+b.c8*a.c23-b.c7*a.c24+b.c6*a.c25+b.c5*a.c26-b.c4*a.c27+b.c3*a.c28-b.c2*a.c29+b.c1*a.c30+b.c0*a.c31;
  return res;
};

//***********************
// CGA.Wedge : res = a ^ b 
// The outer product. (MEET)
//***********************
Mvec outer(Mvec a, Mvec b) {
  Mvec res;
  res.c0=b.c0*a.c0;
  res.c1=b.c1*a.c0+b.c0*a.c1;
  res.c2=b.c2*a.c0+b.c0*a.c2;
  res.c3=b.c3*a.c0+b.c0*a.c3;
  res.c4=b.c4*a.c0+b.c0*a.c4;
  res.c5=b.c5*a.c0+b.c0*a.c5;
  res.c6=b.c6*a.c0+b.c2*a.c1-b.c1*a.c2+b.c0*a.c6;
  res.c7=b.c7*a.c0+b.c3*a.c1-b.c1*a.c3+b.c0*a.c7;
  res.c8=b.c8*a.c0+b.c4*a.c1-b.c1*a.c4+b.c0*a.c8;
  res.c9=b.c9*a.c0+b.c5*a.c1-b.c1*a.c5+b.c0*a.c9;
  res.c10=b.c10*a.c0+b.c3*a.c2-b.c2*a.c3+b.c0*a.c10;
  res.c11=b.c11*a.c0+b.c4*a.c2-b.c2*a.c4+b.c0*a.c11;
  res.c12=b.c12*a.c0+b.c5*a.c2-b.c2*a.c5+b.c0*a.c12;
  res.c13=b.c13*a.c0+b.c4*a.c3-b.c3*a.c4+b.c0*a.c13;
  res.c14=b.c14*a.c0+b.c5*a.c3-b.c3*a.c5+b.c0*a.c14;
  res.c15=b.c15*a.c0+b.c5*a.c4-b.c4*a.c5+b.c0*a.c15;
  res.c16=b.c16*a.c0+b.c10*a.c1-b.c7*a.c2+b.c6*a.c3+b.c3*a.c6-b.c2*a.c7+b.c1*a.c10+b.c0*a.c16;
  res.c17=b.c17*a.c0+b.c11*a.c1-b.c8*a.c2+b.c6*a.c4+b.c4*a.c6-b.c2*a.c8+b.c1*a.c11+b.c0*a.c17;
  res.c18=b.c18*a.c0+b.c12*a.c1-b.c9*a.c2+b.c6*a.c5+b.c5*a.c6-b.c2*a.c9+b.c1*a.c12+b.c0*a.c18;
  res.c19=b.c19*a.c0+b.c13*a.c1-b.c8*a.c3+b.c7*a.c4+b.c4*a.c7-b.c3*a.c8+b.c1*a.c13+b.c0*a.c19;
  res.c20=b.c20*a.c0+b.c14*a.c1-b.c9*a.c3+b.c7*a.c5+b.c5*a.c7-b.c3*a.c9+b.c1*a.c14+b.c0*a.c20;
  res.c21=b.c21*a.c0+b.c15*a.c1-b.c9*a.c4+b.c8*a.c5+b.c5*a.c8-b.c4*a.c9+b.c1*a.c15+b.c0*a.c21;
  res.c22=b.c22*a.c0+b.c13*a.c2-b.c11*a.c3+b.c10*a.c4+b.c4*a.c10-b.c3*a.c11+b.c2*a.c13+b.c0*a.c22;
  res.c23=b.c23*a.c0+b.c14*a.c2-b.c12*a.c3+b.c10*a.c5+b.c5*a.c10-b.c3*a.c12+b.c2*a.c14+b.c0*a.c23;
  res.c24=b.c24*a.c0+b.c15*a.c2-b.c12*a.c4+b.c11*a.c5+b.c5*a.c11-b.c4*a.c12+b.c2*a.c15+b.c0*a.c24;
  res.c25=b.c25*a.c0+b.c15*a.c3-b.c14*a.c4+b.c13*a.c5+b.c5*a.c13-b.c4*a.c14+b.c3*a.c15+b.c0*a.c25;
  res.c26=b.c26*a.c0+b.c22*a.c1-b.c19*a.c2+b.c17*a.c3-b.c16*a.c4+b.c13*a.c6-b.c11*a.c7+b.c10*a.c8+b.c8*a.c10-b.c7*a.c11+b.c6*a.c13+b.c4*a.c16-b.c3*a.c17+b.c2*a.c19-b.c1*a.c22+b.c0*a.c26;
  res.c27=b.c27*a.c0+b.c23*a.c1-b.c20*a.c2+b.c18*a.c3-b.c16*a.c5+b.c14*a.c6-b.c12*a.c7+b.c10*a.c9+b.c9*a.c10-b.c7*a.c12+b.c6*a.c14+b.c5*a.c16-b.c3*a.c18+b.c2*a.c20-b.c1*a.c23+b.c0*a.c27;
  res.c28=b.c28*a.c0+b.c24*a.c1-b.c21*a.c2+b.c18*a.c4-b.c17*a.c5+b.c15*a.c6-b.c12*a.c8+b.c11*a.c9+b.c9*a.c11-b.c8*a.c12+b.c6*a.c15+b.c5*a.c17-b.c4*a.c18+b.c2*a.c21-b.c1*a.c24+b.c0*a.c28;
  res.c29=b.c29*a.c0+b.c25*a.c1-b.c21*a.c3+b.c20*a.c4-b.c19*a.c5+b.c15*a.c7-b.c14*a.c8+b.c13*a.c9+b.c9*a.c13-b.c8*a.c14+b.c7*a.c15+b.c5*a.c19-b.c4*a.c20+b.c3*a.c21-b.c1*a.c25+b.c0*a.c29;
  res.c30=b.c30*a.c0+b.c25*a.c2-b.c24*a.c3+b.c23*a.c4-b.c22*a.c5+b.c15*a.c10-b.c14*a.c11+b.c13*a.c12+b.c12*a.c13-b.c11*a.c14+b.c10*a.c15+b.c5*a.c22-b.c4*a.c23+b.c3*a.c24-b.c2*a.c25+b.c0*a.c30;
  res.c31=b.c31*a.c0+b.c30*a.c1-b.c29*a.c2+b.c28*a.c3-b.c27*a.c4+b.c26*a.c5+b.c25*a.c6-b.c24*a.c7+b.c23*a.c8-b.c22*a.c9+b.c21*a.c10-b.c20*a.c11+b.c19*a.c12+b.c18*a.c13-b.c17*a.c14+b.c16*a.c15+b.c15*a.c16-b.c14*a.c17+b.c13*a.c18+b.c12*a.c19-b.c11*a.c20+b.c10*a.c21-b.c9*a.c22+b.c8*a.c23-b.c7*a.c24+b.c6*a.c25+b.c5*a.c26-b.c4*a.c27+b.c3*a.c28-b.c2*a.c29+b.c1*a.c30+b.c0*a.c31;
  return res;
};

//***********************
// CGA.Vee : res = a & b 
// The regressive product. (JOIN)
//***********************
Mvec regressive(Mvec a, Mvec b) {
  Mvec res;
  res.c31=1*(a.c31*b.c31);
  res.c30=1*(a.c30*b.c31+a.c31*b.c30);
  res.c29=-1*(a.c29*-1*b.c31+a.c31*b.c29*-1);
  res.c28=1*(a.c28*b.c31+a.c31*b.c28);
  res.c27=-1*(a.c27*-1*b.c31+a.c31*b.c27*-1);
  res.c26=1*(a.c26*b.c31+a.c31*b.c26);
  res.c25=1*(a.c25*b.c31+a.c29*-1*b.c30-a.c30*b.c29*-1+a.c31*b.c25);
  res.c24=-1*(a.c24*-1*b.c31+a.c28*b.c30-a.c30*b.c28+a.c31*b.c24*-1);
  res.c23=1*(a.c23*b.c31+a.c27*-1*b.c30-a.c30*b.c27*-1+a.c31*b.c23);
  res.c22=-1*(a.c22*-1*b.c31+a.c26*b.c30-a.c30*b.c26+a.c31*b.c22*-1);
  res.c21=1*(a.c21*b.c31+a.c28*b.c29*-1-a.c29*-1*b.c28+a.c31*b.c21);
  res.c20=-1*(a.c20*-1*b.c31+a.c27*-1*b.c29*-1-a.c29*-1*b.c27*-1+a.c31*b.c20*-1);
  res.c19=1*(a.c19*b.c31+a.c26*b.c29*-1-a.c29*-1*b.c26+a.c31*b.c19);
  res.c18=1*(a.c18*b.c31+a.c27*-1*b.c28-a.c28*b.c27*-1+a.c31*b.c18);
  res.c17=-1*(a.c17*-1*b.c31+a.c26*b.c28-a.c28*b.c26+a.c31*b.c17*-1);
  res.c16=1*(a.c16*b.c31+a.c26*b.c27*-1-a.c27*-1*b.c26+a.c31*b.c16);
  res.c15=1*(a.c15*b.c31+a.c21*b.c30-a.c24*-1*b.c29*-1+a.c25*b.c28+a.c28*b.c25-a.c29*-1*b.c24*-1+a.c30*b.c21+a.c31*b.c15);
  res.c14=-1*(a.c14*-1*b.c31+a.c20*-1*b.c30-a.c23*b.c29*-1+a.c25*b.c27*-1+a.c27*-1*b.c25-a.c29*-1*b.c23+a.c30*b.c20*-1+a.c31*b.c14*-1);
  res.c13=1*(a.c13*b.c31+a.c19*b.c30-a.c22*-1*b.c29*-1+a.c25*b.c26+a.c26*b.c25-a.c29*-1*b.c22*-1+a.c30*b.c19+a.c31*b.c13);
  res.c12=1*(a.c12*b.c31+a.c18*b.c30-a.c23*b.c28+a.c24*-1*b.c27*-1+a.c27*-1*b.c24*-1-a.c28*b.c23+a.c30*b.c18+a.c31*b.c12);
  res.c11=-1*(a.c11*-1*b.c31+a.c17*-1*b.c30-a.c22*-1*b.c28+a.c24*-1*b.c26+a.c26*b.c24*-1-a.c28*b.c22*-1+a.c30*b.c17*-1+a.c31*b.c11*-1);
  res.c10=1*(a.c10*b.c31+a.c16*b.c30-a.c22*-1*b.c27*-1+a.c23*b.c26+a.c26*b.c23-a.c27*-1*b.c22*-1+a.c30*b.c16+a.c31*b.c10);
  res.c9=-1*(a.c9*-1*b.c31+a.c18*b.c29*-1-a.c20*-1*b.c28+a.c21*b.c27*-1+a.c27*-1*b.c21-a.c28*b.c20*-1+a.c29*-1*b.c18+a.c31*b.c9*-1);
  res.c8=1*(a.c8*b.c31+a.c17*-1*b.c29*-1-a.c19*b.c28+a.c21*b.c26+a.c26*b.c21-a.c28*b.c19+a.c29*-1*b.c17*-1+a.c31*b.c8);
  res.c7=-1*(a.c7*-1*b.c31+a.c16*b.c29*-1-a.c19*b.c27*-1+a.c20*-1*b.c26+a.c26*b.c20*-1-a.c27*-1*b.c19+a.c29*-1*b.c16+a.c31*b.c7*-1);
  res.c6=1*(a.c6*b.c31+a.c16*b.c28-a.c17*-1*b.c27*-1+a.c18*b.c26+a.c26*b.c18-a.c27*-1*b.c17*-1+a.c28*b.c16+a.c31*b.c6);
  res.c5=1*(a.c5*b.c31+a.c9*-1*b.c30-a.c12*b.c29*-1+a.c14*-1*b.c28-a.c15*b.c27*-1+a.c18*b.c25-a.c20*-1*b.c24*-1+a.c21*b.c23+a.c23*b.c21-a.c24*-1*b.c20*-1+a.c25*b.c18+a.c27*-1*b.c15-a.c28*b.c14*-1+a.c29*-1*b.c12-a.c30*b.c9*-1+a.c31*b.c5);
  res.c4=-1*(a.c4*-1*b.c31+a.c8*b.c30-a.c11*-1*b.c29*-1+a.c13*b.c28-a.c15*b.c26+a.c17*-1*b.c25-a.c19*b.c24*-1+a.c21*b.c22*-1+a.c22*-1*b.c21-a.c24*-1*b.c19+a.c25*b.c17*-1+a.c26*b.c15-a.c28*b.c13+a.c29*-1*b.c11*-1-a.c30*b.c8+a.c31*b.c4*-1);
  res.c3=1*(a.c3*b.c31+a.c7*-1*b.c30-a.c10*b.c29*-1+a.c13*b.c27*-1-a.c14*-1*b.c26+a.c16*b.c25-a.c19*b.c23+a.c20*-1*b.c22*-1+a.c22*-1*b.c20*-1-a.c23*b.c19+a.c25*b.c16+a.c26*b.c14*-1-a.c27*-1*b.c13+a.c29*-1*b.c10-a.c30*b.c7*-1+a.c31*b.c3);
  res.c2=-1*(a.c2*-1*b.c31+a.c6*b.c30-a.c10*b.c28+a.c11*-1*b.c27*-1-a.c12*b.c26+a.c16*b.c24*-1-a.c17*-1*b.c23+a.c18*b.c22*-1+a.c22*-1*b.c18-a.c23*b.c17*-1+a.c24*-1*b.c16+a.c26*b.c12-a.c27*-1*b.c11*-1+a.c28*b.c10-a.c30*b.c6+a.c31*b.c2*-1);
  res.c1=1*(a.c1*b.c31+a.c6*b.c29*-1-a.c7*-1*b.c28+a.c8*b.c27*-1-a.c9*-1*b.c26+a.c16*b.c21-a.c17*-1*b.c20*-1+a.c18*b.c19+a.c19*b.c18-a.c20*-1*b.c17*-1+a.c21*b.c16+a.c26*b.c9*-1-a.c27*-1*b.c8+a.c28*b.c7*-1-a.c29*-1*b.c6+a.c31*b.c1);
  res.c0=1*(a.c0*b.c31+a.c1*b.c30-a.c2*-1*b.c29*-1+a.c3*b.c28-a.c4*-1*b.c27*-1+a.c5*b.c26+a.c6*b.c25-a.c7*-1*b.c24*-1+a.c8*b.c23-a.c9*-1*b.c22*-1+a.c10*b.c21-a.c11*-1*b.c20*-1+a.c12*b.c19+a.c13*b.c18-a.c14*-1*b.c17*-1+a.c15*b.c16+a.c16*b.c15-a.c17*-1*b.c14*-1+a.c18*b.c13+a.c19*b.c12-a.c20*-1*b.c11*-1+a.c21*b.c10-a.c22*-1*b.c9*-1+a.c23*b.c8-a.c24*-1*b.c7*-1+a.c25*b.c6+a.c26*b.c5-a.c27*-1*b.c4*-1+a.c28*b.c3-a.c29*-1*b.c2*-1+a.c30*b.c1+a.c31*b.c0);
  return res;
};

//***********************
// CGA.Dot : res = a | b 
// The inner product.
//***********************
Mvec inner(Mvec a, Mvec b) {
  Mvec res;
  res.c0=b.c0*a.c0+b.c1*a.c1+b.c2*a.c2+b.c3*a.c3+b.c4*a.c4-b.c5*a.c5-b.c6*a.c6-b.c7*a.c7-b.c8*a.c8+b.c9*a.c9-b.c10*a.c10-b.c11*a.c11+b.c12*a.c12-b.c13*a.c13+b.c14*a.c14+b.c15*a.c15-b.c16*a.c16-b.c17*a.c17+b.c18*a.c18-b.c19*a.c19+b.c20*a.c20+b.c21*a.c21-b.c22*a.c22+b.c23*a.c23+b.c24*a.c24+b.c25*a.c25+b.c26*a.c26-b.c27*a.c27-b.c28*a.c28-b.c29*a.c29-b.c30*a.c30-b.c31*a.c31;
  res.c1=b.c1*a.c0+b.c0*a.c1-b.c6*a.c2-b.c7*a.c3-b.c8*a.c4+b.c9*a.c5+b.c2*a.c6+b.c3*a.c7+b.c4*a.c8-b.c5*a.c9-b.c16*a.c10-b.c17*a.c11+b.c18*a.c12-b.c19*a.c13+b.c20*a.c14+b.c21*a.c15-b.c10*a.c16-b.c11*a.c17+b.c12*a.c18-b.c13*a.c19+b.c14*a.c20+b.c15*a.c21+b.c26*a.c22-b.c27*a.c23-b.c28*a.c24-b.c29*a.c25-b.c22*a.c26+b.c23*a.c27+b.c24*a.c28+b.c25*a.c29-b.c31*a.c30-b.c30*a.c31;
  res.c2=b.c2*a.c0+b.c6*a.c1+b.c0*a.c2-b.c10*a.c3-b.c11*a.c4+b.c12*a.c5-b.c1*a.c6+b.c16*a.c7+b.c17*a.c8-b.c18*a.c9+b.c3*a.c10+b.c4*a.c11-b.c5*a.c12-b.c22*a.c13+b.c23*a.c14+b.c24*a.c15+b.c7*a.c16+b.c8*a.c17-b.c9*a.c18-b.c26*a.c19+b.c27*a.c20+b.c28*a.c21-b.c13*a.c22+b.c14*a.c23+b.c15*a.c24-b.c30*a.c25+b.c19*a.c26-b.c20*a.c27-b.c21*a.c28+b.c31*a.c29+b.c25*a.c30+b.c29*a.c31;
  res.c3=b.c3*a.c0+b.c7*a.c1+b.c10*a.c2+b.c0*a.c3-b.c13*a.c4+b.c14*a.c5-b.c16*a.c6-b.c1*a.c7+b.c19*a.c8-b.c20*a.c9-b.c2*a.c10+b.c22*a.c11-b.c23*a.c12+b.c4*a.c13-b.c5*a.c14+b.c25*a.c15-b.c6*a.c16+b.c26*a.c17-b.c27*a.c18+b.c8*a.c19-b.c9*a.c20+b.c29*a.c21+b.c11*a.c22-b.c12*a.c23+b.c30*a.c24+b.c15*a.c25-b.c17*a.c26+b.c18*a.c27-b.c31*a.c28-b.c21*a.c29-b.c24*a.c30-b.c28*a.c31;
  res.c4=b.c4*a.c0+b.c8*a.c1+b.c11*a.c2+b.c13*a.c3+b.c0*a.c4+b.c15*a.c5-b.c17*a.c6-b.c19*a.c7-b.c1*a.c8-b.c21*a.c9-b.c22*a.c10-b.c2*a.c11-b.c24*a.c12-b.c3*a.c13-b.c25*a.c14-b.c5*a.c15-b.c26*a.c16-b.c6*a.c17-b.c28*a.c18-b.c7*a.c19-b.c29*a.c20-b.c9*a.c21-b.c10*a.c22-b.c30*a.c23-b.c12*a.c24-b.c14*a.c25+b.c16*a.c26+b.c31*a.c27+b.c18*a.c28+b.c20*a.c29+b.c23*a.c30+b.c27*a.c31;
  res.c5=b.c5*a.c0+b.c9*a.c1+b.c12*a.c2+b.c14*a.c3+b.c15*a.c4+b.c0*a.c5-b.c18*a.c6-b.c20*a.c7-b.c21*a.c8-b.c1*a.c9-b.c23*a.c10-b.c24*a.c11-b.c2*a.c12-b.c25*a.c13-b.c3*a.c14-b.c4*a.c15-b.c27*a.c16-b.c28*a.c17-b.c6*a.c18-b.c29*a.c19-b.c7*a.c20-b.c8*a.c21-b.c30*a.c22-b.c10*a.c23-b.c11*a.c24-b.c13*a.c25+b.c31*a.c26+b.c16*a.c27+b.c17*a.c28+b.c19*a.c29+b.c22*a.c30+b.c26*a.c31;
  res.c6=b.c6*a.c0+b.c16*a.c3+b.c17*a.c4-b.c18*a.c5+b.c0*a.c6-b.c26*a.c13+b.c27*a.c14+b.c28*a.c15+b.c3*a.c16+b.c4*a.c17-b.c5*a.c18+b.c31*a.c25-b.c13*a.c26+b.c14*a.c27+b.c15*a.c28+b.c25*a.c31;
  res.c7=b.c7*a.c0-b.c16*a.c2+b.c19*a.c4-b.c20*a.c5+b.c0*a.c7+b.c26*a.c11-b.c27*a.c12+b.c29*a.c15-b.c2*a.c16+b.c4*a.c19-b.c5*a.c20-b.c31*a.c24+b.c11*a.c26-b.c12*a.c27+b.c15*a.c29-b.c24*a.c31;
  res.c8=b.c8*a.c0-b.c17*a.c2-b.c19*a.c3-b.c21*a.c5+b.c0*a.c8-b.c26*a.c10-b.c28*a.c12-b.c29*a.c14-b.c2*a.c17-b.c3*a.c19-b.c5*a.c21+b.c31*a.c23-b.c10*a.c26-b.c12*a.c28-b.c14*a.c29+b.c23*a.c31;
  res.c9=b.c9*a.c0-b.c18*a.c2-b.c20*a.c3-b.c21*a.c4+b.c0*a.c9-b.c27*a.c10-b.c28*a.c11-b.c29*a.c13-b.c2*a.c18-b.c3*a.c20-b.c4*a.c21+b.c31*a.c22-b.c10*a.c27-b.c11*a.c28-b.c13*a.c29+b.c22*a.c31;
  res.c10=b.c10*a.c0+b.c16*a.c1+b.c22*a.c4-b.c23*a.c5-b.c26*a.c8+b.c27*a.c9+b.c0*a.c10+b.c30*a.c15+b.c1*a.c16+b.c31*a.c21+b.c4*a.c22-b.c5*a.c23-b.c8*a.c26+b.c9*a.c27+b.c15*a.c30+b.c21*a.c31;
  res.c11=b.c11*a.c0+b.c17*a.c1-b.c22*a.c3-b.c24*a.c5+b.c26*a.c7+b.c28*a.c9+b.c0*a.c11-b.c30*a.c14+b.c1*a.c17-b.c31*a.c20-b.c3*a.c22-b.c5*a.c24+b.c7*a.c26+b.c9*a.c28-b.c14*a.c30-b.c20*a.c31;
  res.c12=b.c12*a.c0+b.c18*a.c1-b.c23*a.c3-b.c24*a.c4+b.c27*a.c7+b.c28*a.c8+b.c0*a.c12-b.c30*a.c13+b.c1*a.c18-b.c31*a.c19-b.c3*a.c23-b.c4*a.c24+b.c7*a.c27+b.c8*a.c28-b.c13*a.c30-b.c19*a.c31;
  res.c13=b.c13*a.c0+b.c19*a.c1+b.c22*a.c2-b.c25*a.c5-b.c26*a.c6+b.c29*a.c9+b.c30*a.c12+b.c0*a.c13+b.c31*a.c18+b.c1*a.c19+b.c2*a.c22-b.c5*a.c25-b.c6*a.c26+b.c9*a.c29+b.c12*a.c30+b.c18*a.c31;
  res.c14=b.c14*a.c0+b.c20*a.c1+b.c23*a.c2-b.c25*a.c4-b.c27*a.c6+b.c29*a.c8+b.c30*a.c11+b.c0*a.c14+b.c31*a.c17+b.c1*a.c20+b.c2*a.c23-b.c4*a.c25-b.c6*a.c27+b.c8*a.c29+b.c11*a.c30+b.c17*a.c31;
  res.c15=b.c15*a.c0+b.c21*a.c1+b.c24*a.c2+b.c25*a.c3-b.c28*a.c6-b.c29*a.c7-b.c30*a.c10+b.c0*a.c15-b.c31*a.c16+b.c1*a.c21+b.c2*a.c24+b.c3*a.c25-b.c6*a.c28-b.c7*a.c29-b.c10*a.c30-b.c16*a.c31;
  res.c16=b.c16*a.c0-b.c26*a.c4+b.c27*a.c5+b.c31*a.c15+b.c0*a.c16+b.c4*a.c26-b.c5*a.c27+b.c15*a.c31;
  res.c17=b.c17*a.c0+b.c26*a.c3+b.c28*a.c5-b.c31*a.c14+b.c0*a.c17-b.c3*a.c26-b.c5*a.c28-b.c14*a.c31;
  res.c18=b.c18*a.c0+b.c27*a.c3+b.c28*a.c4-b.c31*a.c13+b.c0*a.c18-b.c3*a.c27-b.c4*a.c28-b.c13*a.c31;
  res.c19=b.c19*a.c0-b.c26*a.c2+b.c29*a.c5+b.c31*a.c12+b.c0*a.c19+b.c2*a.c26-b.c5*a.c29+b.c12*a.c31;
  res.c20=b.c20*a.c0-b.c27*a.c2+b.c29*a.c4+b.c31*a.c11+b.c0*a.c20+b.c2*a.c27-b.c4*a.c29+b.c11*a.c31;
  res.c21=b.c21*a.c0-b.c28*a.c2-b.c29*a.c3-b.c31*a.c10+b.c0*a.c21+b.c2*a.c28+b.c3*a.c29-b.c10*a.c31;
  res.c22=b.c22*a.c0+b.c26*a.c1+b.c30*a.c5-b.c31*a.c9+b.c0*a.c22-b.c1*a.c26-b.c5*a.c30-b.c9*a.c31;
  res.c23=b.c23*a.c0+b.c27*a.c1+b.c30*a.c4-b.c31*a.c8+b.c0*a.c23-b.c1*a.c27-b.c4*a.c30-b.c8*a.c31;
  res.c24=b.c24*a.c0+b.c28*a.c1-b.c30*a.c3+b.c31*a.c7+b.c0*a.c24-b.c1*a.c28+b.c3*a.c30+b.c7*a.c31;
  res.c25=b.c25*a.c0+b.c29*a.c1+b.c30*a.c2-b.c31*a.c6+b.c0*a.c25-b.c1*a.c29-b.c2*a.c30-b.c6*a.c31;
  res.c26=b.c26*a.c0-b.c31*a.c5+b.c0*a.c26-b.c5*a.c31;
  res.c27=b.c27*a.c0-b.c31*a.c4+b.c0*a.c27-b.c4*a.c31;
  res.c28=b.c28*a.c0+b.c31*a.c3+b.c0*a.c28+b.c3*a.c31;
  res.c29=b.c29*a.c0-b.c31*a.c2+b.c0*a.c29-b.c2*a.c31;
  res.c30=b.c30*a.c0+b.c31*a.c1+b.c0*a.c30+b.c1*a.c31;
  res.c31=b.c31*a.c0+b.c0*a.c31;
  return res;
};

//***********************
// CGA.Add : res = a + b 
// Multivector addition
//***********************
Mvec add(Mvec a, Mvec b) {
  Mvec res;
      res.c0 = a.c0+b.c0;
    res.c1 = a.c1+b.c1;
    res.c2 = a.c2+b.c2;
    res.c3 = a.c3+b.c3;
    res.c4 = a.c4+b.c4;
    res.c5 = a.c5+b.c5;
    res.c6 = a.c6+b.c6;
    res.c7 = a.c7+b.c7;
    res.c8 = a.c8+b.c8;
    res.c9 = a.c9+b.c9;
    res.c10 = a.c10+b.c10;
    res.c11 = a.c11+b.c11;
    res.c12 = a.c12+b.c12;
    res.c13 = a.c13+b.c13;
    res.c14 = a.c14+b.c14;
    res.c15 = a.c15+b.c15;
    res.c16 = a.c16+b.c16;
    res.c17 = a.c17+b.c17;
    res.c18 = a.c18+b.c18;
    res.c19 = a.c19+b.c19;
    res.c20 = a.c20+b.c20;
    res.c21 = a.c21+b.c21;
    res.c22 = a.c22+b.c22;
    res.c23 = a.c23+b.c23;
    res.c24 = a.c24+b.c24;
    res.c25 = a.c25+b.c25;
    res.c26 = a.c26+b.c26;
    res.c27 = a.c27+b.c27;
    res.c28 = a.c28+b.c28;
    res.c29 = a.c29+b.c29;
    res.c30 = a.c30+b.c30;
    res.c31 = a.c31+b.c31;
  return res;
};

//***********************
// CGA.Sub : res = a - b 
// Multivector subtraction
//***********************
Mvec sub(Mvec a, Mvec b) {
  Mvec res;
      res.c0 = a.c0-b.c0;
    res.c1 = a.c1-b.c1;
    res.c2 = a.c2-b.c2;
    res.c3 = a.c3-b.c3;
    res.c4 = a.c4-b.c4;
    res.c5 = a.c5-b.c5;
    res.c6 = a.c6-b.c6;
    res.c7 = a.c7-b.c7;
    res.c8 = a.c8-b.c8;
    res.c9 = a.c9-b.c9;
    res.c10 = a.c10-b.c10;
    res.c11 = a.c11-b.c11;
    res.c12 = a.c12-b.c12;
    res.c13 = a.c13-b.c13;
    res.c14 = a.c14-b.c14;
    res.c15 = a.c15-b.c15;
    res.c16 = a.c16-b.c16;
    res.c17 = a.c17-b.c17;
    res.c18 = a.c18-b.c18;
    res.c19 = a.c19-b.c19;
    res.c20 = a.c20-b.c20;
    res.c21 = a.c21-b.c21;
    res.c22 = a.c22-b.c22;
    res.c23 = a.c23-b.c23;
    res.c24 = a.c24-b.c24;
    res.c25 = a.c25-b.c25;
    res.c26 = a.c26-b.c26;
    res.c27 = a.c27-b.c27;
    res.c28 = a.c28-b.c28;
    res.c29 = a.c29-b.c29;
    res.c30 = a.c30-b.c30;
    res.c31 = a.c31-b.c31;
  return res;
};

//***********************
// CGA.smul : res = a * b 
// scalar/multivector multiplication
//***********************
Mvec scale(Mvec b, float a) {
  Mvec res;
      res.c0 = a*b.c0;
    res.c1 = a*b.c1;
    res.c2 = a*b.c2;
    res.c3 = a*b.c3;
    res.c4 = a*b.c4;
    res.c5 = a*b.c5;
    res.c6 = a*b.c6;
    res.c7 = a*b.c7;
    res.c8 = a*b.c8;
    res.c9 = a*b.c9;
    res.c10 = a*b.c10;
    res.c11 = a*b.c11;
    res.c12 = a*b.c12;
    res.c13 = a*b.c13;
    res.c14 = a*b.c14;
    res.c15 = a*b.c15;
    res.c16 = a*b.c16;
    res.c17 = a*b.c17;
    res.c18 = a*b.c18;
    res.c19 = a*b.c19;
    res.c20 = a*b.c20;
    res.c21 = a*b.c21;
    res.c22 = a*b.c22;
    res.c23 = a*b.c23;
    res.c24 = a*b.c24;
    res.c25 = a*b.c25;
    res.c26 = a*b.c26;
    res.c27 = a*b.c27;
    res.c28 = a*b.c28;
    res.c29 = a*b.c29;
    res.c30 = a*b.c30;
    res.c31 = a*b.c31;
  return res;
};

float quadraticNorm(Mvec a) {
    return inner(reverse(a), a).c0;
};

Mvec inv(Mvec a) {
    float n = quadraticNorm(a);
    if(abs(n) < 0.0000001) {
        Mvec res;
        return res;
    }
    return scale(reverse(a), 1./n);
}

float norm(Mvec a) {
    return sqrt(abs(
        mul(a, conjugate(a)).c0
    ));
}

float inorm(Mvec a) { return norm(dual(a)); }

Mvec normalized(Mvec a) { 
    return scale(a, 1./norm(a));
}

Mvec fromPoint(vec3 v) {
    Mvec res;
    float d = dot(v, v);
    res.c1 = v.x;
    res.c2 = v.y;
    res.c3 = v.z;
    res.c4 = 1. - 0.5 * d;
    res.c5 = 1. + 0.5 * d;
    return res;
}

Mvec fromDirection(vec3 v) {
    Mvec res;
    res.c1 = v.x;
    res.c2 = v.y;
    res.c3 = v.z;
    return res;
}

Mvec createDualSphere(vec3 center, float radiusSq) {
    Mvec res = fromPoint(center);
    res.c4 += 0.5 * radiusSq;
    res.c5 -= 0.5 * radiusSq;
    return res;
}

void extractPointPair(Mvec ptPair, out Mvec p1, out Mvec p2) {
    Mvec ei;
    ei.c4 = -0.5;
    ei.c5 =  0.5;
    Mvec invDenominator = inv(inner(ei, ptPair));
    Mvec truc; truc.c0 = sqrt(abs(inner(ptPair, ptPair).c0));
    p1 = mul(
            add(
                ptPair,
                truc
            ),
            invDenominator
    );
    p2 = mul(
            sub(
                ptPair,
                truc
            ),
            invDenominator
    );
}

void main()
{
    Mvec e1, e2, e3, e4, e5, e0, ei;
    e1.c1 = 1.;
    e2.c2 = 1.;
    e3.c3 = 1.;
    e4.c4 = 1.;
    e5.c5 = 1.;
    e0 = add(e4, e5);
    ei = scale(sub(e5, e4), 0.5);
    vec3 col = vec3(0.);
    // Setup camera
    Mvec ro = fromPoint(uCamPos);
    Mvec rd = fromDirection(
        normalize(
              uCamX * (vTexCoords.x - 0.5) * uAspectRatio
            + uCamY * (vTexCoords.y - 0.5)
            - uCamZ * uFocalLength
        )
    );
    // Scene params
    float sphereRadius = 2.5;
    Mvec sphereCenter = fromPoint(vec3(0.));
    Mvec dualSphere = createDualSphere(vec3(0.), sphereRadius * sphereRadius);
    Mvec lightDir = normalized(fromDirection(vec3(0., -1., 0.3)));
    // Ray Tracing
    Mvec rayLine = outer(outer(ro, rd), ei);
    Mvec intersectionPtPair = dual(outer(dualSphere, dual(rayLine)));
    float ptPairSq = inner(intersectionPtPair, intersectionPtPair).c0;
    //if (ptPairSq > 0) {
		// Normal
		Mvec p1, p2;
		extractPointPair(intersectionPtPair, p1, p2);
        vec3 ro2p2 = vec3(
            p2.c1 - ro.c1,
            p2.c2 - ro.c2,
            p2.c3 - ro.c3
        );
        vec3 rdLA = vec3(rd.c1, rd.c2, rd.c3);
		Mvec normal;
        if (dot(rdLA, ro2p2) < 0) {
            vec3 ro2p1 = vec3(
                p1.c1 - ro.c1,
                p1.c2 - ro.c2,
                p1.c3 - ro.c3
            );
            if (dot(rdLA, ro2p1) > 0) {
                normal = normalized(sub(p1, sphereCenter));
            }
        }
        else {
            normal = normalized(sub(p2, sphereCenter));
        }
		//
		float t = inner(lightDir, normal).c0;
		col = vec3(max(t, 0.)*0.99+0.01);
    //}
    if (ptPairSq > 0) {
        
    }
    else {
        // Anti-aliasing
        col *= smoothstep(-0.1, 0., ptPairSq);
    }
    // 
    col = pow(max(col, vec3(0.)), vec3(.4545)); // gamma correction
    gl_FragColor = vec4(col,1.0);
}