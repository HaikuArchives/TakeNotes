/*
 * Copyright 2009, Ilio Catallo, Stefano Celentano, Eleonora Ciceri, all rights reserved
 * Distribuited under the terms of the GPL v2 license
 * 
 * Authors:
 *
 *			Eleonora Ciceri
 * 
 * Last revision: Eleonora Ciceri, 16th May 2009
 *
 * Description: TODO
 */
 
#ifndef COLOR_ITEM_H
#define COLOR_ITEM_H

#include <MenuItem.h>

class BMessage;

class ColorMenuItem : public BMenuItem {

	public:
	
							ColorMenuItem(const char*, rgb_color, BMessage*);
	 		
	protected:
	
		virtual	void		DrawContent();
					
	private:
	
				rgb_color	fColor;
};

#endif
