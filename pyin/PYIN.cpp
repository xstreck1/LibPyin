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

#include "PYIN.h"
#include "MonoNote.h"
#include "MonoPitch.h"

#include "vamp-sdk/FFT.h"

#include <vector>
#include <algorithm>

#include <cstdio>
#include <cmath>
#include <complex>

using std::string;
using std::vector;
using Vamp::RealTime;


PYIN::PYIN(float inputSampleRate) :
    Plugin(inputSampleRate),
    m_channels(0),
    m_stepSize(256),
    m_blockSize(2048),
    m_fmin(40),
    m_fmax(700),
    m_yin(2048, inputSampleRate, 0.0),
    m_oF0Candidates(0),
    m_oF0Probs(0),
    m_oVoicedProb(0),
    m_oCandidateSalience(0),
    m_oSmoothedPitchTrack(0),
    m_oNotes(0),
    m_threshDistr(2.0f),
    m_outputUnvoiced(0.0f),
    m_pitchProb(0),
    m_timestamp(0)
{
}

PYIN::~PYIN()
{
}

string
PYIN::getIdentifier() const
{
    return "pyin";
}

string
PYIN::getName() const
{
    return "pYin";
}

string
PYIN::getDescription() const
{
    return "Monophonic pitch and note tracking based on a probabilistic Yin extension.";
}

string
PYIN::getMaker() const
{
    return "Matthias Mauch";
}

int
PYIN::getPluginVersion() const
{
    // Increment this each time you release a version that behaves
    // differently from the previous one
    return 1;
}

string
PYIN::getCopyright() const
{
    return "GPL";
}

PYIN::InputDomain
PYIN::getInputDomain() const
{
    return TimeDomain;
}

size_t
PYIN::getPreferredBlockSize() const
{
    return 2048;
}

size_t 
PYIN::getPreferredStepSize() const
{
    return 256;
}

size_t
PYIN::getMinChannelCount() const
{
    return 1;
}

size_t
PYIN::getMaxChannelCount() const
{
    return 1;
}

PYIN::ParameterList
PYIN::getParameterDescriptors() const
{
    ParameterList list;
    
    ParameterDescriptor d;

    d.identifier = "threshdistr";
    d.name = "Yin threshold distribution";
    d.description = ".";
    d.unit = "";
    d.minValue = 0.0f;
    d.maxValue = 7.0f;
    d.defaultValue = 2.0f;
    d.isQuantized = true;
    d.quantizeStep = 1.0f;
    d.valueNames.push_back("Uniform");
    d.valueNames.push_back("Beta (mean 0.10)");
    d.valueNames.push_back("Beta (mean 0.15)");
    d.valueNames.push_back("Beta (mean 0.20)");
    d.valueNames.push_back("Beta (mean 0.30)");
    d.valueNames.push_back("Single Value 0.10");
    d.valueNames.push_back("Single Value 0.15");
    d.valueNames.push_back("Single Value 0.20");
    list.push_back(d);

    d.identifier = "outputunvoiced";
    d.valueNames.clear();
    d.name = "Output estimates classified as unvoiced?";
    d.description = ".";
    d.unit = "";
    d.minValue = 0.0f;
    d.maxValue = 2.0f;
    d.defaultValue = 0.0f;
    d.isQuantized = true;
    d.quantizeStep = 1.0f;
    d.valueNames.push_back("No");
    d.valueNames.push_back("Yes");
    d.valueNames.push_back("Yes, as negative frequencies");
    list.push_back(d);

    return list;
}

float
PYIN::getParameter(string identifier) const
{
    if (identifier == "threshdistr") {
            return m_threshDistr;
    }
    if (identifier == "outputunvoiced") {
            return m_outputUnvoiced;
    }
    return 0.f;
}

void
PYIN::setParameter(string identifier, float value) 
{
    if (identifier == "threshdistr")
    {
        m_threshDistr = value;
    }
    if (identifier == "outputunvoiced")
    {
        m_outputUnvoiced = value;
    }
    
}

PYIN::ProgramList
PYIN::getPrograms() const
{
    ProgramList list;
    return list;
}

string
PYIN::getCurrentProgram() const
{
    return ""; // no programs
}

void
PYIN::selectProgram(string name)
{
}

