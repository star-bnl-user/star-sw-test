#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "TCernLib.h"
#include "TMath.h"
#include "TMath.h"
#include "StvUtil/StvNodePars.h"
#include "StvUtil/StvDebug.h"
#include "Stv/StvToolkit.h"

  static const int idx66[6][6] =
  {{ 0, 1, 3, 6,10,15},{ 1, 2, 4, 7,11,16},{ 3, 4, 5, 8,12,17}
  ,{ 6, 7, 8, 9,13,18},{10,11,12,13,14,19},{15,16,17,18,19,20}};
static const double recvCORRMAX  = 0.99;
static const double chekCORRMAX  = 0.9999;

static double MAXTAN = 100;
//                              x   y   z  psi   pt  tan      cur
static double MAXNODPARS[]   ={555,555,555,6.66,111, MAXTAN+1, .1};
//                                   h    z   a     l   ptin
static const double MAXFITPARS[]   ={1.0 ,1.0,0.5 ,0.5 ,20  };
static const double BIGFITPARS[]   ={0.1 ,0.1,0.1 ,0.1 ,0.01},BIGFITPART=0.01;
static const double MAXFITERR[5]   ={0.5,0.5 ,0.3 ,.3 ,10};
static const double MAXERRFACT     = 1;
//______________________________________________________________________________ 
void Multiply(Mtx55D_t &res, const Mtx55D_t &A,const Mtx55D_t &B)
{
  memset(res[0],0,5*5*sizeof(res[0][0]));
  for (int j=0;j<5;j++) {
    const double *Bj = B[j];
    for (int i=0;i<5;i++) {
    const double &aij = A[i][j];
    if (!aij) continue;
          double *resi =res[i];
    for (int k=0;k<5;k++) {
      const double &bjk = Bj[k];
      if (!bjk) continue;
      resi[k] += aij*bjk;
  } } }
}
//______________________________________________________________________________ 
void Multiply(double *res, const Mtx55D_t &A,const double *B)
{
  for (int i=0;i<5;i++) {
    double S = 0;
    const double *Ai = A[i];
    for (int j=0;j<5;j++) {if (!Ai[j]) continue;S += Ai[j]*B[j];}
    res[i]=S;
  }
}
//______________________________________________________________________________ 
void Invert(Mtx55D_t &to, const Mtx55D_t &fr)
{
assert(0);
}
//______________________________________________________________________________ 
void Testik(const Mtx55D_t &tt)
{
   TMatrixD T(5,5,tt[0]);
   TMatrixD Tt(T); Tt.T();
   TMatrixD T1 = Tt*T;
   printf("Det = %g ============================\n",T.Determinant());
   T.Print();
   T1.Print();
}
#if 0
//______________________________________________________________________________ 
double StvTrackNode::sinX(double x)
{
  double x2 = x*x;
  if (x2>0.5) return (sin(x)-x)/x2/x;
  double nom = -1./6;
  double sum = nom;
  for (int it=4;1;it+=2) {
    nom = -nom*x2/(it*(it+1));
    sum +=nom;
    if (fabs(nom) <= 1e-10*fabs(sum)) break;
  }
  return sum;
} 
//______________________________________________________________________________
void StvTrackNode::mult6(double Rot[kNPars][kNPars],const double Pro[kNPars][kNPars]) 
{
  double T[kNPars][kNPars];

  if (!Rot[0][0]) {memcpy(Rot[0],Pro[0],sizeof(T)); return;}

  memcpy(T[0],Pro[0],sizeof(T));

  for (int i=0;i<kNPars;i++) {
  for (int j=0;j<kNPars;j++) {
    if(!Rot[i][j]) continue;
    for (int k=0;k<kNPars;k++) {
      if (!Pro[k][i]) continue;
      T[k][j] += Pro[k][i]*Rot[i][j];
  }}} 
  for (int i=0;i<kNPars;i++) {
  for (int k=0;k<kNPars;k++) {
    Rot[i][k] += T[i][k];
}}
}     
#endif //0
//------------------------------------------------------------------------------
void StvFitDers::Reverse() 
{
static const int mius[]= {1,2,5,8,9,10,13,14,0};
  for (int i=0;mius[i];i++) {mMtx[0][mius[i]]=-mMtx[0][mius[i]];}
}
//______________________________________________________________________________
int StvNodePars::check(const char *pri) const
{

  assert(_hz);
  int ierr=0;
//  temp test
  double tmp = _curv - _ptin* _hz;
//		1km for 1GeV is a zero field
  if (fabs(_hz)>=1e-5 && fabs(tmp)> 1e-3*fabs(_curv)) {ierr=1001; 	   	goto FAILED;}
  for (int i=0;i<=kNPars;i++){if (fabs(P[i]) > MAXNODPARS[i]) {ierr = i+ 1;	goto FAILED;}} 

  for (int i=-2;i<0;i++)     {if (fabs(P[i]) > 1.0001)        {ierr = i+12;	goto FAILED;}} 
  tmp = fabs(cos(_psi)-_cosCA);
  ierr = 1002; if (tmp>1e-4) 							goto FAILED;
  tmp = fabs(sin(_psi)-_sinCA);
  ierr = 1003; if (tmp>1e-4) 							goto FAILED;
  ierr = 1004; if (fabs(_z) <= 1e-10) 						goto FAILED;
  return 0;
FAILED: 
  if (!pri ) return ierr;
  printf("StvNodePars::check(%s) == FAILED(%d)\n",pri,ierr);  print();
  return ierr;
} 
//______________________________________________________________________________
StvNodePars &StvNodePars::merge(double wt,StvNodePars &other)
{
  assert(_hz);
   double wt0 = 1.-wt;
   for (int i=0;i<kNPars+1;i++) {P[i] = wt0*P[i] + wt*other.P[i];}
   ready();
   return *this;
}

//______________________________________________________________________________
void StvNodePars::set(const THelixTrack *th, double Hz)
{
  memcpy(&_x,th->Pos(),3*sizeof(_x));
  _psi = atan2(th->Dir()[1],th->Dir()[0]);
  double sinL = th->Dir()[2];
  double cosL = sqrt((1-sinL)*(1+sinL));
  _tanl = sinL/cosL;
  
  _cosCA = th->Dir()[0]/cosL;
  _sinCA = th->Dir()[1]/cosL;
  _curv = th->GetRho();
  _hz =  Hz;
  assert(_hz);
   if (fabs(_curv) > fabs(1e-6*_hz)) 	{_ptin = _curv/_hz;              } 
   else 				{_ptin = 1e-6; _curv = _ptin*_hz;}
} 

//______________________________________________________________________________
void StvNodePars::get(THelixTrack *th) const
{
  assert(_hz);
  double dir[3]={_cosCA,_sinCA,_tanl};
  th->Set(&_x,dir,_curv);
}
//______________________________________________________________________________
void StvNodePars::move(double dLxy)
{
  double dcCA,dsCA,dC,dS,dCR,dSR,dX,dY,dZ;
  double dPhi = _curv*dLxy;

  assert(_hz);
  if (fabs(dPhi) < 0.1) {
    dCR = -dLxy*dPhi/2; dSR = dLxy*(1-dPhi*dPhi/6);
    dC  = dCR*_curv;    dS  = dSR*_curv;
  } else {
    dC = cos(dPhi)-1; dS = sin(dPhi);
    dCR = dC/_curv;   dSR = dS/_curv;
  }
  dX = (_cosCA*dSR + _sinCA*dCR);
  dY = (_sinCA*dSR - _cosCA*dCR);
  dZ = dLxy*_tanl;
  dcCA = _cosCA*dC - _sinCA*dS;
  dsCA = _sinCA*dC + _cosCA*dS;
  _cosCA+=dcCA; _sinCA+=dsCA; _x+=dX; _y+=dY; _z+=dZ; _psi+=dPhi;
}
//______________________________________________________________________________
void StvNodePars::moveToR(double R)
{
  double myR2  = _x*_x     + _y*_y;
  double myDot = _x*_cosCA + _y*_sinCA;
  double dR2 = R*R-myR2;
  double dis = myDot*myDot+dR2;
  assert(_hz);
  if (dis<0) dis = 0;
  dis = sqrt(dis);
  double dL = (dR2)/(dis+fabs(myDot));
  if (myDot<0) dL = -dL;
  move(dL);
//  assert(fabs(_x*_x+ _y*_y-R*R)/(2*R)<1e-2);
}

