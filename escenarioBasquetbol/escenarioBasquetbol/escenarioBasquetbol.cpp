/*
	Proyecto final - Escenario deportivo 3D de Básquetbol

	Laboratorio de Computación Gráfica e Interacción Humano-Computadora, Grupo 09, 2027-1

	Integrantes:
- 		Alvarez Salgado Eduardo Antonio
- 		Flores Nava Nilsen Noé
- 		Ponce de León Reyes Bruno
*/

#include <iostream>
#include <cmath>
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>
#include <filesystem>

#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")


// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Otras bibliotecas
#include "stb_image.h"

// Matemáticas con GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Otros includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Prototipos de funciones
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void UserInputs();
void Animation();
void IniciarTiroLibre();

// Dimensiones de la ventana
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Cámara posicionada para tener perspectiva general de la cancha
Camera  camera(glm::vec3(0.0f, 6.0f, 18.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Atributos de luz
glm::vec3 lightPos(0.0f, 6.0f, 0.0f);
bool active;

// Posiciones de las luces puntuales
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f, 6.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// Tiempo delta
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Funciones auxiliares para la distribución del público
inline float Hash01(int a, int b, int c)
{
	unsigned int h = (unsigned int)a * 73856093u ^ (unsigned int)b * 19349663u ^ (unsigned int)c * 83492791u;
	h = (h ^ (h >> 13)) * 1274126177u;
	return (float)((h ^ (h >> 16)) & 0xFFFF) / 65535.0f;
}

Model* GetSpectatorModel(int index, Model& a, Model& b, Model& c, Model& d, Model& e, Model& f)
{
	switch (index % 6) {
	case 0: return &a;
	case 1: return &b;
	case 2: return &c;
	case 3: return &d;
	case 4: return &e;
	default: return &f;
	}
}

static void SetVec3(GLuint program, const std::string& name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
}

static void SetFloat(GLuint program, const std::string& name, float v)
{
	glUniform1f(glGetUniformLocation(program, name.c_str()), v);
}

static std::string ResolveModel(const std::string& path)
{
	namespace fs = std::filesystem;
	std::error_code ec;
	if (fs::is_regular_file(fs::path(path), ec))
		return path;

	std::cout << "  [Modelo] NO EXISTE: '" << path << "'" << std::endl;
	fs::path dir = fs::path(path).parent_path();
	if (dir.empty()) dir = ".";
	if (!fs::is_directory(dir, ec))
	{
		std::cout << "La carpeta '" << dir.generic_string() << "' tampoco existe (revisa el nombre de la carpeta)." << std::endl;
		return path;
	}

	std::cout << "Modelos encontrados en esa carpeta:" << std::endl;
	bool any = false;
	for (const auto& entry : fs::recursive_directory_iterator(dir, ec))
	{
		if (!entry.is_regular_file(ec)) continue;
		std::string ext = entry.path().extension().string();
		for (char& c : ext) c = (char)std::tolower((unsigned char)c);
		if (ext == ".obj" || ext == ".fbx" || ext == ".gltf" || ext == ".glb" || ext == ".dae" || ext == ".3ds")
		{
			std::cout << "             - " << entry.path().generic_string() << std::endl;
			any = true;
		}
	}
	if (!any)
		std::cout << "    (no hay ningun archivo de modelo en esa carpeta)" << std::endl;
	return path;
}

enum ParteCuerpo {
	TORSO, CABEZA, BICEP_DER, BICEP_IZQ, ANTEBRAZO_DER, ANTEBRAZO_IZQ,
	MANO_DER, MANO_IZQ, MUSLO_DER, MUSLO_IZQ, PIE_DER, PIE_IZQ, PARTES_CUERPO
};

static const int PADRE_PARTE[PARTES_CUERPO] = {
	-1, TORSO, TORSO, TORSO, BICEP_DER, BICEP_IZQ,
	ANTEBRAZO_DER, ANTEBRAZO_IZQ, TORSO, TORSO, MUSLO_DER, MUSLO_IZQ
};

static const char* NOMBRE_PARTE[PARTES_CUERPO] = {
	"Torso", "Cabeza", "BicepDer", "BicepIzq", "AntebrazoDer", "AntebrazoIzq",
	"ManoDer", "ManoIzq", "MusloDer", "MusloIzq", "PieDer", "PieIzq"
};

struct Rig {
	Model* modelo[PARTES_CUERPO];
	glm::vec3 junta[PARTES_CUERPO];
	glm::vec3 centro[PARTES_CUERPO];
	glm::vec3 base;
	float escala;
};

struct Pose
{
	float x[PARTES_CUERPO] = {};
	float y[PARTES_CUERPO] = {};
	float z[PARTES_CUERPO] = {};
};

struct Personaje {
	const Rig* rig = nullptr;
	glm::vec3 posicion = glm::vec3(0.0f);
	float yaw = 0.0f;
	Pose pose;
	float amplitud = 0.0f;
	float velocidad = 2.0f;
	float fase = 0.0f;
	bool balanceoFrontal = false;   // true=oscilan adelante-atrás
	glm::mat4 mundo[PARTES_CUERPO];
	glm::mat4 cabeza = glm::mat4(1.0f);

	glm::vec3 PosicionCabeza() const { return glm::vec3(cabeza[3]); }
	glm::vec3 FrenteCabeza() const { return glm::normalize(glm::vec3(cabeza * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f))); }
	glm::vec3 ArribaCabeza() const { return glm::normalize(glm::vec3(cabeza * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f))); }

};

static Rig CrearRig(Model* partes[PARTES_CUERPO], float alturaDeseada, const char* nombre)
{
	Rig r;
	glm::vec3 mn[PARTES_CUERPO], mx[PARTES_CUERPO];
	for (int i = 0; i < PARTES_CUERPO; i++)
	{
		r.modelo[i] = partes[i];
		mn[i] = partes[i]->GetBoundsMin();
		mx[i] = partes[i]->GetBoundsMax();
		r.centro[i] = (mn[i] + mx[i]) * 0.5f;
	}

	glm::vec3 torsoArribaDcto(mx[TORSO].x, mx[TORSO].y, r.centro[TORSO].z);
	glm::vec3 torsoArribaIzq(mn[TORSO].x, mx[TORSO].y, r.centro[TORSO].z);
	glm::vec3 torsoAbajoDcto(mx[TORSO].x, mn[TORSO].y, r.centro[TORSO].z);
	glm::vec3 torsoAbajoIzq(mn[TORSO].x, mn[TORSO].y, r.centro[TORSO].z);

	r.junta[TORSO] = r.centro[TORSO];
	r.junta[CABEZA] = glm::vec3(r.centro[CABEZA].x, mn[CABEZA].y, r.centro[CABEZA].z);

	r.junta[BICEP_DER] = glm::vec3(mx[BICEP_DER].x, torsoArribaDcto.y, r.centro[BICEP_DER].z);
	r.junta[ANTEBRAZO_DER] = glm::vec3(mx[ANTEBRAZO_DER].x, r.centro[BICEP_DER].y, r.centro[ANTEBRAZO_DER].z);
	r.junta[MANO_DER] = glm::vec3(mx[MANO_DER].x, r.centro[ANTEBRAZO_DER].y, r.centro[MANO_DER].z);
	r.junta[MUSLO_DER] = glm::vec3(mx[MUSLO_DER].x, torsoAbajoDcto.y, r.centro[MUSLO_DER].z);
	r.junta[PIE_DER] = glm::vec3(mx[PIE_DER].x, mx[PIE_DER].y, r.centro[PIE_DER].z);

	r.junta[BICEP_IZQ] = glm::vec3(mn[BICEP_IZQ].x, torsoArribaIzq.y, r.centro[BICEP_IZQ].z);
	r.junta[ANTEBRAZO_IZQ] = glm::vec3(mn[ANTEBRAZO_IZQ].x, r.centro[BICEP_IZQ].y, r.centro[ANTEBRAZO_IZQ].z);
	r.junta[MANO_IZQ] = glm::vec3(mn[MANO_IZQ].x, r.centro[ANTEBRAZO_IZQ].y, r.centro[MANO_IZQ].z);
	r.junta[MUSLO_IZQ] = glm::vec3(mn[MUSLO_IZQ].x, torsoAbajoIzq.y, r.centro[MUSLO_IZQ].z);
	r.junta[PIE_IZQ] = glm::vec3(mn[PIE_IZQ].x, mx[PIE_IZQ].y, r.centro[PIE_IZQ].z);

	float piso = std::min(mn[PIE_DER].y, mn[PIE_IZQ].y);
	float altura = mx[CABEZA].y - piso;
	r.escala = altura > 0.001f ? alturaDeseada / altura : 1.0f;
	r.base = glm::vec3(r.centro[TORSO].x, piso, r.centro[TORSO].z);

	return r;
}

