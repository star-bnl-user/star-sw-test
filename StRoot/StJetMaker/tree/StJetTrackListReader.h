// -*- mode: c++;-*-
// $Id: StJetTrackListReader.h,v 1.2 2008/07/25 01:06:02 tai Exp $
// Copyright (C) 2008 Tai Sakuma <sakuma@bnl.gov>
#ifndef STJETTRACKLISTREADER_H
#define STJETTRACKLISTREADER_H

#include "TrackList.h"
#include <Rtypes.h>

class TTree;

class StJetTrackListReader {

public:
  StJetTrackListReader(TTree *tree);
  virtual ~StJetTrackListReader() { }

  StSpinJet::TrackList GetEntry(Long64_t entry);

private:

  TTree* _tree;

  Int_t    _runNumber;
  Int_t    _eventId;
  Int_t    _detectorId; // 1: TPC
  Int_t    _nTracks;
  Double_t _pt[4096];
  Double_t _eta[4096];
  Double_t _phi[4096];
  Short_t  _flag[4096];
  UShort_t _nHits[4096];
  Short_t  _charge[4096];
  UShort_t _nHitsPoss[4096];
  UShort_t _nHitsDedx[4096];
  UShort_t _nHitsFit[4096];
  Double_t _nSigmaPion[4096];
  Double_t _Tdca[4096];
  Double_t _dcaZ[4096];
  Double_t _dcaD[4096];
  Double_t _BField;
  Double_t _vertexZ;
  Double_t _bemcRadius;
  Int_t    _exitDetectorId[4096]; // 9: BEMC, 13: EEMC
  Int_t    _exitTowerId[4096];
  Double_t _exitEta[4096];
  Double_t _exitPhi[4096];
  Double_t _dEdx[4096];
  Int_t    _trackIndex[4096];
  Short_t  _trackId[4096];

};

#endif // STJETTRACKLISTREADER_H
