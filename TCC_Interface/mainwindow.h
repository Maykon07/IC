#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

// ### NOVO - Includes para os novos widgets ###
#include <QLabel>
#include <QFrame>
// ### FIM NOVO ###

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slots auto-conectados
    void on_btn_salvar_clicked();
    void on_btn_girar_clicked();
    void on_btn_disparar_clicked();
    void on_btn_reiniciar_clicked();
    void on_btn_atualizar_clicked();

    // Slot da resposta
    void on_reply_finished(QNetworkReply *reply);

private:
    // Função helper para enviar requisições GET
    void sendGetRequest(const QString &path);

    // Função para definir o status (substitui logMessage)
    void setStatusMessage(const QString &titulo, const QString &msg, const QString &cor = "black");

    Ui::MainWindow *ui;
    QNetworkAccessManager *m_networkManager;
    QString m_picoIp;
};
#endif // MAINWINDOW_H
