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
	//Model arbitro(ResolveModel("modelos/arbitro-ruper-design/source/manga ralgla_cuello en v.fbx"));
	Model gradas(ResolveModel("modelos/Gradas/Gradas.obj"));
	Model marcador(ResolveModel("modelos/marcador/marcadorBueno.obj"));
	Model barrera(ResolveModel("modelos/barrera/barrera.obj"));
	Model bocina(ResolveModel("modelos/bocina/bocina.obj"));
	Model camaraObj(ResolveModel("modelos/camara/source/camar.fbx"));
	Model ceilingLights(ResolveModel("modelos/ceiling-lights/source/PTRCUNWM24VXBNKZ/ASSET.fbx"));
	Model silla(ResolveModel("modelos/silla_metalica/silla_metalica.fbx"));
	Model mesas(ResolveModel("modelos/mesas/scene.gltf"));
	Model camaraBroadcast(ResolveModel("modelos/set-of-four-4-low-poly-cameras/source/camara_broadcast.obj"));

	std::cout << "[DEBUG] Cargando jugadores..." << std::endl;
	// Jugadores Celtics



	// Jugadores Lakers


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
	const float BANCA_JUGADOR_GIRO = 0.0f;

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

	// Bucle principal
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		UserInputs();
		Animation();

		glClearColor(0.15f, 0.2f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		lightingShader.Use();

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

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

		glm::mat4 view = camera.GetViewMatrix();

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

		// ÁRBITRO Y JUGADORES (Cerca del centro)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.3f, 0.0f, -1.9f));
		model = glm::scale(model, glm::vec3(0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//arbitro.Draw(lightingShader);


		// BALÓN DE BALONCESTO (Al centro de la cancha)
		dibujarSobreSuelo(balon, BALON_X, BALON_Z, 0.0f, BALON_ESCALA);

		// MARCADOR ELECTRÓNICO (Suspendido al centro del techo)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.6f, 10.0f, -2.9f));
		model = glm::scale(model, glm::vec3(MARCADOR_ESCALA));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		marcador.Draw(lightingShader);

		// BANCAS DE EQUIPOS Y MADERA
		float espaciadoBancas = 3.2f;

		// --- LADO IZQUIERDO 
		dibujarSobreSuelo(bancaConJugador1, -BANCAS_DIST, (-2.0f * espaciadoBancas), yawAlineado(bancaConJugador1) + BANCA_JUGADOR_GIRO, escalaBancaJugador1);
		dibujarSobreSuelo(bancaConJugador1, -BANCAS_DIST, (-1.0f * espaciadoBancas), yawAlineado(bancaConJugador1) + BANCA_JUGADOR_GIRO, escalaBancaJugador1);
		dibujarSobreSuelo(bancaMadera, -BANCAS_DIST, (0.0f * espaciadoBancas), yawAlineado(bancaMadera), escalaBancaMadera); 
		dibujarSobreSuelo(bancaConJugador1, -BANCAS_DIST, (1.0f * espaciadoBancas), yawAlineado(bancaConJugador1) + BANCA_JUGADOR_GIRO, escalaBancaJugador1);
		dibujarSobreSuelo(bancaMadera, -BANCAS_DIST, (2.0f * espaciadoBancas), yawAlineado(bancaMadera), escalaBancaMadera); 

		// --- LADO DERECHO
		float yawDerechaEquipo2 = yawAlineado(bancaConJugador2) + 180.0f + BANCA_JUGADOR_GIRO;
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

		// Iluminación de techo
		/*model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 7.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ceilingLights.Draw(lightingShader);*/



		lampShader.Use();
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void UserInputs()
{
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])  camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])  camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);
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
}

void Animation() {}

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

	camera.ProcessMouseMovement(xOffset, yOffset);
}