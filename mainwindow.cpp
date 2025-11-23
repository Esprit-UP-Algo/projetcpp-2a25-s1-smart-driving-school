#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QFileDialog>
#include <QDateTime>
#include <QDir>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QTextDocument>
#include <QPageLayout>
#include <QPageSize>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>
#include <QSettings>
#include <QCoreApplication>
#include <QInputDialog>
#include <QProgressDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , selectedId(-1)
    , voiceRecognition(nullptr)
{
    ui->setupUi(this);

    // Initialisation de l'EmailSender
    emailSender = new EmailSender(this);

    // Configuration de l'email depuis config.ini
    QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
    QString smtpServer = settings.value("Email/smtp_server", "smtp.gmail.com").toString();
    int smtpPort = settings.value("Email/smtp_port", 587).toInt();
    QString emailUser = settings.value("Email/username", "").toString();
    QString emailPass = settings.value("Email/password", "").toString();
    QString fromName = settings.value("Email/from_name", "Auto-École Gestion").toString();

    if (!emailUser.isEmpty() && !emailPass.isEmpty()) {
        emailSender->setSmtpServer(smtpServer, smtpPort);
        emailSender->setCredentials(emailUser, emailPass);
        emailSender->setFromAddress(emailUser, fromName);
        qDebug() << "Configuration email chargée avec succès";
    } else {
        qDebug() << "AVERTISSEMENT: Configuration email manquante dans config.ini";
    }

    refreshTable(Vtmp.afficher());
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::on_tabWidget_currentChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshTable(QSqlQueryModel* model)
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(5);

    QStringList headers;
    headers << "ID" << "Nom Véhicule" << "Immatricule" << "Kilométrage" << "État";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    for(int row = 0; row < model->rowCount(); row++)
    {
        ui->tableWidget->insertRow(row);
        for(int col = 0; col < 5; col++)
        {
            QTableWidgetItem* item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
            ui->tableWidget->setItem(row, col, item);
        }
    }

    delete model;
}

void MainWindow::on_pushButton_clicked()
{
    QString marque = ui->lineEdit->text();
    QString matricule = ui->lineEdit_2->text();
    QString kmText = ui->lineEdit_4->text();
    QString etat = ui->comboBox->currentText();

    if (marque.isEmpty() || matricule.isEmpty() || kmText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs!");
        return;
    }

    bool ok;
    int km = kmText.toInt(&ok);

    if (!ok || km < 0) {
        QMessageBox::warning(this, "Erreur", "Kilométrage invalide!");
        return;
    }

    QSqlQuery query;
    int id = 1;

    qDebug() << "=== GENERATION ID ===";
    if (query.exec("SELECT NVL(MAX(ID_U), 0) + 1 FROM VEHICULE")) {
        if (query.next()) {
            id = query.value(0).toInt();
            qDebug() << "ID généré:" << id;
        }
    } else {
        qDebug() << "Erreur génération ID:" << query.lastError().text();
        if (query.exec("SELECT COUNT(*) FROM VEHICULE")) {
            if (query.next()) {
                id = query.value(0).toInt() + 1;
                qDebug() << "ID alternatif:" << id;
            }
        }
    }

    qDebug() << "Création véhicule avec ID:" << id << "Matricule:" << matricule << "Marque:" << marque;

    Vehicule v(id, matricule, marque, km, etat);

    bool test = v.ajouter();

    if (test) {
        refreshTable(Vtmp.afficher());

        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_4->clear();
        ui->comboBox->setCurrentIndex(0);

        QMessageBox::information(this, "Succès", "Véhicule ajouté avec succès!");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout! Vérifiez la console pour plus de détails.");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString terme = ui->lineEdit_3->text();

    if (terme.isEmpty()) {
        refreshTable(Vtmp.afficher());
    } else {
        refreshTable(Vtmp.rechercher(terme));
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    refreshTable(Vtmp.afficher());
    ui->lineEdit_3->clear();
}

void MainWindow::on_pushButton_10_clicked()
{
    if (selectedId == -1) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez un véhicule dans le tableau!");
        return;
    }

    QString champ = ui->comboBox_2->currentText();
    QString nouvelleValeur = ui->lineEdit_5->text();

    if (nouvelleValeur.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Entrez une nouvelle valeur!");
        return;
    }

    QString champSQL;
    if (champ == "marque") {
        champSQL = "MARQUE";
    } else if (champ == "matricule") {
        champSQL = "MATRICULE";
    } else if (champ == "kilometrage") {
        champSQL = "KILOMETRAGE";

        bool ok;
        nouvelleValeur.toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Erreur", "Le kilométrage doit être un nombre!");
            return;
        }
    } else if (champ == "etat") {
        champSQL = "ETAT";
    }

    bool test = Vtmp.modifier(selectedId, champSQL, nouvelleValeur);

    if (test) {
        refreshTable(Vtmp.afficher());
        ui->lineEdit_5->clear();
        QMessageBox::information(this, "Succès", "Véhicule modifié!");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification");
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    if (selectedId == -1) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez un véhicule dans le tableau!");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer ce véhicule?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool test = Vtmp.supprimer(selectedId);

        if (test) {
            refreshTable(Vtmp.afficher());
            selectedId = -1;
            QMessageBox::information(this, "Succès", "Véhicule supprimé avec succès!");
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression!");
        }
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer TOUS les véhicules?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        if (query.exec("DELETE FROM VEHICULE")) {
            refreshTable(Vtmp.afficher());
            QMessageBox::information(this, "Succès", "Tous les véhicules supprimés!");
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression!");
        }
    }
}

