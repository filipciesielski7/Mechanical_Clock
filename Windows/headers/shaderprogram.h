/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOśCI HANDLOWEJ albo PRZYDATNOśCI DO OKREśLONYCH
ZASTOSOWAń.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/


#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H


#include <GL/glew.h>
#include "stdio.h"


class ShaderProgram {
private:
	GLuint shaderProgram;
	GLuint vertexShader;
	GLuint geometryShader;
	GLuint fragmentShader;
	char* readFile(const char* fileName);
	GLuint loadShader(GLenum shaderType,const char* fileName);
public:
	ShaderProgram(const char* vertexShaderFile,const char* geometryShaderFile,const char* fragmentShaderFile);
	~ShaderProgram();
	void use();
	GLuint u(const char* variableName);
	GLuint a(const char* variableName);
};

extern ShaderProgram* spNew;

void initShaders();
void freeShaders();


#endif
