#include <QtWidgets>

#include "discovervestdialog.h"

// Dialog box used when the user wishes to discover vests on the network
DiscoverVestDialog::DiscoverVestDialog(QWidget *parent)
    : QDialog(parent)
{
    // The part of the dialog box shown by default
    vestView = new QListView(this);
    vestView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    vestView->setAlternatingRowColors(true);
    vestView->setStyleSheet("alternate-background-color: lightyellow;");

    findButton = new QPushButton(tr("Discover Vests"));

    closeButton = new QPushButton(tr("Close"));

    moreButton = new QPushButton(tr("More"));
    moreButton->setCheckable(true);
    moreButton->setAutoDefault(false);

    connectButton = new QPushButton(tr("Connect"));
    connectButton->setEnabled(false);

    buttonBox = new QDialogButtonBox(Qt::Vertical);
    buttonBox->addButton(findButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(connectButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(moreButton, QDialogButtonBox::ActionRole);

    // The part shown by clicking the "More" button
    extension = new QWidget;

    ekgCheckBox = new QCheckBox(tr("Show EKG"));
    echoCheckBox = new QCheckBox(tr("Show echocardiogram"));
    rememberCheckBox = new QCheckBox(tr("Remember this vest"));
    ekgCheckBox->setChecked(true);
    echoCheckBox->setChecked(true);
    rememberCheckBox->setChecked(true);

    // Show the extension when the button is clicked
    connect(moreButton, SIGNAL(toggled(bool)), extension, SLOT(setVisible(bool)));

    // Extension area layout
    QVBoxLayout *extensionLayout = new QVBoxLayout;
    extensionLayout->setMargin(0);
    extensionLayout->addWidget(ekgCheckBox);
    extensionLayout->addWidget(echoCheckBox);
    extensionLayout->addWidget(rememberCheckBox);

    extension->setLayout(extensionLayout);

    // Default area layout
    QHBoxLayout *defaultLayout = new QHBoxLayout;
    defaultLayout->addWidget(vestView);
    defaultLayout->addWidget(buttonBox);

    // Overall layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(defaultLayout);
    mainLayout->addWidget(extension);

    setLayout(mainLayout);
    setWindowTitle("Discover Vests");

    // Hide the extension from the user until button is clicked
    extension->hide();

    // Actually performs the discovery
    findVests = new VestFinder(45679, 45678);
    vests = new QStringListModel();
    vestView->setModel(vests);

    // Set up the signals and slots
    connect(findButton, SIGNAL(clicked()), this, SLOT(populateList()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(connectButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(findVests, SIGNAL(vestFound(QString&,quint16,QString&)),
            this, SLOT(vestDiscovered(QString&,quint16,QString&)));
    connect(vestView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(listSelectionChanged(QItemSelection)));
}

DiscoverVestDialog::~DiscoverVestDialog()
{

}

void DiscoverVestDialog::populateList()
{
    vestsList = new QStringList;
    findVests->findVests();
}

void DiscoverVestDialog::vestDiscovered(QString &ip, quint16 port, QString &name)
{
    *vestsList << "Name: " + name + "\nIP: " + ip + "\nPort: " + QString::number(port);
    vests->setStringList(*vestsList);
}

void DiscoverVestDialog::getOptions(QString &ip, QString &name, qint16 &port,
                                    bool &ekg, bool &echo, bool &remember)
{
    if (vestView->currentIndex().isValid())
    {
        QModelIndexList indexList = vestView->selectionModel()->selectedIndexes();
        QModelIndex index = indexList.at(0);

        QString selected = index.data(Qt::DisplayRole).toString();
        QStringList parts = selected.split("\n");

        ip = parts.value(1).split(":").value(1).trimmed();
        name = parts.value(0).split(":").value(1).trimmed();
        port = parts.value(2).split(":").value(1).trimmed().toInt();
        ekg = ekgCheckBox->isChecked();
        echo = echoCheckBox->isChecked();
        remember = rememberCheckBox->isChecked();
    }
    else
    {
        ip = "";
        name = "";
        port = -1;
        ekg = false;
        echo = false;
        remember = false;
    }
}

void DiscoverVestDialog::listSelectionChanged(const QItemSelection &current)
{
    if (current.indexes().isEmpty())
        connectButton->setEnabled(false);
    else
    {
        connectButton->setEnabled(true);
        connectButton->setFocus();
    }
}

