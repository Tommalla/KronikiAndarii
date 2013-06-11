﻿#include "oknoekwipunek.h"

OknoEkwipunek::OknoEkwipunek(Gracz *gracz, OknoGracza *okno)
{
	this->gracz = gracz;
	this->okno = okno;
	Ekwipunek* ekwipunek = gracz->getEkwipunek();
	QList<Przedmiot*>* plecak = ekwipunek->getPlecak();
	layoutGlowny = new QVBoxLayout(this);
	setWindowTitle("Ekwipunek");

	listaPrzedmiotow = new QListWidget();
	for(int i = 0; i < ekwipunek->getPlecak()->size(); ++i)
	{
		listaPrzedmiotow->addItem(plecak->at(i)->getNazwa());
	}
	opisPrzedmiotu = new QTextBrowser();
	layoutGorny = new QHBoxLayout();
	layoutGorny->addWidget(listaPrzedmiotow);
	layoutGorny->addWidget(opisPrzedmiotu);
	layoutGlowny->addLayout(layoutGorny);

	przyciskMalejMikstury = new QPushButton();
	przyciskMalejMikstury->setIcon(QIcon(":/ikonki/mala_mikstura.png"));
	przyciskMalejMikstury->setText(QString("(") + QString::number(gracz->getEkwipunek()->getMalePoty()) + QString(")"));
	if(gracz->getEkwipunek()->getMalePoty() == 0)
		przyciskMalejMikstury->setEnabled(false);

	przyciskDuzejMikstury = new QPushButton();
	przyciskDuzejMikstury->setIcon(QIcon(":/ikonki/duza_mikstura.png"));
	przyciskDuzejMikstury->setText(QString("(") + QString::number(gracz->getEkwipunek()->getDuzePoty()) + QString(")"));
	if(gracz->getEkwipunek()->getDuzePoty() == 0)
		przyciskDuzejMikstury->setEnabled(false);


	przyciskZaloz = new QPushButton(QString::fromUtf8("Załóż przedmiot"));
	przyciskZaloz->setEnabled(false);
	ok = new QPushButton("Ok");

	layoutDolny = new QHBoxLayout();
	layoutDolny->addWidget(przyciskMalejMikstury);
	layoutDolny->addWidget(przyciskDuzejMikstury);
	layoutDolny->addStretch();
	layoutDolny->addWidget(przyciskZaloz);
	layoutDolny->addWidget(ok);
	layoutGlowny->addLayout(layoutDolny);

	connect(listaPrzedmiotow, SIGNAL(clicked(QModelIndex)), this, SLOT(wyswietlOpis(QModelIndex)));

	connect(przyciskMalejMikstury, SIGNAL(clicked()), this, SLOT(uzyjMalejMikstury()));
	connect(przyciskDuzejMikstury, SIGNAL(clicked()), this, SLOT(uzyjDuzejMikstury()));
	connect(ok, SIGNAL(clicked()), this, SLOT(close()));
	connect(przyciskZaloz, SIGNAL(clicked()), this, SLOT(zaloz()));
}

void OknoEkwipunek::uzyjDuzejMikstury()
{
	gracz->getEkwipunek()->setDuzePoty(gracz->getEkwipunek()->getDuzePoty() - 1);
	przyciskDuzejMikstury->setText(QString("(") + QString::number(gracz->getEkwipunek()->getDuzePoty()) + QString(")"));
	if(gracz->getEkwipunek()->getDuzePoty() == 0)
		przyciskDuzejMikstury->setEnabled(false);
	gracz->setZdrowieAktualne(qMin(gracz->getZdrowieMaks(), (quint8)(gracz->getZdrowieAktualne() + DZIALANIE_DUZYCH_POTOW)));
}

void OknoEkwipunek::uzyjMalejMikstury()
{
	gracz->getEkwipunek()->setMalePoty(gracz->getEkwipunek()->getMalePoty() - 1);
	przyciskMalejMikstury->setText(QString("(") + QString::number(gracz->getEkwipunek()->getMalePoty()) + QString(")"));
	if(gracz->getEkwipunek()->getMalePoty() == 0)
		przyciskMalejMikstury->setEnabled(false);
	gracz->setZdrowieAktualne(qMin(gracz->getZdrowieMaks(), (quint8)(gracz->getZdrowieAktualne() + DZIALANIE_MALYCH_POTOW)));
}

void OknoEkwipunek::wyswietlOpis(QModelIndex element)
{
	Przedmiot* rzecz = gracz->getEkwipunek()->getPlecak()->at(element.row());
	opisPrzedmiotu->setText(wygenerujOpis(rzecz, gracz));
	if(czyDozwolony(rzecz, gracz))
		przyciskZaloz->setEnabled(true);
	else
		przyciskZaloz->setEnabled(false);
}

void OknoEkwipunek::zaloz()
{
	Przedmiot* rzecz = gracz->getEkwipunek()->getPlecak()->at(listaPrzedmiotow->currentRow());
	if (!czyZalozony(rzecz, gracz))
	{
		zalozPrzedmiot(rzecz, gracz);
		przyciskZaloz->setEnabled(false);
	}
}
