// Marc Tremblay
// 100555250
// 01/26/2017


#ifndef WINDOW_H
#define WINDOW_H

#pragma comment(lib, "GL/glew32.lib")
#pragma comment(lib, "GL/glfw3.lib")

#include "GL/glew.h"
#include "GL/glfw3.h"
#include <iostream>

#define MAX_KEYS 1024
#define MAX_BUTTONS 32

class Window
{
private:
	friend struct GLFWwindow;
	const char *m_title;
	int m_width, m_height;
	bool m_closed;
	GLFWwindow *m_window;

	bool m_keys[MAX_KEYS];
	bool m_mouseButtons[MAX_BUTTONS];
	double m_mouseX, m_mouseY;

public:
	Window(const char *a_title, int a_width, int a_height);
	~Window();

	bool Closed() const;
	void Update();
	void Clear() const;

	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }

	bool GetKey(unsigned int a_keycode) const;
	bool GetMouseButton(unsigned int a_button) const;
	void GetMousePosition(double &a_xPos, double &a_yPos) const;

private:
	bool Init();
	friend void key_callback(GLFWwindow *a_window, int a_key, int a_scancode, int a_action, int a_mods);
	friend void mouse_button_callback(GLFWwindow *a_window, int a_button, int a_action, int a_mods);
	friend void cursor_position_callback(GLFWwindow *a_window, double a_xPos, double a_yPos);
};

#endif // WINDOW_H
