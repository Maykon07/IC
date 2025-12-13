#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStyle>
#include <QIcon>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>
#include <QNetworkRequest>

// ### NOVO - Include para os Pop-ups ###
#include <QMessageBox>
// ### FIM NOVO ###

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &MainWindow::on_reply_finished);

    // --- Ícones ---
    ui->btn_salvar->setIcon(this->style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->btn_girar->setIcon(this->style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->btn_disparar->setIcon(this->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_reiniciar->setIcon(this->style()->standardIcon(QStyle::SP_BrowserReload));
    ui->btn_atualizar->setIcon(this->style()->standardIcon(QStyle::SP_ArrowDown));
    ui->label_config_icon->setPixmap(this->style()->standardIcon(QStyle::SP_ComputerIcon).pixmap(QSize(64, 64)));

    // --- Estilo (QSS) ---
    QString styleSheet = R"(
        /* (O mesmo QSS de antes, sem mudanças) */
        QMainWindow, QWidget {
            background-color: #FFFFFF;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        QTabBar::tab {
            background: #f0f0f0; color: #555; padding: 10px 20px;
            font-size: 14px; font-weight: bold; border: 1px solid #E0E0E0;
            border-bottom: none; border-top-left-radius: 4px;
            border-top-right-radius: 4px; margin-right: 2px;
        }
        QTabBar::tab:selected { background: #0078d4; color: white; }
        QTabWidget::pane { border: 1px solid #E0E0E0; border-top: 1px solid #0078d4; }
        QGroupBox {
            font-size: 14px; font-weight: bold; color: #0078d4;
            border: 1px solid #E0E0E0; border-radius: 5px; margin-top: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin; subcontrol-position: top left;
            padding: 0 5px; left: 10px;
        }
        QPushButton {
            background-color: #0078d4; color: white;
            font-size: 13px; font-weight: bold;
            border: none; border-radius: 4px;
            padding: 10px 15px; margin: 5px;
        }
        QPushButton:hover { background-color: #005a9e; }
        QPushButton:pressed { background-color: #004c8c; }
        QLineEdit {
            border: 1px solid #CCCCCC; border-radius: 4px;
            padding: 5px; font-size: 13px;
        }
        QLabel { font-size: 13px; color: #333333; }
    )";
    this->setStyleSheet(styleSheet);

    // --- Estilizando os Labels de Status ---
    QFont tituloFont = ui->label_status_titulo->font();
    tituloFont.setPointSize(16);
    tituloFont.setBold(true);
    ui->label_status_titulo->setFont(tituloFont);
    ui->label_status_titulo->setStyleSheet("color: #0078d4;");

    QFont msgFont = ui->label_status_msg->font();
    msgFont.setItalic(true);
    ui->label_status_msg->setFont(msgFont);
    ui->label_status_msg->setStyleSheet("color: #555555;");

    // Limpa os campos de status
    ui->label_status_acumulado->setText("");
    ui->label_status_progresso->setText("");
    setStatusMessage("Pronto", "Aguardando comando.");

    // Valores padrão
    ui->lineEdit_ip->setText("192.168.0.104");
    ui->lineEdit_graus->setText("90");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// --- Funções de Status ---
void MainWindow::setStatusMessage(const QString &titulo, const QString &msg, const QString &cor)
{
    ui->label_status_titulo->setText(titulo);
    ui->label_status_msg->setText(msg);
    ui->label_status_titulo->setStyleSheet(QString("color: %1;").arg(cor));
    if (cor != "green") {
        ui->label_status_acumulado->setText("");
        ui->label_status_progresso->setText("");
    }
}

// --- Slots dos Botões (Atualizados com Validação) ---

void MainWindow::on_btn_salvar_clicked()
{
    m_picoIp = ui->lineEdit_ip->text().trimmed(); // .trimmed() remove espaços
    // --- VALIDAÇÃO 1: IP VAZIO ---
    if (m_picoIp.isEmpty()) {
        QMessageBox::warning(this, "Erro de Validação",
                             "O campo 'Endereço IP do Pico W' não pode estar vazio.");
        return;
    }
    // --- VALIDAÇÃO 2: GRAUS INVÁLIDOS ---
    QString grausStr = ui->lineEdit_graus->text();
    bool conversaoOk = false;
    double graus = grausStr.toDouble(&conversaoOk); // Tenta converter

    if (!conversaoOk || graus <= 0) {
        // Se a conversão falhou (ex: "abc") OU se o número é zero ou negativo
        QMessageBox::warning(this, "Erro de Validação",
                             "O campo 'Graus por Passo' deve ser um número positivo (ex: 1.8, 45, 90).");
        return;
    }
    QJsonObject jsonBody;
    jsonBody["graus"] = graus;
    QJsonDocument jsonDoc(jsonBody);
    QUrl url("http://" + m_picoIp + "/salvar_config");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    setStatusMessage("Enviando...", QString("POST %1...").arg(url.toString()), "black");
    m_networkManager->post(request, jsonDoc.toJson());
}

void MainWindow::on_btn_girar_clicked()
{
    sendGetRequest("/girar");
}

void MainWindow::on_btn_disparar_clicked()
{
    sendGetRequest("/disparar");
}

void MainWindow::on_btn_reiniciar_clicked()
{
    sendGetRequest("/reiniciar");
}

void MainWindow::on_btn_atualizar_clicked()
{
    sendGetRequest("/status");
}

// --- Funções Helper ---

void MainWindow::sendGetRequest(const QString &path)
{
    // Validação do IP antes de enviar
    if (m_picoIp.isEmpty()) {
        QMessageBox::warning(this, "Erro de Configuração",
                             "IP do Pico não configurado. Salve na aba 'Configuração' primeiro.");
        return;
    }

    QUrl url("http://" + m_picoIp + path);
    setStatusMessage("Enviando...", QString("GET %1...").arg(url.toString()), "black");
    m_networkManager->get(QNetworkRequest(url));
}

// --- Processamento da Resposta (Atualizado com Pop-ups) ---

void MainWindow::on_reply_finished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(responseData);

        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            QString rota = reply->url().path();

            if (rota == "/config") {
                // (Resposta da rota /config)
                setStatusMessage("Configuração do Pico", "Dados recebidos com sucesso.", "green");
                ui->label_status_acumulado->setText(QString("<b>IP:</b> %1").arg(obj["ip"].toString()));
                ui->label_status_progresso->setText(QString("<b>Passos/Rotação:</b> %1 (@ %2 graus)")
                                                        .arg(obj["passos_totais"].toInt())
                                                        .arg(obj["graus"].toDouble()));

            } else {
                // (Resposta das outras rotas: /girar, /salvar_config, etc.)
                bool sucesso = obj["sucesso"].toBool();
                QString titulo = sucesso ? "Sucesso" : "Falha";
                QString cor = sucesso ? "green" : "red";

                setStatusMessage(titulo, obj["mensagem"].toString(), cor);

                ui->label_status_acumulado->setText(QString("<b>Total Acumulado:</b> %1 graus")
                                                        .arg(obj["graus_acumulados"].toDouble()));
                ui->label_status_progresso->setText(QString("<b>Progresso:</b> %1")
                                                        .arg(obj["vezes_giradas"].toString()));

                // --- POP-UP DE CONFIRMAÇÃO (NOVO) ---
                if (rota == "/salvar_config" && sucesso) {
                    QMessageBox::information(this, "Sucesso",
                                             "Configuração salva com sucesso no Pico W!");
                }
            }
        } else {
            setStatusMessage("Erro", "Resposta do Pico não é um JSON válido.", "red");
        }
    } else {
        // --- POP-UP DE ERRO DE REDE (NOVO) ---
        // (Ex: "Connection closed", "Host not found", etc.)
        setStatusMessage("Erro de Rede", reply->errorString(), "red");
        QMessageBox::critical(this, "Erro de Rede",
                              QString("Não foi possível conectar ao Pico W.\n\nVerifique o IP, o Wi-Fi e se o Pico está online.\n\nErro: %1")
                                  .arg(reply->errorString()));
    }

    reply->deleteLater();
}