//______________________________________________________________________________
StvNodePars &StvNodePars::operator=(const StvNodePars& fr) 
{
  if (&fr==this)	return *this;
  memcpy(this,&fr,sizeof(*this));
  assert(_hz);
  return *this;
}
//______________________________________________________________________________
const StvFitPars &StvNodePars::operator-(const StvNodePars& sub) const
{
static StvFitPars fp;

//  assert(isReady());
//  assert(sub.isReady());

  double cos2L = 1./(1+sub._tanl*sub._tanl); 
  double cosL  = sqrt(cos2L);
  double sinL  = sub._tanl*cosL;
  double dx = _x-sub._x;
  double dy = _y-sub._y;
  double dz = _z-sub._z;
  
  fp.mH = dx*(    -sub._sinCA)+ dy*(     sub._cosCA);
  fp.mZ = dx*(-sinL*sub._cosCA)+ dy*(-sinL*sub._sinCA) +dz*cosL;
  fp.mA = (_psi -sub._psi )*cosL;
  if      (fp.mA < -M_PI) {fp.mA += M_PI*2;}
  else if (fp.mA >  M_PI) {fp.mA -= M_PI*2;}
  fp.mP = (_ptin-sub._ptin);
  double tL = (_tanl-sub._tanl)/(1+_tanl*sub._tanl);
  fp.mL = tL*(1+tL*tL*(-1./3+tL*tL/5)); 
  return fp;
}
//______________________________________________________________________________
void StvNodePars::operator+=(const StvFitPars &fp)
{
//  assert(isReady());
  assert(_hz);
  double cos2L = 1./(1+_tanl*_tanl); 
  double cosL  = sqrt(cos2L);
  double sinL  = _tanl*cosL;
  _x += -_sinCA*fp.mH - sinL*_cosCA*fp.mZ;
  _y +=  _cosCA*fp.mH - sinL*_sinCA*fp.mZ;
  _z +=                 cosL       *fp.mZ;

  double a = fp.mA/cosL,cA,sA;
  if (fabs(a) < 0.01) {sA = a*(1-a*a/6); cA = 1-a*a/2;}
  else                {sA = sin(a);      cA = cos(a) ;} 
 _psi   += a;
  double cosCA = _cosCA;
  _cosCA = cosCA*cA-_sinCA*sA;
  _sinCA = cosCA*sA+_sinCA*cA;

  _ptin  += fp.mP;

  double l = fp.mL,tL;
  if (fabs(l) < 0.1) {tL = l*(1+l*l/3);}
  else               {tL = tan(l)     ;}
  _tanl = (_tanl+tL)/(1.-_tanl*tL);

  if (_tanl < -MAXTAN) _tanl = -MAXTAN;
  if (_tanl >  MAXTAN) _tanl =  MAXTAN;
  _curv   = _hz *_ptin;
  if (fabs( _cosCA)>1 || fabs( _sinCA)>1) ready();
//  assert(isReady());
  assert(!check("StvNodePars::operator+=") || 1);
}
//______________________________________________________________________________
void StvNodePars::print() const
{
static const char* tit[]={"cosCA","sinCA","X","Y","Z","Eta","Ptin","TanL","Curv",0};
  for (int i=-2;i<kNPars+1;i++) {printf("%s = %g, ",tit[i+2],P[i]);}
  printf("\n");
}   
//______________________________________________________________________________
void StvNodePars::convert( StvFitDers &fitDer, const StvHlxDers &hlxDer) const
{
enum {kHf,kZf,kAf,kLf,kPf};
enum {kHh,kAh,kCh,kZh,kLh};

  double cosL = 1/sqrt(1+_tanl*_tanl);
  double sinL = _tanl*cosL;
  double rho = _curv;
  double mHz = _hz;

  assert(_hz);
  memset(fitDer[0],0,25*sizeof(fitDer[0][0]));
//====================================
fitDer[kHf][kHf] = hlxDer[kHh][kHh];
fitDer[kHf][kZf] = hlxDer[kHh][kAh]*(sinL*rho);
fitDer[kHf][kAf] = hlxDer[kHh][kAh]*1/cosL;
fitDer[kHf][kPf] = hlxDer[kHh][kCh]*(mHz);
fitDer[kZf][kHf] = cosL*hlxDer[kZh][kHh];
fitDer[kZf][kZf] = cosL*(hlxDer[kZh][kAh]*(sinL*rho)) + 1;
fitDer[kZf][kAf] = cosL*hlxDer[kZh][kAh]*1/cosL;
fitDer[kZf][kLf] = cosL*hlxDer[kZh][kLh];
fitDer[kZf][kPf] = cosL*hlxDer[kZh][kCh]*(mHz);
fitDer[kAf][kHf] = cosL*hlxDer[kAh][kHh] + (-sinL*cosL*cosL*rho)*hlxDer[kZh][kHh];
fitDer[kAf][kZf] = cosL*hlxDer[kAh][kAh]*(sinL*rho) + (-sinL*cosL*cosL*rho)*(hlxDer[kZh][kAh]*(sinL*rho) + 1/cosL);
fitDer[kAf][kAf] = hlxDer[kAh][kAh] + (-sinL*cosL*cosL*rho)*hlxDer[kZh][kAh]*1/cosL;
fitDer[kAf][kLf] = (-sinL*cosL*cosL*rho)*hlxDer[kZh][kLh];
fitDer[kAf][kPf] = cosL*hlxDer[kAh][kCh]*(mHz) + (-sinL*cosL*cosL*rho)*hlxDer[kZh][kCh]*(mHz);
fitDer[kLf][kLf] = 1;
fitDer[kPf][kPf] = 1;
//====================================


}
//______________________________________________________________________________
int StvNodePars::isReady( ) const
{
  if (fabs(_cosCA-cos(_psi))>1e-5) return 0;
  if (fabs(_sinCA-sin(_psi))>1e-5) return 0;
  if (fabs(_curv -_hz*_ptin)>1e-5) return 0;
  return 1;
}

//______________________________________________________________________________
//______________________________________________________________________________
void StvHitErrs::rotate(double angle)
{
  double t[2][2];
  t[0][0] = cos(angle); t[0][1] = -sin(angle);
  t[1][0] = -t[0][1]  ; t[1][1] = t[0][0];
  double r[3];
  TCL::trasat(t[0],&hXX,r,2,2);
  TCL::ucopy(r,&hXX,3);
}


//______________________________________________________________________________
//______________________________________________________________________________
StvFitErrs::StvFitErrs(double hh,double hz,double zz)
{
  memset(this,0,sizeof(*this));
  mHH=hh;mHZ=hz;mZZ=zz;mHz = 3e33;
}
//______________________________________________________________________________
double StvFitErrs::Sign() const {return EmxSign(5,Arr());}
//______________________________________________________________________________
const StvFitErrs &StvFitErrs::operator*(const StvFitDers &how) const
{
static StvFitErrs myFitErrs;
  TCL::trasat(how[0],Arr(),myFitErrs.Arr(),5,5);
  myFitErrs.mHz = mHz;
  myFitErrs.Recov();
  return myFitErrs;
}  
//______________________________________________________________________________
void StvFitErrs::Reset()
{
  memset(this,0,sizeof(*this));
  mHH =  MAXFITERR[0]*MAXFITERR[0]*MAXERRFACT;
  mZZ =  MAXFITERR[1]*MAXFITERR[1]*MAXERRFACT;
  mAA =  MAXFITERR[2]*MAXFITERR[2]*MAXERRFACT;
  mLL =  MAXFITERR[3]*MAXFITERR[3]*MAXERRFACT;
  mPP =  MAXFITERR[4]*MAXFITERR[4]*MAXERRFACT;
  mHz = 3e33;
}

