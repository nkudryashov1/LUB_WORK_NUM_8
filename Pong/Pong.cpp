#include <cmath>
#include <iostream>
#include <string>

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

// Скорость движения платформы противника
float paddle2Speed = 2.0f;

// Переменные для счета очков
int playerScore = 0;
int opponentScore = 0;

// Функция для отрисовки счетчиков
void drawScores() {
  // Рисуем счетчики
  glColor3f(1.0, 1.0, 1.0);  // белый цвет
  glRasterPos2f(width / 2 - 50, height - 20);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0' + playerScore);

  glRasterPos2f(width / 2 + 20, height - 20);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '-');

  glRasterPos2f(width / 2 + 40, height - 20);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0' + opponentScore);
}

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

  // Выводим счет
  glColor3f(1.0, 1.0, 1.0);  // белый цвет
  glRasterPos2f(width / 2 - 20, height - 20);
  std::string playerScoreStr = std::to_string(playerScore);
  std::string opponentScoreStr = std::to_string(opponentScore);
  for (char c : playerScoreStr) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
  }
  glRasterPos2f(width / 2 + 20, height - 20);
  for (char c : opponentScoreStr) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
  }

  glutSwapBuffers();
}

// Функция для обновления состояния игры
void update(int value) {
  // Двигаем мяч
  ballX += ballSpeedX;
  ballY += ballSpeedY;

  // Проверяем, не выходит ли платформа игрока за пределы игрового поля
  if (paddle1Y < 0) {
    paddle1Y = 0;
  } else if (paddle1Y + paddleHeight > height) {
    paddle1Y = height - paddleHeight;
  }

  // Проверяем столкновение мяча со стенами
  if (ballY > height - ballRadius || ballY < ballRadius) {
    ballSpeedY = -ballSpeedY;
  }

  // Проверяем столкновение мяча с ракетками
  if (ballX < paddleWidth && ballY > paddle1Y &&
      ballY < paddle1Y + paddleHeight) {
    ballSpeedX = -ballSpeedX;
    playerScore++;
  }
  if (ballX > width - paddleWidth && ballY > paddle2Y &&
      ballY < paddle2Y + paddleHeight) {
    ballSpeedX = -ballSpeedX;
    opponentScore++;
  }

  // Проверяем границы
  if (ballX < 0) {
    // Мяч пересек левую границу, обнуляем очко игроку
    playerScore = 0;
    // Возвращаем мяч в центр
    ballX = width / 2;
    ballY = height / 2;
  } else if (ballX > width) {
    // Мяч пересек правую границу, обнуляем очко противнику
    opponentScore = 0;
    // Возвращаем мяч в центр
    ballX = width / 2;
    ballY = height / 2;
  }

  // Обновляем позицию платформы противника
  paddle2Y += paddle2Speed;

  // Проверяем, не выходит ли платформа противника за пределы игрового поля
  if (paddle2Y < 0) {
    paddle2Y = 0;
    paddle2Speed =
        -paddle2Speed;  // Изменяем знак скорости, чтобы двигаться вниз
  } else if (paddle2Y + paddleHeight > height) {
    paddle2Y = height - paddleHeight;
    paddle2Speed =
        -paddle2Speed;  // Изменяем знак скорости, чтобы двигаться вверх
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
