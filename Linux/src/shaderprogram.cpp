/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#include "../headers/shaderprogram.h"

ShaderProgram* spNew;

void initShaders() {
	spNew = new ShaderProgram("./shaders/v_new.glsl", NULL, "./shaders/f_new.glsl");
}

void freeShaders() {
	delete spNew;
}

char* ShaderProgram::readFile(const char* fileName) {
	int filesize;
	FILE *plik;
	char* result;

	#pragma warning(suppress : 4996)
	plik=fopen(fileName,"rb");
	if (plik != NULL) {
		fseek(plik, 0, SEEK_END);
		filesize = ftell(plik);
		fseek(plik, 0, SEEK_SET);
		result = new char[filesize + 1];
		#pragma warning(suppress : 6386)
		int readsize=fread(result, 1, filesize, plik);
		result[filesize] = 0;
		fclose(plik);

		return result;
	}

	return NULL;
}

GLuint ShaderProgram::loadShader(GLenum shaderType,const char* fileName) {
	GLuint shader=glCreateShader(shaderType);
	const GLchar* shaderSource=readFile(fileName);

	glShaderSource(shader,1,&shaderSource,NULL);

	glCompileShader(shader);

	delete []shaderSource;

	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 1) {
		infoLog = new char[infologLength];
		glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		delete []infoLog;
	}

	return shader;
}

ShaderProgram::ShaderProgram(const char* vertexShaderFile,const char* geometryShaderFile,const char* fragmentShaderFile) {
	printf("Loading vertex shader...\n");
	vertexShader=loadShader(GL_VERTEX_SHADER,vertexShaderFile);

	if (geometryShaderFile!=NULL) {
		printf("Loading geometry shader...\n");
		geometryShader=loadShader(GL_GEOMETRY_SHADER,geometryShaderFile);
	} else {
		geometryShader=0;
	}

	printf("Loading fragment shader...\n");
	fragmentShader=loadShader(GL_FRAGMENT_SHADER,fragmentShaderFile);

	shaderProgram=glCreateProgram();

	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,fragmentShader);
	if (geometryShaderFile!=NULL) glAttachShader(shaderProgram,geometryShader);
	glLinkProgram(shaderProgram);

	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 1)
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog(shaderProgram, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		delete []infoLog;
	}

	printf("Shader program created \n");
}

ShaderProgram::~ShaderProgram() {
	glDetachShader(shaderProgram, vertexShader);
	if (geometryShader!=0) glDetachShader(shaderProgram, geometryShader);
	glDetachShader(shaderProgram, fragmentShader);

	glDeleteShader(vertexShader);
	if (geometryShader!=0) glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);

	glDeleteProgram(shaderProgram);
}

void ShaderProgram::use() {
	glUseProgram(shaderProgram);
}

GLuint ShaderProgram::u(const char* variableName) {
	return glGetUniformLocation(shaderProgram,variableName);
}

GLuint ShaderProgram::a(const char* variableName) {
	return glGetAttribLocation(shaderProgram,variableName);
}