void MainWindow::on_pushButton_8_clicked()
{
    // First check if there are any broken vehicles
    QSqlQuery checkQuery;
    checkQuery.exec("SELECT COUNT(*) FROM VEHICULE WHERE ETAT = 'en panne'");

    int brokenCount = 0;
    if (checkQuery.next()) {
        brokenCount = checkQuery.value(0).toInt();
    }

    if (brokenCount == 0) {
        QMessageBox::information(this, "Aucun véhicule en panne",
                                 "Il n'y a aucun véhicule en panne à signaler.\n\nTous les véhicules sont en bon état !");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Exporter en PDF - Véhicules en Panne"),
                                                    QDir::homePath() + "/vehicules_en_panne_" +
                                                        QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm") + ".pdf",
                                                    tr("PDF Files (*.pdf)"));

    if (fileName.isEmpty()) {
        return;
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize::A4);
    printer.setPageOrientation(QPageLayout::Portrait);

    QString htmlContent = "<html><head><style>"
                          "body { font-family: Arial, sans-serif; margin: 20px; }"
                          "h1 { color: #d32f2f; text-align: center; margin-bottom: 10px; }"
                          ".warning-box { background-color: #ffebee; border: 2px solid #d32f2f; padding: 15px; margin: 20px 0; border-radius: 8px; }"
                          ".warning-icon { color: #d32f2f; font-size: 24pt; font-weight: bold; }"
                          "table { width: 100%; border-collapse: collapse; margin-top: 20px; }"
                          "th { background-color: #d32f2f; color: white; padding: 10px; border: 1px solid #c62828; }"
                          "td { padding: 8px; border: 1px solid #ddd; text-align: left; }"
                          "tr { background-color: #ffcdd2; }"
                          ".footer { margin-top: 30px; text-align: center; font-size: 10pt; color: #7f8c8d; }"
                          ".subtitle { text-align: center; color: #d32f2f; margin-bottom: 20px; font-weight: bold; }"
                          ".recommendation { background-color: #fff3cd; border: 2px solid #ff9800; padding: 15px; margin: 20px 0; border-radius: 8px; }"
                          ".recommendation h2 { color: #ff6f00; margin-top: 0; }"
                          ".recommendation ul { line-height: 1.8; }"
                          "</style></head><body>";

    htmlContent += "<h1>⚠️ VÉHICULES EN PANNE - RAPPORT URGENT</h1>";
    htmlContent += "<p class='subtitle'>Généré le : " +
                   QDateTime::currentDateTime().toString("dd/MM/yyyy à hh:mm") + "</p>";

    // Warning box
    htmlContent += "<div class='warning-box'>";
    htmlContent += "<div class='warning-icon'>⚠️ ATTENTION</div>";
    htmlContent += "<p style='margin: 10px 0 0 0; font-size: 12pt;'>";
    htmlContent += "Les véhicules listés ci-dessous sont actuellement <strong>EN PANNE</strong> ";
    htmlContent += "et nécessitent une intervention immédiate.";
    htmlContent += "</p>";
    htmlContent += "</div>";

    // Table of broken vehicles
    htmlContent += "<table>";
    htmlContent += "<tr><th>ID</th><th>Nom Véhicule</th><th>Immatricule</th><th>Kilométrage</th><th>État</th></tr>";

    QSqlQuery query;
    query.exec("SELECT ID_U, MATRICULE, MARQUE, KILOMETRAGE, ETAT FROM VEHICULE WHERE ETAT = 'en panne' ORDER BY MARQUE");

    while (query.next()) {
        htmlContent += "<tr>";
        htmlContent += "<td>" + query.value(0).toString() + "</td>";
        htmlContent += "<td>" + query.value(2).toString() + "</td>";
        htmlContent += "<td>" + query.value(1).toString() + "</td>";
        htmlContent += "<td>" + query.value(3).toString() + " km</td>";
        htmlContent += "<td style='color: #c62828; font-weight: bold;'>EN PANNE</td>";
        htmlContent += "</tr>";
    }

    htmlContent += "</table>";

    // Recommendations section
    htmlContent += "<div class='recommendation'>";
    htmlContent += "<h2>📋 RECOMMANDATIONS URGENTES</h2>";
    htmlContent += "<p><strong>Actions à entreprendre immédiatement :</strong></p>";
    htmlContent += "<ul>";
    htmlContent += "<li><strong>Atelier de réparation :</strong> Conduire ou remorquer les véhicules vers un garage agréé pour diagnostic et réparation</li>";
    htmlContent += "<li><strong>Visite technique :</strong> Prévoir une inspection complète et une visite technique obligatoire après réparation</li>";
    htmlContent += "<li><strong>Contrôle de sécurité :</strong> Vérifier l'état des freins, pneus, et systèmes de sécurité</li>";
    htmlContent += "<li><strong>Documentation :</strong> Conserver tous les justificatifs de réparation pour le suivi</li>";
    htmlContent += "<li><strong>Mise hors service :</strong> Ne pas utiliser ces véhicules avant réparation complète</li>";
    htmlContent += "</ul>";
    htmlContent += "<p style='color: #d32f2f; font-weight: bold; margin-top: 15px;'>";
    htmlContent += "⚠️ Ces véhicules doivent être retirés du service jusqu'à leur remise en état conforme.";
    htmlContent += "</p>";
    htmlContent += "</div>";

    htmlContent += "<div class='footer'>";
    htmlContent += "<p><strong>Total de véhicules en panne : " + QString::number(brokenCount) + "</strong></p>";
    htmlContent += "<p>Document généré automatiquement par le système de gestion des véhicules</p>";
    htmlContent += "<p style='color: #d32f2f;'>Ce rapport nécessite une action immédiate de la part de la direction</p>";
    htmlContent += "</div>";

    htmlContent += "</body></html>";

    QTextDocument document;
    document.setHtml(htmlContent);
    document.print(&printer);

    QMessageBox::information(this, "Succès",
                             "Le rapport des véhicules en panne a été exporté avec succès!\n\n"
                             "Nombre de véhicules en panne : " + QString::number(brokenCount) + "\n"
                                                                  "Emplacement : " + fileName);
}

