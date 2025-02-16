#include <corecrt_math_defines.h>

#include "Mandelbrot.h"

Mandelbrot::Mandelbrot() : re_start{ -2.0 }, re_end{ 0.5 }, im_start{ -1.3 }, im_end{ 1.3 }, re_diff{ abs(re_start - re_end) }, im_diff{ abs(im_start - im_end) }, pool{}, colors{}, update{ true }, julia{ false }, julia_re{ 0 }, julia_im{ 0 }, julia_re_start{ -2.0 }, julia_re_end{ 2.0 }, julia_im_start{ -2.0 }, julia_im_end{ 2.0 }, julia_re_diff{ abs(julia_re_start - julia_re_end) }, julia_im_diff{ abs(julia_im_start - julia_im_end) }
{
	sAppName = "Mandelbrot -> iterations max = " + std::to_string(MAXITERATIONS);
	colors.clear();
	majColors();
}

bool Mandelbrot::OnUserCreate()
{
	escapeTime();
	return true;
}

bool Mandelbrot::OnUserUpdate(float fElapsedTime)
{
	update = false;
	if (GetMouseWheel() > 0)
	{
		// Mouse wheel scrolled up;
		zoom(fElapsedTime);
	}
	else if (GetMouseWheel() < 0)
	{
		// Mouse wheel scrolled down
		dezoom(fElapsedTime);
	}

	if (GetMouse(0).bHeld)
	{
		zoom(fElapsedTime);
	}
	if (GetMouse(1).bHeld)
	{
		dezoom(fElapsedTime);
	}
	if (GetMouse(2).bHeld)
	{
		julia_im = ((double)GetMouseY() / ScreenHeight()) * im_diff + im_start;
		julia_re = ((double)GetMouseX() / ScreenWidth()) * re_diff + re_start;
	}

	if (GetKey(olc::Key::LEFT).bHeld)
	{
		re_start -= re_diff * fElapsedTime;
		re_end -= re_diff * fElapsedTime;
		re_diff = abs(re_start - re_end);
		im_diff = abs(im_start - im_end);
		update = true;
	}
	if (GetKey(olc::Key::RIGHT).bHeld)
	{
		re_start += re_diff * fElapsedTime;
		re_end += re_diff * fElapsedTime;
		re_diff = abs(re_start - re_end);
		im_diff = abs(im_start - im_end);
		update = true;
	}
	if (GetKey(olc::Key::UP).bHeld)
	{
		im_start -= im_diff * fElapsedTime;
		im_end -= im_diff * fElapsedTime;
		re_diff = abs(re_start - re_end);
		im_diff = abs(im_start - im_end);
		update = true;
	}
	if (GetKey(olc::Key::DOWN).bHeld)
	{
		im_start += im_diff * fElapsedTime;
		im_end += im_diff * fElapsedTime;
		re_diff = abs(re_start - re_end);
		im_diff = abs(im_start - im_end);
		update = true;
	}
	if (GetKey(olc::Key::R).bPressed)
	{
		MAXITERATIONS = 1000;
		majColors();
		sAppName = "Mandelbrot -> iterations max = " + std::to_string(MAXITERATIONS);
		re_start = -2.0;
		re_end = 0.5;
		im_start = -1.3;
		im_end = 1.3;
		re_diff = abs(re_start - re_end);
		im_diff = abs(im_start - im_end);
		update = true;
		julia = false;
	}
	if (GetKey(olc::Key::NP_ADD).bPressed)
	{
		MAXITERATIONS = std::min(INT_MAX - 100, MAXITERATIONS + 100);
		majColors();
		sAppName = "Mandelbrot -> iterations max = " + std::to_string(MAXITERATIONS);
		update = true;
	}
	if (GetKey(olc::Key::NP_SUB).bPressed)
	{
		MAXITERATIONS = std::max(100, MAXITERATIONS - 100);
		majColors();
		sAppName = "Mandelbrot -> iterations max = " + std::to_string(MAXITERATIONS);
		update = true;
	}
	if (GetKey(olc::Key::J).bHeld)
	{
		julia = true;
		julia_im = ((double)GetMouseY() / ScreenHeight()) * im_diff + im_start;
		julia_re = ((double)GetMouseX() / ScreenWidth()) * re_diff + re_start;
	}
	if (GetKey(olc::Key::H).bPressed)
	{
		julia = false;
		update = true;
	}
	if (GetKey(olc::Key::ESCAPE).bPressed)
	{
		return false;
	}

	if (update)
	{
		escapeTime();
	}
	if (julia)
	{
		drawJulia();
	}

	return true;
}

