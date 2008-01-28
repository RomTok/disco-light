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

#include "mmsgui/mmsmenu.h"
#include "mmsgui/mmsslider.h"



MMSMenu::MMSMenu(MMSWindow *root, string className, MMSTheme *theme) : MMSWidget::MMSWidget() {
    /* initialize the callbacks */
    onSelectItem = new sigc::signal<void, MMSWidget*>;
    onBeforeScroll = new sigc::signal<void, MMSWidget*>;

    create(root, className, theme);
}

MMSMenu::~MMSMenu() {
    /* delete the callbacks */
    if (onSelectItem) delete onSelectItem;
    if (onBeforeScroll) delete onBeforeScroll;

    if (this->itemTemplate)
        delete this->itemTemplate;
}

bool MMSMenu::create(MMSWindow *root, string className, MMSTheme *theme) {
    this->className = className;
    if (theme) this->theme = theme; else this->theme = globalTheme;
    this->menuClass = this->theme->getMenuClass(className);
    this->baseWidgetClass = &(this->theme->menuClass.widgetClass);
    if (this->menuClass) this->widgetClass = &(this->menuClass->widgetClass); else this->widgetClass = NULL;

    this->itemTemplate = NULL;

    this->item_w = 0; 
    this->item_h = 0;
    this->x = 0;
    this->y = 0;
    this->px = 0; 
    this->py = 0; 
    this->v_items = 0; 
    this->h_items = 0;

    this->firstFocus = false;
    this->firstSelection = false;

    this->zoomselwidth = 0;      
    this->zoomselheight = 0;
    this->zoomselshiftx = 0;
    this->zoomselshifty = 0;
    
    smooth_scrolling = getSmoothScrolling();
    scrolling_offset = 0;
    
    return MMSWidget::create(root, true, false, true, true, true, false);
}

MMSWidget *MMSMenu::copyWidget() {
    /* create widget */
    MMSMenu *newWidget = new MMSMenu(this->rootwindow, className); 

    /* copy widget */
    *newWidget = *this;

    /* copy base widget */
    MMSWidget::copyWidget((MMSWidget*)newWidget);

    return newWidget;
}


void MMSMenu::add(MMSWidget *widget) {
    /* no widget to be added here, see newItem() */
}



void MMSMenu::adjustVirtualRect() {
    /* we use a virtual rectangle to support smooth move of items outside widgets inner geometry */
    /* per default the rectangles are the same */
    this->virtualGeom = this->innerGeom;

    /* if smooth scrolling we need to adjust the virtual rect */
	if (this->smooth_scrolling) {
		if (getFixedPos() >= 0) {
			/* menu with fixed selection */
			if (getCols() == 1) {
				/* fixed vertical menu */
				int rih = item_h + getItemVMargin() * 2;
				int used_h = this->h_items * rih + this->zoomselwidth;
				int min_h = this->virtualGeom.h + rih;
				while (used_h < min_h) {
					used_h+=rih;
					this->v_items++;
				}
				this->virtualGeom.y-= (used_h - this->virtualGeom.h)/2;
				this->virtualGeom.h = used_h;
			}
			else {
				/* fixed horizontal menu */
				int riw = item_w + getItemHMargin() * 2;
				int used_w = this->h_items * riw + this->zoomselwidth;
				int min_w = this->virtualGeom.w + riw;
				while (used_w < min_w) {
					used_w+=riw;
					this->h_items++;
				}
				this->virtualGeom.x-= (used_w - this->virtualGeom.w)/2;
				this->virtualGeom.w = used_w;
			}
		}
	}
}

bool MMSMenu::getConfig(bool *firstTime) {
    unsigned int rest;

    if (!isGeomSet()) {
        /* i must have my geometry */
        MMSWindow *root = getRootWindow();
        if (root) {
            root->recalculateChildren();
        }
        else
            return false;
    }

    /* check if config already set */
    if (this->item_w) {
        if (firstTime) *firstTime = false;

        /* we need to adjust the virtual rect */
        adjustVirtualRect();
        
        return true;
    }
    else
        if (firstTime) *firstTime = true;

    /* not set, fill my variables */
    if (getItemWidth() != "")
        getPixelFromSizeHint(&this->item_w, getItemWidth(), this->innerGeom.w, 0);
    else
        this->item_w = this->innerGeom.w;

    if (getItemHeight() != "")
        getPixelFromSizeHint(&this->item_h, getItemHeight(), this->innerGeom.h, 0);
    else
        this->item_h = this->innerGeom.h; 

    if (this->item_w <= 0) {
        /* it seems that width should be a factor of height */
        getPixelFromSizeHint(&this->item_w, getItemWidth(), this->innerGeom.w, this->item_h);
    }
    else {
        /* it seems that height should be a factor of width */
        getPixelFromSizeHint(&this->item_h, getItemHeight(), this->innerGeom.h, this->item_w);
    }

    if (getZoomSelWidth() != "")
        getPixelFromSizeHint((int*)&this->zoomselwidth, getZoomSelWidth(), this->item_w, 0);
    else
        this->zoomselwidth = 0;
    
    if (getZoomSelHeight() != "")
        getPixelFromSizeHint((int*)&this->zoomselheight, getZoomSelHeight(), this->item_h, 0);
    else
        this->zoomselheight = 0;
    
    this->zoomsel = ((this->zoomselwidth)||(this->zoomselheight));

   	if (this->zoomsel) {
	    if (getZoomSelShiftX() != "")
	        getPixelFromSizeHint((int*)&this->zoomselshiftx, getZoomSelShiftX(), this->zoomselwidth, 0);
	    else
	        this->zoomselshiftx = 0;
	
	    if (getZoomSelShiftY() != "")
	        getPixelFromSizeHint((int*)&this->zoomselshifty, getZoomSelShiftY(), this->zoomselheight, 0);
	    else
	        this->zoomselshifty = 0;
    }
    
    
	/* calculate visible horizontal items */
	this->h_items = (this->innerGeom.w - this->zoomselwidth) / this->item_w;
	rest = (this->innerGeom.w - this->zoomselwidth) % this->item_w;
	do {
	    if (this->h_items * getItemHMargin() * 2 <= rest)
	        break;
	    this->h_items--;
	    rest+=item_w;
	} while (this->h_items > 0);
	if (this->h_items == 0) {
	    this->h_items = 1;
	    this->item_w-=getItemHMargin() * 2;
	}

	/* calculate visible vertical items */
    this->v_items = (this->innerGeom.h - this->zoomselheight) / this->item_h;
    rest = (this->innerGeom.h - this->zoomselheight) % this->item_h;
    do {
        if (this->v_items * getItemVMargin() * 2 <= rest)
            break;
        this->v_items--;
        rest+=item_h;
    } while (this->v_items > 0);
    if (this->v_items == 0) {
        this->v_items = 1;
        this->item_h-=getItemVMargin() * 2;
    }

    /* columns */
    if (getCols()==0)
        setCols(this->h_items);

    /* we need to adjust the virtual rect */
    adjustVirtualRect();
    
    return true;
}