//______________________________________________________________________________
void StvFitErrs::Set(const THelixTrack *he, double hz)
{
mHz = hz;assert(fabs(hz)<0.002);
const THEmx_t *emx = he->Emx();
double  cosL = he->GetCos();
double  sinL = he->GetSin();
double  rho  = he->GetRho();
//double  dAdZ = -sinL*rho;

mHH = emx->mHH;
mHZ = cosL*emx->mHZ;
mZZ = cosL*emx->mZZ*cosL;
mHA = cosL*emx->mHA + (-sinL*cosL*cosL*rho)*emx->mHZ;
mZA = (cosL*emx->mAZ + (-sinL*cosL*cosL*rho)*emx->mZZ)*cosL;
mAA = (cosL*emx->mAA + (-sinL*cosL*cosL*rho)*emx->mAZ)*cosL + (cosL*emx->mAZ + (-sinL*cosL*cosL*rho)*emx->mZZ)*(-sinL*cosL*cosL*rho);
mHL = emx->mHL;
mZL = emx->mZL*cosL;
mAL = emx->mAL*cosL + emx->mZL*(-sinL*cosL*cosL*rho);
mLL = emx->mLL;
mHP = (1/mHz)*emx->mHC;
mZP = (1/mHz)*emx->mCZ*cosL;
mAP = (1/mHz)*emx->mAC*cosL + (1/mHz)*emx->mCZ*(-sinL*cosL*cosL*rho);
mLP = (1/mHz)*emx->mCL;
mPP = (1/mHz)*emx->mCC*(1/mHz);


 Recov();
}  
//______________________________________________________________________________
void StvFitErrs::Get(THelixTrack *he) const
{
  assert(mHz && fabs(mHz)<.002);
  he->SetEmx(0);
  THEmx_t *emx = he->Emx();
  double  cosL = he->GetCos();
  double  sinL = he->GetSin();
  double  rho  = he->GetRho();
  double  dAdZeta = sinL*rho;

emx->mHH = mHH;
emx->mHA = dAdZeta*mHZ + 1/cosL*mHA;
emx->mAA = dAdZeta*mZZ*dAdZeta + 2/cosL*(mZA*dAdZeta) + mAA/cosL/cosL;
emx->mHC = (mHz)*mHP;
emx->mAC = (mHz)*(mZP*dAdZeta + mAP/cosL);
emx->mCC = (mHz)*mPP*(mHz);
emx->mHZ = 1/cosL*mHZ;
emx->mAZ = 1/cosL*(mZZ*dAdZeta + mZA*1/cosL);
emx->mCZ = 1/cosL*mZP*(mHz);
emx->mZZ = 1/cosL*mZZ*1/cosL;
emx->mHL = mHL;
emx->mAL = mZL*dAdZeta + mAL*1/cosL;
emx->mCL = mLP*(mHz);
emx->mZL = mZL*1/cosL;
emx->mLL = mLL;


}  
//_____________________________________________________________________________
void StvFitErrs::Set(const StvFitErrs &fr,double errFactor)
{
  Reset();
  mHz = fr.mHz;  
  assert(mHz && fabs(mHz)<0.002);

  double const *e =fr.Arr();
  double       *ee=   Arr();
  int nerr = 0;
  for (int i=0,li=0;i< 5;li+=++i) {
    for (int j=0;j<=i;j++) {
    double myMax =  MAXFITERR[i]*MAXFITERR[j];
    ee[li+j] = e[li+j]*errFactor;
    if (fabs(ee[li+j]) > myMax) nerr++;;
  } }
  if (nerr) Recov();
}
//_____________________________________________________________________________
void StvFitErrs::Backward()
{
  mHA*=-1; mAP*=-1; mHZ*=-1; mZP*=-1; mAL*=-1; mZL*=-1;
}
//_____________________________________________________________________________
int StvFitErrs::Check(const char *tit) const
{
  if (!StvDebug::mgCheck) return 0;
  ((StvFitErrs*)((void*)this))->Recov();
  int ierr=0;
  double dia[5];const double *e=&mHH;
  for (int i=0,li=0;i< 5;li+=++i) {
    dia[i]=e[li+i];
    if (dia[i]< 1e-8*MAXFITERR[i]*MAXFITERR[i]) {ierr = i+1; goto ERR;}
    if (dia[i]> 1e+4*MAXFITERR[i]*MAXFITERR[i]) {ierr = i+6; goto ERR;}
    for (int j=0;j<i;j++) {
       if (e[li+j]*e[li+j]>=dia[i]*dia[j]){ierr = 100+10*i+j;goto ERR;}
    } }
  if (!(mHz && fabs(mHz) <.002)) {ierr = 1001; goto ERR;}
  return 0;
ERR: if (!tit) return ierr;
  printf("StvFitErrs::Check(%s)=%d\n",tit,ierr);
  Print();
  return ierr;
}     
//_____________________________________________________________________________
int StvFitErrs::Recov()
{

  double dia[5],fak[5];double *e=&mHH;

  int nerr=0;
  if (!StvDebug::mgRecov) return 0;
//		Check diag errs
  for (int i=0,li=0;i< 5;li+=++i) {
    fak[i]=1;
    if (e[li+i] < MAXFITERR[i]*MAXFITERR[i]) continue;
    fak[i] = 0.99*MAXFITERR[i]/sqrt(e[li+i]); nerr++;
  };
  if (nerr) {  		//Recovery
    for (int i=0,li=0;i< 5;li+=++i) {
      for (int j=0;j<=i;j++) {
        e[li+j]*=fak[i]*fak[j];
  } } }

  int jerr=0;
//		Check correlations & Recovery
  for (int i=0,li=0;i< 5;li+=++i) {
    dia[i]=e[li+i];
    for (int j=0;j<i;j++) {
       if (e[li+j]*e[li+j]>=dia[i]*dia[j]) jerr++;
  } }
  if (jerr) {  		//Recovery
    for (int i=0,li=0;i< 5;li+=++i) {
      for (int j=0;j<i;j++) {e[li+j]=0;}
  } } 
  
  return nerr+jerr;
}     

//_____________________________________________________________________________
 void StvFitErrs::Print(const char *tit) const
{
static const char *N="HZALP";
  if (!tit) tit = "";
  printf("StvFitErrs::Print(%s) ==\n",tit);
  const double *e = &mHH;
  for (int i=0,li=0;i< 5;li+=++i) {
    printf("%c ",N[i]);
    for (int j=0;j<=i;j++) {
    printf("%g\t",e[li+j]);} 
    printf("\n");
  }
}
//_____________________________________________________________________________
int StvFitPars::TooBig(const StvNodePars &np) const
{
  
  double space = BIGFITPART*(fabs(np._x)+fabs(np._y)+fabs(np._z));
  for (int i=0;i<2;i++) {if (fabs(Arr()[i]) > BIGFITPARS[i]+space) return i+1;};
  for (int i=2;i<4;i++) {if (fabs(Arr()[i]) > BIGFITPARS[i]      ) return i+1;};

  if (fabs(mP) > BIGFITPARS[4]+fabs(np._ptin)*BIGFITPART)     	   return 4+1;
  return 0;
}
//_____________________________________________________________________________
int StvFitPars::Check(const char *tit) const
{
  int ifail = 0;
  for (int i=0;i<5;i++) {if (fabs(Arr()[i]) > MAXFITPARS[i]){ifail=i+1;break;}};
  if (!ifail) return 0;
  if (!tit || !tit[0]) return ifail;
  TString ts(tit);ts +=" *** Check = "; ts+=ifail;ts +=" ***";
  Print(ts.Data());
  return 0;
}
//_____________________________________________________________________________
void StvFitPars::Print(const char *tit) const
{
static const char* Nams[]={"mH","mZ","mA","mL","mP",0};
  if (tit && tit[0]) printf("StvFitPars::Print(%s)\n",tit);
  for (int i=0;Nams[i]; i++) {printf("%s=%g ",Nams[i],Arr()[i]);}
  printf("\n");
}
//_____________________________________________________________________________

