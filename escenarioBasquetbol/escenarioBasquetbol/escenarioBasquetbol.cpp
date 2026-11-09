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
Camera  camera(glm::vec3(0.0f, 5.0f, 15.0f));
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

	// --- Carga de modelos con rutas corregidas ---
	std::cout << "[DEBUG] Cargando modelos del escenario y nuevos elementos..." << std::endl;
	Model arbitro((char*)"modelos/arbitro-ruper-design/source/manga ralgla_cuello en v.fbx");
	Model gradas((char*)"modelos/Gradas/Gradas.obj");
	Model marcador((char*)"modelos/marcador/marcadorBueno.obj");
	Model barrera((char*)"modelos/barrera/barrera.obj");
	Model bocina((char*)"modelos/bocina/bocina.obj");
	Model camaraObj((char*)"modelos/camara/source/camar.fbx");
	Model ceilingLights((char*)"modelos/ceiling-lights/source/PTRCUNWM24VXBNKZ/ASSET.fbx");
	Model mexicanFlag((char*)"modelos/mexican-flag/source/FB SP Mexflag.fbx");
	Model silla((char*)"modelos/silla_metalica/silla_metalica.fbx");
	Model mesas((char*)"modelos/mesas/scene.gltf");
	Model camaraBroadcast((char*)"modelos/set-of-four-4-low-poly-cameras/source/camara_broadcast.obj");

	Model balon((char*)"modelos/balonBaloncesto/BalonBaloncesto5.obj");
	Model bancaConJugador1((char*)"modelos/BancaConJugador/banca_jugador1.obj");
	Model bancaConJugador2((char*)"modelos/BancaConJugador/banca_jugador2.obj");
	Model bancaMadera((char*)"modelos/BancaMadera/banca.obj");

	// Modelos de espectadores (A a F)
	Model espectador_A((char*)"modelos/espectador/espectador_A.obj");
	Model espectador_B((char*)"modelos/espectador/espectador_B.obj");
	Model espectador_C((char*)"modelos/espectador/espectador_C.obj");
	Model espectador_D((char*)"modelos/espectador/espectador_D.obj");
	Model espectador_E((char*)"modelos/espectador/espectador_E.obj");
	Model espectador_F((char*)"modelos/espectador/espectador_F.obj");

	Model celticsTatum((char*)"modelos/jugadores/Celtics/tatum.obj");
	Model lakersGasol((char*)"modelos/jugadores/Lakers/gasol.obj");

	std::cout << "[DEBUG] ¡Todos los modelos cargados con éxito! Iniciando bucle principal..." << std::endl;

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

	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

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

	const float BARRERA_DIST = 6.00f;
	const float BARRERA_PASO = 1.695f;
	const float GRADAS_FRENTE = 11.5f;
	const float GRADAS_SECCION = 3.36f;
	const float MARCADOR_ALTURA = 8.60f;

	// Parámetros geométricos de las gradas para posicionar al público
	const float GRADAS_BORDE[6] = { -14.0f, -9.28f, -4.56f, 0.15f, 4.87f, 9.59f };
	const float GRADAS_ALTO[6] = { 2.52f, 5.22f, 7.91f, 10.61f, 13.3f, 16.0f };
	const float UNIDADES_POR_M = 6.0f;
	const float ASIENTO = 0.46f * UNIDADES_POR_M;
	const float MEDIO_ASIENTO = 0.23f * UNIDADES_POR_M;
	const float PASO = 0.62f * UNIDADES_POR_M;
	const int POR_FILA = 9;
	const float OCUPACION_GRADAS = 0.70f;

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

		// ==========================================================
		// 1. PLANO DE SUELO (44 x 44 m)
		// ==========================================================
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(44.0f, 0.1f, 44.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		GLint diffuseColorLoc = glGetUniformLocation(lightingShader.Program, "material.diffuseColor");
		GLint hasDiffMapLoc = glGetUniformLocation(lightingShader.Program, "material.hasDiffuseMap");
		glUniform3f(diffuseColorLoc, 0.11f, 0.11f, 0.13f);
		glUniform1i(hasDiffMapLoc, 0);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ==========================================================
		// 2. DIBUJAR MODELOS 3D DEL ESCENARIO
		// ==========================================================

		// A. ÁRBITRO Y JUGADORES (Cerca del centro)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.3f, 0.0f, -1.9f));
		model = glm::scale(model, glm::vec3(0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		arbitro.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.4f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		celticsTatum.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.4f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		lakersGasol.Draw(lightingShader);

		// B. BALÓN DE BALONCESTO (Al centro de la cancha)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		balon.Draw(lightingShader);

		// C. MARCADOR ELECTRÓNICO (Suspendido al centro del techo)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, MARCADOR_ALTURA, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f / 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		marcador.Draw(lightingShader);

		// D. BANCAS DE EQUIPOS Y MADERA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.5f, 0.0f, -3.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		bancaConJugador1.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.5f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		bancaConJugador2.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.5f, 0.0f, -3.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		bancaMadera.Draw(lightingShader);

		// E. BARRERAS DE PROTECCIÓN
		for (int i = 0; i < 4; i++)
		{
			for (int k = -3; k <= 3; k++)
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

		// F. GRADAS Y ESPECTADORES DISTRIBUIDOS EN LOS ASIENTOS
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
						specModel = glm::rotate(specModel, glm::radians(arenaSides[i].yawToRing + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
						specModel = glm::scale(specModel, glm::vec3(1.0f));

						glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(specModel));
						currentSpec->Draw(lightingShader);
					}
				}
				sectionCount++;
			}
		}

		// G. MESAS, SILLAS Y ACCESORIOS (Lado -X)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-4.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mesas.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(0.46f / 163.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		silla.Draw(lightingShader);

		// H. BOCINAS Y CÁMARAS DE TRANSMISIÓN
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.6f, 0.0f, 7.6f));
		model = glm::scale(model, glm::vec3(1.0f / 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		bocina.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 4.8f));
		model = glm::scale(model, glm::vec3(0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		camaraObj.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.6f, 0.0f, 4.6f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		camaraBroadcast.Draw(lightingShader);

		// I. ILUMINACIÓN DE TECHO Y BANDERA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 7.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ceilingLights.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 6.0f, 13.0f));
		model = glm::scale(model, glm::vec3(0.005f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mexicanFlag.Draw(lightingShader);

		// ==========================================================
		// 3. RENDERIZAR LA LÁMPARA VISIBLE
		// ==========================================================
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