void MMSMenu::recalculateChildren() {
    DFBRectangle rect;
    bool         firstTime;
    int 		 item_hmargin;
    int 		 item_vmargin;
    unsigned int cols;
    int          fixedpos, realpos;

    /* check something first */
    if(this->children.empty())
        return;

    if(!isGeomSet())
        return;

    if (!getConfig(&firstTime))
        return;

    /* get values */
    item_hmargin = getItemHMargin();
    item_vmargin = getItemVMargin();
    cols = getCols();
    fixedpos = getFixedPos();

    /* normal menu or fixed selection? */
    if (fixedpos < 0) {
        /* normal menu */

        /* item pos with margin */
        int item_xx = this->virtualGeom.x + item_hmargin;
        int item_yy = this->virtualGeom.y + item_vmargin;
    
        /* item width/height with margin */
        int item_ww = item_w + item_hmargin * 2;
        int item_hh = item_h + item_vmargin * 2;
    
        /* menu pos */
        int menu_xx = this->virtualGeom.x + this->virtualGeom.w - item_w - item_hmargin;
        int menu_yy = this->virtualGeom.y + this->virtualGeom.h - item_h - item_vmargin;

        /* calc some help values if the selected item should zoomed in */
        int selected_item, rows_before, rows_after, selected_col;
       	if (this->zoomsel) {
			selected_item = getSelected();
			rows_before = (selected_item / cols) * cols;
			rows_after = rows_before + cols;
			selected_col = selected_item % cols;
       	}

       	/* through all items */
        for(int i = 0; i < (int)this->children.size(); i++) {
            rect.x = item_xx + (i % (int)cols - (int)px) * item_ww;
            rect.y = item_yy + (i / (int)cols - (int)py) * item_hh;
            rect.w = item_w;
            rect.h = item_h;  
    
            bool visibleBefore = this->children.at(i)->isVisible();
            bool visibleAfter = (!((rect.x < item_xx) || (rect.y < item_yy) || (rect.x > menu_xx) || (rect.y > menu_yy)));
    
            if (visibleBefore || visibleAfter) {
                if (visibleAfter) {
                    /* the item is visible, set geometry for it */
                   	if (this->zoomsel) {
                   		/* the selected item should zoomed in, so i have to align the items around it */
						if (cols == 1) {
						    /* menu with one column (vertical) */
							if (i > selected_item)
								rect.y+=this->zoomselheight;                

							if (i == selected_item) {
								rect.w+=this->zoomselwidth;                
								rect.h+=this->zoomselheight;                
							}
							else {
								int xxx = this->zoomselwidth / 2;
								rect.x+=xxx;
								if (this->zoomselshiftx > 0) {
									xxx+=this->zoomselwidth % 2;
									if (this->zoomselshiftx < xxx)
										xxx=this->zoomselshiftx;
								}
								else {
									xxx*=-1;
									if (this->zoomselshiftx > xxx)
										xxx=this->zoomselshiftx;
								}
								rect.x+=xxx;
							}
						}                	
						else {
						    /* menu with columns and rows */
						    if (i < rows_before) {
								/* the rows before */
								if (i > selected_col)
									rect.x+=this->zoomselwidth;                

								if (i == selected_col) {
									int xxx = this->zoomselwidth / 2;
									rect.x+=xxx;
									if (this->zoomselshiftx > 0) {
										xxx+=this->zoomselwidth % 2;
										if (this->zoomselshiftx < xxx)
											xxx=this->zoomselshiftx;
									}
									else {
										xxx*=-1;
										if (this->zoomselshiftx > xxx)
											xxx=this->zoomselshiftx;
									}
									rect.x+=xxx;
								
								}
						    }
						    else
						    if (i < rows_after) {
						    	/* current row */
						    	if (i > selected_item)
						    		rect.x+=this->zoomselwidth;                

						    	if (i == selected_item) {
									rect.w+=this->zoomselwidth;                
									rect.h+=this->zoomselheight;                
								}
								else {
									int yyy = this->zoomselheight / 2;
									rect.y+=yyy;
									if (this->zoomselshifty > 0) {
										yyy+=this->zoomselheight % 2;
										if (this->zoomselshifty < yyy)
											yyy=this->zoomselshifty;
									}
									else {
										yyy*=-1;
										if (this->zoomselshifty > yyy)
											yyy=this->zoomselshifty;
									}
									rect.y+=yyy;
								}
							}
							else {
								/* the rows after */
								rect.y+=this->zoomselheight;                
						
								if (i > selected_col)
									rect.x+=this->zoomselwidth;                

								if (i == selected_col) {
									int xxx = this->zoomselwidth / 2;
									rect.x+=xxx;
									if (this->zoomselshiftx > 0) {
										xxx+=this->zoomselwidth % 2;
										if (this->zoomselshiftx < xxx)
											xxx=this->zoomselshiftx;
									}
									else {
										xxx*=-1;
										if (this->zoomselshiftx > xxx)
											xxx=this->zoomselshiftx;
									}
									rect.x+=xxx;
								
								}
							}
						}
                   	}

                   	
//printf("rect=%s,%d,%d,%d,%d\n", this->children.at(i)->getName().c_str(),rect.x,rect.y,rect.w,rect.h);
//printf("rectx=%d,%d\n", this->innerGeom.x,this->innerGeom.y);
//printf("rectx=%d,%d\n", this->virtualGeom.x,this->virtualGeom.y);

//					rect.x+=this->innerGeom.x;
//					rect.y+=this->innerGeom.y;
                	this->children.at(i)->setGeometry(rect);
                }
    
                /* switch the visibility */
                this->children.at(i)->setVisible(visibleAfter, false);
            }

        
    		if ((i+1)%cols==0)
    			selected_col+=cols;
        
        }
    
        return;
    }
    else {
        /* menu with fixed selection */
        if (cols == 1) {
            /* menu with one column (vertical) */
            if (fixedpos >= this->v_items)
                fixedpos = (this->v_items - 1) / 2;

            /* item pos with margin */
            int item_xx = this->virtualGeom.x + item_hmargin;
            int item_yy = this->virtualGeom.y + item_vmargin;
        
            /* item height with margin */
            int item_hh = item_h + item_vmargin * 2;
        
            /* menu pos */
            int menu_yy = this->virtualGeom.y + this->virtualGeom.h - item_h - item_vmargin;

            /* check if the number of children is less than visible vertical items */
            int lessItems = (int)this->v_items - (int)this->children.size();
            if (lessItems > 0) {
                /* yes, I have to recalculate the temporary menu area around the fixedpos */
                int items_above = (lessItems * (100 * fixedpos) / (this->v_items - 1) + 50) / 100;
                item_yy+= items_above * item_hh;
                fixedpos-= items_above;
                menu_yy-= (lessItems - items_above) * item_hh;
            }

            /* through all items */
            for(int i = 0; i < (int)this->children.size(); i++) {
                rect.x = item_xx;
                rect.y = item_yy + (i + fixedpos - (int)py) * item_hh;
                rect.w = item_w;
                rect.h = item_h;  
        
                bool visibleBefore = this->children.at(i)->isVisible();
                bool visibleAfter = (!((rect.y < item_yy) || (rect.y > menu_yy)));
                if (visibleAfter)
                    realpos = (rect.y - item_yy) / item_hh;
                else
                    realpos = -1;

                if (!visibleAfter) {
                    /* the item is will not visible, but fixedpos is set, try with it */
                    if (rect.y < item_yy) {
                        /* items after, added at bottom of the menu */
                        rect.y = item_yy + (i + (int)this->children.size() + fixedpos - (int)py) * item_hh;
                        visibleAfter = (!((rect.y < item_yy) || (rect.y > menu_yy)));
                        if (visibleAfter)
                            realpos = (rect.y - item_yy) / item_hh;
                    }
                    else {
                        /* items before, added at top of the menu */
                        int ic = (int)this->children.size() - fixedpos + (int)py;
                        if (i >= ic) {
                            /* try if it can be visible from top of menu */
                            rect.y = item_yy + (i - ic) * item_hh;
                            visibleAfter = (!((rect.y < item_yy) || (rect.y > menu_yy)));
                            if (visibleAfter)
                                realpos = (rect.y - item_yy) / item_hh;
                        } 
                    }
                }
        
                if (visibleBefore || visibleAfter) {
                    if (visibleAfter) {
                        /* the item is visible, set geometry for it */
                       	if (this->zoomsel) {
                       		/* the selected item should zoomed in, so i have to align the items around it */
							if (realpos > fixedpos)
								rect.y+=this->zoomselheight;                

							if (realpos == fixedpos) {
								rect.w+=this->zoomselwidth;                
								rect.h+=this->zoomselheight;                
							}
							else {
								int xxx = this->zoomselwidth / 2;
								rect.x+=xxx;
								if (this->zoomselshiftx > 0) {
									xxx+=this->zoomselwidth % 2;
									if (this->zoomselshiftx < xxx)
										xxx=this->zoomselshiftx;
								}
								else {
									xxx*=-1;
									if (this->zoomselshiftx > xxx)
										xxx=this->zoomselshiftx;
								}
								rect.x+=xxx;
							}
                       	}                    	
                    	
                    	this->children.at(i)->setGeometry(rect);

                        /* dim down and/or increase transparency */
                        int bn = 255;
                        int op = 255;
                        if (realpos < fixedpos) {
                            /* pos before selected item */
                            int dim_top = getDimTop();
                            if (dim_top > 0)
                                if (realpos+1 < fixedpos) {
                                    bn = 255 - dim_top + ((realpos * dim_top * 10) / (fixedpos - 1) + 5) / 10;
                                }
                            int trans_top = getTransTop();
                            if (trans_top > 0)
                                if (realpos+1 < fixedpos) {
                                    op = 255 - trans_top + ((realpos * trans_top * 10) / (fixedpos - 1) + 5) / 10;
                                }
                        }
                        else
                        if (realpos > fixedpos) {
                            /* pos after selected item */
                            int dim_bottom = getDimBottom();
                            if (dim_bottom > 0)
                                if (realpos-1 > fixedpos) {
                                    bn = 255 - (((realpos - fixedpos - 1) * dim_bottom * 10) / (this->v_items - fixedpos - 2) + 5) / 10;
                                }
                            int trans_bottom = getTransBottom();
                            if (trans_bottom > 0)
                                if (realpos-1 > fixedpos) {
                                    op = 255 - (((realpos - fixedpos - 1) * trans_bottom * 10) / (this->v_items - fixedpos - 2) + 5) / 10;
                                }
                        }

                        /* check if focused */
                        if ((!isFocused())&&(this->firstFocus)) {
                            bn-= getDimItems();
                            op-= getTransItems();
                        }

                        /* set brightness */
                        if (bn < 0) bn = 0;
                        if (bn > 255) bn = 255;
                        this->children.at(i)->setBrightness(bn, false);

                        /* set opacity */
                        if (op < 0) op = 0;
                        if (op > 255) op = 255;
                        this->children.at(i)->setOpacity(op, false);
                    }

                    /* switch the visibility */
                    this->children.at(i)->setVisible(visibleAfter, false);
                }
            }
        }
        else {
            /* menu with one row (horizontal) */
            if (fixedpos >= this->h_items)
                fixedpos = (this->h_items - 1) / 2;
            
            /* item pos with margin */
            int item_xx = this->virtualGeom.x + item_hmargin;
            int item_yy = this->virtualGeom.y + item_vmargin;
        
            /* item width with margin */
            int item_ww = item_w + item_hmargin * 2;
        
            /* menu pos */
            int menu_xx = this->virtualGeom.x + this->virtualGeom.w - item_w - item_hmargin;

            /* check if the number of children is less than visible horizontal items */
            int lessItems;
            if (cols < this->children.size())
                lessItems = (int)this->h_items - (int)cols;
            else
                lessItems = (int)this->h_items - (int)this->children.size();
            if (lessItems > 0) {
                /* yes, I have to recalculate the temporary menu area around the fixedpos */
                int items_left = (lessItems * (100 * fixedpos) / (this->h_items - 1) + 50) / 100;
                item_xx+= items_left * item_ww;
                fixedpos-= items_left;
                menu_xx-= (lessItems - items_left) * item_ww;
            }

            /* through all items */
            for(int i = 0; i < (int)this->children.size(); i++) {

                if (i >= (int)cols) {
                    /* out of first row */
                    this->children.at(i)->setVisible(false, false);
                    continue;
                }

                rect.x = item_xx + (i + fixedpos - (int)px) * item_ww;

                if (smooth_scrolling)
                	rect.x+=scrolling_offset;

				rect.y = item_yy;
                rect.w = item_w;
                rect.h = item_h;  
        
                bool visibleBefore = this->children.at(i)->isVisible();
                bool visibleAfter = (!((rect.x < item_xx) || (rect.x > menu_xx)));

                if (visibleAfter)
                    realpos = (rect.x - item_xx) / item_ww;
                else
                    realpos = -1;

                if (!visibleAfter) {
                    /* the item is will not visible, but fixedpos is set, try with it */
                    if (rect.x < item_xx) {
                        /* items after, added at right of the menu */
                        if (cols < this->children.size())
                            rect.x = item_xx + (i + (int)cols + fixedpos - (int)px) * item_ww;
                        else
                            rect.x = item_xx + (i + (int)this->children.size() + fixedpos - (int)px) * item_ww;

                        if (!smooth_scrolling)
                        	visibleAfter = (!((rect.x < item_xx) || (rect.x > menu_xx)));
                        else {
                        	rect.x+=scrolling_offset;
                        	visibleAfter = (!((rect.x+rect.w+(int)item_hmargin < item_xx) || (rect.x-rect.w-(int)item_hmargin > menu_xx)));
                        }

                        if (visibleAfter)
                            realpos = (rect.x - item_xx) / item_ww;
                    }
                    else {
                        /* items before, added at left of the menu */
                        int ic;
                        if (cols < this->children.size())
                            ic = (int)cols - fixedpos + (int)px;
                        else
                            ic = (int)this->children.size() - fixedpos + (int)px;

                        if (i >= ic) {
                            /* try if it can be visible from left of menu */
                            rect.x = item_xx + (i - ic) * item_ww;

                            if (!smooth_scrolling)
                            	visibleAfter = (!((rect.x < item_xx) || (rect.x > menu_xx)));
                            else {
                            	rect.x+=scrolling_offset;
                            	visibleAfter = (!((rect.x+rect.w+(int)item_hmargin < item_xx) || (rect.x-rect.w-(int)item_hmargin > menu_xx)));
                            }

                            if (visibleAfter)
                                realpos = (rect.x - item_xx) / item_ww;
                        } 
                    }
                }
        
                if (visibleBefore || visibleAfter) {
                    if (visibleAfter) {
                        /* the item is visible, set geometry for it */
                       	if (this->zoomsel) {
                       		/* the selected item should zoomed in, so i have to align the items around it */
			                if (!smooth_scrolling) {
			                	/* smooth scrolling not desired */
								if (realpos > fixedpos) {
									rect.x+=(int)this->zoomselwidth;
								}

								if (realpos == fixedpos) {
									rect.w+=this->zoomselwidth;
									rect.h+=this->zoomselheight;
								}
								else {
									int yyy = this->zoomselheight / 2;
									rect.y+=yyy;
									if (this->zoomselshifty > 0) {
										yyy+=this->zoomselheight % 2;
										if (this->zoomselshifty < yyy)
											yyy=this->zoomselshifty;
									}
									else {
										yyy*=-1;
										if (this->zoomselshifty > yyy)
											yyy=this->zoomselshifty;
									}
									rect.y+=yyy;
								}
			                }
			                else {
			                	/* smooth scrolling */

			                	/* get the percent of scrolling */
				                int d = (10000*scrolling_offset) / (this->item_w + item_hmargin*2);
				                
								if (realpos > fixedpos) {
									rect.x+=(int)this->zoomselwidth;
								}
	
								if (d>=0)
								{
									if (realpos == fixedpos) {
										/* set blend value */
										this->children.at(i)->setBlend((d*255 + 5000) / 10000, false);										

										/* calculate item's rectangle */
										int dd = ((int)this->zoomselwidth * d + 5000) / 10000;
										rect.x+=dd;
										rect.y+=((((int)this->zoomselheight/2)+this->zoomselshifty) * d + 5000) / 10000;
										rect.w+=(int)this->zoomselwidth - dd;
										rect.h+=(int)this->zoomselheight - ((int)this->zoomselheight * d + 5000) / 10000;
									}
									else {
										int yyy = this->zoomselheight / 2;
										rect.y+=yyy;
										if (this->zoomselshifty > 0) {
											yyy+=this->zoomselheight % 2;
											if (this->zoomselshifty < yyy)
												yyy=this->zoomselshifty;
										}
										else {
											yyy*=-1;
											if (this->zoomselshifty > yyy)
												yyy=this->zoomselshifty;
										}
										rect.y+=yyy;
		
										if ((d>0)&&(realpos+1 == fixedpos)) {
											/* set blend value */
											this->children.at(i)->setBlend((d*255 + 5000) / 10000, false);										

											/* calculate item's rectangle */
											rect.y-=((((int)this->zoomselheight/2)+this->zoomselshifty) * d + 5000) / 10000;
											rect.w+=((int)this->zoomselwidth * d + 5000) / 10000;
											rect.h+=((int)this->zoomselheight * d + 5000) / 10000;                
										}
										else
											/* reset blend value */
											this->children.at(i)->setBlend(0, false);										
									}
								}
								else
								{
									if (realpos+1 == fixedpos) {
										/* set blend value */
										this->children.at(i)->setBlend((d*-255 + 5000) / 10000, false);										

										/* calculate item's rectangle */
										rect.y-=((((int)this->zoomselheight/2)+this->zoomselshifty) * d - 5000) / 10000;
										rect.w+=(int)this->zoomselwidth + ((int)this->zoomselwidth * d - 5000) / 10000;
										rect.h+=(int)this->zoomselheight + ((int)this->zoomselheight * d - 5000) / 10000;
									}
									else {
										int yyy = this->zoomselheight / 2;
										rect.y+=yyy;
										if (this->zoomselshifty > 0) {
											yyy+=this->zoomselheight % 2;
											if (this->zoomselshifty < yyy)
												yyy=this->zoomselshifty;
										}
										else {
											yyy*=-1;
											if (this->zoomselshifty > yyy)
												yyy=this->zoomselshifty;
										}
										rect.y+=yyy;
		
										if (realpos == fixedpos) {
											/* set blend value */
											this->children.at(i)->setBlend((d*-255 + 5000) / 10000, false);										
											
											/* calculate item's rectangle */
											rect.x+=this->zoomselwidth+((int)this->zoomselwidth * d - 5000) / 10000;
											rect.y+=((((int)this->zoomselheight/2)+this->zoomselshifty) * d - 5000) / 10000;
											rect.w-=((int)this->zoomselwidth * d - 5000) / 10000;
											rect.h-=((int)this->zoomselheight * d - 5000) / 10000;
										}
										else
											/* reset blend value */
											this->children.at(i)->setBlend(0, false);										
									}
	                       		}
							
			                }
                       	}
							
                    	this->children.at(i)->setGeometry(rect);

                        /* dim down and/or increase transparency */
                        int bn = 255;
                        int op = 255;
                        if (realpos < fixedpos) {
                            /* pos before selected item */
                            int dim_left = getDimLeft();
                            if (dim_left > 0)
                                if (realpos+1 < fixedpos) {
                                    bn = 255 - dim_left + ((realpos * dim_left * 10) / (fixedpos - 1) + 5) / 10;
                                }
                            int trans_left = getTransLeft();
                            if (trans_left > 0)
                                if (realpos+1 < fixedpos) {
                                    op = 255 - trans_left + ((realpos * trans_left * 10) / (fixedpos - 1) + 5) / 10;
                                }
                        }
                        else
                        if (realpos > fixedpos) {
                            /* pos after selected item */
                            int dim_right = getDimRight();
                            if (dim_right > 0)
                                if (realpos-1 > fixedpos) {
                                    bn = 255 - (((realpos - fixedpos - 1) * dim_right * 10) / (this->h_items - fixedpos - 2) + 5) / 10;
                                }
                            int trans_right = getTransRight();
                            if (trans_right > 0)
                                if (realpos-1 > fixedpos) {
                                    op = 255 - (((realpos - fixedpos - 1) * trans_right * 10) / (this->h_items - fixedpos - 2) + 5) / 10;
                                }
                        }

                        /* check if focused */
                        if (!isFocused()) {
                            bn-= getDimItems();
                            op-= getTransItems();
                        }

                        /* set brightness */
                        if (bn < 0) bn = 0;
                        if (bn > 255) bn = 255;
                        this->children.at(i)->setBrightness(bn, false);

                        /* set opacity */
                        if (op < 0) op = 0;
                        if (op > 255) op = 255;
                        this->children.at(i)->setOpacity((unsigned char)op, false);
                    }

                    /* switch the visibility */
                    this->children.at(i)->setVisible(visibleAfter, false);
                }
            }
        }

        return;
    }
}