//_____________________________________________________________________________
/**
   returns the node information
   double x[6],  : state, for a definition, in radial implementation
                   rad  - radius at start (cm). See also comments
                   phi  - azimuthal angle  (in rad)      
                   z    - z-coord. (cm)                 
                   tanl - tan(dip) =pz/pt               
                   psi  - azimuthal angle of pT vector (in rads)     
                   pti  - signed invert Pt.//  pti = curv/hz 
   double cc[15] : error matrix of the state "x" rad is fixed
                       code definition adopted here, where:
   PhiPhi;
   ZPhi     ,ZZ;                       
   TanlPhi  ,TanlZ ,TanlTanl,                 
   PhiPsi   ,ZPsi  ,TanlPsi , PsiPsi ,           
   PhiPti  ,ZPti ,TanlPti, PsiPti, PtiPti     

*/
//_____________________________________________________________________________
void StvNodePars::GetRadial(double radPar[6],double radErr[15],const StvFitErrs *fitErr) const
{
//Remind StvFitPars:
//double mH;	// direction perpendicular movement and Z
//double mZ;	// Pseudo Z, direction perpendicular movement & H
//double mA;	// Angle in XY. cos(A),sin(A),T moving direction
//double mL;	// Angle lambda in Rxy/Z
//double mP;	// 1/pt with curvature sign

  enum {jRad=0,jPhi,jZ,jTan,jPsi,jPti};
  enum {jRPhiRPhi=0
       ,jRPhiZ  ,jZZ
       ,jRPhiTan,jZTan,jTanTan
       ,jRPhiPsi,jZPsi,jTanPsi,jPsiPsi
       ,jRPhiPti,jZPti,jTanPti,jPsiPti,jPtiPti};
  double r2xy = _x*_x+_y*_y, rxy=sqrt(r2xy);
  double cos2L = 1./(1+_tanl*_tanl);
  double cosL = sqrt(cos2L);
  double sinL = _tanl*cosL;

  radPar[jRad] = rxy;
  radPar[jPhi] = atan2(_y,_x);
  radPar[jZ  ] = _z;
  radPar[jTan] = _tanl;
  radPar[jPsi] = _psi;
  radPar[jPti] = _ptin;
  if (!radErr) return;


  double A[6][5]=
/*		H		   Z    A       L       P */
/*--------------------------------------------------------*/
/*x*/ {{  -_sinCA,	-sinL*_cosCA,	0,	0,	0},
/*y*/  {   _cosCA,   	-sinL*_sinCA,	0,	0,	0},
/*z*/  {        0,           	cosL,	0,	0,	0},
/*A*/  {        0,           	   0,	1,	0,	0},
/*L*/  {        0,           	   0,	0,	1,	0},
/*P*/  {        0,           	   0,	0,	0,	1}};

  double g[6][1]=
      {{ cosL*_cosCA 	},
       { cosL*_sinCA 	},
       { sinL        	},
       { cosL*cosL*_curv},
       { 0         	},
       { 0         	}};

  double myX[1][6] = {{_x,_y,0,0,0,0}};

  TMatrixD MA(6,5,A[0]),Mg(6,1,g[0]),Mx(1,6,myX[0]);
  double xg = -1./(Mx*Mg)[0][0];
  TMatrixD T1 = MA+(Mg*(Mx*MA))*xg;

//
double dRad[5][6] = 
/*                        x,       y,   z,      A,      L,  P*/
/*-----------------------------------------------------------*/
/*jRPhi*/	{{-  _y/rxy,  _x/rxy,   0,      0,      0,  0}
/*jZ  */	,{        0,       0,   1,      0,      0,  0}
/*jTan*/	,{        0,       0,   0,      0,1/cos2L,  0}
/*jPsi*/	,{        0,       0,   0, 1/cosL,      0,  0}
/*jPti*/	,{        0,       0,   0,      0,      0,  1}};
/*-----------------------------------------------------------*/

  TMatrixD T2=TMatrixD(5,6,dRad[0])*T1;
  TCL::trasat(T2.GetMatrixArray(),fitErr->Arr(),radErr,5,5); 

}
#if 0
//_____________________________________________________________________________
void StvNodePars::GetImpact(StvImpact *imp,const StvFitErrs *fe)  const
{
    /// signed impact parameter; Signed in such a way that:
    ///     x =  -impact*sin(Psi)
    ///     y =   impact*cos(Psi)
  imp->mImp = -(_x*(-_sinCA) + _y*(_cosCA));
  double tst = _x*(_cosCA) + _y*(_sinCA);
  assert(fabs(tst)<1e-5 || fabs(imp->mImp) > 1000*fabs(tst));
  imp->mZ   = _z;
  imp->mPsi = _psi;
  imp->mPti = _ptin;
  imp->mTan = _tanl;
  imp->mCurv= _curv;
  if (!fe) return;

  double cos2L = 1./(1+_tanl*_tanl);
  double cosL  = sqrt(cos2L);
  double sinL  = cosL*_tanl;
  double myImp   = imp->mImp;
//		StvFitPars
// mH;	 direction perpendicular movement and Z
// mZ;	 Pseudo Z, direction perpendicular movement & H
// mA;	 Angle in XY. cos(A),sin(A),T moving direction
// mL;	 Angle lambda in Rxy/Z
// mP;	 1/pt with curvature sign

//      		Impacts
//     float  mImpImp;
//     float  mZImp, mZZ;
//     float  mPsiImp, mPsiZ, mPsiPsi;
//     float  mPtiImp, mPtiZ, mPtiPsi, mPtiPti;
//     float  mTanImp, mTanZ, mTanPsi, mTanPti, mTanTan;
 double T[5][5] = {
/*        H,   Zeta,         	  A,       L,P */
/*----------------------------------------------*/
/*Imp*/  {-1,     0,         	   0,	    0,0},
/*Z  */  { 0,1/cosL,               0, 	    0,0},
/*Psi*/  { 0,_curv*sinL,          1/cosL,   0,0},
/*Pti*/  { 0,     0,         	   0, 	    0,1},
/*Tan*/  { 0,     0,         	   0, 1/cos2L,0}};
/*---------------------------------------------*/

  double qwe[15];
  TCL::trasat(T[0],fe->Arr(),qwe,5,5); 
  TCL::ucopy(qwe,&imp->mImpImp,15);
}
#endif
//_____________________________________________________________________________
void StvNodePars::GetPrimial(double radPar[6],double radErr[15],const StvFitErrs *fitErr) const
{
/// This is GetRadial for primary track. Radial representation of errors
/// is senseless for primary. But our oldfashiond TPT format demanding it. 
/// All space errors supposed to be zeros.

//Remind StvFitPars:
//double mH;	// direction perpendicular movement and Z
//double mZ;	// Pseudo Z, direction perpendicular movement & H
//double mA;	// Angle in XY. cos(A),sin(A),T moving direction
//double mL;	// Angle lambda in Rxy/Z
//double mP;	// 1/pt with curvature sign

  enum {jRad =0,jPhi   ,jZ  ,jTan,jPsi,jPti};

  double r2xy = _x*_x+_y*_y, rxy=sqrt(r2xy);
  double cos2L = 1./(1+_tanl*_tanl);
  double cosL = sqrt(cos2L);

  radPar[jRad] = rxy;
  radPar[jPhi] = atan2(_y,_x);
  radPar[jZ  ] = _z;
  radPar[jTan] = _tanl;
  radPar[jPsi] = _psi;
  radPar[jPti] = _ptin;
  if (!radErr) return;

double T[5][5] = 
/*                     H,     Z,     A,      L,      P*/
/*----------------------------------------------------*/
/*jRPhi*/	{{     0,     0,     0,      0,      0}
/*jZ  */	,{     0,     0,     0,      0,      0}
/*jTan*/	,{     0,     0,     0,      1/cos2L,0}
/*jPsi*/	,{     0,     0,1/cosL,      0,      0}
/*jPti*/	,{     0,     0,     0,      0,      1}};
/*----------------------------------------------------*/

  TCL::trasat(T[0],fitErr->Arr(),radErr,5,5); 

}
//______________________________________________________________________________
const StvFitPars &StvFitPars::operator*(const StvFitDers &t) const  
{
static StvFitPars myPars;
  TCL::vmatl(t[0],Arr(),myPars.Arr(),5,5);
//  assert(!myPars.Check("StvFitPars::operator*") || 1);
  return myPars;
}
//_____________________________________________________________________________
void StvFitErrs::Get(const StvNodePars *np,  StvNodeErrs *ne) const
{
  double cos2L = 1./(np->_tanl*np->_tanl+1);
  double dTdL = 1./cos2L;
  double cosL = sqrt(cos2L);

//                       d/dH     d/dZ       d/dA     d/dLam d/dPti
  double T[6][5] = {{ np->_cosCA,   0,         0,         0,  0}   //dX
  		   ,{ np->_sinCA,   0,         0,         0,  0}   //dY
  		   ,{         0,    1/cosL,    0,         0,  0}   //dZ
  		   ,{         0,    0,         1,         0,  0}   //dEta
  		   ,{         0,    0,         0,         0,  1}   //dPti
  		   ,{         0,    0,         0,      dTdL,  0}}; //dTan

  assert(0);
  TCL::trasat(T[0],this->Arr(),ne->A,5,6); 

}
//______________________________________________________________________________
StvImpact::StvImpact() {memset(this,0,sizeof(*this));}

