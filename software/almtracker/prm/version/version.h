/******************************************************************************

 version.h for Allmogetracker

 Allmogetracker receives data from a GPS receiver and transmits it
 over amature radio using the APRS protocol version 1.0.

 Copyright (C)2010, Andreas Kingbäck  (andki234@gmail.com)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#ifndef _VERSION_H_
#define _VERSION_H_

#define VERSION_DATE        __DATE__
#define VERSION_TIME        __TIME__

#define	ALPHA 0
#define BETA  1

#define VERSION_STATUS              BETA
#define ENABLE_FULL_FUNCTIONALITY   YES

static const char firm_version_P[] PROGMEM =
    {'A', 'l', 'm', 't', 'r', 'a', 'c', 'k', 'e', 'r', ' ', 'V', '0', '.', '9', '1', 0x0A, 0x0D, 0x00};

#endif /* _VERSION_H_ */
