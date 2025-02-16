#pragma once

#include <vector>

#include "olcPixelGameEngine.h"
#include "BS_thread_pool.hpp"

olc::Pixel hsv2rgb(float h, float s, float v);
olc::Pixel lch2rgb(float l, float c, float h);

class Mandelbrot : public olc::PixelGameEngine
{
public :
	Mandelbrot();

private :
	void zoom(float fElapsedTime);
	void dezoom(float fElapsedTime);

	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

	void escapeTime();
	unsigned int escapeTime(double x0, double y0);
	unsigned int escapeTimeJulia(double x0, double y0, double xc, double yc);
	void afficheEscapeTime(int i, int iMax);
	void majColors();
	void drawJulia();

	double re_start, re_end, im_start, im_end, re_diff, im_diff;
	double julia_re_start, julia_re_end, julia_im_start, julia_im_end, julia_re_diff, julia_im_diff;
	int MAXITERATIONS = 1000;
	int widthJulia = 200, heightJulia = 200;
	BS::thread_pool pool;
	std::vector<olc::Pixel> colors;
	bool update;
	bool julia;
	double julia_re, julia_im;
};