static void ActualizarPersonaje(Personaje& p, float t)
{
	const Rig& r = *p.rig;
	const glm::mat4 I(1.0f);
	const glm::vec3 ejeX(1.0f, 0.0f, 0.0f);
	const glm::vec3 ejeY(0.0f, 1.0f, 0.0f);

	float s = std::sin(t * p.velocidad + p.fase) * p.amplitud;
	float animX[PARTES_CUERPO] = {};
	animX[BICEP_DER] = 35.0f * s;
	animX[BICEP_IZQ] = -35.0f * s;
	animX[ANTEBRAZO_DER] = 15.0f * s;
	animX[ANTEBRAZO_IZQ] = -15.0f * s;
	animX[MUSLO_DER] = -30.0f * s;
	animX[MUSLO_IZQ] = 30.0f * s;
	animX[PIE_DER] = 10.0f * s;
	animX[PIE_IZQ] = -10.0f * s;

	glm::mat4 base = glm::translate(I, p.posicion);
	base = glm::rotate(base, glm::radians(p.yaw), ejeY);
	base = glm::scale(base, glm::vec3(r.escala));
	base = glm::translate(base, -r.base);

	for (int i = 0; i < PARTES_CUERPO; i++)
	{
		glm::mat4 rot =
			glm::rotate(I, glm::radians(p.pose.y[i]), ejeY)
			* glm::rotate(I, glm::radians(p.pose.x[i]), ejeX)
			* glm::rotate(I, glm::radians(p.pose.z[i]), glm::vec3(0.0f, 0.0f, 1.0f)
			);

		//balanceo de la caminata
		rot = rot * glm::rotate(I, glm::radians(animX[i]), p.balanceoFrontal ? glm::vec3(0.0f, 0.0f, 1.0f) : ejeX);

		if (i == TORSO)
			p.mundo[i] = base * glm::translate(I, r.junta[TORSO]) * rot;
		else
			p.mundo[i] = p.mundo[PADRE_PARTE[i]] * glm::translate(I, r.junta[i] - r.junta[PADRE_PARTE[i]]) * rot;
	}

	p.cabeza = p.mundo[CABEZA] * glm::translate(I, r.centro[CABEZA] - r.junta[CABEZA]);
}

static void DibujarPersonaje(const Personaje& p, Shader& shader, GLint modelLoc, bool omitirCabeza = false)
{
	const Rig& r = *p.rig;
	for (int i = 0; i < PARTES_CUERPO; i++)
	{
		if (omitirCabeza && i == CABEZA) continue;
		glm::mat4 m = p.mundo[i] * glm::translate(glm::mat4(1.0f), -r.junta[i]);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
		r.modelo[i]->Draw(shader);
	}
}

// Personaje controlable por user
Personaje jugadorP;
float jugadorRumbo = 0.0f; // hacia donde ve
const float JUGADOR_VELOCIDAD = 4.0f; 
const float JUGADOR_GIRO_VEL = 120.0f; 
const float JUGADOR_LIMITE = 11.0f;
const float JUGADOR_YAW_AJUSTE = -90.0f;   // corrige hacia dónde mira

// ANIMACION POR KEYFRAMES - TIRO LIBRE
struct KeyFrameTiro
{
	float tiempo;
	float torsoX;
	float bicepDerZ;
	float bicepIzqZ;
	float antebrazoDerZ;
	float antebrazoIzqZ;
	float manoDerZ;
	float manoIzqZ;
};

const int NUM_KEYFRAMES_TIRO = 7;

KeyFrameTiro keyframesTiro[NUM_KEYFRAMES_TIRO];

bool tiroActivo = false;
float tiempoTiro = 0.0f;
int indiceTiro = 0;

glm::vec3 balonPos(0.0f, 0.2f, 0.0f);
float balonRot = 0.0f;

//estado de reflectores
bool reflectoresEncendidos = true;

// Centro del aro
const glm::vec3 POS_ARO_TIRO(0.0f, 3.08f, 8.62f);

// Estas posiciones se calcularan cada vez segun donde se encuentre el jugador
glm::vec3 BALON_INICIAL(0.0f);
glm::vec3 BALON_CABEZA(0.0f);
glm::vec3 BALON_LIBERACION(0.0f);

// Altura de la parabola
float ALTURA_ARCO = 1.40f;

const float TIEMPO_LIBERACION = 0.90f;
const float TIEMPO_ARO = 2.05f;
const float TIEMPO_FIN = 2.60f;

// Se usa un enum para poner los modos de la camara
enum ModoCamara { CAMARA_LIBRE = 1, CAMARA_PRIMERA = 2, CAMARA_TERCERA = 3 };
int modoCamara = CAMARA_LIBRE; //indica modo actual

const float TERCERA_DISTANCIA = 5.0f;  //la distancia que tiene en tercera persoan
const float TERCERA_ALTURA = 2.5f;  
const float PRIMERA_ADELANTE = 0.15f;  // adelanta la camara para no quedar dentro de la cabeza

// Hacia dónde mira el jugador
glm::vec3 FrenteJugador()
{
	float rad = glm::radians(jugadorRumbo);
	return glm::vec3(std::sin(rad), 0.0f, -std::cos(rad));
}

// Calcula posición y matriz de vista de la cámara activa
void ObtenerCamaraActiva(glm::vec3& pos, glm::mat4& view)
{
	const glm::vec3 arriba(0.0f, 1.0f, 0.0f);
	glm::vec3 frente = FrenteJugador();

	if (modoCamara == CAMARA_PRIMERA){
		pos = jugadorP.PosicionCabeza() + frente * PRIMERA_ADELANTE;
		view = glm::lookAt(pos, pos + frente, arriba);
	}
	else if (modoCamara == CAMARA_TERCERA){
		glm::vec3 objetivo = jugadorP.posicion + glm::vec3(0.0f, 1.5f, 0.0f);
		pos = jugadorP.posicion - frente * TERCERA_DISTANCIA + glm::vec3(0.0f, TERCERA_ALTURA, 0.0f);
		view = glm::lookAt(pos, objetivo, arriba);
	}
	else{
		pos = camera.GetPosition();
		view = camera.GetViewMatrix();
	}
}