//______________________________________________________________________________
void StvImpact::Print(const char *opt) const
{
  if (!opt) opt = "";
  printf("StvImpact::Print(%s) ==\n",opt);

static const char* tit[]={"Imp","Z  ","Psi","Pti","Cur",0};
  const float* P=&mImp;
  for (int i=0;i<5;i++) {printf("%s = %g, ",tit[i],P[i]);}
  printf("\n");
  if (mImpImp<=0) return;
  const float *e = &mImpImp;
  for (int i=0,li=0;i< 5;li+=++i) {
    printf("%s ",tit[i]);
    for (int j=0;j<=i;j++) {
    printf("%g\t",e[li+j]);} 
    printf("\n");
  }


}   
#if 0 //
//____________________________________________________________
double EmxSign(int n,const double *a) 
{
   double ans=3e33;
   double buf[55];
   double *B = (n<=10) ? buf : new double[n];
   double *b = B;
   // trchlu.F -- translated by f2c (version 19970219).
   //
   //see original documentation of CERNLIB package F112 

   /* Local variables */
   int ipiv, kpiv, i__, j;
   double r__, dc;
   int id, kd;
   double sum;


   /* CERN PROGLIB# F112    TRCHLU          .VERSION KERNFOR  4.16  870601 */
   /* ORIG. 18/12/74 W.HART */


   /* Parameter adjuTments */
   --b;    --a;

   /* Function Body */
   ipiv = 0;

   i__ = 0;

   do {
      ++i__;
      ipiv += i__;
      kpiv = ipiv;
      r__ = a[ipiv];

      for (j = i__; j <= n; ++j) {
         sum = 0.;
         if (i__ == 1)       goto L40;
         if (r__ == 0.)      goto L42;
         id = ipiv - i__ + 1;
         kd = kpiv - i__ + 1;

         do {
            sum += b[kd] * b[id];
            ++kd;   ++id;
         } while (id < ipiv);

L40:
         sum = a[kpiv] - sum;
L42:
         if (j != i__) b[kpiv] = sum * r__;
         else {
            if (sum<ans) ans = sum;
            if (sum<0.) goto RETN;
            dc = sqrt(sum);
            b[kpiv] = dc;
            if (r__ > 0.)  r__ = (double)1. / dc;
         }
         kpiv += j;
      }

   } while  (i__ < n);

RETN: if (B!=buf) delete B; 
   return ans;
} /* trchlu_ */
#endif //0

//_____________________________________________________________________________
//_____________________________________________________________________________
#include "TRandom.h"
#include "TMatrixDSym.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TVector3.h"
#include "StarRoot/TRandomVector.h"
//_____________________________________________________________________________
ClassImp(StvNodeParsTest);
void StvNodeParsTest::Test()
{
  int saveRecov = StvDebug::mgRecov;  StvDebug::mgRecov=0;
  double thPars[7+15],Hz=0.000299792458 * 4.98478;
  for (int i=0;i<7+15;i++) {thPars[i]=gRandom->Rndm();}
  THelixTrack th(thPars,thPars+3,thPars[6]),thh;
  memcpy(thPars+3,th.Dir(),3*sizeof(double));
  th.SetEmx(thPars+7);
  
  StvNodePars pars;
  StvFitErrs  errs;
  pars.set(&th,Hz);
  errs.Set(&th,Hz);
  pars.reverse();
  errs.Backward();
  pars.get(&thh);
  errs.Get(&thh);
  thh.Backward();

  double thhPars[7+15];
  memcpy(thhPars,thh.Pos(),7*sizeof(double));
  memcpy(thhPars+7,thh.Emx()->Arr(),15*sizeof(double));
  int nerr=0;
  for (int i=0;i<7+15;i++) {
    if (fabs(thhPars[i]-thPars[i]) <1e-6) continue;
    nerr++;printf("%d = %g %g \n",i,thPars[i],thhPars[i]);}
  printf("nmErrs = %d\n",nerr);
  StvDebug::mgRecov=saveRecov;
}
//_____________________________________________________________________________
void StvNodeParsTest::TestGetRadial(int nEv)
{
StvFitErrs fE;
  int saveRecov = StvDebug::mgRecov;  StvDebug::mgRecov=0;
  double f = 0.01;
  TVectorD D(5);
  D[0] = 1.0*1.0	*f;
  D[1] = 2.0*2.0	*f;
  D[2] = 0.03*0.03	*f;
  D[3] = 0.04*0.04	*f;
  D[4] = 0.07*0.07	*f;
  TRandomVector RV(D);

  TMatrixD S(RV.GetMtx());
  S.Print();

  double *e = &fE.mHH;
  for (int i=0,li=0;i< 5;li+=++i) {
    for (int j=0;j<=i;j++    ) {
       e[li+j]= S[i][j];
  } }


  StvNodePars node;
  node.reset();
  double myRad = 100;
  double phi = gRandom->Rndm()*2*M_PI;
  node._x    = myRad*cos(phi);
  node._y    = myRad*sin(phi);
  node._z    = (gRandom->Rndm()-0.5)*200;
  node._psi  = phi + (gRandom->Rndm()-0.5);
  node._tanl = gRandom->Rndm();
  node._ptin = (gRandom->Rndm()-0.5);
  node._hz   = 0.000299792458 * 4.98478;
  node.ready();

  double radPar[6],radErr[15];
  node.GetRadial(radPar,radErr,&fE);
  TMatrixDSym SS(5);
  for (int i=0,li=0;i< 5;li+=++i) {
    for (int j=0;j<=i;j++    ) {
       SS[i][j]=radErr[li+j]; SS[j][i]=radErr[li+j];
  } }
  SS.Print();

  double rE[15]={0};
  for (int ev=0;ev<nEv;ev++) 
  {
    const TVectorD &res = RV.Gaus();
    StvNodePars myNode(node);
    StvFitPars  fitPars(res.GetMatrixArray());
    myNode +=fitPars;
//		Project it to x*x+y*y=r*r
    myNode.moveToR(myRad);
    assert(fabs(pow(myNode._x,2)+pow(myNode._y,2)-myRad*myRad)<1e-4*myRad);
    double rP[6];
    myNode.GetRadial(rP);
    assert(fabs(rP[0]-myRad)<1e-4);
    for (int i=0,li=0;i< 5;li+=++i) {
      for (int j=0;j<=i;j++    ) {
      rE[li+j]+= (rP[i+1]-radPar[i+1])*(rP[j+1]-radPar[j+1]);
    } }
  }  //End events
static const char *radTit[6]= {"Rad","Phi","Z  ","Tan","Psi","Pti"};
  double fak[5]={myRad,1,1,1,1};
  for (int i=0,li=0;i< 5;li+=++i){for (int j=0;j<=i;j++){rE[li+j]*=fak[i]*fak[j]/nEv;}}
  double qA=0,qAmax=0,dia[5];
  for (int i=0,li=0;i< 5;li+=++i) {
    dia[i]=radErr[li+i];
    for (int j=0;j<=i;j++) {
      double nor = sqrt(dia[i]*dia[j]);
      double dif = (rE[li+j]-radErr[li+j])/nor;
      printf("([%s][%s] \t%g = \t%g \t%g\n",radTit[i+1],radTit[j+1],radErr[li+j]/nor,rE[li+j]/nor,dif);
      dif = fabs(dif);
      qA+= (dif); if (dif>qAmax) qAmax=dif;
  } }
  qA/=15;
  printf("Quality %g < %g < 1\n",qA,qAmax);
  StvDebug::mgRecov=saveRecov;
}