PYIN::OutputList
PYIN::getOutputDescriptors() const
{
    OutputList outputs;

    OutputDescriptor d;
    
    int outputNumber = 0;

    d.identifier = "f0candidates";
    d.name = "F0 Candidates";
    d.description = "Estimated fundamental frequency candidates.";
    d.unit = "Hz";
    d.hasFixedBinCount = false;
    // d.binCount = 1;
    d.hasKnownExtents = true;
    d.minValue = m_fmin;
    d.maxValue = 500;
    d.isQuantized = false;
    d.sampleType = OutputDescriptor::FixedSampleRate;
    d.sampleRate = (m_inputSampleRate / m_stepSize);
    d.hasDuration = false;
    outputs.push_back(d);
    m_oF0Candidates = outputNumber++;

    d.identifier = "f0probs";
    d.name = "Candidate Probabilities";
    d.description = "Probabilities  of estimated fundamental frequency candidates.";
    d.unit = "";
    d.hasFixedBinCount = false;
    // d.binCount = 1;
    d.hasKnownExtents = true;
    d.minValue = 0;
    d.maxValue = 1;
    d.isQuantized = false;
    d.sampleType = OutputDescriptor::FixedSampleRate;
    d.sampleRate = (m_inputSampleRate / m_stepSize);
    d.hasDuration = false;
    outputs.push_back(d);
    m_oF0Probs = outputNumber++;
    
    d.identifier = "voicedprob";
    d.name = "Voiced Probability";
    d.description = "Probability that the signal is voiced according to Probabilistic Yin.";
    d.unit = "";
    d.hasFixedBinCount = true;
    d.binCount = 1;
    d.hasKnownExtents = true;
    d.minValue = 0;
    d.maxValue = 1;
    d.isQuantized = false;
    d.sampleType = OutputDescriptor::FixedSampleRate;
    d.sampleRate = (m_inputSampleRate / m_stepSize);
    d.hasDuration = false;
    outputs.push_back(d);
    m_oVoicedProb = outputNumber++;

    d.identifier = "candidatesalience";
    d.name = "Candidate Salience";
    d.description = "Candidate Salience";
    d.hasFixedBinCount = true;
    d.binCount = m_blockSize / 2;
    d.hasKnownExtents = true;
    d.minValue = 0;
    d.maxValue = 1;
    d.isQuantized = false;
    d.sampleType = OutputDescriptor::FixedSampleRate;
    d.sampleRate = (m_inputSampleRate / m_stepSize);
    d.hasDuration = false;
    outputs.push_back(d);
    m_oCandidateSalience = outputNumber++;
    
    d.identifier = "smoothedpitchtrack";
    d.name = "Smoothed Pitch Track";
    d.description = ".";
    d.unit = "Hz";
    d.hasFixedBinCount = true;
    d.binCount = 1;
    d.hasKnownExtents = false;
    // d.minValue = 0;
    // d.maxValue = 1;
    d.isQuantized = false;
    d.sampleType = OutputDescriptor::FixedSampleRate;
    d.sampleRate = (m_inputSampleRate / m_stepSize);
    d.hasDuration = false;
    outputs.push_back(d);
    m_oSmoothedPitchTrack = outputNumber++;

    d.identifier = "notes";
    d.name = "Notes";
    d.description = "Derived fixed-pitch note frequencies";
    // d.unit = "MIDI unit";
    d.unit = "Hz";
    d.hasFixedBinCount = true;
    d.binCount = 1;
    d.hasKnownExtents = false;
    d.isQuantized = false;
    d.sampleType = OutputDescriptor::VariableSampleRate;
    d.sampleRate = (m_inputSampleRate / m_stepSize);
    d.hasDuration = true;
    outputs.push_back(d);
    m_oNotes = outputNumber++;

    return outputs;
}

bool
PYIN::initialise(size_t channels, size_t stepSize, size_t blockSize)
{
    if (channels < getMinChannelCount() ||
	channels > getMaxChannelCount()) return false;

/*
    std::cerr << "PYIN::initialise: channels = " << channels
          << ", stepSize = " << stepSize << ", blockSize = " << blockSize
          << std::endl;
*/
    m_channels = channels;
    m_stepSize = stepSize;
    m_blockSize = blockSize;
    
    reset();

    return true;
}

void
PYIN::reset()
{    
    m_yin.setThresholdDistr(m_threshDistr);
    m_yin.setFrameSize(m_blockSize);
    
    m_pitchProb.clear();
    m_timestamp.clear();
/*    
    std::cerr << "PYIN::reset"
          << ", blockSize = " << m_blockSize
          << std::endl;
*/
}