void IniciarTiroLibre()
{
	if (tiroActivo)
		return;

	tiroActivo = true;
	tiempoTiro = 0.0f;
	indiceTiro = 0;

	// Detener completamente el balanceo de caminata
	jugadorP.amplitud = 0.0f;

	// Direccion desde la posicion ACTUAL del jugador hacia el aro
	glm::vec3 haciaAro(POS_ARO_TIRO.x - jugadorP.posicion.x, 0.0f, POS_ARO_TIRO.z - jugadorP.posicion.z);

	float distanciaAro = glm::length(haciaAro);

	glm::vec3 frente = glm::normalize(haciaAro);

	// Vector hacia la derecha del jugador
	glm::vec3 derecha = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), frente));

	// Hacer que el jugador mire hacia la canasta
	jugadorRumbo =glm::degrees(std::atan2(frente.x, -frente.z));

	jugadorP.yaw = -jugadorRumbo + JUGADOR_YAW_AJUSTE;

	// Balon frente al cuerpo
	BALON_INICIAL = jugadorP.posicion + derecha * 0.16f + frente * 0.25f + glm::vec3(0.0f, 1.10f, 0.0f);

	// Balon frente a la cabeza
	BALON_CABEZA = jugadorP.posicion + derecha * 0.12f + frente * 0.38f + glm::vec3(0.0f, 1.80f, 0.0f);

	// Punto donde se libera
	BALON_LIBERACION = jugadorP.posicion + derecha * 0.10f + frente * 0.55f + glm::vec3(0.0f, 2.13f, 0.0f);

	// Hacer el arco un poco mayor si el jugador esta lejos
	ALTURA_ARCO = glm::clamp(distanciaAro * 0.18f, 0.90f, 2.00f);

	// Borrar cualquier pose anterior
	jugadorP.pose = Pose();

	// KF 0 - Balon frente al cuerpo
	keyframesTiro[0] = {
		0.0f,
		3.0f,
		 15.0f,    // bicep derecho
		-15.0f,    // bicep izquierdo
		 45.0f,    // antebrazo derecho
		-45.0f,    // antebrazo izquierdo
		 0.0f,
		 0.0f
	};

	// KF 1 - Flexion / preparacion
	keyframesTiro[1] = {
		0.30f,
		6.0f,
		 22.0f,
		-22.0f,
		 58.0f,
		-58.0f,
		 0.0f,
		 0.0f
	};

	// KF 2 - Balon frente a la cara
	keyframesTiro[2] = {
		0.65f,
		3.0f,
		 42.0f,
		-42.0f,
		 52.0f,
		-52.0f,
		 0.0f,
		 0.0f
	};

	// KF 3 - Liberacion
	keyframesTiro[3] = {
		0.90f,
		0.0f,
		 62.0f,
		-62.0f,
		 30.0f,
		-30.0f,
		 8.0f,
		-3.0f
	};

	// KF 4 - Follow-through
	keyframesTiro[4] = {
		1.25f,
		0.0f,
		 70.0f,
		-70.0f,
		 15.0f,
		-15.0f,
		 18.0f,
		-5.0f
	};

	// KF 5 - Regreso de brazos
	keyframesTiro[5] = {
		2.10f,
		0.0f,
		 30.0f,
		-30.0f,
		 20.0f,
		-20.0f,
		 5.0f,
		-5.0f
	};

	// KF 6 - Pose normal
	keyframesTiro[6] = {
		2.60f,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		0.0f
	};

	balonPos = BALON_INICIAL;
	balonRot = 0.0f;

	std::cout
		<< "[ANIMACION] Tiro libre iniciado"
		<< std::endl;
}