void Mandelbrot::zoom(float fElapsedTime)
{
	//zoom moins beau mais plus rapide
	/*double x = ((double)GetMouseX() / ScreenWidth()) * re_diff + re_start;
	double y = ((double)GetMouseY() / ScreenHeight()) * im_diff + im_start;
	double zoomFactor = 1.0 + (fElapsedTime / (fElapsedTime + 1.0)) / 1.0;
	re_start = x - (re_diff / 4) * zoomFactor;
	re_end = x + (re_diff / 4) * zoomFactor;
	im_start = y - (im_diff / 4) * zoomFactor;
	im_end = y + (im_diff / 4) * zoomFactor;
	re_diff = abs(re_start - re_end);
	im_diff = abs(im_start - im_end);*/
	//zoom plus beau mais plus lent
	double x = ((double)GetMouseX() / ScreenWidth()) * re_diff + re_start;
	double y = ((double)GetMouseY() / ScreenHeight()) * im_diff + im_start;
	double zoomFactor = exp(fElapsedTime * log(0.6));
	double re_center = (re_start + re_end) / 2;
	double im_center = (im_start + im_end) / 2;
	double re_diff_new = abs(re_start - re_end) * zoomFactor;
	double im_diff_new = abs(im_start - im_end) * zoomFactor;
	double x_diff = x - re_center;
	double y_diff = y - im_center;
	re_start = x - (x_diff * zoomFactor) - (re_diff_new / 2);
	re_end = x - (x_diff * zoomFactor) + (re_diff_new / 2);
	im_start = y - (y_diff * zoomFactor) - (im_diff_new / 2);
	im_end = y - (y_diff * zoomFactor) + (im_diff_new / 2);
	re_diff = abs(re_start - re_end);
	im_diff = abs(im_start - im_end);

	update = true;
}

void Mandelbrot::dezoom(float fElapsedTime)
{
	//zoom moins beau mais plus rapide
	/*double x = ((double)GetMouseX() / ScreenWidth()) * re_diff + re_start;
	double y = ((double)GetMouseY() / ScreenHeight()) * im_diff + im_start;
	double zoomFactor = 1.0 + (fElapsedTime / (fElapsedTime + 1.0)) / 1.0;
	re_start = x - re_diff * zoomFactor;
	re_end = x + re_diff * zoomFactor;
	im_start = y - im_diff * zoomFactor;
	im_end = y + im_diff * zoomFactor;
	re_diff = abs(re_start - re_end);
	im_diff = abs(im_start - im_end);*/
	//zoom plus beau mais plus lent
	double x = ((double)GetMouseX() / ScreenWidth()) * re_diff + re_start;
	double y = ((double)GetMouseY() / ScreenHeight()) * im_diff + im_start;
	double zoomFactor = exp(fElapsedTime * log(1.6));
	double re_center = (re_start + re_end) / 2;
	double im_center = (im_start + im_end) / 2;
	double re_diff_new = abs(re_start - re_end) * zoomFactor;
	double im_diff_new = abs(im_start - im_end) * zoomFactor;
	double x_diff = x - re_center;
	double y_diff = y - im_center;
	re_start = x - (x_diff * zoomFactor) - (re_diff_new / 2);
	re_end = x - (x_diff * zoomFactor) + (re_diff_new / 2);
	im_start = y - (y_diff * zoomFactor) - (im_diff_new / 2);
	im_end = y - (y_diff * zoomFactor) + (im_diff_new / 2);
	re_diff = abs(re_start - re_end);
	im_diff = abs(im_start - im_end);	

	update = true;
}

void Mandelbrot::escapeTime()
{
	int width = ScreenWidth();

	pool.push_loop(width, [&](const int i, const int iMax) {Mandelbrot::afficheEscapeTime(i, iMax); });
	pool.wait_for_tasks();
}

unsigned int Mandelbrot::escapeTime(double x0, double y0)
{
	double first = x0 - 0.25;
	first *= first;
	double p = sqrt(first + (y0 * y0));
	if ((x0 < p - 2 * (p * p) + 0.25) || (((x0 + 1) * (x0 + 1) + y0 * y0) < 0.0625))
	{
		return MAXITERATIONS;
	}

	double x = 0.0;
	double y = 0.0;
	unsigned int iterations = 0;
	double x2 = 0;
	double y2 = 0;
	double w = 0;

	while (x2 + y2 <= 4 && iterations < MAXITERATIONS)
	{
		x = x2 - y2 + x0;
		y = w - x2 - y2 + y0;
		x2 = x * x;
		y2 = y * y;
		w = (x + y) * (x + y);
		iterations++;
	}

	return iterations;
}

unsigned int Mandelbrot::escapeTimeJulia(double x0, double y0, double xc, double yc)
{
	int iterations = 0;
	while (x0 * x0 + y0 * y0 < 16 && MAXITERATIONS > iterations)
	{
		double xtemp = x0 * x0 - y0 * y0 + xc;
		y0 = 2 * x0 * y0 + yc;
		x0 = xtemp;
		iterations++;
	}
	return iterations;
}

