import sys
from PyQt5 import QtWidgets, QtCore


class ViewTree(QtWidgets.QWidget):
    def __init__(self, value):
        super().__init__()

        # refresh timer
        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.refresh)
        self.timer.start(1000)

    def refresh(self):
        dataDict['temp']['a'] += 1
        # dataWidget.updateDict(dataDict)


dataDict = {'temp': {'a': 1}}


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    treeWidget = ViewTree(dataDict)
    treeWidget.show()
    sys.exit(app.exec_())
