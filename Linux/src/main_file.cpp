/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ. W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <ctime>

#include <./assimp/include/assimp/Importer.hpp>
#include <./assimp/include/assimp/scene.h>
#include <./assimp/include/assimp/postprocess.h>

#include "./headers/constants.h"
#include "./headers/allmodels.h"
#include "./headers/lodepng.h"
#include "./headers/shaderprogram.h"
#include "./headers/myCube.h"

GLuint tex;
GLuint tex22;
GLuint tex33;
GLuint tex44;
GLuint tex55;
GLuint tex77;

int liczba_obiektow = 0;
std::vector<glm::vec4> verts[100];
std::vector<glm::vec4> norms[100];
std::vector<glm::vec2> texCoords[100];
std::vector<unsigned int> indices[100];

float aspect = 1;
float speed_x = 0;
float speed_y = 0;
float walk_speed = 0;
float open = 0;

glm::vec3 pos = glm::vec3(0, 2, -11);

glm::vec3 calcDir(float kat_x, float kat_y) {
	glm::vec4 dir = glm::vec4(0, 0, 1, 0);
	glm::mat4 M = glm::rotate(glm::mat4(1.0f), kat_y, glm::vec3(0, 1, 0));
	M = glm::rotate(M, kat_x, glm::vec3(1, 0, 0));
	dir = M * dir;
	return glm::vec3(dir);
}

void key_callback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mod
) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) speed_y = 1;
		if (key == GLFW_KEY_RIGHT) speed_y = -1;
		if (key == GLFW_KEY_PAGE_UP || key == GLFW_KEY_P) speed_x = -1;
		if (key == GLFW_KEY_PAGE_DOWN || key == GLFW_KEY_L) speed_x = 1;
		if (key == GLFW_KEY_UP) walk_speed = 2;
		if (key == GLFW_KEY_DOWN) walk_speed = -2;
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT) speed_y = 0;
		if (key == GLFW_KEY_RIGHT) speed_y = 0;
		if (key == GLFW_KEY_PAGE_UP || key == GLFW_KEY_P) speed_x = 0;
		if (key == GLFW_KEY_PAGE_DOWN || key == GLFW_KEY_L) speed_x = 0;
		if (key == GLFW_KEY_UP) walk_speed = 0;
		if (key == GLFW_KEY_DOWN) walk_speed = 0;
		if (key == GLFW_KEY_O) open = !open;
	}
}

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filename);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void windowResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	if (height != 0) {
		aspect = (float)width / (float)height;
	}
	else {
		aspect = 1;
	}
}

