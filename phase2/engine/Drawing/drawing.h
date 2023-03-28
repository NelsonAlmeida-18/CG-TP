void tokenize(std::string const &str, const char* delim, std::vector<float> &out);
int drawFigure(std::string filename, GLuint *buffers, int buffer);
std::vector<int> drawModels(Scene scene, GLuint *buffers);
void drawAxis();