void MainWindow::on_pushButton_9_clicked()
{
    QString critere = ui->comboBox_3->currentText();
    QString critereSQL;

    if (critere == "en kilometrage") {
        critereSQL = "KILOMETRAGE DESC";
    } else if (critere == "en etat") {
        critereSQL = "ETAT";
    }

    refreshTable(Vtmp.trier(critereSQL));
}

void MainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    selectedId = ui->tableWidget->item(index.row(), 0)->text().toInt();
    qDebug() << "ID sélectionné:" << selectedId;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == 1) {
        updateStatistics();
    }
}

void MainWindow::updateStatistics()
{
    QSqlQuery query;

    int totalVehicles = 0;
    int goodCondition = 0;
    int brokenDown = 0;
    double avgKm = 0;

    query.exec("SELECT COUNT(*) FROM VEHICULE");
    if (query.next()) totalVehicles = query.value(0).toInt();

    query.exec("SELECT COUNT(*) FROM VEHICULE WHERE ETAT = 'bonne etat'");
    if (query.next()) goodCondition = query.value(0).toInt();

    query.exec("SELECT COUNT(*) FROM VEHICULE WHERE ETAT = 'en panne'");
    if (query.next()) brokenDown = query.value(0).toInt();

    query.exec("SELECT AVG(KILOMETRAGE) FROM VEHICULE");
    if (query.next()) avgKm = query.value(0).toDouble();

    QWidget* statsWidget = ui->tab_2->findChild<QWidget*>("statsContainer");
    if (statsWidget) {
        delete statsWidget;
    }

    QWidget* container = new QWidget(ui->tab_2);
    container->setObjectName("statsContainer");
    container->setGeometry(50, 80, 1050, 500);

    QVBoxLayout* mainLayout = new QVBoxLayout(container);
    mainLayout->setSpacing(20);

    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->append("Bonne état", goodCondition);
    pieSeries->append("En panne", brokenDown);

    QPieSlice *goodSlice = pieSeries->slices().at(0);
    goodSlice->setBrush(QColor("#4CAF50"));
    goodSlice->setLabelVisible(true);
    goodSlice->setLabelColor(Qt::black);
    goodSlice->setLabelPosition(QPieSlice::LabelOutside);

    QPieSlice *brokenSlice = pieSeries->slices().at(1);
    brokenSlice->setBrush(QColor("#F44336"));
    brokenSlice->setLabelVisible(true);
    brokenSlice->setLabelColor(Qt::black);
    brokenSlice->setLabelPosition(QPieSlice::LabelOutside);

    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("📊 État des Véhicules");
    pieChart->setAnimationOptions(QChart::SeriesAnimations);
    pieChart->setTheme(QChart::ChartThemeLight);

    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    pieChart->setTitleFont(titleFont);

    QChartView *pieChartView = new QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    pieChartView->setMinimumHeight(250);

    QBarSet *set = new QBarSet("Statistiques");
    *set << totalVehicles << goodCondition << brokenDown << (avgKm / 1000);
    set->setColor(QColor("#009688"));

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(set);

    QChart *barChart = new QChart();
    barChart->addSeries(barSeries);
    barChart->setTitle("📈 Statistiques Générales");
    barChart->setAnimationOptions(QChart::SeriesAnimations);
    barChart->setTheme(QChart::ChartThemeLight);
    barChart->setTitleFont(titleFont);

    QStringList categories;
    categories << "Total" << "Bon état" << "En panne" << "Moy. KM (x1000)";

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, qMax(totalVehicles, (int)(avgKm / 1000)) + 5);
    barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    barChart->legend()->setVisible(false);

    QChartView *barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);
    barChartView->setMinimumHeight(250);

    mainLayout->addWidget(pieChartView);
    mainLayout->addWidget(barChartView);

    container->setLayout(mainLayout);
    container->show();

    QString html = R"(
        <style>
            .title { color:#1E88E5; font-size:18px; font-weight:700; margin-bottom:10px; }
            .info { font-size:14px; color:#455A64; line-height: 1.8; }
        </style>
        <div>
            <div class='title'>📊 Résumé des Statistiques</div>
            <div class='info'>
                <b>Total:</b> )" + QString::number(totalVehicles) + R"( véhicules<br>
                <b>En bon état:</b> )" + QString::number(goodCondition) + R"(<br>
                <b>En panne:</b> )" + QString::number(brokenDown) + R"(<br>
                <b>Kilométrage moyen:</b> )" + QString::number(avgKm, 'f', 0) + R"( km
            </div>
        </div>
    )";

    ui->label_stats_info->setText(html);
    ui->label_stats_info->setTextFormat(Qt::RichText);
    ui->label_stats_info->setGeometry(50, 20, 400, 150);
}


