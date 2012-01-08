/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009-2012 BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <pupeider@gmx.de>,                              *
 *      Guido Madaus       <guido.madaus@diskohq.org>,                     *
 *      Patrick Helterhoff <patrick.helterhoff@diskohq.org>,               *
 *      René Bählkow       <rene.baehlkow@diskohq.org>                     *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License version 2.1 as published by the Free Software Foundation.     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 **************************************************************************/

#include "mmsgui/3d/mms3dmaterial.h"

//! number of user defined materials
int MMS3DMaterial::material_cnt = 0;

//! materials
MMS3D_MATERIAL MMS3DMaterial::mat_buffer[MMS3DMaterial::MAT_SIZE] = {
	// init space for user defined materials
		{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},
		{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},
		{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},
		{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},
		{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},
		{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},
		{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},
		{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},
		{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},
		{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},
		{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},
		{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},
		{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},
		{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},
		{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},
		{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},

	// basic colors ---- Emission ----------  Ambient -----------  Diffuse -----------  Specular ----------  Shininess -
	// ----------------- Re   Ge   Be   Ae    Ra   Ga   Ba   Aa    Rd   Gd   Bd   Ad    Rs   Gs   Bs   As
						{0.00,0.00,0.00,1.00, 0.00,0.00,0.00,1.00, 0.00,0.00,0.00,1.00, 0.00,0.00,0.00,1.00, 0.000},
						{0.00,0.00,0.00,1.00, 0.50,0.00,0.00,1.00, 1.00,0.00,0.00,1.00, 0.00,0.00,0.00,1.00, 0.000},
						{0.00,0.00,0.00,1.00, 0.00,0.50,0.00,1.00, 0.00,1.00,0.00,1.00, 0.00,0.00,0.00,1.00, 0.000},
						{0.00,0.00,0.00,1.00, 0.00,0.00,0.50,1.00, 0.00,0.00,1.00,1.00, 0.00,0.00,0.00,1.00, 0.000},
						{0.00,0.00,0.00,1.00, 0.50,0.50,0.00,1.00, 1.00,1.00,0.00,1.00, 0.00,0.00,0.00,1.00, 0.000},
						{0.00,0.00,0.00,1.00, 0.00,0.50,0.50,1.00, 0.00,1.00,1.00,1.00, 0.00,0.00,0.00,1.00, 0.000},
						{0.00,0.00,0.00,1.00, 0.50,0.00,0.50,1.00, 1.00,0.00,1.00,1.00, 0.00,0.00,0.00,1.00, 0.000},
						{0.00,0.00,0.00,1.00, 0.50,0.50,0.50,1.00, 1.00,1.00,1.00,1.00, 0.00,0.00,0.00,1.00, 0.000},

	// gemstone / ------ Emission ----------  Ambient -----------  Diffuse -----------  Specular ----------  Shininess -
	// glass ----------- Re   Ge   Be   Ae    Ra   Ga   Ba   Aa    Rd   Gd   Bd   Ad    Rs   Gs   Bs   As
						{0.00,0.00,0.00,1.00, 0.14,0.22,0.16,1.00, 0.54,0.89,0.63,1.00, 0.32,0.32,0.32,1.00, 0.100},
						{0.00,0.00,0.00,1.00, 0.14,0.22,0.16,0.90, 0.54,0.89,0.63,0.90, 0.32,0.32,0.32,0.90, 0.100},
						{0.00,0.00,0.00,1.00, 0.05,0.05,0.07,1.00, 0.18,0.17,0.23,1.00, 0.33,0.33,0.35,1.00, 0.300},
						{0.00,0.00,0.00,1.00, 0.05,0.05,0.07,0.80, 0.18,0.17,0.23,0.80, 0.33,0.33,0.35,0.80, 0.300},
						{0.00,0.00,0.00,1.00, 0.25,0.21,0.21,1.00, 0.99,0.83,0.83,1.00, 0.30,0.30,0.30,1.00, 0.088},
						{0.00,0.00,0.00,1.00, 0.25,0.21,0.21,0.90, 0.99,0.83,0.83,0.90, 0.30,0.30,0.30,0.90, 0.088},
						{0.00,0.00,0.00,1.00, 0.17,0.01,0.01,1.00, 0.61,0.04,0.04,1.00, 0.73,0.63,0.63,1.00, 0.600},
						{0.00,0.00,0.00,1.00, 0.17,0.01,0.01,0.50, 0.61,0.04,0.04,0.50, 0.73,0.63,0.63,0.50, 0.600},
						{0.00,0.00,0.00,1.00, 0.02,0.17,0.02,1.00, 0.08,0.61,0.08,1.00, 0.63,0.73,0.63,1.00, 0.600},
						{0.00,0.00,0.00,1.00, 0.02,0.17,0.02,0.50, 0.08,0.61,0.08,0.50, 0.63,0.73,0.63,0.50, 0.600},
						{0.00,0.00,0.00,1.00, 0.10,0.19,0.17,1.00, 0.40,0.74,0.69,1.00, 0.30,0.31,0.31,1.00, 0.100},
						{0.00,0.00,0.00,1.00, 0.10,0.19,0.17,0.80, 0.40,0.74,0.69,0.80, 0.30,0.31,0.31,0.80, 0.100},

	// synthetic ------- Emission ----------  Ambient -----------  Diffuse -----------  Specular ----------  Shininess -
	// ----------------- Re   Ge   Be   Ae    Ra   Ga   Ba   Aa    Rd   Gd   Bd   Ad    Rs   Gs   Bs   As
						{0.00,0.00,0.00,1.00, 0.00,0.00,0.00,1.00, 0.01,0.01,0.01,1.00, 0.50,0.50,0.50,1.00, 0.250},
						{0.00,0.00,0.00,1.00, 0.00,0.00,0.00,1.00, 0.50,0.00,0.00,1.00, 0.70,0.60,0.60,1.00, 0.250},
						{0.00,0.00,0.00,1.00, 0.00,0.00,0.00,1.00, 0.10,0.35,0.10,1.00, 0.45,0.55,0.45,1.00, 0.250},
						{0.00,0.00,0.00,1.00, 0.00,0.00,0.00,1.00, 0.00,0.00,0.50,1.00, 0.60,0.60,0.70,1.00, 0.250},
						{0.00,0.00,0.00,1.00, 0.00,0.00,0.00,1.00, 0.50,0.50,0.00,1.00, 0.60,0.60,0.50,1.00, 0.250},
						{0.00,0.00,0.00,1.00, 0.00,0.10,0.06,1.00, 0.00,0.51,0.51,1.00, 0.50,0.50,0.50,1.00, 0.250},
						{0.00,0.00,0.00,1.00, 0.06,0.00,0.06,1.00, 0.51,0.00,0.51,1.00, 0.50,0.50,0.50,1.00, 0.250},
						{0.00,0.00,0.00,1.00, 0.00,0.00,0.00,1.00, 0.55,0.55,0.55,1.00, 0.70,0.70,0.70,1.00, 0.250},
						{0.00,0.00,0.00,1.00, 0.02,0.02,0.02,1.00, 0.01,0.01,0.01,1.00, 0.40,0.40,0.40,1.00, 0.078},
						{0.00,0.00,0.00,1.00, 0.05,0.00,0.00,1.00, 0.50,0.40,0.40,1.00, 0.70,0.04,0.04,1.00, 0.078},
						{0.00,0.00,0.00,1.00, 0.00,0.05,0.00,1.00, 0.40,0.50,0.40,1.00, 0.04,0.70,0.04,1.00, 0.078},
						{0.00,0.00,0.00,1.00, 0.00,0.00,0.05,1.00, 0.40,0.40,0.50,1.00, 0.04,0.04,0.70,1.00, 0.078},
						{0.00,0.00,0.00,1.00, 0.05,0.05,0.00,1.00, 0.50,0.50,0.40,1.00, 0.70,0.70,0.04,1.00, 0.078},
						{0.00,0.00,0.00,1.00, 0.00,0.05,0.05,1.00, 0.40,0.50,0.50,1.00, 0.04,0.70,0.70,1.00, 0.078},
						{0.00,0.00,0.00,1.00, 0.05,0.00,0.05,1.00, 0.50,0.40,0.50,1.00, 0.70,0.04,0.70,1.00, 0.078},
						{0.00,0.00,0.00,1.00, 0.05,0.05,0.05,1.00, 0.50,0.50,0.50,1.00, 0.70,0.70,0.70,1.00, 0.078},

	// metal ----------- Emission ----------  Ambient -----------  Diffuse -----------  Specular ----------  Shininess -
	// ----------------- Re   Ge   Be   Ae    Ra   Ga   Ba   Aa    Rd   Gd   Bd   Ad    Rs   Gs   Bs   As
						{0.00,0.00,0.00,1.00, 0.21,0.13,0.05,1.00, 0.71,0.43,0.18,1.00, 0.39,0.27,0.17,1.00, 0.200},
						{0.00,0.00,0.00,1.00, 0.25,0.15,0.06,1.00, 0.40,0.24,0.10,1.00, 0.77,0.46,0.20,1.00, 0.600},
						{0.00,0.00,0.00,1.00, 0.25,0.25,0.25,1.00, 0.40,0.40,0.40,1.00, 0.77,0.77,0.77,1.00, 0.600},
						{0.00,0.00,0.00,1.00, 0.25,0.20,0.07,1.00, 0.75,0.61,0.23,1.00, 0.63,0.65,0.37,1.00, 0.400},
						{0.00,0.00,0.00,1.00, 0.25,0.22,0.06,1.00, 0.35,0.31,0.09,1.00, 0.80,0.72,0.21,1.00, 0.650},
						{0.00,0.00,0.00,1.00, 0.19,0.07,0.02,1.00, 0.70,0.27,0.08,1.00, 0.26,0.14,0.09,1.00, 0.100},
						{0.00,0.00,0.00,1.00, 0.23,0.09,0.03,1.00, 0.55,0.21,0.07,1.00, 0.58,0.22,0.07,1.00, 0.400},
						{0.00,0.00,0.00,1.00, 0.33,0.22,0.03,1.00, 0.78,0.57,0.11,1.00, 0.99,0.94,0.81,1.00, 0.218},
						{0.00,0.00,0.00,1.00, 0.19,0.19,0.19,1.00, 0.51,0.51,0.51,1.00, 0.51,0.51,0.51,1.00, 0.400},
						{0.00,0.00,0.00,1.00, 0.23,0.23,0.23,1.00, 0.28,0.28,0.28,1.00, 0.77,0.77,0.77,1.00, 0.700},
						{0.00,0.00,0.00,1.00, 0.11,0.06,0.11,1.00, 0.43,0.47,0.54,1.00, 0.33,0.33,0.52,1.00, 0.077}
};


