from PySide2.QtWidgets import QApplication
from PySide2.QtCore import QUrl
from PySide2.QtQml import QQmlApplicationEngine, qmlRegisterType
import sys
import PySide2.QtMultimedia

from unittest import mock , TestCase

from subprocess import check_output

def print_contents_of_cwd():
    return check_output(['ls']).split()

class TestExamples(TestCase):
    @mock.patch('main.check_output', return_value = b'foo\nbar\n')
    def test(self, mock_check_output):
        actual_res = print_contents_of_cwd()

        expected_dir = b'tests'
        self.assertIn(expected_dir, actual_res)

def runApplication():
    app = QApplication(sys.argv)
    engine = QQmlApplicationEngine()
    url = QUrl('script/qml/main.qml')
    engine.load(url)
    return app.exec_()

if __name__ == "__main__":
    tests = TestExamples()
    tests.test()
    #runApplication()

