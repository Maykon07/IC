/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *lineEdit_ip;
    QLabel *label_2;
    QLineEdit *lineEdit_graus;
    QLabel *label_config_icon;
    QPushButton *btn_salvar;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_status_progresso;
    QLabel *label_status_acumulado;
    QLabel *label_status_msg;
    QLabel *label_status_titulo;
    QFrame *frame;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout;
    QPushButton *btn_atualizar;
    QPushButton *btn_girar;
    QPushButton *btn_disparar;
    QPushButton *btn_reiniciar;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(602, 692);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(0, 10, 561, 611));
        tab = new QWidget();
        tab->setObjectName("tab");
        verticalLayout_3 = new QVBoxLayout(tab);
        verticalLayout_3->setObjectName("verticalLayout_3");
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName("groupBox");
        formLayout = new QFormLayout(groupBox);
        formLayout->setObjectName("formLayout");
        label = new QLabel(groupBox);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label);

        lineEdit_ip = new QLineEdit(groupBox);
        lineEdit_ip->setObjectName("lineEdit_ip");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, lineEdit_ip);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_2);

        lineEdit_graus = new QLineEdit(groupBox);
        lineEdit_graus->setObjectName("lineEdit_graus");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, lineEdit_graus);


        verticalLayout_3->addWidget(groupBox);

        label_config_icon = new QLabel(tab);
        label_config_icon->setObjectName("label_config_icon");
        label_config_icon->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_3->addWidget(label_config_icon);

        btn_salvar = new QPushButton(tab);
        btn_salvar->setObjectName("btn_salvar");

        verticalLayout_3->addWidget(btn_salvar);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        verticalLayout_2 = new QVBoxLayout(tab_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        groupBox_3 = new QGroupBox(tab_2);
        groupBox_3->setObjectName("groupBox_3");
        verticalLayout_4 = new QVBoxLayout(groupBox_3);
        verticalLayout_4->setObjectName("verticalLayout_4");
        label_status_progresso = new QLabel(groupBox_3);
        label_status_progresso->setObjectName("label_status_progresso");

        verticalLayout_4->addWidget(label_status_progresso);

        label_status_acumulado = new QLabel(groupBox_3);
        label_status_acumulado->setObjectName("label_status_acumulado");

        verticalLayout_4->addWidget(label_status_acumulado);

        label_status_msg = new QLabel(groupBox_3);
        label_status_msg->setObjectName("label_status_msg");

        verticalLayout_4->addWidget(label_status_msg);

        label_status_titulo = new QLabel(groupBox_3);
        label_status_titulo->setObjectName("label_status_titulo");

        verticalLayout_4->addWidget(label_status_titulo);

        frame = new QFrame(groupBox_3);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::Shape::HLine);
        frame->setFrameShadow(QFrame::Shadow::Raised);

        verticalLayout_4->addWidget(frame);


        verticalLayout_2->addWidget(groupBox_3);

        groupBox_2 = new QGroupBox(tab_2);
        groupBox_2->setObjectName("groupBox_2");
        verticalLayout = new QVBoxLayout(groupBox_2);
        verticalLayout->setObjectName("verticalLayout");
        btn_atualizar = new QPushButton(groupBox_2);
        btn_atualizar->setObjectName("btn_atualizar");

        verticalLayout->addWidget(btn_atualizar);

        btn_girar = new QPushButton(groupBox_2);
        btn_girar->setObjectName("btn_girar");

        verticalLayout->addWidget(btn_girar);

        btn_disparar = new QPushButton(groupBox_2);
        btn_disparar->setObjectName("btn_disparar");

        verticalLayout->addWidget(btn_disparar);

        btn_reiniciar = new QPushButton(groupBox_2);
        btn_reiniciar->setObjectName("btn_reiniciar");

        verticalLayout->addWidget(btn_reiniciar);


        verticalLayout_2->addWidget(groupBox_2);

        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 602, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Conex\303\243o e Par\303\242metros", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Endere\303\247o IP do Pico W:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Graus por Passo:", nullptr));
        label_config_icon->setText(QString());
        btn_salvar->setText(QCoreApplication::translate("MainWindow", "Salvar Configura\303\247\303\243o", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Configura\303\247\303\243o", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "Status do Experimento", nullptr));
        label_status_progresso->setText(QCoreApplication::translate("MainWindow", "Progresso: N/A", nullptr));
        label_status_acumulado->setText(QCoreApplication::translate("MainWindow", "Total Acumulado: 0.00 graus", nullptr));
        label_status_msg->setText(QCoreApplication::translate("MainWindow", "Aguardando comando...", nullptr));
        label_status_titulo->setText(QCoreApplication::translate("MainWindow", "Pronto", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Controles", nullptr));
        btn_atualizar->setText(QCoreApplication::translate("MainWindow", "Atualizar Status", nullptr));
        btn_girar->setText(QCoreApplication::translate("MainWindow", "Girar Passo", nullptr));
        btn_disparar->setText(QCoreApplication::translate("MainWindow", "Disparar Fonte", nullptr));
        btn_reiniciar->setText(QCoreApplication::translate("MainWindow", "Reiniciar Ciclo", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Opera\303\247\303\243o e Status", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
