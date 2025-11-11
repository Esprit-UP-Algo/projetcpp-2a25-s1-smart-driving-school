#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#include "condidat.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ajouterButton_clicked();           // AJOUT (ID auto BD)
    void on_modifierButton_clicked();          // MODIFIER (ID dans lineEdit_id_modify)
    void on_supprimerButton_clicked();         // SUPPRIMER (ID dans lineEdit_id_delete)



private:
    Ui::MainWindow *ui;
    Condidat tmpCondidat;

    QSortFilterProxyModel *proxy{nullptr};     // 🔴 proxy pour filtrer toutes colonnes

    void actualiserTable();                    // recharge le modèle SQL et MAJ proxy
    bool validateInputs();                     // valide NOM, PRENOM, NUM_TEL, MAIL, CIN
    void clearFields();                        // nettoie NOM..CIN
};

#endif // MAINWINDOW_H
