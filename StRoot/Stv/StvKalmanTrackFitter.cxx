#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "TMath.h"

#include "TCernLib.h"
#include "StvKalmanTrackFitter.h"
#include "Stv/StvToolkit.h"
#include "Stv/StvHit.h"
#include "StvUtil/StvNodePars.h"
#include "StvUtil/StvDebug.h"
#include "Stv/StvFitter.h"
#include "Stv/StvConst.h"
#include "Stv/StvStl.h"
#include "Stv/StvNode.h"
#include "Stv/StvTrack.h"
ClassImp(StvKalmanTrackFitter)
#define DIST2(a,b) ((a[0]-b[0])*(a[0]-b[0])+(a[1]-b[1])*(a[1]-b[1])+(a[2]-b[2])*(a[2]-b[2]))



//_____________________________________________________________________________
StvKalmanTrackFitter::StvKalmanTrackFitter():StvTrackFitter("StvKalmanTrackFitter")
{
  memset(mBeg,0,mEnd-mBeg+1);
}  
//_____________________________________________________________________________
void StvKalmanTrackFitter::Clear(const char*)
{
 StvTrackFitter::Clear("");
}

//_____________________________________________________________________________
int  StvKalmanTrackFitter::Refit(StvTrack *trak,int dir,int mode)
{
///	refit or smouthe track, using the previous Kalman.
///     dir=0 moving from out to in
///     dir=1 moving from in to out 
///     mode=0 No join
///     mode=1 Join
///     fit direction is imagined from left to rite
static int nCall=0; nCall++;
static const double kBigErrFact = 10;

static StvFitter *fitt = StvFitter::Inst();
static StvConst  *kons = StvConst::Inst();

  StvNode *breakNode = 0;
  trak->MakeFitTally();
  StvNodeIter it,itBeg,itEnd;
  if (dir) { //fit in ==> out
    itBeg = trak->begin();        itEnd = trak->end();
  } else   {//fit out ==> in
    itBeg = trak->end(); --itBeg; itEnd = trak->begin();--itEnd;
  }


  double myXi2;
  StvNode *node=0,*preNode=0;
  int iNode=0,nFitLeft=0;

  for (it=itBeg; it!=itEnd; (dir)? ++it:--it) {//Main loop
    preNode=node;
//VP    assert(!mode || !preNode || !preNode->Check());
    node = *it; iNode++;

enum myCase {kNull=0,kLeft=1,kRite=2,kHit=4};
    int kase = 0;
    if (nFitLeft) 				kase|=kLeft;
    if (mode && node->FitTally()[1-dir]) 	kase|=kRite;
    if (node->GetHit() && (!mode || node->GetXi2(1-dir)<1000 || nFitLeft>2))
    						kase|=kHit;
// kLeft 		= left fits only, no hit
// kLeft+kHit 		= Left fits only, now hit, 
// kLeft+kGood 		= Left fits only, now Good Hit, 
// kRite 		= No left, no hit, rite fits only
// kLeft+kRite 		= left fits, no hit, rite fits
// kHit+kRite 		= No left fits, now hit, rite fits
// kLeft+kRite+kHit 	= Left fits,now hit, rite fits
// kLeft+kRite+kGood 	= Left fits,now good Hit, Rite fits
    node->SetXi2(3e33,dir);
    switch (kase) {// 1st switch, fill prediction

      default: assert(0 && "Wrong Case1");

      case kNull: {	// Empty leading node
        node->SetPre(node->mFP[2],node->mFE[2],dir);
        assert(node->mPP[dir].isValid());
        break;
      }  
      case kLeft: 		// Left fits only, no  Hit
      case kLeft|      kHit: 	// Left fits only, now Hit
      case kLeft|kRite     : 	// Left fits, no Hit, Rite fits
      case kLeft|kRite|kHit: 	// Left fits,now Hit, Rite fits
      {
        const StvNode *losNode = (dir)? preNode:node;
#if 1
        node->mPP[dir] = node->mFP[1-dir];
        StvFitPars delPre = preNode->mFP[dir]-preNode->mPP[1-dir];
        breakNode = (dir)? node:preNode;
        if (delPre.TooBig()) return 1;
        StvFitPars del    = delPre*preNode->mDer[dir];
        if (del.TooBig())    return 2;; 
        node->mPP[dir]+= del;node->mFP[dir]=node->mPP[dir];
//        node->mPP[dir]+= ((preNode->mFP[dir]-preNode->mPP[1-dir])*node->mDer[dir]);
        node->mPE[dir] = preNode->mFE[dir]*node->mDer[dir];
        node->mPE[dir].SetHz(node->mPP[dir]._hz);
        node->mPE[dir].Add(losNode->mELossData,node->mPP[dir]);
        if(node->mPE[dir].Check()) { StvDebug::Count("mFE[dir].Check");
	                             /*trak->CutTail(breakNode);*/ return 3;}; 
#endif //0
#if 0
        const double *P = node->mFP[1-dir].P;
        THelixTrack myHlx;
        preNode->mFP[dir].get(&myHlx);
        preNode->mFE[dir].Get(&myHlx);
	double dS = myHlx.Path(P);
        myHlx.Move(dS);
        double rho = myHlx.GetRho();
        rho += rho*losNode->GetELoss().mdPtidL*dS;
        myHlx.Set(rho);
        node->mPP[dir].set(&myHlx,preNode->mFP[dir]._hz);
        node->mPE[dir].Set(&myHlx,preNode->mFP[dir]._hz);
        node->mPE[dir].Add(losNode->mELossData,node->mPP[dir]);
        if(node->mPE[dir].Check()) { StvDebug::Count("mFE[dir].Check");
	                             /*trak->CutTail(breakNode);*/ return 3;}; 
#endif //0
        break;
      }  

      case kRite: 	// No left, no  Hit, rite fits 
      {
        node->SetPre(node->mFP[2],node->mFE[2],dir); 	
        break;
      }
      case kNull|kHit: // No left, now Hit, No rite  
      case kRite|kHit: // No left, now Hit, rite fits 
      {
        node->mPP[dir] = node->mFP[2]; 	
        node->mPE[dir].Set(node->mFE[2],kBigErrFact);
        break;
      }
    }//End 1st switch


    switch (kase) {// 2nd switch, fill fit

      case kNull: 	// No   fits  no  Hit
      case kLeft: 	// Left fits only, no  Hit
      case kRite: 	// Rite fits only, no  Hit
      case kLeft|kRite: // Left fits, no Hit, Rite fits
      {
        node->SetFit(node->mPP[dir],node->mPE[dir],dir); 
        break;
      }

      case kNull|      kHit: // Left fits only, now Hit
      case kLeft|      kHit: // Left fits only, now Hit
      case kRite|      kHit: // No left, now Hit, rite fits 
      case kLeft|kRite|kHit: // Left fits,now Hit, Rite fits
      {
	StvHit *hit = node->GetHit();
	fitt->Set(node->mPP+dir,node->mPE+dir,node->mFP+dir,node->mFE+dir);
	fitt->Prep();

	myXi2 = fitt->Xi2(hit);
	if (myXi2> kons->mXi2Hit)  	{
	  node->SetFit(node->mPP[dir],node->mPE[dir],dir); 	
          kase -= kHit;
	} else {
          nFitLeft++; kase|=kLeft;
	  node->SetXi2(myXi2,dir);
	  fitt->Update();
          node->mFP[2] = node->mFP[dir];
          node->mFE[2] = node->mFE[dir];
        }
        break;
      }
      default: assert(0 && "Wrong Case2");

    }//end 2nd switch
    if (!mode) continue;

    switch (kase) {// 3rd switch,Join

      case kLeft+kRite     : 	// Left fits, no Hit, Rite fits
      case kLeft+kRite+kHit: 	// Left fits,now Hit, Rite fits
      {
	breakNode = (dir)? node:preNode;
	fitt->Set(node->mFP+0 ,node->mFE+0
        	 ,node->mFP+1 ,node->mFE+1
        	 ,node->mFP+2 ,node->mFE+2);
	node->SetXi2(3e33,2);
	myXi2 = fitt->Xi2(); 
	if (myXi2 < kons->mXi2Joi) {
	  fitt->Update();
	  node->SetXi2(myXi2,2);
          break;
	}
//		We got a problem, join is faled
//              if no guilty hit refit failed
	  const char *key = (dir)? "Join111.Fail":"Join000.Fail";
	  StvDebug::Count(key,myXi2); 
	  /*trak->CutTail(breakNode);*/ return 4;
	}

    }//End 3rd case

  }//endMainLoop
  return 0;
}
//_____________________________________________________________________________
int StvKalmanTrackFitter::Fit(const StvTrack *trak,const StvHit *vtx,StvNode *node)
{
static       StvToolkit *kit     = StvToolkit::Inst();
static const StvConst   *myConst =   StvConst::Inst();
static const double dca3dVertex = myConst->mDca3dVertex;
static StvFitter *fitt = StvFitter::Inst();

  const StvNode *lastNode = trak->GetNode(StvTrack::kDcaPoint);
  if (!lastNode) return 1;
  THelixTrack th;
  lastNode->GetFP().get(&th);
  lastNode->GetFE().Get(&th);
  const float *h = vtx->x();
  double d[3]={h[0],h[1],h[2]};
  double len = th.Path(d);
  double x[3];
  th.Eval(len,x);
  mDca3 = DIST2(d,x);
  if (mDca3 > dca3dVertex*dca3dVertex) {
    StvDebug::Count("PrimDca3Rej",sqrt(mDca3));
    return 2;
  }
  Mtx55D_t derivFit,derivHlx;
  if (node) {th.Move(len,derivHlx);} else {th.Move(len);}
  double Hz = kit->GetHz(th.Pos());
  StvNodePars par[2]; par[0].set(&th,Hz);
  StvFitErrs  err[2]; err[0].Set(&th,Hz);
  fitt->Set(par+0,err+0,par+1,err+1);
  fitt->Prep();
  mXi2 = fitt->Xi2(vtx);
  if (mXi2>myConst->mXi2Vtx) {
    StvDebug::Count("PrimXi2Rej",mXi2);
    return 3;
  }
  if (!node) return 0;
  
  fitt->Update();
  node->SetPre(par[0],err[0],0);
  node->SetFit(par[1],err[1],0);
  par[1].convert(derivFit,derivHlx);
  node->SetDer(derivHlx,0);
  return 0;
}   
//_____________________________________________________________________________
int StvKalmanTrackFitter::Helix(StvTrack *trak,int mode)
{
// mode.bit0 = use err
// mode.bit1 = update track
// mode.bit2 = print

  THelixFitter hlx;
  int iNode=0;
  StvNode *fstNode = 0,*lstNode = 0,*node=0,*preNode=0;
  for (StvNodeIter it=trak->begin();it!=trak->end(); ++it) {
    node = *it; iNode++;
    const StvHit *hit= node->GetHit();
    if (!hit) continue;
    if (!fstNode) fstNode = node;
    lstNode = node;
    hlx.Add(hit->x()[0],hit->x()[1],hit->x()[2]);
    double cos2l = node->GetFP()._tanl; cos2l = 1/(1+cos2l*cos2l);
    const double *rr = node->GetHE();    
    if(mode&1) hlx.AddErr( rr[0],rr[2]*cos2l);
  }  
  mXi2 =hlx.Fit();
  if(mode&1) hlx.MakeErrs();
  double dL = hlx.Path(trak->front()->GetFP().P);
  hlx.Move(dL);

  iNode=0;
  int nHits = 0;
  iNode = 0;node=0;
  double dHit[3],tstXi2=0;
//		Find total energy loss and update length
  double totLoss = 0,totLen=0;
  THelixTrack myHlx(hlx);
  int on = 0;
  for (StvNodeIter it=trak->begin();it!=trak->end(); ++it) {
    preNode=node; node = *it; iNode++;
    StvNodePars FP = node->GetFP(2);
    const StvHit *hit = node->GetHit();
    const double *X = FP.P;
    if (hit) {for (int i=0;i<3;i++) {dHit[i]=hit->x()[i];};X = dHit;}
    double dS = myHlx.Path(X); myHlx.Move(dS);
    totLen+=dS; node->mLen = totLen;
    if (on) totLoss += node->GetELoss().mdPtidL*dS;
    if (node == fstNode) on=1;
    if (node == lstNode) on=0;
  }

//	Now find the middle iterator & node
  double midLoss = 0.5*fabs(totLoss),midLen=0; totLoss = 0;
  StvNodeIter midIt; iNode = 0; node=0;totLen=0;
  on = 0;
  for (midIt=trak->begin();midIt!=trak->end(); ++midIt) {
    preNode = node; node = *midIt; iNode++;
    if (preNode) {
      double dS = node->mLen-preNode->mLen; midLen+=dS;
      if (on) totLoss += fabs(node->GetELoss().mdPtidL*dS);
    }
    if (node == fstNode) on=1;
    if (node == lstNode) on=0;
    if (totLoss>= midLoss) break; 
  }
//  		Now update  
  mWorstNode = 0;mWorstXi2=0;
  for (int idir = 0; idir<2; idir++) { 
    StvNodeIter itEnd;
    if (idir) { itEnd = trak->end()		;}
    else      { itEnd = trak->begin();--itEnd	;}

    myHlx=hlx; myHlx.Move(midLen);
    node =0;
    for (StvNodeIter it=midIt;it!=itEnd; (idir)? ++it:--it) {
      preNode = node; node = *it; iNode++;
      StvNodePars FP = node->GetFP(2);
      StvFitErrs  FE = node->GetFE(2);
      const StvHit *hit = node->GetHit();
      const double *X = FP.P;
      if (hit) {for (int i=0;i<3;i++) {dHit[i]=hit->x()[i];};X = dHit;}
      double dS = (node->GetType()==StvNode::kDcaNode)? myHlx.Path(0.,0.) : myHlx.Path(X);
      double Fhlx[5][5],Fstv[5][5];
      myHlx.Move(dS,Fhlx);
      if (preNode) { //Update helix
        double rho = myHlx.GetRho();
        rho += rho*node->GetELoss().mdPtidL*dS;
        myHlx.Set(rho);
      }
      FP.set(&myHlx,FP._hz); 
      FE.Set(&myHlx,FP._hz); 
      if (mode&2){
        node->SetPre(FP,FE,0);node->SetPre(FP,FE,1);
        node->SetFit(FP,FE,0);node->SetFit(FP,FE,1);
        if (preNode) {FP.convert(Fstv,Fhlx); node->SetDer(Fstv,idir);}    
      }
      if (hit) {
	nHits++;
        double cosL = myHlx.GetCos(), tanL = myHlx.GetTan();
        const double *pos = myHlx.Pos();
        const double *dir = myHlx.Dir();
        const double *hRR = node->GetHE();
        const double dX[3] = { X[0]-pos[0],X[1]-pos[1],X[2]-pos[2]};
        double delta = (dX[0]*dir[1]-dX[1]*dir[0])/cosL;
        double myXi2 = delta /(FE.mHH+hRR[0]) *delta;
        double nL[3] = { -dir[0]*tanL,-dir[1]*tanL, cosL};
        delta = dX[0]*nL[0]+dX[1]*nL[1]+dX[2]*nL[2];
        myXi2 += delta /(FE.mZZ+hRR[2]) *delta;
        if (mWorstXi2 < myXi2) { mWorstXi2 = myXi2; mWorstNode = node;}
        tstXi2 +=  myXi2;
        if (mode&2){ node->SetXi2(myXi2,0);node->SetXi2(myXi2,1);}
      } 
  } 
  tstXi2/=nHits*3-5;
  double qqq = mXi2;if (qqq){};
//VP    assert(fabs(mXi2-tstXi2)<0.1*(mXi2+tstXi2));
  }
  if (!mode || mode&4) {//Printout only
    double myPsi = atan2(hlx.Dir()[1],hlx.Dir()[0]);
    double myTan = tan(asin(hlx.Dir()[2]));
    double myCur = hlx.GetRho();
    printf("StvTrack::Approx(fstHelx) Xi2=%g \tPsi,Tan,Curv=%g %g %g\n", mXi2,myPsi,myTan,myCur);
    dL = hlx.Path(lstNode->GetFP().P); hlx.Move(dL);
    myPsi = atan2(hlx.Dir()[1],hlx.Dir()[0]);
    myTan = tan(asin(hlx.Dir()[2]));
    myCur = hlx.GetRho();
    printf("StvTrack::Approx(lstHelx) Xi2=%g \tPsi,Tan,Curv=%g %g %g\n", mXi2,myPsi,myTan,myCur);
    myPsi = fstNode->GetFP()._psi;
    myTan = fstNode->GetFP()._tanl;
    myCur = fstNode->GetFP()._curv;
    double myXi2 = fstNode->GetXi2();
    printf("StvTrack::Approx(fstNode) Xi2=%g \tPsi,Tan,Curv=%g %g %g\n",myXi2,myPsi,myTan,myCur);
    myPsi = lstNode->GetFP()._psi;
    myTan = lstNode->GetFP()._tanl;
    myCur = lstNode->GetFP()._curv;
    myXi2 = lstNode->GetXi2();
    printf("StvTrack::Approx(lstNode) Xi2=%g \tPsi,Tan,Curv=%g %g %g\n",myXi2,myPsi,myTan,myCur);
  }
  return 0;
}
  
