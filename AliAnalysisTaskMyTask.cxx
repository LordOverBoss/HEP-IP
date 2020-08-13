/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* AliAnaysisTaskMyTask
 *
 * empty task which can serve as a starting point for building an analysis
 * as an example, one histogram is filled
 */

#include "TChain.h"
#include "TH1F.h"
#include "TList.h"
#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliAODEvent.h"
#include "AliAODInputHandler.h"
#include "AliAnalysisTaskMyTask.h"
#include "AliPIDResponse.h" //pid responde
#include "AliMCEvent.h"	//para MC
#include "AliAODMCParticle.h"	//para MC
#include "AliMultSelection.h"	//para centralidad

class AliPIDResponse;		//cargamos la clase de PID
class AliAnalysisTaskMyTask;    // your analysis class

using namespace std;            // std namespace: so you can do things like 'cout'

ClassImp(AliAnalysisTaskMyTask) // classimp: necessary for root

AliAnalysisTaskMyTask::AliAnalysisTaskMyTask() : AliAnalysisTaskSE(), 
    fAOD(0), fOutputList(0), fHistPt(0), fHistPtV(0), fHistPt1(0), fHistPt128(0), fHistPt512(0), fYour2DHistogram(0), fPIDResponse(0), fMCEvent(0) //agregamos inicializadores
{
    // default constructor, don't allocate memory here!
    // this is used by root for IO purposes, it needs to remain empty
}
//_____________________________________________________________________________
AliAnalysisTaskMyTask::AliAnalysisTaskMyTask(const char* name) : AliAnalysisTaskSE(name),
    fAOD(0), fOutputList(0), fHistPt(0), fHistPtV(0), fHistPt1(0), fHistPt128(0), fHistPt512(0), fYour2DHistogram(0), fPIDResponse(0), fMCEvent(0)  //agregamos inicializadores
{
    // constructor
    DefineInput(0, TChain::Class());    // define the input of the analysis: in this case we take a 'chain' of events
                                        // this chain is created by the analysis manager, so no need to worry about it, 
                                        // it does its work automatically
    DefineOutput(1, TList::Class());    // define the ouptut of the analysis: in this case it's a list of histograms 
                                        // you can add more output objects by calling DefineOutput(2, classname::Class())
                                        // if you add more output objects, make sure to call PostData for all of them, and to
                                        // make changes to your AddTask macro!
}
//_____________________________________________________________________________
AliAnalysisTaskMyTask::~AliAnalysisTaskMyTask()
{
    // destructor
    if(fOutputList) {
        delete fOutputList;     // at the end of your task, it is deleted from memory by calling this function
    }
}
//_____________________________________________________________________________
void AliAnalysisTaskMyTask::UserCreateOutputObjects()
{
    // create output objects
    //
    // this function is called ONCE at the start of your analysis (RUNTIME)
    // here you ceate the histograms that you want to use 
    //
    // the histograms are in this case added to a tlist, this list is in the end saved
    // to an output file
    //
    fOutputList = new TList();          // this is a list which will contain all of your histograms
                                        // at the end of the analysis, the contents of this list are written
                                        // to the output file
    fOutputList->SetOwner(kTRUE);       // memory stuff: the list is owner of all objects it contains and will delete them
                                        // if requested (dont worry about this now)

    // example of a histogram
    fHistPt = new TH1F("fHistPt", "fHistPt", 100, 0, 10);       // create your histogra
    fOutputList->Add(fHistPt);          // don't forget to add it to the list! the list will be written to file, so if you want
    PostData(1, fOutputList);           // postdata will notify the analysis manager of changes / updates to the 
                                        // fOutputList object. the manager will in the end take care of writing your output to file
                                        // so it needs to know what's in the output
                                        
                                        
    fHistPtV = new TH1F("fHistPtV", "fHistPtV", 100, 0, 10);     		 //histograma vertices
    fOutputList->Add(fHistPtV);
    PostData(1, fOutputList);
    
    fHistPt1 = new TH1F("fHistPt1", "fHistPt1", 100, 0, 10);		//histograma filterbit 1
    fOutputList->Add(fHistPt1);
    PostData(1, fOutputList);
    
    fHistPt128 = new TH1F("fHistPt128", "fHistPt128", 100, 0, 10);		//histograma filterbit 128
    fOutputList->Add(fHistPt128);
    PostData(1, fOutputList);
    
    fHistPt512 = new TH1F("fHistPt512", "fHistPt512", 100, 0, 10);		//histograma filterbit 512
    fOutputList->Add(fHistPt512);
    PostData(1, fOutputList);
    
    fYour2DHistogram = new TH1F("fYour2DHistogram", "fYour2DHistogram", 100, 0, 10);		//histograma 2d
    fOutputList->Add(fYour2DHistogram);
    PostData(1, fOutputList);
    
    fPIDResponse = new AliPIDResponse();		//response object
    fOutputList->Add(fPIDResponse);
    PostData(1, fOutputList);
    
}

//_____________________________________________________________________________
void AliAnalysisTaskMyTask::ProcessMCParticles()		//hacemos uso de la nueva funcion para MC
{
    // process MC particles
    TClonesArray* AODMCTrackArray = dynamic_cast<TClonesArray*>(fInputEvent->FindListObject(AliAODMCParticle::StdBranchName()));
    if (AODMCTrackArray == NULL) return;

    // Loop over all primary MC particle
    for(Long_t i = 0; i < AODMCTrackArray->GetEntriesFast(); i++) {

      AliAODMCParticle* particle = static_cast<AliAODMCParticle*>(AODMCTrackArray->At(i));
      if (!particle) continue;
      cout << "PDG CODE = " << particle->GetPdgCode() << endl;
    }
}

