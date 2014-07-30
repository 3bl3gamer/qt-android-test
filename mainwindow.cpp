#include "mainwindow.h"
#include <QSettings>
#include <QVBoxLayout>
#include <QWidget>
#include <QDir>
#include <QScrollArea>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <unistd.h>

struct CPUInfo
{
    int ticks_total;
    int ticks_idle;

    CPUInfo(int total, int idle)
        : ticks_total(total), ticks_idle(idle) {}

    bool isValid()
    {
        return ticks_total >= 0 && ticks_idle >= 0;
    }
};

struct ProcInfo
{
    int pid;
    QString name;
    int time;
    int rss;

    ProcInfo(int pid, QString name, int time, int rss)
        : pid(pid), name(name), time(time), rss(rss) {}

    bool isValid()
    {
        return pid > 0 && time >= 0 && rss >= 0;
    }
};

class IntSortTableWidgetItem : public QTableWidgetItem {
    public:
        IntSortTableWidgetItem(int val)
            : QTableWidgetItem(QString::number(val)) {}
        bool operator <(const QTableWidgetItem &other) const
        {
            return text().toInt() < other.text().toInt();
        }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //QSettings settings;
    //auto str = settings.value("key", "Hello World!").toString();

    auto mainLayout = new QVBoxLayout();

    auto dir = new QDir("/proc");

    auto t = new QTableWidget();
    t->setColumnCount(6);
    QStringList labels;
    labels << "PID" << "Name" << "RSS MiB" << "Ticks" << "%all" << "%CPU";
    t->setHorizontalHeaderLabels(labels);
    t->setSortingEnabled(true);


    auto cpu_info = getCpuInfo();
    for(auto dirname : dir->entryList(QDir::Dirs))
    {
       int pid = dirname.toInt();
       if (pid == 0) continue;
       auto info = getProcInfo(pid);

       int row = t->rowCount();
       t->insertRow(row);
       QTableWidgetItem *i;

       i = new IntSortTableWidgetItem(pid);
       t->setItem(row, 0, i);

       i = new QTableWidgetItem(info.name);
       t->setItem(row, 1, i);

       i = new IntSortTableWidgetItem(info.rss/1024/1024);
       t->setItem(row, 2, i);

       i = new IntSortTableWidgetItem(info.time);
       t->setItem(row, 3, i);

       i = new IntSortTableWidgetItem(info.time*100/(cpu_info.ticks_total - cpu_info.ticks_idle));
       t->setItem(row, 4, i);

       i = new IntSortTableWidgetItem(info.time*100/cpu_info.ticks_total);
       t->setItem(row, 5, i);
    }

    mainLayout->addWidget(t);

    button = new QPushButton("Checkpoint");
    mainLayout->addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(checkpoint()));

    auto w = new QWidget();
    w->setLayout(mainLayout);
    setCentralWidget(w);
}

CPUInfo MainWindow::getCpuInfo()
{
    QFile file("/proc/stat");
    if (!file.open(QIODevice::ReadOnly))
    {
        return CPUInfo(-1, -1);
    }

    auto data = file.readAll();
    int summ=0, idle=0, id=1, val=0, pos=0;

    while (data[pos] != ' ') {pos++;} //skip "cpu"
    while (data[pos] == ' ') {pos++;} //skip spaces after
    pos--;

    while (data[pos++] != (char)10)
    {
        int v = data[pos];
        if (v < (char)48) //32 or 10
        {
            summ += val;
            if (id == 4) idle = val;
            val = 0;
            id++;
        }
        else val = val*10+v-48;
    }

    file.close();

    return CPUInfo(summ, idle);
}

//http://man7.org/linux/man-pages/man5/proc.5.html - /proc/[pid]/stat
ProcInfo MainWindow::getProcInfo(int pid)
{
    QFile file("/proc/"+QString::number(pid)+"/stat");
    if (!file.open(QIODevice::ReadOnly))
    {
        return ProcInfo(pid, "", -1, -1);
    }

    auto data = file.readAll();
    int pos=0, count;

    //skipping pid, pointing to '(' before name
    while (data[pos++] != ' ') {}
    int name_from = pos + 1;

    pos += 15 + 1;
    //pointing to last char of name, it's before ')'
    while (data[pos--] != ')') {}
    QString name(data.mid(name_from, pos-name_from+1));

    pos += 3;
    count = 3;

    //Amount of time that this process has been
    //scheduled in user mode, measured in clock ticks
    while (count < 14) //utime (14), stime (15), cutime (16), cstime (17)
        if (data[pos++] == ' ') count++;

    int summ=0, val=0, v;
    //utime stime cutime cstime
    while (count < 18)
    {
        v = data[pos++];
        if (v == 32) {summ+=val; val=0; count++;}
        else val = val*10 + v-48;
    }

    while (count < 24) //rss (24)
        if (data[pos++] == ' ') count++;

    int rss_pages = 0;
    while ((v=data[pos++]) != 32)
    rss_pages = rss_pages*10 + v-48;

    file.close();

    return ProcInfo(pid, name, summ, rss_pages*getpagesize());
}

void MainWindow::checkpoint()
{
    QSettings settings;
    settings.setValue("key", "...");
}

MainWindow::~MainWindow()
{

}