void MMSMenu::switchArrowWidgets() {
    /* connect arrow widgets */
    loadArrowWidgets();

    /* get columns */
    unsigned int cols = getCols();

    /* arrow support is not needed for menus with fixed selection position */
    if (getFixedPos() >= 0) {
        /* select the correct arrows */
        if (cols > 1) {
            /* horizontal menu */
            if (this->leftArrowWidget)    
                this->leftArrowWidget->setSelected(true);
            if (this->rightArrowWidget)    
                this->rightArrowWidget->setSelected(true);
            if (this->upArrowWidget)    
                this->upArrowWidget->setSelected(false);
            if (this->downArrowWidget)    
                this->downArrowWidget->setSelected(false);
        }
        else {
            /* vertical menu */
            if (this->leftArrowWidget)    
                this->leftArrowWidget->setSelected(false);
            if (this->rightArrowWidget)    
                this->rightArrowWidget->setSelected(false);
            if (this->upArrowWidget)    
                this->upArrowWidget->setSelected(true);
            if (this->downArrowWidget)    
                this->downArrowWidget->setSelected(true);
        }
        return;
    }

    /* switch arrow widgets */
    if (this->leftArrowWidget) {    
        if (this->px == 0)
            this->leftArrowWidget->setSelected(false);
        else
            this->leftArrowWidget->setSelected(true);
    }

    if (this->upArrowWidget) {
        if (this->py == 0)
            this->upArrowWidget->setSelected(false);
        else
            this->upArrowWidget->setSelected(true);
    }

    if (this->rightArrowWidget) {
        unsigned int columns = cols;
        if (columns > children.size())
            columns = children.size();
        if ((int)(columns - this->px) > this->h_items)
            this->rightArrowWidget->setSelected(true);
        else
            this->rightArrowWidget->setSelected(false);
    }

    if (this->downArrowWidget) {
        if ((int)(children.size() / cols + ((children.size() % cols)?1:0) - this->py) > this->v_items)
            this->downArrowWidget->setSelected(true);
        else
            this->downArrowWidget->setSelected(false);
    }
}