//_____________________________________________________________________________
void AliAnalysisTaskMyTask::UserExec(Option_t *)
{

fMCEvent = MCEvent();		//sacamos informacion de los MC
if(fMCEvent) ProcessMCParticles(); 	//y la procesamos
 
 
    // user exec
    // this function is called once for each event
    // the manager will take care of reading the events from file, and with the static function InputEvent() you 
    // have access to the current event. 
    // once you return from the UserExec function, the manager will retrieve the next event from the chain
    fAOD = dynamic_cast<AliAODEvent*>(InputEvent());    // get an event (called fAOD) from the input file
    							  // there's another event format (ESD) which works in a similar wya
                                                        // but is more cpu/memory unfriendly. for now, we'll stick with aod's 
    if(!fAOD) return;                                   // if the pointer to the event is empty (getting it failed) skip this event
        // example part: i'll show how to loop over the tracks in an event 
        // and extract some information from them which we'll store in a histogram
        
    Int_t iTracks(fAOD->GetNumberOfTracks());           // see how many tracks there are in the event
    
    for(Int_t i(0); i < iTracks; i++) {                 // loop ove rall these tracks
        AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(i));         // get a track (type AliAODTrack) from the event
        if(!track || !track->TestFilterBit(1)) continue;                            // if we failed, skip this track
        fHistPt->Fill(track->Pt());                     // plot the pt value of the track in a histogram
    }                                                   // continue until all the tracks are processed
    
    PostData(1, fOutputList);                           // stream the results the analysis of this event to
                                                        // the output manager which will take care of writing
                                                        // it to a file
    
    fAOD = dynamic_cast<AliAODEvent*>(InputEvent());				//histograma vertices
    if(!fAOD) return;									
    Int_t jTracks(fAOD->GetNumberOfTracks());
    for(Int_t j(0); j < jTracks; j++) {
    	AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(j));
    	if(!track || !track->TestFilterBit(1)) 
    	continue;
    	float vertexZ = fAOD->GetPrimaryVertex()->GetZ();
    	fHistPtV->Fill(track->Pt());
    }
    PostData(1, fOutputList);
    
    fAOD = dynamic_cast<AliAODEvent*>(InputEvent());					//histograma filterbit 1	
    if(!fAOD) return;									
    Int_t kTracks(fAOD->GetNumberOfTracks());
    for(Int_t k(0); k < kTracks; k++) {
    	AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(k));
    	if(!track || !track->TestFilterBit(1)) continue;
    	fHistPt1->Fill(track->Pt()); 
    }
    PostData(1, fOutputList);
    
    fAOD = dynamic_cast<AliAODEvent*>(InputEvent());					//histograma filterbit 128	
    if(!fAOD) return;									
    Int_t mTracks(fAOD->GetNumberOfTracks());
    for(Int_t m(0); m < mTracks; m++) {
    	AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(m));
    	if(!track || !track->TestFilterBit(128)) continue;
    	fHistPt128->Fill(track->Pt());
    }
    PostData(1, fOutputList);
    
    fAOD = dynamic_cast<AliAODEvent*>(InputEvent());					//histograma filterbit 512	
    if(!fAOD) return;									
    Int_t nTracks(fAOD->GetNumberOfTracks());
    for(Int_t n(0); n < nTracks; n++) {
    	AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(n));
    	if(!track || !track->TestFilterBit(512)) continue;
    	fHistPt512->Fill(track->Pt());
    }
    PostData(1, fOutputList);
    
    fAOD = dynamic_cast<AliAODEvent*>(InputEvent());					//histograma 2d	
    if(!fAOD) return;									
    Int_t qTracks(fAOD->GetNumberOfTracks());
    for(Int_t q(0); q < qTracks; q++) {
    	AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(q));
    	if(!track || !track->TestFilterBit(1)) continue;
    	fYour2DHistogram->Fill(track->P(), track->GetTPCsignal());
    double kaonSignal = fPIDResponse->NumberOfSigmasTPC(track, AliPID::kKaon);		//identificacion de particulas
    double pionSignal = fPIDResponse->NumberOfSigmasTPC(track, AliPID::kPion);
    double protonSignal = fPIDResponse->NumberOfSigmasTPC(track, AliPID::kProton);
    if (std::abs(fPIDResponse->NumberOfSigmasTPC(track, AliPID::kPion)) < 3 ) {
   //jippy, i'm a pion
    };
    Float_t centrality(0);
    AliMultSelection *multSelection =static_cast<AliMultSelection*>(fAOD->FindListObject("MultSelection"));	//centralidad
    if(multSelection) centrality = multSelection->GetMultiplicityPercentile("V0M");
    PostData(1, fOutputList);
    };

AliAnalysisManager *man = AliAnalysisManager::GetAnalysisManager();
    if (man) {
    	AliInputEventHandler* inputHandler = (AliInputEventHandler*)(man->GetInputEventHandler());		//pid response
    	if (inputHandler)   fPIDResponse = inputHandler->GetPIDResponse();
    }

}
//_____________________________________________________________________________
void AliAnalysisTaskMyTask::Terminate(Option_t *)
{
    // terminate
    // called at the END of the analysis (when all events are processed)
}
//_____________________________________________________________________________