void MainWindow::on_pushButton_microphone_clicked()
{
    if (!voiceRecognition) {
        voiceRecognition = new VoiceRecognition(this);

        connect(voiceRecognition, &VoiceRecognition::textRecognized,
                this, &MainWindow::onVoiceTextRecognized);
        connect(voiceRecognition, &VoiceRecognition::error,
                this, &MainWindow::onVoiceError);
        connect(voiceRecognition, &VoiceRecognition::recordingStarted,
                this, &MainWindow::onRecordingStarted);
        connect(voiceRecognition, &VoiceRecognition::recordingStopped,
                this, &MainWindow::onRecordingStopped);
    }

    if (voiceRecognition->isRecording()) {
        voiceRecognition->stopRecording();
    } else {
        voiceRecognition->startRecording();
    }
}

void MainWindow::onRecordingStarted()
{
    ui->pushButton_microphone->setText("🎤 Arrêter");
    ui->pushButton_microphone->setStyleSheet("background-color: #f44336; color: white; font-weight: bold;");

    // Check if label exists in UI, otherwise create or skip
    QLabel* statusLabel = this->findChild<QLabel*>("label_voice_status");
    if (statusLabel) {
        statusLabel->setText("🔴 Enregistrement... Parlez maintenant!");
        statusLabel->setStyleSheet("color: #f44336; font-weight: bold;");
    }
}

void MainWindow::onRecordingStopped()
{
    ui->pushButton_microphone->setText("🎤 Parler");
    ui->pushButton_microphone->setStyleSheet("");

    QLabel* statusLabel = this->findChild<QLabel*>("label_voice_status");
    if (statusLabel) {
        statusLabel->setText("⏸️ Traitement...");
        statusLabel->setStyleSheet("color: #ff9800;");
    }
}

