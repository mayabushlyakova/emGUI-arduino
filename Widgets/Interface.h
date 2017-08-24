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

#ifndef __INTERFACE_H
#define __INTERFACE_H


#include "Widget.h"
#include "Window.h"
#include <stdint.h>
#include "StatusBar.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef LCD_SizeX
#define LCD_SizeX 240
#endif

#ifndef LCD_SizeY
#define LCD_SizeY 240
#endif

#ifndef LCD_STATUS_BAR_HEIGHT
#define LCD_STATUS_BAR_HEIGHT 26
#endif

	typedef xWidget xInterface;

	xInterface * pxInterfaceCreate(bool(*pxOnCreateHandler)(xWidget *));
	void vInterfaceDraw();
	xInterface *pxInterfaceGet();
	uint16_t inline usInterfaceGetW() {
		return LCD_SizeX;
	}
	uint16_t inline usInterfaceGetH() {
		return LCD_SizeY;
	}
	uint16_t inline usInterfaceGetWindowH() {
		return LCD_SizeY - LCD_STATUS_BAR_HEIGHT;
	}
	uint16_t inline usInterfaceGetWindowW() {
		return LCD_SizeX;
	}
	uint16_t inline usInterfaceGetWindowX() {
		return 0;
	}
	uint16_t inline usInterfaceGetWindowY() {
		return LCD_STATUS_BAR_HEIGHT;
	}
	void vInterfaceInvalidate();
	void bInterfaceCheckTouchScreenEvent();
	void vInterfaceDebug(bool bDebug);
	bool bInterfaceGetDebug();
	void vInterfaceOpenWindow(eWindow eWnd);
	void vInterfaceCloseActiveWindow();
	void vInterfaceCloseWindow(eWindow eWnd);
	void vInterfaceUpdateWindow();
	xWindow * pxInterfaceIsWindowActive(eWindow eWnd);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif	//__INTERFACE_H