void texCube2(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
	spNew->use();
	glUniformMatrix4fv(spNew->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spNew->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spNew->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(spNew->a("vertex"));
	glVertexAttribPointer(spNew->a("vertex"), 4, GL_FLOAT, false, 0, myCubeVertices);

	glEnableVertexAttribArray(spNew->a("normal"));
	glVertexAttribPointer(spNew->a("normal"), 4, GL_FLOAT, false, 0, myCubeNormals);

	glEnableVertexAttribArray(spNew->a("texCoord"));
	glVertexAttribPointer(spNew->a("texCoord"), 2, GL_FLOAT, false, 0, myCubeCoords);	

	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(spNew->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount);

	glDisableVertexAttribArray(spNew->a("vertex"));
	glDisableVertexAttribArray(spNew->a("normal"));
	glDisableVertexAttribArray(spNew->a("texCoord"));
}

void texFloor(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
	spNew->use();
	glUniformMatrix4fv(spNew->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spNew->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spNew->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(spNew->a("vertex"));
	glVertexAttribPointer(spNew->a("vertex"), 4, GL_FLOAT, false, 0, myCubeVertices);

	glEnableVertexAttribArray(spNew->a("normal"));
	glVertexAttribPointer(spNew->a("normal"), 4, GL_FLOAT, false, 0, myCubeNormals);

	glEnableVertexAttribArray(spNew->a("texCoord"));
	glVertexAttribPointer(spNew->a("texCoord"), 2, GL_FLOAT, false, 0, myCubeCoords);

	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, tex77);
	glUniform1i(spNew->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount);

	glDisableVertexAttribArray(spNew->a("vertex"));
	glDisableVertexAttribArray(spNew->a("normal"));
	glDisableVertexAttribArray(spNew->a("texCoord"));
}

void texGlass(glm::mat4 P, glm::mat4 V, glm::mat4 M, int obiekt) {
	spNew->use();
	glUniformMatrix4fv(spNew->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spNew->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spNew->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(spNew->a("vertex"));
	glVertexAttribPointer(spNew->a("vertex"), 4, GL_FLOAT, false, 0, verts[obiekt].data());

	glEnableVertexAttribArray(spNew->a("normal"));
	glVertexAttribPointer(spNew->a("normal"), 4, GL_FLOAT, false, 0, norms[obiekt].data());

	glEnableVertexAttribArray(spNew->a("texCoord"));
	glVertexAttribPointer(spNew->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords[obiekt].data());

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_DST_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, tex55);
	glUniform1i(spNew->u("tex"), 0);

	glDrawElements(GL_TRIANGLES, indices[obiekt].size(), GL_UNSIGNED_INT, indices[obiekt].data());

	glDisableVertexAttribArray(spNew->a("vertex"));
	glDisableVertexAttribArray(spNew->a("normal"));
	glDisableVertexAttribArray(spNew->a("texCoord"));
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void texCube3(glm::mat4 P, glm::mat4 V, glm::mat4 M, int obiekt) {
	spNew->use();
	glUniformMatrix4fv(spNew->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spNew->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spNew->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(spNew->a("vertex"));
	glVertexAttribPointer(spNew->a("vertex"), 4, GL_FLOAT, false, 0, verts[obiekt].data());

	glEnableVertexAttribArray(spNew->a("normal"));
	glVertexAttribPointer(spNew->a("normal"), 4, GL_FLOAT, false, 0, norms[obiekt].data());

	glEnableVertexAttribArray(spNew->a("texCoord"));
	glVertexAttribPointer(spNew->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords[obiekt].data());

	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(spNew->u("tex"), 0);

	glDrawElements(GL_TRIANGLES, indices[obiekt].size(), GL_UNSIGNED_INT, indices[obiekt].data());

	glDisableVertexAttribArray(spNew->a("vertex"));
	glDisableVertexAttribArray(spNew->a("normal"));
	glDisableVertexAttribArray(spNew->a("texCoord"));
}

void texGear(glm::mat4 P, glm::mat4 V, glm::mat4 M, int obiekt) {
	spNew->use();
	glUniformMatrix4fv(spNew->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spNew->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spNew->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(spNew->a("vertex"));
	glVertexAttribPointer(spNew->a("vertex"), 4, GL_FLOAT, false, 0, verts[obiekt].data());

	glEnableVertexAttribArray(spNew->a("normal"));
	glVertexAttribPointer(spNew->a("normal"), 4, GL_FLOAT, false, 0, norms[obiekt].data());

	glEnableVertexAttribArray(spNew->a("texCoord"));
	glVertexAttribPointer(spNew->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords[obiekt].data());

	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, tex33);
	glUniform1i(spNew->u("tex"), 0);

	glDrawElements(GL_TRIANGLES, indices[obiekt].size(), GL_UNSIGNED_INT, indices[obiekt].data());

	glDisableVertexAttribArray(spNew->a("vertex"));
	glDisableVertexAttribArray(spNew->a("normal"));
	glDisableVertexAttribArray(spNew->a("texCoord"));
}

void texClock(glm::mat4 P, glm::mat4 V, glm::mat4 M, int obiekt) {
	spNew->use();
	glUniformMatrix4fv(spNew->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spNew->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spNew->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(spNew->a("vertex"));
	glVertexAttribPointer(spNew->a("vertex"), 4, GL_FLOAT, false, 0, verts[obiekt].data());

	glEnableVertexAttribArray(spNew->a("normal"));
	glVertexAttribPointer(spNew->a("normal"), 4, GL_FLOAT, false, 0, norms[obiekt].data());

	glEnableVertexAttribArray(spNew->a("texCoord"));
	glVertexAttribPointer(spNew->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords[obiekt].data());

	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, tex22);
	glUniform1i(spNew->u("tex"), 0);

	glDrawElements(GL_TRIANGLES, indices[obiekt].size(), GL_UNSIGNED_INT, indices[obiekt].data());

	glDisableVertexAttribArray(spNew->a("vertex"));
	glDisableVertexAttribArray(spNew->a("normal"));
	glDisableVertexAttribArray(spNew->a("texCoord"));
}

void texHands(glm::mat4 P, glm::mat4 V, glm::mat4 M, int obiekt) {
	spNew->use();
	glUniformMatrix4fv(spNew->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spNew->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spNew->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(spNew->a("vertex"));
	glVertexAttribPointer(spNew->a("vertex"), 4, GL_FLOAT, false, 0, verts[obiekt].data());

	glEnableVertexAttribArray(spNew->a("normal"));
	glVertexAttribPointer(spNew->a("normal"), 4, GL_FLOAT, false, 0, norms[obiekt].data());

	glEnableVertexAttribArray(spNew->a("texCoord"));
	glVertexAttribPointer(spNew->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords[obiekt].data());

	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, tex44);
	glUniform1i(spNew->u("tex"), 0);

	glDrawElements(GL_TRIANGLES, indices[obiekt].size(), GL_UNSIGNED_INT, indices[obiekt].data());

	glDisableVertexAttribArray(spNew->a("vertex"));
	glDisableVertexAttribArray(spNew->a("normal"));
	glDisableVertexAttribArray(spNew->a("texCoord"));
}

void texCopula(glm::mat4 P, glm::mat4 V, glm::mat4 M, int obiekt) {
	spNew->use();
	glUniformMatrix4fv(spNew->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spNew->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spNew->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(spNew->a("vertex"));
	glVertexAttribPointer(spNew->a("vertex"), 4, GL_FLOAT, false, 0, verts[obiekt].data());

	glEnableVertexAttribArray(spNew->a("normal"));
	glVertexAttribPointer(spNew->a("normal"), 4, GL_FLOAT, false, 0, norms[obiekt].data());

	glEnableVertexAttribArray(spNew->a("texCoord"));
	glVertexAttribPointer(spNew->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords[obiekt].data());

	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(spNew->u("tex"), 0);

	glDrawElements(GL_TRIANGLES, indices[obiekt].size(), GL_UNSIGNED_INT, indices[obiekt].data());

	glDisableVertexAttribArray(spNew->a("vertex"));
	glDisableVertexAttribArray(spNew->a("normal"));
	glDisableVertexAttribArray(spNew->a("texCoord"));
}

void texWeight(glm::mat4 P, glm::mat4 V, glm::mat4 M, int obiekt) {
	spNew->use();
	glUniformMatrix4fv(spNew->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spNew->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spNew->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(spNew->a("vertex"));
	glVertexAttribPointer(spNew->a("vertex"), 4, GL_FLOAT, false, 0, verts[obiekt].data());

	glEnableVertexAttribArray(spNew->a("normal"));
	glVertexAttribPointer(spNew->a("normal"), 4, GL_FLOAT, false, 0, norms[obiekt].data());

	glEnableVertexAttribArray(spNew->a("texCoord"));
	glVertexAttribPointer(spNew->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords[obiekt].data());

	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, tex33);
	glUniform1i(spNew->u("tex"), 0);

	glDrawElements(GL_TRIANGLES, indices[obiekt].size(), GL_UNSIGNED_INT, indices[obiekt].data());

	glDisableVertexAttribArray(spNew->a("vertex"));
	glDisableVertexAttribArray(spNew->a("normal"));
	glDisableVertexAttribArray(spNew->a("texCoord"));
}

void loadModel(std::string plik) {
	using namespace std;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(plik,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	cout << importer.GetErrorString() << endl;
	aiMesh* mesh = scene->mMeshes[0];

	for (int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D vertex = mesh->mVertices[i];
		verts[liczba_obiektow].push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1));
		
		aiVector3D normal = mesh->mNormals[i];
		norms[liczba_obiektow].push_back(glm::vec4(normal.x, normal.y, normal.z, 0));

		aiVector3D texCoord = mesh->mTextureCoords[0][i];
		texCoords[liczba_obiektow].push_back(glm::vec2(texCoord.x, texCoord.y));
	}

	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace& face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) {
			indices[liczba_obiektow].push_back(face.mIndices[j]);
		}
	}
	liczba_obiektow++;
}

void initOpenGLProgram(GLFWwindow* window) {
	initShaders();
	
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, windowResize);

	tex = readTexture("./textures/wood.png");
	tex22 = readTexture("./textures/clockface.png");
	tex33 = readTexture("./textures/iron.png");
	tex44 = readTexture("./textures/hands_of_clock.png");
	tex55 = readTexture("./textures/glass.png");
	tex77 = readTexture("./textures/floor.png");

	loadModel(std::string("./objects/Gear.fbx"));
	loadModel(std::string("./objects/Pendulum.fbx"));
	loadModel(std::string("./objects/Clock_hands.fbx"));
	loadModel(std::string("./objects/Clockface.obj"));
	loadModel(std::string("./objects/Copula.fbx"));
	loadModel(std::string("./objects/Weight.fbx"));
	loadModel(std::string("./objects/Chain.fbx"));
	loadModel(std::string("./objects/Museum.obj"));
	loadModel(std::string("./objects/Glass.fbx"));
	loadModel(std::string("./objects/Frame.fbx"));
	loadModel(std::string("./objects/Handle.fbx"));
}