void MMSMenu::setSliders() {

    /* get columns */
    unsigned int cols = getCols();

    if (this->vSliderWidget) {
        unsigned int pos = 0;
        int size = (int)children.size() - 1;
        if (size > 0)
            pos = (this->y * 100) / (size / cols + ((size % cols)?1:0));
        ((MMSSlider *)this->vSliderWidget)->setPosition(pos);
    }

    if ((this->hSliderWidget)&&(cols>1)) {
        unsigned int pos = 0;
        int size = (int)children.size() - 1;
        if (size >= (int)cols) size = cols - 1;
        if (size > 0)
            pos = (this->x * 100) / size;
        ((MMSSlider *)this->hSliderWidget)->setPosition(pos);
    }
}

void MMSMenu::selectItem(MMSWidget *item, bool set, bool refresh) {
    item->setSelected(set, refresh);
    if (set)
        this->onSelectItem->emit(item);
}

bool MMSMenu::scrollDownEx(unsigned int count, bool refresh, bool test) {
    bool pyChanged = false;
    unsigned int oldx=0;
    unsigned int oldy;
    unsigned int cols;
    int fixedpos;

    /* check something */
    if (count==0 || children.empty())
        return false;

    /* get settings */
    cols = getCols();
    fixedpos = getFixedPos();

    /* test for deactivated items */
    while((this->x + (this->y + count) * cols) < children.size()) {
        if(children.at(this->x + (this->y + count) * cols)->isActivated()) break;
        count++;
    }

    /* normal menu or fixed selection? */
    if (fixedpos < 0) {
        /* normal menu */
        if (this->x + (this->y + count) * cols >= children.size()) {
            if (this->x == 0) {
                /* really nothing to scroll? */
                if (getVLoop()) {
                    /* I should not give up the focus */
                    if (this->y)
                        return scrollUpEx(this->y, refresh, test);
                    else
                        return true;
                }
                /* nothing to scroll */
                return false;
            }

            for (int i = (int)this->x - 1; i >= 0; i--)
                if (i + (this->y + count) * cols < children.size()) {
                    /* save old and set new x selection */
                    oldx = this->x;
                    if (!test)
                        this->x = i;
                    break;
                }

            if (!oldx) {
                /* really nothing to scroll? */
                if (getVLoop()) {
                    /* I should not give up the focus */
                    if (this->y)
                        return scrollUpEx(this->y, refresh, test);
                    else
                        return true;
                }
                /* nothing to scroll */
                return false;
            }
        }
    
        /* in test mode we can say that we can scroll */
        if (test)
            return true;

        /* save old and set new y selection */
        oldy = this->y;
        this->y+=count;

        /* recalculate scroll position */    
        if ((int)(this->y - this->py) >= this->v_items) {
            this->py = this->y - this->v_items + 1;
            pyChanged = true;
        }
        if (oldx) {
            if (this->x < this->px) {
                this->px = this->x;
                pyChanged = true;
            }
        }

        /* get access to widgets */    
        MMSWidget *olditem = children.at(((oldx)?oldx:this->x) + oldy * cols);
        MMSWidget *item    = children.at(this->x + this->y * cols);
    
        if (!pyChanged) {
            /* not scrolled, switch focus between visible childs */
			selectItem(olditem, false, false);
			selectItem(item, true, false);
			if (refresh) {
			    this->refresh();
			}
        }
        else {
            /* scrolled, switch focus needs recalculate children */
            selectItem(olditem, false, false);
    
            if (refresh)
                recalculateChildren();
    
            selectItem(item, true, false);
    
            if (refresh) {
                this->refresh();
            }
        }

        /* set the sliders */
        setSliders();
    
        return true;
    }
    else {
        /* menu with fixed selection */
        if (cols == 1) {
            /* in test mode we can say that we can scroll */
            if (test)
                return true;

            /* correct menu with one column */
            count%=children.size();

            /* save old and set new y selection */
            oldy = this->y;
            this->y+=count;

            if (this->y >= children.size()) {
                /* go back to begin of the list (round robin) */
                this->y = this->y - children.size();
            }

            /* recalculate scroll position */
            this->py = this->y;
            
            /* get access to widgets */    
            MMSWidget *olditem = children.at(oldy);
            MMSWidget *item    = children.at(this->y);
        
            /* switch focus and recalculate children */
            selectItem(olditem, false, false);
    
            if (refresh)
                recalculateChildren();
    
            selectItem(item, true, false);
    
            if (refresh) {
                this->refresh();
            }

            /* set the sliders */
            setSliders();
    
            return true;
        }
        else
            /* menu with more than one column cannot be scrolled down in fixed selection mode */
            return false;
    }
}

