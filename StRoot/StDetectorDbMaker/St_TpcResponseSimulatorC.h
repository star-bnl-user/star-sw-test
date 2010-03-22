#ifndef St_TpcResponseSimulatorC_h
#define St_TpcResponseSimulatorC_h

#include "TChair.h"
#include "tables/St_TpcResponseSimulator_Table.h"

class St_TpcResponseSimulatorC : public TChair {
 public:
  static St_TpcResponseSimulatorC* 	instance();
  TpcResponseSimulator_st 	*Struct(Int_t i = 0) 	const {return ((St_TpcResponseSimulator*) Table())->GetTable()+i;}
  UInt_t     	getNumRows()                	const {return GetNRows();}
  Float_t 	I0(Int_t i = 0) 	const {return Struct(i)->I0;}
  Float_t 	Cluster(Int_t i = 0) 	const {return Struct(i)->Cluster;}
  Float_t 	W(Int_t i = 0) 	const {return Struct(i)->W;}
  Float_t 	OmegaTau(Int_t i = 0) 	const {return Struct(i)->OmegaTau;}
  Float_t 	K3IP(Int_t i = 0) 	const {return Struct(i)->K3IP;}
  Float_t 	K3IR(Int_t i = 0) 	const {return Struct(i)->K3IR;}
  Float_t 	K3OP(Int_t i = 0) 	const {return Struct(i)->K3OP;}
  Float_t 	K3OR(Int_t i = 0) 	const {return Struct(i)->K3OR;}
  Float_t 	FanoFactor(Int_t i = 0) 	const {return Struct(i)->FanoFactor;}
  Float_t 	AveragePedestal(Int_t i = 0) 	const {return Struct(i)->AveragePedestal;}
  Float_t 	AveragePedestalRMS(Int_t i = 0) 	const {return Struct(i)->AveragePedestalRMS;}
  Float_t 	AveragePedestalRMSX(Int_t i = 0) 	const {return Struct(i)->AveragePedestalRMSX;}
  Float_t 	tauIntegration(Int_t i = 0) 	const {return Struct(i)->tauIntegration;}
  Float_t 	tauF(Int_t i = 0) 	const {return Struct(i)->tauF;}
  Float_t 	tauP(Int_t i = 0) 	const {return Struct(i)->tauP;}
  Float_t      *tauX(Int_t i = 0) 	const {return &Struct(i)->tauXI;}
  Float_t 	tauXI(Int_t i = 0) 	const {return Struct(i)->tauXI;}
  Float_t 	tauXO(Int_t i = 0) 	const {return Struct(i)->tauXO;}
  Float_t      *tauC(Int_t i = 0) 	const {return &Struct(i)->tauCI;}
  Float_t 	tauCI(Int_t i = 0) 	const {return Struct(i)->tauCI;}
  Float_t 	tauCO(Int_t i = 0) 	const {return Struct(i)->tauCO;}
  Float_t 	SigmaJitterTI(Int_t i = 0) 	const {return Struct(i)->SigmaJitterTI;}
  Float_t 	SigmaJitterTO(Int_t i = 0) 	const {return Struct(i)->SigmaJitterTO;}
  Float_t 	longitudinalDiffusion(Int_t i = 0) 	const {return Struct(i)->longitudinalDiffusion;}
  Float_t 	transverseDiffusion(Int_t i = 0) 	const {return Struct(i)->transverseDiffusion;}
  Float_t       NoElPerAdc(Int_t i = 0) const {return Struct(i)->NoElPerAdc;}
  Float_t       OmegaTauScaleI(Int_t i = 0) const {return Struct(i)->OmegaTauScaleI;}
  Float_t       OmegaTauScaleO(Int_t i = 0) const {return Struct(i)->OmegaTauScaleO;}
  Float_t      *SecRowCor(Int_t i = 0)      const {return &Struct(i)->SecRowCorIW[0];}
  Float_t      *SecRowCorIW(Int_t i = 0)    const {return &Struct(i)->SecRowCorIW[0];}
  Float_t      *SecRowCorOW(Int_t i = 0)    const {return &Struct(i)->SecRowCorOW[0];}
  Float_t      *SecRowCorIE(Int_t i = 0)    const {return &Struct(i)->SecRowCorIE[0];}
  Float_t      *SecRowCorOE(Int_t i = 0)    const {return &Struct(i)->SecRowCorOE[0];}
 protected:
  St_TpcResponseSimulatorC(St_TpcResponseSimulator *table=0) : TChair(table) {}
  virtual ~St_TpcResponseSimulatorC() {fgInstance = 0;}
 private:
  static St_TpcResponseSimulatorC* fgInstance;
  ClassDefChair(St_TpcResponseSimulator, TpcResponseSimulator_st )
  ClassDef(St_TpcResponseSimulatorC,1) //C++ TChair for TpcResponseSimulator table class
};
#endif