PYIN::FeatureSet
PYIN::process(const float *const *inputBuffers, RealTime timestamp)
{
    timestamp = timestamp + Vamp::RealTime::frame2RealTime(m_blockSize/4, lrintf(m_inputSampleRate));
    FeatureSet fs;
    
    double *dInputBuffers = new double[m_blockSize];
    for (size_t i = 0; i < m_blockSize; ++i) dInputBuffers[i] = inputBuffers[0][i];
    
    Yin::YinOutput yo = m_yin.processProbabilisticYin(dInputBuffers);
    delete [] dInputBuffers;

    // First, get the things out of the way that we don't want to output 
    // immediately, but instead save for later.
    vector<pair<double, double> > tempPitchProb;
    for (size_t iCandidate = 0; iCandidate < yo.freqProb.size(); ++iCandidate)
    {
        double tempPitch = 12 * std::log(yo.freqProb[iCandidate].first/440)/std::log(2.) + 69;
        tempPitchProb.push_back(pair<double, double>
            (tempPitch, yo.freqProb[iCandidate].second));
    }
    m_pitchProb.push_back(tempPitchProb);
    m_timestamp.push_back(timestamp);

    // F0 CANDIDATES
    Feature f;
    f.hasTimestamp = true;
    f.timestamp = timestamp;
    for (size_t i = 0; i < yo.freqProb.size(); ++i)
    {
        f.values.push_back(yo.freqProb[i].first);
    }
    fs[m_oF0Candidates].push_back(f);
    
    // VOICEDPROB
    f.values.clear();
    float voicedProb = 0;
    for (size_t i = 0; i < yo.freqProb.size(); ++i)
    {
        f.values.push_back(yo.freqProb[i].second);
        voicedProb += yo.freqProb[i].second;
    }
    fs[m_oF0Probs].push_back(f);
    
    f.values.clear();
    f.values.push_back(voicedProb);
    fs[m_oVoicedProb].push_back(f);

    // SALIENCE -- maybe this should eventually disappear
    f.values.clear();
    float salienceSum = 0;
    for (size_t iBin = 0; iBin < yo.salience.size(); ++iBin)
    {
        f.values.push_back(yo.salience[iBin]);
        salienceSum += yo.salience[iBin];
    }
    fs[m_oCandidateSalience].push_back(f);

    return fs;
}

PYIN::FeatureSet
PYIN::getRemainingFeatures()
{
    FeatureSet fs;
    Feature f;
    f.hasTimestamp = true;
    f.hasDuration = false;
    
    if (m_pitchProb.empty()) {
        return fs;
    }

    // MONO-PITCH STUFF
    MonoPitch mp;
    vector<float> mpOut = mp.process(m_pitchProb);
    for (size_t iFrame = 0; iFrame < mpOut.size(); ++iFrame)
    {
        if (mpOut[iFrame] < 0 && (m_outputUnvoiced==0)) continue;
        f.timestamp = m_timestamp[iFrame];
        f.values.clear();
        if (m_outputUnvoiced == 1)
        {
            f.values.push_back(fabs(mpOut[iFrame]));
        } else {
            f.values.push_back(mpOut[iFrame]);
        }
        
        fs[m_oSmoothedPitchTrack].push_back(f);
    }
    
    // MONO-NOTE STUFF
    MonoNote mn;
    std::vector<std::vector<std::pair<double, double> > > smoothedPitch;
    for (size_t iFrame = 0; iFrame < mpOut.size(); ++iFrame) {
        std::vector<std::pair<double, double> > temp;
        if (mpOut[iFrame] > 0)
        {
            double tempPitch = 12 * std::log(mpOut[iFrame]/440)/std::log(2.) + 69;
            temp.push_back(std::pair<double,double>(tempPitch, .9));
        }
        smoothedPitch.push_back(temp);
    }
    // vector<MonoNote::FrameOutput> mnOut = mn.process(m_pitchProb);
    vector<MonoNote::FrameOutput> mnOut = mn.process(smoothedPitch);
    
    // turning feature into a note feature
    f.hasTimestamp = true;
    f.hasDuration = true;
    f.values.clear();
        
    int onsetFrame = 0;
    bool isVoiced = 0;
    bool oldIsVoiced = 0;
    size_t nFrame = m_pitchProb.size();
    
    std::vector<float> notePitchTrack; // collects pitches for one note at a time
    for (size_t iFrame = 0; iFrame < nFrame; ++iFrame)
    {
        isVoiced = mnOut[iFrame].noteState < 3 && smoothedPitch[iFrame].size() > 0;
        if (isVoiced && iFrame != nFrame-1)
        {
            if (oldIsVoiced == 0) // beginning of a note
            {
                onsetFrame = iFrame;
                notePitchTrack.clear();
            }
            float pitch = smoothedPitch[iFrame][0].first;
            notePitchTrack.push_back(pitch); // add to the note's pitch track
        } else { // not currently voiced
            if (oldIsVoiced == 1 && notePitchTrack.size() > 4) // end of the note
            {
                std::sort(notePitchTrack.begin(), notePitchTrack.end());
                float medianPitch = notePitchTrack[notePitchTrack.size()/2];
                float medianFreq = std::pow(2,(medianPitch - 69) / 12) * 440;
                f.values.clear();
                f.values.push_back(medianFreq);
                f.timestamp = m_timestamp[onsetFrame];
                f.duration = m_timestamp[iFrame] - m_timestamp[onsetFrame];
                fs[m_oNotes].push_back(f);
            }
        }
        oldIsVoiced = isVoiced;
    }
    return fs;
}
