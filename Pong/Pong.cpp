#include <cmath>
#include <cstdlib>  // Для генерации случайных чисел
#include <iostream>
#include <string>

#include "glut.h"

// Время начала игры в миллисекундах
int startTime = 0;
// Флаг для отслеживания начала игры
bool gameStarted = false;

// Размеры окна
int width = 800;
int height = 600;

// Скорость игрока
int player_speed = 10;

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

// Переменные для хранения координат кнопок выбора сложности
int easyButtonX = width / 2 - 50;
int easyButtonY = height / 2 + 20;
int easyButtonWidth = 100;
int easyButtonHeight = 20;

int normalButtonX = width / 2 - 50;
int normalButtonY = height / 2 - 10;
int normalButtonWidth = 100;
int normalButtonHeight = 20;

int hardButtonX = width / 2 - 50;
int hardButtonY = height / 2 - 40;
int hardButtonWidth = 100;
int hardButtonHeight = 20;

int impossibleButtonX = width / 2 - 50;
int impossibleButtonY = height / 2 - 70;
int impossibleButtonWidth = 100;
int impossibleButtonHeight = 20;

// Объявляем перечисление для уровня сложности
enum Difficulty { EASY, NORMAL, HARD, IMPOSSIBLE };

int paddle1Direction = 0;  // 0 - стоит, 1 - вверх, -1 - вниз

// Переменная для хранения текущего уровня сложности
Difficulty currentDifficulty =
    NORMAL;  // По умолчанию устанавливаем нормальный уровень

// Прототипы функций
void keyboard(unsigned char key, int x, int y);
void draw();
void update(int value);
void mouse(int button, int state, int x, int y);
void drawMenu();
void keyDown(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);

// Предположим, это функция, которая вызывается при нажатии клавиши
void keyDown(unsigned char key, int x, int y) {
  if (key == 'w') {
    paddle1Direction = 1;  // Вверх
  } else if (key == 's') {
    paddle1Direction = -1;  // Вниз
  }
}

// И обратно к 0, когда клавиша отпущена
void keyUp(unsigned char key, int x, int y) {
  if (key == 'w' || key == 's') {
    paddle1Direction = 0;
  }
}

// Функция для обработки событий клика мыши
void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    // Получаем цвет пикселя в точке нажатия мыши
    unsigned char pixel[3];
    glReadBuffer(GL_FRONT);
    glReadPixels(x, height - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

    // Проверяем цвет пикселя и выбираем уровень сложности
    if (pixel[0] == 255 && pixel[1] == 0 &&
        pixel[2] == 0) {  // Красный цвет (кнопка Impossible)
      currentDifficulty = IMPOSSIBLE;
      ballSpeedX *= 10;
      ballSpeedY *= 10;
      paddle2Speed *= 20;
      player_speed *= 5;
    } else if (pixel[0] == 0 && pixel[1] == 255 &&
               pixel[2] == 0) {  // Зеленый цвет (кнопка Easy)
      currentDifficulty = EASY;
    } else if (pixel[0] == 0 && pixel[1] == 0 &&
               pixel[2] == 255) {  // Синий цвет (кнопка Hard)
      currentDifficulty = HARD;
      ballSpeedX *= 3;
      ballSpeedY *= 3;
      paddle2Speed *= 3;
      player_speed *= 2;
    } else if (pixel[0] == 255 && pixel[1] == 255 &&
               pixel[2] == 0) {  // Желтый цвет (кнопка Normal)
      currentDifficulty = NORMAL;
      ballSpeedX *= 1.5;
      ballSpeedY *= 1.5;
      paddle2Speed *= 1.5;
      player_speed *= 1.5;
    } else {
      // Нажатие вне области кнопок, устанавливаем уровень сложности по
      // умолчанию
      currentDifficulty =
          NORMAL;  // Например, устанавливаем нормальный уровень сложности
    }
    startTime = glutGet(GLUT_ELAPSED_TIME);
    gameStarted = true;

    // Начинаем игру
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(draw);
    glutTimerFunc(16, update, 0);
  }
}