void freeOpenGLProgram(GLFWwindow* window) {
	freeShaders();
	glDeleteTextures(1, &tex);
}

void drawScene(GLFWwindow* window, float angle_x, float angle_y, float wahadlo, float open) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	time_t theTime = time(NULL);
//	std::tm aTime{};
//	localtime_r(aTime, &theTime);
	struct tm *tms;
	tms = localtime(&theTime);
	float hh = tms -> tm_hour; 	// aTime.tm_hour;
	float mm = tms -> tm_min; 	// aTime.tm_min;
	float ss = tms -> tm_sec;	// aTime.tm_sec;

	glm::mat4 V = glm::lookAt(pos, pos + calcDir(angle_x, angle_y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 0.1f, 50.0f);

	// Right wall
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(-1.0f, 0.0f, 0.0f));
	M = glm::rotate(M, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::scale(M, glm::vec3(1.0f, 2.0f, .03f));
	texCube2(P, V, M);

	// Left wall
	glm::mat4 Mi = glm::mat4(1.0f);
	Mi = glm::translate(Mi, glm::vec3(1.0f, 0.0f, 0.0f));
	Mi = glm::rotate(Mi, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Mi = glm::scale(Mi, glm::vec3(1.0f, 2.0f, .03f));
	texCube2(P, V, Mi);

	// Back wall
	glm::mat4 Mi2 = glm::mat4(1.0f);
	Mi2 = glm::translate(Mi2, glm::vec3(0.0f, 0.0f, 1.0f));
	Mi2 = glm::scale(Mi2, glm::vec3(1.0f, 2.0f, .03f));
	texCube2(P, V, Mi2);

	// Front bottom wall
	glm::mat4 Mi3 = glm::mat4(1.0f);
	Mi3 = glm::translate(Mi3, glm::vec3(0.0f, -1.6f, -1.0f));
	Mi3 = glm::scale(Mi3, glm::vec3(1.0f, .4f, .03f));
	texCube2(P, V, Mi3);

	// Bottom wall
	glm::mat4 Mi5 = glm::mat4(1.0f);
	Mi5 = glm::translate(Mi5, glm::vec3(0.0f, -1.5f, 0.0f));
	Mi5 = glm::scale(Mi5, glm::vec3(1.0f, .03f, 1.0f));
	texCube2(P, V, Mi5);

	// Bottom wall
	glm::mat4 Mi6 = glm::mat4(1.0f);
	Mi6 = glm::translate(Mi6, glm::vec3(0.0f, -1.8f, 0.0f));
	Mi6 = glm::scale(Mi6, glm::vec3(1.0f, .03f, 1.0f));
	texCube2(P, V, Mi6);

	// Floor
	glm::mat4 Mi7 = glm::mat4(1.0f);
	Mi7 = glm::translate(Mi7, glm::vec3(0.0f, -2.0f, 0.0f));
	Mi7 = glm::scale(Mi7, glm::vec3(10.0f, .03f, 10.0f));
	texFloor(P, V, Mi7);

	// Top wall
	glm::mat4 Mi8 = glm::mat4(1.0f);
	Mi8 = glm::translate(Mi8, glm::vec3(0.0f, 2.0f, 0.0f));
	Mi8 = glm::scale(Mi8, glm::vec3(1.0f, 0.03f, 1.0f));
	texCube2(P, V, Mi8);

	// Front bottom right wall
	glm::mat4 Mi9 = glm::mat4(1.0f);
	Mi9 = glm::translate(Mi9, glm::vec3(.8f, 0.0f, -1.0f));
	Mi9 = glm::scale(Mi9, glm::vec3(.2f, 2.0f, .03f));
	texCube2(P, V, Mi9);

	// Front door
	glm::mat4 Mi29 = glm::mat4(1.0f);
	Mi29 = glm::translate(Mi29, glm::vec3(.7f, 0.35f, -1.05f));
	Mi29 = glm::rotate(Mi29, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));	
	if (open == 1) {
		Mi29 = glm::rotate(Mi29, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else {
		Mi29 = glm::rotate(Mi29, glm::radians(-110.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	Mi29 = glm::scale(Mi29, glm::vec3(.7f, 1.55f, .03f));
	texCube3(P, V, Mi29, 9);

	// Glass
	glm::mat4 Mi32 = glm::mat4(1.0f) * Mi29;
	Mi32 = glm::translate(Mi32, glm::vec3(0.0f, 0.0f, 0.0f));
	Mi32 = glm::rotate(Mi32, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Mi32 = glm::scale(Mi32, glm::vec3(1.0f, 1.0f, 0.5f));
	texGlass(P, V, Mi32, 8);

	// Top hinge
	glm::mat4 Mi30 = glm::mat4(1.0f) * Mi29;
	Mi30 = glm::translate(Mi30, glm::vec3(0.0f, 0.5f, 0.0f));
	Mi30 = glm::scale(Mi30, glm::vec3(1.42f, 0.65f, 33.33f));
	Mi30 = glm::scale(Mi30, glm::vec3(0.05f, 0.05f, 0.05f));
	Mi30 = glm::rotate(Mi30, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	texWeight(P, V, Mi30, 5);

	// Bottom hinge
	glm::mat4 Mi31 = glm::mat4(1.0f) * Mi29;
	Mi31 = glm::translate(Mi31, glm::vec3(0.0f, -0.5f, 0.0f));
	Mi31 = glm::scale(Mi31, glm::vec3(1.42f, 0.65f, 33.33f));
	Mi31 = glm::scale(Mi31, glm::vec3(0.05f, 0.05f, 0.05f));
	Mi31 = glm::rotate(Mi31, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	texWeight(P, V, Mi31, 5);

	// Front right door wall
	glm::mat4 Mi10 = glm::mat4(1.0f);
	Mi10 = glm::translate(Mi10, glm::vec3(-0.8f, 0.0f, -1.0f));
	Mi10 = glm::scale(Mi10, glm::vec3(.2f, 2.0f, .03f));
	texCube2(P, V, Mi10);

	// Top rim
	glm::mat4 Mi11 = glm::mat4(1.0f);
	Mi11 = glm::translate(Mi11, glm::vec3(0.0f, 2.0f, 0.0f));
	Mi11 = glm::scale(Mi11, glm::vec3(1.1f, 0.1f, 1.1f));
	texCube2(P, V, Mi11);

	// Clock part
	glm::mat4 Mi12 = glm::mat4(1.0f);
	Mi12 = glm::translate(Mi12, glm::vec3(0.0f, 2.1f, 0.0f));
	Mi12 = glm::scale(Mi12, glm::vec3(0.5f, 0.74f, 1.1f));
	texCopula(P, V, Mi12, 4);

	// Stand
	glm::mat4 Mi13 = glm::mat4(1.0f);
	Mi13 = glm::translate(Mi13, glm::vec3(0.0f, -1.8f, 0.0f));
	Mi13 = glm::scale(Mi13, glm::vec3(1.2f, 0.16f, 1.2f));
	texCube2(P, V, Mi13);

	// Clock front
	glm::mat4 Mi14 = glm::mat4(1.0f);
	Mi14 = glm::translate(Mi14, glm::vec3(0.0f, 2.8f, -1.1f));
	Mi14 = glm::rotate(Mi14, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	Mi14 = glm::rotate(Mi14, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	Mi14 = glm::scale(Mi14, glm::vec3(0.65f, 1.0f, 0.65f));
	texClock(P, V, Mi14, 3);

	// Big left gear
	glm::mat4 Mi16 = glm::mat4(1.0f);
	Mi16 = glm::translate(Mi16, glm::vec3(.25f, 1.55f, 0.0f));
	Mi16 = glm::rotate(Mi16, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Mi16 = glm::rotate(Mi16, ss * 12 * PI / 180, glm::vec3(0.0f, 0.0f, 1.0f));
	Mi16 = glm::scale(Mi16, glm::vec3(0.2f, 0.2f, 0.2f));
	texGear(P, V, Mi16, 0);

	// Big right gear
	glm::mat4 Mi17 = glm::mat4(1.0f);
	Mi17 = glm::translate(Mi17, glm::vec3(-.25f, 1.55f, 0.0f));
	Mi17 = glm::rotate(Mi17, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Mi17 = glm::rotate(Mi17, -ss * 12 * PI / 180, glm::vec3(0.0f, 0.0f, 1.0f));
	Mi17 = glm::scale(Mi17, glm::vec3(0.2f, 0.2f, 0.2f));
	texGear(P, V, Mi17, 0);

	// Small right gear
	glm::mat4 Mi18 = glm::mat4(1.0f);
	Mi18 = glm::translate(Mi18, glm::vec3(-.19f, 1.91f, -0.06f));
	Mi18 = glm::rotate(Mi18, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Mi18 = glm::rotate(Mi18, glm::radians(18.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	Mi18 = glm::rotate(Mi18, -ss * 12 * PI / 180, glm::vec3(0.0f, 0.0f, 1.0f));
	Mi18 = glm::scale(Mi18, glm::vec3(0.15f, 0.15f, 0.15f));
	texGear(P, V, Mi18, 0);

	// Small left gear
	glm::mat4 Mi19 = glm::mat4(1.0f);
	Mi19 = glm::translate(Mi19, glm::vec3(.19f, 1.91f, -0.06f));
	Mi19 = glm::rotate(Mi19, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Mi19 = glm::rotate(Mi19, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	Mi19 = glm::rotate(Mi19, ss * 12 * PI / 180, glm::vec3(0.0f, 0.0f, 1.0f));
	Mi19 = glm::scale(Mi19, glm::vec3(0.15f, 0.15f, 0.15f));
	texGear(P, V, Mi19, 0);

	// Pendulum
	glm::mat4 Mi20 = glm::mat4(1.0f);
	Mi20 = glm::translate(Mi20, glm::vec3(-0.05f, 2.0f, 0.05f));
	Mi20 = glm::rotate(Mi20, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Mi20 = glm::rotate(Mi20, glm::radians(85.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	if (wahadlo < 15) {
		Mi20 = glm::rotate(Mi20, glm::radians(wahadlo/2), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else {
		Mi20 = glm::rotate(Mi20, glm::radians(-wahadlo/2+15), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	Mi20 = glm::scale(Mi20, glm::vec3(0.3f, 0.3f, 0.3f));
	texGear(P, V, Mi20, 1);

	// Minute hand
	float min = (mm + ss / 60) * 6;
	glm::mat4 Mi21 = glm::mat4(1.0f);
	Mi21 = glm::translate(Mi21, glm::vec3(0.0f, 2.8f, -1.04f));
	Mi21 = glm::rotate(Mi21, min * PI / 180, glm::vec3(0.0f, 0.0f, 1.0f));
	Mi21 = glm::scale(Mi21, glm::vec3(0.3f, 0.5f, 0.4f));
	texHands(P, V, Mi21, 2);

	// Hour hand
	float hour = (hh + mm / 60) * 30;
	glm::mat4 Mi22 = glm::mat4(1.0f);
	Mi22 = glm::translate(Mi22, glm::vec3(0.0f, 2.8f, -1.04f));
	Mi22 = glm::rotate(Mi22, hour * PI / 180, glm::vec3(0.0f, 0.0f, 1.0f));
	Mi22 = glm::scale(Mi22, glm::vec3(0.5f, 0.3f, 0.4f));
	texHands(P, V, Mi22, 2);

	// Second hand
	glm::mat4 Mi23 = glm::mat4(1.0f);
	Mi23 = glm::translate(Mi23, glm::vec3(0.0f, 2.8f, -1.04f));
	Mi23 = glm::rotate(Mi23, ss * PI / 180 * 6, glm::vec3(0.0f, 0.0f, 1.0f));
	Mi23 = glm::scale(Mi23, glm::vec3(0.1f, 0.56f, 0.4f));
	texHands(P, V, Mi23, 2);

	// Right weight
	glm::mat4 Mi24 = glm::mat4(1.0f);
	Mi24 = glm::translate(Mi24, glm::vec3(-0.2f, 0.5f, -0.35f));
	Mi24 = glm::rotate(Mi24, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	Mi24 = glm::rotate(Mi24, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	if (ss <= 30) {
		Mi24 = glm::translate(Mi24, glm::vec3(0.0f, 0.0f, ss * 0.02f));
	}
	else {
		Mi24 = glm::translate(Mi24, glm::vec3(0.0f, 0.0f, -ss * 0.02f + 1.2f));
	}
	Mi24 = glm::scale(Mi24, glm::vec3(0.2f, 0.2f, 0.2f));
	texWeight(P, V, Mi24, 5);

	// Left weight
	glm::mat4 Mi25 = glm::mat4(1.0f);
	Mi25 = glm::translate(Mi25, glm::vec3(0.2f, 1.2f, -0.35f));
	Mi25 = glm::rotate(Mi25, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	Mi25 = glm::rotate(Mi25, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	if (ss <= 30) {
		Mi25 = glm::translate(Mi25, glm::vec3(0.0f, 0.0f, -ss * 0.02f));
	}
	else {
		Mi25 = glm::translate(Mi25, glm::vec3(0.0f, 0.0f, ss * 0.02f - 1.2f));
	}
	Mi25 = glm::scale(Mi25, glm::vec3(0.2f, 0.2f, 0.2f));
	texWeight(P, V, Mi25, 5);

	// Left chain
	glm::mat4 Mi26 = glm::mat4(1.0f) * Mi25;
	Mi26 = glm::translate(Mi26, glm::vec3(0.0f, 0.0f, 0.0f));
	Mi26 = glm::rotate(Mi26, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Mi26 = glm::scale(Mi26, glm::vec3(.16f, .1f, .1));
	texWeight(P, V, Mi26, 6);

	// Right chain
	glm::mat4 Mi27 = glm::mat4(1.0f) * Mi24;
	Mi27 = glm::translate(Mi27, glm::vec3(0.0f, 0.0f, 0.0f));
	Mi27 = glm::rotate(Mi27, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Mi27 = glm::scale(Mi27, glm::vec3(.16f, .1f, .1f));
	texWeight(P, V, Mi27, 6);

	// Handle
	glm::mat4 Mi35 = glm::mat4(1.0f) * Mi29;
	Mi35 = glm::translate(Mi35, glm::vec3(-2.0f, 0.0f, .3f));
	Mi35 = glm::rotate(Mi35, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
	Mi35 = glm::scale(Mi35, glm::vec3(1.42f, 0.65f, 33.33f));
	Mi35 = glm::scale(Mi35, glm::vec3(0.02f, 0.02f, 0.02f));
	texGear(P, V, Mi35, 10);

	glfwSwapBuffers(window);
}


int main(void)
{
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(750, 750, "OpenGL", NULL, NULL);

	if (!window)
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); 
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window);

	float angle_x = 0;
	float angle_y = 0;
	float kat_x = 0;
	float kat_y = 0;
	float wahadlo = 0;
	float open1 = 0;
	glfwSetTime(0);
	while (!glfwWindowShouldClose(window))
	{
		kat_x += speed_x * glfwGetTime();
		kat_y += speed_y * glfwGetTime();
		if (wahadlo < 30) {
			wahadlo = wahadlo + 0.3;
		}
		else {
			wahadlo = 0;
		}
		pos += (float)(walk_speed * glfwGetTime()) * calcDir(kat_x, kat_y);
		glfwSetTime(0);

		open1 = open;
		glfwSetTime(0); 
		drawScene(window, kat_x, kat_y, wahadlo, open1); 
		glfwPollEvents();
	}

	freeOpenGLProgram(window);
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