//_____________________________________________________________________________
static void Add(THelixTrack &ht,const double add[5]) 
{
// add = H,A,C,Z,L
  TVector3  pos(ht.Pos()),dir(ht.Dir()),ort(-dir[1],dir[0],0.);
  ort = ort.Unit();
  double rho=ht.GetRho();
  pos+=ort*add[0]; pos[2]+=add[3];
  dir.SetMagThetaPhi(1.,dir.Theta()-add[4],dir.Phi()+add[1]);
  double wk[7]={pos[0],pos[1],pos[2],dir[0],dir[1],dir[2],rho+add[2]};
  ht.Set(wk,wk+3,wk[6]);
}


//_____________________________________________________________________________
void StvNodeParsTest::TestErrProp(int nEv)
{

StvNodePars iP,iPR,oP,oPR;
THelixTrack iH,iHR,oH,ht;

THEmx_t oHE,oHER;
double dia[5],*e,*er,*vtx;

  iP._cosCA = 0.051522195951218416; iP._sinCA = -0.99867184876021664;  iP._x   = 56.80456301948584; 
  iP._y     = -179.95090442478528;  iP._z     = 16.833129146428401;    iP._psi = -1.5192513089402997; iP._ptin = -4.286089548109465; 
  iP._tanl  = -0.71077992742240803; iP._curv  = -0.0063779138641975935;iP._hz=(0.0014880496061989194);
  iP.ready();
StvFitErrs iE,oE,oER;
  iE.mHH = 0.0025928369042255385;  iE.mHZ = -4.9934860023454386e-11; iE.mZZ = 0.014598355970801268; iE.mHA = -0.00059887440419442305; 
  iE.mZA = 1.0958739205478152e-11; iE.mAA = 0.00026524379894739812;  iE.mHL = 3.463001237863329e-12; iE.mZL = -0.0016525557966380938; 
  iE.mAL = 8.3669926017237923e-13; iE.mLL = 0.00041855110437868546;  iE.mHP = 0.0043962440767417576; iE.mZP = -2.904206508909407e-11; 
  iE.mAP = -0.0041320793241820105; iE.mLP = -2.5031139398137018e-12; iE.mPP = 0.78568815092933286; iE.SetHz(0.0014880496061989194);

  oER*=0.;
  oHER.Clear();
  int saveRecov = StvDebug::mgRecov;  StvDebug::mgRecov=0;

//		Prepare error matrix for TRandomVector
  TMatrixDSym S(5);
  e = iE.Arr();
  for (int i=0,li=0;i< 5;li+=++i) {S[i][i] = e[li+i];}
  TRandomVector::RandRotate(S);

  for (int i=0,li=0;i< 5;li+=++i) {
    for (int j=0;j<=i;j++)        { e[li+j] = S[i][j];}}
//
  iE.Print("Input StvFitErrs");

  iP.get(&iH);	// nodePar => Helix
  iE.Get(&iH);	// fitErr  => HelixErr
  iE.Set(&iH,iP._hz);
  iE.Print("Input StvFitErrs => THEmx_t => StvFitErrs");


  iH.Emx()->Print("Input Helix Errs");
  oH = iH;
  double myDist = 33;
  oH.Move(myDist);		//Move helix to 100cm
  oP.set(&oH,iP._hz);		//helix100 => nodePar100
  oE.Set(&oH,iP._hz);		//helixErr100 => fitErr100
  oE.Print("Output StvFitErrs");
  oH.Emx()->Print("Output Helix Errs");

  vtx = oH.Pos();
  TVector3 Vtx(vtx);
  TVector3 Dir(oH.Dir());
  TVector3 Nxy(-Dir[1],Dir[0],0.); Nxy = Nxy.Unit();

static int iHELIX=0;

//		Prepare error matrix for TRandomVector
  e = (iHELIX) ? iH.Emx()->Arr() : iE.Arr();
  for (int i=0,li=0;i< 5;li+=++i) {
    for (int j=0;j<=i;j++    ) {
       S[i][j]=e[li+j]; S[j][i]=e[li+j];
    } }
  TRandomVector RV(S);

//		Event loop
  for (int ev=0;ev <= nEv;ev++) {
    iPR = iP;
    ht = iH;
//		Randomize fit parameters
    const TVectorD res = RV.Gaus();

    if (iHELIX) { //Randomize helix directly
      Add(ht,res.GetMatrixArray());
    } else {
      StvFitPars fp(res.GetMatrixArray()); iPR+=fp;
//		Create THelixTrack from StvNodePars
      iPR.get(&ht);
//		Set no error matrix to helix
      ht.SetEmx(0);
    }
      
// //		Change helix direction for fun
//     ht.Backward();
//		Move random helix to DCA point of etalon vertex
    double my100 = ht.Path(vtx);
    assert(fabs(my100-myDist) <20);
    ht.Move(my100);
    oPR.set(&ht,iP._hz);
    StvFitPars fp = (oPR-oP);
    double *e = oER.Arr();
    double *d = fp.Arr();
    for (int i=0,li=0;i< 5;li+=++i) {
      for (int j=0;j<=i;j++    ) {
        e[li+j]+=d[i]*d[j];
    } } 
    double dS = ht.Path(vtx[0],vtx[1]);
    ht.Move(dS);
    double D[5];
    TVector3 VPos(ht.Pos()),VDir(ht.Dir());
    D[0] = (VPos-Vtx)*Nxy;
    D[1] = VDir.DeltaPhi(Dir);
    D[2] = ht.GetRho()-oH.GetRho();
    D[3] = VPos[2]-Vtx[2];
    D[4] = -(VDir.Theta()-Dir.Theta());// "-" due to theta)= Pi-Lambda

    e = oHER.Arr();
    for (int i=0,li=0;i< 5;li+=++i) {
      for (int j=0;j<=i;j++    ) {
        e[li+j]+=D[i]*D[j];
    } } 


  }//EndEvts

  oER *= (1./nEv);
  oHER*= (1./nEv);
  
  printf("*** Check THelixTrack Error matrix ***\n");
  e = oH.Emx()->Arr();
  er = oHER.Arr();
  double qA=0,qAmax=0;
  for (int i=0,li=0;i< 5;li+=++i) {
    dia[i]=e[li+i];
    for (int j=0;j<=i;j++) {
    double dif = (er[li+j]-e[li+j])/sqrt(dia[i]*dia[j]);
    printf("(%d %d) \t%g = \t%g \t%g\n",i,j,er[li+j],e[li+j],dif);
    dif = fabs(dif);
    qA+= (dif); if (dif>qAmax) qAmax=dif;
  } }
  printf("Quality %g < %g < 1\n",qA,qAmax);


  printf("*** Check StvFitErr Error matrix ***\n");
  e = oE.Arr();
  er = oER.Arr();
  qA=0;qAmax=0;
  for (int i=0,li=0;i< 5;li+=++i) {
    dia[i]=e[li+i];
    for (int j=0;j<=i;j++) {
    double dif = (er[li+j]-e[li+j])/sqrt(dia[i]*dia[j]);
    printf("(%d %d) \t%g = \t%g \t%g\n",i,j,er[li+j],e[li+j],dif);
    dif = fabs(dif);
    qA+= (dif); if (dif>qAmax) qAmax=dif;
  } }
  qA/=15;
  printf("Quality %g < %g < 1\n",qA,qAmax);
  StvDebug::mgRecov=saveRecov;

}
//_____________________________________________________________________________
//_____________________________________________________________________________
void StvNodeParsTest::TestMtx() 
{
  int saveRecov = StvDebug::mgRecov;  StvDebug::mgRecov=0;
  double maxEps = 0;  
  double hz = 0.0014880496061989194;
  int nErr=0;
  int iR = 10+ gRandom->Rndm()*100;
  int iAlf=10+ gRandom->Rndm()*100;
  int iLam=10+ gRandom->Rndm()*100;
  double alf = iAlf/180.*M_PI;
  double lam = iLam/180.*M_PI;
  StvNodePars basePar,modiPar,baseEndPar,modiEndPar;
  double Rho = 1./iR;
  basePar._x=0.10;
  basePar._y=0.20; 
  basePar._z=0.30;
  basePar._psi=alf;
  basePar._ptin=Rho/hz;;  
  basePar._tanl=tan(lam);
  basePar._curv=Rho;  
  basePar._hz=hz;  
  basePar.ready();
  
  THelixTrack baseHlx,modiHlx,baseEndHlx,modiEndHlx;
  StvFitDers mtxStv,mtxNum;
  StvHlxDers mtxHlx;
  basePar.get(&baseHlx);
  double len = 33;
  baseEndHlx = baseHlx; baseEndHlx.Move(len,mtxHlx);
  baseEndPar.set(&baseEndHlx,hz);
  baseEndPar.convert(mtxStv , mtxHlx);

  double stpArr[]={0.1, 0.1, 3.14/180, 3.14/180, 0.1*basePar._ptin+1e-2};
  double fak=0.1;
  memset(mtxNum[0],0,sizeof(mtxNum));
  for (int ip=0;ip<5; ip++) { //Loop thru input parameters
    StvFitPars fp; fp.Arr()[ip]=stpArr[ip]*fak;
    modiPar = basePar;
    modiPar+= fp;
    modiPar.get(&modiHlx);
    modiEndHlx = modiHlx;
    modiEndHlx.Move(len);
    double myLen = modiEndHlx.Path(baseEndHlx.Pos());
    modiEndHlx.Move(myLen);
    myLen = (TVector3(modiEndHlx.Pos())-TVector3(baseEndHlx.Pos()))*TVector3(baseEndHlx.Dir());
    myLen/=-(TVector3(modiEndHlx.Dir())*TVector3(baseEndHlx.Dir()));
    modiEndHlx.Move(myLen);
    modiEndPar.set(&modiEndHlx,hz);
    fp = modiEndPar-baseEndPar;

    for (int jp=0;jp<5;jp++) {
      mtxNum[jp][ip] = fp.Arr()[jp]/(stpArr[ip]*fak);
  } }
    printf("TestMtx: Angle=%d Lam=%d \tRad=%d\n",iAlf,iLam,iR);

static const char T[]="HZALP";
  for (int ip=0;ip<5; ip++) { //Loop thru input parameters
    for (int jp=0;jp<5;jp++) {
      double est = mtxNum[jp][ip];
      double ana = mtxStv[jp][ip];
      double eps = 2*fabs(est-ana)/(stpArr[jp]/stpArr[ip]);
      if (eps>maxEps) maxEps=eps;
      if (eps < 1e-2) continue;
      nErr++;
      printf(" m%c%c \t%g \t%g \t%g\n",T[jp],T[ip],ana,est,eps);
  } }  
  printf("TestMtx: %d errors maxEps=%g\n",nErr,maxEps);
  StvDebug::mgRecov=saveRecov;

}