bool MMSMenu::scrollUpEx(unsigned int count, bool refresh, bool test) {
    bool pyChanged = false;
    unsigned int oldy;
    unsigned int cols;
    int fixedpos;

    /* check something */
    if (count==0 || children.empty())
        return false;

    /* get settings */
    cols = getCols();
    fixedpos = getFixedPos();

    /* test for deactivated items */
    while(int(this->x + (this->y - count) * cols) > 0) {
        if(children.at(this->x + (this->y - count) * cols)->isActivated()) break;
        count++;
    }

    /* normal menu or fixed selection? */
    if (fixedpos < 0) {
        /* normal menu */
        if (this->y < count) {
            /* really nothing to scroll? */
            if (getVLoop()) {
                /* I should not give up the focus */
                unsigned int lines = this->children.size() /* / cols */; 
                /* if (this->children.size() % cols > 0) lines++; */
                if ((int)lines - (int)this->y > 1)
                    return scrollDownEx(lines - this->y - 1, refresh, test);
                else
                    return true;
            }
            /* nothing to scroll */
            return false;
        }

        /* in test mode we can say that we can scroll */
        if (test)
            return true;

        /* save old and set new y selection */
        oldy = this->y;
        this->y-=count;
    
        /* recalculate scroll position */    
        if (this->y < this->py) {
            this->py = this->y;
            pyChanged = true;
        }
    
        /* get access to widgets */    
        MMSWidget *olditem = children.at(this->x + oldy * cols);
        MMSWidget *item    = children.at(this->x + this->y * cols);
    
        if (!pyChanged) {
            /* not scrolled, switch focus between visible childs */
			selectItem(olditem, false, false);
			selectItem(item, true, false);
			if (refresh) {
			    this->refresh();
			}
        }
        else {
            /* scrolled, switch focus needs recalculate children */
            selectItem(olditem, false, false);
    
            if (refresh)
                recalculateChildren();
    
            selectItem(item, true, false);
    
            if (refresh) {
                this->refresh();
            }
        }
    
        /* set the sliders */
        setSliders();

        return true;
    }
    else {
        /* menu with fixed selection */
        if (cols == 1) {
            /* in test mode we can say that we can scroll */
            if (test)
                return true;

            /* correct menu with one column */
            count%=children.size();

            /* save old and set new y selection */
            oldy = this->y;
            this->y-=count;

            if ((int)this->y < 0) {
                /* go back to end of the list (round robin) */
                this->y = children.size() + (int)this->y;
            }

            /* recalculate scroll position */
            this->py = this->y;
            
            /* get access to widgets */    
            MMSWidget *olditem = children.at(oldy);
            MMSWidget *item    = children.at(this->y);
        
            /* switch focus and recalculate children */
            selectItem(olditem, false, false);
    
            if (refresh)
                recalculateChildren();
    
            selectItem(item, true, false);
    
            if (refresh) {
                this->refresh();
            }

            /* set the sliders */
            setSliders();
    
            return true;
        }
        else
            /* menu with more than one column cannot be scrolled up in fixed selection mode */
            return false;
    }
}


bool MMSMenu::scrollRightEx(unsigned int count, bool refresh, bool test) {
    bool pxChanged = false;
    unsigned int oldx;
    unsigned int oldy=0;
    unsigned int cols;
    int fixedpos;

    /* check something */
    if (count==0 || children.empty())
        return false;

    /* get settings */
    cols = getCols();
    fixedpos = getFixedPos();

    /* normal menu or fixed selection? */
    if (fixedpos < 0) {
        /* normal menu */
        if (this->x + count + this->y * cols >= children.size()) {
            if ((this->x + count >= cols) || (this->y == 0)) {
                /* really nothing to scroll? */
                if (getHLoop()) {
                    /* I should not give up the focus */
                    if (this->x)
                        return scrollLeftEx(this->x, refresh, test);
                    else
                        return true;
                }
                /* nothing to scroll */
                return false;
            }

            for (int i = (int)this->y - 1; i >= 0; i--)
                if (this->x + count + i * cols < children.size()) {
                    oldy = this->y;
                    if (!test)
                        this->y = i;
                    break;
                }

            if (!oldy) {
                /* really nothing to scroll? */
                if (getHLoop()) {
                    /* I should not give up the focus */
                    if (this->x)
                        return scrollLeftEx(this->x, refresh, test);
                    else
                        return true;
                }
                /* nothing to scroll */
                return false;
            }
        }
        else
        if (this->x + count >= cols) {
            /* really nothing to scroll? */
            if (getHLoop()) {
                /* I should not give up the focus */
                if (this->x)
                    return scrollLeftEx(this->x, refresh, test);
                else
                    return true;
            }
            /* nothing to scroll */
            return false;
        }

        /* in test mode we can say that we can scroll */
        if (test)
            return true;
 
        /* save old and set new x selection */
        oldx = this->x;
        this->x+=count;
    
        /* recalculate scroll position */    
        if ((int)(this->x - this->px) >= this->h_items) {
            this->px = this->x - this->h_items + 1;
            pxChanged = true;
        }
        if (oldy) {
            if (this->y < this->py) {
                this->py = this->y;
                pxChanged = true;
            }
        }
    
        /* get access to widgets */    
        MMSWidget *olditem = children.at(oldx + ((oldy)?oldy:this->y) * cols);
        MMSWidget *item    = children.at(this->x + this->y * cols);
    
        if (!pxChanged) {
            /* not scrolled, switch focus between visible childs */
			selectItem(olditem, false, false);
			selectItem(item, true, false);
			if (refresh) {
			    this->refresh();
			}
        }
        else {
            /* scrolled, switch focus needs recalculate children */
            selectItem(olditem, false, false);
    
            if (refresh)
                recalculateChildren();
    
            selectItem(item, true, false);
    
            if (refresh) {
                this->refresh();
            }
        }
    
        /* set the sliders */
        setSliders();

        return true;
    }
    else {
        /* menu with fixed selection */
        if (cols > 1) {
            /* in test mode we can say that we can scroll */
            if (test)
                return true;

            /* callback */
            this->onBeforeScroll->emit(this);
            
            if ((smooth_scrolling)&&(refresh)) {
            	/* scrolling animation */
				/* smooth scrolling with fix 5 steps */
				int sloop = 5;
//				int sloop = this->item_w + getItemHMargin()*2;
				int soffs = (getItemHMargin()*2 + this->item_w) / sloop;
				scrolling_offset=0;
				for (int z = 0; z < sloop - 1; z++) {
			        scrolling_offset-=soffs;
			        this->refresh();
				}
				scrolling_offset=0;
            }

			/* correct menu with more than one column */
            count%=cols;

            /* save old and set new x selection */
            oldx = this->x;
            this->x+=count;

            if (this->x >= cols) {
                /* go back to begin of the first row (round robin) */
                this->x = this->x - cols;
            }

            if (this->x >= children.size()) {
                /* go back to begin of the list (round robin) */
                this->x = this->x - children.size();
            }

            /* recalculate scroll position */
            this->px = this->x;
            
            /* get access to widgets */    
            MMSWidget *olditem = children.at(oldx);
            MMSWidget *item    = children.at(this->x);

            if ((smooth_scrolling)&&(refresh)) {
            	/* reset the blend value */
            	olditem->setBlend(0, false);
            	item->setBlend(0, false);
            }

            /* switch focus and recalculate children */
            selectItem(olditem, false, false);
    
            if (refresh)
                recalculateChildren();
    
            selectItem(item, true, false);
    
            if (refresh) {
                this->refresh();
            }

            /* set the sliders */
            setSliders();

            return true;
        }
        else
            /* menu with only one column cannot be scrolled right in fixed selection mode */
            return false;
    }
}

