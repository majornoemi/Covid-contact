#include "mainwindow.h"

#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QTableWidget>
#include <QTextStream>
#include <QFileDialog>
#include <QDateEdit>
#include <QWidget>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    //p_table->model()->setHeaderData( column_index, Qt::Horizontal, "Hello", Qt::DisplayRole );
    ui->myTableWidget->insertColumn(0);
    ui->myTableWidget->insertColumn(1);
    ui->myTableWidget->insertColumn(2);
    ui->myTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Név"));
    ui->myTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Kontakt neve"));
    ui->myTableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Dátum"));
    ui->tableWidget->insertColumn(0);//nev
    ui->tableWidget->insertColumn(1);//kontaktjai
    ui->tableWidget->insertColumn(2);//datum
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Név"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Kontakt neve"));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Dátum"));
    ui->szurttable->insertColumn(0);
    ui->szurttable->insertColumn(1);
    ui->szurttable->insertColumn(2);
    ui->szurttable->setHorizontalHeaderItem(0, new QTableWidgetItem("Név"));
    ui->szurttable->setHorizontalHeaderItem(1, new QTableWidgetItem("Kontakt neve"));
    ui->szurttable->setHorizontalHeaderItem(2, new QTableWidgetItem("Dátum"));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoad_triggered()//fajlbetoltes
{

    filename = QFileDialog::getOpenFileName(this, "Open File","."  );
    QFile infile(filename);
    infile.open(QFile :: ReadOnly | QFile :: Text);
    QTextStream inFile (&infile);
    inFile.setCodec("UTF-8");

    while (!inFile.atEnd())
        {
           QString line = inFile.readLine();
           //QString line = infile.readLine();
           Human h (line);
           emberlista.push_back(h);
           ui->listWidget->addItem(line);
        }

}

void MainWindow::on_actionSave_triggered() // uj nevsor mentese
{

    QFile outfile(filename);
    outfile.open(QFile :: WriteOnly | QFile :: Text);
    QTextStream outFile (&outfile);
    outFile.setCodec("UTF-8");
    int c = ui->listWidget->count();

        for ( int i = 0 ; i < c ; i ++)
                {
                    QModelIndex * modelindex = new QModelIndex (ui->listWidget->model()->index(i,0));
                    QString text = modelindex->data(Qt::DisplayRole).toString();
                    outFile << text <<endl;
                }

}


void MainWindow::on_pushButton_clicked()// nev hozzaadasa
{
    QString s = ui->lineEdit->text();
    Human ember (s);
    emberlista.push_back(ember);
    ui->listWidget->addItem(s);

}





void MainWindow::on_pushButton_2_clicked() //save contacts
{
    QListWidgetItem * item = ui->listWidget->selectedItems().at(0);
    const int rowCount = ui->myTableWidget->model()->rowCount();
    for (int i = 0; i < rowCount; ++i)
    {
        bool checked = (ui->myTableWidget->model()->index(i, 1).data(Qt::CheckStateRole).toInt() == Qt::Checked);
        QString date = ui->myTableWidget->model()->index(i, 2).data().toString();
        QDate datum = QDate :: fromString(date, "yyyy.MM.dd");
        Kontakt * actualKontakt = nullptr;
        for (int j = 0 ; j < kontaktlista.size(); j++)
                {
                    if((kontaktlista[j].h1.name == item->text() && kontaktlista[j].h2.name == ui->myTableWidget->model()->index(i,0).data().toString())
                    || (kontaktlista[j].h2.name == item->text() && kontaktlista[j].h1.name == ui->myTableWidget->model()->index(i,0).data().toString())){
                        kontaktlista[j].vanekontakt = checked;
                        kontaktlista[j].time = datum;
                        actualKontakt = &kontaktlista[j];
                    }
                }
        if(actualKontakt == nullptr){
            Human h1 (item->text());
            Human h2 ( ui->myTableWidget->model()->index(i,0).data().toString());
            Kontakt k(h1,h2,checked,datum);
            kontaktlista.push_back(k);
        }
    }
}

void MainWindow::on_listWidget_itemSelectionChanged()
{
    QListWidgetItem * item = ui->listWidget->selectedItems().at(0);
    QSignalMapper * map = new QSignalMapper(this);
    int row = 0;
    ui->myTableWidget->setRowCount(0);
    for (int i = 0 ; i < emberlista.size(); i++)
    {
        if (item->text() != emberlista[i].name)
        {
            ui->myTableWidget->insertRow(row);
            ui->myTableWidget->setItem(row , 0, new QTableWidgetItem(emberlista[i].name));
            QTableWidgetItem *item2 = new QTableWidgetItem();
            bool checkState = false;
            QDate *date = nullptr;

            for (int j = 0 ; j < kontaktlista.size(); j++)
                    {
                        if((kontaktlista[j].h1.name == item->text() && kontaktlista[j].h2.name == emberlista[i].name)
                        || (kontaktlista[j].h2.name == item->text() && kontaktlista[j].h1.name == emberlista[i].name)){
                           checkState =  kontaktlista[j].vanekontakt;
                           date = &kontaktlista[j].time;

                        }
                    }

            if(checkState){
                item2->setCheckState
                    (Qt :: Checked);

            }
            else{
                item2->setCheckState
                    (Qt :: Unchecked);
            }

            ui->myTableWidget->setItem(row , 1, item2);

            if(date != nullptr && checkState){
                ui->myTableWidget->setItem(row, 2, new QTableWidgetItem(date->toString("yyyy.MM.dd")));

            }

            row++;

        }
    }
}