//_____________________________________________________________________________
// Some math for:
// 
// 	(cL*cP)
// T =  (cL*sP)
// 	(sL   )
// 
// 
// 	(-sP)
// P = 	( cP)
// 	( 0 )
// 
// 
// 
// 	(-sL*cP)
// L = 	(-sL*sP)
// 	( cL   )
// 
// 
// dX = T*t + P*h + L*l
// 
// 
// 	(cP)
// D =  (sP)
// 	(0 )
// 
// X0 = -imp*P
// dD = P*dPsi = P*(a/cL + rho*(cL*t -sL*l))
// 
// 
// 
// (Tk =  T*s/cL + P*(-Imp+h+dFi0*s +rho*s*s/2) + L*l
// 
// ACCOUNT ONLY 2D
// Tk = D*(s-sL*l) + P*(-Imp+h+dFi0*s+rho*s*s/2) 
// dTk= D          + P*(dFi0+rho*s)
// 
// (Tk*dTk) = 0
// 
// Now keep only the biggest order of magnitude
// 
// Tk = D*(s-sL*l) + P*(-Imp) 
// dTk= D          + P*(dFi0+rho*s)
// 
// (s-sL*l)-Imp*(dFi0+rho*s)= 0
// (1-Imp*rho)*s -sL*l -Imp*dFi0=0
// s= (sL*l+Imp*dFi0)/(1-Imp*rho)
// dFi = dFi0 +rho*(sL*l+Imp*dFi0)/(1-Imp*rho)
// dFi = (dFi0*(1+rho*Imp) +rho*sL*l)/(1-rho*Imp)
// 
// dFidFi0 = (1.)/(1-rho*Imp);
// dFidl   =      rho*sL/(1-rho*Imp);
// 
// dFidA = (1+rho*Imp)/(1-rho*Imp) /cL
// 
// dtdA = Imp/c2L/(1-Imp*rho)
// dtdl =     tL/(1-Imp*rho)
//  
// dZda = sL*dtdA; 
// dZdl = sL*dtdl+cL; 
//  

