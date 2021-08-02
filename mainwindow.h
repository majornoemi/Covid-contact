#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QDateTime>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QSignalMapper>
#include <QDateEdit>
#include <QDate>
#include <QTreeWidgetItem>
#include <QTreeWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Human
{

    QString name;

    Human (QString _name) : name(_name)
    {}
};

struct Kontakt
{
    Human h1;
    Human h2;
    bool vanekontakt = false;
    QDate time;

    Kontakt (Human _h1, Human _h2, bool _vanekontakt, QDate _time) : h1(_h1), h2(_h2), vanekontakt (_vanekontakt), time(_time){}
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::vector <Human> emberlista;
    std::vector <Kontakt> kontaktlista ;
    std::vector <QString> namelist; // felhoben levo nevek
    QString filename;


private slots:
    void on_actionLoad_triggered();

    void on_actionSave_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_listWidget_itemSelectionChanged();

    void on_adathalom_clicked();

    void on_pushButton_3_clicked();

    void on_szures_clicked();

    void on_pushButton_4_clicked();

    void on_felho_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void algorithm (std::list<int>&indexhalmaz , QString name , QTreeWidgetItem* item );


private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
