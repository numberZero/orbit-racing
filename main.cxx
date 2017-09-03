#include <ctime>
#include <cstdint>
#include <algorithm>
#include <deque>
#include <list>
#include <vector>
#include <GL/gl.h>
#include <GL/glut.h>
#include <SDL.h>
#include "text.hxx"
#include "vector.hxx"

typedef Vector<double, 2> v2;
typedef Vector<double, 3> v3;

SDL_GLContext context;
SDL_Window *window;
std::uint8_t const *keys;
std::uint32_t t_base;
double dt;

struct obj
{
	v2 pos, vel;
	v3 color;

	obj(v2 x, v2 v, v3 c)
		: pos(x), vel(v), color(c)
	{
	}

	std::deque<v2> trace;
	void step();
	void draw() const;
};

void obj::step()
{
	trace.push_back(pos);
	double r = pos.norm();
	v2 force = -pos / (r * r * r);
	vel += force * dt;
	pos += vel * dt;
}

void obj::draw() const
{
	glColor3dv(color);
	glVertex2dv(pos);
}

std::list<obj> os;

obj &add(v2 x, v2 v, v3 c)
{
	os.emplace_back(x, v, c);
	return os.back();
}

obj &o_st = add({0.0, 1.0}, {-1.0, 0.0}, {1.0, 1.0, 1.0});
obj &o_me = add({1.0, 0.0}, {0.0, 1.0}, {1.0, 0.0, 0.0});

void init()
{
}

void step()
{
	static double dd = 0.0;
	dd += dt;
	double jx = 0.0;
	double jy = 0.0;
	if (keys[SDL_SCANCODE_RIGHT])
		jx += 1.0;
	if (keys[SDL_SCANCODE_LEFT])
		jx -= 1.0;
	if (keys[SDL_SCANCODE_UP])
		jy += 1.0;
	if (keys[SDL_SCANCODE_DOWN])
		jy -= 1.0;
	v2 vx = o_me.pos.normed();
	v2 vy = ortho(vx);
	o_me.vel += 0.01 * (jx * vx + jy * vy) * dt;
	for (obj &o: os)
		o.step();
}

void draw()
{
	double h = o_me.pos.norm();
	v2 vertical = o_me.pos / h;
	double v_vertical = o_me.vel * vertical;
	double v_orbital = o_me.vel * ortho(vertical);
	double v_norm = std::sqrt(1.0 / h);
	double e_k = 0.5 * o_me.vel.squaredNorm();
	double e_p = -1.0 / h;

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glColor4d(.0, 1., .0, .5);
	vglBeginPrint(-400.0, 300.0, 20.0, 1.0);
	vglWriteLn("Position:");
	vglWriteLn(" - altitude: %.3f", h);
	vglWriteLn(" - angle: %.1f", sangle(o_st.pos, o_me.pos) * (180.0 / M_PI));
	vglWriteLn("Velocity:");
	vglWriteLn(" - orbital: %.3f (%+.1f%%)", v_orbital, 100.0 * (v_orbital / v_norm - 1.0));
	vglWriteLn(" - vertical: %+.3f", v_vertical);
	vglWriteLn("Energy:");
	vglWriteLn(" - kinetic: %.3f", e_k);
	vglWriteLn(" - potential: %.3f", e_p);
	vglWriteLn(" - total: %.3f", e_k + e_p);
	vglEndPrint();

	glScaled(150.0, 150.0, 150.0);
	for(obj const &o: os) {
		glColor4d(o.color[0], o.color[1], o.color[2], 0.1);
		glBegin(GL_LINE_STRIP);
		for (v2 const &v: o.trace)
			glVertex2dv(v);
		glEnd();
	}
	glColor4d(1.0, 1.0, 1.0, 0.1);
	glBegin(GL_POINTS);
	glColor4f(1.0, 1.0, 0.0, 1.0);
	glVertex2d(0.0, 0.0);
	for(obj const &o: os)
		o.draw();
	glEnd();
	glFlush();
	glFinish();
	SDL_GL_SwapWindow(window);
}

void resize()
{
	int window_width, window_height;
	SDL_GL_GetDrawableSize(window, &window_width, &window_height);
	glViewport(0, 0, window_width, window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double viewport_half_width = std::max(400.0, 300.0 * window_width / window_height);
	double viewport_half_height = std::max(300.0, 400.0 * window_height / window_width);
	glOrtho(-viewport_half_width, viewport_half_width, -viewport_half_height, viewport_half_height, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

bool events()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				return false;
			case SDL_KEYDOWN:
				if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					return false;
				break;
		}
	}
	return true;
}

void run()
{
	static std::uint32_t t0 = SDL_GetTicks();
	while(events()) {
		std::uint32_t t1 = SDL_GetTicks();
		dt = std::min(0.050, 1e-3 * (t1 - t0));
		t0 = t1;
		resize();
		step();
		draw();
	}
}

void initSDL()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	keys = SDL_GetKeyboardState(nullptr);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	window = SDL_CreateWindow(
		"Orbital Race",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		800,
		600,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED
		);
	context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, context);
	SDL_GL_SetSwapInterval(-1);
	t_base = SDL_GetTicks();
}

void initGL()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH);
	
	glEnable(GL_MULTISAMPLE);
	glPointSize(5.0);
	glLineWidth(1.5);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-400, 400, -300, 300, -10, 100);
	glMatrixMode(GL_MODELVIEW);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	initSDL();
	initGL();
	init();
	run();
	SDL_Quit();
	return 0;
}
