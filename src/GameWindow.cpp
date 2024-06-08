// GameWindow class for the QT6 view
// Author: Fabrice Renard
// Date : 23 / 06 / 2023

#include "GameWindow.hpp"

GameWindow::GameWindow(QWidget* parent, int gridSize, std::shared_ptr<Game> game)
	: QWidget(parent), gridSize_(gridSize), game_(game), gridLabels(std::vector<std::vector<QLabel*>>(gridSize_)) {

	for (std::vector<QLabel*>& row : gridLabels) {
		row.resize(gridSize_);
	}

	setupWindow();
	setupGrid();
	applyStyleSheet();
	setWindowSize();
}

void GameWindow::setupWindow() {
	setWindowTitle("2048 Solver");
};

void GameWindow::updateGrid() {
	// Update QLabels of grid with values from Game grid
	for (int row = 0; row < gridSize_; row++) {
		for (int col = 0; col < gridSize_; col++) {
			int value = game_->getGrid()[row][col];
			QLabel* label = gridLabels[row][col];

			if (value == 0) {
				label->setText("");
			}
			else {
				label->setText(QString::number(value));
			}

			setLabelStyle(label, value);
		}
	}

	// Reset game and grid if game finished
	if (game_->isGameOver()) {
		game_ = std::make_shared<Game>(gridSize_);
		updateGrid();
	}
}

void GameWindow::keyPressEvent(QKeyEvent* event) {
	//Move bestMove = Move::LEFT;
	std::map<double, Move, Compare> bestMoves;

	// Perform Monte Carlo Computation
	if (event->key() == SPACEBAR_CHAR) {
		bestMoves = performMC(*(game_.get()), NUMBER_OF_SIMULATIONS_PER_MOVE);
	}

	emit keyPressed(event->key(), bestMoves, game_);

	QWidget::keyPressEvent(event);
	updateGrid();
}

void GameWindow::setupGrid() {
	QGridLayout* gridLayout = new QGridLayout();
	gridLayout->setSpacing(GRID_SPACING);

	// Initial setup of QLabels grid
	for (int row = 0; row < gridSize_; row++) {
		for (int col = 0; col < gridSize_; col++) {
			QLabel* label = new QLabel(this);
			label->setFrameShape(QFrame::Panel);
			label->setFrameShadow(QFrame::Sunken);
			label->setAlignment(Qt::AlignCenter);

			gridLayout->addWidget(label, row, col);
			gridLabels[row][col] = label;
		}
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

	// Set style according to tile value
	switch (value) {
	case 2:
		backgroundColor = QColor("#eee4da");
		textColor = QColor("#776e65");
		break;
	case 4:
		backgroundColor = QColor("#ede0c8");
		textColor = QColor("#776e65");
		break;
	case 8:
		backgroundColor = QColor("#f2b179");
		textColor = QColor("#f9f6f2");
		break;
	case 16:
		backgroundColor = QColor("#f59563");
		textColor = QColor("#f9f6f2");
		break;
	case 32:
		backgroundColor = QColor("#f67c5f");
		textColor = QColor("#f9f6f2");
		break;
	case 64:
		backgroundColor = QColor("#f65e3b");
		textColor = QColor("#f9f6f2");
		break;
	case 128:
		backgroundColor = QColor("#edcf72");
		textColor = QColor("#f9f6f2");
		break;
	case 256:
		backgroundColor = QColor("#edcc61");
		textColor = QColor("#f9f6f2");
		break;
	case 512:
		backgroundColor = QColor("#edc850");
		textColor = QColor("#f9f6f2");
		break;
	case 1024:
		backgroundColor = QColor("#edc53f");
		textColor = QColor("#f9f6f2");
		break;
	case 2048:
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