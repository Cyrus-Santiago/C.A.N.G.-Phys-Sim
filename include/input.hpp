class Input {
    public:
        static void mouseClickCallback(GLFWwindow * window, int button, int action, int mods);
        static void mousePosCallback(GLFWwindow * window, double xPos, double yPos);
        static void processInput(GLFWwindow *window);
};