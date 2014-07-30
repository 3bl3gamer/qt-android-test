#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QLabel *label;
    QLineEdit *line_edit;
    QPushButton *button;

private slots:
    void save();
};

#endif // MAINWINDOW_H
