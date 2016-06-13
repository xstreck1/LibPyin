/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    pYIN - A fundamental frequency estimator for monophonic audio
    Centre for Digital Music, Queen Mary, University of London.
    
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.  See the file
    COPYING included with this distribution for more information.
*/

#include "MonoNoteParameters.h"

MonoNoteParameters::MonoNoteParameters() :
    minPitch(36), 
    nPPS(3), 
    nS(43), 
    nSPP(4), // states per pitch
    n(0),
    initPi(0), 
    pAttackSelftrans(0.5),
    pStableSelftrans(0.999),
    pStable2Silent(0.005),
    pSilentSelftrans(0.5), 
    sigma2Note(0.7),
    maxJump(13),
    pInterSelftrans(0.99),
    priorPitchedProb(.7),
    priorWeight(0.5),
    minSemitoneDistance(.5),
    sigmaYinPitchAttack(5), 
    sigmaYinPitchStable(0.9),
    sigmaYinPitchInter(5),
    yinTrust(0.1)
{
    // just in case someone put in a silly value for pRelease2Unvoiced
    n = nPPS * nS * nSPP;
}

MonoNoteParameters::~MonoNoteParameters()
{
}
