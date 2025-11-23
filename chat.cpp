#include "chat.h"
#include "ui_chat.h"
#include <random>
#include <algorithm>
#include "mainwindow.h"
#include <QRegularExpression>


chat::chat(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::chat)
{
    ui->setupUi(this);

    // Initialize network manager
    networkManager = new QNetworkAccessManager(this);

    // Connect signals and slots
    connect(ui->SendButton, &QPushButton::clicked, this, &chat::sendMessage);
}

void chat::setMainWindow(MainWindow *mw)
{
    m_mainWindow = mw;
}

chat::~chat()
{
    delete ui;
}

void chat::sendMessage()
{
    QString message = ui->messageTextEdit->toPlainText();
    message = message.trimmed();
    if (message.isEmpty())
    {
        ui->messageTextEdit->clear();
        return;
    }
    ui->chatDisplayTextEdit->append("Utilisateur : " + message);
    ui->messageTextEdit->clear();

    QString reply = generateChatbotReply(message);
    if (!reply.isEmpty())
    {
        ui->chatDisplayTextEdit->append("Chatbot : " + reply);
    }
}

void chat::on_SendButton_clicked()
{
    sendMessage();
}

void chat::fetchTriviaQuestion(const QString& apiUrl)
{
    QNetworkRequest request;
    request.setUrl(QUrl(apiUrl));

    QNetworkReply* reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument jsonDoc(QJsonDocument::fromJson(response));
            QJsonObject jsonObject = jsonDoc.object();
            QJsonArray results = jsonObject["results"].toArray();
            if (!results.isEmpty()) {
                QJsonObject triviaQuestion = results[0].toObject();
                m_currentQuestion.question = triviaQuestion["question"].toString();
                m_currentQuestion.correctAnswer = triviaQuestion["correct_answer"].toString();
                QJsonArray incorrectAnswers = triviaQuestion["incorrect_answers"].toArray();
                for (const QJsonValue& value : incorrectAnswers)
                {
                    m_currentQuestion.options.append(value.toString());
                }

                // Add the correct answer to the options list
                m_currentQuestion.options.append(m_currentQuestion.correctAnswer);

                // Shuffle the options to randomize their order
                std::random_device rd;
                std::mt19937 gen(rd());
                std::shuffle(m_currentQuestion.options.begin(), m_currentQuestion.options.end(), gen);

                // Remove HTML entities from the question and options
                m_currentQuestion.question = decodeHtmlEntities(m_currentQuestion.question);
                for (QString& option : m_currentQuestion.options)
                {
                    option = decodeHtmlEntities(option);
                }

                ui->chatDisplayTextEdit->append("Chatbot: " + m_currentQuestion.question);

                for (int i = 0; i < m_currentQuestion.options.size(); ++i)
                {
                    QString optionNumber = QString::number(i + 1);
                    QString optionText = m_currentQuestion.options[i];
                    ui->chatDisplayTextEdit->append(optionNumber + ". " + optionText);
                }

                m_currentQuestion.isValid = true;
            }
            else {
                ui->chatDisplayTextEdit->append("Chatbot: Sorry, I couldn't fetch a trivia question.");
            }
        } else {
            QString error = reply->errorString();
            qDebug() << "Network error:" << error;
            ui->chatDisplayTextEdit->append("Chatbot: Sorry, I couldn't fetch a trivia question. Error: " + error);
        }
        reply->deleteLater();
    });
}

