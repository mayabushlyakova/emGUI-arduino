/*
	emGUI Library V1.0.0 - Copyright (C) 2013 Roman Savrulin <romeo.deepmind@gmail.com>

	This file is part of the emGUI Library distribution.

	emGUI Library is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License (version 2) as published by the
	Roman Savrulin AND MODIFIED BY the emGUI Library exception.

	>>>NOTE<<< The modification to the GPL is included to allow you to
	distribute a combined work that includes emGUI Library without being obliged to
	provide the source code for proprietary components outside of the emGUI Library.
	emGUI Library is distributed in the hope that it will be useful, but
	WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
	or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
	more details. You should have received a copy of the GNU General Public
	License and the emGUI Library license exception along with emGUI Library; if not it
	can be obtained by writing to Roman Savrulin <romeo.deepmind@gmail.com>.

	Created on: 14.11.2012
*/

#include "emGUI/Widgets/Interface.h"
#include <stdio.h>
#include <malloc.h>
#include "emGUI/Draw/Draw.h"

typedef struct xWindowListItem_t {
	struct xWindowListItem_t *xPrev;
	xWindow *xWnd;
} xWindowListItem;

static xInterface *xInterfaceInstance = NULL;
static xStatusBar *xStatusBarInstance = NULL;
static xTouchEventType eTouchState = popTs;

static xWindowListItem *xActiveWindow = NULL;

static xWindowListItem* prvWindowListFind(xWindow *pxW) {
	xWindowListItem *xL = xActiveWindow;
	if (!pxW)
		return NULL;
	
	while (xL) {
		if (xL->xWnd == pxW)
			return xL;
		xL = xL->xPrev;
	}

	return xL;
}

static void prvWindowListAppend(xWindow *pxW) {
	xWindowListItem *xL = xActiveWindow;
	if (!pxW)
		return;

	xWindowListItem* xLNew = malloc(sizeof(xWindowListItem));

	if (!xLNew)
		return;

	xLNew->xPrev = xActiveWindow;
	xLNew->xWnd = pxW;

	xActiveWindow = xLNew;
	return;
}

static void prvWindowListDelete(xWindow *pxW) {
	xWindowListItem *xL = xActiveWindow;
	xWindowListItem *xLPrev = NULL;
	if (!pxW)
		return;

	while (xL) {
		if (xL->xWnd == pxW ) {
			if(!xLPrev)
				xActiveWindow = xL->xPrev;
			else {
				xLPrev->xPrev = xL->xPrev;
			}
			free(xL);
			break;
		}
		xLPrev = xL;
		xL = xL->xPrev;
	}

	return;
}

xInterface * pxInterfaceCreate(bool(*pxOnCreateHandler)(xWidget *)) {
	if (!xInterfaceInstance) {
		xInterfaceInstance = pxWidgetCreate(0, 0, EMGUI_LCD_WIDTH, EMGUI_LCD_HEIGHT, NULL, true);
		xStatusBarInstance = xStatusBarCreate(EMGUI_COLOR_STATUS_BAR_BG);
		if (pxOnCreateHandler)
			pxOnCreateHandler(xInterfaceInstance);
	}
	return xInterfaceInstance;
}

void vInterfaceDraw() {
	vWidgetDraw(xInterfaceInstance);
}

xInterface *pxInterfaceGet() {
	return xInterfaceInstance;
}

void vInterfaceInvalidate() {
	if( xInterfaceInstance )
		vWidgetInvalidate(xInterfaceInstance);
}

bool bInterfaceCheckTouchScreenEvent(xTouchEvent *pxTouchScreenEv) {
	return bWidgetCheckTouchScreenEvent(xInterfaceInstance, pxTouchScreenEv);
}

xWindow * pxInterfaceGetWindow(int eWnd) {

	xWidget *pxN = xInterfaceInstance->pxFirstChild;
	while (pxN) {
		if (iWindowGetID(pxN) == eWnd)
			return pxN;
		pxN = pxWidgetGetNextChild(pxN);
	}

	return NULL;
}

bool bInterfaceIsWindowActive(int eWnd) {
	if (!xActiveWindow)
		return false;
	return iWindowGetID(xActiveWindow->xWnd) == eWnd;
}

void vInterfaceOpenWindow(int eWnd) {
	xWindow * pxWnd;
	xWindowListItem * pxActivePrev;

	pxWnd = pxInterfaceGetWindow(eWnd);

	if (!pxWnd) //window is not created yet
		return;

	// If window is already opened, just update its info
	// TODO: is that really needed?
	if (bInterfaceIsWindowActive(eWnd)) {
		vWidgetShow(pxWnd);
		vInterfaceUpdateWindow();
		return;
	}

	// window is already opened, but is not active at the moment
	if (prvWindowListFind(pxWnd)) {
		// bring it on top of the list.

		prvWindowListDelete(pxWnd);
		prvWindowListAppend(pxWnd);

		if(xActiveWindow)
			vWidgetHide(xActiveWindow->xWnd);

		vWidgetShow(pxWnd);

		vInterfaceUpdateWindow();
		return;
	}

	// Window is closed, check if we can open it
	if (!bWindowOpen(pxWnd)) { // if we unable to open window,
		return;
	}

	if(xActiveWindow)
		vWidgetHide(xActiveWindow->xWnd);

	prvWindowListAppend(pxWnd);
	vInterfaceUpdateWindow();
}

void vInterfaceCloseActiveWindow() {
	if(xActiveWindow)
		vInterfaceCloseWindow(iWindowGetID(xActiveWindow->xWnd));
}

void vInterfaceCloseWindow(int eWnd) {
	xWindow * pxWnd;
	xWindowListItem * pxPrev;
	pxWnd = pxInterfaceGetWindow(eWnd);

	if (!pxWnd) //window is not created yet
		return;

	if (!bWindowClose(pxWnd))
		return;

	if(xActiveWindow && xActiveWindow->xPrev && xActiveWindow->xPrev->xWnd)
		vInterfaceOpenWindow(iWindowGetID(xActiveWindow->xPrev->xWnd));
	//else open blank

	prvWindowListDelete(pxWnd);
}

xStatusBar *pxInterfaceGetStatusBar() {
	return xStatusBarInstance;
}

void vInterfaceUpdateWindow() {

	if (!xActiveWindow || !xActiveWindow->xWnd)
		return;

	xStatusBar *pxSB = pxInterfaceGetStatusBar();

	xWindow *pxW = xActiveWindow->xWnd;

	if (bWindowGetFullScreen(pxW)) {
		vWidgetHide(pxSB);
	}
	else {
		vWidgetShow(pxSB);
	}
	vStatusBarSetWindowHeader(pxSB, pcWindowGetHeader(pxW));

}