void Mandelbrot::afficheEscapeTime(int i, int iMax)
{
	for (; i < iMax; i++)
	{
		for (int j = 0; j < ScreenHeight(); j++)
		{
			double x = ((double)i / ScreenWidth()) * re_diff + re_start;
			double y = ((double)j / ScreenHeight()) * im_diff + im_start;
			unsigned int iterations = escapeTime(x, y);

			Draw(i, j, colors[iterations]);
		}
	}
}

void Mandelbrot::majColors()
{
	if (MAXITERATIONS + 1 != colors.size())
	{
		colors.clear();
		colors.reserve(MAXITERATIONS + 1);
		//old colors
		for (int i = 0; i < MAXITERATIONS; i++)
		{
			//old hsv
			float hue = (float)((360.0 * 10 * i) / MAXITERATIONS);
			float saturation = 1;
			float value = 1;
			colors.emplace_back(hsv2rgb(hue, saturation, value));

			//wikipedia hsv
			/*int hue = (int)pow(i * 1.f / MAXITERATIONS * 360, 1.5) % 360;
			float saturation = 1;
			float value = i * 1.f / MAXITERATIONS * 100;
			colors.emplace_back(hsv2rgb(hue, saturation, value));*/

			//wikipedia lch
			/*float s = i / (float)MAXITERATIONS;
			float cosPi = cos(M_PI * s);
			float v = 1.0 - (cosPi * cosPi);
			float l = 75 - (75 * v);
			float c = 28 + (75 - (75 * v));
			float h = (int)pow(360 * s, 1.5) % 360;
			colors.emplace_back(lch2rgb(l, c, h));*/
		}
		
		//max iterations color is always black
		colors.emplace_back(olc::BLACK);
	}
}

void Mandelbrot::drawJulia()
{
	pool.push_loop(widthJulia, [&](int i, int iMax)
		{
			for (; i < iMax; i++)
			{
				for (int j = 0; j < heightJulia; j++)
				{
					double x = ((double)i / widthJulia) * julia_re_diff + julia_re_start;
					double y = ((double)j / heightJulia) * julia_im_diff + julia_im_start;
					double xc = julia_re;
					double yc = julia_im;
					unsigned int iterations = escapeTimeJulia(x, y, xc, yc);
					//black and white
					/*int color = iterations * 255 / MAXITERATIONS;

					Draw(i, j, olc::Pixel(color, color, color));*/
					Draw(i, j, iterations <= 2 ? olc::DARK_GREY : colors[iterations]);
				}
			}
		});
	pool.wait_for_tasks();
}

olc::Pixel hsv2rgb(float h, float s, float v)
{//h between 0 and 360, s and v between 0 and 1
	float r, g, b;
	int i;
	float f, p, q, t;

	if (s == 0)
	{
		r = g = b = v;
		return olc::Pixel(r, g, b);
	}

	h /= 60;
	i = floor(h);
	f = h - i;
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));

	switch (i)
	{
	case 0:
		r = v;
		g = t;
		b = p;
		break;

	case 1:
		r = q;
		g = v;
		b = p;
		break;

	case 2:
		r = p;
		g = v;
		b = t;
		break;

	case 3:
		r = p;
		g = q;
		b = v;
		break;

	case 4:
		r = t;
		g = p;
		b = v;
		break;

	default:
		r = v;
		g = p;
		b = q;
		break;
	}

	return olc::Pixel(r * 255, g * 255, b * 255);
}

olc::Pixel lch2rgb(float l, float c, float h)
{//l between 0 and 100, c between 0 and 128, h between 0 and 360
	float u = c * cos(h);
	float v = c * sin(h);

	float x = u / 0.607;
	float y = v / 0.707;
	float z = l;

	float r = x * 3.2406 + y * -1.5372 + z * -0.4986;
	float g = x * -0.9689 + y * 1.8758 + z * 0.0415;
	float b = x * 0.0557 + y * -0.2040 + z * 1.0570;

	if (r > 0.0031308)
	{
		r = 1.055 * pow(r, 1 / 2.4) - 0.055;
	}
	else
	{
		r = 12.92 * r;
	}

	if (g > 0.0031308)
	{
		g = 1.055 * pow(g, 1 / 2.4) - 0.055;
	}
	else
	{
		g = 12.92 * g;
	}

	if (b > 0.0031308)
	{
		b = 1.055 * pow(b, 1 / 2.4) - 0.055;
	}
	else
	{
		b = 12.92 * b;
	}

	return olc::Pixel(r * 255, g * 255, b * 255);
}