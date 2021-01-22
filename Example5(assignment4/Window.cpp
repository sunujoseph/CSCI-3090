// Marc Tremblay
// 100555250
// 01/26/2017

#include "Window.h"

void window_resize(GLFWwindow *a_window, int a_width, int a_height);

Window::Window(const char *a_title, int a_width, int a_height)
{
	m_title = a_title;
	m_width = a_width;
	m_height = a_height;
	if (!Init()) glfwTerminate();

	for (int i = 0; i < MAX_KEYS; i++) m_keys[i] = false;
	for (int i = 0; i < MAX_BUTTONS; i++) m_mouseButtons[i] = false;

}
Window::~Window()
{
	glfwTerminate();
}

bool Window::Init()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW!" << std::endl;
		return false;
	}

	m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
	if (!m_window)
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		return false;
	}
	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, this);

	//set callbacks
	glfwSetWindowSizeCallback(m_window, window_resize);
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	glfwSetCursorPosCallback(m_window, cursor_position_callback);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Could not initialize GLEW!" << std::endl;
		return false;
	}

	glClearColor(0.2f, 0.3f, 0.5f, 1.0f);

	std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
	return true;
}

bool Window::GetKey(unsigned int a_keycode) const
{
	if (a_keycode >= MAX_KEYS) return false;
	return m_keys[a_keycode];
}

bool Window::GetMouseButton(unsigned int a_button) const
{
	if (a_button >= MAX_BUTTONS) return false;
	return m_mouseButtons[a_button];
}

void Window::GetMousePosition(double &a_xPos, double &a_yPos) const
{
	a_xPos = m_mouseX;
	a_yPos = m_mouseY;
}

void Window::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Update()
{
	glfwPollEvents();
	glfwSwapBuffers(m_window);
}

bool Window::Closed() const
{
	return glfwWindowShouldClose(m_window) == 1;
}

void window_resize(GLFWwindow *a_window, int a_width, int a_height)
{
	glViewport(0, 0, a_width, a_height);
}

void key_callback(GLFWwindow *a_window, int a_key, int a_scancode, int a_action, int a_mods)
{
	Window *win = (Window*)glfwGetWindowUserPointer(a_window);
	win->m_keys[a_key] = a_action != GLFW_RELEASE;
}
void mouse_button_callback(GLFWwindow *a_window, int a_button, int a_action, int a_mods)
{
	Window *win = (Window*)glfwGetWindowUserPointer(a_window);
	win->m_mouseButtons[a_button] = a_action != GLFW_RELEASE;
}

void cursor_position_callback(GLFWwindow *a_window, double a_xPos, double a_yPos)
{
	Window *win = (Window*)glfwGetWindowUserPointer(a_window);
	win->m_mouseX = a_xPos;
	win->m_mouseY = a_yPos;
}