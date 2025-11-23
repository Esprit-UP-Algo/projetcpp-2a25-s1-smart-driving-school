#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QRegularExpression>
#include <QSqlQueryModel>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    actualiserTable(); // crée le modèle SQL et l'attache au proxy

    connect(ui->lineEdit_searchGlobal, &QLineEdit::textChanged,
            this, &MainWindow::on_searchGlobalTextChanged);

    // Click sur la table
    connect(ui->tableView, &QTableView::clicked,
            this, &MainWindow::on_tableView_clicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::actualiserTable() {
    // Base model depuis la BD
    QSqlQueryModel *base = tmpCondidat.afficher();

    // Créer/configurer le proxy au premier appel
    if (!proxy) {
        proxy = new QSortFilterProxyModel(this);
        proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
        proxy->setFilterKeyColumn(-1); // 🔴 -1 = toutes les colonnes
    }

    // Brancher le proxy sur le modèle SQL et afficher dans la table
    proxy->setSourceModel(base);
    ui->tableView->setModel(proxy);
}

bool MainWindow::validateInputs() {
    // NOM
    if (ui->lineEdit_2->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le NOM est requis.");
        return false;
    }
    // PRENOM
    if (ui->lineEdit_3->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le PRÉNOM est requis.");
        return false;
    }
    // NUM_TEL
    bool ok = false;
    int numTel = ui->lineEdit_4->text().trimmed().toInt(&ok);
    if (!ok || numTel <= 0) {
        QMessageBox::warning(this, "Erreur", "NUM_TEL invalide.");
        return false;
    }
    // MAIL
    {
        QRegularExpression rx(R"(\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}\b)");
        if (!rx.match(ui->lineEdit_5->text().trimmed()).hasMatch()) {
            QMessageBox::warning(this, "Erreur", "MAIL invalide.");
            return false;
        }
    }
    // CIN
    int cin = ui->lineEdit_6->text().trimmed().toInt(&ok);
    if (!ok || cin <= 0) {
        QMessageBox::warning(this, "Erreur", "CIN invalide.");
        return false;
    }
    return true;
}

void MainWindow::clearFields() {
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();
}

void MainWindow::on_ajouterButton_clicked() {
    if (!validateInputs()) return;

    Condidat c(
        ui->lineEdit_2->text().trimmed(),              // NOM
        ui->lineEdit_3->text().trimmed(),              // PRENOM
        ui->lineEdit_4->text().trimmed().toInt(),      // NUM_TEL
        ui->lineEdit_5->text().trimmed(),              // MAIL
        ui->lineEdit_6->text().trimmed().toInt()       // CIN
        );

    if (c.ajouter()) {
        QMessageBox::information(this, "Succès", "Candidat ajouté.");
        actualiserTable();   // recharge base+proxy (l’ID apparaitra dans la table)
        // Sélectionner dernière ligne visible (via proxy)
        auto *m = ui->tableView->model();
        if (m && m->rowCount() > 0) ui->tableView->selectRow(m->rowCount() - 1);
        clearFields();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout.");
    }
}

void MainWindow::on_modifierButton_clicked() {
    if (!validateInputs()) return;

    bool ok = false;
    int id = ui->lineEdit_id_modify->text().trimmed().toInt(&ok);
    if (!ok || id <= 0) {
        QMessageBox::warning(this, "Erreur", "ID de modification invalide.");
        return;
    }

    Condidat c(
        id,
        ui->lineEdit_2->text().trimmed(),
        ui->lineEdit_3->text().trimmed(),
        ui->lineEdit_4->text().trimmed().toInt(),
        ui->lineEdit_5->text().trimmed(),
        ui->lineEdit_6->text().trimmed().toInt()
        );

    if (c.modifier()) {
        QMessageBox::information(this, "Succès", "Candidat modifié.");
        actualiserTable();
    } else {
        QMessageBox::critical(this, "Erreur", "Aucune ligne modifiée.");
    }
}

void MainWindow::on_supprimerButton_clicked() {
    bool ok = false;
    int id = ui->lineEdit_id_delete->text().trimmed().toInt(&ok);
    if (!ok || id <= 0) {
        QMessageBox::warning(this, "Erreur", "ID de suppression invalide.");
        return;
    }

    if (QMessageBox::question(this, "Confirmation", "Supprimer ce candidat ?")
        == QMessageBox::Yes) {
        if (tmpCondidat.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Candidat supprimé.");
            actualiserTable();
            ui->lineEdit_id_delete->clear();
        } else {
            QMessageBox::critical(this, "Erreur", "Suppression échouée.");
        }
    }
}

// 🔴 Recherche globale sur toutes les colonnes via proxy
void MainWindow::on_searchGlobalTextChanged() {
    const QString term = ui->lineEdit_searchGlobal->text();
    // Filtrage insensible à la casse ; échappe le texte pour REGEX
    proxy->setFilterRegularExpression(
        QRegularExpression(QRegularExpression::escape(term),
                           QRegularExpression::CaseInsensitiveOption));
}

// Click sur la table : bien récupérer la ligne source si proxy actif
void MainWindow::on_tableView_clicked(const QModelIndex &index) {
    if (!index.isValid()) return;

    QModelIndex srcIndex = index;
    const QAbstractItemModel *viewModel = ui->tableView->model();

    const QSortFilterProxyModel *pm = qobject_cast<const QSortFilterProxyModel*>(viewModel);
    const QAbstractItemModel *baseModel = viewModel;

    if (pm) {
        srcIndex = pm->mapToSource(index);
        baseModel = pm->sourceModel();
    }

    if (!baseModel || !srcIndex.isValid()) return;

    // Colonnes: 0=ID, 1=NOM, 2=PRENOM, 3=NUM_TEL, 4=MAIL, 5=CIN
    int row = srcIndex.row();
    int id      = baseModel->data(baseModel->index(row, 0)).toInt();
    QString nom = baseModel->data(baseModel->index(row, 1)).toString();
    QString pre = baseModel->data(baseModel->index(row, 2)).toString();
    int tel     = baseModel->data(baseModel->index(row, 3)).toInt();
    QString mail= baseModel->data(baseModel->index(row, 4)).toString();
    int cin     = baseModel->data(baseModel->index(row, 5)).toInt();

    // Remplir les champs d’édition
    ui->lineEdit_2->setText(nom);
    ui->lineEdit_3->setText(pre);
    ui->lineEdit_4->setText(QString::number(tel));
    ui->lineEdit_5->setText(mail);
    ui->lineEdit_6->setText(QString::number(cin));

    // Préremplir les IDs d’action
    ui->lineEdit_id_modify->setText(QString::number(id));
    ui->lineEdit_id_delete->setText(QString::number(id));
}