QString chat::generateChatbotReply(const QString& message)
{
    QString reply;
    QString m = message.toLower();


    // 1) "transaction 3"
    if (m.contains("transaction") && m_mainWindow) {
        QRegularExpression re("(transaction[^0-9]*)(\\d+)");
        QRegularExpressionMatch match = re.match(m);
        if (match.hasMatch()) {
            int id = match.captured(2).toInt();
            return m_mainWindow->getTransactionInfoById(id);
        }
    }

    // 2) "id 3"  (default: treat as transaction ID)
    if (m.contains("id") && m_mainWindow) {
        QRegularExpression re("(id[^0-9]*)(\\d+)");
        QRegularExpressionMatch match = re.match(m);
        if (match.hasMatch()) {
            int id = match.captured(2).toInt();
            return m_mainWindow->getTransactionInfoById(id);
        }
    }

    // ---------- FINANCE / PROJECT QUESTIONS (use data) ----------

    // Total balance / solde
    if ((m.contains("total") || m.contains("solde") || m.contains("balance")) && m_mainWindow)
    {
        double bal = m_mainWindow->getTotalBalance();
        reply = QString("Le solde total actuel est de %1.").arg(bal);
    }
    // Revenues / income
    else if ((m.contains("revenu") || m.contains("income") || m.contains("entrées") || m.contains("entrees")) && m_mainWindow)
    {
        double inc = m_mainWindow->getTotalIncome();
        reply = QString("Le total des revenus est de %1.").arg(inc);
    }
    // Expenses
    else if ((m.contains("dépense") || m.contains("depense") || m.contains("expenses") || m.contains("sorties")) && m_mainWindow)
    {
        double exp = m_mainWindow->getTotalExpenses();
        reply = QString("Le total des dépenses est de %1.").arg(exp);
    }

    // Help about how to use the app
    else if (m.contains("aide") || m.contains("help") || m.contains("comment ça marche") || m.contains("comment ca marche"))
    {
        reply =
            "Je peux t'aider avec l'application de gestion financière :\n"
            "- \"comment ajouter un client\"\n"
            "- \"comment ajouter une transaction\"\n"
            "- \"que montre le dashboard\" ou \"bilan\"\n"
            "- \"qr code\" pour savoir à quoi il sert.\n"
            "Pose-moi ta question 😉";
    }
    else if ((m.contains("ajouter") && m.contains("client")) || m.contains("nouveau client"))
    {
        reply =
            "Pour ajouter un client :\n"
            "1. Clique sur le bouton \"Ajouter Client\".\n"
            "2. Remplis les informations du client.\n"
            "3. Clique sur enregistrer.\n"
            "Le client sera ensuite disponible pour les transactions.";
    }
    else if (m.contains("ajouter transaction") || (m.contains("transaction") && m.contains("ajouter")))
    {
        reply =
            "Pour ajouter une transaction :\n"
            "1. Ouvre la fenêtre des transactions.\n"
            "2. Choisis le client, le montant, la catégorie et la date.\n"
            "3. Enregistre.\n"
            "Elle sera prise en compte dans le bilan et le dashboard.";
    }
    else if (m.contains("dashboard") || m.contains("bilan") || m.contains("statistiques") || m.contains("stats"))
    {
        reply =
            "Le dashboard/bilan montre :\n"
            "- Total des revenus et dépenses\n"
            "- Solde actuel\n"
            "- Graphiques par période/catégorie.\n"
            "Tu peux l'utiliser pour suivre rapidement la situation financière.";
    }
    else if (m.contains("qr") || m.contains("code qr") || m.contains("qr code"))
    {
        reply =
            "Le QR code permet d'ouvrir rapidement le dashboard/bilan sur un autre appareil.\n"
            "Scanne-le avec ton téléphone pour afficher la page liée.";
    }

    // ---------- FUN STUFF (jokes, trivia, small talk) ----------

    else if (m.contains("bonjour") || m.contains("salut") || m.contains("hello"))
    {
        reply = "Salut ! Je suis le chatbot de l'application de gestion financière. En quoi je peux t'aider ?";
    }
    else if (m.contains("ça va") || m.contains("ca va") || m.contains("comment ça va"))
    {
        reply = "Je vais bien, merci ! Et toi ? 🙂";
    }
    else if (m.contains("bien") || m.contains("super"))
    {
        reply = "C'est génial à entendre !";
    }
    else if (m.contains("mal") || m.contains("pas bien") || m.contains("triste"))
    {
        reply = "Je suis désolé d'entendre ça. Si tu veux, on peut parler finances, blagues ou trivia pour te changer les idées 💬";
    }
    else if (m.contains("heureux") || m.contains("content"))
    {
        reply = "Parfait, on garde cette bonne énergie ! 💪";
    }

    // Random joke (now uses getRandomJoke)
    else if (m.contains("blague") || m.contains("joke"))
    {
        reply = getRandomJoke();
    }

    // Trivia / culture générale
    else if (m.contains("trivia") || m.contains("question de culture") || m.contains("quiz") || m.contains("quizz"))
    {
        fetchTriviaQuestion("https://opentdb.com/api.php?amount=1&type=multiple");
        reply = "Voici une question de culture générale :";  // la question détaillée sera ajoutée par fetchTriviaQuestion
    }

    else if (m.contains("merci"))
    {
        reply = "Avec plaisir ! Si tu as d'autres questions, je suis là.";
    }
    else
    {
        reply = "Désolé, je n'ai pas compris. Essaie par exemple : \"total\", \"revenus\", \"dépenses\", \"bilan\" ou \"blague\" 🙂";
    }

    return reply;
}


QString chat::getRandomJoke()
{
    QStringList jokes;
    jokes << "Pourquoi les poissons détestent-ils les ordinateurs ? Parce qu'ils ont peur du net !"
          << "Quel est le comble pour un électricien ? De prendre une décharge en se levant !"
          << "Comment appelle-t-on un chien magicien ? Un labracadabrador !"
          << "Qu'est-ce qu'un chat dit lorsqu'il est surpris ? Miaou-ment !"
          << "Pourquoi les plongeurs plongent-ils toujours en arrière et jamais en avant ? Parce que sinon ils tombent dans le bateau !";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, jokes.size() - 1);
    int index = dist(gen);
    return jokes[index];
}

QString chat::decodeHtmlEntities(const QString& text)
{
    QString decodedText = text;
    decodedText.replace("&quot;", "\"");
    decodedText.replace("&amp;", "&");
    decodedText.replace("&lt;", "<");
    decodedText.replace("&gt;", ">");
    decodedText.replace("&#039;", "'");
    return decodedText;
}
