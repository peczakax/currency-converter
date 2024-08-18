#include <stdexcept>
#include <string>

#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>

#include "CurrencyConverter.h"

double CurrencyConverter::parseExchangeRate(const std::string& jsonResponse, const std::string& targetCurrency) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray::fromStdString(jsonResponse));
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        throw std::runtime_error("Failed to parse JSON response");
    }

    QJsonObject jsonObj = jsonDoc.object();
    if (!jsonObj.contains("rates") || !jsonObj["rates"].isObject()) {
        throw std::runtime_error("Invalid JSON response: missing 'rates' object");
    }

    QJsonObject ratesObj = jsonObj["rates"].toObject();
    if (!ratesObj.contains(QString::fromStdString(targetCurrency))) {
        throw std::runtime_error("Target currency not found in rates");
    }

    QJsonValue rateValue = ratesObj[QString::fromStdString(targetCurrency)];
    if (!rateValue.isDouble()) {
        throw std::runtime_error("Invalid rate value for target currency");
    }

    return rateValue.toDouble();
}


double CurrencyConverter::getExchangeRate(const std::string& baseCurrency, const std::string& targetCurrency) {
    try {
        std::string url = "https://api.exchangerate-api.com/v4/latest/" + baseCurrency;
        std::string response = getHttpResponse(url);
        return parseExchangeRate(response, targetCurrency);
    }
    catch (const std::runtime_error& e) {
        QMessageBox::critical(nullptr, "Error", e.what());
        return -1;
    }
}

CurrencyConverter::CurrencyConverter(QWidget* parent) : QWidget(parent) {
    initUI();
}

void CurrencyConverter::initUI() {
    setWindowTitle("Currency Converter");

    baseCurrencyLabel = new QLabel("Base Currency:", this);
    targetCurrencyLabel = new QLabel("Target Currency:", this);
    amountLabel = new QLabel("Amount:", this);
    resultLabel = new QLabel("", this);

    baseCurrencyInput = new QComboBox(this);
    targetCurrencyInput = new QComboBox(this);
    QStringList currencies = { "USD", "EUR", "GBP", "PLN", "JPY", "AUD", "CAD", "CHF", "CNY", "SEK" };
    baseCurrencyInput->addItems(currencies);
    targetCurrencyInput->addItems(currencies);

    targetCurrencyInput->setCurrentText("PLN");

    amountInput = new QLineEdit(this);
    amountInput->setPlaceholderText("Enter amount");
    amountInput->setText("100");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(baseCurrencyLabel);
    layout->addWidget(baseCurrencyInput);
    layout->addWidget(targetCurrencyLabel);
    layout->addWidget(targetCurrencyInput);
    layout->addWidget(amountLabel);
    layout->addWidget(amountInput);
    layout->addWidget(resultLabel);

    setLayout(layout);

    connect(baseCurrencyInput, &QComboBox::activated, this, &CurrencyConverter::convertCurrency);
    connect(targetCurrencyInput, &QComboBox::activated, this, &CurrencyConverter::convertCurrency);
    connect(amountInput, &QLineEdit::returnPressed, this, &CurrencyConverter::convertCurrency);

    convertCurrency();
}

void CurrencyConverter::convertCurrency() {
    QString baseCurrency = baseCurrencyInput->currentText();
    QString targetCurrency = targetCurrencyInput->currentText();
    bool ok;
    double amount = amountInput->text().toDouble(&ok);

    if (!ok) {
        QMessageBox::warning(this, "Error", "Please enter a valid amount.");
        return;
    }

    double exchangeRate = getExchangeRate(baseCurrency.toStdString(), targetCurrency.toStdString());

    if (exchangeRate > 0) {
        double convertedAmount = amount * exchangeRate;
        resultLabel->setText(QString("%1 %2 is %3 %4")
            .arg(amount)
            .arg(baseCurrency)
            .arg(convertedAmount, 0, 'f', 2)
            .arg(targetCurrency));
    }
}



// #include "moc_CurrencyConverter.cpp"