int main()
{
	std::cout << "[DEBUG] Inicializando GLFW..." << std::endl;
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Escenario Deportivo 3D - Basquetbol", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	std::cout << "[DEBUG] Cargando Shaders..." << std::endl;
	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");

	// Carga de modelos
	std::cout << "[DEBUG] Cargando modelos del escenario..." << std::endl;
	Model cancha(ResolveModel("modelos/cancha/canchaBasquet.obj"));
	Model gradas(ResolveModel("modelos/Gradas/Gradas.obj"));
	Model marcador(ResolveModel("modelos/marcador/marcadorBueno.obj"));
	Model barrera(ResolveModel("modelos/barrera/barrera.obj"));
	Model bocina(ResolveModel("modelos/bocina/bocina.obj"));
	Model camaraObj(ResolveModel("modelos/camara/source/camar.fbx"));
	Model ceilingLights(ResolveModel("modelos/ceiling-lights/source/PTRCUNWM24VXBNKZ/ASSET.fbx"));
	Model silla(ResolveModel("modelos/silla_metalica/silla_metalica.fbx"));
	Model mesas(ResolveModel("modelos/mesas/scene.gltf"));
	Model camaraBroadcast(ResolveModel("modelos/set-of-four-4-low-poly-cameras/source/camara_broadcast.obj"));
	Model spotlight(ResolveModel("modelos/Spotlight/source/spotlight_simple_animado.fbx"));

	std::cout << "[DEBUG] Cargando personajes..." << std::endl;

	// Árbitro
	Model cabezaArbitro(ResolveModel("modelos/Player_Arbitro/Cabeza_Arbitro.obj"));
	Model torsoArbitro(ResolveModel("modelos/Player_Arbitro/Torso_Arbitro.obj"));
	Model bicepDerArbitro(ResolveModel("modelos/Player_Arbitro/BicepDer_Arbitro.obj"));
	Model bicepIzqArbitro(ResolveModel("modelos/Player_Arbitro/BicepIzq_Arbitro.obj"));
	Model antebrazoDerArbitro(ResolveModel("modelos/Player_Arbitro/AntebrazoDer_Arbitro.obj"));
	Model antebrazoIzqArbitro(ResolveModel("modelos/Player_Arbitro/AntebrazoIzq_Arbitro.obj"));
	Model manoDerArbitro(ResolveModel("modelos/Player_Arbitro/ManoDer_Arbitro.obj"));
	Model manoIzqArbitro(ResolveModel("modelos/Player_Arbitro/ManoIzq_Arbitro.obj"));
	Model musloDerArbitro(ResolveModel("modelos/Player_Arbitro/MusloDer_Arbitro.obj"));
	Model musloIzqArbitro(ResolveModel("modelos/Player_Arbitro/MusloIzq_Arbitro.obj"));
	Model pieDerArbitro(ResolveModel("modelos/Player_Arbitro/PieDer_Arbitro.obj"));
	Model pieIzqArbitro(ResolveModel("modelos/Player_Arbitro/PieIzq_Arbitro.obj"));

	// Jugadores Lakers
	Model cabezaLakers(ResolveModel("modelos/Player_Lakers/Cabeza_Jugador.obj"));
	Model torsoLakers(ResolveModel("modelos/Player_Lakers/Torso_Jugador.obj"));
	Model bicepDerLakers(ResolveModel("modelos/Player_Lakers/BicepDer_Jugador.obj"));
	Model bicepIzqLakers(ResolveModel("modelos/Player_Lakers/BicepIzq_Jugador.obj"));
	Model antebrazoDerLakers(ResolveModel("modelos/Player_Lakers/AntebrazoDer_Jugador.obj"));
	Model antebrazoIzqLakers(ResolveModel("modelos/Player_Lakers/AntebrazoIzq_Jugador.obj"));
	Model manoDerLakers(ResolveModel("modelos/Player_Lakers/ManoDer_Jugador.obj"));
	Model manoIzqLakers(ResolveModel("modelos/Player_Lakers/ManoIzq_Jugador.obj"));
	Model musloDerLakers(ResolveModel("modelos/Player_Lakers/MusloDer_Jugador.obj"));
	Model musloIzqLakers(ResolveModel("modelos/Player_Lakers/MusloIzq_Jugador.obj"));
	Model pieDerLakers(ResolveModel("modelos/Player_Lakers/PieDer_Jugador.obj"));
	Model pieIzqLakers(ResolveModel("modelos/Player_Lakers/PieIzq_Jugador.obj"));

	// Jugadores Bulls
	Model cabezaBulls(ResolveModel("modelos/Player_Bulls/Cabeza_Bulls.obj"));
	Model torsoBulls(ResolveModel("modelos/Player_Bulls/Torso_Bulls.obj"));
	Model bicepDerBulls(ResolveModel("modelos/Player_Bulls/BicepDer_Bulls.obj"));
	Model bicepIzqBulls(ResolveModel("modelos/Player_Bulls/BicepIzq_Bulls.obj"));
	Model antebrazoDerBulls(ResolveModel("modelos/Player_Bulls/AntebrazoDer_Bulls.obj"));
	Model antebrazoIzqBulls(ResolveModel("modelos/Player_Bulls/AntebrazoIzq_Bulls.obj"));
	Model manoDerBulls(ResolveModel("modelos/Player_Bulls/ManoDer_Bulls.obj"));
	Model manoIzqBulls(ResolveModel("modelos/Player_Bulls/ManoIzq_Bulls.obj"));
	Model musloDerBulls(ResolveModel("modelos/Player_Bulls/MusloDer_Bulls.obj"));
	Model musloIzqBulls(ResolveModel("modelos/Player_Bulls/MusloIzq_Bulls.obj"));
	Model pieDerBulls(ResolveModel("modelos/Player_Bulls/PieDer_Bulls.obj"));
	Model pieIzqBulls(ResolveModel("modelos/Player_Bulls/PieIzq_Bulls.obj"));

	// Extras
	Model balon(ResolveModel("modelos/balonBaloncesto/BalonBaloncesto5.obj"));
	Model bancaConJugador1(ResolveModel("modelos/BancaConJugador/banca_jugador1/bancaConJugador1.fbx"));
	Model bancaConJugador2(ResolveModel("modelos/BancaConJugador/banca_jugador2/bancaConJugador2.fbx"));
	Model bancaMadera(ResolveModel("modelos/BancaMadera/banca.obj"));
	Model cuarto(ResolveModel("modelos/cuarto/cuarto.obj"));

	// Modelos de espectadores
	Model espectador_A(ResolveModel("modelos/espectador/espectador_A.obj"));
	Model espectador_B(ResolveModel("modelos/espectador/espectador_B.obj"));
	Model espectador_C(ResolveModel("modelos/espectador/espectador_C.obj"));
	Model espectador_D(ResolveModel("modelos/espectador/espectador_D.obj"));
	Model espectador_E(ResolveModel("modelos/espectador/espectador_E.obj"));
	Model espectador_F(ResolveModel("modelos/espectador/espectador_F.obj"));

	std::cout << "[DEBUG] Modelos cargados exitosamente. Iniciando ciclo de renderizado..." << std::endl;

	// Configurar VAO y VBO para el suelo y lámparas
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);


	// Luces puntuales
	for (int i = 1; i < 4; i++)
	{
		std::string base = "pointLights[" + std::to_string(i) + "].";
		SetVec3(lightingShader.Program, base + "position", 0.0f, 0.0f, 0.0f);
		SetVec3(lightingShader.Program, base + "ambient", 0.0f, 0.0f, 0.0f);
		SetVec3(lightingShader.Program, base + "diffuse", 0.0f, 0.0f, 0.0f);
		SetVec3(lightingShader.Program, base + "specular", 0.0f, 0.0f, 0.0f);
		SetFloat(lightingShader.Program, base + "constant", 1.0f);
		SetFloat(lightingShader.Program, base + "linear", 0.09f);
		SetFloat(lightingShader.Program, base + "quadratic", 0.032f);
	}

	// Spot light: apagada
	SetVec3(lightingShader.Program, "spotLight.position", 0.0f, 6.0f, 0.0f);
	SetVec3(lightingShader.Program, "spotLight.direction", 0.0f, -1.0f, 0.0f);
	SetVec3(lightingShader.Program, "spotLight.ambient", 0.0f, 0.0f, 0.0f);
	SetVec3(lightingShader.Program, "spotLight.diffuse", 0.0f, 0.0f, 0.0f);
	SetVec3(lightingShader.Program, "spotLight.specular", 0.0f, 0.0f, 0.0f);
	SetFloat(lightingShader.Program, "spotLight.constant", 1.0f);
	SetFloat(lightingShader.Program, "spotLight.linear", 0.09f);
	SetFloat(lightingShader.Program, "spotLight.quadratic", 0.032f);
	SetFloat(lightingShader.Program, "spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	SetFloat(lightingShader.Program, "spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Estructura para los lados de la arena
	struct ArenaSide {
		glm::vec3 n;
		glm::vec3 t;
		float yawToRing;
	};

	ArenaSide arenaSides[4] = {
		{ glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), 180.0f },  // Norte (+Z)
		{ glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), -90.0f },   // Este (+X)
		{ glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 90.0f },   // Oeste (-X)
		{ glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.0f }     // Sur (-Z)
	};


	const float BARRERA_DIST = 12.0f;
	const float BARRERA_PASO = 1.695f;
	const int   BARRERA_MITAD = 6;          // barreras por lado = 2 * BARRERA_MITAD + 1
	const float GRADAS_FRENTE = 15.5f;
	const float GRADAS_SECCION = 3.36f;
	const float MARCADOR_ESCALA = 0.85f;

	const float ESPECTADOR_GIRO = 0.0f;

	// Parámetros geométricos de las gradas para posicionar al público
	const float GRADAS_BORDE[6] = { -14.0f, -9.28f, -4.56f, 0.15f, 4.87f, 9.59f };
	const float GRADAS_ALTO[6] = { 2.52f, 5.22f, 7.91f, 10.61f, 13.3f, 16.0f };
	const float UNIDADES_POR_M = 6.0f;
	const float ASIENTO = 0.46f * UNIDADES_POR_M;
	const float MEDIO_ASIENTO = 0.23f * UNIDADES_POR_M;
	const float PASO = 0.62f * UNIDADES_POR_M;
	const int POR_FILA = 9;
	const float OCUPACION_GRADAS = 0.70f;

	const float SUELO_Y = 0.05f;

	const float BALON_X = 0.0f, BALON_Z = 0.0f;
	const float BALON_ESCALA = 0.2f;

	const float BOCINA_DIST = 13.0f;
	const float BOCINA_ESCALA = 0.6f;
	const float BOCINA_GIRO = 180.0f;

	const float MESAS_DIST = 9.0f;
	const float MESAS_Z = 0.0f;
	const int   SILLAS_CANTIDAD = 3;
	const float SILLA_ESCALA = 0.46f / 163.0f;
	const float SILLA_PASO = 1.0f;
	const float SILLA_SEPARACION = 0.6f;
	const float SILLA_GIRO = 180.0f;

	const float BANCAS_DIST = 6.2f;
	const float BANCAS_SEPARACION = 0.1f;


	const float BANCA_MADERA_LARGO = 2.2f;
	const float BANCA_JUGADOR_LARGO = 2.2f;

	const float CUARTO_ESCALA_X = 1.0f;
	const float CUARTO_ESCALA_Y = 0.8f;
	const float CUARTO_ESCALA_Z = 1.1f;

	const float CUARTO_GIRO = 0.0f;
	const float CUARTO_Y = -0.5f;

	auto yawAlineado = [](const Model& m) {
		glm::vec3 sz = m.GetSize();
		return sz.x >= sz.z ? 90.0f : 0.0f;
		};
	auto largoAlineado = [](const Model& m) {
		glm::vec3 sz = m.GetSize();
		return sz.x >= sz.z ? sz.x : sz.z;
		};
	auto fondoAlineado = [](const Model& m) {
		glm::vec3 sz = m.GetSize();
		return sz.x >= sz.z ? sz.z : sz.x;
		};

	const float yawMesas = yawAlineado(mesas);
	const float fondoMesas = fondoAlineado(mesas);
	const float sillasX = -MESAS_DIST - fondoMesas * 0.5f - SILLA_SEPARACION;
	const float sillasYaw = glm::degrees(std::atan2(1.0f, 0.0f)) + SILLA_GIRO;

	auto escalaPorLargo = [&](const Model& m, float largoDeseado) {
		return largoDeseado / std::max(largoAlineado(m), 0.001f);
		};
	const float escalaBancaMadera = escalaPorLargo(bancaMadera, BANCA_MADERA_LARGO);
	const float escalaBancaJugador1 = escalaPorLargo(bancaConJugador1, BANCA_JUGADOR_LARGO);
	const float escalaBancaJugador2 = escalaPorLargo(bancaConJugador2, BANCA_JUGADOR_LARGO);

	const float largoMesas = largoAlineado(mesas);
	const float zBancaJugador = largoMesas * 0.5f + BANCAS_SEPARACION + BANCA_JUGADOR_LARGO * 0.5f;
	const float zBancaMadera = -(largoMesas * 0.5f + BANCAS_SEPARACION + BANCA_MADERA_LARGO * 0.5f);

	const float ALTURA_ARBITRO = 1.80f;
	const float ALTURA_JUGADOR = 1.95f;
	const float JUGADOR_GIRO = 0.0f;
	const bool  ANIMAR_PERSONAJES = true;
	const float PERSONAJES_AMPLITUD = 0.5f;
	const int   JUGADOR_CAMARA_BULLS = 0;
	const int   JUGADOR_CAMARA_LAKERS = 0;

	Model* partesArbitro[PARTES_CUERPO] = {
		&torsoArbitro, &cabezaArbitro, &bicepDerArbitro, &bicepIzqArbitro, &antebrazoDerArbitro, &antebrazoIzqArbitro,
		&manoDerArbitro, &manoIzqArbitro, &musloDerArbitro, &musloIzqArbitro, &pieDerArbitro, &pieIzqArbitro };
	Model* partesLakers[PARTES_CUERPO] = {
		&torsoLakers, &cabezaLakers, &bicepDerLakers, &bicepIzqLakers, &antebrazoDerLakers, &antebrazoIzqLakers,
		&manoDerLakers, &manoIzqLakers, &musloDerLakers, &musloIzqLakers, &pieDerLakers, &pieIzqLakers };
	Model* partesBulls[PARTES_CUERPO] = {
		&torsoBulls, &cabezaBulls, &bicepDerBulls, &bicepIzqBulls, &antebrazoDerBulls, &antebrazoIzqBulls,
		&manoDerBulls, &manoIzqBulls, &musloDerBulls, &musloIzqBulls, &pieDerBulls, &pieIzqBulls };

	const Rig rigArbitro = CrearRig(partesArbitro, ALTURA_ARBITRO, "Arbitro");
	const Rig rigLakers = CrearRig(partesLakers, ALTURA_JUGADOR, "Lakers");
	const Rig rigBulls = CrearRig(partesBulls, ALTURA_JUGADOR, "Bulls");

	// Posiciones (X, Z) para los 5 jugadores dentro de la duela
	const float posBulls[5][2] = {
		{ 0.0f, -2.5f },  
		{ -3.5f, -5.5f },  
		{ 3.5f, -5.5f },  
		{ -2.0f, -8.5f },  
		{ 2.0f, -8.5f }   
	};

	const float posLakers[5][2] = {
		{ 0.0f,  2.5f },  
		{ 3.5f,  5.5f },  
		{ -3.5f,  5.5f }, 
		{ 2.0f,  8.5f },  
		{ -2.0f,  8.5f }   
	};

	Personaje arbitroP;
	Personaje bulls[5];
	Personaje lakers[5];

	arbitroP.rig = &rigArbitro;
	arbitroP.posicion = glm::vec3(-2.0f, SUELO_Y, 0.0f);
	arbitroP.yaw = 180.0f;
	arbitroP.amplitud = ANIMAR_PERSONAJES ? PERSONAJES_AMPLITUD * 0.4f : 0.0f;

	jugadorP.rig = &rigBulls;
	jugadorP.posicion = glm::vec3(5.0f, SUELO_Y, 0.0f);
	jugadorP.balanceoFrontal = true;
	jugadorP.amplitud = 0.0f;
	jugadorP.velocidad = 6.0f;
	jugadorP.yaw = -jugadorRumbo + JUGADOR_YAW_AJUSTE;

	for (int i = 0; i < 5; i++)
	{
		bulls[i].rig = &rigBulls;
		bulls[i].posicion = glm::vec3(posBulls[i][0], SUELO_Y, posBulls[i][1]);
		bulls[i].yaw = 90.0f;
		bulls[i].amplitud = ANIMAR_PERSONAJES ? PERSONAJES_AMPLITUD : 0.0f;
		bulls[i].velocidad = 2.0f + 0.2f * i;
		bulls[i].fase = 1.3f * i;

		lakers[i].rig = &rigLakers;
		lakers[i].posicion = glm::vec3(posLakers[i][0], SUELO_Y, posLakers[i][1]);
		lakers[i].yaw = 270.0f;
		lakers[i].amplitud = ANIMAR_PERSONAJES ? PERSONAJES_AMPLITUD : 0.0f;
		lakers[i].velocidad = 2.0f + 0.2f * i;
		lakers[i].fase = 0.9f * i + 0.5f;
	}

	Personaje* jugadorCamaraBulls = &bulls[JUGADOR_CAMARA_BULLS];
	Personaje* jugadorCamaraLakers = &lakers[JUGADOR_CAMARA_LAKERS];

	// Bucle principal
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		UserInputs();
		Animation();

		ActualizarPersonaje(arbitroP, currentFrame);
		ActualizarPersonaje(jugadorP, currentFrame);

		for (int i = 0; i < 5; i++)
		{
			ActualizarPersonaje(bulls[i], currentFrame);
			ActualizarPersonaje(lakers[i], currentFrame);
		}

		glClearColor(0.15f, 0.2f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		lightingShader.Use();

		glm::vec3 camPos;
		glm::mat4 view;
		ObtenerCamaraActiva(camPos, view);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z);

		//GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		//glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// Luz direccional
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

		// Luz puntual de apoyo
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		//glm::mat4 view = camera.GetViewMatrix();

		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model(1);

		auto dibujarSobreSuelo = [&](Model& m, float x, float z, float yawDeg, float s) {
			glm::vec3 mn = m.GetBoundsMin(), mx = m.GetBoundsMax();
			glm::vec3 c = (mn + mx) * 0.5f;
			glm::mat4 mm(1.0f);
			mm = glm::translate(mm, glm::vec3(x, SUELO_Y, z));
			mm = glm::rotate(mm, glm::radians(yawDeg), glm::vec3(0.0f, 1.0f, 0.0f));
			mm = glm::scale(mm, glm::vec3(s));
			mm = glm::translate(mm, glm::vec3(-c.x, -mn.y, -c.z));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mm));
			m.Draw(lightingShader);
			};

		// ESTRUCTURA DEL CUARTO
		glm::vec3 cuartoMin = cuarto.GetBoundsMin();
		glm::vec3 cuartoMax = cuarto.GetBoundsMax();
		glm::vec3 cuartoCentro = (cuartoMin + cuartoMax) * 0.5f;

		glm::mat4 cuartoModel(1.0f);

		cuartoModel = glm::translate(cuartoModel, glm::vec3(0.0f, CUARTO_Y, 0.0f));
		cuartoModel = glm::rotate(cuartoModel, glm::radians(CUARTO_GIRO), glm::vec3(0.0f, 1.0f, 0.0f));

		cuartoModel = glm::scale(cuartoModel, glm::vec3(
			CUARTO_ESCALA_X,
			CUARTO_ESCALA_Y,
			CUARTO_ESCALA_Z
		));

		cuartoModel = glm::translate(cuartoModel, glm::vec3(
			-cuartoCentro.x,
			-cuartoMin.y,
			-cuartoCentro.z
		));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cuartoModel));
		cuarto.Draw(lightingShader);

		// Plano del suelo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(52.0f, 0.1f, 52.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glVertexAttrib3f(3, 1.0f, 1.0f, 1.0f);
		glVertexAttrib2f(2, 0.0f, 0.0f);

		GLint diffuseColorLoc = glGetUniformLocation(lightingShader.Program, "material.diffuseColor");
		GLint hasDiffMapLoc = glGetUniformLocation(lightingShader.Program, "material.hasDiffuseMap");
		glUniform3f(diffuseColorLoc, 0.11f, 0.11f, 0.13f);
		glUniform1i(hasDiffMapLoc, 0);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.hasSpecularMap"), 0);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specularColor"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "material.emissive"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.opacity"), 1.0f);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.alphaTest"), 0);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// CANCHA DE BASQUETBOL
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.06f, 0.0f));
		model = glm::scale(model, glm::vec3(0.064f));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cancha.Draw(lightingShader);
		glDisable(GL_BLEND);

		// ÁRBITRO Y JUGADORE
		DibujarPersonaje(arbitroP, lightingShader, modelLoc);
		DibujarPersonaje(jugadorP, lightingShader, modelLoc, modoCamara == CAMARA_PRIMERA);

		for (int i = 0; i < 5; i++){
			DibujarPersonaje(bulls[i], lightingShader, modelLoc);
			DibujarPersonaje(lakers[i], lightingShader, modelLoc);
		}

		// BALON DE BASQUETBOL
		glm::vec3 balonMin = balon.GetBoundsMin();
		glm::vec3 balonMax = balon.GetBoundsMax();
		glm::vec3 balonCentro = (balonMin + balonMax) * 0.5f;

		glm::mat4 balonModel(1.0f);

		// Posicion generada por los keyframes
		balonModel = glm::translate(balonModel, balonPos);

		// Rotacion del balon durante el tiro
		balonModel = glm::rotate( balonModel, glm::radians(balonRot), glm::vec3(1.0f, 0.0f, 0.0f));

		// Escala original
		balonModel = glm::scale( balonModel, glm::vec3(BALON_ESCALA));

		// Centrar el modelo sobre su origen
		balonModel = glm::translate(balonModel, -balonCentro);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(balonModel));

		balon.Draw(lightingShader);

		// MARCADOR ELECTRÓNICO
		glm::mat4 marcadorModel(1.0f);
		marcadorModel = glm::translate(marcadorModel, glm::vec3(2.6f, 10.0f, -2.9f));
		marcadorModel = glm::scale(marcadorModel, glm::vec3(MARCADOR_ESCALA));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(marcadorModel));
		marcador.Draw(lightingShader);

		// --- REFLECTORES
		glm::vec3 minB = marcador.GetBoundsMin();
		glm::vec3 maxB = marcador.GetBoundsMax();

		float margen = 0.8f;
		float ajusteLateral = 1.0f;

		glm::vec3 esquinasLocales[] = {
			glm::vec3(minB.x * margen, minB.y, maxB.z * margen - ajusteLateral),
			glm::vec3(minB.x * margen, minB.y, minB.z * margen),
			glm::vec3(maxB.x * margen, minB.y, maxB.z * margen + ajusteLateral),
			glm::vec3(maxB.x * margen, minB.y, minB.z * margen)
		};

		const float ESCALA_REFLECTOR_LOCAL = 0.05f;

		glm::vec3 coloresReflectores[4] = {
			glm::vec3(1.0f, 0.05f, 0.2f), 
			glm::vec3(0.0f, 0.75f, 1.0f), 
			glm::vec3(0.1f, 1.0f, 0.2f),
			glm::vec3(1.0f, 0.65f, 0.0f)  
		};

		for (int i = 0; i < 4; i++) {
			float yawAnim = 0.0f;
			float pitchAnim = 0.0f;
			glm::vec3 colorLuz;
			float multiplicadorBrillo;

			if (reflectoresEncendidos) {
				//MODO SHOW
				float fase = i * 1.5707963f;
				yawAnim = 35.0f * std::sin(currentFrame * 1.5f + fase);
				pitchAnim = 15.0f * std::sin(currentFrame * 3.0f + fase);

				colorLuz = coloresReflectores[i];
				multiplicadorBrillo = 3.5f; // Potencia el color
			}
			else {
				// MODO REPOSO
				colorLuz = glm::vec3(1.0f, 1.0f, 1.0f);
				multiplicadorBrillo = 2.0f;
			}

			glm::mat4 spotModel = marcadorModel;
			spotModel = glm::translate(spotModel, esquinasLocales[i]);

			float yawReflector = 45.0f + (i * 90.0f);
			spotModel = glm::rotate(spotModel, glm::radians(yawReflector + yawAnim), glm::vec3(0.0f, 1.0f, 0.0f));
			spotModel = glm::rotate(spotModel, glm::radians(25.0f + pitchAnim), glm::vec3(1.0f, 0.0f, 0.0f));
			spotModel = glm::scale(spotModel, glm::vec3(ESCALA_REFLECTOR_LOCAL));

			
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(spotModel));
			spotlight.Draw(lightingShader);

			glm::vec3 spotPos = glm::vec3(spotModel * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			glm::vec3 spotDir = glm::normalize(glm::mat3(spotModel) * glm::vec3(0.0f, -0.6f, 1.0f));

			std::string base = "spotLights[" + std::to_string(i) + "].";
			SetVec3(lightingShader.Program, base + "position", spotPos.x, spotPos.y, spotPos.z);
			SetVec3(lightingShader.Program, base + "direction", spotDir.x, spotDir.y, spotDir.z);
			SetVec3(lightingShader.Program, base + "ambient", 0.04f * colorLuz.r, 0.04f * colorLuz.g, 0.04f * colorLuz.b);
			SetVec3(lightingShader.Program, base + "diffuse", colorLuz.r * multiplicadorBrillo, colorLuz.g * multiplicadorBrillo, colorLuz.b * multiplicadorBrillo);
			SetVec3(lightingShader.Program, base + "specular", colorLuz.r * multiplicadorBrillo, colorLuz.g * multiplicadorBrillo, colorLuz.b * multiplicadorBrillo);

			SetFloat(lightingShader.Program, base + "constant", 1.0f);
			SetFloat(lightingShader.Program, base + "linear", 0.014f);
			SetFloat(lightingShader.Program, base + "quadratic", 0.0007f);
			SetFloat(lightingShader.Program, base + "cutOff", glm::cos(glm::radians(20.0f)));
			SetFloat(lightingShader.Program, base + "outerCutOff", glm::cos(glm::radians(30.0f)));
		}

		// BANCAS DE EQUIPOS Y MADERA
		float espaciadoBancas = 3.2f;

		// --- LADO IZQUIERDO 
		dibujarSobreSuelo(bancaConJugador1, -BANCAS_DIST, (-2.0f * espaciadoBancas), yawAlineado(bancaConJugador1), escalaBancaJugador1);
		dibujarSobreSuelo(bancaConJugador1, -BANCAS_DIST, (-1.0f * espaciadoBancas), yawAlineado(bancaConJugador1), escalaBancaJugador1);
		dibujarSobreSuelo(bancaMadera, -BANCAS_DIST, (0.0f * espaciadoBancas), yawAlineado(bancaMadera), escalaBancaMadera);
		dibujarSobreSuelo(bancaConJugador1, -BANCAS_DIST, (1.0f * espaciadoBancas), yawAlineado(bancaConJugador1), escalaBancaJugador1);
		dibujarSobreSuelo(bancaMadera, -BANCAS_DIST, (2.0f * espaciadoBancas), yawAlineado(bancaMadera), escalaBancaMadera);

		// --- LADO DERECHO
		float yawDerechaEquipo2 = yawAlineado(bancaConJugador2) + 180.0f;
		dibujarSobreSuelo(bancaConJugador2, BANCAS_DIST, (-2.0f * espaciadoBancas), yawDerechaEquipo2, escalaBancaJugador2);
		dibujarSobreSuelo(bancaConJugador2, BANCAS_DIST, (-1.0f * espaciadoBancas), yawDerechaEquipo2, escalaBancaJugador2);
		dibujarSobreSuelo(bancaMadera, BANCAS_DIST, (0.0f * espaciadoBancas), yawAlineado(bancaMadera), escalaBancaMadera);
		dibujarSobreSuelo(bancaConJugador2, BANCAS_DIST, (1.0f * espaciadoBancas), yawDerechaEquipo2, escalaBancaJugador2);
		dibujarSobreSuelo(bancaMadera, BANCAS_DIST, (2.0f * espaciadoBancas), yawAlineado(bancaMadera), escalaBancaMadera);

		// BARRERAS DE PROTECCIÓN
		for (int i = 0; i < 4; i++)
		{
			for (int k = -BARRERA_MITAD; k <= BARRERA_MITAD; k++)
			{
				float t = k * BARRERA_PASO;
				glm::vec3 p = arenaSides[i].n * BARRERA_DIST + arenaSides[i].t * t;
				model = glm::mat4(1.0f);
				model = glm::translate(model, p);
				model = glm::rotate(model, glm::radians(arenaSides[i].yawToRing), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(0.625f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				barrera.Draw(lightingShader);
			}
		}

		// GRADAS Y ESPECTADORES DISTRIBUIDOS EN LOS ASIENTOS
		int sectionCount = 0;
		for (int i = 0; i < 4; i++)
		{
			for (int k = 0; k < 2; k++)
			{
				float offsetT = (k == 0 ? -1.0f : 1.0f) * GRADAS_SECCION;
				glm::vec3 p = arenaSides[i].n * GRADAS_FRENTE + arenaSides[i].t * offsetT;

				glm::mat4 gradaModel = glm::mat4(1.0f);
				gradaModel = glm::translate(gradaModel, p);
				gradaModel = glm::rotate(gradaModel, glm::radians(arenaSides[i].yawToRing + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				gradaModel = glm::scale(gradaModel, glm::vec3(1.0f / 6.0f));

				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(gradaModel));
				gradas.Draw(lightingShader);

				// Dibujar espectadores de manera lógica en cada peldaño/fila de las gradas
				for (int tier = 0; tier < 6; tier++)
				{
					for (int s = 0; s < POR_FILA; s++)
					{
						if (Hash01(sectionCount, tier, s) > OCUPACION_GRADAS) continue;
						float posX = (s - (POR_FILA - 1) * 0.5f) * PASO - 0.82f;
						glm::vec3 localSeatPos(posX, GRADAS_ALTO[tier] - ASIENTO, GRADAS_BORDE[tier] + MEDIO_ASIENTO);
						glm::vec3 worldSeatPos = glm::vec3(gradaModel * glm::vec4(localSeatPos, 1.0f));

						int specIndex = (int)(Hash01(sectionCount, tier, s + 17) * 6.0f) % 6;
						Model* currentSpec = GetSpectatorModel(specIndex, espectador_A, espectador_B, espectador_C, espectador_D, espectador_E, espectador_F);

						glm::mat4 specModel = glm::mat4(1.0f);
						specModel = glm::translate(specModel, worldSeatPos);
						specModel = glm::rotate(specModel, glm::radians(arenaSides[i].yawToRing + ESPECTADOR_GIRO), glm::vec3(0.0f, 1.0f, 0.0f));
						specModel = glm::scale(specModel, glm::vec3(1.0f));

						glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(specModel));
						currentSpec->Draw(lightingShader);
					}
				}
				sectionCount++;
			}
		}

		// MESAS Y SILLAS
		dibujarSobreSuelo(mesas, -MESAS_DIST, MESAS_Z, yawMesas, 0.7f);
		for (int i = 0; i < SILLAS_CANTIDAD; i++)
		{
			float sz = MESAS_Z + (i - (SILLAS_CANTIDAD - 1) * 0.5f) * SILLA_PASO;
			dibujarSobreSuelo(silla, sillasX, sz, sillasYaw, SILLA_ESCALA);
		}

		// BOCINAS Y CÁMARAS DE TRANSMISIÓN
		for (int sx = -1; sx <= 1; sx += 2)
		{
			for (int sz = -1; sz <= 1; sz += 2)
			{
				float bx = sx * BOCINA_DIST;
				float bz = sz * BOCINA_DIST;
				float yaw = glm::degrees(std::atan2(-bx, -bz)) + BOCINA_GIRO;
				dibujarSobreSuelo(bocina, bx, bz, yaw, BOCINA_ESCALA);
			}
		}

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 9.2f));
		model = glm::rotate(model, glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		camaraObj.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(9.0f, 0.0f, 4.0f));
		model = glm::rotate(model, glm::radians(240.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		camaraBroadcast.Draw(lightingShader);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void UserInputs()
{
	// CAMARA LIBRE
	if (modoCamara == CAMARA_LIBRE)
	{
		if (keys[GLFW_KEY_W])
			camera.ProcessKeyboard(FORWARD, deltaTime);

		if (keys[GLFW_KEY_S])
			camera.ProcessKeyboard(BACKWARD, deltaTime);

		if (keys[GLFW_KEY_A])
			camera.ProcessKeyboard(LEFT, deltaTime);

		if (keys[GLFW_KEY_D])
			camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	// BLOQUEO DURANTE EL TIRO. Evita que la animacion de caminata se mezcle con los keyframes
	if (tiroActivo)
	{
		jugadorP.amplitud = 0.0f;
		return;
	}

	// GIRO DEL JUGADOR
	if (keys[GLFW_KEY_LEFT])
	{
		jugadorRumbo -= JUGADOR_GIRO_VEL * deltaTime;
	}

	if (keys[GLFW_KEY_RIGHT])
	{
		jugadorRumbo += JUGADOR_GIRO_VEL * deltaTime;
	}


	glm::vec3 frente =
		FrenteJugador();


	// MOVIMIENTO DEL JUGADOR
	float avance = 0.0f;

	if (keys[GLFW_KEY_UP])
	{
		avance += 1.0f;
	}

	if (keys[GLFW_KEY_DOWN])
	{
		avance -= 1.0f;
	}


	if (avance != 0.0f)
	{
		jugadorP.posicion +=
			frente *
			avance *
			JUGADOR_VELOCIDAD *
			deltaTime;


		jugadorP.posicion.x =
			glm::clamp(
				jugadorP.posicion.x,
				-JUGADOR_LIMITE,
				JUGADOR_LIMITE
			);


		jugadorP.posicion.z =
			glm::clamp(
				jugadorP.posicion.z,
				-JUGADOR_LIMITE,
				JUGADOR_LIMITE
			);


		// Activar balanceo solamente al caminar
		jugadorP.amplitud = 0.5f;
	}
	else
	{
		jugadorP.amplitud = 0.0f;
	}


	// Actualizar orientacion del modelo
	jugadorP.yaw =
		-jugadorRumbo +
		JUGADOR_YAW_AJUSTE;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) keys[key] = true;
		else if (action == GLFW_RELEASE) keys[key] = false;
	}

	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		reflectoresEncendidos = !reflectoresEncendidos;
	}

	if (action == GLFW_PRESS){
		// Modos de camara
		if (key == GLFW_KEY_1) modoCamara = CAMARA_LIBRE;
		if (key == GLFW_KEY_2) modoCamara = CAMARA_PRIMERA;
		if (key == GLFW_KEY_3) modoCamara = CAMARA_TERCERA;

		// Animacion por keyframes
		if (key == GLFW_KEY_SPACE)
		{
			IniciarTiroLibre();
		}

	}

	if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		PlaySound(TEXT("sonidos/silbato.wav"), NULL, SND_ASYNC | SND_FILENAME);
	}
}

