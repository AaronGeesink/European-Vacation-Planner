#include "resultswindow.h"
#include "ui_resultswindow.h"

ResultsWindow::ResultsWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ResultsWindow)
{
	ui->setupUi(this);
    ui->distanceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->foodTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

ResultsWindow::~ResultsWindow()
{
	delete ui;
}

void ResultsWindow::setResults(std::vector<City>& loadedCities, int numCities)
{
	// setup for the distance table
	ui->distanceTable->clear();
	ui->distanceTable->setRowCount(numCities);
    ui->distanceTable->setColumnCount(3);

	ui->distanceTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Start"));
	ui->distanceTable->setHorizontalHeaderItem(1, new QTableWidgetItem("End"));
	ui->distanceTable->setHorizontalHeaderItem(2, new QTableWidgetItem("Distance"));
	ui->distanceTable->setSelectionMode(QAbstractItemView::NoSelection);

	// setup for the food table
	ui->foodTable->clear();
	int numFoods = 0;
	std::vector<QString> foodCities;

	for (unsigned int i = 0; i < loadedCities.size(); i++)
	{
		numFoods = numFoods + loadedCities[i].getNumNonZeroFoods();
		if (loadedCities[i].getNumNonZeroFoods() > 0)
			foodCities.push_back(loadedCities[i].getName());
	}

	ui->foodTable->setRowCount(numFoods + foodCities.size() + 2);
	ui->foodTable->setColumnCount(5);

	ui->foodTable->setHorizontalHeaderItem(0, new QTableWidgetItem("City"));
	ui->foodTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Food"));
	ui->foodTable->setHorizontalHeaderItem(2, new QTableWidgetItem("Price"));
	ui->foodTable->setHorizontalHeaderItem(3, new QTableWidgetItem("Quantity"));
	ui->foodTable->setHorizontalHeaderItem(4, new QTableWidgetItem("Total"));

	ui->foodTable->setSelectionMode(QAbstractItemView::NoSelection);

	// Displaying data to the distance table
	QLabel *name;

	std::vector<QString> visited;
	City city = getStartingCity(loadedCities);
	QString current;

	int totalDistance = 0;

	for (int i = 0; i < numCities - 1; i++)
	{
		for (int j = 0; j < visited.size(); j++)
		{
			city.removeCityDistance(visited[j]);
		}

		name = new QLabel();
		name->setText(city.getName());
		ui->distanceTable->setCellWidget(i, 0, name);

		name = new QLabel();
		name->setText(city.getShortestDistance().endCity);
		ui->distanceTable->setCellWidget(i, 1, name);

		name = new QLabel();
		name->setText(QString::number(city.getShortestDistance().distance));
		ui->distanceTable->setCellWidget(i, 2, name);
		totalDistance = totalDistance + city.getShortestDistance().distance;

		visited.push_back(city.getShortestDistance().endCity);
		if (i == 0)
			visited.push_back(city.getName());

		city = getClosestCity(loadedCities, city.getShortestDistance().endCity);
	}

	name = new QLabel();
	name->setText("Total Distance: ");
	ui->distanceTable->setCellWidget(numCities - 1, 1, name);

	name = new QLabel();
	name->setText(QString::number(totalDistance));
	ui->distanceTable->setCellWidget(numCities - 1, 2, name);

	// Displaying data to the food table
	QLabel *label;
	int foodsLoaded = 0;
	double grandTotal = 0;
	std::vector<double> cityTotals;

	for (unsigned int i = 0; i < loadedCities.size(); i++)
	{
		// set the name of the city if a food was purchases there
		if (loadedCities[i].getNumNonZeroFoods() > 0)
		{
			label = new QLabel();
			label->setText(loadedCities[i].getName());
			ui->foodTable->setCellWidget(foodsLoaded, 0, label);

			double cityTotal = 0;

			for (unsigned int j = 0; j < loadedCities[i].getNonZeroFoods().size(); j++)
			{
				// set the name of the food
				label = new QLabel();
				label->setText(loadedCities[i].getNonZeroFoods()[j].getName());
				ui->foodTable->setCellWidget(foodsLoaded, 1, label);

				// set the price for an individual food item
				label = new QLabel();
				label->setText("$" + QString::number(loadedCities[i].getNonZeroFoods()[j].getPrice()));
				ui->foodTable->setCellWidget(foodsLoaded, 2, label);

				// set the quantity purchases
				label = new QLabel();
				label->setText("x" + QString::number(loadedCities[i].getNonZeroFoods()[j].getQuantity()));
				ui->foodTable->setCellWidget(foodsLoaded, 3, label);

				// display total Price for the food items
				label = new QLabel();
				label->setText("$" + QString::number(loadedCities[i].getNonZeroFoods()[j].calculateTotal()));
				ui->foodTable->setCellWidget(foodsLoaded, 4, label);

				grandTotal = grandTotal + loadedCities[i].getNonZeroFoods()[j].calculateTotal();
				cityTotal = cityTotal + loadedCities[i].getNonZeroFoods()[j].calculateTotal();
				foodsLoaded++;
			}
			cityTotals.push_back(cityTotal);
		}
	}

	label = new QLabel();
	label->setText("Price per City:");
	ui->foodTable->setCellWidget(foodsLoaded, 3, label);
	foodsLoaded++;

	for (int i = 0; i < foodCities.size(); i++)
	{
		label = new QLabel();
		label->setText(foodCities[i]  + ":");
		ui->foodTable->setCellWidget(foodsLoaded + i, 3, label);

		label = new QLabel();
		label->setText("$" + QString::number(cityTotals[i]));
		ui->foodTable->setCellWidget(foodsLoaded + i, 4, label);
	}

	label = new QLabel();
	label->setText("Grand Total: ");
	ui->foodTable->setCellWidget(foodsLoaded + foodCities.size(), 3, label);

	label = new QLabel();
	label->setText("$" + QString::number(grandTotal));
	ui->foodTable->setCellWidget(foodsLoaded + foodCities.size(), 4, label);
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