bool MMSMenu::scrollLeftEx(unsigned int count, bool refresh, bool test) {
    bool pxChanged = false;
    unsigned int oldx;
    unsigned int cols;
    int fixedpos;

    /* check something */
    if (count==0 || children.empty())
        return false;

    /* get settings */
    cols = getCols();
    fixedpos = getFixedPos();

    /* normal menu or fixed selection? */
    if (fixedpos < 0) {
        /* normal menu */
        if (this->x < count) {
            /* really nothing to scroll? */
            if (getHLoop()) {
                /* I should not give up the focus */
                unsigned int columns;
                if (cols < this->children.size())
                    columns = cols;
                else
                    columns = this->children.size();
                if ((int)columns - (int)this->x > 1)
                    return scrollRightEx(columns - this->x - 1, refresh, test);
                else
                    return true;
            }
            /* nothing to scroll */
            return false;
        }

        /* in test mode we can say that we can scroll */
        if (test)
            return true;

        /* save old and set new x selection */
        oldx = this->x;
        this->x-=count;
    
        /* recalculate scroll position */    
        if (this->x < this->px) {
            this->px = this->x;
            pxChanged = true;
        }
    
        /* get access to widgets */    
        MMSWidget *olditem = children.at(oldx + this->y * cols);
        MMSWidget *item    = children.at(this->x + this->y * cols);
    
        if (!pxChanged) {
            /* not scrolled, switch focus between visible childs */
			selectItem(olditem, false, false);
			selectItem(item, true, false);
			if (refresh) {
			    this->refresh();
			}
        }
        else {
            /* scrolled, switch focus needs recalculate children */
            selectItem(olditem, false, false);
    
            if (refresh)
                recalculateChildren();
    
            selectItem(item, true, false);
    
            if (refresh) {
                this->refresh();
            }
        }
    
        /* set the sliders */
        setSliders();

        return true;
    }
    else {
        /* menu with fixed selection */
        if (cols > 1) {
            /* in test mode we can say that we can scroll */
            if (test)
                return true;

            /* callback */
            this->onBeforeScroll->emit(this);

            if ((smooth_scrolling)&&(refresh)) {
            	/* scrolling animation */
				/* smooth scrolling with fix 5 steps */
				int sloop = 5;
//				int sloop = this->item_w + getItemHMargin()*2;
				int soffs = (getItemHMargin()*2 + this->item_w) / sloop;
				scrolling_offset=0;
				for (int z = 0; z < sloop - 1; z++) {
			        scrolling_offset+=soffs;
			        this->refresh();
				}
				scrolling_offset=0;
            }

			/* correct menu with more than one column */
            count%=cols;

            /* save old and set new x selection */
            oldx = this->x;
            this->x-=count;

            if ((int)this->x < 0) {
                /* go back to end of the first row (round robin) */
                this->x = (int)cols + (int)this->x;

                if (this->x >= children.size()) {
                    /* go back to begin of the list (round robin) */
                    this->x = children.size() - (cols - this->x);
                }
            }

            /* recalculate scroll position */
            this->px = this->x;
            
            /* get access to widgets */    
            MMSWidget *olditem = children.at(oldx);
            MMSWidget *item    = children.at(this->x);

            if ((smooth_scrolling)&&(refresh)) {
            	/* reset the blend value */
            	olditem->setBlend(0, false);
            	item->setBlend(0, false);
            }
            
            /* switch focus and recalculate children */
            selectItem(olditem, false, false);
    
            if (refresh)
                recalculateChildren();
    
            selectItem(item, true, false);
    
            if (refresh) {
                this->refresh();
            }

            /* set the sliders */
            setSliders();

            return true;
        }
        else
            /* menu with only one column cannot be scrolled right in fixed selection mode */
            return false;
    }
}

bool MMSMenu::scrollDown(unsigned int count, bool refresh, bool test) {

	if (this->children.size()==0)
		return false;
	
	if ((!test)&&(smooth_scrolling)&&(refresh)) {
	    int fixedpos = getFixedPos();
	    if (fixedpos >= 0) {
	    	/* this is a vertical menu and we have to check which is the shortest way - up or down scrolling */
	    	/* it's important to know because of smooth scrolling */
	    	count = count % this->children.size();
            if (fixedpos >= this->v_items)
                fixedpos = (this->v_items - 1) / 2;
            
	    	if (count > (this->children.size() / 2) - (fixedpos - ((this->v_items - 1) / 2))) {
	    		count = this->v_items-count;
	    		while (count--)
	    			scrollUpEx(1, refresh, test);
	    		return true;
	    	}
	    	
    		while (count--)
    			scrollDownEx(1, refresh, test);
    		return true;
	    }
	}

	return scrollDownEx(count, refresh, test);
}

bool MMSMenu::scrollUp(unsigned int count, bool refresh, bool test) {

	if (this->children.size()==0)
		return false;
	
	if ((!test)&&(smooth_scrolling)&&(refresh)) {
	    int fixedpos = getFixedPos();
	    if (fixedpos >= 0) {
	    	/* this is a vertical menu and we have to check which is the shortest way - up or down scrolling */
	    	/* it's important to know because of smooth scrolling */
	    	count = count % this->children.size();
            if (fixedpos >= this->v_items)
                fixedpos = (this->v_items - 1) / 2;
            
	    	if (count > (this->children.size() / 2) - (fixedpos - ((this->v_items - 1) / 2))) {
	    		count = this->v_items-count;
	    		while (count--)
	    			scrollDownEx(1, refresh, test);
	    		return true;
	    	}
	    	
    		while (count--)
    			scrollUpEx(1, refresh, test);
    		return true;
	    }
	}

	return scrollUpEx(count, refresh, test);
}

bool MMSMenu::scrollRight(unsigned int count, bool refresh, bool test) {

	if (this->children.size()==0)
		return false;

	if ((!test)&&(smooth_scrolling)&&(refresh)) {
	    int fixedpos = getFixedPos();
	    if (fixedpos >= 0) {
	    	/* this is a horizontal menu and we have to check which is the shortest way - left or right scrolling */
	    	/* it's important to know because of smooth scrolling */
	    	count = count % this->children.size();
            if (fixedpos >= this->h_items)
                fixedpos = (this->h_items - 1) / 2;
            
	    	if (count > (this->children.size() / 2) - (fixedpos - ((this->h_items - 1) / 2))) {
	    		count = this->children.size()-count;
	    		while (count--)
	    			scrollLeftEx(1, refresh, test);
	    		return true;
	    	}

    		while (count--)
    			scrollRightEx(1, refresh, test);
    		return true;
	    }
	}

	return scrollRightEx(count, refresh, test);
}