void Animation()
{
	if (!tiroActivo)
		return;
	// Garantizar que la caminata JAMAS se mezcle
	jugadorP.amplitud = 0.0f;

	tiempoTiro += deltaTime;

	// BUSCAR SEGMENTO DE KEYFRAMES
	while (indiceTiro < NUM_KEYFRAMES_TIRO - 2 && tiempoTiro > keyframesTiro[indiceTiro + 1].tiempo)
	{
		indiceTiro++;
	}

	const KeyFrameTiro& a = keyframesTiro[indiceTiro];

	const KeyFrameTiro& b = keyframesTiro[indiceTiro + 1];

	float duracion = b.tiempo - a.tiempo;

	float tPose = 0.0f;

	if (duracion > 0.0f)
	{
		tPose = (tiempoTiro - a.tiempo) / duracion;
	}


	tPose = glm::clamp(tPose, 0.0f, 1.0f);


	// Smoothstep manual
	// evita cambios bruscos entre keyframes
	tPose = tPose * tPose * (3.0f - 2.0f * tPose);


	// POSE DEL JUGADOR
	jugadorP.pose.x[TORSO] = glm::mix(a.torsoX, b.torsoX, tPose);


	// Bíceps
	jugadorP.pose.z[BICEP_DER] = glm::mix(a.bicepDerZ, b.bicepDerZ, tPose);
	jugadorP.pose.z[BICEP_IZQ] = glm::mix(a.bicepIzqZ, b.bicepIzqZ, tPose);

	// Antebrazos
	jugadorP.pose.z[ANTEBRAZO_DER] = glm::mix( a.antebrazoDerZ, b.antebrazoDerZ, tPose);
	jugadorP.pose.z[ANTEBRAZO_IZQ] = glm::mix(a.antebrazoIzqZ, b.antebrazoIzqZ, tPose);

	// Manos
	jugadorP.pose.z[MANO_DER] = glm::mix(a.manoDerZ, b.manoDerZ, tPose);
	jugadorP.pose.z[MANO_IZQ] = glm::mix(a.manoIzqZ, b.manoIzqZ, tPose);

	// BALON: CUERPO -> CABEZA
	if (tiempoTiro < 0.65f)
	{
		float t = tiempoTiro / 0.65f;
		t = glm::clamp(t, 0.0f, 1.0f);

		// Suavizar
		t = t * t * (3.0f - 2.0f * t);

		balonPos = glm::mix(BALON_INICIAL,	BALON_CABEZA, t);
	}

	// CABEZA -> LIBERACION
	else if (tiempoTiro < TIEMPO_LIBERACION)
	{
		float t = (tiempoTiro - 0.65f) / (TIEMPO_LIBERACION - 0.65f);

		t = glm::clamp(t, 0.0f,1.0f);

		balonPos = glm::mix(BALON_CABEZA, BALON_LIBERACION, t);
	}

// VUELO PARABOLICO DIRECTO AL ARO
	else if (tiempoTiro < TIEMPO_ARO)
	{
		float t = (tiempoTiro - TIEMPO_LIBERACION) / (TIEMPO_ARO - TIEMPO_LIBERACION);

		t = glm::clamp(t, 0.0f, 1.0f);

		// Movimiento directo desde la mano hasta el aro
		balonPos = glm::mix( BALON_LIBERACION, POS_ARO_TIRO, t);

		// Arco parabolico
		float arco = 4.0f * ALTURA_ARCO * t * (1.0f - t);

		balonPos.y += arco;

		balonRot += 300.0f * deltaTime;
	}

	// BALON DESPUES DE ENCESTAR

	else
	{
		float t = (tiempoTiro - TIEMPO_ARO) / (TIEMPO_FIN - TIEMPO_ARO);

		t = glm::clamp(	t, 0.0f, 1.0f);

		glm::vec3 debajoAro = POS_ARO_TIRO + glm::vec3(0.0f, -1.80f, 0.0f);

		balonPos = glm::mix( POS_ARO_TIRO, debajoAro, t);

		balonRot += 300.0f * deltaTime;
	}

	// FINAL DE ANIMACION
	if (tiempoTiro >= TIEMPO_FIN)
	{
		tiroActivo = false;

		tiempoTiro = 0.0f;
		indiceTiro = 0;

		// Pose normal
		jugadorP.pose = Pose();

		// No volver a activar caminata automáticamente
		jugadorP.amplitud = 0.0f;


		std::cout
			<< "[ANIMACION] Tiro libre terminado"
			<< std::endl;
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	if (modoCamara == CAMARA_LIBRE)
		camera.ProcessMouseMovement(xOffset, yOffset);
}