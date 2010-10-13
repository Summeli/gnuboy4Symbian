/* gnuboy4cute
 *
 * Copyright (C) 2010 Summeli <summeli@summeli.fi>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <e32cmn.h>

#include "bitmapblit.h"


inline void copyPixel16MU( TUint32* bitmap, TUint16* screen, TUint32 red,
					TUint32 green, TUint32 blue )
	{
	red = (*screen >> 11) & 0x1F;
	green = (*screen  >> 5) & 0x3F;
	blue = (*screen  & 0x001F);  
	
	red = (red << 3) | (red >> 2);
	green = (green << 2) | (green >> 4);
	blue = (blue << 3) | (blue >> 2);

	*bitmap = (red << 16) | (green << 8) | (blue);
	}

/*
void basicBlit( TUint8* aScreen, TUint8* aBitmap )
	{
	TUint32* bitmap = (TUint32*) aBitmap;
	TUint16* screen = (TUint16*) aScreen;

	TUint32 red = 0;
	TUint32 green = 0;
	TUint32 blue = 0;
	
	for(TInt j=0; j<144;j++)
		{
		for( TInt i=0; i<160; i++)
			{
			copyPixel16MU( bitmap, screen, red, green, blue );
			bitmap++;
			screen++;
			}
		}
	
	}
	*/
void basicBlit( TUint8* aScreen, TUint8* aBitmap )
	{
	TUint32* bitmap = (TUint32*) aBitmap;
	TUint16* screen = (TUint16*) aScreen;
	TUint32* screen_temp;
	
	TUint32 red = 0;
	TUint32 green = 0;
	TUint32 blue = 0;
	
	TUint32 extintw = 0;
	TUint32 extinth = 0;
	for(TInt j=0; j<144;j++)
		{
		for( TInt i=0; i<160; i++)
			{
			copyPixel16MU( bitmap, screen, red, green, blue );
			bitmap++;
			copyPixel16MU( bitmap, screen, red, green, blue );
			bitmap++;
			extintw++;
			if( extintw == 2 )
				{
				copyPixel16MU( bitmap, screen, red, green, blue );
				bitmap++;
				extintw = 0;
				}
			screen++;
			}
		 screen_temp = bitmap - 400;
		 memcpy(bitmap, screen_temp, 4*400);
		 bitmap += 400;
		 extinth++;
		 if( extinth == 2 )
			 {
			 screen_temp = bitmap - 400;
			 memcpy(bitmap, screen_temp, 4*400);
			 bitmap += 400;
			 extinth = 0;
			 }
		}
	
	}

