#include <cmath>

#include "glut.h"

// Размеры окна
int width = 800;
int height = 600;

// Позиция и скорость мяча
float ballX = width / 2;
float ballY = height / 2;
float ballSpeedX = 2.5f;
float ballSpeedY = 1.5f;
float ballRadius = 10.0f;

// Размеры ракеток
float paddleWidth = 10.0f;
float paddleHeight = 80.0f;

// Позиции ракеток
float paddle1Y = height / 2 - paddleHeight / 2;
float paddle2Y = height / 2 - paddleHeight / 2;

// Функция для отрисовки
void draw() {
  glClear(GL_COLOR_BUFFER_BIT);

  // Рисуем мяч
  glColor3f(1.0, 1.0, 1.0);  // белый цвет
  glBegin(GL_TRIANGLE_FAN);
  for (float angle = 0; angle <= 360; angle += 10) {
    float x = ballX + ballRadius * cos(angle * 3.14159 / 180);
    float y = ballY + ballRadius * sin(angle * 3.14159 / 180);
    glVertex2f(x, y);
  }
  glEnd();

  // Рисуем ракетки
  glBegin(GL_QUADS);
  glVertex2f(0, paddle1Y);
  glVertex2f(paddleWidth, paddle1Y);
  glVertex2f(paddleWidth, paddle1Y + paddleHeight);
  glVertex2f(0, paddle1Y + paddleHeight);

  glVertex2f(width - paddleWidth, paddle2Y);
  glVertex2f(width, paddle2Y);
  glVertex2f(width, paddle2Y + paddleHeight);
  glVertex2f(width - paddleWidth, paddle2Y + paddleHeight);
  glEnd();

  glutSwapBuffers();
}

// Функция для обновления состояния игры
void update(int value) {
  // Двигаем мяч
  ballX += ballSpeedX;
  ballY += ballSpeedY;

  // Проверяем столкновение мяча со стенами
  if (ballY > height - ballRadius || ballY < ballRadius) {
    ballSpeedY = -ballSpeedY;
  }

  // Проверяем столкновение мяча с ракетками
  if (ballX < paddleWidth && ballY > paddle1Y &&
      ballY < paddle1Y + paddleHeight) {
    ballSpeedX = -ballSpeedX;
  }
  if (ballX > width - paddleWidth && ballY > paddle2Y &&
      ballY < paddle2Y + paddleHeight) {
    ballSpeedX = -ballSpeedX;
  }

  // Проверяем границы
  if (ballX < 0 || ballX > width) {
    ballX = width / 2;
    ballY = height / 2;
  }

  // Перерисовываем окно
  glutPostRedisplay();

  // Запускаем обновление снова через 16 миллисекунд
  glutTimerFunc(16, update, 0);
}

// Функция для обработки нажатий клавиш
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'w':
      paddle1Y += 10;
      break;
    case 's':
      paddle1Y -= 10;
      break;
    case 'i':
      paddle2Y += 10;
      break;
    case 'k':
      paddle2Y -= 10;
      break;
    default:
      break;
  }
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(width, height);
  glutCreateWindow("Pong");
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, width, 0, height);
  glutDisplayFunc(draw);
  glutTimerFunc(16, update, 0);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
