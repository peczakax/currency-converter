import sys
import requests

try:
    from PyQt6.QtWidgets import QApplication, QWidget, QLabel, QComboBox, QLineEdit, QVBoxLayout, QMessageBox
except ImportError:    
    from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QComboBox, QLineEdit, QVBoxLayout, QMessageBox


class CurrencyConverter(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Konwerter Walut')

        # Labels
        self.base_currency_label = QLabel('Waluta bazowa:', self)
        self.target_currency_label = QLabel('Waluta docelowa:', self)
        self.amount_label = QLabel('Kwota:', self)
        self.result_label = QLabel('', self)

        # ComboBoxes
        self.base_currency_input = QComboBox(self)
        self.target_currency_input = QComboBox(self)
        currencies = ["USD", "EUR", "GBP", "PLN", "JPY", "AUD", "CAD", "CHF", "CNY", "SEK"]
        self.base_currency_input.addItems(currencies)
        self.target_currency_input.addItems(currencies)

        # Set default target currency to PLN
        self.target_currency_input.setCurrentText("PLN")

        # LineEdit
        self.amount_input = QLineEdit(self)
        self.amount_input.setPlaceholderText("Wprowadź kwotę")
        self.amount_input.setText("100")  # Set default amount to 100

        # Layout
        layout = QVBoxLayout()
        layout.addWidget(self.base_currency_label)
        layout.addWidget(self.base_currency_input)
        layout.addWidget(self.target_currency_label)
        layout.addWidget(self.target_currency_input)
        layout.addWidget(self.amount_label)
        layout.addWidget(self.amount_input)
        layout.addWidget(self.result_label)

        self.setLayout(layout)

        # Connect signals to convert_currency method
        self.base_currency_input.activated.connect(self.convert_currency)
        self.target_currency_input.activated.connect(self.convert_currency)
        self.amount_input.returnPressed.connect(self.convert_currency)

        # Perform initial conversion
        self.convert_currency()

    def get_exchange_rate(self, base_currency, target_currency):
        url = f"https://api.exchangerate-api.com/v4/latest/{base_currency}"
        try:
            response = requests.get(url)
            response.raise_for_status()  # Raise an error for bad responses
            data = response.json()

            if target_currency not in data['rates']:
                QMessageBox.critical(self, "Błąd", "Nie znaleziono kursu wymiany dla podanej waluty docelowej.")
                return None

            return data['rates'][target_currency]
        
        except requests.exceptions.RequestException as e:
            QMessageBox.critical(self, "Błąd", f"Błąd w połączeniu z API: {e}")
            return None

    def convert_currency(self):
        base_currency = self.base_currency_input.currentText()
        target_currency = self.target_currency_input.currentText()
        try:
            amount = float(self.amount_input.text())
        except ValueError:
            QMessageBox.warning(self, "Błąd", "Proszę wprowadzić prawidłową kwotę.")
            return
        
        exchange_rate = self.get_exchange_rate(base_currency, target_currency)
        
        if exchange_rate is not None:
            converted_amount = amount * exchange_rate
            self.result_label.setText(f"{amount} {base_currency} to {converted_amount:.2f} {target_currency}")

def main():
    app = QApplication(sys.argv)
    converter = CurrencyConverter()
    converter.show()
    sys.exit(app.exec())

if __name__ == '__main__':
    main()