#include <QtWidgets>

#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent)
{
    ekgPortLabel = new QLabel(tr("Default EKG Port:"));
    ekgPortEdit = new QLineEdit(tr("1234"));

    echoPortLabel = new QLabel(tr("Default Echocardiogram Port:"));
    echoPortEdit = new QLineEdit(tr("8081"));

    discoveryListenPortLabel = new QLabel(tr("Discovery Response Port:"));
    discoveryListenPortEdit = new QLineEdit(tr("45679"));

    discoverySendPortLabel = new QLabel(tr("Discovery Send Port:"));
    discoverySendPortEdit = new QLineEdit(tr("45678"));

    ekgDefaultCheckBox = new QCheckBox(tr("EKG Enabled by Default"));
    ekgDefaultCheckBox->setChecked(true);

    echoDefaultCheckBox = new QCheckBox(tr("Echocardiogram Enabled by Default"));
    echoDefaultCheckBox->setChecked(true);

    saveButton = new QPushButton(tr("Save"));

    closeButton = new QPushButton(tr("Close"));
    closeButton->setDefault(true);

    buttonBox = new QDialogButtonBox(Qt::Vertical);
    buttonBox->addButton(saveButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::ActionRole);

    QHBoxLayout *ekgPortLayout = new QHBoxLayout;
    ekgPortLayout->addWidget(ekgPortLabel);
    ekgPortLayout->addWidget(ekgPortEdit);

    QHBoxLayout *echoPortLayout = new QHBoxLayout;
    echoPortLayout->addWidget(echoPortLabel);
    echoPortLayout->addWidget(echoPortEdit);

    QHBoxLayout *listenPortLayout = new QHBoxLayout;
    listenPortLayout->addWidget(discoveryListenPortLabel);
    listenPortLayout->addWidget(discoveryListenPortEdit);

    QHBoxLayout *sendPortLayout = new QHBoxLayout;
    sendPortLayout->addWidget(discoverySendPortLabel);
    sendPortLayout->addWidget(discoverySendPortEdit);

    QVBoxLayout *settingsLayout = new QVBoxLayout;
    settingsLayout->addLayout(ekgPortLayout);
    settingsLayout->addLayout(echoPortLayout);
    settingsLayout->addLayout(listenPortLayout);
    settingsLayout->addLayout(sendPortLayout);
    settingsLayout->addWidget(ekgDefaultCheckBox);
    settingsLayout->addWidget(echoDefaultCheckBox);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(settingsLayout);
    layout->addWidget(buttonBox);

    setLayout(layout);
    setWindowTitle("Settings");

    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));

}

SettingsDialog::~SettingsDialog()
{

}

