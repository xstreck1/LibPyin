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

#include <cmath>

#include <vamp/vamp.h>
#include <vamp-sdk/PluginAdapter.h>

#include "PYIN.h"
#include "VampYin.h"

static Vamp::PluginAdapter<PYIN> pyinPluginAdapter;
static Vamp::PluginAdapter<VampYin> vampyinPluginAdapter;

const VampPluginDescriptor *
vampGetPluginDescriptor(unsigned int version, unsigned int index)
{
    if (version < 1) return 0;

    switch (index) {
    case  0: return pyinPluginAdapter.getDescriptor();
    case  1: return vampyinPluginAdapter.getDescriptor();
    default: return 0;
    }
}
