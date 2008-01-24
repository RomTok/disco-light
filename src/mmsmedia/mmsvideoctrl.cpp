/***************************************************************************
 *   Copyright (C) 2005-2007 by                                            *
 *                                                                         *
 *      Stefan Schwarzer <sxs@morphine.tv>                                 *
 *      Guido Madaus     <bere@morphine.tv>                                *
 *      Jens Schneider   <pupeider@morphine.tv>                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.        *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "mmsmedia/mmsvideoctrl.h"

MMS_CREATEERROR(MMSVideoCtrlError);

#define THROW_DFB_ERROR(dfbres,msg) {if (dfbres) { string s1 = msg; string s2 = DirectFBErrorString((DFBResult)dfbres); throw new MMSVideoCtrlError(dfbres,s1 + " [" + s2 + "]"); }else{ throw new MMSVideoCtrlError(0,msg); }}

DFBDisplayLayerDescription    MMSVideoCtrl::desc;   
DFBColorAdjustment           MMSVideoCtrl::cadj;

/**
 * Constructor of class MMSVideoCtrl.
 * 
 * It initializes some DirectFB stuff.
 */
MMSVideoCtrl::MMSVideoCtrl() {
    DFBResult   dfbres;

    /* init */
    this->dfb = NULL;
    this->layer = NULL;

    /* create a dfb access object */
    /* NOTE: DirectFBInit(); must be called before using this class */
    if ((dfbres = DirectFBCreate(&this->dfb)) != DFB_OK)
        THROW_DFB_ERROR(dfbres, "DirectFBCreate() failed");

    /* get layer */
    if ((dfbres = this->dfb->GetDisplayLayer(this->dfb,DLID_PRIMARY,&this->layer)) != DFB_OK)
        THROW_DFB_ERROR(dfbres, "IDirectFB::GetDisplayLayer() failed");

    /* set cooperative level */
    if ((dfbres = this->layer->SetCooperativeLevel(this->layer, DLSCL_ADMINISTRATIVE)) != DFB_OK)
        THROW_DFB_ERROR(dfbres, "IDirectFBDisplayLayer::SetCooperativeLevel() failed");

    /* get description of the layer */
    if ((dfbres = this->layer->GetDescription(this->layer, &this->desc)) != DFB_OK)
        THROW_DFB_ERROR(dfbres, "IDirectFBDisplayLayer::GetDescription() failed");

    /* get color settings */
    if ((dfbres = this->layer->GetColorAdjustment(this->layer, &this->cadj)) != DFB_OK)
        THROW_DFB_ERROR(dfbres, "IDirectFBDisplayLayer::GetColorAdjustment() failed");
}

/**
 * Destructor of class MMSVideoCtrl.
 */
MMSVideoCtrl::~MMSVideoCtrl() {
    /* close all relevant objects */
    if (this->layer) {
        delete (this->layer);
        this->layer = NULL;
    }
    if (this->dfb) {
        delete (this->dfb);
        this->dfb = NULL;
    }
}

/**
 * Gets the description of the Display Layer.
 * 
 * @note Take a look at http://www.directfb.org for the
 * documentation of this structure.
 */
DFBDisplayLayerDescription MMSVideoCtrl::getDisplayLayerDescription() {
    return this->desc;
}

/**
 * Gets the color adjustment.
 * 
 * @note Take a look at http://www.directfb.org for the
 * documentation of this structure.
 */
DFBColorAdjustment MMSVideoCtrl::getColorAdjustment() {
    return this->cadj;
}

/**
 * Sets the brightness.
 *
 * @param   val [in]    amount of brightness 
 */
void MMSVideoCtrl::adjustBrightness(unsigned val) {
    DFBResult   dfbres;

    if (!(this->desc.caps & DLCAPS_BRIGHTNESS))
        throw new MMSVideoCtrlError(0, "Adjustment of brightness is not supported.");
    this->cadj.flags        = DCAF_BRIGHTNESS;
    this->cadj.brightness   = val;
    if ((dfbres = this->layer->SetColorAdjustment(this->layer, &this->cadj)) != DFB_OK)
        THROW_DFB_ERROR(dfbres, "IDirectFBDisplayLayer::SetColorAdjustment(brightness) failed");
}

/**
 * Sets the contrast.
 *
 * @param   val [in]    amount of contrast 
 */
void MMSVideoCtrl::adjustContrast(unsigned val) {
    DFBResult   dfbres;

    if (!(this->desc.caps & DLCAPS_CONTRAST))
        throw new MMSVideoCtrlError(0, "Adjustment of contrast is not supported.");
    this->cadj.flags        = DCAF_CONTRAST;
    this->cadj.contrast     = val;
    if ((dfbres = this->layer->SetColorAdjustment(this->layer, &this->cadj)) != DFB_OK)
        THROW_DFB_ERROR(dfbres, "IDirectFBDisplayLayer::SetColorAdjustment(contrast) failed");
}

/**
 * Sets the hue.
 *
 * @param   val [in]    amount of hue 
 */
void MMSVideoCtrl::adjustHue(unsigned val) {
    DFBResult   dfbres;

    if (!(this->desc.caps & DLCAPS_HUE))
        throw new MMSVideoCtrlError(0, "Adjustment of hue is not supported.");
    this->cadj.flags        = DCAF_HUE;
    this->cadj.hue          = val;
    if ((dfbres = this->layer->SetColorAdjustment(this->layer, &this->cadj)) != DFB_OK)
        THROW_DFB_ERROR(dfbres, "IDirectFBDisplayLayer::SetColorAdjustment(hue) failed");
}

/**
 * Sets the saturation.
 *
 * @param   val [in]    amount of saturation 
 */
void MMSVideoCtrl::adjustSaturation(unsigned val) {
    DFBResult   dfbres;

    if (!(this->desc.caps & DLCAPS_SATURATION))
        throw new MMSVideoCtrlError(0, "Adjustment of saturation is not supported.");
    this->cadj.flags        = DCAF_SATURATION;
    this->cadj.saturation   = val;
    if ((dfbres = this->layer->SetColorAdjustment(this->layer, &this->cadj)) != DFB_OK)
        THROW_DFB_ERROR(dfbres, "IDirectFBDisplayLayer::SetColorAdjustment(saturation) failed");
}