//_____________________________________________________________________________
void StvNodePars::GetImpact(StvImpact *imp,const StvFitErrs *fe)  const
{
    /// signed impact parameter; Signed in such a way that:
    ///     x =  -impact*sin(Psi)
    ///     y =   impact*cos(Psi)
  imp->mImp  = -(_x*(-_sinCA) + _y*(_cosCA));//- because point == (0-x.0-y)
  double tst =   _x*( _cosCA) + _y*(_sinCA);
  assert(fabs(tst)<1e-5 || fabs(imp->mImp) > 1000*fabs(tst));
  imp->mZ   = _z;
  imp->mPsi = _psi;
  imp->mPti = _ptin;
  imp->mTan = _tanl;
  imp->mCurv= _curv;
  if (!fe) return;

  double c2L = 1./(1+_tanl*_tanl);
  double cL  = sqrt(c2L);
  double sL  = cL*_tanl;
//		StvFitPars
// mH;	 direction perpendicular movement and Z
// mZ;	 Pseudo Z, direction perpendicular movement & H
// mA;	 Angle in XY. cos(A),sin(A),T moving direction
// mL;	 Angle lambda in Rxy/Z
// mP;	 1/pt with curvature sign

//      		Impacts
//     float  mImpImp;
//     float  mZImp, mZZ;
//     float  mPsiImp, mPsiZ, mPsiPsi;
//     float  mPtiImp, mPtiZ, mPtiPsi, mPtiPti;
//     float  mTanImp, mTanZ, mTanPsi, mTanPti, mTanTan;

// t = tL*l + imp/cL/cL/(1-imp*rho)*a
// ===========================


double mImp = imp->mImp;
double nomMins = (1-mImp*_curv);
//		d/dA
double dtda = mImp/nomMins  /c2L;
double dpsida = 1/nomMins   /cL;
double dZda = sL*dtda;
//		d/dl
double dsdl   = sL/nomMins;
double dPsidl = _curv*dsdl;
double dtdl   = dsdl/cL;
double dZdl   = sL*dtdl + cL;


double T[5][5]={
/*       h     l         A   Lam  Pti  */
/*------------------------------------*/
/*Imp*/{-1,     0,       0,    0,   0},
/*Z  */{ 0,  dZdl,    dZda,    0,   0},
/*Psi*/{ 0,dPsidl,  dpsida,    0,   0},
/*Pti*/{ 0,     0,       0,    0,   1},
/*Tan*/{ 0,     0,       0,1/c2L,   0}};
/*-------------------------------------*/
  double qwe[15];
  TCL::trasat(T[0],fe->Arr(),qwe,5,5); 
  TCL::ucopy(qwe,&imp->mImpImp,15);
}
//_____________________________________________________________________________
void StvNodeParsTest::TestImpact(int nEv)
{

StvNodePars iP,iPR,oP,oPR;
THelixTrack iH,iHR,oH,ht;

THEmx_t oHE,oHER;
double dia[5],*e,qA,qAmax;
float *ef,*erf;

  iP._cosCA = 0.051522195951218416; iP._sinCA = -0.99867184876021664;  iP._x   = 56.80456301948584; 
  iP._y     = -179.95090442478528;  iP._z     = 16.833129146428401;    iP._psi = -1.5192513089402997; iP._ptin = -4.286089548109465; 
  iP._tanl  = -0.71077992742240803; iP._curv  = -0.0063779138641975935;iP._hz=(0.0014880496061989194);
  iP.ready();
StvFitErrs iE,oE,oER;
  iE.mHH = 0.0025928369042255385;  iE.mHZ = -4.9934860023454386e-11; iE.mZZ = 0.014598355970801268; iE.mHA = -0.00059887440419442305; 
  iE.mZA = 1.0958739205478152e-11; iE.mAA = 0.00026524379894739812;  iE.mHL = 3.463001237863329e-12; iE.mZL = -0.0016525557966380938; 
  iE.mAL = 8.3669926017237923e-13; iE.mLL = 0.00041855110437868546;  iE.mHP = 0.0043962440767417576; iE.mZP = -2.904206508909407e-11; 
  iE.mAP = -0.0041320793241820105; iE.mLP = -2.5031139398137018e-12; iE.mPP = 0.78568815092933286; iE.SetHz(0.0014880496061989194);


  oER*=0.;
  oHER.Clear();
  int saveRecov = StvDebug::mgRecov;  StvDebug::mgRecov=0;

//		Prepare error matrix for TRandomVector
  TMatrixDSym S(5);
  e = iE.Arr();
  for (int i=0,li=0;i< 5;li+=++i) {S[i][i] = e[li+i];}
  S*=0.01;
//		Make huge correlations
  TRandomVector::RandRotate(S);
//		And put it back
  for (int i=0,li=0;i< 5;li+=++i) {for (int j=0;j<=i;j++){e[li+j] = S[i][j];}}
//
//  iE.Print("Input StvFitErrs");

  iP.get(&iH);			// nodePar => Helix
  iE.Get(&iH);			// fitErr  => HelixErr
  iE.Set(&iH,iP._hz);
//  iE.Print("Input StvFitErrs => THEmx_t => StvFitErrs");


//  iH.Emx()->Print("Input Helix Errs");
  oH = iH;
  double myDist = oH.Path(0.,0.);
  oH.Move(myDist);		//Move helix to 100cm
  oP.set(&oH,iP._hz);		//helix100 => nodePar100
  oE.Set(&oH,iP._hz);		//helixErr100 => fitErr100
  oE.Print("Output StvFitErrs");
//  oH.Emx()->Print("Output Helix Errs");
  StvImpact oI,oIR;
  oP.GetImpact(&oI,&oE);
  oI.Print("Output StvImpact");



//		Prepare error matrix for TRandomVector
  e = iE.Arr();
  for (int i=0,li=0;i< 5;li+=++i) {
    for (int j=0;j<=i;j++    ) {
       S[i][j]=e[li+j]; S[j][i]=e[li+j];
    } }
  TRandomVector RV(S);

//		Event loop
  double oErr[15]={0};
  for (int ev=0;ev <= nEv;ev++) {
    iPR = iP;
    ht = iH;
//		Randomize fit parameters
    TVectorD res = RV.Gaus();
    StvFitPars fp(res.GetMatrixArray()); iPR+=fp;
//		Create THelixTrack from StvNodePars
    iPR.get(&ht);
//		Set no error matrix to helix
    ht.SetEmx(0);
      
    double my100 = ht.Path(0.,0.);
    assert(fabs(my100-myDist)<fabs(myDist*0.1));
    ht.Move(my100);
    oPR.set(&ht,iP._hz);
    oPR.GetImpact(&oIR,0);
    TVectorF d = TVectorF(5,&oIR.mImp)-TVectorF(5,&oI.mImp);
    if (d[2]<=-M_PI) d[2]+=2*M_PI;
    if (d[2]>= M_PI) d[2]-=2*M_PI;
//    float *ef = &oIR.mImpImp;
    for (int i=0,li=0;i< 5;li+=++i) {
      for (int j=0;j<=i;j++    ) {
//      ef[li+j]+=d[i]*d[j];
        oErr[li+j]+=d[i]*d[j];
    } } 

    my100 = ht.Path(oP.P);
    ht.Move(my100);
    oPR.set(&ht,iP._hz);
    fp = oPR-oP;
    const double *q = fp.Arr();
    e = oER.Arr();
    for (int i=0,li=0;i< 5;li+=++i) {
      for (int j=0;j<=i;j++    ) {
      e[li+j]+=q[i]*q[j];
    } } 
  }//EndEvts
  TCL::vscale(oErr,(1./nEv),oErr,15);
  TCL::ucopy (oErr,&oIR.mImpImp, 15);
  oER*=(1./nEv);

  printf("*** Check StvFitErr matrix ***\n");

  qA=0;qAmax=0;
  for (int i=0,li=0;i< 5;li+=++i) {
    dia[i]=oE[li+i];
    for (int j=0;j<=i;j++) {
    double dif = (oER[li+j]-oE[li+j])/sqrt(dia[i]*dia[j]);
    printf("(%d %d) \t%g = \t%g \t%g\n",i,j,oER[li+j],oE[li+j],dif);
    dif = fabs(dif);
    qA+= (dif); if (dif>qAmax) qAmax=dif;
  } }
  qA/=15;
  printf("Quality %g < %g < 1\n",qA,qAmax);
  
  
  printf("/n*** Check StvImpact Error matrix ***\n");
  ef  = &oI.mImpImp;
  erf = &oIR.mImpImp;
  qA=0;qAmax=0;
  for (int i=0,li=0;i< 5;li+=++i) {
    dia[i]=ef[li+i];
    for (int j=0;j<=i;j++) {
    double dif = (erf[li+j]-ef[li+j])/sqrt(dia[i]*dia[j]);
    printf("(%d %d) \t%g = \t%g \t%g\n",i,j,erf[li+j],ef[li+j],dif);
    dif = fabs(dif);
    qA+= (dif); if (dif>qAmax) qAmax=dif;
  } }
  qA/=15;
  printf("Quality %g < %g < 1\n",qA,qAmax);
  StvDebug::mgRecov=saveRecov;

}
//_____________________________________________________________________________
#include "TMatrixT.h"
#include "TMatrixTSym.h"
#include "TVectorT.h"
//_____________________________________________________________________________
double StvFitErrs::EmxSign(int n,const float *e)
{
  enum {maxN =10,maxE = (maxN*maxN-maxN)/2+maxN};
  double d[maxE];
  assert(n>0 && n<=maxN);
  TCL::ucopy(e,d,(n*(n+1))/2);
  return EmxSign(n,d);
}
//_____________________________________________________________________________
double StvFitErrs::EmxSign(int n,const double *e)
{
  TMatrixDSym S(n);  
  TVectorD coe(n);
  for (int i=0,li=0;i< n;li+=++i) {
    double qwe = e[li+i];
    if(qwe<=0) return qwe;
    qwe = pow(2.,-int(log(qwe)/(2*log(2))));
    coe[i]=qwe;
    for (int j=0;j<=i;j++    ) {
       S[i][j]=e[li+j]*coe[i]*coe[j]; S[j][i]=S[i][j];
  } }
  TMatrixD EigMtx(n,n);
  TVectorD EigVal(n);  
  EigMtx = S.EigenVectors(EigVal);

  double ans = 3e33;
  for (int i=0;i<n;i++) {if (EigVal[i]<ans) ans = EigVal[i];}
  return ans;
} 
//_____________________________________________________________________________
void StvFitErrs::SetHz(double hz)
{ mHz=hz; assert(mHz && fabs(mHz)<0.002) ;}
double StvFitErrs::GetHz() const
{assert(mHz && fabs(mHz)<0.002);  return mHz ;}