bool MMSMenu::scrollLeft(unsigned int count, bool refresh, bool test) {

	if (this->children.size()==0)
		return false;

	if ((!test)&&(smooth_scrolling)&&(refresh)) {
	    int fixedpos = getFixedPos();
	    if (fixedpos >= 0) {
	    	/* this is a horizontal menu and we have to check which is the shortest way - left or right scrolling */
	    	/* it's important to know because of smooth scrolling */
	    	count = count % this->children.size();
            if (fixedpos >= this->h_items)
                fixedpos = (this->h_items - 1) / 2;

	    	if (count > (this->children.size() / 2) - (fixedpos - ((this->h_items - 1) / 2))) {
	    		count = this->children.size()-count;
	    		while (count--)
	    			scrollRightEx(1, refresh, test);
	    		return true;
			}

    		while (count--)
    			scrollLeftEx(1, refresh, test);
    		return true;
	    }
	}

	return scrollLeftEx(count, refresh, test);
}

bool MMSMenu::scrollTo(int posx, int posy, bool refresh) {
	for (unsigned int i = 0; i < this->children.size(); i++) {
		if (!this->children.at(i)->isVisible())
			continue;
		DFBRectangle mygeom = this->children.at(i)->getGeometry();
		if   ((posx >= mygeom.x)&&(posy >= mygeom.y)
			&&(posx < mygeom.x + mygeom.w)&&(posy < mygeom.y + mygeom.h)) {
			/* that's the right menu item, scroll smooth to the position */
			setSelected(i, refresh);
			return true;
		}
	}
	return false;
}

void MMSMenu::setItemTemplate(MMSWidget *itemTemplate) {
	bool b;
	
	if (!itemTemplate)
        throw new MMSWidgetError(0, "item template not set");

    /* we need menu items which can be selected */
    if (!itemTemplate->getSelectable(b))
        throw new MMSWidgetError(0, "widget cannot be selected");
    if (!b)
    	throw new MMSWidgetError(0, "widget cannot be selected");

    /* we need menu items which can be selected and we must switch focusable off */
    if (itemTemplate->getFocusable(b))
    	if (b)
    		itemTemplate->setFocusable(false, false);
    itemTemplate->unsetFocusableForAllChildren(false);
    itemTemplate->setVisible(false, false);

    /* item template can be set once only */
    if (this->itemTemplate)
        throw new MMSWidgetError(0, "item template can be set once only");

    this->itemTemplate = itemTemplate;
}

MMSWidget *MMSMenu::getItemTemplate() {
    return this->itemTemplate;
}

MMSWidget *MMSMenu::newItem() {
    MMSWidget *widget;

    if (!this->itemTemplate)
        throw new MMSWidgetError(0, "item template not set");

    widget = itemTemplate->copyWidget();

    widget->setParent(this);
    widget->setRootWindow(this->rootwindow);
    this->children.push_back(widget);


    recalculateChildren();

    this->refresh();


//    if (isFocused())
//        if (this->children.size()==1)
            /* first item inserted */
//            this->children.at(0)->setSelected(true, false);

  //              setSelected(0);

    return widget;
}

void MMSMenu::clear() {
    for(int i = (int)this->children.size() - 1; i >= 0 ; i--) {
        delete this->children.at(i);
        this->children.erase(this->children.end()-1);
    }

    this->x = 0;
    this->y = 0;
    this->px = 0;
    this->py = 0;
    this->firstFocus = false;
    this->firstSelection = false;

    recalculateChildren();

    this->refresh();
}

void MMSMenu::setFocus(bool set, bool refresh) {

	/* switch the brightness of the menu items */
    if (set) {
        /* get the focus -> dim up and/or decrease transparency */
        if ((!MMSWidget::isFocused())&&(this->firstFocus)) {
            for (unsigned int i = 0; i < children.size(); i++) {
                children.at(i)->setBrightness(children.at(i)->getBrightness() + getDimItems(), false);
                children.at(i)->setOpacity(children.at(i)->getOpacity() + getTransItems(), false);
            }
        }
    }
    else {
        /* loose the focus -> dim down and/or increase transparency */
        if ((MMSWidget::isFocused())||(!this->firstFocus)) {
            for (unsigned int i = 0; i < children.size(); i++) {
                children.at(i)->setBrightness(children.at(i)->getBrightness() - getDimItems(), false);
                children.at(i)->setOpacity(children.at(i)->getOpacity() - getTransItems(), false);
            }
        }
    }
    this->firstFocus = true;

    /* set the focus */
    if (!this->firstSelection) {
        if (!children.empty()) {
            MMSWidget::setFocus(set, false);
            selectItem(children.at(0), true, refresh);
        }
        else
            MMSWidget::setFocus(set, refresh);
        this->firstSelection = true;
    }
    else {
        MMSWidget::setFocus(set, refresh);
    }
}


void MMSMenu::setSelected(unsigned int item, bool refresh) {
    if (!getConfig())
        return;

    if (item >= children.size())
        return;

    if (!this->firstSelection) {
        if (item == 0)
            if (!children.empty())
                selectItem(children.at(0), refresh);
        this->firstSelection = true;
    }

    unsigned int cols = getCols();
    unsigned int mx = item % cols;
    unsigned int my = item / cols;

    
    /* scroll left-down */
    if ((mx < this->x)&&(my > this->y)) {
        if (scrollLeft(this->x - mx, false))
       		scrollDown(my - this->y, refresh);
    }
    else
    /* scroll right-down */
    if ((mx > this->x)&&(my > this->y)) {
        if (scrollRight(mx - this->x, false))
      		scrollDown(my - this->y, refresh);
    }
    else
    /* scroll left-up */
    if ((mx < this->x)&&(my < this->y)) {
        if (scrollUp(this->y - my, false))
       		scrollLeft(this->x - mx, refresh);
    }
    else
    /* scroll right-up */
    if ((mx > this->x)&&(my < this->y)) {
        if (scrollUp(this->y - my, false))
       		scrollRight(mx - this->x, refresh);
    }
    else
    /* scroll down */
    if (my > this->y) {
   		scrollDown(my - this->y, refresh);
    }
    else
    /* scroll up */
    if (my < this->y) {
   		scrollUp(this->y - my, refresh);
    }
    else
    /* scroll left */
    if (mx < this->x) {
   		scrollLeft(this->x - mx, refresh);
    }
    else
    /* scroll right */
    if (mx > this->x) {
   		scrollRight(mx - this->x, refresh);
    }
}

unsigned int MMSMenu::getSelected() {
    return (this->x + this->y * getCols());
}

MMSWidget *MMSMenu::getItem(unsigned int item) {
    if (item < this->children.size())
        return this->children.at(item);
    return NULL;
}

MMSWidget *MMSMenu::getSelectedItem() {
    return getItem(getSelected());
}

unsigned int MMSMenu::getSize() {
    return this->children.size();
}

unsigned int MMSMenu::getVItems() {
    return this->v_items;
}

unsigned int MMSMenu::getHItems() {
    return this->h_items;
}



bool MMSMenu::init() {
    /* init widget basics */
    if (!MMSWidget::init())
        return false;

    return true;
}

bool MMSMenu::draw(bool *backgroundFilled) {
    bool myBackgroundFilled = false;

    if (backgroundFilled) {
    	if (this->has_own_surface)
    		*backgroundFilled = false;
    }
    else
        backgroundFilled = &myBackgroundFilled;

    /* lock */
    this->surface->lock();

    /* draw widget basics */
    if (MMSWidget::draw(backgroundFilled)) {
        /* update window surface with an area of surface */
        updateWindowSurfaceWithSurface(!*backgroundFilled);
    }

    /* unlock */
    this->surface->unlock();

    /* draw widgets debug frame */
    return MMSWidget::drawDebug();
}


/***********************************************/
/* begin of theme access methods (get methods) */
/***********************************************/

