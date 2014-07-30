#include "mainwindow.h"
#include <QSettings>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QSettings settings;
    auto str = settings.value("key", "Hello World!").toString();

    auto mainLayout = new QVBoxLayout();

    label = new QLabel(str);
    mainLayout->addWidget(label);

    line_edit = new QLineEdit();
    mainLayout->addWidget(line_edit);

    button = new QPushButton("Save");
    mainLayout->addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(save()));

    auto w = new QWidget();
    w->setLayout(mainLayout);
    setCentralWidget(w);
}

void MainWindow::save()
{
    QSettings settings;
    settings.setValue("key", line_edit->text());
}

MainWindow::~MainWindow()
{

}