void MainWindow::on_adathalom_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

    int row =0;
    //std::list<int> indexhalmaz;


    for (int i = 0 ; i < emberlista.size() ; i++)
    {
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(emberlista[i].name));
        int szamlalo = 0;
        for (int j = 0; j < kontaktlista.size() ; j++)
            {
            //if (std::find(indexhalmaz.begin(), indexhalmaz.end(), j) != indexhalmaz.end() ) continue;

                if (kontaktlista[j].h1.name == emberlista[i].name && kontaktlista[j].vanekontakt)
                    {
                            szamlalo ++;
                            ui->tableWidget->insertRow(row+szamlalo);
                            ui->tableWidget->setItem(row+szamlalo-1, 1, new QTableWidgetItem (kontaktlista[j].h2.name));
                            ui->tableWidget->setItem(row+szamlalo-1, 2, new QTableWidgetItem (kontaktlista[j].time.toString("yyyy.MM.dd")) );
                            //indexhalmaz.push_back(j);
                    }
                else if (kontaktlista[j].h2.name == emberlista[i].name && kontaktlista[j].vanekontakt)
                    {
                            szamlalo ++;
                            ui->tableWidget->insertRow(row+szamlalo);
                            ui->tableWidget->setItem(row+szamlalo-1, 1, new QTableWidgetItem(kontaktlista[j].h1.name));
                            ui->tableWidget->setItem(row+szamlalo-1, 2, new QTableWidgetItem (kontaktlista[j].time.toString("yyyy.MM.dd")) );
                            //indexhalmaz.push_back(j);
                    }

            }
        if (szamlalo != 0) row+=szamlalo;
        else row++;
    }


}

void MainWindow::on_pushButton_3_clicked() //vissza
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->tableWidget->setRowCount(0);
}

void MainWindow::on_szures_clicked()// datum szerint szures
{
    ui->stackedWidget->setCurrentIndex(2);
    int row = 0;
    for (int i =0 ; i < kontaktlista.size() ; i ++)
    {
        if (kontaktlista[i].time >= ui->tol->date() && kontaktlista[i].time<=ui->ig->date() && kontaktlista[i].vanekontakt)
                {

                       ui->szurttable->insertRow(row);
                       ui->szurttable->setItem(row,0, new QTableWidgetItem(kontaktlista[i].h1.name));
                       ui->szurttable->setItem(row,1, new QTableWidgetItem(kontaktlista[i].h2.name));
                       ui->szurttable->setItem(row,2, new QTableWidgetItem(kontaktlista[i].time.toString("yyyy.MM.dd")));
                       row++;

                }
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->szurttable->setRowCount(0);
}

void MainWindow::on_felho_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);

}

void MainWindow::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->treeWidget->clear();
}

void MainWindow::on_pushButton_6_clicked() // felho
{
    ui->treeWidget->clear();
    QListWidgetItem * item = ui->listWidget->selectedItems().at(0);
    QTreeWidgetItem * root = new QTreeWidgetItem(ui->treeWidget);
    root->setText(0,item->text());
    QString name = item->text();
    namelist.push_back(name);
    std::list<int> indexhalmaz ;
    ui->treeWidget->setSortingEnabled(true);
    ui->treeWidget->setHeaderLabel("Név");

    algorithm(indexhalmaz, item->text(), root);

}


void MainWindow::algorithm (std::list<int>&indexhalmaz , QString name , QTreeWidgetItem* item)
{
    std::vector<QTreeWidgetItem*> localitems;
    for (int i = 0; i < kontaktlista.size() ; i++ )
    {
        if (std::find(indexhalmaz.begin(), indexhalmaz.end(), i) != indexhalmaz.end()
                || kontaktlista[i].time < ui->from->date()
                || kontaktlista[i].time > ui->to->date()) continue;


        if ((kontaktlista[i].h1.name == name || kontaktlista[i].h2.name == name) && kontaktlista[i].vanekontakt)
            {
                QString kontaktname;
                if(kontaktlista[i].h1.name == name)
                    kontaktname = kontaktlista[i].h2.name;
                else
                    kontaktname = kontaktlista[i].h1.name;

                indexhalmaz.push_back(i);
                QTreeWidgetItem * child = new QTreeWidgetItem();
                child->setText(0, kontaktname);
                child->setText(1, kontaktlista[i].time.toString());
                item->addChild(child);

                localitems.push_back(child);
            }
    }

    for (int i = 0; i < localitems.size() ; i++ )
    {
        algorithm(indexhalmaz,localitems[i]->text(0), localitems[i]);
    }
}