// Функция для отрисовки кнопок выбора сложности
void drawMenu() {
  glClear(GL_COLOR_BUFFER_BIT);

  // Отрисовка кнопок выбора сложности
  glBegin(GL_QUADS);
  // Кнопка Easy
  // (зеленый цвет)
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex2f(easyButtonX, easyButtonY);
  glVertex2f(easyButtonX + easyButtonWidth, easyButtonY);
  glVertex2f(easyButtonX + easyButtonWidth, easyButtonY + easyButtonHeight);
  glVertex2f(easyButtonX, easyButtonY + easyButtonHeight);
  // Кнопка Normal
  // (желтый цвет)
  glColor3f(1.0f, 1.0f, 0.0f);
  glVertex2f(normalButtonX, normalButtonY);
  glVertex2f(normalButtonX + normalButtonWidth, normalButtonY);
  glVertex2f(normalButtonX + normalButtonWidth,
             normalButtonY + normalButtonHeight);
  glVertex2f(normalButtonX, normalButtonY + normalButtonHeight);
  // Кнопка Hard
  // (синий цвет)
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex2f(hardButtonX, hardButtonY);
  glVertex2f(hardButtonX + hardButtonWidth, hardButtonY);
  glVertex2f(hardButtonX + hardButtonWidth, hardButtonY + hardButtonHeight);
  glVertex2f(hardButtonX, hardButtonY + hardButtonHeight);
  // Кнопка Impossible
  // (красный цвет)
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex2f(impossibleButtonX, impossibleButtonY);
  glVertex2f(impossibleButtonX + impossibleButtonWidth, impossibleButtonY);
  glVertex2f(impossibleButtonX + impossibleButtonWidth,
             impossibleButtonY + impossibleButtonHeight);
  glVertex2f(impossibleButtonX, impossibleButtonY + impossibleButtonHeight);
  glEnd();

  // Вывод текста на кнопках
  glColor3f(1.0f, 1.0f, 1.0f);  // белый цвет
  glRasterPos2f(easyButtonX + 20, easyButtonY + 10);
  std::string easyText = "Easy";
  for (char c : easyText) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
  }

  glRasterPos2f(normalButtonX + 20, normalButtonY + 10);
  std::string normalText = "Normal";
  for (char c : normalText) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
  }

  glRasterPos2f(hardButtonX + 20, hardButtonY + 10);
  std::string hardText = "Hard";
  for (char c : hardText) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
  }

  glRasterPos2f(impossibleButtonX + 20, impossibleButtonY + 10);
  std::string impossibleText = "Impossible";
  for (char c : impossibleText) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
  }

  glutSwapBuffers();
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

  if (gameStarted) {
    int elapsedTime = glutGet(GLUT_ELAPSED_TIME) - startTime;
    int seconds = elapsedTime / 1000;  // Преобразование миллисекунд в секунды

    // Отображение таймера
    glColor3f(1.0, 1.0, 1.0);        // белый цвет
    glRasterPos2f(width - 100, 20);  // Позиция на экране
    std::string timerText = "Time: " + std::to_string(seconds) + "s";
    for (char c : timerText) {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
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

  // Проверяем столкновение мяча с ракеткой игрока
  if (ballX < paddleWidth && ballY > paddle1Y &&
      ballY < paddle1Y + paddleHeight) {
    ballSpeedX = -ballSpeedX;
    playerScore++;

    // Меняем направление по Y в зависимости от направления движения платформы
    if (paddle1Direction == 1) {          // Если платформа движется вверх
      ballSpeedY -= 1;                    // Делаем наклон вверх
    } else if (paddle1Direction == -1) {  // Если платформа движется вниз
      ballSpeedY += 1;                    // Делаем наклон вниз
    }
    // Если платформа стоит на месте, не меняем скорость по Y
  }

  // Аналогично для платформы противника
  if (ballX > width - paddleWidth && ballY > paddle2Y &&
      ballY < paddle2Y + paddleHeight) {
    ballSpeedX = -ballSpeedX;
    opponentScore++;

    // Меняем направление по Y в зависимости от движения платформы
    if (paddle2Speed < 0) {
      ballSpeedY -= 3;  // Делаем наклон вверх
    } else if (paddle2Speed > 0) {
      ballSpeedY += 3;  // Делаем наклон вниз
    }
  }

  // Проверяем границы поля и изменяем направление движения шарика при промахе
  if (ballX < 0) {
    // Промах игрока, изменяем направление движения к другому игроку от центра
    // поля
    playerScore = 0;
    ballSpeedX = ballSpeedX > 0 ? ballSpeedX : -ballSpeedX;
    ballSpeedY = 0;           // Шарик движется только по горизонтали
    ballX = width / 2;        // Перемещаем шарик в центр поля
    ballY = rand() % height;  // Случайная высота в пределах игрового поля
  } else if (ballX > width) {
    // Промах оппонента, изменяем направление движения к другому игроку от
    // центра поля
    opponentScore = 0;
    ballSpeedX = ballSpeedX < 0 ? ballSpeedX : -ballSpeedX;
    ballSpeedY = 0;           // Шарик движется только по горизонтали
    ballX = width / 2;        // Перемещаем шарик в центр поля
    ballY = rand() % height;  // Случайная высота в пределах игрового поля
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
      paddle1Y += player_speed;
      break;
    case 's':
      paddle1Y -= player_speed;
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

  // Регистрация функций обработки событий мыши и отрисовки заставочного экрана
  glutMouseFunc(mouse);
  glutDisplayFunc(drawMenu);

  glutMainLoop();
  return 0;
}
