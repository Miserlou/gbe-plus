// GB Enhanced+ Copyright Daniel Baxter 2015
// Licensed under the GPLv2
// See LICENSE.txt for full license text

// File : opengl.cpp
// Date : May 29, 2015
// Description : Handles OpenGL functionality
//
// Sets up OpenGL for use in GBE+
// Handles blit operations to the screen

#include "lcd.h"

/****** Initialize OpenGL through SDL ******/
void DMG_LCD::opengl_init()
{
	SDL_SetVideoMode((160 * config::scaling_factor), (144 * config::scaling_factor), 32, SDL_OPENGL | config::flags);
		
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0, 0, 0, 0);

	glViewport(0, 0, (160 * config::scaling_factor), (144 * config::scaling_factor));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, (160 * config::scaling_factor), (144 * config::scaling_factor), 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glGenTextures(1, &lcd_texture);
	glBindTexture(GL_TEXTURE_2D, lcd_texture);

	final_screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 160, 144, 32, 0, 0, 0, 0);
}

/****** Blit using OpenGL ******/
void DMG_LCD::opengl_blit()
{
	glTexImage2D(GL_TEXTURE_2D, 0, 4, 160, 144, 0, GL_BGRA, GL_UNSIGNED_BYTE, final_screen->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width = (80 * config::scaling_factor);
	int height = (72 * config::scaling_factor);

	glTranslatef(width, height, 0);

	glBindTexture(GL_TEXTURE_2D, lcd_texture);
	glBegin(GL_QUADS);

	//Top Left
	glTexCoord2i(0, 1);
	glVertex2i(-width, height);

	//Top Right
	glTexCoord2i(1, 1);
	glVertex2i(width, height);

	//Bottom Right
	glTexCoord2i(1, 0);
	glVertex2i(width, -height);

	//Bottom Left
	glTexCoord2f(0, 0);
	glVertex2i(-width, -height);
	glEnd();

	glLoadIdentity();

	SDL_GL_SwapBuffers();
}