#include "resultswindow.h"
#include "ui_resultswindow.h"

ResultsWindow::ResultsWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ResultsWindow)
{
	ui->setupUi(this);
}

ResultsWindow::~ResultsWindow()
{
	delete ui;
}

void ResultsWindow::setResults(std::vector<City>& loadedCities)
{
	ui->resultsTable->setRowCount(loadedCities.size() - 1);
	ui->resultsTable->setColumnCount(4);

	ui->resultsTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Start"));
	ui->resultsTable->setHorizontalHeaderItem(1, new QTableWidgetItem("End"));
	ui->resultsTable->setHorizontalHeaderItem(2, new QTableWidgetItem("Distance"));
	ui->resultsTable->setHorizontalHeaderItem(3, new QTableWidgetItem("Total Distance"));

	ui->resultsTable->setSelectionMode(QAbstractItemView::NoSelection);

	QLabel *name;

	int i = 0;
	std::vector<QString> visited;
	City city = getStartingCity(loadedCities);
	QString current;

	int totalDistance = 0;

	for (int i = 0; i < loadedCities.size() - 1; i++)
	{
		qDebug() << city.getName();

		for (int i = 0; i < visited.size(); i++)
		{
			city.removeCityDistance(visited[i]);
		}

		name = new QLabel();
		name->setText(city.getName());
		ui->resultsTable->setCellWidget(i, 0, name);

		name = new QLabel();
		name->setText(city.getShortestDistance().endCity);
		ui->resultsTable->setCellWidget(i, 1, name);

		name = new QLabel();
		name->setText(QString::number(city.getShortestDistance().distance));
		ui->resultsTable->setCellWidget(i, 2, name);
		totalDistance = totalDistance + city.getShortestDistance().distance;

		visited.push_back(city.getShortestDistance().endCity);
		if (i == 0)
			visited.push_back(city.getName());

		city = getClosestCity(loadedCities, city.getShortestDistance().endCity);
	}

	name = new QLabel();
	name->setText(QString::number(totalDistance));
	ui->resultsTable->setCellWidget(0, 3, name);
}

City ResultsWindow::getClosestCity(std::vector<City> loadedCities, QString name)
{
	City city = loadedCities[0];
	for (int i = 0; i < loadedCities.size(); i++)
	{
		if (loadedCities[i].getName() == name)
		{
			city = loadedCities[i];
		}
	}
	return city;
}

City ResultsWindow::getStartingCity(std::vector<City> loadedCities)
{
	for (int i = 0; i < loadedCities.size(); i++)
	{
		if (loadedCities[i].getIsStart() == true)
		{
			return loadedCities[i];
		}
	}
	return loadedCities[0];
}

void ResultsWindow::on_moveToMain_clicked()
{
	emit moveToMainClicked();
}

void ResultsWindow::on_moveToFoodSelect_clicked()
{
	emit moveToFoodSelectClicked();
}
