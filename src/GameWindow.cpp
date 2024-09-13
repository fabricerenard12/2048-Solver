// GameWindow class for the QT6 view
// Author: Fabrice Renard
// Date : 23 / 06 / 2023

#include "GameWindow.hpp"

GameWindow::GameWindow(QWidget* parent, std::shared_ptr<Game> game, bool autoplay)
    : QWidget(parent), game_(game), gridLabels(std::vector<std::vector<QLabel*>>(GRID_SIZE)) {

    for (std::vector<QLabel*>& row : gridLabels) {
        row.resize(GRID_SIZE);
    }

    setupWindow();
    setupGrid();
    applyStyleSheet();
    setWindowSize();

    if (autoplay) {
        startAutoPlay();
    }
}

void GameWindow::setupWindow() {
    setWindowTitle("2048 Solver");
}

void GameWindow::updateGrid() {
    Grid grid = game_->getGrid();
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
        int bitPosition = i * 4;
        int value = (grid >> bitPosition) & 0xF;
        QLabel* label = gridLabels[i / GRID_SIZE][i % GRID_SIZE];

        if (value == 0) {
            label->setText("");
        } else {
            label->setText(QString::number(1 << (value)));
        }

        setLabelStyle(label, value);
    }

    if (game_->isGameOver()) {
        game_ = std::make_shared<Game>();
        updateGrid();
    }
}

void GameWindow::startAutoPlay() {
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWindow::autoPlayMove);
    timer->start(DELAY);
}

void GameWindow::autoPlayMove() {
    Move bestMove;

    bestMove = performMC(
        *(game_.get()),
        NUMBER_OF_SIMULATIONS_PER_MOVE,
        std::thread::hardware_concurrency()
    );

    emit keyPressed(SPACEBAR_CHAR, bestMove, game_);
    updateGrid();
}


void GameWindow::keyPressEvent(QKeyEvent* event) {
    Move bestMove;

    if (event->key() == SPACEBAR_CHAR) {
        bestMove = performMC(
            *(game_.get()),
            NUMBER_OF_SIMULATIONS_PER_MOVE,
            std::thread::hardware_concurrency()
        );
    }

    emit keyPressed(event->key(), bestMove, game_);

    QWidget::keyPressEvent(event);
    updateGrid();
}

void GameWindow::setupGrid() {
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(GRID_SPACING);

    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
        QLabel* label = new QLabel(this);
        label->setFrameShape(QFrame::Panel);
        label->setFrameShadow(QFrame::Sunken);
        label->setAlignment(Qt::AlignCenter);

        int row = i / GRID_SIZE;
        int col = i % GRID_SIZE;

        gridLayout->addWidget(label, row, col);
        gridLabels[row][col] = label;
    }

    setLayout(gridLayout);
    updateGrid();
}

void GameWindow::applyStyleSheet() {
    QString styleSheet = "QLabel {"
        "background-color: #aaa;"
        "border-radius: 5px;"
        "font-size: 24px;"
        "}";

    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor("#ccc"));

    setAutoFillBackground(true);
    setPalette(pal);

    setStyleSheet(styleSheet);
}

void GameWindow::setWindowSize() {
    setFixedSize(WINDOW_SIZE, WINDOW_SIZE);
}

void GameWindow::setLabelStyle(QLabel* label, int value) {
    QColor backgroundColor;
    QColor textColor;

    switch (value) {
    case 1:
        backgroundColor = QColor("#eee4da");
        textColor = QColor("#776e65");
        break;
    case 2:
        backgroundColor = QColor("#ede0c8");
        textColor = QColor("#776e65");
        break;
    case 3:
        backgroundColor = QColor("#f2b179");
        textColor = QColor("#f9f6f2");
        break;
    case 4:
        backgroundColor = QColor("#f59563");
        textColor = QColor("#f9f6f2");
        break;
    case 5:
        backgroundColor = QColor("#f67c5f");
        textColor = QColor("#f9f6f2");
        break;
    case 6:
        backgroundColor = QColor("#f65e3b");
        textColor = QColor("#f9f6f2");
        break;
    case 7:
        backgroundColor = QColor("#edcf72");
        textColor = QColor("#f9f6f2");
        break;
    case 8:
        backgroundColor = QColor("#edcc61");
        textColor = QColor("#f9f6f2");
        break;
    case 9:
        backgroundColor = QColor("#edc850");
        textColor = QColor("#f9f6f2");
        break;
    case 10:
        backgroundColor = QColor("#edc53f");
        textColor = QColor("#f9f6f2");
        break;
    case 11:
        backgroundColor = QColor("#edc22e");
        textColor = QColor("#f9f6f2");
        break;
    default:
        backgroundColor = QColor("#aaa");
        textColor = QColor("#776e65");
        break;
    }

    QString styleSheet = QString("QLabel {"
        "background-color: %1;"
        "color: %2;"
        "border-radius: 5px;"
        "font-size: 32px;"
        "font-weight: bold;"
        "}").arg(backgroundColor.name()).arg(textColor.name());

    label->setStyleSheet(styleSheet);
}