MMS3DMaterial::MMS3DMaterial() {
}

void MMS3DMaterial::getBuffer(MMS3D_MATERIAL **mat_buffer) {
	*mat_buffer = this->mat_buffer;
}

int MMS3DMaterial::genMaterial(MMSFBColor emission, MMSFBColor ambient, MMSFBColor diffuse, MMSFBColor specular, unsigned char shininess) {
	if (this->material_cnt > MAT_MAX_USER_DEFINED) {
		// buffer is full
		return MAT_NOTSET;
	}

	// convert to float, range 0..1
	mat_buffer[this->material_cnt].s.emission.r = (float)emission.r / 255;
	mat_buffer[this->material_cnt].s.emission.g = (float)emission.g / 255;
	mat_buffer[this->material_cnt].s.emission.b = (float)emission.b / 255;
	mat_buffer[this->material_cnt].s.emission.a = (float)emission.a / 255;
	mat_buffer[this->material_cnt].s.ambient.r  = (float)ambient.r / 255;
	mat_buffer[this->material_cnt].s.ambient.g  = (float)ambient.g / 255;
	mat_buffer[this->material_cnt].s.ambient.b  = (float)ambient.b / 255;
	mat_buffer[this->material_cnt].s.ambient.a  = (float)ambient.a / 255;
	mat_buffer[this->material_cnt].s.diffuse.r  = (float)diffuse.r / 255;
	mat_buffer[this->material_cnt].s.diffuse.g  = (float)diffuse.g / 255;
	mat_buffer[this->material_cnt].s.diffuse.b  = (float)diffuse.b / 255;
	mat_buffer[this->material_cnt].s.diffuse.a  = (float)diffuse.a / 255;
	mat_buffer[this->material_cnt].s.specular.r = (float)specular.r / 255;
	mat_buffer[this->material_cnt].s.specular.g = (float)specular.g / 255;
	mat_buffer[this->material_cnt].s.specular.b = (float)specular.b / 255;
	mat_buffer[this->material_cnt].s.specular.a = (float)specular.a / 255;
	mat_buffer[this->material_cnt].s.shininess	= (float)shininess / 255;
	this->material_cnt++;
	return this->material_cnt - 1;
}



