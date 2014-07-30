#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

struct ProcInfo;
struct CPUInfo;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    CPUInfo getCpuInfo();
    ProcInfo getProcInfo(int pid);
    ~MainWindow();

    QLabel *label;
    QLineEdit *line_edit;
    QPushButton *button;

private slots:
    void checkpoint();
};

#endif // MAINWINDOW_H