#define GETMENU(x) \
    if (this->myMenuClass.is##x()) return myMenuClass.get##x(); \
    else if ((menuClass)&&(menuClass->is##x())) return menuClass->get##x(); \
    else return this->theme->menuClass.get##x();

xmlNode *MMSMenu::getXMLNode() {
    xmlNode *node;
    if ((node=myMenuClass.getXMLNode()))
        return node;
    if ((menuClass)&&((node=menuClass->getXMLNode())))
        return node;
    return this->theme->menuClass.getXMLNode();
}

string MMSMenu::getItemWidth() {
    GETMENU(ItemWidth);
}

string MMSMenu::getItemHeight() {
    GETMENU(ItemHeight);
}

unsigned int MMSMenu::getItemHMargin() {
    GETMENU(ItemHMargin);
}

unsigned int MMSMenu::getItemVMargin() {
    GETMENU(ItemVMargin);
}

unsigned int MMSMenu::getCols() {
    GETMENU(Cols);
}

unsigned int MMSMenu::getDimItems() {
    GETMENU(DimItems);
}

int MMSMenu::getFixedPos() {
    GETMENU(FixedPos);
}

bool MMSMenu::getHLoop() {
    GETMENU(HLoop);
}

bool MMSMenu::getVLoop() {
    GETMENU(VLoop);
}

unsigned int MMSMenu::getTransItems() {
    GETMENU(TransItems);
}

unsigned int MMSMenu::getDimTop() {
    GETMENU(DimTop);
}

unsigned int MMSMenu::getDimBottom() {
    GETMENU(DimBottom);
}

unsigned int MMSMenu::getDimLeft() {
    GETMENU(DimLeft);
}

unsigned int MMSMenu::getDimRight() {
    GETMENU(DimRight);
}

unsigned int MMSMenu::getTransTop() {
    GETMENU(TransTop);
}

unsigned int MMSMenu::getTransBottom() {
    GETMENU(TransBottom);
}

unsigned int MMSMenu::getTransLeft() {
    GETMENU(TransLeft);
}

unsigned int MMSMenu::getTransRight() {
    GETMENU(TransRight);
}

string MMSMenu::getZoomSelWidth() {
    GETMENU(ZoomSelWidth);
}

string MMSMenu::getZoomSelHeight() {
    GETMENU(ZoomSelHeight);
}

string MMSMenu::getZoomSelShiftX() {
    GETMENU(ZoomSelShiftX);
}

string MMSMenu::getZoomSelShiftY() {
    GETMENU(ZoomSelShiftY);
}

bool MMSMenu::getSmoothScrolling() {
    GETMENU(SmoothScrolling);
}

/***********************************************/
/* begin of theme access methods (set methods) */
/***********************************************/

void MMSMenu::setItemWidth(string itemwidth, bool refresh) {
    myMenuClass.setItemWidth(itemwidth);
    if (refresh)
        this->refresh();
}

void MMSMenu::setItemHeight(string itemheight, bool refresh) {
    myMenuClass.setItemHeight(itemheight);
    if (refresh)
        this->refresh();
}

void MMSMenu::setItemHMargin(unsigned int itemhmargin, bool refresh) {
    myMenuClass.setItemHMargin(itemhmargin);
    if (refresh)
        this->refresh();
}

void MMSMenu::setItemVMargin(unsigned int itemvmargin, bool refresh) {
    myMenuClass.setItemVMargin(itemvmargin);
    if (refresh)
        this->refresh();
}

void MMSMenu::setCols(unsigned int cols, bool refresh) {
    myMenuClass.setCols(cols);
    if (refresh)
        this->refresh();
}

void MMSMenu::setDimItems(unsigned int dimitems, bool refresh) {
    myMenuClass.setDimItems(dimitems);
    if (refresh)
        this->refresh();
}

void MMSMenu::setFixedPos(int fixedpos, bool refresh) {
    myMenuClass.setFixedPos(fixedpos);
    if (refresh)
        this->refresh();
}

void MMSMenu::setHLoop(bool hloop) {
    myMenuClass.setHLoop(hloop);
}

void MMSMenu::setVLoop(bool vloop) {
    myMenuClass.setVLoop(vloop);
}

void MMSMenu::setTransItems(unsigned int transitems, bool refresh) {
    myMenuClass.setTransItems(transitems);
    if (refresh)
        this->refresh();
}

void MMSMenu::setDimTop(unsigned int dimtop, bool refresh) {
    myMenuClass.setDimTop(dimtop);
    if (refresh)
        this->refresh();
}

void MMSMenu::setDimBottom(unsigned int dimbottom, bool refresh) {
    myMenuClass.setDimBottom(dimbottom);
    if (refresh)
        this->refresh();
}

void MMSMenu::setDimLeft(unsigned int dimleft, bool refresh) {
    myMenuClass.setDimLeft(dimleft);
    if (refresh)
        this->refresh();
}

void MMSMenu::setDimRight(unsigned int dimright, bool refresh) {
    myMenuClass.setDimRight(dimright);
    if (refresh)
        this->refresh();
}

void MMSMenu::setTransTop(unsigned int transtop, bool refresh) {
    myMenuClass.setTransTop(transtop);
    if (refresh)
        this->refresh();
}

void MMSMenu::setTransBottom(unsigned int transbottom, bool refresh) {
    myMenuClass.setTransBottom(transbottom);
    if (refresh)
        this->refresh();
}

void MMSMenu::setTransLeft(unsigned int transleft, bool refresh) {
    myMenuClass.setTransLeft(transleft);
    if (refresh)
        this->refresh();
}

void MMSMenu::setTransRight(unsigned int transright, bool refresh) {
    myMenuClass.setTransRight(transright);
    if (refresh)
        this->refresh();
}

void MMSMenu::setZoomSelWidth(string zoomselwidth, bool refresh) {
    myMenuClass.setZoomSelWidth(zoomselwidth);
    if (refresh)
        this->refresh();
}

void MMSMenu::setZoomSelHeight(string zoomselheight, bool refresh) {
    myMenuClass.setZoomSelHeight(zoomselheight);
    if (refresh)
        this->refresh();
}

void MMSMenu::setZoomSelShiftX(string zoomselshiftx, bool refresh) {
    myMenuClass.setZoomSelShiftX(zoomselshiftx);
    if (refresh)
        this->refresh();
}

void MMSMenu::setZoomSelShiftY(string zoomselshifty, bool refresh) {
    myMenuClass.setZoomSelShiftY(zoomselshifty);
    if (refresh)
        this->refresh();
}

void MMSMenu::setSmoothScrolling(bool smoothscrolling) {
    myMenuClass.setSmoothScrolling(smoothscrolling);
    this->smooth_scrolling = smoothscrolling;
}

void MMSMenu::updateFromThemeClass(MMSMenuClass *themeClass) {
   if (themeClass->isItemWidth())
        setItemWidth(themeClass->getItemWidth());
   if (themeClass->isItemHeight())
        setItemHeight(themeClass->getItemHeight());
   if (themeClass->isItemHMargin())
        setItemHMargin(themeClass->getItemHMargin());
   if (themeClass->isItemVMargin())
        setItemVMargin(themeClass->getItemVMargin());
   if (themeClass->isCols())
        setCols(themeClass->getCols());
   if (themeClass->isDimItems())
        setDimItems(themeClass->getDimItems());
   if (themeClass->isFixedPos())
        setFixedPos(themeClass->getFixedPos());
   if (themeClass->isHLoop())
        setHLoop(themeClass->getHLoop());
   if (themeClass->isVLoop())
        setVLoop(themeClass->getVLoop());
   if (themeClass->isTransItems())
        setTransItems(themeClass->getTransItems());
   if (themeClass->isDimTop())
        setDimTop(themeClass->getDimTop());
   if (themeClass->isDimBottom())
        setDimBottom(themeClass->getDimBottom());
   if (themeClass->isDimLeft())
        setDimLeft(themeClass->getDimLeft());
   if (themeClass->isDimRight())
        setDimRight(themeClass->getDimRight());
   if (themeClass->isTransTop())
        setTransTop(themeClass->getTransTop());
   if (themeClass->isTransBottom())
        setTransBottom(themeClass->getTransBottom());
   if (themeClass->isTransLeft())
        setTransLeft(themeClass->getTransLeft());
   if (themeClass->isTransRight())
        setTransRight(themeClass->getTransRight());
   if (themeClass->isZoomSelWidth())
        setZoomSelWidth(themeClass->getZoomSelWidth());
   if (themeClass->isZoomSelHeight())
        setZoomSelHeight(themeClass->getZoomSelHeight());
   if (themeClass->isZoomSelShiftX())
        setZoomSelShiftX(themeClass->getZoomSelShiftX());
   if (themeClass->isZoomSelShiftY())
        setZoomSelShiftY(themeClass->getZoomSelShiftY());
   if (themeClass->isSmoothScrolling())
        setSmoothScrolling(themeClass->getSmoothScrolling());

    MMSWidget::updateFromThemeClass(&(themeClass->widgetClass));
}

/***********************************************/
/* end of theme access methods                 */
/***********************************************/

