/*
 * mmscanvaswidget.cpp
 *
 *  Created on: Oct 26, 2011
 *      Author: sxs
 */

#include "mmsgui/mmscanvaswidget.h"
#include <iostream>
#include <sstream>


MMSCanvasWidget::MMSCanvasWidget(MMSWindow *root, string className, MMSTheme *theme) : MMSWidget() {

	create(root,className, theme);
}

MMSCanvasWidget::~MMSCanvasWidget() {

}

MMSWidget *MMSCanvasWidget::copyWidget() {
	return NULL;
}

void MMSCanvasWidget::updateFromThemeClass(MMSCanvasWidgetClass *themeClass) {

	if(!themeClass)
		return;

	if( themeClass->isAttributes()) {
		this->setAttributes(themeClass->getAttributes());


	}

	MMSWidget::updateFromThemeClass(&(themeClass->widgetClass));
}

bool MMSCanvasWidget::create(MMSWindow *root, string className, MMSTheme *theme) {
	this->type = MMSWIDGETTYPE_CANVAS;
    this->className = className;

    // init attributes for drawable widgets
	this->da = new MMSWIDGET_DRAWABLE_ATTRIBUTES;
    if (theme) this->da->theme = theme; else this->da->theme = globalTheme;
    this->canvasWidgetClass = this->da->theme->getCanvasWidgetClass(className);
    this->da->baseWidgetClass = &(this->da->theme->canvasWidgetClass.widgetClass);
    if (this->canvasWidgetClass) this->da->widgetClass = &(this->canvasWidgetClass->widgetClass); else this->da->widgetClass = NULL;

    // clear
	this->current_fgset = false;


    // create widget base
	return MMSWidget::create(root, true, false, true, true, false, false, true);

}

bool MMSCanvasWidget::init() {
    // init widget basics
    if (!MMSWidget::init())
        return false;


    return initFunc();
}

bool MMSCanvasWidget::release() {
    // release widget basics
    if (!MMSWidget::release())
        return false;

    return releaseFunc();
}

bool MMSCanvasWidget::draw(bool *backgroundFilled) {


	if (MMSWidget::draw(backgroundFilled)) {
    	drawingFunc(this->surface, this->surfaceGeom, backgroundFilled);
    }

	/* draw widgets debug frame */
	return MMSWidget::drawDebug();
}


bool MMSCanvasWidget::enableRefresh(bool enable) {
	if (!MMSWidget::enableRefresh(enable)) return false;

	// mark foreground as not set
	this->current_fgset = false;

	return true;
}

bool MMSCanvasWidget::checkRefreshStatus() {
	if (MMSWidget::checkRefreshStatus()) return true;

	if (this->current_fgset) {
		// current foreground initialized
			return false;
	}

	// (re-)enable refreshing
	enableRefresh();

	return true;
}

void MMSCanvasWidget::handleInput(MMSInputEvent *inputevent) {
	if(!handleInputFunc(inputevent))
		throw MMSWidgetError(1,"input not handled");
}

void MMSCanvasWidget::setAttributes(string attr) {
	std::stringstream ss(attr);
	std::string item, item2;

	while(std::getline(ss,item, ';')) {
	std::stringstream ss2(item);
		std::string key = "";
		std::string val = "";
		while(std::getline(ss2,item2, ':')) {
				//trim result
				trim(item2);
			if(key.empty())
				key = item2;
			else
				val = item2;
		}
		if(!key.empty())
			attributemap.insert(std::make_pair(key,val));
	}
}

MMSFontManager *MMSCanvasWidget::getFontManager() {
	return this->rootwindow->fm;
}
