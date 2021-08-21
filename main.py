from PySide2.QtWidgets import QApplication
from PySide2.QtCore import QUrl
from PySide2.QtQml import QQmlApplicationEngine
import sys
import PySide2.QtMultimedia

def runApplication():
    app = QApplication(sys.argv)
    engine = QQmlApplicationEngine()
    url = QUrl('script/qml/main.qml')
    engine.load(url)
    return app.exec_()

if __name__ == "__main__":
    runApplication()