void MainWindow::onVoiceTextRecognized(const QString &text)
{
    QLabel* statusLabel = this->findChild<QLabel*>("label_voice_status");
    if (statusLabel) {
        statusLabel->setText("✅ Reconnu: " + text);
        statusLabel->setStyleSheet("color: #4CAF50; font-weight: bold;");
    }

    // Remplir le champ de modification avec le texte reconnu
    ui->lineEdit_5->setText(text);

    qDebug() << "Voix reconnue:" << text;
}

void MainWindow::onVoiceError(const QString &error)
{
    QLabel* statusLabel = this->findChild<QLabel*>("label_voice_status");
    if (statusLabel) {
        statusLabel->setText("❌ Erreur: " + error);
        statusLabel->setStyleSheet("color: #f44336;");
    }

    QMessageBox::warning(this, "Erreur de reconnaissance vocale", error);
}

void MainWindow::on_pushButton_email_clicked()
{
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM VEHICULE WHERE ETAT = 'bonne etat'");

    int availableCount = 0;
    if (query.next()) {
        availableCount = query.value(0).toInt();
    }

    if (availableCount == 0) {
        QMessageBox::warning(this, "Aucun véhicule",
                             "Il n'y a aucun véhicule en bon état à notifier.");
        return;
    }

    QString instructorEmail = selectInstructor();

    if (instructorEmail.isEmpty()) {
        return;
    }

    QString vehiclesText = getAvailableVehiclesText();
    QString subject = "Véhicules Disponibles - Liste du " +
                      QDateTime::currentDateTime().toString("dd/MM/yyyy");

    QString body = "Bonjour,\n\n";
    body += "Voici la liste des véhicules disponibles et en bon état:\n\n";
    body += vehiclesText;
    body += "\nTotal de véhicules disponibles: " + QString::number(availableCount) + "\n\n";
    body += "Cordialement,\n";
    body += "Système de Gestion des Véhicules";

    QProgressDialog progress("Envoi de l'email en cours...", "Annuler", 0, 0, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.setValue(0);
    progress.show();
    QApplication::processEvents();

    bool success = emailSender->sendEmail(instructorEmail, "Instructeur", subject, body);

    progress.close();

    if (success) {
        QMessageBox::information(this, "Succès",
                                 "Email envoyé avec succès à " + instructorEmail + "!");
    } else {
        QMessageBox::critical(this, "Erreur d'envoi",
                              "Impossible d'envoyer l'email:\n" + emailSender->lastError());
    }
}

QString MainWindow::getAvailableVehiclesText()
{
    QString text;
    QSqlQuery query;

    query.exec("SELECT MARQUE, MATRICULE, KILOMETRAGE FROM VEHICULE WHERE ETAT = 'bonne etat' ORDER BY MARQUE");

    int count = 1;
    while (query.next()) {
        QString marque = query.value(0).toString();
        QString matricule = query.value(1).toString();
        int km = query.value(2).toInt();

        text += "═══════════════════════════════════\n";
        text += "🚗 Véhicule #" + QString::number(count++) + "\n";
        text += "   Marque: " + marque + "\n";
        text += "   Immatricule: " + matricule + "\n";
        text += "   Kilométrage: " + QString::number(km) + " km\n";
        text += "   État: Bonne état\n\n";
    }

    text += "═══════════════════════════════════\n";

    return text;
}

QStringList MainWindow::getInstructorsList()
{
    QStringList instructors;
    QSqlQuery query;

    if (query.exec("SELECT NOM, PRENOM, MAIL FROM MONITEUR ORDER BY NOM")) {
        while (query.next()) {
            QString nom = query.value(0).toString();
            QString prenom = query.value(1).toString();
            QString email = query.value(2).toString();

            instructors << (nom + " " + prenom + " (" + email + ")");
        }
    } else {
        qDebug() << "Erreur récupération moniteurs:" << query.lastError().text();
    }

    return instructors;
}

QString MainWindow::selectInstructor()
{
    QStringList instructors = getInstructorsList();

    if (instructors.isEmpty()) {
        QMessageBox::warning(this, "Aucun moniteur",
                             "Aucun moniteur trouvé dans la base de données.\n\n"
                             "Veuillez d'abord ajouter des moniteurs dans la table MONITEUR.");
        return QString();
    }

    bool ok;
    QString selected = QInputDialog::getItem(this,
                                             "Sélectionner un moniteur",
                                             "Choisissez le moniteur à notifier:",
                                             instructors,
                                             0,
                                             false,
                                             &ok);

    if (ok && !selected.isEmpty()) {
        int start = selected.indexOf('(');
        int end = selected.indexOf(')');
        if (start != -1 && end != -1) {
            return selected.mid(start + 1, end - start - 1);
        }
    }

    return QString();
}
